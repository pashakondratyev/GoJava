#ifndef TREE_H
#define TREE_H

#include <stdbool.h>


typedef struct PROG PROG;
typedef struct PACKAGE PACKAGE;
typedef struct DECL DECL;

typedef struct VAR_SPECS VAR_SPECS;
typedef struct TYPE_SPECS TYPE_SPECS;
typedef struct FUNC_DECL FUNC_DECL;
typedef struct PARAM_LIST PARAM_LIST;
typedef struct SIGNATURE SIGNATURE;

typedef struct STMT STMT;
typedef struct STMT_LIST STMT_LIST;
typedef struct ASSIGNMENT ASSIGN;

typedef struct ID_LIST ID_LIST;
typedef struct FIELD_DECLS FIELD_DECLS;
typedef struct CASE_CLAUSE CASE_CLAUSE;
typedef struct CASE_CLAUSE_LIST CASE_CLAUSE_LIST;
typedef struct FOR_CLAUSE FOR_CLAUSE;

typedef struct EXP EXP;
typedef struct EXP_LIST EXP_LIST;

typedef struct TYPE TYPE;


typedef enum {
	tk_int,
	tk_float,
	tk_rune,
	tk_string,
	tk_boolean,
	tk_struct,
	tk_array
} TypeKind;

typedef enum {
  dk_type, 
  dk_func, 
  dk_var
} DecKind;

typedef enum {
	sk_empty, // needed?
	sk_block,
	sk_expr,
	sk_assign,
	sk_decl,
	sk_shortDecl,
	sk_incr,
	sk_decr,
	sk_print,
	sk_println,
	sk_return,
	sk_if,
	sk_else,
	sk_switch, 
	sk_for,
	sk_break,
	sk_continue
} StmtKind;

typedef enum {
	aok_plus,
	aok_minus,
	aok_times,
	aok_div,
	aok_mod,
	aok_bitAnd,
	aok_bitOr,
	aok_bitXor,
	aok_bitLeftShift,
	aok_bitRightShift,
	aok_bitClear
} AssignOpKind;

typedef enum {
	ck_case,
	ck_default
} CaseKind;

typedef enum {
	ek_id,
	ek_float,
	ek_int,
	ek_string,
	ek_boolean,
	ek_rune,
	ek_plus,
	ek_minus,
	ek_times,
	ek_div,
	ek_mod,
	ek_bitAnd,
	ek_bitOr,
	ek_eq,
	ek_ne,
	ek_ge,
	ek_le,
	ek_gt,
	ek_lt,
	ek_logicAnd,
	ek_logicOr,
	ek_bitXor,
	ek_bitLeftShift,
	ek_bitRightShift,
	ek_bitClear,
	ek_uplus,
	ek_uminus,
	ek_bang,
	ek_func,	// includes casts
	ek_append,
	ek_len,
	ek_cap,
	ek_indexExp,	// for slice or array 
	ek_structField
} ExpKind;

struct PROG {
	int lineno;
	PACKAGE *package;
	DECL *root_decl;
};

struct PACKAGE {
	int lineno;
	char *name;
};

struct DECL {
	int lineno;
	DecKind kind;
	union {
		VAR_SPECS *varSpecs;
		TYPE_SPECS *typeSpecs;
		FUNC_DECL *funcDecl;
	} val;
	DECL *next;
};

struct VAR_SPECS {
	char *id;
	bool hasType;
	bool isInitialized;
	TYPE *type;
	EXP *exp;	
	VAR_SPECS *next;
};

struct TYPE_SPECS {
	char *name; // new name used for the type
	TYPE *type; 
	TYPE_SPECS *next;
};

struct FUNC_DECL {
	char *name;
	PARAM_LIST *params;
	STMT_LIST *body;
	TYPE *returnType;	// NULL if void return type
};

struct PARAM_LIST {
	int lineno;
	char *id;
	bool hasType;
	TYPE *type;
	PARAM_LIST *next;
};

// Intermediate structure. Not contained in the final AST. 
struct SIGNATURE {
	PARAM_LIST *params;
	TYPE *returnType; 	
};

struct STMT {
	int lineno;
	StmtKind kind;
	union {
		STMT_LIST *block;
		DECL *decl;	
		// for all expression-based statements: expression, increment, decrement, return, print, println
		EXP *exp;
		ASSIGN *assign;
		struct { EXP *lhs; EXP *rhs; AssignOpKind kind; } assignOp;
		VAR_SPECS *shortVarDecl;
		struct { STMT *simpleStmt; EXP *exp; CASE_CLAUSE_LIST *caseClauses; } switchStmt;
		struct { EXP *whileExp; FOR_CLAUSE *forClause; STMT_LIST *body; } forLoop;
		struct { STMT *simpleStmt; EXP *exp; STMT_LIST *body; } ifBranch;
		STMT_LIST *elseBody;
	} val;
};

