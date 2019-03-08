#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SYMBOL {
  char *name;
  struct SYMBOL *next;
  DecKind kind;
  TYPE *type; 
} SYMBOL;

typedef struct SymbolTable {
  SYMBOL *table[HashSize];
  struct SymbolTable *parent;
} SymbolTable;

typedef enum {
  SymbolTableConstruct,
  SymbolTablePrint,
} SymbolTableMode;

SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *s);

void symProgram(PROG *prog, SymbolTableMode m);
void symTypesStatements(STMT *stmt, SymbolTable *st);
void symTypesExpressions(EXP *exp, SymbolTable *st);
void symTypesDeclarations(DECL *dcl, SymbolTable *st);
void symTypesDefaults(SymbolTable *st);

void createScope(STMT *stmt, SymbolTable *st);

SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, TYPE *type, int lineno);
SYMBOL *getSymbol(SymbolTable *t, char *name);
SYMBOL *getSymbolCurrentScope(SymbolTable *t, char *name);

void putTypeDecl(SymbolTable *st, TYPE_SPECS *ts, int lineno);
void putFuncDecl(SymbolTable *st, FUNC_DECL *fd, int lineno);
void putShortDecl(SymbolTable *st, SHORT_SPECS *ss, int lineno);
void putVarDecl(SymbolTable *st, VAR_SPECS *vs, int lineno);

STMT *paramListToStmt(PARAM_LIST *pl, int lineno);
STMT *combineStmt(STMT *s1, STMT *s2, int lineno);

void printType(TYPE *type);
void printParamList(PARAM_LIST *pl);

#endif
