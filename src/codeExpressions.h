#ifndef CODE_EXPRESSIONS_H
#define CODE_EXPRESSIONS_H

#include "tree.h"
#include "symbol.h"
#include "codeStructs.h"
#include "codeIdentifiers.h"

void codeExp(EXP *exp, SymbolTable *st, IdentifierTable *it, int tabCount);

void codeFunctionCall(EXP *exp, SymbolTable *st, IdentifierTable *it, int tabCount);
#endif