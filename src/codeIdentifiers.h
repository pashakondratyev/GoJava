#ifndef CODE_IDENTIFIERS_H
#define CODE_IDENTIFIERS_H

#include "code.h"
#include "tree.h"
#include "symbol.h"

typedef struct IDENTIFIER {
    char *identifier;
    int scopeCount;
    struct IDENTIFIER *next;
} IDENTIFIER;

typedef struct IdentifierTable {
    IDENTIFIER *table[HashSize];
    struct IdentifierTable *parent;
} IdentifierTable;

IdentifierTable *identifierTable;

IdentifierTable *initIdentifierTable();
IdentifierTable *scopeIdentifierTable(IdentifierTable *identifierTable);
IDENTIFIER *addToIdentifierTable(char *identifier, int scopeCount, IdentifierTable *it);
IDENTIFIER *getFromIdentifierTable(char *id, IdentifierTable *it);
IDENTIFIER *addIfNotInTable(char *id, IdentifierTable *it);
#endif