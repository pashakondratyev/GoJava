#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "codeStructs.h"
#include "codeTypes.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

void writeTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    fprintf(outputFile, "\t");
  }
}

// prefix all identifiers
char *prefix(char *str) {
  char *prefix = "__golite__";
  char *result = malloc((strlen(str) + strlen(prefix) + 1) * sizeof(char));
  sprintf(result, "%s%s", prefix, str);
  return result;
}

int indexLastForwardSlash(char *str) {
  int index = -1;
  for (int i = 0; i <= strlen(str); i++) {
    if (str[i] == '/') index = i;
  }
  return index;
}

void codeProgram(PROG *prog, SymbolTable *st, char *inputFileName) {
  // create file name and open it
  numInitFunc = 0;
  identifierCount = 0;
  char *outputFileName = malloc((strlen(inputFileName) + 6) * sizeof(char));
  strncpy(outputFileName, inputFileName, strlen(inputFileName));
  sprintf(outputFileName, "%s.java", inputFileName);
  outputFile = fopen(outputFileName, "w");
  free(outputFileName);
  if (outputFile == NULL) {
    fprintf(stderr, "Can't open output file!\n");
    exit(1);
  }

  // ignore package declaration

  // set class name as the file name excluding the path
  int index = indexLastForwardSlash(inputFileName);
  initStructTable();
  // Creates all the structs necessary as java objects that can be reused later
  makeStructTable(prog->root_decl, st);
  // Outputs the created java objects into the outtput file by traversing
  //printStructTable();
  codeStructTable();
  codeSetup(&inputFileName[index + 1]);

  codeDeclarations(prog->root_decl, st, 1);
  codeComplete();
  fclose(outputFile);
}

// setup classes and other defaults
void codeSetup(char *className) {
  // TODO: complete
  // class name must match file name
  fprintf(outputFile, "public class %s {\n", className);
  // define Go boolean variables
  fprintf(outputFile, "\tpublic static Boolean __golite__true = Boolean.TRUE;\n");
  fprintf(outputFile, "\tpublic static Boolean __golite__false = Boolean.FALSE;\n\n");
  // utility class to handle casts/type conversions
  fprintf(outputFile, "\tpublic static Cast castUtil = new Cast();\n");
}

// complete class definition
void codeComplete() {
  // TODO: complete
  fprintf(outputFile, "\tpublic static void main(String[] args) {\n");
  for (int i = 0; i < numInitFunc; i++) {
    fprintf(outputFile, "\t\t__golite__init_%d();\n", i);
  }
  fprintf(outputFile, "\t\t__golite__main();\n");
  fprintf(outputFile, "\t}\n");
  fprintf(outputFile, "}\n");
}

// Declarations

void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount) {
  if (dcl != NULL) {
    switch (dcl->kind) {
      case dk_var:
        codeVarDecl(dcl->val.varSpecs, st, tabCount);
        break;
      case dk_short:
        codeShortDecl(dcl->val.shortSpecs, st, tabCount);
        break;
      case dk_type:
        codeTypeDecl(dcl->val.typeSpecs, st, tabCount);
        break;
      case dk_func:
        codeFuncDecl(dcl->val.funcDecl, st, tabCount);
        break;
    }
    codeDeclarations(dcl->next, st, tabCount);
  }
}

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount) {
  // TODO: implement
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount) {
  // TODO: implement
}

void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount) {
  // TODO: implement
}

