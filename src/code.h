#ifndef CODE_H
#define CODE_H

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
void makeStructTableStmt(STMT *stmt, SymbolTable *st);
STRUCT *addToStructTable(TYPE *type, char *name, SymbolTable *st);
STRUCT *getFromStructTable(char *id);
void initStructTable();
void printStructTable();

char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount);
void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount);
void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount);
void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount);

void codeStmt(STMT *stmt, SymbolTable *st, int tabCount);
void codeExp(EXP *exp, SymbolTable *st, int tabCount);
TYPE* resolveExpType(TYPE* type, SymbolTable *st);

void codeType(TYPE *type, SymbolTable *st, int tabcount);
char *codeStructType(char *BUFFER, FIELD_DECLS *fd, SymbolTable *st, STRUCT *s, char *name);
void codeStructTable();

char *javaTypeString(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name);
#endif