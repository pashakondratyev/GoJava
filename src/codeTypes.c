#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeTypes.h"

#include "code.h"
#include "codeStructs.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

// Types - Recursively gets the string representations of types
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

// Returns the string of the Java type
char *javaTypeString(TYPE *type, SymbolTable *st, char *name) {
  char BUFFER[1024];
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
        getRecTypeString(BUFFER, type, st, name);
        STRUCT *s = getFromStructTable(BUFFER);
        if (s == NULL) {
          s = addToStructTable(type, NULL, st);
          if (s == NULL) {
            fprintf(stderr, "Error! Could not retrieve struct during code generation\n");
            exit(1);
          }
        }
        return s->className;
      case tk_array:
        if (DEBUG) printf("javaTypeString: tk_array\n");
        sprintf(BUFFER, "%s[]", javaTypeString(type->val.array.elemType, st, name));
        return strdup(BUFFER);
      case tk_slice:
        if (DEBUG) printf("javaTypeString: tk_slice\n");
        sprintf(BUFFER, "Slice<%s>", javaTypeString(type->val.sliceType, st, name));
        return strdup(BUFFER);
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

// Returns the java string for the constructor of the type, minus the paranthesis
char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name) {
  char *BUFFER = (char *)(malloc(1024));
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
        getRecTypeString(BUFFER, type, st, name);
        STRUCT *s = getFromStructTable(BUFFER);
        if (s == NULL) {
          s = addToStructTable(type, NULL, st);
          if (s == NULL) {
            fprintf(stderr, "Error! Could not retrieve struct during code generation\n");
            exit(1);
          }
        }
        return strdup(s->className);
      case tk_array:
        sprintf(BUFFER, "%s[%d]", javaTypeStringConstructor(type->val.array.elemType, st, name), type->val.array.size);
        return strdup(BUFFER);
      case tk_slice:
        sprintf(BUFFER, "Slice<%s>", javaTypeString(type->val.sliceType, st, name));
        return strdup(BUFFER);
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

char *javaConstructorForBasicTypes(TYPE *type, SymbolTable *st) {
  char *BUFFER = (char *)(malloc(1024));
  SYMBOL *symbol;
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
        return "new Integer";
      case tk_float:
        return "new Double";
      case tk_rune:
        return "new Character";
      case tk_string:
        return "new String";
      case tk_boolean:
        return "new Boolean";
      case tk_ref:
        symbol = getSymbol(st, type->val.name);
        return javaConstructorForBasicTypes(symbol->val.typeDecl.resolvesTo, st);
      case tk_res:
        fprintf(stderr, "Encountered tk_res type during code generation");
        exit(1);
      default:
        return "";
    }
  }
  return "";
}

char *javaTypeStringDefaultConstructor(TYPE *type, SymbolTable *st, char *name) {
  char *BUFFER = (char *)(malloc(1024));
  SYMBOL *symbol;
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
        return "Integer(0)";
      case tk_float:
        return "Double(0.0)";
      case tk_rune:
        return "Character(' ')";
      case tk_string:
        return "String(\"\")";
      case tk_boolean:
        return "Boolean(false)";
      case tk_struct:
        getRecTypeString(BUFFER, type, st, name);
        STRUCT *s = getFromStructTable(BUFFER);
        if (s == NULL) {
          s = addToStructTable(type, NULL, st);
          if (s == NULL) {
            fprintf(stderr, "Error! Could not retrieve struct during code generation\n");
            exit(1);
          }
        }
        sprintf(BUFFER, "%s()", s->className);
        return strdup(BUFFER);
      case tk_array:
        sprintf(BUFFER, "%s[%d]", javaTypeStringConstructor(type->val.array.elemType, st, name), type->val.array.size);
        return strdup(BUFFER);
      case tk_slice:
        sprintf(BUFFER, "Slice<%s>()", javaTypeString(type->val.sliceType, st, name));
        return strdup(BUFFER);
      case tk_ref:
        symbol = getSymbol(st, type->val.name);
        return javaTypeStringDefaultConstructor(symbol->val.typeDecl.resolvesTo, st, name);
      case tk_res:
        fprintf(stderr, "Encountered tk_res type during code generation");
        exit(1);
    }
  }
  return "";
}