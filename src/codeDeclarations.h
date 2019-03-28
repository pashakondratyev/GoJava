#ifndef CODE_DECLARATIONS_H
#define CODE_DECLARATIONS_H

#include "tree.h"
#include "symbol.h"

void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount);
void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount);
void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount);
void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount);
void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount);
#endif