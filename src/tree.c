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

DECL *makeDecls(DECL *declHead, DECL *next) {
	if (next == NULL) {
		//printf("ERROR: Logical error in makeDecls.\n");
	}

	if (declHead == NULL) {
		declHead = next;
	} 
	else {
		DECL *cur = declHead;
		while (cur->next != NULL) {
			cur = declHead->next;
		}
		cur->next = next;
	}
	return declHead;
}

DECL *makeFunctionDcl(char *name, PARAM_LIST *args, STMT_LIST *body, TYPE *returnType, int lineno) {
	FUNC_DECL *func = malloc(sizeof(FUNC_DECL));
	func->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(func->name, name);
	func->params = args;
	func->body = body;
	func->returnType = returnType;

	DECL *decl = malloc(sizeof(DECL));
	decl->lineno = lineno;
	decl->kind = dk_func;
	decl->val.funcDecl = func;

	return decl;
}