#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deal with basic types in typechecker phase

PROG *makeProg(PACKAGE *package, DECL *decl, int lineno) {
  PROG *p = malloc(sizeof(PROG));
  p->lineno = lineno;
  p->package = package;
  p->root_decl = decl;
  return p;
}

PACKAGE *makePackage(char *name, int lineno) {
  PACKAGE *p = malloc(sizeof(PACKAGE));
  p->lineno = lineno;
  p->name = strdup(name);
  return p;
}

//Corresponds to declaration lists
DECL *makeDecls(DECL *firstDecl, DECL *declList) {
  firstDecl->next = declList;
  return firstDecl;
}

DECL *makeTypeDecl(TYPE_SPECS *typeSpecs, int lineno) {
  DECL *d = malloc(sizeof(DECL));
  d->lineno = lineno;
  d->kind = dk_type;
  d->val.typeSpecs = typeSpecs;
  d->next = NULL;
  return d;
}

TYPE_SPECS *makeTypeSpec(char *name, TYPE *type) {
  TYPE_SPECS *ts = malloc(sizeof(TYPE_SPECS));
  ts->name = strdup(name);
  ts->type = type;
  ts->next = NULL;
  return ts;
}

TYPE_SPECS *makeTypeSpecList(TYPE_SPECS *listHead, TYPE_SPECS *nextSpec) {
  if (listHead == NULL) {
    return nextSpec;
  }
  TYPE_SPECS *cur = listHead;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = nextSpec;
  return listHead;
}

DECL *makeVarDecl(VAR_SPECS *varSpecs, int lineno) {
  DECL *d = malloc(sizeof(DECL));
  d->lineno = lineno;
  d->kind = dk_var;
  d->val.varSpecs = varSpecs;
  d->next = NULL;
  return d;
}

VAR_SPECS *makeVarSpecs(ID_LIST *idList, EXP_LIST *expList, TYPE *type, int lineno) {
  // Weeding check: lists are equally sized or the variables aren't being
  // defined
  int numIds = 0;
  int numExps = 0;

  ID_LIST *curId = idList;
  EXP_LIST *curExp = expList;
  while (curId != NULL) {
    curId = curId->next;
    numIds++;
  }
  while (curExp != NULL) {
    curExp = curExp->next;
    numExps++;
  }
  if (numIds != numExps && numExps != 0) {
    fprintf(stderr,
            "Error: (line %d) variable declaration has unequal number of ids "
            "and expressions.\n",
            lineno);
    exit(1);
  }
  // create the VarSpecs
  curId = idList;
  curExp = expList;
  VAR_SPECS *firstSpec = NULL;
  VAR_SPECS *lastSpec = NULL;
  while (curId != NULL) {
    VAR_SPECS *vs = malloc(sizeof(VAR_SPECS));
    vs->id = strdup(curId->id);
    vs->type = type;
    vs->exp = curExp == NULL ? NULL : curExp->exp;
    vs->next = NULL;

    if (lastSpec != NULL) {
      lastSpec->next = vs;
      lastSpec=lastSpec->next;
    }
    if (firstSpec == NULL) {
      firstSpec = vs;
      lastSpec = vs;
    }
    curId = curId->next;
    curExp = curExp == NULL ? NULL : curExp->next;
  }
  return firstSpec;
}

VAR_SPECS *addVarSpec(VAR_SPECS *listHead, VAR_SPECS *nextSpec) {
  if (listHead == NULL) {
    return nextSpec;
  }
  VAR_SPECS *cur = listHead;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = nextSpec;
  return listHead;
}

