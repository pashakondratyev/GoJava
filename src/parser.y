%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"

// Reference to the line number generated by the scanner
extern int yylineno;
// Reference to the yylex function to quiet a compiler warning
int yylex();

// Root of the AST
extern PROG *root;

void yyerror(const char *s) {
  fprintf(stderr, "Error: (line %d) %s\n", yylineno, s);
  exit(1);
}
%}

%locations
%code requires{
  #include<stdbool.h>
}
%union {
  int intval;
  float floatval;
  bool boolval;
  char runeval;
  char *stringval;
  char *identifier;
}

%token <intval> tINTVAL
%token <floatval> tFLOATVAL
%token <boolval> tBOOLVAL
%token <runeval> tRUNEVAL
%token <stringval> tSTRINGVAL
%token <identifier> tIDENTIFIER

/* Keywords */
%token tBREAK tCASE tCHAN tCONST tCONTINUE tDEFAULT tDEFER
%token tELSE tFALLTHROUGH tFOR tFUNC tGO tGOTO tIF tIMPORT tINTERFACE
%token tMAP tPACKAGE tRANGE tRETURN tSELECT tSTRUCT tSWITCH tTYPE tVAR
%token tPRINT tPRINTLN tAPPEND tLEN tCAP

/* Operators */
%token tPLUS tMINUS tTIMES tDIV tMOD
%token tBITAND tBITOR tBITXOR tLEFTSHIFT tRIGHTSHIFT tBITCLEAR
%token tPLUSEQ tMINUSEQ tTIMESEQ tDIVEQ tMODEQ
%token tBITANDEQ tBITOREQ tBITXOREQ tBITDIVEQ tLEFTSHIFTEQ tRIGHTSHIFTEQ tBITCLEAREQ
%token tAND tOR tCHANNEL tINC tDEC
%token tEQ tLESS tGREATER tASSIGN tBANG
%token tNOTEQ tLTEQ tGTEQ tDECL tELIP
%token tLPAREN tRPAREN tLSBRACE tRSBRACE tLCBRACE tRCBRACE
%token tCOMMA tPERIOD tCOLON tSEMICOLON

/* Types */
%union {
    char *id;
    struct PROG *prog;
    struct PACKAGE *package;
    struct DECL *decl;
};


%type <prog> prog 
%type <package> PackageDecl
%type <decl> TopLevelDeclList FuncDecl Declaration



%left tOR
%left tAND
%left tEQ tNOTEQ
%left tLTEQ tGTEQ tLESS tGREATER
%left tPLUS tMINUS tBITOR tBITXOR
%left tTIMES tDIV tMOD tBITAND tLEFTSHIFT tRIGHTSHIFT tBITCLEAR
%left tBANG
%left UNARY BINARY

%nonassoc tELSE

%start prog
%%
prog: PackageDecl TopLevelDeclList { root = makeProg($1, $2, @1.first_line); }
    ;


PackageDecl: tPACKAGE tIDENTIFIER tSEMICOLON { $$ = makePackage($2, @2.first_line); }
    ;

TopLevelDeclList: Declaration tSEMICOLON    
    | FuncDecl tSEMICOLON   
    | Declaration tSEMICOLON TopLevelDeclList   { $$ = makeDecls($3, $1); }
    | FuncDecl tSEMICOLON TopLevelDeclList  { $$ = makeDecls($3, $1); }
    ;

Declaration: TypeDecl   { $$ = NULL; }
    | VarDecl       { $$ = NULL; }
    ;

VarDecl: tVAR VarSpec 
    | tVAR tLPAREN VarSpecList tRPAREN
    ;

ShortVarDecl: ExpressionList tDECL ExpressionList
    ;

VarSpec: IdentifierList Type tASSIGN ExpressionList
    | IdentifierList tASSIGN ExpressionList
    | IdentifierList Type
    ;

VarSpecList: %empty
    | VarSpecList VarSpec tSEMICOLON
    ;

Type: tIDENTIFIER
    | CompoundType
    ;

CompoundType: ArrayType
    | SliceType
    | StructType
    ;

IdentifierList: tIDENTIFIER
    | IdentifierList tCOMMA tIDENTIFIER
    ;

Expression: UnaryExpression %prec UNARY
    | Expression BinaryOp Expression %prec BINARY
    ;

ExpressionList: Expression
    | ExpressionList tCOMMA Expression 
    ; 

UnaryExpression: PrimaryExpression
    | UnaryOp UnaryExpression
    ;

UnaryOp: tPLUS
    | tMINUS
    | tBANG
    | tBITXOR
    | tBITAND
    | tTIMES
    | tCHAN
    ;

PrimaryExpression: tIDENTIFIER
    | Literal
    | tLPAREN Expression tRPAREN
    | PrimaryExpression tPERIOD tIDENTIFIER
    | PrimaryExpression tLSBRACE ExpressionList tRSBRACE
    | PrimaryExpression tLSBRACE ExpressionOrEmpty tCOLON ExpressionOrEmpty tRSBRACE
    | PrimaryExpression tLSBRACE ExpressionOrEmpty tCOLON Expression tCOLON Expression tRSBRACE
    | FunctionCall
    | AppendExpression
    | LenExpression
    | CapExpression
    ;

