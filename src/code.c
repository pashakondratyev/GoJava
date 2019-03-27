#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

FILE *outputFile;
StructTable *structTable;
int numInitFunc = 0;
int identifierCount = 0;

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
  makeStructTable(prog->root_decl, st);
  // For Debugging purposes
  //printStructTable();
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
    fprintf(outputFile, "\t\t__golite__init_%d();\n", numInitFunc);
  }
  fprintf(outputFile, "\t\t__golite__main();\n");
  fprintf(outputFile, "\t}\n");
  fprintf(outputFile, "}\n");
}

// Struct Table Functions

void makeStructTable(DECL *decl, SymbolTable *st) {
  if (decl != NULL) {
    switch (decl->kind) {
      case dk_var:
        makeStructTableVarDecl(decl->val.varSpecs, st);
        break;
      case dk_short:
        break;
      case dk_type:
        makeStructTableTypeDecl(decl->val.typeSpecs, st);
        break;
      case dk_func:
        makeStructTableFuncDecl(decl->val.funcDecl, st);
        break;
    }
    makeStructTable(decl->next, st);
  }
}

void makeStructTableVarDecl(VAR_SPECS *vs, SymbolTable *st) {
  while (vs != NULL) {
    if (vs->type->kind == tk_struct) {
      addToStructTable(vs->type, NULL, st);
    }
    vs = vs->next;
  }
}

void makeStructTableTypeDecl(TYPE_SPECS *ts, SymbolTable *st) {
  while (ts != NULL) {
    if (ts->type->kind == tk_struct) {
      addToStructTable(ts->type, ts->name, st);
    }
    ts = ts->next;
  }
}

void makeStructTableFuncDecl(FUNC_DECL *fd, SymbolTable *st) {
  if (fd->body != NULL) {
    makeStructTableStmt(fd->body, st);
  }
  if (fd->returnType != NULL) {
    if (fd->returnType->kind == tk_struct) {
      addToStructTable(fd->returnType, NULL, st);
    }
  }
  if (fd->params != NULL) {
    PARAM_LIST *pl = fd->params;
    while (pl != NULL) {
      if (pl->type->kind == tk_struct) {
        addToStructTable(pl->type, NULL, st);
      }
      pl = pl->next;
    }
  }
}

void makeStructTableStmt(STMT *stmt, SymbolTable *st) {
  if (stmt != NULL) {
    STMT_LIST *sl;
    CASE_CLAUSE_LIST *ccl;
    switch (stmt->kind) {
      case sk_block:
        sl = stmt->val.block.blockStatements;
        while (sl != NULL) {
          makeStructTableStmt(sl->stmt, stmt->val.block.scope);
          sl = sl->next;
        }
        break;
      case sk_decl:
        makeStructTable(stmt->val.decl, st);
        break;
      case sk_if:
        makeStructTableStmt(stmt->val.ifStmt.body, st);
        if (stmt->val.ifStmt.elseStmt != NULL) {
          makeStructTableStmt(stmt->val.ifStmt.elseStmt, st);
        }
      case sk_else:
        makeStructTableStmt(stmt->val.elseBody, st);
        break;
      case sk_for:
        makeStructTableStmt(stmt->val.forStmt.body, st);
        break;
      case sk_switch:
        ccl = stmt->val.switchStmt.caseClauses;
        while (ccl != NULL){
          switch (ccl->clause->kind){
            case ck_case:
              sl = ccl->clause->val.caseClause.clauses;
              while (sl != NULL){
                makeStructTableStmt(sl->stmt, st);
                sl = sl->next;
              }
              break;
            case ck_default:
              sl = ccl->clause->val.defaultClauses;
              while (sl != NULL){
                makeStructTableStmt(sl->stmt, st);
                sl = sl->next;
              }
              break;
          }
          ccl = ccl->next;
        }
        break;
      default:
        break;
    }
  }
}

void initStructTable() {
  StructTable *newStructTable = (StructTable *)malloc(sizeof(StructTable));
  for (int i = 0; i < HashSize; i++) {
    newStructTable->table[i] = NULL;
  }
  structTable = newStructTable;
}

