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