#ifndef CODE_TYPES_H
#define CODE_TYPES_H

#include "code.h"
#include "tree.h"
#include "symbol.h"

char *javaTypeString(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringConstructor(TYPE *type, SymbolTable *st, char *name);
char *javaTypeStringDefaultConstructor(TYPE *type, SymbolTable *st, char *name);
char *javaConstructorForBasicTypes(TYPE *type, SymbolTable *st);
char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);
char *getRecTypeString(char *BUFFER, TYPE *type, SymbolTable *st, char *name);

#endif