// Take a struct type, check if it exists in the struct table, and add it if it doesn't
STRUCT *addToStructTable(TYPE *type, char *name, SymbolTable *st) {
  if (DEBUG) printf("Creating Struct in Table\n");
  char buffer[1024];
  if (name == NULL) {
    if (type->kind == tk_ref) {
      name = type->val.name;
    } else {
      name = "_";
    }
  }
  getRecTypeString(buffer, type, st, name);
  int i = Hash(buffer);
  for (STRUCT *s = structTable->table[i]; s; s = s->next) {
    if (strcmp(s->structString, buffer) == 0) {
      // Struct string already exists in struct table
      return NULL;
    }
  }

  STRUCT *s = (STRUCT *)malloc(sizeof(STRUCT));
  s->structString = strdup(buffer);
  char javaClassName[10];
  sprintf(javaClassName, "STRUCT%d", identifierCount);
  identifierCount += 1;
  s->className = strdup(javaClassName);
  s->next = structTable->table[i];
  s->type = type;
  s->comparable = resolvesToComparable(type, st);
  structTable->table[i] = s;

  if (DEBUG) printf("Creating Java Object: %s\n", s->className);
  codeStructType(type->val.structFields, st, s, name);
  if (DEBUG) printf("Java Object Created\n");
  return s;
}

STRUCT *getFromStructTable(char *id) {
  int i = Hash(id);
  for (STRUCT *s = structTable->table[i]; s; s = s->next) {
    if (strcmp(s->structString, id) == 0) {
      return s;
    }
  }
  return NULL;
}

