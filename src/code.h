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

void codeExp(EXP *exp, SymbolTable *st, int tabCount);
#endif