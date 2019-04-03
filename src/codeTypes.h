#ifndef CODE_TYPES_H
#define CODE_TYPES_H

#include "code.h"
#include "tree.h"
#include "symbol.h"

char *javaTypeString(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringConstructorArray(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringDefaultConstructor(TYPE *type, SymbolTable *st, char *name);
char *javaConstructorForBasicTypes(TYPE *type, SymbolTable *st);
char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);
char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);
void codeZeroOutArray(char *identifier, char *index, TYPE *type, SymbolTable *st, int tabCount);
void codeCopyArray(char *target, char *source, char *index, TYPE *type, SymbolTable *st, int tabCount);
void codeCopyArrayBuffer(char *BUFFER, char *target, char *source, char *index, TYPE *type, SymbolTable *st, int tabCount);
void codeZeroOutArrayBuffer(char *BUFFER, char *identifier, char *index, TYPE *type, SymbolTable *st, int tabCount);
#endif
