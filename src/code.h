#ifndef CODE_H
#define CODE_H

#include "tree.h"
#include "symbol.h"

typedef struct STRUCT {
    char *structString;
    char *className;
    struct STRUCT *next;
} STRUCT;

typedef struct StructTable {
    STRUCT *table[HashSize];
} StructTable;

void writeTab(int tabCount);
char* prefix(char* str);

int indexLastForwardSlash(char *str);
void codeProgram(PROG *prog, SymbolTable *st, char *filename);
void codeSetup();
void codeComplete();
void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount);

void makeStructTable(DECL *decl, SymbolTable *st);
void makeStructTableVarDecl(VAR_SPECS *vs, SymbolTable *st);
void makeStructTableShortDecl(SHORT_SPECS *ss, SymbolTable *st);
void makeStructTableTypeDecl(TYPE_SPECS *ts, SymbolTable *st);
void makeStructTableFuncDecl(FUNC_DECL *fd, SymbolTable *st);

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount);
void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount);
void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount);
void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount);

void codeStmt(STMT *stmt, SymbolTable *st, TYPE *returnType, int tabCount);
void codeExp(EXP *exp, SymbolTable *st, int tabCount);

void codeType(TYPE *type, SymbolTable *st, int tabcount);
void codeStructType(FIELD_DECLS *fd, SymbolTable *st);

#endif