#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	if (stmt != NULL) {
		switch (stmt->kind) {
			case sk_block:
				break;
			case sk_exp:
				break;
			case sk_assign:
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
				break;
			case sk_else:
				break;
			case sk_switch:
				break;
			case sk_for:
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
	if (exp != NULL) {
		switch (exp->kind) {
			case ek_id:
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
				break;
			case ek_minus:
				break;
			case ek_times:
				break;
			case ek_div:
				break;
			case ek_mod:
				break;
			case ek_bitAnd:
				break;
			case ek_bitOr:
				break;
			case ek_eq:
				break;
			case ek_ne:
				break;
			case ek_ge:
				break;
			case ek_le:
				break;
			case ek_gt:
				break;
			case ek_lt:
				break;
			case ek_and:
				break;
			case ek_or:
				break;
			case ek_bitXor:
				break;
			case ek_bitLeftShift:
				break;
			case ek_bitRightShift:
				break;
			case ek_bitClear:
				break;
			case ek_uplus:
				break;
			case ek_uminus:
				break;
			case ek_bang:
				break;
			case ek_ubitXor:
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
				break;
			case ek_conv:
				break;
		}

	}
}
