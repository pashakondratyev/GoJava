#ifndef CODE_H
#define CODE_H

#include "codeStructs.h"
#include "tree.h"
#include "symbol.h"

int numInitFunc;
int identifierCount;
FILE *outputFile;

void writeTab(int tabCount);
char* prefix(char* str);

int indexLastForwardSlash(char *str);
void codeProgram(PROG *prog, SymbolTable *st, char *filename);
void codeSetup();
void codeComplete();
void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount);

char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount);
void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount);
void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount);
void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount);

void codeStmt(STMT *stmt, SymbolTable *st, int tabCount);
void codeExp(EXP *exp, SymbolTable *st, int tabCount);

void codeType(TYPE *type, SymbolTable *st, int tabcount);
char *codeStructType(char *BUFFER, FIELD_DECLS *fd, SymbolTable *st, STRUCT *s, char *name);
void codeStructTable();

char *javaTypeString(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name);
#endif