struct STMT_LIST {
	STMT *stmt;
	STMT_LIST *next;
};

struct ASSIGN {
	EXP *lhs;
	EXP *rhs;
	ASSIGN *next;
};

struct ID_LIST {
	char *id;
	ID_LIST *next;
};

struct FIELD_DECLS {
	int lineno;
	char *id;
	TYPE *type;
	FIELD_DECLS *next;
};

struct CASE_CLAUSE {
	int lineno;
	CaseKind kind;
	union {
		struct { EXP_LIST *cases; STMT_LIST *clauses; } caseClause;
		STMT_LIST *default_clauses;
	} val;
};

struct CASE_CLAUSE_LIST {
	CASE_CLAUSE *clause;
	CASE_CLAUSE_LIST *next;
};

struct FOR_CLAUSE {
	int lineno;
	STMT *init;
	EXP *cond;
	STMT *post;
};

struct EXP {
	int lineno;
	ExpKind kind;
	union {
		char *id;
		double floatval;
		int intval;
		char *stringval;
		bool booleanval;
		char runeval;
		struct { EXP *lhs; EXP *rhs; } binary;
		struct { EXP *exp; } unary;
		// function call - (cast calls treated as functions)
		struct { EXP *funcExp; EXP_LIST *args; } funcCall;
		struct { EXP *sliceExp; EXP *elem; } append;
		EXP *lenExp;
		EXP *capExp;
		struct { EXP *sliceOrArrayExp; int index; } indexExp;
		struct { EXP *structExp; char *field_name; } structField;
	} val;
};

struct EXP_LIST {
	EXP *exp;
	EXP_LIST *next;
};


struct TYPE {
	int lineno;
	TypeKind kind;
	union {
		char *name;
		struct { int size; TYPE *type; } array;
		TYPE *sliceType;
		FIELD_DECLS *structFields;
	} val;
};


// To be completed
PROG *makeProg(PACKAGE *package, DECL *decl, int lineno);
PACKAGE *makePackage(char *name, int lineno);

DECL *makeDecls(DECL *firstDecl, DECL *declList);
DECL *makeFuncDecl(char *name, SIGNATURE *signature, STMT_LIST *body, int lineno);
SIGNATURE *makeSignature(PARAM_LIST *params, TYPE *type);
TYPE_SPECS *makeTypeSpec(char *name, TYPE *type);
TYPE_SPECS *makeTypeSpecList(TYPE_SPECS *specHead, TYPE_SPECS *nextSpec);

STMT *makeBlockStmt(STMT_LIST *stmts, int lineno);
STMT *makeExpStmt(EXP *exp, int lineno);
STMT *makeDeclStmt(DECL *decl, int lineno);
STMT *makeIncrStmt(EXP *incrExp, int lineno);
STMT *makeDecrStmt(EXP *decrExp, int lineno);
STMT *makePrintStmt(EXP_LIST *exprList, int lineno);
STMT *makePrintlnStmt(EXP_LIST *exprList, int lineno);
STMT *makeReturnStmt(EXP *returnExp, int lineno);
STMT *makeBreakStmt(int lineno);
STMT *makeContinueStmt(int lineno);
STMT_LIST *makeStmtList(STMT *firstStmt, STMT_LIST *stmtList);

ID_LIST *makeIdList(ID_LIST *list, char *id);

EXP *makeIdentifierExp(char *id, int lineno);
EXP *makeFloatValExp(double floatval, int lineno);
EXP *makeIntValExp(int intval, int lineno);
EXP *makeStringValExp(char *stringval, int lineno);
EXP *makeBooleanValExp(bool booleanval, int lineno);
EXP *makeRuneValExp(char runeval, int lineno);
EXP *makeBinaryExp(ExpKind kind, EXP *lhs, EXP *rhs, int lineno);
EXP *makeUnaryExp(ExpKind kind, EXP *exp, int lineno);
EXP *makeFunctionCall(EXP *funcExp, EXP_LIST *args, int lineno);
EXP *makeAppendCall(EXP *sliceExp, EXP *elem, int lineno);
EXP *makeLenCall(EXP *sliceOrArrayExp, int lineno);
EXP *makeCapCall(EXP *sliceOrArrayExp, int lineno);
EXP *makeIndexExp(EXP *sliceOrArrayExp, int index, int lineno);
EXP *makeStructFieldAccess(EXP *structExp, char *field_name, int lineno);

EXP_LIST *makeExpList(EXP_LIST *list, EXP *exp);

TYPE *makeType(char *name);

#endif