DECL *makeShortVarDecl(EXP_LIST *lhsList, EXP_LIST *rhsList, int lineno) {
  // Weeding check: lists are equally sized
  int numLhsExp = 0;
  int numRhsExp = 0;
  EXP_LIST *cur = lhsList;
  while (cur != NULL) {
    cur = cur->next;
    numLhsExp++;
  }
  cur = rhsList;
  while (cur != NULL) {
    cur = cur->next;
    numRhsExp++;
  }
  if (numLhsExp != numRhsExp) {
    fprintf(stderr,
            "Error: (line %d) assignment has unequal number of expressions on "
            "either side.\n",
            lineno);
    exit(1);
  }

  // create the ShortSpecs
  EXP_LIST *curLhs = lhsList;
  EXP_LIST *curRhs = rhsList;
  SHORT_SPECS *firstSpec = NULL;
  SHORT_SPECS *lastSpec = NULL;
  while (curLhs != NULL) {
    SHORT_SPECS *ss = malloc(sizeof(SHORT_SPECS));
    ss->lhs = curLhs->exp;
    ss->rhs = curRhs->exp;
    ss->declared = 0;
    ss->next = NULL;
    // If tail not empty, shifts tail over
    if (lastSpec != NULL) {
      lastSpec->next = ss;
    }
    lastSpec = ss; 
    if (firstSpec == NULL) {
      firstSpec = ss;
    }
    curLhs = curLhs->next;
    curRhs = curRhs->next;
  }
  DECL *d = malloc(sizeof(DECL));
  d->lineno = lineno;
  d->kind = dk_short;
  d->val.shortSpecs = firstSpec;
  d->next = NULL;
  return d;
}

DECL *makeFuncDecl(char *name, SIGNATURE *signature, STMT *block, int lineno) {
  FUNC_DECL *func = malloc(sizeof(FUNC_DECL));
  func->name = strdup(name);
  func->params = signature->params;
  func->body = block;
  func->returnType = signature->returnType;

  DECL *decl = malloc(sizeof(DECL));
  decl->lineno = lineno;
  decl->kind = dk_func;
  decl->val.funcDecl = func;
  decl->next = NULL;
  return decl;
}

SIGNATURE *makeSignature(PARAM_LIST *params, TYPE *type) {
  SIGNATURE *s = malloc(sizeof(SIGNATURE));
  s->params = params;
  s->returnType = type;
  return s;
}

STMT_LIST *makeStmtList(STMT *firstStmt, STMT_LIST *stmtList) {
  // if (firstStmt == NULL) {
  // 	printf("ERROR: Logical error in makeStmtList\n");
  // }
  STMT_LIST *head = malloc(sizeof(STMT_LIST));
  head->stmt = firstStmt;
  head->next = stmtList;
  return head;
}

PARAM_LIST *makeParamList(PARAM_LIST *firstParam, PARAM_LIST *paramList) {
  if (firstParam == NULL) {
    // printf("ERROR: Logical error in makeParamList.\n");
  }
  firstParam->next = paramList;
  return firstParam;
}

PARAM_LIST *makeParamListFromIdList(ID_LIST *idList, TYPE *type, int lineno) {
  PARAM_LIST *head = NULL;
  PARAM_LIST *lastParam = NULL;

  ID_LIST *curId = idList;
  while (curId != NULL) {
    PARAM_LIST *p = malloc(sizeof(PARAM_LIST));
    p->lineno = lineno;
    p->id = strdup(curId->id);
    p->type = type;
    p->next = NULL;
    if (lastParam != NULL) {
      lastParam->next = p;
    }
    if (head == NULL) {
      head = p;
    }

    lastParam = p;
    curId = curId->next;
  }
  return head;
}

ID_LIST *makeIdList(ID_LIST *listHead, char *nextId) {
  // if (nextId == NULL) {
  // 	printf("ERROR: logical error in makeIdList.\n");
  // }
  ID_LIST *newId = malloc(sizeof(ID_LIST));
  newId->id = strdup(nextId);
  newId->next = NULL;
  if (listHead == NULL) {
    return newId;
  }
  ID_LIST *cur = listHead;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = newId;
  return listHead;
}

STMT *makeDeclStmt(DECL *decl, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_decl;
  s->val.decl = decl;
  return s;
}

STMT *makeShortDeclStmt(DECL *decl, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_shortDecl;
  s->val.decl = decl;
  return s;
}

STMT *makeBlockStmt(STMT_LIST *stmts, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_block;
  s->val.block.blockStatements = stmts;
  s->val.block.scope = NULL;
  return s;
}