void printStructTable() {
  for (int i = 0; i < HashSize; i++) {
    STRUCT *s = structTable->table[i];
    while (s != NULL) {
      printf("%s : %s\n", s->className, s->structString);
      s = s->next;
    }
  }
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

// TODO: make this read from hash table of classes
void codeStructType(FIELD_DECLS *fd, SymbolTable *st, STRUCT *s, char *name) {
  fprintf(outputFile, "class %s {\n", s->className);
  char *constructor;
  char *typeName;
  // put all the public parameters
  for (FIELD_DECLS *temp = fd; temp; temp = temp->next) {
    if (strcmp(temp->id, "_") == 0) {
      continue;
    }
    if (DEBUG) printf("Creating field : %s\n", temp->id);
    TYPE *type;
    if (temp->type->kind == tk_ref) {
      SYMBOL *s = getSymbol(st, temp->type->val.name);
      type = s->val.typeDecl.resolvesTo;
    } else {
      type = temp->type;
    }
    switch (type->kind) {
      case tk_array:
        constructor = javaTypeStringConstructor(type, st, name);
        typeName = javaTypeString(type, st, name);
        fprintf(outputFile, "\t%s %s = new %s;\n", typeName, temp->id, constructor);
        break;
      case tk_slice:
        typeName = javaTypeString(type, st, name);
        fprintf(outputFile, "\t%s %s = new Slice<>();\n", typeName, temp->id);
        break;
      default:
        constructor = javaTypeStringConstructor(type, st, name);
        typeName = javaTypeString(type, st, name);
        fprintf(outputFile, "\t%s %s = new %s();\n", typeName, temp->id, constructor);
        break;
    }
  }
  // Equality method, note it should not be generated if there is an incomparable type
  if (s->comparable) {
    fprintf(outputFile, "\tpublic Boolean equals(%s other){\n\t\treturn ", s->className);

    for (FIELD_DECLS *temp = fd; temp; temp = temp->next) {
      if (strcmp(temp->id, "_") == 0) {
        continue;
      }
      TYPE *type;
      if (temp->type->kind == tk_ref) {
        SYMBOL *s = getSymbol(st, temp->type->val.name);
        type = s->val.typeDecl.resolvesTo;
      } else {
        type = temp->type;
      }
      switch (type->kind) {
        case tk_array:
          name = javaTypeString(type, st, name);
          fprintf(outputFile, "Arrays.deepEquals(this.%s, other.%s)", temp->id, temp->id);
          break;
        case tk_slice:
          break;
        default:
          name = javaTypeString(type, st, name);
          fprintf(outputFile, "this.%s.equals(other.%s)", temp->id, temp->id);
          break;
      }
      if (temp->next != NULL) {
        fprintf(outputFile, " && ");
      }
    }
    fprintf(outputFile, ";\n\t}\n");
  }

  fprintf(outputFile, "}\n");
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
  char buffer[1024];
  // If this is not a reference we need to handle this specially
  char *returnTypeString = fd->returnType == NULL ? "void" : javaTypeString(fd->returnType, st, NULL);
  fprintf(outputFile, "\tpublic static %s %s (", returnTypeString, prefix(fd->name));
  for (PARAM_LIST *temp = fd->params; temp; temp = temp->next) {
    fprintf(outputFile, "%s %s", javaTypeString(temp->type, st, NULL), temp->id);
    if (temp->next) {
      fprintf(outputFile, ", ");
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.binary.lhs->type, st);
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
        type = resolveExpType(exp->val.unary.exp->type, st);
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
        type = resolveExpType(exp->val.unary.exp->type, st);
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
      	if (type1->kind == tk_int || type2->kind == tk_int || type3->kind == tk_int) {		// int
      		fprintf(outputFile, "castUtil.castToInteger(");
        	EXP_LIST *exps = exp->val.funcCall.args;
	      	codeExp(exps->exp, st, tabCount);
        	fprintf(outputFile, ")");
      		break;
      	}
      	if (type1->kind == tk_float || type2->kind == tk_float || type3->kind == tk_float) {		// float64
      		fprintf(outputFile, "castUtil.castToDouble(");
        	EXP_LIST *exps = exp->val.funcCall.args;
	      	codeExp(exps->exp, st, tabCount);
        	fprintf(outputFile, ")");
      		break;
      	}
      	if (type1->kind == tk_string || type2->kind == tk_string || type3->kind == tk_string) {		// string
      		fprintf(outputFile, "castUtil.castToString(");
        	EXP_LIST *exps = exp->val.funcCall.args;
	      	codeExp(exps->exp, st, tabCount);
        	fprintf(outputFile, ")");
      		break;
      	}
      	if (type1->kind == tk_rune || type2->kind == tk_rune || type3->kind == tk_rune) {		// rune
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

      		exps = exps -> next;
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
      	type = resolveExpType(exp->val.unary.exp->type, st);
        if (type->kind == tk_string) {  // string
        	codeExp(exp->val.lenExp, st, tabCount);
        	fprintf(outputFile, ".length()");
        } else if (type->kind == tk_array) {  // array
        	codeExp(exp->val.lenExp, st, tabCount);
        	fprintf(outputFile, ".length");
        } else 	{		// slice
        	codeExp(exp->val.lenExp, st, tabCount);
        	fprintf(outputFile, ".len");
        }
        break;
      case ek_cap:
      	type = resolveExpType(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
        	codeExp(exp->val.capExp, st, tabCount);
        	fprintf(outputFile, ".length");
        } else 	{		// slice
        	codeExp(exp->val.capExp, st, tabCount);
        	fprintf(outputFile, ".cap");
        }
        break;
      case ek_indexExp:
      	type = resolveExpType(exp->val.unary.exp->type, st);
        if (type->kind == tk_array) {  // array
        	codeExp(exp->val.indexExp.objectExp, st, tabCount);
        	fprintf(outputFile, "[");
        	codeExp(exp->val.indexExp.indexExp, st, tabCount);
        	fprintf(outputFile, "]");
        } else 	{		// slice
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

TYPE *resolveExpType(TYPE *type, SymbolTable *st) {
  if (type->kind == tk_ref) {
    SYMBOL *s = getSymbol(st, type->val.name);
    type = s->val.typeDecl.resolvesTo;
  }
  if (type->kind == tk_ref) {
    fprintf(stderr, "Logical failure: shouldn't reach a reference type.\n");
    exit(1);
  }
  return type;
}

// Types - Get string representations of types
char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name) {
  FIELD_DECLS *d;
  if (type == NULL) {
    sprintf(BUFFER, " ");
    return BUFFER;
  }
  switch (type->kind) {
    case tk_array:
      sprintf(BUFFER, "[%d]", type->val.array.size);
      getRecTypeString(BUFFER + strlen(BUFFER), type->val.array.elemType, st, name);
      break;
    case tk_slice:
      sprintf(BUFFER, "[]");
      getRecTypeString(BUFFER + strlen(BUFFER), type->val.sliceType, st, name);
      break;
    case tk_struct:
      sprintf(BUFFER, "struct {");
      d = type->val.structFields;
      while (d != NULL) {
        sprintf(BUFFER + strlen(BUFFER), " %s ", d->id);
        getRecTypeString(BUFFER + strlen(BUFFER), d->type, st, name);
        sprintf(BUFFER + strlen(BUFFER), ";");
        d = d->next;
      }
      sprintf(BUFFER + strlen(BUFFER), " }");
      break;
    case tk_ref:
      if (name == NULL || strcmp(name, type->val.name) != 0) {
        getRecTypeString(BUFFER + strlen(BUFFER), typeResolve(type, st), st, type->val.name);
      } else {
        sprintf(BUFFER + strlen(BUFFER), "%s", type->val.name);
      }
      break;
    default:
      getTypeString(BUFFER + strlen(BUFFER), type);
      break;
  }
  return BUFFER;
}

// TODO: rest of the types
void codeType(TYPE *type, SymbolTable *st, int tabCount) {
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
        fprintf(outputFile, "Integer ");
        break;
      case tk_float:
        fprintf(outputFile, "Double ");
        break;
      case tk_rune:
        fprintf(outputFile, "Character ");
        break;
      case tk_string:
        fprintf(outputFile, "String ");
        break;
      case tk_boolean:
        fprintf(outputFile, "Boolean ");
        break;
      case tk_struct:
        // codeStructType(type->val.structFields, st);
        break;
      case tk_array:
        break;
      case tk_slice:
        break;
      case tk_ref:
        break;
      case tk_res:
        fprintf(stderr, "Encountered tk_res type during code generation");
        exit(1);
    }
  }
}

// TODO: rest of the types
char *javaTypeString(TYPE *type, SymbolTable *st, char *name) {
  char buffer[1024];
  SYMBOL *symbol;
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
        return "Integer";
      case tk_float:
        return "Double";
      case tk_rune:
        return "Character";
      case tk_string:
        return "String";
      case tk_boolean:
        return "Boolean";
      case tk_struct:
        if (DEBUG) printf("javaTypeString: tk_struct\n");
        getRecTypeString(buffer, type, st, name);
        STRUCT *s = getFromStructTable(buffer);
        if (s == NULL) {
          fprintf(stderr, "Error! Could not retrieve struct during code generation\n");
          exit(1);
        }
        return s->className;
      case tk_array:
        if (DEBUG) printf("javaTypeString: tk_array\n");
        sprintf(buffer, "%s[]", javaTypeString(type->val.array.elemType, st, name));
        return strdup(buffer);
      case tk_slice:
        if (DEBUG) printf("javaTypeString: tk_slice\n");
        sprintf(buffer, "Slice<%s>", javaTypeString(type->val.sliceType, st, name));
        return strdup(buffer);
      case tk_ref:
        if (DEBUG) printf("javaTypeString: tk_ref %s\n", name);
        symbol = getSymbol(st, type->val.name);
        return javaTypeString(symbol->val.typeDecl.resolvesTo, st, name);
      case tk_res:
        fprintf(stderr, "Encountered tk_res type during code generation");
        exit(1);
    }
  }
  return "";
}

char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name) {
  char *buffer = (char *)(malloc(1024));
  SYMBOL *symbol;
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
        return "Integer";
      case tk_float:
        return "Double";
      case tk_rune:
        return "Character";
      case tk_string:
        return "String";
      case tk_boolean:
        return "Boolean";
      case tk_struct:
        getRecTypeString(buffer, type, st, name);
        STRUCT *s = getFromStructTable(buffer);
        if (s == NULL) {
          fprintf(stderr, "Error! Could not retrieve struct during code generation\n");
          exit(1);
        }
        return strdup(s->className);
      case tk_array:
        sprintf(buffer, "%s[%d]", javaTypeStringConstructor(type->val.array.elemType, st, name), type->val.array.size);
        return strdup(buffer);
      case tk_slice:
        sprintf(buffer, "Slice<%s>", javaTypeString(type->val.sliceType, st, name));
        return strdup(buffer);
      case tk_ref:
        symbol = getSymbol(st, type->val.name);
        return javaTypeStringConstructor(symbol->val.typeDecl.resolvesTo, st, name);
      case tk_res:
        fprintf(stderr, "Encountered tk_res type during code generation");
        exit(1);
    }
  }
  return "";
}
