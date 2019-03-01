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

// Checks if a block is guaranteed to have a return in it
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
      return 1;
    case sk_if:
      if (stmt->val.ifStmt.elseStmt == NULL) {
        return 0;
      }
      return weedBlockReturns(stmt->val.ifStmt.body) && weedBlockReturns(stmt->val.ifStmt.elseStmt);
    case sk_switch:
      return weedSwitchReturn(stmt);
    case sk_exp:
    case sk_assign:
    case sk_assignOp:
    case sk_decl:
    case sk_shortDecl:
    case sk_incr:
    case sk_decr:
    case sk_print:
    case sk_println:
    case sk_else:
    case sk_for:
    case sk_break:
    case sk_continue:
    case sk_fallthrough:
      return 0;
  }
  return 0;
}

// Checks if every path of a switch case has a return statement
int weedSwitchReturn(STMT *stmt) {
  int default_exists = 0;
  int return_exists = 0;
  int final_clause_returns = 0;
  int fallthrough_exists = 0;
  CASE_CLAUSE_LIST *curr_case_clause = stmt->val.switchStmt.caseClauses;
  STMT_LIST *curr_stmt;
  while (curr_case_clause->next != NULL) {
    if (curr_case_clause->clause->kind == ck_default) {
      default_exists = 1;
      curr_stmt = curr_case_clause->clause->val.defaultClauses;
    } else {
      curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
    }
    // Checks if a return exsists in the current stmt list
    if (curr_stmt->next == NULL) {
      return_exists = weedBlockReturns(curr_stmt->stmt);
    } else {
      while (curr_stmt->next != NULL) {
        if (weedBlockReturns(curr_stmt->stmt)) {
          return_exists = 1;
        }
        curr_stmt = curr_stmt->next;
      }
    }
    // Checks if last statement is a fall through
    fallthrough_exists = curr_stmt->stmt->kind == sk_fallthrough;
    if (!fallthrough_exists && !return_exists) {
      return 0;
    }
    curr_case_clause = curr_case_clause->next;
  }
  // Last clause cannot have a fallthrough, but this might have a return
  if (curr_case_clause->clause->kind == ck_default) {
    default_exists = 1;
    curr_stmt = curr_case_clause->clause->val.defaultClauses;
  } else {
    curr_stmt = curr_case_clause->clause->val.caseClause.clauses;
  }
  if (curr_stmt->next == NULL) {
    return_exists = weedBlockReturns(curr_stmt->stmt);
  }
  while (curr_stmt->next != NULL) {
    if (weedBlockReturns(curr_stmt->stmt)) {
      return_exists = 1;
      curr_stmt = curr_stmt->next;
    }
  }
  return final_clause_returns && default_exists;
}

void weedFunction(FUNC_DECL *func_decl, int lineno) {
  if (func_decl->returnType != NULL) {
    if(!weedBlockReturns(func_decl->body)){
      reportError("Function does not have return statement", lineno);
    };
  }
  weedStatement(func_decl->body);
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