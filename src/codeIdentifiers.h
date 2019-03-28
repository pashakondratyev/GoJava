#ifndef CODE_IDENTIFIERS_H
#define CODE_IDENTIFIERS_H

#include "code.h"
#include "tree.h"
#include "symbol.h"

typedef struct IDENTIFIER {
    SYMBOL *symbol;
    char *newIdentifier; 
    struct IDENTIFIER *next;
} IDENTIFIER;

typedef struct StructTable {
    IDENTIFIER *table[HashSize];
} StructTable;

StructTable *structTable;

void makeStructTable(DECL *decl, SymbolTable *st);
void makeStructTableVarDecl(VAR_SPECS *vs, SymbolTable *st);
void makeStructTableShortDecl(SHORT_SPECS *ss, SymbolTable *st);
void makeStructTableTypeDecl(TYPE_SPECS *ts, SymbolTable *st);
void makeStructTableFuncDecl(FUNC_DECL *fd, SymbolTable *st);
void makeStructTableStmt(STMT *stmt, SymbolTable *st);
IDENTIFIER *addToStructTable(TYPE *type, char *name, SymbolTable *st);
IDENTIFIER *getFromStructTable(char *id);
void initStructTable();
void printStructTable();

char *codeStructType(char *BUFFER, FIELD_DECLS *fd, SymbolTable *st, IDENTIFIER *s, char *name);
void codeStructTable();
#endif