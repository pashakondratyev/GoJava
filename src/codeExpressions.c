#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "codeExpressions.h"
#include "codeIdentifiers.h"
#include "codeStructs.h"
#include "codeTypes.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

void codeExp(EXP *exp, SymbolTable *st, IdentifierTable *it, int tabCount) {
  TYPE *type = NULL;
  TYPE *type1 = NULL;
  TYPE *type2 = NULL;
  TYPE *type3 = NULL;
  SYMBOL *s = NULL;
  if (exp != NULL) {
    if(DEBUG) printf("Code Expression Kind : %d\n", exp->kind);
    switch (exp->kind) {
      case ek_id:
        if (strcmp(exp->val.id, "_") == 0) {
          fprintf(outputFile, "%s_%d", prefix("blank"), blankVar);
          break;
        } else if (strcmp(exp->val.id, "true") == 0 && getFromIdentifierTable(exp->val.id, it) == NULL) {
            fprintf(outputFile, "__golite__true");
            break;
        } else if (strcmp(exp->val.id, "false") == 0 && getFromIdentifierTable(exp->val.id, it) == NULL) {
          fprintf(outputFile, "__golite__false");
          break;
        }
        if (getSymbol(st, exp->val.id)->kind != dk_func) {
          IDENTIFIER *i = getFromIdentifierTable(exp->val.id, it);
          if (i == NULL) {
            i = addToIdentifierTable(exp->val.id, 1, it);
          }
          fprintf(outputFile, "%s_%d", prefix(i->identifier), i->scopeCount);
        } else {
          fprintf(outputFile, "%s", prefix(exp->val.id));
        }
        break;
      case ek_float:
        fprintf(outputFile, "new Double(%f)", exp->val.floatval);
        break;
      case ek_int:
        fprintf(outputFile, "new Integer(%i)", exp->val.intval);
        break;
      case ek_string:
        fprintf(outputFile, "%s", exp->val.stringval);
        break;
      case ek_boolean:
        fprintf(outputFile, "new Boolean(%s)", exp->val.booleanval ? "__golite__true" : "__golite__false");
        break;
      case ek_rune:
        fprintf(outputFile, "new Character('%c')", exp->val.runeval);
        break;
      case ek_plus:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " + ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " + ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_minus:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " - ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " - ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_times:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " * ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " * ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_div:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " / ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, " / ");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_eq:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_array) {  // arrays
          fprintf(outputFile, "(Arrays.deepEquals(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, ",");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {  // all other types: int, float64, rune, string, structs
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, ".equals(");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        }
        break;
      case ek_ne:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_array) {  // arrays
          fprintf(outputFile, "(!Arrays.deepEquals(");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, ",");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {  // all other types: int, float64, rune, string, structs
          fprintf(outputFile, "(!");
          codeExp(exp->val.binary.lhs, st, it, tabCount);
          fprintf(outputFile, ".equals(");
          codeExp(exp->val.binary.rhs, st, it, tabCount);
          fprintf(outputFile, "))");
        }
        break;
      case ek_ge:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ") >= 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_le:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ") <= 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_gt:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ") > 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_lt:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ") < 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_and:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " && ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_or:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " || ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_mod:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " %% ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitAnd:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " & ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitOr:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " | ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitXor:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " ^ ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitLeftShift:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " << ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitRightShift:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " >> ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitClear:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, it, tabCount);
        fprintf(outputFile, " & (~ ");
        codeExp(exp->val.binary.rhs, st, it, tabCount);
        fprintf(outputFile, "))");
        break;
      case ek_uplus:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(+");
          codeExp(exp->val.unary.exp, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {  // int, float64
          fprintf(outputFile, "(+");
          codeExp(exp->val.unary.exp, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_uminus:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(-");
          codeExp(exp->val.unary.exp, st, it, tabCount);
          fprintf(outputFile, "))");
        } else {  // int, float64
          fprintf(outputFile, "(-");
          codeExp(exp->val.unary.exp, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_bang:
        fprintf(outputFile, "(!");
        codeExp(exp->val.unary.exp, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_ubitXor:
        fprintf(outputFile, "(~");
        codeExp(exp->val.unary.exp, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_func:
        // type casting checked first
        s = getSymbol(st, exp->val.funcCall.funcId);
        type1 = NULL;
        type2 = NULL;
        type3 = NULL;
        if (s->kind == dk_type) {
          type1 = s->val.type;
          type2 = s->val.typeDecl.type;
          type3 = s->val.typeDecl.resolvesTo;
        } else{
          codeFunctionCall(exp, st, it, tabCount);
          break;
        }
        if (type1->kind == tk_int || type2->kind == tk_int || type3->kind == tk_int) {  // int
          fprintf(outputFile, "castUtil.castToInteger(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_float || type2->kind == tk_float || type3->kind == tk_float) {  // float64
          fprintf(outputFile, "castUtil.castToDouble(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_string || type2->kind == tk_string || type3->kind == tk_string) {  // string
          fprintf(outputFile, "castUtil.castToString(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_rune || type2->kind == tk_rune || type3->kind == tk_rune) {  // rune
          fprintf(outputFile, "castUtil.castToCharacter(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        break;
      case ek_append:
        codeExp(exp->val.append.sliceExp, st, it, tabCount);
        fprintf(outputFile, ".append(");
        codeExp(exp->val.append.elem, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_len:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_string) {  // string
          codeExp(exp->val.lenExp, st, it, tabCount);
          fprintf(outputFile, ".length()");
        } else if (type->kind == tk_array) {  // array
          codeExp(exp->val.lenExp, st, it, tabCount);
          fprintf(outputFile, ".length");
        } else {  // slice
          codeExp(exp->val.lenExp, st, it, tabCount);
          fprintf(outputFile, ".len");
        }
        break;
      case ek_cap:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
          codeExp(exp->val.capExp, st, it, tabCount);
          fprintf(outputFile, ".length");
        } else {  // slice
          codeExp(exp->val.capExp, st, it, tabCount);
          fprintf(outputFile, ".cap");
        }
        break;
      case ek_indexExp:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
          codeExp(exp->val.indexExp.objectExp, st, it, tabCount);
          fprintf(outputFile, "[");
          codeExp(exp->val.indexExp.indexExp, st, it, tabCount);
          fprintf(outputFile, "]");
        } else {  // slice
          codeExp(exp->val.indexExp.objectExp, st, it, tabCount);
          fprintf(outputFile, ".get(");
          codeExp(exp->val.indexExp.indexExp, st, it, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_structField:
        codeExp(exp->val.structField.structExp, st, it, tabCount);
        fprintf(outputFile, ".%s", exp->val.structField.fieldName);
        break;
      case ek_paren:
        fprintf(outputFile, "(");
        codeExp(exp->val.parenExp, st, it, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_conv:
        s = getSymbol(st, exp->val.funcCall.funcId);
        type1 = NULL;
        type2 = NULL;
        type3 = NULL;
        if (s->kind == dk_type) {
          type1 = s->val.type;
          type2 = s->val.typeDecl.type;
          type3 = s->val.typeDecl.resolvesTo;
        }
        if (type1->kind == tk_int || type2->kind == tk_int || type3->kind == tk_int) {  // int
          fprintf(outputFile, "castUtil.castToInteger(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_float || type2->kind == tk_float || type3->kind == tk_float) {  // float64
          fprintf(outputFile, "castUtil.castToDouble(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_string || type2->kind == tk_string || type3->kind == tk_string) {  // string
          fprintf(outputFile, "castUtil.castToString(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_rune || type2->kind == tk_rune || type3->kind == tk_rune) {  // rune
          fprintf(outputFile, "castUtil.castToCharacter(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, it, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        break;
    }
  }
}

void codeFunctionCall(EXP *exp, SymbolTable *st, IdentifierTable *it, int tabCount) {
  fprintf(outputFile, "%s(", prefix(exp->val.funcCall.funcId));
  EXP_LIST *exps = exp->val.funcCall.args;
  while (exps != NULL) {
    codeExp(exps->exp, st, it, tabCount);

    exps = exps->next;
    if (exps != NULL) {
      fprintf(outputFile, ", ");
    }
  }
  fprintf(outputFile, ")");
}