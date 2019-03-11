#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol.h"
#include "type.h"

SymbolTable *st;

void typeProgram(PROG *prog, SymbolTable *st) {
	if (prog != NULL) 
		typeDeclarations(prog->root_decl, st);
}

void typeDeclarations(DECL *dcl, SymbolTable *st) {
	if (dcl != NULL) {
		switch (dcl->kind) {
			case dk_var:
				typeVarDecl(dcl->val.varSpecs, st);
				break;
			case dk_short:
				typeShortDecl(dcl->val.shortSpecs, st);
				break;
			case dk_type:
				typeTypeDecl(dcl->val.typeSpecs, st);
				break;
			case dk_func:
				typeFuncDecl(dcl->val.funcDecl, st);
				break;
		}
		typeDeclarations(dcl->next, st);
	}
}

// TODO: Implement
void typeVarDecl(VAR_SPECS *vs, SymbolTable *st) {
	if (vs != NULL) {

	}
}

// TODO: Implement
void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st) {
	if (ss != NULL) {

	}
}

// TODO: Implement
void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st) {
	if (ts != NULL) {

	}
}

// TODO: Implement
void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st) {
	if (fd != NULL) {

	}
}

// TODO: Implement
void typeStmt(STMT *stmt, SymbolTable *st) {
	STMT_LIST *sl;
	ASSIGN *al;
	CASE_CLAUSE_LIST *ccl;
	if (stmt != NULL) {
		switch (stmt->kind) {
			case sk_block:
				// Change scope to the block
				st = stmt->val.block.scope;
				sl = stmt->val.block.blockStatements;
				while(st != NULL){
					typeStmt(sl->stmt, st);
					sl = sl->next;
				}
				break;
			case sk_exp:
				typeExp(stmt->val.exp, st);
				break;
			case sk_assign:
				al = stmt->val.assign;
				while(al != NULL){
					typeExp(al->lhs, st);
					typeExp(al->rhs, st);
					if(al->lhs->type != al->rhs->type){
						fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement", stmt->lineno, al->lhs->type->val.name, al->rhs->type->val.name);
						exit(1);
					}
					al=al->next; 
				}
				break;
			case sk_assignOp:
				break;
			case sk_decl:
				break;
			case sk_shortDecl:
				break;
			case sk_incr:
				break;
			case sk_decr:
				break;
			case sk_print:
				break;
			case sk_println:
				break;
			case sk_return:
				break;
			case sk_if:
				st = stmt->val.ifStmt.scope;
				if(stmt->val.ifStmt.simpleStmt != NULL){
					typeStmt(stmt->val.ifStmt.simpleStmt, st);
				}
				typeExp(stmt->val.ifStmt.cond, st);
				typeStmt(stmt->val.ifStmt.body, st);
				if(stmt->val.ifStmt.elseStmt != NULL){
					typeStmt(stmt->val.ifStmt.elseStmt, st);
				}
				break;
			case sk_else:
				typeStmt(stmt->val.elseBody, st);
				break;
			case sk_switch:
				st = stmt->val.switchStmt.scope;
				if(stmt->val.switchStmt.simpleStmt != NULL){
					typeStmt(stmt->val.switchStmt.simpleStmt, st);
				}
				typeExp(stmt->val.switchStmt.exp, st);
				ccl = stmt->val.switchStmt.caseClauses;
				while(ccl != NULL){
					if(ccl->clause->kind == ck_case){
						//TODO: Kabilan do the rest
					} else {

					}
					ccl = ccl->next;
				}				
				break;
			case sk_for:
				st = stmt->val.forStmt.scope;
				if(stmt->val.forStmt.forClause != NULL){
					FOR_CLAUSE *fc = stmt->val.forStmt.forClause;
					if(fc->init != NULL){
						typeStmt(fc->init, st);
					}
					if(fc->cond != NULL){
						typeExp(fc->cond, st);
					}
					if(fc->post != NULL){
						typeStmt(fc->post, st);
					}
				} else{
					typeExp(stmt->val.forStmt.whileExp, st);
				}
				typeStmt(stmt->val.forStmt.body, st);
				break;
			case sk_break:
				break;
			case sk_continue:
				break;
			case sk_fallthrough:
				break;
			case sk_empty:
				break;
		}
	}
}

