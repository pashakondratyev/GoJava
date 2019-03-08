#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "weed.h"

void reportError(char *err, int lineno) {
  fprintf(stderr, "Error: (line %d) %s\n", lineno, err);
  exit(1);
}

void weedProgram(PROG *prog) {
  if (prog->root_decl != NULL) {
    weedDeclaration(prog->root_decl);
  }
}

void weedStatement(STMT *stmt) {
  STMT_LIST *cur;
  if(stmt == NULL){
    return;
  }
  switch (stmt->kind) {
    case sk_block:
      cur = stmt->val.block;
      while (cur != NULL) {
        weedStatement(cur->stmt);
        cur = cur->next;
      }
      break;
    case sk_decl:
    case sk_shortDecl:
      weedDeclaration(stmt->val.decl);
      break;
    case sk_exp:
      weedExpStmt(stmt->val.exp);
    default:
      return;
  }
}



void weedDeclaration(DECL *decl) {
  // Checks if declaration list
  if (decl->next != NULL) {
    weedDeclaration(decl->next);
  }
  SHORT_SPECS *shortSpecList;
  switch (decl->kind) {
    case dk_type:
      break;
    case dk_func:
      weedFunction(decl->val.funcDecl, decl->lineno);
      break;
    case dk_var:
      break;
    case dk_short:
      shortSpecList = decl->val.shortSpecs;
      if (!isShortSpecIdentifierType(shortSpecList)) {
        reportError("left hand side of declaration should only contain identifiers", decl->lineno);
      }
  }
}

void weedExpStmt(EXP *exp){
  switch(exp->kind){
    case ek_func:
      return;
    case ek_append:
    case ek_cap:
    case ek_len:
      reportError("built-in function cannot be inside expression statement", exp->lineno);
    default:
      reportError("expression statments must be function calls", exp->lineno);
  }
}

// Checks if all return statements return an expression
int weedBlockReturns(STMT *stmt) {
  // Checks if all paths return
  if (stmt == NULL) {
    return 0;
  }
  // STMT *curr_stmt;
  STMT_LIST *curr_stmt;
  CASE_CLAUSE_LIST *curr_case_clause;
  switch (stmt->kind) {
    case sk_block:
      curr_stmt = stmt->val.block;
      while (curr_stmt != NULL && curr_stmt->stmt != NULL) {
        if (weedBlockReturns(curr_stmt->stmt)) {
          return 1;
        }
        curr_stmt = curr_stmt->next;
      }
      return 0;
    case sk_return:
      return stmt->val.exp != NULL;
    case sk_if:
      if (stmt->val.ifStmt.elseStmt == NULL) {
        return 0;
      }
      return weedBlockReturns(stmt->val.ifStmt.body) && weedBlockReturns(stmt->val.ifStmt.elseStmt);
    case sk_else:
      return weedBlockReturns(stmt->val.elseBody);
    case sk_switch:
      return weedSwitchReturns(stmt->val.switchStmt.caseClauses);
    case sk_exp:
    case sk_assign:
    case sk_assignOp:
    case sk_decl:
    case sk_shortDecl:
    case sk_incr:
    case sk_decr:
    case sk_print:
    case sk_println:
    case sk_for:
    case sk_break:
    case sk_continue:
    case sk_fallthrough:
    case sk_empty:
      return 0;
  }
  return 0;
}

// Checks if switch statement has at most one default case
void weedSwitchDefault(STMT *stmt) {
  int num_defaults = 0;
  CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  if (curr_case_clause != NULL && curr_case_clause->next == NULL) {
    num_defaults = num_defaults + 1;
  }
  while (curr_case_clause != NULL){
    if (curr_case_clause->clause->kind == ck_default){
      num_defaults = num_defaults+1;
    }
    curr_case_clause = curr_case_clause->next;
  }
  if (num_defaults > 1) {
    reportError("Too many default cases in switch statement", stmt->lineno);
  }
}

