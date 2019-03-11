#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pretty.h"
#include "symbol.h"
#include "tree.h"

extern SymbolTable *programSymbolTable;

int tabCount = 0;
SymbolTableMode mode;

// TODO: Add expression check 
int Hash(char *str) {
  unsigned int hash = 0;
  while (*str) hash = (hash << 1) + *str++;
  return hash % HashSize;
}

SymbolTable *initSymbolTable() {
  SymbolTable *t = (SymbolTable *)malloc(sizeof(SymbolTable));
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

void openScope() {
  if (mode == SymbolTablePrint) {
    printTab(tabCount);
    printf("{\n");
  }
  tabCount++;
}

void closeScope() {
  tabCount--;
  if (mode == SymbolTablePrint) {
    printTab(tabCount);
    printf("}\n");
  }
}

// TODO: Type has lineno consider refactoring
SYMBOL *putSymbol(SymbolTable *t, DecKind kind, char *identifier, TYPE *type, int lineno) {
  int i = Hash(identifier);
  for (SYMBOL *s = t->table[i]; s; s = s->next) {
    if (strcmp(s->name, identifier) == 0) {
      fprintf(stderr, "Error: (line %d) \"%s\" is already declared\n", lineno, identifier);
      exit(1);
    }
  }

  SYMBOL *s = (SYMBOL *)malloc(sizeof(SYMBOL));
  s->name = strdup(identifier);
  s->type = type;
  s->kind = kind;
  s->next = t->table[i];
  t->table[i] = s;
  return s;
}

// TODO: probably should store Struct values
void putTypeDecl(SymbolTable *st, TYPE_SPECS *ts, int lineno) {
  while (ts != NULL) {
    if (strcmp(ts->name, "_") == 0) {
      ts = ts->next;
      continue;
    }
    putSymbol(st, dk_type, ts->name, ts->type, lineno);
    // Line 0 is the types loaded into the symbol table initially
    if (mode == SymbolTablePrint) {
      printTab(tabCount);
      if (lineno == 0) {
        printf("%s [type] = %s", ts->name, ts->name);
      } else {
        printf("%s [type] = %s ->", ts->name, ts->name);
        printType(ts->type);
      }
      printf("\n");
    }
    ts = ts->next;
  }
}

// Functions are top level decl
void putFuncDecl(SymbolTable *st, FUNC_DECL *fd, int lineno) {
  if(strcmp(fd->name, "init") == 0 || strcmp(fd->name, "_") == 0){
    if(fd->params != NULL || fd->returnType != NULL){
      fprintf(stderr, "Error: (line %d) init function cannot have return type of param list\n", lineno);
      exit(1);
    }
  }
  else{
    putSymbol(st, dk_func, fd->name, NULL, lineno);
  }
  if (mode == SymbolTablePrint) {
    printTab(tabCount);
    printf("%s [function] = ", fd->name);
    if (strcmp(fd->name, "_") == 0 || strcmp(fd->name, "init") == 0) {
      printf("<unmapped>");
    } else {
      printParamList(fd->params);
      printf(" -> ");
      if (fd->returnType != NULL) {
        printType(fd->returnType);
      } else {
        printf("void");
      }
    }
    printf("\n");
  }

  STMT *pl = paramListToStmt(fd->params, lineno);
  pl = combineStmt(pl, fd->body, lineno);
  symTypesStatements(pl, st);
}

void putShortDecl(SymbolTable *st, SHORT_SPECS *ss, int lineno) {
  //Check if one of the lhs hasn't been declared yet
  int newDecl = 0;
  while (ss != NULL) {
    if (strcmp(ss->lhs->val.id, "_") == 0) {
      ss = ss->next;
      continue;
    }
    symTypesExpressions(ss->rhs, st);
    
    if(getSymbolCurrentScope(st, ss->lhs->val.id) == NULL){
      putSymbol(st, dk_short, ss->lhs->val.id, NULL, lineno);

      newDecl = 1;
      if (mode == SymbolTablePrint) {
        printTab(tabCount);
        prettyPrintExp(ss->lhs);
        printf(" [variable] = <infer>\n");
      }
    }
    ss = ss->next;
  }

  if(newDecl == 0){
    fprintf(stderr, "Error: (line %d) short declaration must have at least one previously undeclared variable", lineno);
    exit(1);
  }
}

void putVarDecl(SymbolTable *st, VAR_SPECS *vs, int lineno) {
  while (vs != NULL) {
    if (strcmp(vs->id, "_") == 0) {
      vs = vs->next;
      continue;
    }
    if(vs->exp != NULL){
      symTypesExpressions(vs->exp, st);
    }
    putSymbol(st, dk_var, vs->id, vs->type, lineno);
    if (mode == SymbolTablePrint) {
      printTab(tabCount);
      printf("%s [variable] = ", vs->id);
      if (vs->type == NULL) {
        printf("<infer>");
      } else {
        printType(vs->type);
      }
      printf("\n");
    }

    vs = vs->next;
  }
}

/* Scopes the symbol table and adjusts tab count as necessary */
void createScope(STMT *stmt, SymbolTable *st) {
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  // Add scope into stmt
  symTypesStatements(stmt, scope);
  closeScope();
}

/* Takes care of properly scoping an if statment and the else blocks associated with it */
void createIfStmtScope(STMT *stmt, SymbolTable *st) {
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  if (stmt->val.ifStmt.simpleStmt != NULL) {
    symTypesStatements(stmt->val.ifStmt.simpleStmt, scope);
  }
  if (stmt->val.ifStmt.cond != NULL){
    symTypesExpressions(stmt->val.ifStmt.cond, scope);
  }
  symTypesStatements(stmt->val.ifStmt.body, scope);
  if (stmt->val.ifStmt.elseStmt != NULL) {
    symTypesStatements(stmt->val.ifStmt.elseStmt, scope);
  }
  closeScope();
}

void createForStmtScope(STMT *stmt, SymbolTable *st) {
  SymbolTable *scope = scopeSymbolTable(st);
  openScope();
  if (stmt->val.forStmt.forClause != NULL && stmt->val.forStmt.forClause->init != NULL) {
    symTypesStatements(stmt->val.forStmt.forClause->init, scope);
  }
  symTypesStatements(stmt->val.forStmt.body, scope);
  closeScope();
}

void createSwitchStmtScope(STMT *stmt, SymbolTable *st) {
  SymbolTable *scope = scopeSymbolTable(st);

  openScope();
  if (stmt->val.switchStmt.simpleStmt != NULL) {
    symTypesStatements(stmt->val.switchStmt.simpleStmt, scope);
  }

  CASE_CLAUSE_LIST *ccl = stmt->val.switchStmt.caseClauses;
  while (ccl != NULL) {
    STMT *newBlock;
    if (ccl->clause->kind == ck_case) {
      newBlock = makeBlockStmt(ccl->clause->val.caseClause.clauses, stmt->lineno);
    } else {  // Else is defauly
      newBlock = makeBlockStmt(ccl->clause->val.defaultClauses, stmt->lineno);
    }
    symTypesStatements(newBlock, st);
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

SYMBOL *getSymbolCurrentScope(SymbolTable *t, char *name) {
  int i = Hash(name);
  // Check the current scope
  for (SYMBOL *s = t->table[i]; s; s = s->next) {
    if (strcmp(s->name, name) == 0) return s;
  }
  return NULL;
}

void symProgram(PROG *root, SymbolTableMode m) {
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

void symTypesDeclarations(DECL *decl, SymbolTable *st) {
  // We could have passed the entire declaration into the put symbol tabl
  while (decl != NULL) {
    switch (decl->kind) {
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
    decl = decl->next;
  }
}

void symTypesStatements(STMT *stmt, SymbolTable *st) {
  STMT_LIST *sl;
  CASE_CLAUSE_LIST *ccl;
  SymbolTable *block;
  switch (stmt->kind) {
    case sk_block:
      block = scopeSymbolTable(st);
      openScope();
      sl = stmt->val.block.blockStatements;
      //Add scope to the block stmt 
      stmt->val.block.scope = block;
      while (sl != NULL) {
        symTypesStatements(sl->stmt, block);
        sl = sl->next;
      }
      closeScope();
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
    default:
      break;
  }
}

void symTypesExpressions(EXP *exp, SymbolTable *st){
  EXP_LIST *el;
  switch (exp->kind) {
			case ek_id:
        getSymbol(st, exp->val.id);
        if(getSymbol(st, exp->val.id) == NULL){
          fprintf(stderr, "Error: (line %d) %s is not declared", exp->lineno, exp->val.id);
          exit(1);
        }
				break;
			case ek_float:
				break;
			case ek_int:
				break;
			case ek_string:
				break;
			case ek_boolean:
				break;
			case ek_rune:
				break;
			case ek_plus:
			case ek_minus:
			case ek_times:
			case ek_div:
			case ek_mod:
			case ek_bitAnd:
			case ek_bitOr:
			case ek_eq:
			case ek_ne:
			case ek_ge:
			case ek_le:
			case ek_gt:
			case ek_lt:
			case ek_and:
			case ek_or:
			case ek_bitXor:
			case ek_bitLeftShift:
			case ek_bitRightShift:
      case ek_bitClear:
        symTypesExpressions(exp->val.binary.lhs, st);
        symTypesExpressions(exp->val.binary.rhs, st);
        break;
			case ek_uplus:
			case ek_uminus:
			case ek_bang:
			case ek_ubitXor:
        symTypesExpressions(exp->val.unary.exp, st);
        break;
			case ek_func:
        if(getSymbol(st,exp->val.funcCall.funcId) == NULL){
          fprintf(stderr, "Error: (line %d) %s is not declared", exp->lineno, exp->val.funcCall.funcId);
          exit(1); 
        }
        el = exp->val.funcCall.args;
        while(el != NULL){
          symTypesExpressions(el->exp, st);
          el = el->next;
        }
				break;
			case ek_append:
        symTypesExpressions(exp->val.append.elem, st);
        symTypesExpressions(exp->val.append.sliceExp, st);
				break;
			case ek_len:
        symTypesExpressions(exp->val.lenExp, st);
				break;
			case ek_cap:
        symTypesExpressions(exp->val.capExp, st);
				break;
			case ek_indexExp:
        symTypesExpressions(exp->val.indexExp.indexExp, st);
        symTypesExpressions(exp->val.indexExp.objectExp, st);
				break;
			case ek_structField:
        if(getSymbol(st,exp->val.structField.fieldName) == NULL){
          fprintf(stderr, "Error: (line %d) %s is not declared", exp->lineno, exp->val.structField.fieldName);
          exit(1); 
        } 
        symTypesExpressions(exp->val.structField.structExp, st);
				break;
			case ek_paren:
        symTypesExpressions(exp->val.parenExp, st);
				break;
			case ek_conv:
        el = exp->val.convField.args;
        while(el != NULL){
          symTypesExpressions(el->exp, st);
          el = el->next;
        }
        // TODO: Check if convField.type is well defined
				break;
		}
}

void symTypesDefaults(SymbolTable *st) {
  // int
  TYPE *t = makeType((char *)"int", 0);
  t->kind = tk_int;
  TYPE_SPECS *ts = makeTypeSpec((char *)"int", t);
  putTypeDecl(st, ts, 0);
  // float64
  t = makeType((char *)"float64", 0);
  t->kind = tk_float;
  baseFloat = t;
  ts = makeTypeSpec((char *)"float64", t);
  putTypeDecl(st, ts, 0);
  // rune
  t = makeType((char *)"rune", 0);
  t->kind = tk_rune;
  baseRune = t;
  ts = makeTypeSpec((char *)"rune", t);
  putTypeDecl(st, ts, 0);

  // string
  t = makeType((char *)"string", 0);
  t->kind = tk_string;
  baseString = t;
  ts = makeTypeSpec((char *)"string", t);
  putTypeDecl(st, ts, 0);
  // bool
  t = makeType((char *)"bool", 0);
  t->kind = tk_boolean;
  baseBool = t;
  ts = makeTypeSpec((char *)"bool", t);
  putTypeDecl(st, ts, 0);
  // true
  putSymbol(st, dk_var, (char *)"true", t, 0);
  if (mode == SymbolTablePrint) {
    printTab(tabCount);
    printf("true [constant] = bool\n");
  }
  // false
  putSymbol(st, dk_var, (char *)"false", t, 0);
  if (mode == SymbolTablePrint) {
    printTab(tabCount);
    printf("false [constant] = bool\n");
  }
}

/* Converts a list of parameters into a list of decl statments */
STMT *paramListToStmt(PARAM_LIST *pl, int lineno) {
  VAR_SPECS *vs = NULL;
  VAR_SPECS *last = NULL;
  while (pl != NULL) {
    if (vs == NULL) {
      vs = (VAR_SPECS *)malloc(sizeof(VAR_SPECS));
      last = vs;
    } else {
      last->next = (VAR_SPECS *)malloc(sizeof(VAR_SPECS));
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
STMT *combineStmt(STMT *s1, STMT *s2, int lineno) {
  STMT_LIST *sl;
  if(s2->kind == sk_block){
    sl = s2->val.block.blockStatements;
  } else {
    sl = makeStmtList(s2, NULL);
  }
  sl = makeStmtList(s1, sl);
  return makeBlockStmt(sl, lineno);
}

void printType(TYPE *type) {
  FIELD_DECLS *d;
  switch (type->kind) {
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
      while (d != NULL) {
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

void printParamList(PARAM_LIST *pl) {
  printf("(");
  while (pl != NULL) {
    printType(pl->type);
    if (pl->next != NULL) {
      printf(", ");
    }
    pl = pl->next;
  }
  printf(")");
}
