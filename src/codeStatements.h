#ifndef CODE_STATEMENTS_H
#define CODE_STATEMENTS_H

#include "tree.h"
#include "symbol.h"

void codeStmt(STMT *stmt, SymbolTable *st, int tabCount);
#endif