#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

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
	p->name = malloc((strlen(name)+1)*sizeof(char));
    strcpy(p->name, name);
    return p;
}

DECL *makeDecls(DECL *firstDecl, DECL *declList) {
	if (firstDecl == NULL) {
		//printf("ERROR: Logical error in makeDecls.\n");
	}
	firstDecl->next = declList;
	return firstDecl;
}

DECL *makeFuncDecl(char *name, SIGNATURE *signature, STMT_LIST *body, int lineno) {
	FUNC_DECL *func = malloc(sizeof(FUNC_DECL));
	func->name = malloc((strlen(name)+1)*sizeof(char));
	strcpy(func->name, name);
	func->params = signature->params;
	func->body = body;
	func->returnType = signature->returnType;

	DECL *decl = malloc(sizeof(DECL));
	decl->lineno = lineno;
	decl->kind = dk_func;
	decl->val.funcDecl = func;

	return decl;
}

SIGNATURE *makeSignature(PARAM_LIST *params, TYPE *type) {
	SIGNATURE *s = malloc(sizeof(SIGNATURE));
	s->params = params;
	s->returnType = type;
	return s;
}

STMT_LIST *makeStmtList(STMT *firstStmt, STMT_LIST *stmtList) {
	if (firstStmt == NULL) {
		//printf("ERROR: Logical error in makeStmtList\n");
	}
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

	ID_LIST *curId = idList;
	while (curId != NULL) {
		PARAM_LIST *p = malloc(sizeof(PARAM_LIST));
		p->lineno = lineno;
		p->id = malloc((strlen(curId->id)+1)*sizeof(char));
		strcpy(p->id, curId->id);
		p->type = type;

		if(head == NULL) {
			head = p;
		}

		curId = curId->next;
	}
	return head;
}

ID_LIST *makeIdList(ID_LIST *listHead, char *nextId) {
	if (nextId == NULL) {
		//printf("ERROR: logical error in makeIdList.\n");
	}
	ID_LIST *newId = malloc(sizeof(ID_LIST));
	newId->id = malloc((strlen(nextId)+1)*sizeof(char));
	strcpy(newId->id, nextId);

	if(listHead == NULL) {
		return newId;
	}
	ID_LIST *cur = listHead;
	while(cur->next != NULL) {
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
	// Weeding check
	int numLhsExp = 0;
	int numRhsExp = 0;
	EXP_LIST *cur = lhsList;
	while(cur != NULL) {
		cur = cur->next;
		numLhsExp++;
	}
	cur = rhsList;
	while(cur != NULL) {
		cur = cur->next;
		numRhsExp++;
	}
	if (numLhsExp != numRhsExp) {
		printf("ERROR CAUGHT!!!!!!!!!!!!!!!");			// REMOVE PRINT STMT
		fprintf(stderr, "Error: (line %d) assignment has unequal number of expressions on either side.\n", lineno);
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

		if(lastAssign != NULL) {
			lastAssign->next = a;
		}

		if(firstAssign == NULL) {
			firstAssign = a;
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













EXP_LIST *makeExpList(EXP_LIST *listHead, EXP *nextExp) {
	if (nextExp == NULL) {
		//printf("ERROR: Logical error in makeExpList.\n");
	}
	EXP_LIST *newExp = malloc(sizeof(EXP_LIST));
	newExp->exp = nextExp;

	if(listHead == NULL) {
		return newExp;
	}
	EXP_LIST *cur = listHead;
	while(cur->next != NULL) {
		cur = cur->next;
	}
	cur->next = newExp;
	return listHead;
}


EXP *makeIdentifierExp(char *id, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_id;
	e->val.id = malloc((strlen(id)+1)*sizeof(char));
	strcpy(e->val.id, id);
	return e;
}

EXP *makeIntValExp(int intval, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_int;
	e->val.intval = intval;
	return e;
}

EXP *makeFloatValExp(double floatval, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_float;
	e->val.floatval = floatval;
	return e;
}

EXP *makeStringValExp(char *stringval, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_string;
	e->val.stringval = malloc((strlen(stringval)+1)*sizeof(char));
	strcpy(e->val.stringval, stringval);
	return e;
}

EXP *makeRuneValExp(char runeval, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_rune;
	e->val.runeval = runeval;
	return e;
}

EXP *makeBooleanValExp(bool booleanval, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_boolean;
	e->val.booleanval = booleanval;
	return e;
}

EXP *makeBinaryExp(ExpKind kind, EXP *lhs, EXP *rhs, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = kind;
	e->val.binary.lhs = lhs;
	e->val.binary.rhs = rhs;
	return e;
}

EXP *makeUnaryExp(ExpKind kind, EXP *exp, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = kind;
	e->val.unary.exp = exp;
	return e;
}

EXP *makeFunctionCall(char *funcId, EXP_LIST *args, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_func;
	e->val.funcCall.funcId = malloc((strlen(funcId)+1)*sizeof(char));
	strcpy(e->val.funcCall.funcId, funcId);
	e->val.funcCall.args = args;
	return e;
}

EXP *makeAppendCall(EXP *sliceExp, EXP *elem, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_append;
	e->val.append.sliceExp = sliceExp;
	e->val.append.elem = elem;
	return e;
}

EXP *makeLenCall(EXP *sliceOrArrayExp, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_len;
	e->val.lenExp = sliceOrArrayExp;
	return e;
}

EXP *makeCapCall(EXP *sliceOrArrayExp, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_cap;
	e->val.capExp = sliceOrArrayExp;
	return e;
}

EXP *makeIndexExp(EXP *objectExp, EXP *indexExp, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_indexExp;
	e->val.indexExp.objectExp = objectExp;
	e->val.indexExp.indexExp = indexExp;
	return e;
}

EXP *makeStructFieldAccess(EXP *structExp, char *fieldName, int lineno) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = lineno;
	e->kind = ek_structField;
	e->val.structField.structExp = structExp;
	e->val.structField.fieldName = malloc((strlen(fieldName)+1)*sizeof(char));
	strcpy(e->val.structField.fieldName, fieldName);
	return e;
}
