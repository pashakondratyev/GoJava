#ifndef WEED_H
#define WEED_H

#include "tree.h"

void weedProgram(PROG *prog);

void weedDeclaration(DECL *decl);
void weedStatement(STMT *stmt);
void weedFunction(FUNC_DECL *func_decl, int lineno);

void weedSwitchDefault(STMT *stmt);
void weedBreakCont(STMT *stmt, int allow_cont, int allow_break);
void weedSwitchBreak(STMT *stmt, int allow_cont);
int weedSwitchReturns(CASE_CLAUSE_LIST *c);
int isShortSpecIdentifierType(SHORT_SPECS *shorts_specs);

#endif 
