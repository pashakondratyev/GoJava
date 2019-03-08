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

#endif