STMT *makeExpStmt(EXP *exp, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_exp;
  s->val.exp = exp;
  return s;
}

STMT *makeIncrStmt(EXP *incrExp, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_incr;
  s->val.exp = incrExp;
  return s;
}

STMT *makeDecrStmt(EXP *decrExp, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_decr;
  s->val.exp = decrExp;
  return s;
}

STMT *makePrintStmt(EXP_LIST *expList, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_print;
  s->val.printExps = expList;
  return s;
}

STMT *makePrintlnStmt(EXP_LIST *expList, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_println;
  s->val.printExps = expList;
  return s;
}

STMT *makeReturnStmt(EXP *returnExp, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_return;
  s->val.exp = returnExp;
  return s;
}

STMT *makeBreakStmt(int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_break;
  return s;
}

STMT *makeContinueStmt(int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_continue;
  return s;
}

STMT *makeFallthroughStmt(int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_fallthrough;
  return s;
}

STMT *makeAssignStmt(EXP_LIST *lhsList, EXP_LIST *rhsList, int lineno) {
  // Weeding check: lists are equally sized
  int numLhsExp = 0;
  int numRhsExp = 0;
  EXP_LIST *cur = lhsList;
  while (cur != NULL) {
    cur = cur->next;
    numLhsExp++;
  }
  cur = rhsList;
  while (cur != NULL) {
    cur = cur->next;
    numRhsExp++;
  }
  if (numLhsExp != numRhsExp) {
    fprintf(stderr,
            "Error: (line %d) assignment has unequal number of expressions on "
            "either side.\n",
            lineno);
    exit(1);
  }
  // Create the ASSIGNs and the STMT
  EXP_LIST *curLhs = lhsList;
  EXP_LIST *curRhs = rhsList;
  ASSIGN *firstAssign = NULL;
  ASSIGN *lastAssign = NULL;
  while (curLhs != NULL) {
    ASSIGN *a = malloc(sizeof(ASSIGN));
    a->lhs = curLhs->exp;
    a->rhs = curRhs->exp;
    a->next = NULL;

    if (lastAssign != NULL) {
      lastAssign->next = a;
      lastAssign = lastAssign->next;
    }

    if (firstAssign == NULL) {
      firstAssign = a;
      lastAssign = a;
    }

    curLhs = curLhs->next;
    curRhs = curRhs->next;
  }

  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_assign;
  s->val.assign = firstAssign;
  return s;
}

STMT *makeAssignOpStmt(EXP *lhs, EXP *rhs, AssignOpKind kind, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_assignOp;
  s->val.assignOp.lhs = lhs;
  s->val.assignOp.rhs = rhs;
  s->val.assignOp.kind = kind;
  return s;
}

STMT *makeForStmt(EXP *whileExp, FOR_CLAUSE *forClause, STMT *body, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_for;
  s->val.forStmt.whileExp = whileExp;
  s->val.forStmt.forClause = forClause;
  s->val.forStmt.body = body;
  return s;
}

STMT *makeEmptyStmt(int lineno){
  // Empty statement is easier to work with than checking if a stmt is null
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_empty;
  return s;
}

CASE_CLAUSE_LIST *makeCaseClauseList(CASE_CLAUSE *firstClause, CASE_CLAUSE_LIST *caseClauseList) {
  // if (firstClause == NULL) {
  // 	printf("ERROR: Logical error in makeCaseClauseList.\n");
  // }
  CASE_CLAUSE_LIST *head = malloc(sizeof(CASE_CLAUSE_LIST));
  head->clause = firstClause;
  head->next = caseClauseList;
  return head;
}

CASE_CLAUSE *makeCaseClause(EXP_LIST *cases, STMT_LIST *clauses, int lineno) {
  CASE_CLAUSE *cc = malloc(sizeof(CASE_CLAUSE));
  cc->kind = ck_case;
  cc->val.caseClause.cases = cases;
  cc->val.caseClause.clauses = clauses;
  cc->lineno = lineno;
  cc->scope = NULL;
  return cc;
}

