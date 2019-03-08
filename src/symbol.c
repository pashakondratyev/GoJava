#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "pretty.h"
#include "tree.h"

extern SymbolTable *programSymbolTable;

int tabCount = 0;
SymbolTableMode mode;

int Hash(char *str) {
  unsigned int hash = 0;
  while (*str) hash = (hash << 1) + *str++;
  return hash % HashSize;
}

SymbolTable *initSymbolTable() {
  SymbolTable *t = (SymbolTable*)malloc(sizeof(SymbolTable));
  for (int i = 0; i < HashSize; i++) {
    t->table[i] = NULL;
  }
  t->parent = NULL;
  return t; 
}

SymbolTable *scopeSymbolTable(SymbolTable *s) {
  SymbolTable *t = initSymbolTable();
  t->parent = s;
  return t;
}

void openScope(){
  if(mode == SymbolTablePrint){
    printTab(tabCount);
    printf("{\n"); 
  }
  tabCount++;
}

void closeScope(){
  tabCount--;
  if(mode == SymbolTablePrint){
    printTab(tabCount);
    printf("}\n");
  }
}

//TODO: Type has lineno consider refactoring
SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, TYPE *type, int lineno) {
  int i = Hash(identifier);
  for(SYMBOL *s = t->table[i]; s; s = s->next){
    if(strcmp(s->name, identifier) == 0){
      fprintf(stderr, "Error: (line %d) \"%s\" is already declared\n", lineno, identifier);
      exit(1);
    }
  }

  SYMBOL *s = (SYMBOL*)malloc(sizeof(SYMBOL));
  s->name = strdup(identifier);
  s->type = type;
  s->kind = kind;
  s->next = t->table[i];
  t->table[i] = s;
  return s;
}

void putTypeDecl(SymbolTable *st, TYPE_SPECS *ts, int lineno){
  while(ts != NULL){
    if(strcmp(ts->name, "_") == 0){
      ts = ts->next;
      continue;
    }
    putSymbol(st, dk_type, ts->name, ts->type, lineno);
    // Line 0 is the types loaded into the symbol table initially
    printTab(tabCount);
    if(lineno == 0){
      printf("%s [type] = %s", ts->name, ts->name);
    } else{
      printf("%s [type] = %s ->", ts->name, ts->name);
      printType(ts->type);
    }
    ts = ts->next;     
    printf("\n");
  }
}

// Functions are top level decl 
void putFuncDecl(SymbolTable *st, FUNC_DECL *fd, int lineno){
  printTab(tabCount);
  printf("%s [function] = ", fd->name);
  if(strcmp(fd->name, "_") == 0){
    printf("<unmapped>");
  }
  else{
    putSymbol(st, dk_func, fd->name, NULL, lineno);
    printParamList(fd->params);
    printf(" -> ");
    if(fd->returnType != NULL){
      printType(fd->returnType);
    }
    else{
      printf("void");
    }

  }
  printf("\n");
  STMT *pl = paramListToStmt(fd->params, lineno);  
  pl = combineStmt(pl, fd->body, lineno);
  createScope(pl, st);
}

void putShortDecl(SymbolTable *st, SHORT_SPECS *ss, int lineno){
  while(ss != NULL){
    if(strcmp(ss->lhs->val.id, "_") == 0){
      ss=ss->next;
      continue;
    }
    //Even though storing NULL for type, can check if dk_short
    putSymbol(st, dk_short, ss->lhs->val.id, NULL, lineno);

    printTab(tabCount);
    prettyPrintExp(ss->lhs);
    printf(" [variable] = <infer>\n");
    ss=ss->next;
  }
}

void putVarDecl(SymbolTable *st, VAR_SPECS *vs, int lineno){
  while(vs != NULL){
    if(strcmp(vs->id,"_") == 0){
      vs = vs->next;
      continue;
    }
    putSymbol(st, dk_var, vs->id, vs->type, lineno);
    printTab(tabCount);
    printf("%s [variable] = ", vs->id);
    if(vs->type == NULL){
      printf("<infer>");
    }
    else{
      printType(vs->type);
    }
    printf("\n");
    vs = vs->next;
  }
}

/* Scopes the symbol table and adjusts tab count as necessary */
void createScope(STMT *stmt, SymbolTable *st){
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  symTypesStatements(stmt, scope);
  closeScope();
}

/* Takes care of properly scoping an if statment and the else blocks associated with it */
void createIfStmtScope(STMT *stmt, SymbolTable *st){
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  if(stmt->val.ifStmt.simpleStmt != NULL){
    symTypesStatements(stmt->val.ifStmt.simpleStmt, scope);
  }
  createScope(stmt->val.ifStmt.body, scope);
  if(stmt->val.ifStmt.elseStmt != NULL){
    createScope(stmt->val.ifStmt.elseStmt, scope);
  }
  closeScope();
}

void createForStmtScope(STMT *stmt, SymbolTable *st){
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  if(stmt->val.forStmt.forClause != NULL && stmt->val.forStmt.forClause->init != NULL){
    symTypesStatements(stmt->val.forStmt.forClause->init, scope);
  }
  createScope(stmt->val.forStmt.body, scope);
  closeScope();
}

void createSwitchStmtScope(STMT *stmt, SymbolTable *st){
  SymbolTable *scope = scopeSymbolTable(st);

  openScope();
  if(stmt->val.switchStmt.simpleStmt != NULL){
    symTypesStatements(stmt->val.switchStmt.simpleStmt, scope);
  } 

  CASE_CLAUSE_LIST *ccl = stmt->val.switchStmt.caseClauses;
  while(ccl != NULL){
    STMT *newBlock;
    if(ccl->clause->kind == ck_case){
      newBlock = makeBlockStmt(ccl->clause->val.caseClause.clauses, stmt->lineno); 
    }
    else{ //Else is defauly
      newBlock = makeBlockStmt(ccl->clause->val.defaultClauses, stmt->lineno);
    }
    createScope(newBlock, st);
    ccl = ccl->next;
  }
  closeScope();
}

