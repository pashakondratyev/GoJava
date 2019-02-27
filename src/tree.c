#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

// deal with basic types in typechecker phase


PROG *makeProg(PACKAGE *package, DECL *decl, int lineno) {
	PROG *p = malloc(sizeof(PROG));
	p->lineno = lineno;
	p->package = package;
	p->root_decl = decl;
	return p;
}

PACKAGE *makePackage(char *name, int lineno) {
	PACKAGE *p = malloc(sizeof(PACKAGE));
	p->lineno = lineno;
	p->name = malloc((strlen(name)+1)*sizeof(char));
    strcpy(p->name, name);
    return p;
}

DECL *makeDecls(DECL *firstDecl, DECL *declList) {
	if (firstDecl == NULL) {
		//printf("ERROR: Logical error in makeDecls.\n");
	}
	firstDecl->next = declList;
	return firstDecl;
}

DECL *makeFuncDecl(char *name, SIGNATURE *signature, STMT_LIST *body, int lineno) {
	FUNC_DECL *func = malloc(sizeof(FUNC_DECL));
	func->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(func->name, name);
	func->params = signature->params;
	func->body = body;
	func->returnType = signature->returnType;

	DECL *decl = malloc(sizeof(DECL));
	decl->lineno = lineno;
	decl->kind = dk_func;
	decl->val.funcDecl = func;

	return decl;
}

SIGNATURE *makeSignature(PARAM_LIST *params, TYPE *type) {
	SIGNATURE *s = malloc(sizeof(SIGNATURE));
	s->params = params;
	s->returnType = type;
	return s;
}

STMT_LIST *makeStmtList(STMT *firstStmt, STMT_LIST *stmtList) {
	if (firstStmt == NULL) {
		//printf("ERROR: Logical error in makeStmtList\n");
	}
	STMT_LIST *head = malloc(sizeof(STMT_LIST));
	head->stmt = firstStmt;
	head->next = stmtList;
	return head;
}

TYPE_SPECS *makeTypeSpecList(TYPE_SPECS *specHead, TYPE_SPECS *nextSpec) {
	return NULL;
}