CASE_CLAUSE *makeDefaultClause(STMT_LIST *clauses, int lineno) {
  CASE_CLAUSE *cc = malloc(sizeof(CASE_CLAUSE));
  cc->kind = ck_default;
  cc->val.defaultClauses = clauses;
  cc->lineno = lineno;
  cc->scope = NULL;
  return cc;
}

STMT *makeSwitchStmt(STMT *simpleStmt, EXP *exp, CASE_CLAUSE_LIST *caseClauses, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_switch;
  s->val.switchStmt.simpleStmt = simpleStmt;
  s->val.switchStmt.exp = exp;
  s->val.switchStmt.caseClauses = caseClauses;
  return s;
}

STMT *makeIfStmt(STMT *simpleStmt, EXP *cond, STMT *body, STMT *elseStmt, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_if;
  s->val.ifStmt.simpleStmt = simpleStmt;
  s->val.ifStmt.cond = cond;
  s->val.ifStmt.body = body;
  s->val.ifStmt.elseStmt = elseStmt;
  return s;
}

STMT *makeElseStmt(STMT *body, int lineno) {
  STMT *s = malloc(sizeof(STMT));
  s->lineno = lineno;
  s->kind = sk_else;
  s->val.elseBody = body;
  return s;
}

FOR_CLAUSE *makeForClause(STMT *init, EXP *cond, STMT *post) {
  FOR_CLAUSE *fc = malloc(sizeof(FOR_CLAUSE));
  fc->init = init;
  fc->cond = cond;
  fc->post = post;
  return fc;
}

EXP_LIST *makeExpList(EXP_LIST *listHead, EXP *nextExp) {
  // if (nextExp == NULL) {
  // 	printf("ERROR: Logical error in makeExpList.\n");
  // }
  EXP_LIST *newExp = malloc(sizeof(EXP_LIST));
  newExp->exp = nextExp;
  newExp->next = NULL;
  if (listHead == NULL) {
    return newExp;
  }
  EXP_LIST *cur = listHead;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = newExp;
  return listHead;
}

EXP *makeIdentifierExp(char *id, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_id;
  e->val.id = strdup(id);
  e->type = NULL;
  return e;
}

EXP *makeIntValExp(int intval, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_int;
  e->val.intval = intval;
  e->type = NULL;
  return e;
}

EXP *makeFloatValExp(double floatval, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_float;
  e->val.floatval = floatval;
  e->type = NULL;
  return e;
}

EXP *makeStringValExp(char *stringval, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_string;
  e->val.stringval = strdup(stringval);
  e->type = NULL;
  return e;
}

EXP *makeRuneValExp(char runeval, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_rune;
  e->val.runeval = runeval;
  e->type = NULL;
  return e;
}

EXP *makeBooleanValExp(bool booleanval, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_boolean;
  e->val.booleanval = booleanval;
  e->type = NULL;
  return e;
}

EXP *makeBinaryExp(ExpKind kind, EXP *lhs, EXP *rhs, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = kind;
  e->val.binary.lhs = lhs;
  e->val.binary.rhs = rhs;
  e->type = NULL;
  return e;
}

EXP *makeUnaryExp(ExpKind kind, EXP *exp, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = kind;
  e->val.unary.exp = exp;
  e->type = NULL;
  return e;
}

EXP *makeFunctionCall(char *funcId, EXP_LIST *args, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_func;
  e->val.funcCall.funcId = strdup(funcId);
  e->val.funcCall.args = args;
  e->type = NULL;
  return e;
}

// TODO: Test this thoroughly
EXP *makeArgumentExp(EXP *iden, EXP_LIST *args, TYPE *type, int lineno){
  // The purpose of combining function calls and type casts is due to the similarities
  // In their syntax, which would otherwise create shift/reduce errors so a more general
  // CFG was created and depending on the types passed, it will process the expression
  // to figure out if we are dealing with a function call or a typecast
  // If the identifier is a pure identifier, it can be treated as a function call
  if(iden->kind == ek_id){
    return makeFunctionCall(iden->val.id, args, lineno);
  }
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_conv;
  e->val.convField.args = args;
  e->type = NULL;
  if(type != NULL){
    e->val.convField.type = type;
    return e;
  }
  else{
    // Unrolls the type identifier 
    while(iden->kind != ek_id){
      if(iden == NULL || iden->kind != ek_paren){
        fprintf(stderr, "Error: (line %d) not a function call or type conversion expression\n", lineno);
      }
      iden = iden->val.parenExp;
    } 
    return makeFunctionCall(iden->val.id, args, lineno);
  }
}

