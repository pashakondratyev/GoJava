#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeDeclarations.h"

#include "code.h"
#include "codeExpressions.h"
#include "codeIdentifiers.h"
#include "codeStatements.h"
#include "codeStructs.h"
#include "codeTypes.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

void codeDeclarations(DECL *dcl, SymbolTable *st, IdentifierTable *it, int tabCount) {
  if (dcl != NULL) {
    if (DEBUG) printf("Code Declaration: %d\n", dcl->kind);
    switch (dcl->kind) {
      case dk_var:
        codeVarDecl(dcl->val.varSpecs, st, it, tabCount);
        break;
      case dk_short:
        codeShortDecl(dcl->val.shortSpecs, st, it, tabCount);
        break;
      case dk_type:
        // ignore type declarations
        break;
      case dk_func:
        codeFuncDecl(dcl->val.funcDecl, st, it, tabCount);
        break;
    }
    codeDeclarations(dcl->next, st, it, tabCount);
  }
}

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, IdentifierTable *it, int tabCount) {
  if (vs != NULL && vs->declared != -1) {
    char *type = javaTypeString(vs->type, st, NULL);
    char *constructor;
    if (vs->exp != NULL) {
      constructor = javaTypeStringConstructor(vs->type, st, NULL);
    } else {
      constructor = javaTypeStringDefaultConstructor(vs->type, st, NULL);
    }
    //writeTab(tabCount);
    if (tabCount==1) fprintf(outputFile, "\tpublic static ");

    if (strcmp(vs->id, "_") == 0) {
      fprintf(outputFile, "%s %s_%d = new %s", type, prefix("blank"), blankVar, constructor);
      if(vs->exp == NULL){
        fprintf(outputFile, ";");
      } else {
        fprintf(outputFile, "(");
        codeExp(vs->exp, st, it, tabCount);
        fprintf(outputFile, ")");
        fprintf(outputFile, ";"); 
      }
      blankVar++;
    } else {
      IDENTIFIER *i = addIfNotInTable(vs->id, it);
      i->identifier = " ";
      char identifier[1024];
      sprintf(identifier, "%s_%d", prefix(vs->id), i->scopeCount);
      TYPE *resolvedType = typeResolve(vs->type, st);
      if (vs->exp != NULL && resolvedType->kind == tk_array) {
        fprintf(outputFile, "%s %s_temp_%d = ", type, prefix(vs->id), i->scopeCount);
        codeExp(vs->exp, st, it, tabCount);
        fprintf(outputFile, ";\n");
        writeTab(tabCount);
      }
      fprintf(outputFile, "%s %s = new %s", type, identifier, constructor);
      if (resolvedType->kind == tk_array && tabCount == 1) { //Declaring array in top level
        fprintf(outputFile, ";\n");
        if(vs->exp){
          char source[1000];
          sprintf(source, "%s_temp_%d", prefix(vs->id), i->scopeCount);
          codeCopyArrayBuffer(initArraysBuffer + strlen(initArraysBuffer), identifier, source, "", resolvedType, st, tabCount + 1);
        } else{
          codeZeroOutArrayBuffer(initArraysBuffer + strlen(initArraysBuffer), identifier, "", resolvedType, st, tabCount + 1);
        }
      } else if (resolvedType->kind == tk_array && vs->exp == NULL) { //Declaring empty array
        fprintf(outputFile, ";\n");
        writeTab(tabCount);
        codeZeroOutArray(identifier, "", resolvedType, st, tabCount);
      } else if (resolvedType->kind == tk_array && vs->exp != NULL) { //Declaring new array(pass by value)
        fprintf(outputFile, ";\n");
        writeTab(tabCount);
        char source[1024];
        sprintf(source, "%s_temp_%d", prefix(vs->id), i->scopeCount);
        codeCopyArray(identifier, source, "", resolvedType, st, tabCount);
      } else if(vs->exp == NULL){ //Declaring without expression
        fprintf(outputFile, ";");
      } else {
        fprintf(outputFile, "(");
        codeExp(vs->exp, st, it, tabCount);
        fprintf(outputFile, ")");
        fprintf(outputFile, ";"); 
      }
      i->identifier = vs->id;
    }
    fprintf(outputFile, "\n");
    if(vs->next) writeTab(tabCount);
    codeVarDecl(vs->next, st, it, tabCount);
  }
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, IdentifierTable *it, int tabCount) {
  int tempIdCount = identifierCount;
  // Want to save rhs as temps
  for (SHORT_SPECS *temp = ss; temp; temp = temp->next) {
    char *type = javaTypeString(temp->lhs->type, st, NULL);
    char *constructor = javaTypeStringConstructor(temp->lhs->type, st, NULL);
    if (strcmp(temp->lhs->val.id, "_") == 0) {
      type = javaTypeString(temp->rhs->type, st, NULL);
      fprintf(outputFile, "%s %s_%d = %s", type, prefix("blank"), blankVar, constructor);
      blankVar++;
    } else {
      fprintf(outputFile, "%s %s_temp_%d = ", type, prefix(temp->lhs->val.id), identifierCount);
      identifierCount++;
    }
    // fprintf(outputFile, "%s %s_temp_%d = ", type, prefix(temp->lhs->val.id), i->scopeCount);
    codeExp(temp->rhs, st, it, tabCount);
    if(temp->rhs->type->kind == tk_struct){
      fprintf(outputFile, ".copy()");
    }
    fprintf(outputFile, ";\n");
    writeTab(tabCount);
  }
  int printLine = 0;
  for (SHORT_SPECS *temp = ss; temp; temp = temp->next) {
    if (strcmp(temp->lhs->val.id, "_") == 0) {
      continue;
    }
    if (printLine) {
      fprintf(outputFile, "\n");
      writeTab(tabCount);
    }
    char *type = javaTypeString(temp->lhs->type, st, NULL);
    char *constructor = javaTypeStringConstructor(temp->lhs->type, st, NULL);

    IDENTIFIER *i;
    if(!temp->declared){
      i = addIfNotInTable(temp->lhs->val.id, it);
    } else {
      i = getFromIdentifierTable(temp->lhs->val.id, it); 
    }
    char source[1024];
    char target[1024];
    sprintf(target, "%s_%d", prefix(temp->lhs->val.id), i->scopeCount);
    sprintf(source, "%s_temp_%d", prefix(temp->lhs->val.id), tempIdCount);
    tempIdCount++;
    if(!temp->declared){
      fprintf(outputFile, "%s ", type);
    }
    fprintf(outputFile, "%s = ", target);
    if(temp->lhs->type->kind == tk_array){
      fprintf(outputFile, "new %s;\n", constructor);
      writeTab(tabCount);
      codeCopyArray(target, source, "", temp->lhs->type, st, tabCount);
    } else{
      fprintf(outputFile, "%s;", source);
    }
    // fprintf(outputFile, "%s %s_%d = ", type, prefix(temp->lhs->val.id), i->scopeCount);
    // fprintf(outputFile, "%s_temp_%d;", prefix(temp->lhs->val.id), i->scopeCount);
    if (ss->next != NULL) {
      printLine = 1;
    }
  }
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, IdentifierTable *it, int tabCount) {
  char BUFFER[1024];
  char methodName[1024];
  // If this is not a reference we need to handle this specially
  if (DEBUG) printf("Processing header %s\n", fd->name);
  if (strcmp(fd->name, "_") == 0) {
    if (DEBUG) printf("Blank function\n");
    return;
  } else if (strcmp(fd->name, "init") == 0) {
    fprintf(outputFile, "\n\tpublic static void %s_%d (", prefix(fd->name), numInitFunc);
    numInitFunc++;
  } else {
    char *returnTypeString = fd->returnType == NULL ? "void" : javaTypeString(fd->returnType, st, NULL);
    fprintf(outputFile, "\n\tpublic static %s %s (", returnTypeString, prefix(fd->name));
    for (PARAM_LIST *temp = fd->params; temp; temp = temp->next) {
      if (strcmp(temp->id, "_") == 0) {
        fprintf(outputFile, "%s %s_%d", javaTypeString(temp->type, st, NULL), prefix("blank"), blankVar);
        blankVar++;
      } else {
        IDENTIFIER *i = addIfNotInTable(temp->id, it);
        fprintf(outputFile, "%s %s_%d", javaTypeString(temp->type, st, NULL), prefix(i->identifier), i->scopeCount);
      }
      if (temp->next) {
        fprintf(outputFile, ", ");
      }
    }
  }
  // print args
  fprintf(outputFile, ") {\n");
  if (DEBUG) printf("Finished converting header\n");
  it = scopeIdentifierTable(it);
  for(PARAM_LIST *temp = fd->params; temp; temp = temp->next){
    IDENTIFIER *i = getFromIdentifierTable(temp->id, it);
    switch (temp->type->kind)
    {
      case tk_array:
        writeTab(tabCount);
        fprintf(outputFile, "%s %s_declaration_copy_%d", javaTypeString(temp->type, st, NULL), prefix(i->identifier), i->scopeCount);
        fprintf(outputFile, "= new %s;\n", javaTypeStringConstructorArray(temp->type, st, NULL));
        char source[1024];
        char target[1024];
        sprintf(target, "%s_declaration_copy_%d", prefix(i->identifier), i->scopeCount);
        sprintf(source, "%s_%d", prefix(i->identifier), i->scopeCount);
        codeCopyArray(target, source, "", temp->type, st, tabCount);
        writeTab(tabCount);
        fprintf(outputFile, "%1$s_%2$d = %1$s_declaration_copy_%2$d;\n", prefix(i->identifier), i->scopeCount);
        break;
      case tk_struct:
      case tk_slice: 
        writeTab(tabCount);
        fprintf(outputFile, "%1$s_%2$d = %1$s_%2$d.copy();\n", prefix(i->identifier), i->scopeCount);
      default:
        break;
    }
  }
  writeTab(tabCount + 1);
  codeStmt(fd->body, st, it, tabCount + 1, false, NULL);
  if (DEBUG) printf("Finished converting body\n");
  fprintf(outputFile, "\n");
  writeTab(tabCount);
  fprintf(outputFile, "}\n");
  // TODO: implement
}
