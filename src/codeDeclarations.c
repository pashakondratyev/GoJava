#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeDeclarations.h"

#include "code.h"
#include "codeTypes.h"
#include "codeIdentifiers.h"
#include "codeExpressions.h"
#include "codeStructs.h"
#include "codeStatements.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0
// Struct Table Functions


void codeDeclarations(DECL *dcl, SymbolTable *st, IdentifierTable *it, int tabCount) {
  if (dcl != NULL) {
    switch (dcl->kind) {
      case dk_var:
        codeVarDecl(dcl->val.varSpecs, st, it, tabCount);
        break;
      case dk_short:
        codeShortDecl(dcl->val.shortSpecs, st, it, tabCount);
        break;
      case dk_type:
        codeTypeDecl(dcl->val.typeSpecs, st, it, tabCount);
        break;
      case dk_func:
        codeFuncDecl(dcl->val.funcDecl, st, it, tabCount);
        break;
    }
    codeDeclarations(dcl->next, st, it, tabCount);
  }
}

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, IdentifierTable *it, int tabCount) {
    if(vs != NULL){
        char *type = javaTypeString(vs->type, st, NULL);
        char *constructor = javaTypeStringConstructor(vs->type, st, NULL);

        IDENTIFIER *i = getFromIdentifierTable(vs->id, it);
        if(i != NULL){
            int scopeCount = i->scopeCount + 1;
            i = addToIdentifierTable(vs->id, scopeCount, it);
        } else{
            i = addToIdentifierTable(vs->id, 1, it);
        }
        i->identifier = "_";

        char identifier[1024];
        sprintf(identifier, "%s_%d", prefix(vs->id), i->scopeCount);

        fprintf(outputFile,"%s %s = new %s(", type, identifier, constructor);
        if(vs->exp != NULL){
            codeExp(vs->exp, st, it, tabCount);
        }
        fprintf(outputFile,");");
        if(vs->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
        }
        i->identifier = vs->id;
        codeVarDecl(vs->next, st, it, tabCount);
    }
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, IdentifierTable *it, int tabCount) {
    if(ss != NULL){
        char *type = javaTypeString(ss->lhs->type, st, NULL);
        char *constructor = javaTypeStringConstructor(ss->lhs->type, st, NULL);
        fprintf(outputFile,"%s %s = new %s(", type, prefix(ss->lhs->val.id), constructor);
        codeExp(ss->rhs, st, it, tabCount);
        fprintf(outputFile,");");
        if(ss->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
        }
        codeShortDecl(ss->next, st, it, tabCount);
    }
  // TODO: implement
}

void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, IdentifierTable *it, int tabCount) {
  // TODO: implement
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, IdentifierTable *it, int tabCount) {
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
  codeStmt(fd->body, st, it, tabCount);
  fprintf(outputFile, "\n");
  // TODO: implement
}