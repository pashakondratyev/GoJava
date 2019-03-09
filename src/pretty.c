#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
  int nextTabCount = tabCount == -1 ? -1 : tabCount + 1;
  SHORT_SPECS *s;
  TYPE_SPECS *t;
  VAR_SPECS *v;
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
          prettyPrintType(type, nextTabCount);
        }

        prettyPrintStmt(cur->val.funcDecl->body, 0);
        printf("\n");
        break;
      case dk_short:
        s = cur->val.shortSpecs;
        while(s != NULL){
          prettyPrintExp(s->lhs);
          if(s->next != NULL){
            printf(", ");
          }
          s = s->next;
        }
        printf(" := ");
        s = cur->val.shortSpecs;
        while(s != NULL){
          prettyPrintExp(s->rhs);
          if(s->next != NULL){
            printf(", ");
          }
          s = s->next;
        } 
        if(tabCount == 0) printf("\n");
        break;
      case dk_type:
        printf("type ");
        //Handle if multiple type specs
        t = cur->val.typeSpecs;
        if(t->next != NULL){
          printf("(\n");
          while(t != NULL){
            printTab(nextTabCount);
            printf("%s ", t->name);
            prettyPrintType(t->type, nextTabCount);
            if (t->next != NULL) printf("\n");
            t = t->next;
          }
          printf(")");
        }
        else{
          printf("%s ", t->name);
          prettyPrintType(t->type, tabCount);
        }
        if(tabCount == 0) printf("\n");
        break;
      case dk_var:
        printf("var ");
        if(cur->val.varSpecs->next != NULL){
          printf("(");
          v = cur->val.varSpecs;
          while(v != NULL){
            printf("\n");
            printTab(nextTabCount);
            printf("%s ", v->id);
            if(v->type != NULL){
              prettyPrintType(v->type, nextTabCount);
            }
            if(v->exp != NULL){
              printf(" = ");
              prettyPrintExp(v->exp);
            }
            v=v->next;
          }
          printf("\n)");
        }
        else{
          printf("%s ", cur->val.varSpecs->id);
          if(cur->val.varSpecs->type != NULL){
            prettyPrintType(cur->val.varSpecs->type, nextTabCount);
          }
          if(cur->val.varSpecs->exp != NULL){
            printf("= ");
            prettyPrintExp(cur->val.varSpecs->exp);
          }
        }
        //printf("\n");
        //printf("func %s(", cur->val.varSpecs)
        if(tabCount == 0) printf("\n");
        break;
    }
    cur = cur->next;
  }
}

void prettyPrintExp(EXP *exp) {
  char *c;
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
      //printf("%s", exp->val.stringval);
      c = exp->val.stringval;
      printf("\"");
      for(int i = 1;  c[i+1]!= '\0'; i++){
        getEscapedAsString(c[i]);
      }
      printf("\"");
      break;
    case ek_boolean:
      if (exp->val.booleanval == true) {
        printf("true");
      } else {
        printf("false");
      }
      break;
    case ek_rune:
      printf("'");
      getEscapedAsString(exp->val.runeval);
      printf("'");
      break;
    case ek_plus:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" + ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_minus:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" - ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_times:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" * ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_div:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" / ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_mod:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" %% ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitAnd:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" & ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitOr:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" | ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_eq:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" == ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_ne:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" != ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_ge:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" >= ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_le:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" <= ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_gt:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" > ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_lt:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" < ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_and:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" && ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_or:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" || ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitXor:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" ^ ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitLeftShift:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" << ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitRightShift:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" >> ");
      prettyPrintExp(exp->val.binary.rhs);
      break;
    case ek_bitClear:
      prettyPrintExp(exp->val.binary.lhs);
      printf(" &^ ");
      prettyPrintExp(exp->val.binary.rhs);
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
    case ek_conv:
      prettyPrintType(exp->val.convField.type, 0);
      printf("(");
      prettyPrintExpList(exp->val.convField.args);
      printf(")");
      break;
  }
  return;
}

