#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeDeclarations.h"

#include "code.h"
#include "codeTypes.h"
#include "codeStructs.h"
#include "codeStatements.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0
// Struct Table Functions


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
    if(vs != NULL){
        char *type = javaTypeString(vs->type, st, NULL);
        char *constructor = javaTypeStringConstructor(vs->type, st, NULL);
        fprintf(outputFile,"%s %s = new %s(", type, prefix(vs->id), constructor);
        if(vs->exp != NULL){
            codeExp(vs->exp, st, tabCount);
        }
        fprintf(outputFile,");");
        if(vs->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
        }
        codeVarDecl(vs->next, st, tabCount);
    }
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount) {
  // TODO: implement
}

void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount) {
  // TODO: implement
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
  char BUFFER[1024];
  char methodName[1024];
  // If this is not a reference we need to handle this specially
  if (strcmp(fd->name, "init") == 0){
    fprintf(outputFile, "\tpublic static void %s_%d (", prefix(fd->name), numInitFunc);
    numInitFunc++;
  } else {
    char *returnTypeString = fd->returnType == NULL ? "void" : javaTypeString(fd->returnType, st, NULL);
    fprintf(outputFile, "\tpublic static %s %s (", returnTypeString, prefix(fd->name));
    for (PARAM_LIST *temp = fd->params; temp; temp = temp->next) {
      fprintf(outputFile, "%s %s", javaTypeString(temp->type, st, NULL), temp->id);
      if (temp->next) {
        fprintf(outputFile, ", ");
      }
    }
  }

  // print args
  fprintf(outputFile, ") ");
  codeStmt(fd->body, st, tabCount);
  fprintf(outputFile, "\n");
  // TODO: implement
}