EXP *makeAppendCall(EXP *sliceExp, EXP *elem, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_append;
  e->val.append.sliceExp = sliceExp;
  e->val.append.elem = elem;
  e->type = NULL;
  return e;
}

EXP *makeLenCall(EXP *sliceOrArrayExp, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_len;
  e->val.lenExp = sliceOrArrayExp;
  e->type = NULL;
  return e;
}

EXP *makeCapCall(EXP *sliceOrArrayExp, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_cap;
  e->val.capExp = sliceOrArrayExp;
  e->type = NULL;
  return e;
}

EXP *makeIndexExp(EXP *objectExp, EXP *indexExp, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_indexExp;
  e->val.indexExp.objectExp = objectExp;
  e->val.indexExp.indexExp = indexExp;
  e->type = NULL;
  return e;
}

EXP *makeParenExp(EXP *exp, int lineno){
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_paren;
  e->val.parenExp = exp;
  e->type = NULL;
  return e;
}

EXP *makeStructFieldAccess(EXP *structExp, char *fieldName, int lineno) {
  EXP *e = malloc(sizeof(EXP));
  e->lineno = lineno;
  e->kind = ek_structField;
  e->val.structField.structExp = structExp;
  e->val.structField.fieldName = strdup(fieldName);
  e->type = NULL;
  return e;
}

TYPE *makeType(char *name, int lineno) {
  TYPE *t = malloc(sizeof(TYPE));
  t->lineno = lineno;
  t->val.name = strdup(name);
  t->kind = tk_res;
  return t;
}

TYPE *makeRefType(char *name, int lineno) {
  TYPE *t = malloc(sizeof(TYPE));
  t->lineno = lineno;
  t->val.name = strdup(name);
  t->kind = tk_ref;
  return t;
}

TYPE *makeSliceType(TYPE *elemType, int lineno) {
  TYPE *t = malloc(sizeof(TYPE));
  t->lineno = lineno;
  t->kind = tk_slice;
  t->val.sliceType = elemType;
  return t;
}

TYPE *makeArrayType(int size, TYPE *elemType, int lineno) {
  TYPE *t = malloc(sizeof(TYPE));
  t->lineno = lineno;
  t->kind = tk_array;
  t->val.array.size = size;
  t->val.array.elemType = elemType;
  return t;
}

TYPE *makeStructType(FIELD_DECLS *fields, int lineno) {
  TYPE *t = malloc(sizeof(TYPE));
  t->lineno = lineno;
  t->kind = tk_struct;
  t->val.structFields = fields;
  return t;
}

FIELD_DECLS *makeFieldDecls(ID_LIST *idList, TYPE *type, int lineno) {
  FIELD_DECLS *head = NULL;
  FIELD_DECLS *lastField = NULL;

  ID_LIST *curId = idList;
  while (curId != NULL) {
    FIELD_DECLS *fd = malloc(sizeof(FIELD_DECLS));
    fd->lineno = lineno;
    fd->id = strdup(curId->id);
    fd->type = type;
    fd->next = NULL;

    if (lastField != NULL) {
      lastField->next = fd;
      lastField = lastField->next;
    }

    if (head == NULL) {
      head = fd;
      lastField = fd;
    }

    curId = curId->next;
  }
  lastField = head;
  return head;
}

FIELD_DECLS *makeFieldDeclsList(FIELD_DECLS *firstField, FIELD_DECLS *fieldList) {
  // if (firstField == NULL) {
  // 	printf("ERROR: Logical error in makeFieldDeclsList.\n");
  // }
  FIELD_DECLS *temp = firstField;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = fieldList;
  
  return firstField;
}