// Invariant: Each statement must print its own newline
void prettyPrintStmt(STMT *stmt, int tabCount) {
  int newTabCount = tabCount == -1 ? -1 : tabCount + 1;
  STMT_LIST *sl;
  ASSIGN *a;
  FOR_CLAUSE *f;
  STMT *elseStmt;
  CASE_CLAUSE_LIST *clause_list;
  switch (stmt->kind) {
    case sk_block:
      printf("{\n");
      sl = stmt->val.block;
      while(sl != NULL){
        printTab(newTabCount);
        prettyPrintStmt(sl->stmt, newTabCount);
        printf("\n");
        sl = sl->next;
      }
      printTab(tabCount);
      printf("}");
      break;
    case sk_exp:
      prettyPrintExp(stmt->val.exp);
      break;
    case sk_assign:
      a = stmt->val.assign;
      while(a != NULL){
        prettyPrintExp(a->lhs);
        a = a->next;
      }
      printf(" = ");
      a = stmt->val.assign;
      while(a != NULL){
        prettyPrintExp(a->rhs);
        a = a->next;
      }
      break;
    case sk_assignOp:
      prettyPrintExp(stmt->val.assignOp.lhs);
      prettyPrintAssignOp(stmt->val.assignOp.kind);
      prettyPrintExp(stmt->val.assignOp.rhs);
      break;
    case sk_decl:
    case sk_shortDecl:
      prettyPrintDecl(stmt->val.decl, tabCount);
      //printf("\n");
      break;
    case sk_incr:
      prettyPrintExp(stmt->val.exp);
      printf("++");
      break;
    case sk_decr:
      prettyPrintExp(stmt->val.exp);
      printf("--");
      break;
    case sk_print:
      printf("print(");
      prettyPrintExpList(stmt->val.printExps);
      printf(")");
      break;
    case sk_println:
      printf("println(");
      prettyPrintExpList(stmt->val.printExps);
      printf(")");
      break;
    case sk_return:
      printf("return ");
      if(stmt->val.exp != NULL){
        prettyPrintExp(stmt->val.exp);
      }
      break;
    case sk_if:
      printf("if ");
      if(stmt->val.ifStmt.simpleStmt != NULL){
        prettyPrintSimpleStmt(stmt->val.ifStmt.simpleStmt);
        printf("; ");
      }
      prettyPrintExp(stmt->val.ifStmt.cond);
      prettyPrintStmt(stmt->val.ifStmt.body, tabCount);
      // Handle condition where there is an else statment
      if(stmt->val.ifStmt.elseStmt != NULL){
        prettyPrintStmt(stmt->val.ifStmt.elseStmt, tabCount);
      }
      break;
    case sk_else:
      printf(" else ");
      // Check if next is if
      elseStmt = stmt->val.elseBody;
      if(elseStmt->kind == sk_if){
        printf(" if ");
        if(elseStmt->val.ifStmt.simpleStmt != NULL){
          prettyPrintSimpleStmt(elseStmt->val.ifStmt.simpleStmt);
          printf("; ");
        }

        prettyPrintExp(elseStmt->val.ifStmt.cond);
        //Body is a block
        prettyPrintStmt(elseStmt->val.ifStmt.body, tabCount);
      }
      else{
        prettyPrintStmt(elseStmt, tabCount);
      }
      // If this is an else if and has an else/else if statement
      if(elseStmt->kind == sk_if && elseStmt->val.ifStmt.elseStmt != NULL){
        prettyPrintStmt(elseStmt->val.ifStmt.elseStmt, tabCount);
      }
      break;
    case sk_switch:
      printf("switch ");
      if(stmt->val.switchStmt.simpleStmt != NULL){
        prettyPrintSimpleStmt(stmt->val.switchStmt.simpleStmt);
        printf("; ");
      }
      if(stmt->val.switchStmt.exp != NULL){
        prettyPrintExp(stmt->val.switchStmt.exp);
      }
      printf(" {\n");
      clause_list = stmt->val.switchStmt.caseClauses;
      while(clause_list != NULL){
        prettyPrintCase(clause_list->clause, newTabCount);
        clause_list=clause_list->next;
      }
      printf("\n");
      printTab(tabCount);
      printf("}");
      break;
    case sk_for:
      printf("for ");
      if(stmt->val.forStmt.whileExp != NULL){
        prettyPrintExp(stmt->val.forStmt.whileExp);
      }
      else if(stmt->val.forStmt.forClause != NULL){
        f = stmt->val.forStmt.forClause;
        prettyPrintSimpleStmt(f->init);
        printf("; ");
        prettyPrintExp(f->cond);
        printf("; ");
        prettyPrintSimpleStmt(f->post);
      }
      prettyPrintStmt(stmt->val.forStmt.body, tabCount);
      break;
    case sk_break:
      printf("break");
      break;
    case sk_continue:
      printf("continue");
      break;
    case sk_fallthrough:
      printf("fallthrough");
      break;
    case sk_empty:
      break;
  }
  return;
}

