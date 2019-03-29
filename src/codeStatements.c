#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeStatements.h"

#include "code.h"
#include "codeExpressions.h"
#include "codeIdentifiers.h"
#include "codeDeclarations.h"
#include "codeStructs.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0 

void codeStmt(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount) {
  // TODO: implement
  int newTabCount = tabCount == -1 ? -1 : tabCount + 1;
  if(DEBUG) printf("Code Statement: %d\n", stmt->kind);
  if (stmt != NULL) {
    switch (stmt->kind) {
      case sk_block:
        if(DEBUG) printf("Entering block scope\n");
        fprintf(outputFile, "{");
        IdentifierTable *child = scopeIdentifierTable(it);
        for(STMT_LIST *temp = stmt->val.block.blockStatements; temp; temp = temp->next){
            //We want to completely ignore type declarations
            if(temp->stmt->kind == sk_decl && temp->stmt->val.decl->kind == dk_type){
              continue;
            }
            fprintf(outputFile, "\n");
            writeTab(newTabCount);
            codeStmt(temp->stmt, stmt->val.block.scope, child, newTabCount);
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        fprintf(outputFile, "}");
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
        // TODO: fix output for doubles
        for(EXP_LIST *temp = stmt->val.printExps; temp; temp=temp->next){
          fprintf(outputFile, "System.out.print(");
          if (temp->exp->type->kind == tk_rune) fprintf(outputFile, "(int)");
          codeExp(temp->exp, st, it, tabCount);
          fprintf(outputFile, ");");
          if(temp->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
          }
        }
        break;
      case sk_println:
        // TODO: fix for multiple items
        // TODO: fix output for doubles
        for(EXP_LIST *temp = stmt->val.printExps; temp; temp=temp->next){ 
          fprintf(outputFile, "System.out.println(");
          if (temp->exp->type->kind == tk_rune) fprintf(outputFile, "(int)");
          codeExp(temp->exp, st, it, tabCount);
          fprintf(outputFile, ");");
          if(temp->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
          }
        }
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
        if(stmt->val.ifStmt.simpleStmt != NULL){
          codeStmt(stmt->val.ifStmt.simpleStmt, stmt->val.ifStmt.scope, it, newTabCount);
          fprintf(outputFile,"\n");
          writeTab(newTabCount);
        }
        fprintf(outputFile, "if (");
        codeExp(stmt->val.ifStmt.cond, stmt->val.ifStmt.scope, it, newTabCount);
        fprintf(outputFile, ")");
        codeStmt(stmt->val.ifStmt.body, stmt->val.ifStmt.scope, it, newTabCount);
        if(stmt->val.ifStmt.elseStmt != NULL){
          codeStmt(stmt->val.ifStmt.elseStmt, stmt->val.ifStmt.scope, it, tabCount);
        }
        fprintf(outputFile, "\n");
        writeTab(tabCount);
        fprintf(outputFile, "}");
        // TODO: complete
        break;
      case sk_else:
        fprintf(outputFile, "else");
        codeStmt(stmt->val.elseBody, st, it, newTabCount);
        writeTab(tabCount);
        break;
      case sk_switch:
        // TODO: complete
        break;
      case sk_for:
        // TODO: complete
        break;
      case sk_break:
        // TODO: fix with control flow
        fprintf(outputFile, "break;");  
        break;
      case sk_continue:
        // TODO: fix with control flow
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

void codeAssignment(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount){
  for(ASSIGN *temp = stmt->val.assign; temp; temp = temp->next){
    codeExp(temp->lhs, st, it, tabCount);
    fprintf(outputFile, " = ");
    codeExp(temp->rhs, st, it, tabCount);
    fprintf(outputFile, ";");
    if(temp->next){
      fprintf(outputFile, "\n");
      writeTab(tabCount);
    }
  }
}

void codeAssignmentOp(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount){
  codeExp(stmt->val.assignOp.lhs, st, it, tabCount);
  switch(stmt->val.assignOp.kind){
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

