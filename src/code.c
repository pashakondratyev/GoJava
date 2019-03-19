#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "code.h"

void codeProgram(PROG *prog, SymbolTable *st) {
  if (prog != NULL) {
  	codePackage(prog->package);
  	// setup classes and other defaults
  	codeDeclarations(prog->root_decl, st);
  }
}

void codePackage(PACKAGE *package) {
	if (package != NULL) {
		printf("package %s\n\n", package->name);

	} else {
		printf("LOGIC ERROR: package is null in codegen.\n");
	}
}

void codeDeclarations(DECL *dcl, SymbolTable *st) {
	// TODO: implement
}