// Subset of statrments, doesn't prince newlines
void prettyPrintSimpleStmt(STMT *stmt){
  ASSIGN *a;
  switch(stmt->kind){
    case sk_exp:
      prettyPrintExp(stmt->val.exp);
      break;
    case sk_decr:
      prettyPrintExp(stmt->val.exp);
      printf("--");
      break;
    case sk_incr:
      prettyPrintExp(stmt->val.exp);
      printf("++");
      break;
    case sk_assign:
      a = stmt->val.assign;
      while(a != NULL){
        prettyPrintExp(a->lhs);
        a = a->next;
      }
      printf(" = ");
      a = stmt->val.assign;
      while(a != NULL){
        prettyPrintExp(a->rhs);
        a = a->next;
      }
      break;
    case sk_assignOp:
      prettyPrintExp(stmt->val.assignOp.lhs);
      prettyPrintAssignOp(stmt->val.assignOp.kind);
      prettyPrintExp(stmt->val.assignOp.rhs);
      break;
    case sk_decl:
    case sk_shortDecl:
      //Short Declarations have a 0 indentation so that we can 
      prettyPrintDecl(stmt->val.decl, -1);
      break;
    default:
      return;
  }
}

void prettyPrintCase(CASE_CLAUSE *c, int tabCount){
  int newTabCount = tabCount == -1 ? -1 : tabCount + 1;
  STMT_LIST *sl;
  switch(c->kind){
    case ck_case:
      printTab(tabCount);
      printf("case ");
      if(c->val.caseClause.cases != NULL){
        prettyPrintExpList(c->val.caseClause.cases);
        printf(":\n");
      }
      sl = c->val.caseClause.clauses;
      while(sl != NULL){
        printTab(newTabCount);
        prettyPrintStmt(sl->stmt, newTabCount);
        sl=sl->next;
      }
      break;
    case ck_default:
      printTab(tabCount);
      printf("default:\n");
      sl = c->val.defaultClauses;
      while(sl != NULL){
        printTab(tabCount);
        prettyPrintStmt(sl->stmt, newTabCount);
        sl=sl->next;
      }
      printf("\n");
      break;
  }
}

void prettyPrintType(TYPE *type, int tabCount){
  int nextTabCount = tabCount == -1 ? -1 : tabCount + 1;
  FIELD_DECLS *d;
  switch(type->kind){
    case tk_array:
      printf("[");
      printf("%d", type->val.array.size);
      printf("]");
      prettyPrintType(type->val.array.elemType, tabCount);
      break;
   case tk_slice:
      printf("[]%s", type->val.sliceType->val.name);
      break;
    case tk_struct:
      printf("struct {\n");
      d = type->val.structFields;
      while(d != NULL){
        printTab(nextTabCount);
        printf("%s ", d->id);
        prettyPrintType(d->type, nextTabCount);
        if(d->next != NULL) printf("\n");
        d = d->next;
      }
      printf("\n");
      printTab(tabCount);
      printf("}");
      break;
    default:
      printf("%s", type->val.name);
  }
}

// Should only be called if guaranteed stmt is 
void prettyPrintAssignOp(AssignOpKind op){
  switch(op){
    case aok_plus:
      printf(" += ");
      break;
    case aok_minus:
      printf(" -= ");
      break;
    case aok_times:
      printf(" *= ");
      break;
    case aok_div:
      printf(" /= ");
      break;
    case aok_mod:
      printf(" %%= ");
      break;
    case aok_bitAnd:
      printf(" &= ");
      break;
    case aok_bitOr:
      printf(" |= ");
      break;
    case aok_bitXor:
      printf(" ^= ");
      break;
    case aok_bitLeftShift:
      printf(" <<= ");
      break;
    case aok_bitRightShift:
      printf(" >>= ");
      break;
    case aok_bitClear:
      printf(" &^= ");
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
    printf("%s ", cur->id);
    if(cur->type != NULL){
      prettyPrintType(cur->type, 0);
    }
    if (cur->next != NULL) {
      printf(", ");
    }
    cur = cur->next;
  }
}

char *getEscapedAsString(char c){
  char *string;
  char strChar[1];
  switch(c){
    case '\a':
      string = "\\a";
      break;
    case '\b':
      string = "\\b";
      break;
    case '\f':
      string = "\\f";
      break;
    case '\n':
      string = "\\n";
      break;
    case '\r':
      string = "\\r";
      break;
    case '\t':
      string = "\\t";
      break;
    case '\v':
      string = "\\v";
      break;
    case '\\':
      string = "\\\\";
      break;
    case '\'':
      string = "\\'";
      break;
    case '\"':
      string = "\\\"";
      break;
    default:
      //strChar[0] = c;
      printf("%c", c);
      return strdup(strChar);
  }
  printf("%s", string);
  return strdup(string);
}
