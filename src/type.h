#ifndef TYPE_H
#define TYPE_H

#include "tree.h"
#include "symbol.h"

void typeProgram(PROG *prog, SymbolTable *st);
void typeDeclarations(DECL *dcl, SymbolTable *st);

void typeVarDecl(VAR_SPECS *vs, SymbolTable *st);
void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st);
void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st);
void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st);

void typeStmt(STMT *stmt, SymbolTable *st);
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
int typeIsBase(TYPE *type);
int resolvesToList(TYPE *type, SymbolTable *st);
int isLValue(EXP *exp);
TYPE *typeOfList(TYPE *type, SymbolTable *st);
int resolvesToTSlice(TYPE *s, TYPE *t, SymbolTable *st);

#endif