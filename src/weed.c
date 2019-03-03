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
      return weedSwitchDefault(stmt);
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
      return 0;
  }
  return 0;
}

// Checks if switch statement has at most one default case
int weedSwitchDefault(STMT *stmt) {
  int num_defaults = 0;
  CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  if (curr_case_clause->next == NULL){
    num_defaults = num_defaults+1;
  }
  while (curr_case_clause->next != NULL){
    if (curr_case_clause->clause->kind == ck_default){
      num_defaults = num_defaults+1;
    }
    curr_case_clause = curr_case_clause->next;
  }
  if (num_defaults > 1){
    reportError("Too many default cases in switch statement", stmt->lineno);
  }
  return 0;
}

// Checks if switch statement has no inappropriate continue statement
void weedSwitchBreak(STMT *stmt, int allow_cont, int allow_break){
  stmt->val.switchStmt.caseClauses->clause->kind;
  stmt->val.switchStmt.caseClauses->clause->val;
  CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  curr_case_clause->clause->kind;
  curr_stmt = curr_case_clause->clause->val.defaultClauses;
  if (curr_case_clause->clause->kind == ck_default) {
    //curr_case_clause->clause->val.defaultClauses;  
  }
  /*CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  if (curr_case_clause->clause->kind == ck_default){
    curr_case_clause->clause->val;
    //curr_stmt = curr_case_clause->clause->val.defaultClauses;
  } else {
    //curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
  }*/
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
      while (curr_stmt->next != NULL){
        curr_stmt = curr_stmt->next;
        weedBreakCont(curr_stmt->stmt, allow_cont, allow_break);
      }
      break;
    case sk_for:
      weedBreakCont(stmt->val.forStmt.body, 1, 1);
      break;
    case sk_continue:
      if (!allow_cont){
        reportError("Continue statement outside of loop", stmt->lineno);
      }
      break;
    case sk_break:
      if (!allow_break){
        reportError("Break statement outside of loop or switch", stmt->lineno);
      }
      break;
    case sk_switch:
      weedSwitchBreak(stmt, allow_cont, allow_break);
      /*
      if (curr_stmt != NULL) {
        weedBreakCont(curr_stmt->stmt, allow_cont, 1);
        while (curr_stmt->next != NULL){
          curr_stmt = curr_stmt->next;
          weedBreakCont(curr_stmt->stmt, allow_cont, 1);
        }
      }
      while (curr_case_clause->next != NULL){
        curr_case_clause = curr_case_clause->next;
        if (curr_case_clause->clause->kind == ck_default){
          curr_stmt = curr_case_clause->clause->val.defaultClauses;
        } else {
          curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
        }
        if (curr_stmt != NULL) {
          weedBreakCont(curr_stmt->stmt, allow_cont, 1);
          while (curr_stmt->next != NULL){
            curr_stmt = curr_stmt->next;
            weedBreakCont(curr_stmt->stmt, allow_cont, 1);
          }
        }
      }*/
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
    case sk_if:
    case sk_else:
    case sk_fallthrough:
      break;
  }
  return;
}

void weedFunction(FUNC_DECL *func_decl, int lineno) {
  if (func_decl->returnType != NULL) {
    if(!weedBlockReturns(func_decl->body)){
      reportError("Function does not have return statement", lineno);
    }
  }
  weedStatement(func_decl->body);
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
