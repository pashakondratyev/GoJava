#include <stdbool.h>
#include <stdio.h>

#include "tree.h"
#include "pretty.h"

void printTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    printf("\t");
  }
}

void prettyPrint(PROG *prog) {
  prettyPrintPackage(prog->package);
  prettyPrintDecl(prog->root_decl, 0);
}

void prettyPrintPackage(PACKAGE *package) { 
    printf("package %s\n", package->name); 
}

void prettyPrintDecl(DECL *decl, int tabCount) {
  DECL *cur = decl;
  int nextTabCount = tabCount + 1;
  SHORT_SPECS *s;
  while (cur != NULL) {
    switch (cur->kind) {
      case dk_func:
        printf("func %s(", cur->val.funcDecl->name);

        if(cur->val.funcDecl->params != NULL){
          prettyPrintParamList(cur->val.funcDecl->params);
        }

        printf(")");

        if(cur->val.funcDecl->returnType != NULL){
          TYPE *type = cur->val.funcDecl->returnType;
          prettyPrintType(type);
        }

        printf("{");
        prettyPrintStmt(cur->val.funcDecl->body, nextTabCount);
        printf("\n}\n");
        break;
      case dk_short:
        s = cur->val.shortSpecs;
        while(s != NULL){
          prettyPrintExp(s->lhs);
          s = s->next;
        }
        printf(" := ");
        s = cur->val.shortSpecs;
        while(s != NULL){
          prettyPrintExp(s->rhs);
          s = s->next;
        } 
        break;
      case dk_type:
        //printf("func %s(", decl->val.typeSpecs);
        break;
      case dk_var:
        //printf("func %s(", decl->val.varSpecs)
        break;
    }
    cur = cur->next;
  }
}

void prettyPrintExp(EXP *exp) {
  switch (exp->kind) {
    case ek_id:
      printf("%s", exp->val.id);
      break;
    case ek_float:
      printf("%f", exp->val.floatval);
      break;
    case ek_int:
      printf("%d", exp->val.intval);
      break;
    case ek_string:
      printf("%s", exp->val.stringval);
      break;
    case ek_boolean:
      if (exp->val.booleanval == true) {
        printf("true");
      } else {
        printf("false");
      }
    case ek_rune:
      printf("%c", exp->val.runeval);
      break;
    case ek_plus:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" + ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_minus:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" - ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_times:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" * ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_div:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" / ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_mod:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" %% ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitAnd:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" & ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitOr:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" | ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_eq:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" += ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_ne:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" != ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_ge:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" >= ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_le:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" <= ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_gt:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" > ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_lt:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" > ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_and:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" && ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_or:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" || ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitXor:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" ^ ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitLeftShift:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" << ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitRightShift:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" >> ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_bitClear:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" &^ ");
      prettyPrintExp(exp->val.binary.lhs);
      break;
    case ek_uplus:
      printf("+");
      prettyPrintExp(exp->val.unary.exp);
      break;
    case ek_uminus:
      printf("-");
      prettyPrintExp(exp->val.unary.exp);
      break;
    case ek_bang:
      printf("!");
      prettyPrintExp(exp->val.unary.exp);
      break;
    case ek_ubitXor:
      printf("^");
      prettyPrintExp(exp->val.unary.exp);
      break;
    case ek_func:
      printf("%s(", exp->val.funcCall.funcId);
      prettyPrintExpList(exp->val.funcCall.args);
      printf(")");
      break;
    case ek_append:
      printf("append(");
      prettyPrintExp(exp->val.append.sliceExp);
      printf(",");
      prettyPrintExp(exp->val.append.elem);
      printf(")");
      break;
    case ek_len:
      printf("len(");
      prettyPrintExp(exp->val.lenExp);
      printf(")");
      break;
    case ek_cap:
      printf("cap(");
      prettyPrintExp(exp->val.capExp);
      printf(")");
      break;
    case ek_indexExp:
      prettyPrintExp(exp->val.indexExp.objectExp);
      printf("[");
      prettyPrintExp(exp->val.indexExp.indexExp);
      printf("]");
      break;
    case ek_structField:
      printf("%s.", exp->val.structField.fieldName);
      prettyPrintExp(exp->val.structField.structExp);
      break;
    case ek_paren:
      printf("(");
      prettyPrintExp(exp->val.parenExp);
      printf(")");
      break;
  }
  return;
}

void prettyPrintStmt(STMT *stmt, int tabCount) {
  int newTabCount = tabCount++;
  switch (stmt->kind) {
    case sk_block:
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
    case sk_switch:
    case sk_for:
    case sk_break:
    case sk_continue:
    case sk_fallthrough:
      return;
  }
  return;
}

void prettyPrintType(TYPE *type){
  switch(type->kind){
    case tk_array:
      printf(" []%s", type->val.name);
      break;
    case tk_boolean:
    case tk_float:
    case tk_int:
    case tk_rune:
    case tk_string:
      printf(" %s", type->val.name);
      break;
    case tk_ref:
    case tk_slice:
      printf(" []%s", type->val.sliceType->val.name);
      break;
    case tk_struct:
      printf("%s", type->val.name);
      break;
  }
}

void prettyPrintExpList(EXP_LIST *exp_list) {
  EXP_LIST *cur = exp_list;
  while (cur != NULL) {
    prettyPrintExp(cur->exp);
    if (cur->next != NULL) {
      printf(", ");
    }
    cur = cur->next;
  }
}

void prettyPrintIdList(ID_LIST *id_list) {
  ID_LIST *cur = id_list;
  while (cur != NULL) {
    printf("%s", cur->id);
    if (cur->next != NULL) {
      printf(", ");
    }
    cur = cur->next;
  }
}

void prettyPrintParamList(PARAM_LIST *param_list){
  PARAM_LIST *cur = param_list;
  while(cur != NULL) {
    printf("%s", cur->id);
    if(cur->type != NULL){
      prettyPrintType(cur->type);
    }
    if (cur->next != NULL) {
      printf(", ");
    }
    cur = cur->next;
  }
}