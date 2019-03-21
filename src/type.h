#ifndef TYPE_H
#define TYPE_H

#include "tree.h"
#include "symbol.h"

typedef enum {
  NoReturn,
  Returns,
  Breaks
} ReturnStatus; 

void typeProgram(PROG *prog, SymbolTable *st);
void typeDeclarations(DECL *dcl, SymbolTable *st);

void typeVarDecl(VAR_SPECS *vs, SymbolTable *st);
void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st);
void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st);
void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st);

ReturnStatus typeStmt(STMT *stmt, SymbolTable *st, TYPE *returnType);
void typeExp(EXP *exp, SymbolTable *st);

int typeCompare(TYPE *type1, TYPE *type2, SymbolTable *st);

int typeComparable(TYPE *type);
int typeOrdered(TYPE *type);
int typeNumeric(TYPE *type);
int typeInteger(TYPE *type);
int typeString(TYPE *type);
int typeBool(TYPE *type);
int typeList(TYPE *type);

int resolves(TYPE *source, TYPE *target);

int resolvesToList(TYPE *type, SymbolTable *st);
int isLValue(EXP *exp, SymbolTable *st);
TYPE *typeOfList(TYPE *type, SymbolTable *st);
int resolvesToTSlice(TYPE *s, TYPE *t, SymbolTable *st);
int resolvesToSame(TYPE *s, TYPE *t, SymbolTable *st);
int resolvesToBase(TYPE *type, SymbolTable *st);
int resolvesToInteger(TYPE *type, SymbolTable *st);
int resolvesToString(TYPE *type, SymbolTable *st);
int resolvesToBool(TYPE *type, SymbolTable *st);
int resolvesToNumeric(TYPE *type, SymbolTable *st);
int resolvesToOrdered(TYPE *type, SymbolTable *st);
int resolvesToComparable(TYPE *type, SymbolTable *st);

int resolvesToStruct(TYPE *type, SymbolTable *st);
char *getAssignOpString(AssignOpKind kind);

TYPE *isAddressableStruct(EXP *exp, SymbolTable *st);
int isAddressableArray(EXP *exp, SymbolTable *st);

#endif