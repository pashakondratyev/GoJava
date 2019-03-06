#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "pretty.h"
#include "tree.h"

extern SymbolTable *programSymbolTable;

int tabCount = 0;
SymbolTableMode mode;

int Hash(char *str) {
  unsigned int hash = 0;
  while (*str) hash = (hash << 1) + *str++;
  return hash % HashSize;
}

SymbolTable *initSymbolTable() {
  SymbolTable *t = (SymbolTable*)malloc(sizeof(SymbolTable));
  for (int i = 0; i < HashSize; i++) {
    t->table[i] = NULL;
  }
  t->parent = NULL;
  return t; 
}

SymbolTable *scopeSymbolTable(SymbolTable *s) {
  SymbolTable *t = initSymbolTable();
  t->parent = s;
  return t;
}

SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, TYPE *type, int lineno) {
  int i = Hash(identifier);
  for(SYMBOL *s = t->table[i]; s; s = s->next){
    if(strcmp(s->name, identifier) == 0){
      fprintf(stderr, "Error: (line %d) \"%s\" is already declared\n", lineno, identifier);
      exit(1);
    }
  }

  SYMBOL *s = (SYMBOL*)malloc(sizeof(SYMBOL));
  s->name = identifier;
  s->type = type;
  s->kind = kind;
  s->next = t->table[i];
  t->table[i] = s;
  return s;
}

void putTypeDecl(SymbolTable *st, TYPE_SPECS *ts, int lineno){
  while(ts != NULL){
    if(strcmp(ts->name, "_") == 0){
      ts = ts->next;
      continue;
    }
    putSymbol(st, dk_type, ts->name, ts->type, lineno);
    printf("%s [type] = %s ->", ts->name, ts->name);
    printType(ts->type);
    ts = ts->next;     
    printf("\n");
  }
}

// Functions are top level decl 
void putFuncDecl(SymbolTable *st, FUNC_DECL *fd, int lineno){
  printTab(tabCount);
  printf("%s [function] = ", fd->name);
  if(strcmp(fd->name, "_") == 0){
    printf("<unmapped>");
  }
  else{
    putSymbol(st, dk_func, fd->name, NULL, lineno);
    printParamList(fd->params);
    printf(" -> ");
    if(fd->returnType != NULL){
      printType(fd->returnType);
    }
    else{
      printf("void");
    }

  }
  printf("\n");
  //Need to create a new scope here
  tabCount++;
  symTypesStatements(fd->body, st);
  tabCount--;
}

void putShortDecl(SymbolTable *st, SHORT_SPECS *ss, int lineno){
  while(ss != NULL){
    if(strcmp(ss->lhs->val.id, "_") == 0){
      ss=ss->next;
      continue;
    }
    putSymbol(st, dk_type, ss->lhs->val.id, NULL, lineno);

    prettyPrintExp(ss->lhs);
    printTab(tabCount);
    printf(" [variable] = <infer>\n");
    ss=ss->next;
  }
}

void putVarDecl(SymbolTable *st, VAR_SPECS *vs, int lineno){
  while(vs != NULL){
    if(strcmp(vs->id,"_") == 0){
      vs = vs->next;
      continue;
    }
    putSymbol(st, dk_var, vs->id, vs->type, lineno);
    printTab(tabCount);
    printf("%s [variable] = ", vs->id);
    if(vs->type == NULL){
      printf("<infer>");
    }
    else{
      printType(vs->type);
    }
    printf("\n");
    vs = vs->next;
  }
}

/* Scopes the symbol table and adjusts tab count as necessary */
void createScope(STMT *stmt, SymbolTable *st){
  SymbolTable *scope = scopeSymbolTable(st);
  if(mode == SymbolTablePrint){
    printTab(tabCount);
    printf("{\n"); 
  }
  tabCount++;
  symTypesStatements(stmt, scope);
  tabCount--;
  if(mode == SymbolTablePrint){
    printTab(tabCount);
    printf("}\n");
  }
}

SYMBOL *getSymbol(SymbolTable *t, char *name) {
  int i = Hash(name);
  // Check the current scope
  for (SYMBOL *s = t->table[i]; s; s = s->next) {
    if (strcmp(s->name, name) == 0) return s;
  }
  // Check for existence of a parent scope
  if (t->parent == NULL) return NULL;
  // Check the parent scopes
  return getSymbol(t->parent, name);
}

void symProgram(PROG *root, SymbolTableMode m){
  mode = m; 
  programSymbolTable = initSymbolTable();
  symTypesDeclarations(root->root_decl, programSymbolTable);
}

void symTypesDeclarations(DECL *decl, SymbolTable *st){
  // We could have passed the entire declaration into the put symbol tabl
  while(decl != NULL){
    switch(decl->kind){
      case dk_type:
        putTypeDecl(st, decl->val.typeSpecs, decl->lineno);
        break;
      case dk_func:
        putFuncDecl(st, decl->val.funcDecl, decl->lineno);
        break;
      case dk_var:
        putVarDecl(st, decl->val.varSpecs, decl->lineno);
        break;
      case dk_short:
        putShortDecl(st, decl->val.shortSpecs, decl->lineno);
        break;
    }
    decl=decl->next;
  }
}

void symTypesStatements(STMT *stmt, SymbolTable *st){
  switch(stmt->kind){
    default: break;
  }
  if(st == NULL){
    return;
  }
}

void printType(TYPE *type){
  FIELD_DECLS *d;
  switch(type->kind){
    case tk_array:
      printf("[");
      prettyPrintExp(type->val.array.size);
      printf("]");
      printType(type->val.array.elemType);
      break;
    case tk_slice:
      printf("[]%s", type->val.sliceType->val.name);
      break;
    case tk_struct:
      printf("struct {");
      d = type->val.structFields;
      while(d != NULL){
        printf("%s ", d->id);
        printType(d->type);
        d = d->next;
      }
      printf("}");
      break;
    default:
      printf("%s", type->val.name);
  }
}

void printParamList(PARAM_LIST *pl){
  printf("(");
  while(pl != NULL){
    printType(pl->type);
    if(pl->next != NULL){
      printf(", ");
    }
    pl=pl->next;
  }
  printf(")");
}
