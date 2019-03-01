#ifndef WEED_H
#define WEED_H

#include "tree.h"

void weedProgram(PROG *prog);

void weedDeclaration(DECL *decl);
void weedStatement(STMT *stmt);
void weedFunction(FUNC_DECL *func_decl);

int weedSwitchReturn(STMT *stmt);
int isShortSpecIdentifierType(SHORT_SPECS *shorts_specs);

#endif 