Literal: tINTVAL
    | tFLOATVAL
    | tSTRINGVAL
    | tRUNEVAL
    | tBOOLVAL
    ;

BinaryOp: tOR
    | tAND
    | RelOp
    | AddOp
    | MulOp
    ;

RelOp: tEQ
    | tNOTEQ
    | tLESS
    | tLTEQ
    | tGREATER
    | tGTEQ
    ;

AddOp: tPLUS
    | tMINUS
    | tBITOR
    | tBITXOR
    ;

MulOp: tTIMES
    | tDIV
    | tMOD
    | tLEFTSHIFT
    | tRIGHTSHIFT
    | tBITAND
    | tBITCLEAR
    ;

FunctionCall: tIDENTIFIER tLPAREN ExpressionList tRPAREN
    | tIDENTIFIER tLPAREN tRPAREN
    ;
    
AppendExpression: tAPPEND tLPAREN Expression tCOMMA Expression tRPAREN
    ;

LenExpression: tLEN tLPAREN Expression tRPAREN
    ;

CapExpression: tCAP tLPAREN Expression tRPAREN
    ;

TypeDecl: tTYPE TypeSpec
    | tTYPE tLPAREN TypeSpecList tRPAREN
    ;

TypeSpec: tIDENTIFIER Type
    ;

TypeSpecList: %empty
    | TypeSpecList TypeSpec tSEMICOLON 
    ;

FuncDecl: tFUNC tIDENTIFIER Signature Block     { $$ = NULL; }
    ;

Block: tLCBRACE StatementList tRCBRACE
    ;

StatementList: Statement tSEMICOLON
    | Statement tSEMICOLON StatementList
    ;

Statement: Declaration 
    | SimpleStatement
    | PrintStatement
    | PrintlnStatement
    | ReturnStatement
    | IfStatement
    | ExprSwitchStatement
    | ForStatement
    | ContinueStatement
    | BreakStatement
    | FallthroughStatement
    ;

SimpleStatement: %empty
    | Expression
    | IncDecStatement
    | AssignStatement
    | ShortVarDecl
    ;

Signature: Parameters
    | Parameters Type
    ;

Parameters: tLPAREN ParameterList tRPAREN
    | tLPAREN tRPAREN
    ;

ParameterList: ParameterDecl
    | ParameterDecl tCOMMA ParameterList
    ;

ParameterDecl: IdentifierList Type
    ;    

SliceType: tLSBRACE tRSBRACE tIDENTIFIER
    ;    

ArrayType: tLSBRACE Expression tRSBRACE tIDENTIFIER
    ;    

StructType: tSTRUCT tLCBRACE FieldDeclList tRCBRACE
    ;    

FieldDeclList: FieldDecl tSEMICOLON
    | FieldDecl tSEMICOLON FieldDeclList
    ;        

FieldDecl: IdentifierList Type
    ;    

AssignStatement: ExpressionList tASSIGN ExpressionList
    | Expression AssignOp Expression
    ;

AssignOp: tPLUSEQ
    | tMINUSEQ
    | tTIMESEQ
    | tDIVEQ
    | tMODEQ
    | tBITANDEQ 
    | tBITOREQ
    | tBITXOREQ
    | tBITDIVEQ
    | tLEFTSHIFTEQ
    | tRIGHTSHIFTEQ
    | tBITCLEAREQ
    ;

IncDecStatement: Expression tINC
    | Expression tDEC
    ;

PrintStatement: tPRINT tLPAREN ExpressionList tRPAREN
    | tPRINT tLPAREN tRPAREN
    ;

PrintlnStatement: tPRINTLN tLPAREN ExpressionList tRPAREN
    | tPRINTLN tLPAREN tRPAREN
    ;

ReturnStatement: tRETURN 
    | tRETURN Expression
    ;

IfStatement: tIF Expression Block ElseIfs
    | tIF SimpleStatement tSEMICOLON Expression Block ElseIfs
    ;

ElseIfs: %empty 
    | tELSE tIF Expression Block ElseIfs
    | tELSE tIF SimpleStatement tSEMICOLON Expression Block ElseIfs
    | tELSE Block
    ;

ExprSwitchStatement: tSWITCH tLCBRACE ExprCaseClauseList tRCBRACE
    | tSWITCH SimpleStatement tSEMICOLON tLCBRACE ExprCaseClauseList tRCBRACE
    | tSWITCH Expression tLCBRACE ExprCaseClauseList tRCBRACE
    | tSWITCH SimpleStatement tSEMICOLON Expression tLCBRACE ExprCaseClauseList tRCBRACE
    ;

ExprCaseClauseList: %empty
    | ExprCaseClause ExprCaseClauseList
    ;    

ExprCaseClause: ExprSwitchCase tCOLON StatementList
    ;

ExprSwitchCase: tCASE ExpressionList
    | tDEFAULT
    ;

ForStatement: tFOR Block
    | tFOR ForClause Block
    | tFOR Expression Block
    ;

ForClause: SimpleStatement tSEMICOLON ExpressionOrEmpty tSEMICOLON SimpleStatement
    ;

ExpressionOrEmpty: %empty
    | Expression
    ;

BreakStatement: tBREAK
    ;
            
ContinueStatement: tCONTINUE
    ;

FallthroughStatement: tFALLTHROUGH
    ;
%%
