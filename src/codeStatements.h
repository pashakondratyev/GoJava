#ifndef CODE_STATEMENTS_H
#define CODE_STATEMENTS_H

#include <stdbool.h>
#include "tree.h"
#include "symbol.h"
#include "codeIdentifiers.h"

void codeStmt(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount, bool incompleteBlock, STMT *parentPost);

void codeAssignment(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount);
void codeAssignmentOp(STMT *stmt, SymbolTable *st, IdentifierTable *it, int tabCount);
#endif