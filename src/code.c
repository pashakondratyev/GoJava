#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "code.h"

FILE *outputFile;

void writeTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    fprintf(outputFile, "\t");
  }
}

void codeProgram(PROG *prog, SymbolTable *st, char *filename) {
	outputFile = fopen(filename, "w+");
  	codePackage(prog->package);
  	codeSetup();
  	codeDeclarations(prog->root_decl, st, 1);
  	codeComplete();
  	fclose(outputFile);
}

// setup classes and other defaults
void codeSetup() {
	// TODO: complete
	fprintf(outputFile, "public class GoLite {\n");
}

// complete class definition
void codeComplete() {
	// TODO: complete
	fprintf(outputFile, "}\n");
}


void codePackage(PACKAGE *package) {
	fprintf(outputFile, "package %s;\n\n", package->name);
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