// Specific Constructs

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
  char BUFFER[1024];
  char methodName[1024];
  // If this is not a reference we need to handle this specially
  if (strcmp(fd->name, "init") == 0){
    fprintf(outputFile, "\tpublic static void %s_%d (", prefix(fd->name), numInitFunc);
    numInitFunc++;
  } else {
    char *returnTypeString = fd->returnType == NULL ? "void" : javaTypeString(fd->returnType, st, NULL);
    fprintf(outputFile, "\tpublic static %s %s (", returnTypeString, prefix(fd->name));
    for (PARAM_LIST *temp = fd->params; temp; temp = temp->next) {
      fprintf(outputFile, "%s %s", javaTypeString(temp->type, st, NULL), temp->id);
      if (temp->next) {
        fprintf(outputFile, ", ");
      }
    }
  }

  // print args
  fprintf(outputFile, ") {\n");
  // TODO: implement
  fprintf(outputFile, "\t}\n");
}

void codeStmt(STMT *stmt, SymbolTable *st, int tabCount) {
  // TODO: implement
  if (stmt != NULL) {
    switch (stmt->kind) {
      case sk_block:
        break;
      case sk_exp:
        break;
      case sk_assign:
        break;
      case sk_assignOp:
        break;
      case sk_decl:
        break;
      case sk_shortDecl:
        break;
      case sk_incr:
        break;
      case sk_decr:
        break;
      case sk_print:
        break;
      case sk_println:
        break;
      case sk_return:
        break;
      case sk_if:
        break;
      case sk_else:
        break;
      case sk_switch:
        break;
      case sk_for:
        break;
      case sk_break:
        break;
      case sk_continue:
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

void codeExp(EXP *exp, SymbolTable *st, int tabCount) {
  TYPE *type = NULL;
  SYMBOL *s = NULL;
  if (exp != NULL) {
    switch (exp->kind) {
      case ek_id:
        if (strcmp(exp->val.id, "_") == 0) {
          fprintf(stderr, "ERROR: blank identifier not handled.\n");
          break;
        }
        fprintf(outputFile, "%s", prefix(exp->val.id));
        break;
      case ek_float:
        fprintf(outputFile, "new Double(%f)", exp->val.floatval);
        break;
      case ek_int:
        fprintf(outputFile, "new Integer(%i)", exp->val.intval);
        break;
      case ek_string:
        fprintf(outputFile, "\"%s\"", exp->val.stringval);
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
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " + ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " + ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_minus:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " - ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " - ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_times:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " * ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " * ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_div:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " / ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, " / ");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_eq:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_array) {  // arrays
          fprintf(outputFile, "(Arrays.deepEquals(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, ",");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {  // all other types: int, float64, rune, string, structs
          fprintf(outputFile, "(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, ".equals(");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        }
        break;
      case ek_ne:
        type = typeResolve(exp->val.binary.lhs->type, st);
        if (type->kind == tk_array) {  // arrays
          fprintf(outputFile, "(!Arrays.deepEquals(");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, ",");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        } else {  // all other types: int, float64, rune, string, structs
          fprintf(outputFile, "(!");
          codeExp(exp->val.binary.lhs, st, tabCount);
          fprintf(outputFile, ".equals(");
          codeExp(exp->val.binary.rhs, st, tabCount);
          fprintf(outputFile, "))");
        }
        break;
      case ek_ge:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ") >= 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_le:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ") <= 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_gt:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ") > 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_lt:
        fprintf(outputFile, "((");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, ".compareTo(");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ") < 0) ? Boolean.TRUE : Boolean.FALSE)");
        break;
      case ek_and:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " && ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_or:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " || ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_mod:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " %% ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitAnd:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " & ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitOr:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " | ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitXor:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " ^ ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitLeftShift:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " << ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitRightShift:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " >> ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_bitClear:
        fprintf(outputFile, "(");
        codeExp(exp->val.binary.lhs, st, tabCount);
        fprintf(outputFile, " & (~ ");
        codeExp(exp->val.binary.rhs, st, tabCount);
        fprintf(outputFile, "))");
        break;
      case ek_uplus:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(+");
          codeExp(exp->val.unary.exp, st, tabCount);
          fprintf(outputFile, "))");
        } else {  // int, float64
          fprintf(outputFile, "(+");
          codeExp(exp->val.unary.exp, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_uminus:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_rune) {  // rune
          fprintf(outputFile, "((char)(-");
          codeExp(exp->val.unary.exp, st, tabCount);
          fprintf(outputFile, "))");
        } else {  // int, float64
          fprintf(outputFile, "(-");
          codeExp(exp->val.unary.exp, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_bang:
        fprintf(outputFile, "(!");
        codeExp(exp->val.unary.exp, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_ubitXor:
        fprintf(outputFile, "(~");
        codeExp(exp->val.unary.exp, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_func:
        // type casting checked first
        s = getSymbol(st, exp->val.funcCall.funcId);
        TYPE *type1 = NULL;
        TYPE *type2 = NULL;
        TYPE *type3 = NULL;
        if (s->kind == dk_type) {
          type1 = s->val.type;
          type2 = s->val.typeDecl.type;
          type3 = s->val.typeDecl.resolvesTo;
        }
        if (type1->kind == tk_int || type2->kind == tk_int || type3->kind == tk_int) {  // int
          fprintf(outputFile, "castUtil.castToInteger(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_float || type2->kind == tk_float || type3->kind == tk_float) {  // float64
          fprintf(outputFile, "castUtil.castToDouble(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_string || type2->kind == tk_string || type3->kind == tk_string) {  // string
          fprintf(outputFile, "castUtil.castToString(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        if (type1->kind == tk_rune || type2->kind == tk_rune || type3->kind == tk_rune) {  // rune
          fprintf(outputFile, "castUtil.castToCharacter(");
          EXP_LIST *exps = exp->val.funcCall.args;
          codeExp(exps->exp, st, tabCount);
          fprintf(outputFile, ")");
          break;
        }
        // normal function call
        fprintf(outputFile, "%s(", prefix(exp->val.funcCall.funcId));
        EXP_LIST *exps = exp->val.funcCall.args;
        while (exps != NULL) {
          codeExp(exps->exp, st, tabCount);

          exps = exps->next;
          if (exps != NULL) {
            fprintf(outputFile, ", ");
          }
        }
        fprintf(outputFile, ")");
        break;
      case ek_append:
        codeExp(exp->val.append.sliceExp, st, tabCount);
        fprintf(outputFile, ".append(");
        codeExp(exp->val.append.elem, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_len:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_string) {  // string
          codeExp(exp->val.lenExp, st, tabCount);
          fprintf(outputFile, ".length()");
        } else if (type->kind == tk_array) {  // array
          codeExp(exp->val.lenExp, st, tabCount);
          fprintf(outputFile, ".length");
        } else {  // slice
          codeExp(exp->val.lenExp, st, tabCount);
          fprintf(outputFile, ".len");
        }
        break;
      case ek_cap:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
          codeExp(exp->val.capExp, st, tabCount);
          fprintf(outputFile, ".length");
        } else {  // slice
          codeExp(exp->val.capExp, st, tabCount);
          fprintf(outputFile, ".cap");
        }
        break;
      case ek_indexExp:
        type = typeResolve(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
          codeExp(exp->val.indexExp.objectExp, st, tabCount);
          fprintf(outputFile, "[");
          codeExp(exp->val.indexExp.indexExp, st, tabCount);
          fprintf(outputFile, "]");
        } else {  // slice
          codeExp(exp->val.indexExp.objectExp, st, tabCount);
          fprintf(outputFile, ".get(");
          codeExp(exp->val.indexExp.indexExp, st, tabCount);
          fprintf(outputFile, ")");
        }
        break;
      case ek_structField:
        codeExp(exp->val.structField.structExp, st, tabCount);
        fprintf(outputFile, ".%s", exp->val.structField.fieldName);
        break;
      case ek_paren:
        fprintf(outputFile, "(");
        codeExp(exp->val.parenExp, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_conv:
        fprintf(stderr, "Logical failure: type conversion should be caught by function expressions.\n");
        break;
    }
  }
}