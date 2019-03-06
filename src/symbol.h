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

void createScope(STMT *stmt, SymbolTable *st);

SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, TYPE *type, int lineno);
SYMBOL *getSymbol(SymbolTable *t, char *name);

void printType(TYPE *type);
void printParamList(PARAM_LIST *pl);
