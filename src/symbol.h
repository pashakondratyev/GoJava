#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SYMBOL {
  char *name;
  struct SYMBOL *next;
  DecKind kind;
  union{
    TYPE *type;
    struct {
      TYPE *returnType;
      PARAM_LIST *paramList;
    } functionDecl;
    struct {
      TYPE *type;
      TYPE *resolvesTo;
      TypeKind resolvesToKind;
    } typeDecl;
  } val;
  int constant; //Can we overwrite this?
} SYMBOL;

typedef struct SymbolTable {
  SYMBOL *table[HashSize];
  struct SymbolTable *parent;
} SymbolTable;

typedef enum {
  SymbolTableConstruct,
  SymbolTablePrint,
} SymbolTableMode;

int Hash(char *str);
SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *s);

void symProgram(PROG *prog, SymbolTableMode m);
void symTypesStatements(STMT *stmt, SymbolTable *st);
void symTypesExpressions(EXP *exp, SymbolTable *st);
void symTypesDeclarations(DECL *dcl, SymbolTable *st);
void symTypesDefaults(SymbolTable *st);

void createScope(STMT *stmt, SymbolTable *st);

SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, int lineno);
SYMBOL *getSymbol(SymbolTable *t, char *name);
SYMBOL *getSymbolCurrentScope(SymbolTable *t, char *name);

void putTypeDecl(SymbolTable *st, TYPE_SPECS *ts, int lineno);
void putFuncDecl(SymbolTable *st, FUNC_DECL *fd, int lineno);
void putShortDecl(SymbolTable *st, SHORT_SPECS *ss, int lineno);
void putVarDecl(SymbolTable *st, VAR_SPECS *vs, int lineno);

STMT *paramListToStmt(PARAM_LIST *pl, int lineno);
STMT *combineStmt(STMT *s1, STMT *s2, int lineno);

int duplicateShortDeclarationsExist(char *elementName, SHORT_SPECS *remaining);
int duplicateElementExists(char *elementName, FIELD_DECLS *remaining);

void printType(TYPE *type);
char *getTypeString(char *BUFFER, TYPE *type);
TYPE *fixType(SymbolTable *st, TYPE *type);
void printParamList(PARAM_LIST *pl);
int typeIsBase(TYPE *type);
#endif