#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "code.h"
#include "symbol.h"
#include "type.h"

FILE *outputFile;
StructTable *structTable;
int numInitFunc = 0;
int identifierCount = 0;

StructTable *initStructTable() {
  StructTable *structTable = (StructTable *)malloc(sizeof(StructTable));
  for (int i = 0; i < HashSize; i++) {
    structTable->table[i] = NULL;
  }
  return structTable;
}

void printStructTable(StructTable *structTable) {
  for (int i = 0; i < HashSize; i++) {
    STRUCT *s = structTable->table[i];
    while (s != NULL) {
      printf("%s : %s\n", s->className, s->structString);
      s = s->next;
    }
  }
}

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
  codeSetup(&inputFileName[index + 1]);
  structTable = initStructTable();
  makeStructTable(prog->root_decl, st);
  printStructTable(structTable);
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
  fprintf(outputFile, "\tpublic Boolean __golite__true = Boolean.TRUE;\n");
  fprintf(outputFile, "\tpublic Boolean __golite__false = Boolean.FALSE;\n\n");

  fprintf(outputFile, "\tpublic static boolean stringLessThan(String s1, String s2) {\n");
  fprintf(outputFile, "\t\treturn (s1.compareTo(s2) < 0) ? Boolean.TRUE : Boolean.FALSE;\n");
  fprintf(outputFile, "\t}\n\n");
  fprintf(outputFile, "\tpublic static boolean stringGreaterEqual(String s1, String s2) {\n");
  fprintf(outputFile, "\t\treturn (s1.compareTo(s2) >= 0) ? Boolean.TRUE : Boolean.FALSE;\n");
  fprintf(outputFile, "\t}\n\n");
  fprintf(outputFile, "\tpublic static boolean stringLessEqual(String s1, String s2) {\n");
  fprintf(outputFile, "\t\treturn (s1.compareTo(s2) <= 0) ? Boolean.TRUE : Boolean.FALSE;\n");
  fprintf(outputFile, "\t}\n\n");

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
    if (resolvesToStruct(fd->returnType, st)) {
      addToStructTable(fd->returnType, NULL, st);
    }
  }
  if (fd->params != NULL) {
    PARAM_LIST *pl = fd->params;
    while (pl != NULL) {
      if (resolvesToStruct(pl->type, st)) {
        addToStructTable(pl->type, NULL, st);
      }
      pl = pl->next;
    }
  }
}

void makeStructTableStmt(STMT *stmt, SymbolTable *st) {
  if (stmt != NULL) {
    STMT_LIST *sl;
    switch (stmt->kind) {
      case sk_block:
        sl = stmt->val.block.blockStatements;
        while (sl != NULL) {
          makeStructTableStmt(sl->stmt, st);
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
      default:
        break;
    }
  }
}

// Take a struct type, check if it exists in the struct table, and add it if it doesn't
void addToStructTable(TYPE *type, char *name, SymbolTable *st) {
  char buffer[100];
  if (name != NULL) {
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
      return;
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
  structTable->table[i] = s;
}

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

char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name) {
  FIELD_DECLS *d;
  if (type == NULL) {
    sprintf(BUFFER, " ");
    return BUFFER;
  }
  switch (type->kind) {
    case tk_array:
      sprintf(BUFFER, "[");
      sprintf(BUFFER + strlen(BUFFER), "%d", type->val.array.size);
      sprintf(BUFFER + strlen(BUFFER), "]");
      getRecTypeString(BUFFER, type->val.array.elemType, st, name);
      break;
    case tk_slice:
      sprintf(BUFFER, "[]");
      getRecTypeString(BUFFER, type->val.sliceType, st, name);
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
      if (strcmp(name, type->val.name) != 0) {
        getRecTypeString(BUFFER + strlen(BUFFER), typeResolve(type, st), st, type->val.name);
      } else {
        sprintf(BUFFER + strlen(BUFFER), "%s", type->val.name);
      }
      break;
    default:
      sprintf(BUFFER, "%s", type->val.name);
      break;
  }
  return BUFFER;
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
  char buffer[1024];
  // If this is not a reference we need to handle this specially
  char *returnTypeString = fd->returnType == NULL ? "void" : getTypeString(buffer, fd->returnType);
  fprintf(outputFile, "public %s %s (", returnTypeString, prefix(fd->name));
  // print args
  fprintf(outputFile, ") {\n");
  // print function body
  // TODO: implement & handle init functions
}

void codeStmt(STMT *stmt, SymbolTable *st, TYPE *returnType, int tabCount) {
  // TODO: implement
}

void codeExp(EXP *exp, SymbolTable *st, int tabCount) {
  // TODO: complete
  TYPE *type;
  if (exp != NULL) {
    switch (exp->kind) {
      case ek_id:
        if (strcmp(exp->val.id, "_") == 0) {
          // TODO: do something with blank id
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
      	if (type->kind == tk_array) {	// arrays
      		fprintf(outputFile, "(Arrays.deepEquals(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, ",");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, "))");
      	} else {	// all other types: int, float64, rune, string, structs
	        fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, ".equals(");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, "))");
	      }
        break;
      case ek_ne:
      	type = resolveExpType(exp->val.binary.lhs->type, st);
      	if (type->kind == tk_array) {	// arrays
      		fprintf(outputFile, "(!Arrays.deepEquals(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, ",");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, "))");
      	} else {	// all other types: int, float64, rune, string, structs
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
        // TODO: complete
        break;
      case ek_append:
        // TODO: complete
      	// call the slice append method: idName.append(value)
        break;
      case ek_len:
        // TODO: complete
      	//valid on string, slice, and array
        break;
      case ek_cap:
        // TODO: complete
      	// valid on slice, and array
        break;
      case ek_indexExp:
        // TODO: complete
      	// valid on slice and array
        break;
      case ek_structField:
        // TODO: complete
      	// java object equivalent.fieldname
        break;
      case ek_paren:
        fprintf(outputFile, "(");
        codeExp(exp->val.parenExp, st, tabCount);
        fprintf(outputFile, ")");
        break;
      case ek_conv:
        // TODO: complete
      	// create functions for valid casts
        break;
    }
  }
}

TYPE* resolveExpType(TYPE* type, SymbolTable* st) {
  if (type->kind == tk_ref) {
    SYMBOL *s = getSymbol(st, type->val.name);
    type = s->val.typeDecl.resolvesTo;
  }
  if (type->kind == tk_ref) {
    fprintf(stderr, "Logical failure: shouldn't reach a reference type\n");
    exit(1);
  }
  return type;
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
        codeStructType(type->val.structFields, st);
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

// TODO: make this read from hash table of classes
void codeStructType(FIELD_DECLS *fd, SymbolTable *st) {}
