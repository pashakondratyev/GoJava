#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeStatements.h"

#include "code.h"
#include "codeDeclarations.h"
#include "codeExpressions.h"
#include "codeIdentifiers.h"
#include "codeStructs.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

int switchCount = 0;

void codeStmt(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount, bool incompleteBlock, STMT *parentPost) {
  int newTabCount = tabCount == -1 ? -1 : tabCount + 1;
  if (DEBUG) printf("Code Statement: %d\n", stmt->kind);
  if (stmt != NULL) {
    switch (stmt->kind) {
      case sk_block:
        if (DEBUG) printf("Entering block scope\n");
        fprintf(outputFile, "{");
        IdentifierTable *child = scopeIdentifierTable(it);
        for (STMT_LIST *temp = stmt->val.block.blockStatements; temp; temp = temp->next) {
          // We want to completely ignore type declarations
          if (temp->stmt->kind == sk_decl && temp->stmt->val.decl->kind == dk_type) {
            continue;
          }
          fprintf(outputFile, "\n");
          writeTab(newTabCount);
          codeStmt(temp->stmt, stmt->val.block.scope, child, newTabCount, false, parentPost);
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        if (!incompleteBlock) fprintf(outputFile, "}");
        break;
      case sk_exp:
        codeExp(stmt->val.exp, st, it, tabCount);
        break;
      case sk_assign:
        codeAssignment(stmt, st, it, tabCount);
        break;
      case sk_assignOp:
        codeAssignmentOp(stmt, st, it, tabCount);
        break;
      case sk_decl:
      case sk_shortDecl:
        codeDeclarations(stmt->val.decl, st, it, tabCount);
        break;
      case sk_incr:
        codeExp(stmt->val.exp, st, it, tabCount);
        fprintf(outputFile, "++;");
        break;
      case sk_decr:
        codeExp(stmt->val.exp, st, it, tabCount);
        fprintf(outputFile, "--;");
        break;
      case sk_print:
        for (EXP_LIST *temp = stmt->val.printExps; temp; temp = temp->next) {
          if (temp->exp->type->kind == tk_float) {
            fprintf(outputFile, "System.out.printf(\"%%+7.6e\", ");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          } else {
            fprintf(outputFile, "System.out.print(");
            if (temp->exp->type->kind == tk_rune) fprintf(outputFile, "(int)");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          }
          if (temp->next != NULL) {
            fprintf(outputFile, "\n");
            writeTab(tabCount);
          }
        }
        break;
      case sk_println:
        for (EXP_LIST *temp = stmt->val.printExps; temp; temp = temp->next) {
          if (temp->exp->type->kind == tk_float) {
            fprintf(outputFile, "System.out.printf(\"%%+7.6e\", ");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          } else {
            if (temp->exp->type->kind == tk_rune) fprintf(outputFile, "(int)");
            fprintf(outputFile, "System.out.print(");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          }
          if (temp->next != NULL) {
            fprintf(outputFile, "\n");
            writeTab(tabCount);
            fprintf(outputFile, "System.out.print(\" \");\n");
            writeTab(tabCount);
          }
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        fprintf(outputFile, "System.out.println();\n");
        writeTab(tabCount);
        break;
      case sk_return:
        fprintf(outputFile, "return ");
        codeExp(stmt->val.exp, st, it, tabCount);
        fprintf(outputFile, ";");
        break;
      case sk_if:
        fprintf(outputFile, "{\n");
        writeTab(newTabCount);
        it = scopeIdentifierTable(it);
        if (stmt->val.ifStmt.simpleStmt != NULL) {
          codeStmt(stmt->val.ifStmt.simpleStmt, stmt->val.ifStmt.scope, it, newTabCount, false, parentPost);
          fprintf(outputFile, "\n");
          writeTab(newTabCount);
        }
        fprintf(outputFile, "if (");
        codeExp(stmt->val.ifStmt.cond, stmt->val.ifStmt.scope, it, newTabCount);
        fprintf(outputFile, ")");
        codeStmt(stmt->val.ifStmt.body, stmt->val.ifStmt.scope, it, newTabCount + 1, false, parentPost);
        if (stmt->val.ifStmt.elseStmt != NULL) {
          codeStmt(stmt->val.ifStmt.elseStmt, stmt->val.ifStmt.scope, it, newTabCount + 1, false, parentPost);
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        fprintf(outputFile, "}\n");
        writeTab(tabCount);
        break;
      case sk_else:
        fprintf(outputFile, "else");
        codeStmt(stmt->val.elseBody, st, it, newTabCount, false, parentPost);
        writeTab(tabCount);
        break;
      case sk_switch:
        fprintf(outputFile, "{\n");
        writeTab(newTabCount);
        it = scopeIdentifierTable(it);
        if (stmt->val.switchStmt.simpleStmt != NULL) {
          codeStmt(stmt->val.switchStmt.simpleStmt, stmt->val.switchStmt.scope, it, newTabCount, false, parentPost);
          fprintf(outputFile, "\n");
          writeTab(newTabCount);
        }
        fprintf(outputFile, "while (true) {\n");
        writeTab(newTabCount + 1);

        char *type = javaTypeString(stmt->val.switchStmt.exp->type, st, NULL);
        char *condId = (char *)malloc(15);
        sprintf(condId, "switchCond_%d", switchCount++);
        fprintf(outputFile, "%s %s = ", type, condId);
        codeExp(stmt->val.switchStmt.exp, stmt->val.switchStmt.scope, it, newTabCount + 1);
        fprintf(outputFile, ";\n");
        writeTab(newTabCount + 1);

        CASE_CLAUSE *defaultClause = NULL;
        CASE_CLAUSE_LIST *clauseList = stmt->val.switchStmt.caseClauses;
        bool ifStmtUsed = false;
        while (clauseList != NULL) {
          if (clauseList->clause != NULL) {
            if (clauseList->clause->kind == ck_default) {
              defaultClause = clauseList->clause;
            } else {
              if (!ifStmtUsed) {
                fprintf(outputFile, "if (");
                codeClauseCases(condId, clauseList->clause->val.caseClause.cases, st, it, newTabCount + 1, false,
                                parentPost);
                fprintf(outputFile, ") {\n");
                writeTab(newTabCount + 1);
                codeClauses(clauseList->clause->val.caseClause.clauses, st, it, newTabCount + 1, false, parentPost);
                fprintf(outputFile, "} ");
              } else {
                fprintf(outputFile, "else if (");
                codeClauseCases(condId, clauseList->clause->val.caseClause.cases, st, it, newTabCount + 1, false,
                                parentPost);
                fprintf(outputFile, ") {\n");
                writeTab(newTabCount + 1);
                codeClauses(clauseList->clause->val.caseClause.clauses, st, it, newTabCount + 1, false, parentPost);
                fprintf(outputFile, "} ");
              }
            }
          }
          clauseList = clauseList->next;
        }
        if (defaultClause != NULL) {
          fprintf(outputFile, "else { \n");
          writeTab(newTabCount + 1);
          codeClauses(defaultClause->val.defaultClauses, st, it, newTabCount + 1, false, parentPost);
          fprintf(outputFile, "}\n");
          writeTab(newTabCount);
        }

        fprintf(outputFile, "break;\n");
        writeTab(newTabCount);
        fprintf(outputFile, "}\n");  // terminate while(true)
        writeTab(tabCount);
        fprintf(outputFile, "}");
        break;
      case sk_for:
        // infinite loops
        if (stmt->val.forStmt.whileExp == NULL && stmt->val.forStmt.forClause == NULL) {
          fprintf(outputFile, "while (true) ");
          codeStmt(stmt->val.forStmt.body, stmt->val.forStmt.scope, it, newTabCount, false, NULL);
          fprintf(outputFile, "\n");
          writeTab(tabCount);
          break;
        }

        // while loops
        if (stmt->val.forStmt.whileExp != NULL) {
          fprintf(outputFile, "while (");
          codeExp(stmt->val.forStmt.whileExp, stmt->val.forStmt.scope, it, newTabCount);
          fprintf(outputFile, ")");
          codeStmt(stmt->val.forStmt.body, stmt->val.forStmt.scope, it, newTabCount, false, NULL);
          fprintf(outputFile, "\n");
          writeTab(tabCount);
          break;
        }

        // 3-part loops
        if (stmt->val.forStmt.forClause != NULL) {
          fprintf(outputFile, "{\n");
          writeTab(newTabCount);
          it = scopeIdentifierTable(it);
          if (stmt->val.forStmt.forClause->init != NULL) {
            codeStmt(stmt->val.forStmt.forClause->init, stmt->val.forStmt.scope, it, newTabCount, false, NULL);
            fprintf(outputFile, "\n");
            writeTab(newTabCount);
          }
          fprintf(outputFile, "while (");
          codeExp(stmt->val.forStmt.forClause->cond, stmt->val.forStmt.scope, it, newTabCount);
          fprintf(outputFile, ")");
          codeStmt(stmt->val.forStmt.body, stmt->val.forStmt.scope, it, newTabCount, true,
                   stmt->val.forStmt.forClause->post);
          if (stmt->val.forStmt.forClause->post != NULL) {
            codeStmt(stmt->val.forStmt.forClause->post, stmt->val.forStmt.scope, it, newTabCount, false, NULL);
          }
          fprintf(outputFile, "}\n");  // complete body of loop
          writeTab(tabCount);
          fprintf(outputFile, "}");
          break;
        }

        fprintf(stderr, "Logical Failure: for loop not caught by any loop type.\n");
        break;
      case sk_break:
        fprintf(outputFile, "break;");
        break;
      case sk_continue:
        if (parentPost != NULL) {
          codeStmt(parentPost, st, it, newTabCount, false, NULL);
          fprintf(outputFile, "\n");
          writeTab(newTabCount);
        }
        fprintf(outputFile, "continue;");
        break;
      case sk_fallthrough:
        fprintf(stderr, "Error: fallthough not supported.\n");
        break;
      case sk_empty:
        // empty statement
        break;
    }
  }
}

void codeClauseCases(char *switchExpId, EXP_LIST *cases, SymbolTable *st, IdentifierTable *it, int tabCount,
                     bool incompleteBlock, STMT *parentPost) {
  while (cases != NULL) {
    if (cases->exp != NULL) {
      if (cases->exp->type->kind == tk_array) {
        fprintf(outputFile, "Arrays.deepEquals(%s, ", switchExpId);
        codeExp(cases->exp, st, it, tabCount);
        fprintf(outputFile, ")");
      } else {
        fprintf(outputFile, "%s.equals(", switchExpId);
        codeExp(cases->exp, st, it, tabCount);
        fprintf(outputFile, ")");
      }
    }

    cases = cases->next;
    if (cases != NULL) {
      fprintf(outputFile, " || ");
    }
  }
}

void codeClauses(STMT_LIST *clauses, SymbolTable *st, IdentifierTable *it, int tabCount, bool incompleteBlock,
                 STMT *parentPost) {
  while (clauses != NULL) {
    if (clauses->stmt != NULL) {
      codeStmt(clauses->stmt, st, it, tabCount, incompleteBlock, parentPost);
    }
    clauses = clauses->next;
  }
}

void codeAssignment(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount) {
  for (ASSIGN *temp = stmt->val.assign; temp; temp = temp->next) {
    if (temp->lhs->kind == ek_indexExp && 
      temp->lhs->val.indexExp.objectExp->type != NULL &&
      temp->lhs->val.indexExp.objectExp->type->kind == tk_slice) {
      codeExp(temp->lhs->val.indexExp.objectExp, st, it, tabCount);
      fprintf(outputFile, ".add(");
      codeExp(temp->lhs->val.indexExp.indexExp, st, it, tabCount);
      fprintf(outputFile, ", ");
      codeExp(temp->rhs, st, it, tabCount);
      fprintf(outputFile, ");");
    } else {
      codeExp(temp->lhs, st, it, tabCount);
      fprintf(outputFile, " = ");
      codeExp(temp->rhs, st, it, tabCount);
      fprintf(outputFile, ";");
    }
    if (temp->next) {
      fprintf(outputFile, "\n");
      writeTab(tabCount);
    }
  }
}

void codeAssignmentOp(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount) {
  codeExp(stmt->val.assignOp.lhs, st, it, tabCount);
  switch (stmt->val.assignOp.kind) {
    case aok_plus:
      fprintf(outputFile, " += ");
      break;
    case aok_minus:
      fprintf(outputFile, " -= ");
      break;
    case aok_times:
      fprintf(outputFile, " *= ");
      break;
    case aok_div:
      fprintf(outputFile, " /= ");
      break;
    case aok_mod:
      fprintf(outputFile, " %%= ");
      break;
    case aok_bitAnd:
      fprintf(outputFile, " &= ");
      break;
    case aok_bitOr:
      fprintf(outputFile, " |= ");
      break;
    case aok_bitXor:
      fprintf(outputFile, " ^= ");
      break;
    case aok_bitLeftShift:
      fprintf(outputFile, " <<= ");
      break;
    case aok_bitRightShift:
      fprintf(outputFile, " >>= ");
      break;
    case aok_bitClear:
      fprintf(outputFile, " &= ~(");
      codeExp(stmt->val.assignOp.rhs, st, it, tabCount);
      fprintf(outputFile, ");");
      break;
  }
  codeExp(stmt->val.assignOp.rhs, st, it, tabCount);
  fprintf(outputFile, ";");
}
