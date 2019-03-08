#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "type.h"

SymbolTable *st;

void typeProgram(PROG *prog, SymbolTable *st) {
	if (prog != NULL) 
		typeDeclarations(prog->root_decl, st);
}

void typeDeclarations(DECL *dcl, SymbolTable *st) {
	if (dcl != NULL) {
		switch (dcl->kind) {
			case dk_var:
				typeVarDecl(dcl->val.varSpecs, st);
				break;
			case dk_short:
				typeShortDecl(dcl->val.shortSpecs, st);
				break;
			case dk_type:
				typeTypeDecl(dcl->val.typeSpecs, st);
				break;
			case dk_func:
				typeFuncDecl(dcl->val.funcDecl, st);
				break;
		}
		typeDeclarations(dcl->next, st);
	}
}

// TODO: Implement
void typeVarDecl(VAR_SPECS *vs, SymbolTable *st) {
	if (vs != NULL) {

	}
}

// TODO: Implement
void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st) {
	if (ss != NULL) {

	}
}

// TODO: Implement
void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st) {
	if (ts != NULL) {

	}
}

// TODO: Implement
void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st) {
	if (fd != NULL) {

	}
}

// TODO: Implement
void typeStmt(STMT *stmt, SymbolTable *st) {
	if (stmt != NULL) {

	}
}

// TODO: Implement
void typeExp(EXP *exp, SymbolTable *st) {
	if (exp != NULL) {

	}
}
