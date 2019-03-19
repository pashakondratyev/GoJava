#ifndef CODE_H
#define CODE_H

#include "tree.h"
#include "symbol.h"

void codeProgram(PROG *prog, SymbolTable *st);
void codePackage(PACKAGE *package);
void codeDeclarations(DECL *dcl, SymbolTable *st);

#endif