SYMBOL *getSymbol(SymbolTable *t, char *name) {
  int i = Hash(name);
  // Check the current scope
  for (SYMBOL *s = t->table[i]; s; s = s->next) {
    if (strcmp(s->name, name) == 0) return s;
  }
  // Check for existence of a parent scope
  if (t->parent == NULL) return NULL;
  // Check the parent scopes
  return getSymbol(t->parent, name);
}

void symProgram(PROG *root, SymbolTableMode m){
  mode = m; 
  programSymbolTable = initSymbolTable();
  openScope();
  symTypesDefaults(programSymbolTable);
  openScope();
  //  Default types should be one scope above the top level decls
  SymbolTable *topLevel = scopeSymbolTable(programSymbolTable);
  symTypesDeclarations(root->root_decl, topLevel);
  closeScope();
  closeScope();
}

void symTypesDeclarations(DECL *decl, SymbolTable *st){
  // We could have passed the entire declaration into the put symbol tabl
  while(decl != NULL){
    switch(decl->kind){
      case dk_type:
        putTypeDecl(st, decl->val.typeSpecs, decl->lineno);
        break;
      case dk_func:
        putFuncDecl(st, decl->val.funcDecl, decl->lineno);
        break;
      case dk_var:
        putVarDecl(st, decl->val.varSpecs, decl->lineno);
        break;
      case dk_short:
        putShortDecl(st, decl->val.shortSpecs, decl->lineno);
        break;
    }
    decl=decl->next;
  }
}

void symTypesStatements(STMT *stmt, SymbolTable *st){
  STMT_LIST *sl;
  CASE_CLAUSE_LIST *ccl;
  SymbolTable *nextScope;
  switch(stmt->kind){
    case sk_block:
      sl = stmt->val.block;
      while(sl != NULL){
        symTypesStatements(sl->stmt, st);
        sl = sl->next;
      }
      break;
    case sk_if:
      createIfStmtScope(stmt, st);
      break;
    case sk_switch:
      createSwitchStmtScope(stmt, st);
      break;
    case sk_for:
      createForStmtScope(stmt, st);
      break;
    case sk_else:
      symTypesStatements(stmt->val.elseBody, st);
      break;
    case sk_decl:
    case sk_shortDecl:
      symTypesDeclarations(stmt->val.decl, st);
    default: break;
  }
}

void symTypesDefaults(SymbolTable *st){
  //int 
  TYPE *t = makeType((char*)"int", 0);
  t->kind = tk_int;
  TYPE_SPECS *ts = makeTypeSpec((char*)"int", t);
  putTypeDecl(st, ts, 0);
  //float64
  t = makeType((char*)"float64", 0);
  t->kind = tk_float;
  ts = makeTypeSpec((char*)"float64", t);
  putTypeDecl(st, ts, 0);
  //rune 
  t = makeType((char*)"rune", 0);
  t->kind = tk_rune;
  ts = makeTypeSpec((char*)"rune", t);
  putTypeDecl(st, ts, 0);

  //string
  t = makeType((char*)"string", 0);
  t->kind = tk_string;
  ts = makeTypeSpec((char*)"string", t);
  putTypeDecl(st, ts, 0);
  //bool
  t = makeType((char*)"bool", 0);
  t->kind = tk_boolean;
  ts = makeTypeSpec((char*)"bool", t);
  putTypeDecl(st, ts, 0);
  //true
  printTab(tabCount);
  putSymbol(st, dk_var, (char*)"true", t, 0); 
  printf("true [constant] = bool\n");
  //false
  printTab(tabCount);
  putSymbol(st, dk_var, (char*)"false", t, 0); 
  printf("false [constant] = bool\n");
}

/* Converts a list of parameters into a list of decl statments */
STMT *paramListToStmt(PARAM_LIST *pl, int lineno){
  VAR_SPECS *vs = NULL; 
  VAR_SPECS *last = NULL;
  while(pl != NULL){
    if(vs == NULL){
      vs = (VAR_SPECS*)malloc(sizeof(VAR_SPECS));
      last = vs;
    }
    else{
      last->next = (VAR_SPECS*)malloc(sizeof(VAR_SPECS)); 
      last = last->next;
    }
    last->id = pl->id;
    last->type = pl->type;
    pl = pl->next;
  }
  DECL *d = makeVarDecl(vs, lineno);
  return makeDeclStmt(d, lineno);
}

/* Combines two statements into a single statment */
STMT *combineStmt(STMT *s1, STMT *s2, int lineno){
  STMT_LIST *sl = makeStmtList(s2, NULL);
  sl = makeStmtList(s1, sl);
  return makeBlockStmt(sl, lineno);
}

void printType(TYPE *type){
  FIELD_DECLS *d;
  switch(type->kind){
    case tk_array:
      printf("[");
      printf("%d", type->val.array.size);
      printf("]");
      printType(type->val.array.elemType);
      break;
    case tk_slice:
      printf("[]%s", type->val.sliceType->val.name);
      break;
    case tk_struct:
      printf("struct {");
      d = type->val.structFields;
      while(d != NULL){
        printf("%s ", d->id);
        printType(d->type);
        d = d->next;
      }
      printf("}");
      break;
    default:
      printf("%s", type->val.name);
  }
}

void printParamList(PARAM_LIST *pl){
  printf("(");
  while(pl != NULL){
    printType(pl->type);
    if(pl->next != NULL){
      printf(", ");
    }
    pl=pl->next;
  }
  printf(")");
}
