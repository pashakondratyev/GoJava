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
#include "tree.h"

#define DEBUG 0

int switchCount = 0;
int assignCount = 0;



void codeStmt(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount, bool incompleteBlock, STMT *parentPost) {
  terminate_t status = nonterminating;
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
          if(!temp->stmt->terminates){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
            if (!incompleteBlock) fprintf(outputFile, "}");
            return;
          }
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        if (!incompleteBlock) fprintf(outputFile, "}");
        break;
      case sk_exp:
        codeExp(stmt->val.exp, st, it, tabCount);
        fprintf(outputFile, ";\n");
        writeTab(tabCount);
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
          if (typeResolve(temp->exp->type, st)->kind == tk_float) {
            fprintf(outputFile, "System.out.printf(\"%%+7.6e\", ");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          } else {
            fprintf(outputFile, "System.out.print(");
            if (typeResolve(temp->exp->type, st)->kind == tk_rune) fprintf(outputFile, "(int)");
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
          if (typeResolve(temp->exp->type, st)->kind == tk_float) {
            fprintf(outputFile, "System.out.printf(\"%%+7.6e\", ");
            codeExp(temp->exp, st, it, tabCount);
            fprintf(outputFile, ");");
          } else {
            fprintf(outputFile, "System.out.print(");
            if (typeResolve(temp->exp->type, st)->kind == tk_rune) fprintf(outputFile, "(int)");
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
        fprintf(outputFile, "System.out.println();");
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
        st = stmt->val.switchStmt.scope;
        if (stmt->val.switchStmt.simpleStmt != NULL) {
          codeStmt(stmt->val.switchStmt.simpleStmt, st, it, newTabCount, false, parentPost);
          fprintf(outputFile, "\n");
          writeTab(newTabCount);
        }
        fprintf(outputFile, "while (true) {\n");
        writeTab(newTabCount + 1);
        it = scopeIdentifierTable(it);
        
        char *type;
        if (stmt->val.switchStmt.exp != NULL) {
          type = javaTypeString(stmt->val.switchStmt.exp->type, st, NULL);
        } else {
          type = (char *)"Boolean";
        }
        char *condId = (char *)malloc(15);
        sprintf(condId, "switchCond_%d", switchCount++);
        fprintf(outputFile, "%s %s = ", type, condId);
        if (stmt->val.switchStmt.exp != NULL) {
          codeExp(stmt->val.switchStmt.exp, st, it, newTabCount + 1);
        } else {
          fprintf(outputFile, "Boolean.TRUE");
        }
        fprintf(outputFile, ";\n");
        writeTab(newTabCount + 1);

        CASE_CLAUSE *defaultClause = NULL;
        CASE_CLAUSE_LIST *clauseList = stmt->val.switchStmt.caseClauses;
        bool ifStmtUsed = false;
        while (clauseList != NULL) {
          st = clauseList->clause->scope;
          if (clauseList->clause != NULL) {
            if (clauseList->clause->kind == ck_default) {
              defaultClause = clauseList->clause;
            } else {
              if (!ifStmtUsed) {
                ifStmtUsed = true;
                fprintf(outputFile, "if (");
                codeClauseCases(condId, clauseList->clause->val.caseClause.cases, st, it, newTabCount + 1, false,
                                parentPost);
                fprintf(outputFile, ") {\n");
                writeTab(newTabCount + 2);
                it = scopeIdentifierTable(it);
                codeClauses(clauseList->clause->val.caseClause.clauses, st, it, newTabCount + 1, false, parentPost);
                fprintf(outputFile, "\n");
                writeTab(newTabCount + 1);
                fprintf(outputFile, "} ");
              } else {
                fprintf(outputFile, "else if (");
                codeClauseCases(condId, clauseList->clause->val.caseClause.cases, st, it, newTabCount + 1, false,
                                parentPost);
                fprintf(outputFile, ") {\n");
                it = scopeIdentifierTable(it);
                writeTab(newTabCount + 2);
                codeClauses(clauseList->clause->val.caseClause.clauses, st, it, newTabCount + 1, false, parentPost);
                fprintf(outputFile, "\n");
                writeTab(newTabCount + 1);
                fprintf(outputFile, "} ");
              }
            }
          }
          clauseList = clauseList->next;
        }
        if (defaultClause != NULL) {
          if (ifStmtUsed == true) {
            fprintf(outputFile, "else { \n");
          } else {
            fprintf(outputFile, "if (true) { \n");
          }
          writeTab(newTabCount + 2);
          it = scopeIdentifierTable(it);
          codeClauses(defaultClause->val.defaultClauses, st, it, newTabCount + 1, false, parentPost);
          fprintf(outputFile, "\n");
          writeTab(newTabCount + 1);
          fprintf(outputFile, "} \n");
          writeTab(newTabCount);
        } else {
          writeTab(newTabCount);
        }

        if (stmt->val.switchStmt.returnStatus == Undefined) {
          fprintf(stderr, "Logical error! Return status of switch statement should be defined during typechecking\n");
        }

        if (stmt->val.switchStmt.returnStatus != Returns) {
          fprintf(outputFile, "\tbreak;\n");
          writeTab(newTabCount);
        }
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
          if (stmt->val.forStmt.whileExp != NULL)
            codeExp(stmt->val.forStmt.whileExp, stmt->val.forStmt.scope, it, newTabCount);
          else
            fprintf(outputFile, "true");
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
          if (stmt->val.forStmt.forClause->cond != NULL)
            codeExp(stmt->val.forStmt.forClause->cond, stmt->val.forStmt.scope, it, newTabCount);
          else
              fprintf(outputFile, "true");
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
    if(clauses->next != NULL){
      writeTab(tabCount + 1);
    }
    clauses = clauses->next;
  }
}    

void codeAssignment(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount) {
  int curAssignCount = assignCount;
  for (ASSIGN *temp = stmt->val.assign; temp; temp = temp->next) {
    if (temp->lhs->kind == ek_indexExp && 
      temp->lhs->val.indexExp.objectExp->type != NULL &&
      temp->lhs->val.indexExp.objectExp->type->kind == tk_slice) {
      codeExp(temp->lhs->val.indexExp.objectExp, st, it, tabCount);
      fprintf(outputFile, ".put(");
      codeExp(temp->lhs->val.indexExp.indexExp, st, it, tabCount);
      fprintf(outputFile, ", ");
      codeExp(temp->rhs, st, it, tabCount);
      fprintf(outputFile, ");");
    } else if (temp->lhs->kind == ek_id && strcmp(temp->lhs->val.id, "_") == 0) {
      char *type = javaTypeString(temp->rhs->type, st, NULL);
      fprintf(outputFile, "%s %s_%d = ", type, prefix("blank"), blankVar);
      blankVar++;
      codeExp(temp->rhs, st, it, tabCount);
      fprintf(outputFile, ";");
    } else{
      if(temp->lhs->kind == ek_id){
        char *type = javaTypeString(temp->lhs->type, st, NULL);
        fprintf(outputFile, "%s %s_temp_assign_%d", type, prefix(temp->lhs->val.id), assignCount);
        assignCount++;
      } else if(temp->lhs->type->kind == tk_array) {
        char *type = javaTypeString(temp->lhs->type, st, NULL);
        fprintf(outputFile, "%s %s_temp_assign_rhs_%d", type, prefix("array"), assignCount);
        assignCount++;
      } else {
        codeExp(temp->lhs, st, it, tabCount);
      }
      fprintf(outputFile, " = ");
      codeExp(temp->rhs, st, it, tabCount);
      if(temp->rhs->type->kind == tk_array){
        // TODO: currently arrays are being assigned in the second half of this method,
        // Maybe we'll need to move this back
      } else if(temp->rhs->type->kind == tk_struct){
        fprintf(outputFile, ".copy()");
      }
      fprintf(outputFile, ";");

    }
    if (temp->next) {
      fprintf(outputFile, "\n");
      writeTab(tabCount);
    }
  }
  for (ASSIGN *temp = stmt->val.assign; temp; temp = temp->next) {
    // TODO: check if resolves to identifier for cases like 
    // (x),(y) = x,y
    char source[1024];
    char target[1024];
    if(temp->lhs->kind == ek_id && strcmp(temp->lhs->val.id, "_") != 0){
      fprintf(outputFile, "\n");
      IDENTIFIER *i = getFromIdentifierTable(temp->lhs->val.id, it);
      sprintf(target, "%s_%d", prefix(temp->lhs->val.id), i->scopeCount);
      sprintf(source, "%s_temp_assign_%d", prefix(temp->lhs->val.id), curAssignCount);
      curAssignCount++;
      writeTab(tabCount);
      if(temp->lhs->type->kind == tk_array){
        codeCopyArray(target, source, "", temp->lhs->type, st, tabCount);
      } else{
        codeExp(temp->lhs, st, it, tabCount);
        fprintf(outputFile, " = %s;", source);
      }
    } else if(temp->lhs->type!=NULL && temp->lhs->type->kind == tk_array) { //TODO: address empty identifiers
        fprintf(outputFile, "\n");
        sprintf(source, "%s_temp_assign_rhs_%d", prefix("array"), curAssignCount);
        sprintf(target, "%s_temp_assign_lhs_%d", prefix("array"), curAssignCount);
        char *type = javaTypeString(temp->lhs->type, st, NULL);
        writeTab(tabCount);
        fprintf(outputFile, "%s %s = ", type, target);
        codeExp(temp->lhs, st, it, tabCount);
        fprintf(outputFile, ";\n");
        writeTab(tabCount);
        codeCopyArray(target, source, "", temp->lhs->type, st, tabCount);
    } 
  }
}

void codeAssignmentOp(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount) {
  codeExp(stmt->val.assignOp.lhs, st, it, tabCount);
  fprintf(outputFile, " = ");
  EXP *binaryExp = NULL;
  switch (stmt->val.assignOp.kind) {
    case aok_plus:
      binaryExp = makeBinaryExp(ek_plus, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_minus:
      binaryExp = makeBinaryExp(ek_minus, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_times:
      binaryExp = makeBinaryExp(ek_times, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_div:
      binaryExp = makeBinaryExp(ek_div, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_mod:
      binaryExp = makeBinaryExp(ek_mod, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitAnd:
      binaryExp = makeBinaryExp(ek_bitAnd, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitOr:
      binaryExp = makeBinaryExp(ek_bitOr, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitXor:
      binaryExp = makeBinaryExp(ek_bitXor, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitLeftShift:
      binaryExp = makeBinaryExp(ek_bitLeftShift, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitRightShift:
      binaryExp = makeBinaryExp(ek_bitRightShift, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
    case aok_bitClear:
      binaryExp = makeBinaryExp(ek_bitClear, stmt->val.assignOp.lhs, stmt->val.assignOp.rhs, stmt->lineno);
      break;
  }
  
  if (stmt->val.assignOp.lhs == NULL) {
    fprintf(stderr, "Logical error! LHS of assignment op shouldn't be NULL\n" );
  }

  if (typeResolve(stmt->val.assignOp.lhs->type, st)->kind == tk_rune) {
    fprintf(outputFile, "new Integer((int) ");
    codeExp(binaryExp, st, it, tabCount);
    fprintf(outputFile, ")");
  } else {
   codeExp(binaryExp, st, it, tabCount);  
  }
  
  fprintf(outputFile, ";");

}
