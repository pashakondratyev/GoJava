#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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


void codeDeclarations(DECL *dcl, SymbolTable *st, IdentifierTable *it, int tabCount) {
  if (dcl != NULL) {
    if(DEBUG) printf("Code Declaration: %d\n", dcl->kind);
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
    if(vs != NULL && vs->declared != -1){
        char *type = javaTypeString(vs->type, st, NULL);
        char *constructor = javaTypeStringConstructor(vs->type, st, NULL);

        if(strcmp(vs->id, "_") == 0){
          fprintf(outputFile, "%s %s_%d = new %s", javaTypeString(vs->type, st, NULL), prefix("blank"), blankVar, constructor);
          blankVar++;
        } else{
          IDENTIFIER *i = addIfNotInTable(vs->id, it); 
          i->identifier = " ";
          char identifier[1024];
          sprintf(identifier, "%s_%d", prefix(vs->id), i->scopeCount);
          fprintf(outputFile,"%s %s_%d = new %s", type, prefix(vs->id), i->scopeCount, constructor);
          i->identifier = vs->id;
        }
        if(typeResolve(vs->type, st)->kind != tk_array){
          fprintf(outputFile, "(");
        }
        if(vs->exp != NULL){
            codeExp(vs->exp, st, it, tabCount);
        } else {
          // TODO: code default values if needed
        }
        if(typeResolve(vs->type, st)->kind != tk_array){
          fprintf(outputFile,")");
        }
        fprintf(outputFile, ";");
        if(vs->next != NULL){
            fprintf(outputFile, "\n");
            writeTab(tabCount);
        }
        codeVarDecl(vs->next, st, it, tabCount);
    }
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, IdentifierTable *it, int tabCount) {
  int temp = identifierCount;
  //Want to save rhs as temps
  for(SHORT_SPECS *temp = ss; temp; temp = temp->next){
    char *type = javaTypeString(temp->lhs->type, st, NULL);
    char *constructor = javaTypeStringConstructor(temp->lhs->type, st, NULL);
    IDENTIFIER *i;
    if(strcmp(temp->lhs->val.id, "_") == 0){
      fprintf(outputFile, "%s %s_%d = %s", type, prefix("blank"), blankVar, constructor);
      blankVar++; 
      codeExp(temp->rhs, st, it, tabCount);
      fprintf(outputFile,";");
      if(ss->next != NULL){
        fprintf(outputFile, "\n");
        writeTab(tabCount);
      }
      continue;
    } else if(!temp->declared){
      i = addIfNotInTable(temp->lhs->val.id, it);
      i->identifier = " ";
    } else{
      i = getFromIdentifierTable(temp->lhs->val.id, it);
    }
    fprintf(outputFile, "%s %s_temp_%d = ", type, prefix(temp->lhs->val.id), i->scopeCount);
    codeExp(temp->rhs, st, it, tabCount);
    fprintf(outputFile,";\n");
    writeTab(tabCount);

    if(!temp->declared){
      i->identifier = temp->lhs->val.id;
    }
  }
  int printLine = 0;
  for(SHORT_SPECS *temp = ss; temp; temp = temp->next){
    if(strcmp(temp->lhs->val.id, "_") == 0){
      continue;
    }
    if(printLine){
      fprintf(outputFile, "\n");
      writeTab(tabCount); 
    }
    char *type = javaTypeString(temp->lhs->type, st, NULL);
    char *constructor = javaTypeStringConstructor(temp->lhs->type, st, NULL);

    IDENTIFIER *i = getFromIdentifierTable(temp->lhs->val.id, it);
    
    fprintf(outputFile, "%s %s_%d = ", type, prefix(temp->lhs->val.id), i->scopeCount);
    fprintf(outputFile, "%s_temp_%d;", prefix(temp->lhs->val.id), i->scopeCount);
    if(ss->next != NULL){
        printLine = 1;
    }
  }


  /*if(ss != NULL && strcmp(ss->lhs->val.id, "_") != 0){
    char *type = javaTypeString(ss->lhs->type, st, NULL);
    char *constructor = javaTypeStringConstructor(ss->lhs->type, st, NULL);

    IDENTIFIER *i = addIfNotInTable(ss->lhs->val.id, it);
    i->identifier = " ";
    
    fprintf(outputFile,"%s %s_%d = new %s(", type, prefix(ss->lhs->val.id), i->scopeCount, constructor);
    codeExp(ss->rhs, st, it, tabCount);
    fprintf(outputFile,");");
    if(ss->next != NULL){
        fprintf(outputFile, "\n");
        writeTab(tabCount);
    }
    i->identifier = ss->lhs->val.id;
  }
  if(ss != NULL){
    codeShortDecl(ss->next, st, it, tabCount);
  }*/
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, IdentifierTable *it, int tabCount) {
  char BUFFER[1024];
  char methodName[1024];
  // If this is not a reference we need to handle this specially
  if(DEBUG) printf("Processing header %s\n", fd->name);
  if (strcmp(fd->name, "_") == 0){
    if(DEBUG) printf("Blank function\n");
    return;
  } else if (strcmp(fd->name, "init") == 0){
    fprintf(outputFile, "\tpublic static void %s_%d (", prefix(fd->name), numInitFunc);
    numInitFunc++;
  } else {
    char *returnTypeString = fd->returnType == NULL ? "void" : javaTypeString(fd->returnType, st, NULL);
    fprintf(outputFile, "\tpublic static %s %s (", returnTypeString, prefix(fd->name));
    for (PARAM_LIST *temp = fd->params; temp; temp = temp->next) {
      if(strcmp(temp->id, "_") == 0){
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
  fprintf(outputFile, ") ");
  if(DEBUG) printf("Finished converting header\n");
  codeStmt(fd->body, st, it, tabCount, false, NULL);
  if(DEBUG) printf("Finished converting body\n");
  fprintf(outputFile, "\n");
  // TODO: implement
}