// Checks if switch statement has no inappropriate continue statement
void weedSwitchBreak(STMT *stmt, int allow_cont) {
  CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  while (curr_case_clause != NULL && curr_case_clause->next != NULL) {
    if (curr_case_clause->clause->kind == ck_default) {
      curr_stmt = curr_case_clause->clause->val.defaultClauses;
    } else {
      curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
    }
    if (curr_stmt != NULL && curr_stmt->next == NULL) {
      weedBreakCont(curr_stmt->stmt, allow_cont, 1);
    } else {
      while (curr_stmt != NULL && curr_stmt->next != NULL) {
        curr_stmt = curr_stmt->next;
        weedBreakCont(curr_stmt->stmt, allow_cont, 1);
      }
    }
    curr_case_clause = curr_case_clause->next;
  }
  if (curr_case_clause != NULL && curr_case_clause->clause != NULL) {
    
    if (curr_case_clause->clause->kind == ck_default) {
      curr_stmt = curr_case_clause->clause->val.defaultClauses;
    } else {
      curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
    }
    while(curr_stmt != NULL){
        weedBreakCont(curr_stmt->stmt, allow_cont, 1);
        curr_stmt = curr_stmt->next; 
    }
    /*if (curr_stmt->next == NULL) {
      weedBreakCont(curr_stmt->stmt, allow_cont, 1);
    } else {
      while (curr_stmt->next != NULL) {
        weedBreakCont(curr_stmt->stmt, allow_cont, 1);
        curr_stmt = curr_stmt->next;
      }
    } */
  }
  weedSwitchDefault(stmt);
}

// Checks if there are no misplaced break or continue statements
void weedBreakCont(STMT *stmt, int allow_cont, int allow_break) {
  STMT_LIST *curr_stmt;
  CASE_CLAUSE_LIST *curr_case_clause = malloc(sizeof(CASE_CLAUSE_LIST));
  CaseKind k;
  CASE_CLAUSE *c;
  switch (stmt->kind) {
    case sk_block:
      curr_stmt = stmt->val.block;
      weedBreakCont(curr_stmt->stmt, allow_cont, allow_break);
      while (curr_stmt->next != NULL) {
        curr_stmt = curr_stmt->next;
        weedBreakCont(curr_stmt->stmt, allow_cont, allow_break);
      }
      break;
    case sk_for:
      weedBreakCont(stmt->val.forStmt.body, 1, 1);
      break;
    case sk_continue:
      if (!allow_cont) {
        reportError("Continue statement outside of loop", stmt->lineno);
      }
      break;
    case sk_break:
      if (!allow_break) {
        reportError("Break statement outside of loop or switch", stmt->lineno);
      }
      break;
    case sk_switch:
      weedSwitchBreak(stmt, allow_cont);
      break;
    case sk_if:
      weedBreakCont(stmt->val.ifStmt.body, allow_cont, allow_break);
      if (stmt->val.ifStmt.elseStmt != NULL) {
        weedBreakCont(stmt->val.ifStmt.elseStmt, allow_cont, allow_break);
      }
      break;
    case sk_else:
      weedBreakCont(stmt->val.elseBody, allow_cont, allow_break);
      break;
    case sk_exp:
    case sk_assign:
    case sk_assignOp:
    case sk_decl:
    case sk_shortDecl:
    case sk_incr:
    case sk_decr:
    case sk_print:
    case sk_println:
    case sk_return:
    case sk_fallthrough:
    case sk_empty:
      break;
  }
}

int weedSwitchReturns(CASE_CLAUSE_LIST *c) {
  CASE_CLAUSE_LIST *clauses = c;
  STMT_LIST *s;
  STMT *curr;
  // printf("valid\n");
  int stmtListHasReturn;
  while (clauses != NULL) {
    if (clauses->clause->val.defaultClauses == NULL && clauses->clause->val.caseClause.clauses == NULL) {
      return 0;
    }
    switch (clauses->clause->kind) {
      case ck_default:
        s = clauses->clause->val.defaultClauses;
        break;
      case ck_case:
        s = clauses->clause->val.caseClause.clauses;
        break;
        // Check if each statement list has a a continue or a return
    }
    stmtListHasReturn = 0;
    while (s != NULL) {
      curr = s->stmt;
      switch (curr->kind) {
        case sk_return:
          // If the statement has a return statement we can break;
          stmtListHasReturn = 1;
          break;
        default:
          stmtListHasReturn = weedBlockReturns(curr);
          break;
      }
      if (stmtListHasReturn) {
        break;
      }
      s = s->next;
    }
    if (!stmtListHasReturn) {
      return 0;
    }
    clauses = clauses->next;
  }
  return stmtListHasReturn;
}

void weedFunction(FUNC_DECL *func_decl, int lineno) {
  // Weed body first
  weedStatement(func_decl->body);
  if (func_decl->returnType != NULL) {
    if (!weedBlockReturns(func_decl->body)) {
      reportError("Function does not have return statement", lineno);
    }
  } else {
    if (weedBlockReturns(func_decl->body)) {
      reportError("Too many arguments to return", lineno);
    }
  }
  weedBreakCont(func_decl->body, 0, 0);
}

int isShortSpecIdentifierType(SHORT_SPECS *shortSpecList) {
  while (shortSpecList != NULL) {
    if (shortSpecList->lhs->kind != ek_id) {
      return 0;
    }
    shortSpecList = shortSpecList->next;
  }
  return 1;
}
