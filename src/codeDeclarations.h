#ifndef CODE_DECLARATIONS_H
#define CODE_DECLARATIONS_H

#include "tree.h"
#include "symbol.h"

#include "codeIdentifiers.h"

void codeDeclarations(DECL *dcl, SymbolTable *st, IdentifierTable *it, int tabCount);
void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, IdentifierTable *it, int tabCount);
void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, IdentifierTable *it, int tabCount);
void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, IdentifierTable *it, int tabCount);
#endif