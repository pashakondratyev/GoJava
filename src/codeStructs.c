#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeStructs.h"

#include "code.h"
#include "codeTypes.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0
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
        while (ccl != NULL) {
          switch (ccl->clause->kind) {
            case ck_case:
              sl = ccl->clause->val.caseClause.clauses;
              while (sl != NULL) {
                makeStructTableStmt(sl->stmt, st);
                sl = sl->next;
              }
              break;
            case ck_default:
              sl = ccl->clause->val.defaultClauses;
              while (sl != NULL) {
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
  if (DEBUG) printf("Creating Struct in StructTable\n");
  char BUFFER[1024];
  char structCode[2048];
  if (name == NULL) {
    if (type->kind == tk_ref) {
      name = type->val.name;
    } else {
      name = "_";
    }
  }
  getRecTypeString(BUFFER, type, st, name);
  int i = Hash(BUFFER);
  for (STRUCT *s = structTable->table[i]; s; s = s->next) {
    if (strcmp(s->structString, BUFFER) == 0) {
      // Struct string already exists in struct table
      return NULL;
    }
  }

  if (DEBUG) printf("Created struct in StructTable: %s\n", BUFFER);
  STRUCT *s = (STRUCT *)malloc(sizeof(STRUCT));
  s->structString = strdup(BUFFER);
  char javaClassName[10];
  sprintf(javaClassName, "STRUCT%d", identifierCount);
  identifierCount += 1;
  s->className = strdup(javaClassName);
  s->next = structTable->table[i];
  s->type = type;
  s->comparable = resolvesToComparable(type, st);
  structTable->table[i] = s;

  char *javaClass = (char *)malloc(4096 * sizeof(char));
  if (DEBUG) printf("Creating Java Object: %s\n", s->className);
  javaClass = codeStructType(javaClass, type->val.structFields, st, s, name);
  s->javaClass = javaClass;
  if (DEBUG) printf("Java Object Created\n");
  return s;
}

// Retrieves a struct from the structtable
STRUCT *getFromStructTable(char *id) {
  int i = Hash(id);
  for (STRUCT *s = structTable->table[i]; s; s = s->next) {
    if (strcmp(s->structString, id) == 0) {
      return s;
    }
  }
  return NULL;
}

// Prints all the elements in the struct table, for debugging
void printStructTable() {
  for (int i = 0; i < HashSize; i++) {
    STRUCT *s = structTable->table[i];
    while (s != NULL) {
      printf("%s : %s\n", s->className, s->structString);
      s = s->next;
    }
  }
}

// Puts the java representation of a struct into buffer
char *codeStructType(char *BUFFER, FIELD_DECLS *fd, SymbolTable *st, STRUCT *s, char *name) {
  sprintf(BUFFER + strlen(BUFFER), "class %s {\n", s->className);
  char constructorBUFFER[2048];
  char *constructor;
  char *typeName;
  // put all the public parameters

  sprintf(constructorBUFFER, "\tpublic %s() {\n", s->className);

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
        constructor = javaTypeStringConstructorArray(type, st, name);
        typeName = javaTypeString(type, st, name);
        sprintf(BUFFER + strlen(BUFFER), "\t%s %s = new %s;\n", typeName, temp->id, constructor);
        codeZeroOutArrayBuffer(constructorBUFFER + strlen(constructorBUFFER), temp->id, "", temp->type, st, 2);
        break;
      case tk_slice:
        typeName = javaTypeString(type, st, name);
        sprintf(BUFFER + strlen(BUFFER), "\t%s %s = new %s();\n", typeName, temp->id, typeName);
        break;
      default:
        constructor = javaTypeStringDefaultConstructor(type, st, name);
        typeName = javaTypeString(type, st, name);
        sprintf(BUFFER + strlen(BUFFER), "\t%s %s = new %s;\n", typeName, temp->id, constructor);
        break;
    }
  }
  sprintf(constructorBUFFER + strlen(constructorBUFFER), "\n\t}\n");
  // Equality method, note it should not be generated if there is an incomparable type
  if (s->comparable) {
    //Note, overridden equals method MUST use Object as it's parameter
    sprintf(BUFFER + strlen(BUFFER), "\tpublic boolean equals(Object o){\n");

    sprintf(BUFFER + strlen(BUFFER), "\t\tif(this == o) return true;\n");
    sprintf(BUFFER + strlen(BUFFER), "\t\tif(!(o instanceof %s)) return false;\n", s->className);
    sprintf(BUFFER + strlen(BUFFER), "\t\t%1$s other = (%1$s)o;\n", s->className);
    sprintf(BUFFER + strlen(BUFFER), "\t\treturn ");

    if (fd == NULL) { // empty struct
    	sprintf(BUFFER + strlen(BUFFER), "Boolean.TRUE");
    }

    int needsAnd = 0;
    for (FIELD_DECLS *temp = fd; temp; temp = temp->next) {
      if (strcmp(temp->id, "_") == 0) {
        continue;
      }
      if (needsAnd) {
        sprintf(BUFFER + strlen(BUFFER), " && ");
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
          sprintf(BUFFER + strlen(BUFFER), "Arrays.deepEquals(this.%s, other.%s)", temp->id, temp->id);
          break;
        case tk_slice:
          break;
        default:
          name = javaTypeString(type, st, name);
          sprintf(BUFFER + strlen(BUFFER), "this.%s.equals(other.%s)", temp->id, temp->id);
          break;
      }
      if (temp->next != NULL) {
        needsAnd = 1;
      }
    }
    sprintf(BUFFER + strlen(BUFFER), ";\n\t}\n");
  }

  sprintf(constructorBUFFER + strlen(constructorBUFFER), "\tpublic %1$s(%1$s other) {", s->className);
  // Create clone method
  sprintf(BUFFER + strlen(BUFFER), "\tpublic %s copy(){\n\t\t", s->className);
  sprintf(BUFFER + strlen(BUFFER), "%1$s structCopy = new %1$s();", s->className);
  for (FIELD_DECLS *temp = fd; temp; temp = temp->next) {
    if (strcmp(temp->id, "_") == 0) {
      continue;
    }
    sprintf(BUFFER + strlen(BUFFER), "\n\t\t");
    sprintf(constructorBUFFER + strlen(constructorBUFFER), "\n\t\t");
    TYPE *type;
    if (temp->type->kind == tk_ref) {
      SYMBOL *s = getSymbol(st, temp->type->val.name);
      type = s->val.typeDecl.resolvesTo;
    } else {
      type = temp->type;
    }
    char *constructor = javaTypeStringConstructor(temp->type, st, NULL);
    char source[1024];
    char target[1024];
    char other[1024];
    switch (type->kind){
      case tk_array:
        sprintf(source, "structCopy.%s", temp->id);
        sprintf(target, "this.%s", temp->id);
        sprintf(other, "other.%s", temp->id);
        constructor = javaTypeStringConstructorArray(temp->type, st, NULL);
        sprintf(BUFFER + strlen(BUFFER), "structCopy.%1$s = new %2$s;\n", temp->id, constructor);
        sprintf(constructorBUFFER  + strlen(constructorBUFFER), "this.%1$s = new %2$s;\n", temp->id, constructor);  
        codeCopyArrayBuffer(BUFFER + strlen(BUFFER), source, target, "", temp->type, st, 2);
        codeCopyArrayBuffer(constructorBUFFER + strlen(constructorBUFFER), target, other, "", temp->type, st, 2);
        break;
      case tk_slice:
      case tk_struct:
        sprintf(BUFFER + strlen(BUFFER), "structCopy.%1$s = this.%1$s.copy();", temp->id);
        sprintf(constructorBUFFER + strlen(constructorBUFFER), "this.%1$s = other.%1$s.copy();", temp->id);
        break;
      default:
        sprintf(BUFFER + strlen(BUFFER), "structCopy.%1$s = this.%1$s;", temp->id);
        sprintf(constructorBUFFER + strlen(constructorBUFFER), "this.%1$s = other.%1$s;", temp->id);
        break;
    }
  }
  sprintf(BUFFER + strlen(BUFFER), "\n\t\treturn structCopy;");
  sprintf(BUFFER + strlen(BUFFER), "\n\t}\n");

  sprintf(constructorBUFFER  + strlen(constructorBUFFER), "\n\t}\n");  
  sprintf(BUFFER + strlen(BUFFER), "%s", constructorBUFFER); 

  sprintf(BUFFER + strlen(BUFFER), "}\n");
  return BUFFER;
}

// Outputs the java objects based on all the structs encountered in the file
void codeStructTable() {
  for (int i = 0; i < HashSize; i++) {
    STRUCT *s = structTable->table[i];
    while (s != NULL) {
      fprintf(outputFile, "%s\n", s->javaClass);
      s = s->next;
    }
  }
}