// TODO: Implement
void typeExp(EXP *exp, SymbolTable *st) {
	TYPE *type;
	if (exp != NULL) {
		switch (exp->kind) {
			case ek_id:
				if(getSymbol(st, exp->val.id) == NULL){
					fprintf(stderr, "Error: (line %d) use of undeclared identifier \"%s\"", exp->lineno, exp->val.id);
					exit(1);
				}
				exp->type = getSymbol(st, exp->val.id)->type;
				break;
			case ek_float:
				exp->type = baseFloat;
				break;
			case ek_int:
				exp->type = baseInt;
				break;
			case ek_string:
				exp->type = baseString;
				break;
			case ek_boolean:
				exp->type = baseBool;
				break;
			case ek_rune:
				exp->type = baseRune;
				break;
			case ek_plus:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.lhs, st);
				if (exp->val.binary.rhs->type == exp->val.binary.lhs->type){
					if (typeNumeric(exp->val.binary.lhs->type) || typeString(exp->val.binary.lhs->type)){
						exp->type = exp->val.binary.lhs->type;
					} else {
						fprintf(stderr, "Error: (line %d) Operation only works with numeric or string types", exp->lineno);
						exit(1);
					}
				} else {
					fprintf(stderr, "Error: (line %d) Both sides of algebraic operation must be numeric", exp->lineno);
					exit(1);
				}
				break;
			case ek_minus:
			case ek_times:
			case ek_div:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.lhs, st);
				if (exp->val.binary.rhs->type == exp->val.binary.lhs->type){
					if (typeNumeric(exp->val.binary.lhs->type)){
						exp->type = exp->val.binary.lhs->type;
					} else {
						fprintf(stderr, "Error: (line %d) Operation only works with numeric types", exp->lineno);
						exit(1);
					}
				} else {
					fprintf(stderr, "Error: (line %d) Both sides of algebraic operation must be numeric", exp->lineno);
					exit(1);
				}
				break;
			case ek_eq:
			case ek_ne:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.rhs, st);
				if (exp->val.binary.lhs->type == exp->val.binary.rhs->type){
					if (typeComparable(exp->val.binary.lhs->type)){
						exp->type = baseBool;
					} else {
						fprintf(stderr, "Error: (line %d) Types not comparable", exp->lineno);
						exit(1);
					}
				} else {
					fprintf(stderr, "Error: (line %d) Both sides of equality test must be of same type", exp->lineno);
					exit(1);
				}
				break;
			case ek_ge:
			case ek_le:
			case ek_gt:
			case ek_lt:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.rhs, st);
				if (exp->val.binary.lhs->type == exp->val.binary.rhs->type){
					if (typeOrdered(exp->val.binary.lhs->type)){
						exp->type = baseBool;
					} else {
						fprintf(stderr, "Error: (line %d) Types not ordered", exp->lineno);
						exit(1);
					}
				} else {
					fprintf(stderr, "Error: (line %d) Both sides of comparison must be of same type", exp->lineno);
					exit(1);
				}
				break;
			case ek_and:
			case ek_or:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.rhs, st);
				if (typeBool(exp->val.binary.lhs->type) && typeBool(exp->val.binary.rhs->type)){
					exp->type = baseBool;
				} else {
					fprintf(stderr, "Error: (line %d) Logical operators and/or only accept two bools", exp->lineno);
					exit(1);
				}
				break;
			case ek_mod:
			case ek_bitAnd:
			case ek_bitOr:
			case ek_bitXor:
			case ek_bitLeftShift:
			case ek_bitRightShift:
			case ek_bitClear:
				typeExp(exp->val.binary.lhs, st);
				typeExp(exp->val.binary.rhs, st);
				if (typeInteger(exp->val.binary.lhs->type) && typeInteger(exp->val.binary.rhs->type == baseBool)){
					exp->type = exp->val.binary.lhs->type;
				} else {
					fprintf(stderr, "Error: (line %d) Logical operators and/or only accept two bools", exp->lineno);
					exit(1);
				}
				break;
			case ek_uplus:
			case ek_uminus:
				typeExp(exp->val.unary.exp, st);
				if (typeNumeric(exp->val.unary.exp->type)){
					exp->type = exp->val.unary.exp->type;
				} else {
					fprintf(stderr, "Error: (line %d) Unary plus/minus only accepts numeric types", exp->lineno);
					exit(1);
				}
				break;
			case ek_bang:
				typeExp(exp->val.unary.exp, st);
				if (typeBool(exp->val.unary.exp->type)){
					exp->type = baseBool;
				} else {
					fprintf(stderr, "Error: (line %d) Negation operator only accepts booleans", exp->lineno);
					exit(1);
				}
				break;
			case ek_ubitXor:
				typeExp(exp->val.unary.exp, st);
				if (typeInteger(exp->val.unary.exp->type)){
					exp->type = exp->val.unary.exp->type;
				} else {
					fprintf(stderr, "Error: (line %d) Bitwise negation operator only accepts integer types", exp->lineno);
					exit(1);
				}
				break;
			case ek_func:
				break;
			case ek_append:
				break;
			case ek_len:
				break;
			case ek_cap:
				break;
			case ek_indexExp:
				break;
			case ek_structField:
				break;
			case ek_paren:
				typeExp(exp->val.parenExp, st);
				exp->type = exp->val.parenExp;
				break;
			case ek_conv:
				break;
		}

	}
}

int typeInteger(TYPE *type){
	if (type != NULL){
		switch (type->kind){
			case tk_int:
			case tk_rune:
				return 1;
			default:
				return 0;
		}
	}
	return 0;
}

int typeBool(TYPE *type){
	if (type != NULL){
		switch (type->kind){
			case tk_boolean:
				return 1;
			default:
				return 0;
		}
	}
	return 0;
}

int typeString(TYPE *type){
	if (type != NULL){
		switch (type->kind){
			case tk_string:
				return 1;
			default:
				return 0;
		}
	}
	return 0;
}

int typeNumeric(TYPE *type){
	if (type != NULL){
		switch (type->kind){
			case tk_int:
			case tk_float:
			case tk_rune:
				return 1;
			default:
				return 0;
		}
	}
	return 0;
}

int typeComparable(TYPE *type){
	if (type != NULL){
		FIELD_DECLS *fd;
		switch (type->kind){
			case tk_int:
			case tk_float:
			case tk_rune:
			case tk_string:
			case tk_boolean:
				return 1;
			case tk_struct:
				fd = type->val.structFields;
				while (fd != NULL){
					if (!typeComparable(fd->type)){
						return 0;
					}
					fd = fd->next;
				}
				return 1;
			case tk_array:
				return typeComparable(type->val.array.elemType);
			case tk_slice:
			case tk_ref:
				return 0;
		}
	}
	return 0;
}

int typeOrdered(TYPE *type){
	if (type != NULL){
		switch (type->kind){
			case tk_int:
			case tk_float:
			case tk_rune:
			case tk_string:
				return 1;
			default:
				return 0;
		}
	}
	return 0;
}


