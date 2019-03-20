#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "code.h"

FILE *outputFile;

void codeProgram(PROG *prog, SymbolTable *st, char *filename) {
	outputFile = fopen(filename, "w+");
  	if (prog != NULL) {
  		codePackage(prog->package);
  		codeSetup();
  		codeDeclarations(prog->root_decl, st, 0);
  		codeComplete();
  	}
  	fclose(outputFile);
}

// setup classes and other defaults
void codeSetup() {
	// TODO: complete
}

// complete class 
void codeComplete() {
	// TODO: implement
}


void codePackage(PACKAGE *package) {
	if (package != NULL) {
		fprintf(outputFile, "package %s\n\n", package->name);

	} else {
		printf("LOGIC ERROR: package is null in codegen.\n");
	}
}

void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount) {
	if (dcl != NULL) {
    switch (dcl->kind) {
      case dk_var:
        codeVarDecl(dcl->val.varSpecs, st, tabCount);
        break;
      case dk_short:
        codeShortDecl(dcl->val.shortSpecs, st, tabCount);
        break;
      case dk_type:
        codeTypeDecl(dcl->val.typeSpecs, st, tabCount);
        break;
      case dk_func:
        codeFuncDecl(dcl->val.funcDecl, st, tabCount);
        break;
    }
    codeDeclarations(dcl->next, st, tabCount);
  }
}

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount) {
	// TODO: implement
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount) {
	// TODO: implement
}

void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount) {
	// TODO: implement
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
	// TODO: implement
}