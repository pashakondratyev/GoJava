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

// void codeProgram(PROG *prog, SymbolTable *st, char *filename) {
// 	outputFile = fopen(filename, "w+");
//   	codePackage(prog->package);
//   	codeSetup();
//   	codeDeclarations(prog->root_decl, st, 1);
//   	codeComplete();
//   	fclose(outputFile);
// }


int indexLastForwardSlash(char *str){
	int index = -1;
  	for(int i = 0; i <= strlen(str); i++) {
  		if(str[i] == '/')  
  			index = i;	
	}
   	return index;
 }

void codeProgram(PROG *prog, SymbolTable *st, char *inputFileName) {
	// create file in proper package directory, with proper file name
	char *outputFileName = malloc((strlen(inputFileName)+6) *sizeof(char));
	strncpy(outputFileName, inputFileName, strlen(inputFileName));
	sprintf(outputFileName, "%s.java", inputFileName);
	char *packageName = prog->package->name;
	char *filePath = malloc((strlen(outputFileName)+strlen(packageName)+3)*sizeof(char));
	int index = indexLastForwardSlash(outputFileName);
	if (index == -1) {
		mkdir(packageName, 0777);
		sprintf(filePath, "%s/%s", packageName, outputFileName);
	} else {
		char *pathBeforeFile = malloc(strlen(outputFileName)*sizeof(char));
		strncpy(pathBeforeFile, outputFileName, index);
		char *fileName = &outputFileName[index+1];
		char *directory = malloc((strlen(outputFileName)+strlen(packageName)+1)*sizeof(char));
		sprintf(directory, "%s/%s", pathBeforeFile, packageName);
		mkdir(directory, 0777);
		sprintf(filePath, "%s/%s/%s",pathBeforeFile, packageName, fileName);

	}
	printf("%s\n", filePath);

	outputFile = fopen(filePath, "w");
	if (outputFile == NULL) {
 		fprintf(stderr, "Can't open output file %s!\n", filePath);
  		exit(1);
	}

  	codePackage(prog->package);

  	index = indexLastForwardSlash(inputFileName);
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