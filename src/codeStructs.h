#ifndef CODE_STRUCTS_H
#define CODE_STRUCTS_H

#include "code.h"
#include "tree.h"
#include "symbol.h"

typedef struct STRUCT {
    char *structString;
    char *className;
    char *javaClass;
    int comparable;
    TYPE *type;
    struct STRUCT *next;
} STRUCT;

typedef struct StructTable {
    STRUCT *table[HashSize];
} StructTable;

StructTable *structTable;

void makeStructTable(DECL *decl, SymbolTable *st);
void makeStructTableVarDecl(VAR_SPECS *vs, SymbolTable *st);
void makeStructTableShortDecl(SHORT_SPECS *ss, SymbolTable *st);
void makeStructTableTypeDecl(TYPE_SPECS *ts, SymbolTable *st);
void makeStructTableFuncDecl(FUNC_DECL *fd, SymbolTable *st);
void makeStructTableStmt(STMT *stmt, SymbolTable *st);
STRUCT *addToStructTable(TYPE *type, char *name, SymbolTable *st);
STRUCT *getFromStructTable(char *id);
void initStructTable();
void printStructTable();
#endif