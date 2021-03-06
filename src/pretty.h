#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"
void prettyPrint(PROG *prog);

void prettyPrintPackage(PACKAGE *package);

void prettyPrintDecl(DECL *decl, int tabCount);
void prettyPrintStmt(STMT *stmt, int tabCount);
void prettyPrintExp(EXP *exp);
void prettyPrintType(TYPE *type, int tabCount);
void prettyPrintCase(CASE_CLAUSE *c, int tabCount);

void prettyPrintAssignOp(AssignOpKind op);
void prettyPrintSimpleStmt(STMT *stmt);
void prettyPrintExpList(EXP_LIST *exp_list);
void prettyPrintParamList(PARAM_LIST *param_list);
void prettyPrintIdList(ID_LIST *id_list);

char *getEscapedAsString(char c);
void printTab(int tabCount);
#endif
