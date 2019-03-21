#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "symbol.h"
#include "code.h"

FILE *outputFile;

void writeTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    fprintf(outputFile, "\t");
  }
}

int indexLastForwardSlash(char *str){
	int index = -1;
  	for(int i = 0; i <= strlen(str); i++) {
  		if(str[i] == '/')  
  			index = i;	
	}
   	return index;
 }

void codeProgram(PROG *prog, SymbolTable *st, char *inputFileName) {
	// create file name and open it
	char *outputFileName = malloc((strlen(inputFileName)+6) *sizeof(char));
	strncpy(outputFileName, inputFileName, strlen(inputFileName));
	sprintf(outputFileName, "%s.java", inputFileName);
	outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL) {
 		fprintf(stderr, "Can't open output file %s!\n", outputFileName);
  		exit(1);
	}

	// ignore package declaration

	// set class name as the file name excluding the path
  	int index = indexLastForwardSlash(inputFileName);
  	codeSetup(&inputFileName[index+1]);

  	codeDeclarations(prog->root_decl, st, 1);
  	codeComplete();
  	fclose(outputFile);
}



// setup classes and other defaults
void codeSetup(char *className) {
	// TODO: complete
	// class name must match file name
	fprintf(outputFile, "public class %s {\n", className);
}

// complete class definition
void codeComplete() {
	// TODO: complete
	fprintf(outputFile, "\tpublic static void main(String[] args) {}\n");
	fprintf(outputFile, "}\n");
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