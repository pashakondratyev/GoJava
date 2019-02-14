%{ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
int g_tokens;
%} 
DIGIT [0-9]
OCTALDIGITS [0-7]
HEXDECSYMBOLS [0-9a-f]

STRINGSYMBOLS [~@#$%^&*\-+/`<>=_|'.,;:!?{}\[\]() ]
ESCAPESYMBOLS \\[abfnrtv\"\\]
%option yylineno
%option noyywrap
%%
  
  /* White Space */
[ \t\n\r]+

  /* Keywords */
"break"        if (g_tokens){printf("tBREAK\n");} return tBREAK;
"case"         if (g_tokens){printf("tCASE\n");} return tCASE;
"chan"         if (g_tokens){printf("tCHAN\n");} return tCHAN;
"const"        if (g_tokens){printf("tCONST\n");} return tCONST;
"continue"     if (g_tokens){printf("tCONTINUE\n");} return tCONTINUE;
"default"      if (g_tokens){printf("tDEFAULT\n");} return tDEFAULT;
"defer"        if (g_tokens){printf("tDEFER\n");} return tDEFER;
"else"         if (g_tokens){printf("tELSE\n");} return tELSE;
"fallthrough"  if (g_tokens){printf("tFALLTHROUGH\n");} return tFALLTHROUGH;
"for"          if (g_tokens){printf("tFOR\n");} return tFOR;
"func"         if (g_tokens){printf("tFUNC\n");} return tFUNC;
"go"           if (g_tokens){printf("tGO\n");} return tGO;
"goto"         if (g_tokens){printf("tGOTO\n");} return tGOTO;
"if"           if (g_tokens){printf("tIF\n");} return tIF;
"import"       if (g_tokens){printf("tIMPORT\n");} return tIMPORT;
"interface"    if (g_tokens){printf("tINTERFACE\n");} return tINTERFACE;
"map"          if (g_tokens){printf("tMAP\n");} return tMAP;
"package"      if (g_tokens){printf("tPACKAGE\n");} return tPACKAGE;
"range"        if (g_tokens){printf("tRANGE\n");} return tRANGE;
"return"       if (g_tokens){printf("tRETURN\n");} return tRETURN;
"select"       if (g_tokens){printf("tSELECT\n");} return tSELECT;
"struct"       if (g_tokens){printf("tSTRUCT\n");} return tSTRUCT;
"switch"       if (g_tokens){printf("tSWITCH\n");} return tSWITCH;
"type"         if (g_tokens){printf("tTYPE\n");} return tTYPE;
"var"          if (g_tokens){printf("tVAR\n");} return tVAR;
"print"        if (g_tokens){printf("tPRINT\n");} return tPRINT;
"println"      if (g_tokens){printf("tPRINTLN\n");} return tPRINTLN;
"append"       if (g_tokens){printf("tAPPEND\n");} return tAPPEND;
"len"          if (g_tokens){printf("tLEN\n");} return tLEN;
"cap"          if (g_tokens){printf("tCAP\n");} return tCAP; 

  /* Types */
"rune"         if (g_tokens){printf("tRUNE\n");} return tRUNE;
"string"       if (g_tokens){printf("tSTRING\n");} return tSTRING;
"int"          if (g_tokens){printf("tFLOAT\n");} return tFLOAT;
"float"        if (g_tokens){printf("tFLOAT\n");} return tFLOAT;
"bool"         if (g_tokens){printf("tBOOLEAN\n");} return tBOOLEAN;

  /* Arithmetic Operators */
"+"            if (g_tokens){printf("tPLUS\n");} return tPLUS;
"-"            if (g_tokens){printf("tMINUS\n");} return tMINUS;
"*"            if (g_tokens){printf("tTIMES\n");} return tTIMES;
"/"            if (g_tokens){printf("tDIV\n");} return tDIV;
"%"            if (g_tokens){printf("tMOD\n");} return tMOD;

"&"            if (g_tokens){printf("tBITAND\n");} return tBITAND;
"|"            if (g_tokens){printf("tBITOR\n");} return tBITOR;
"^"            if (g_tokens){printf("tBITXOR\n");} return tBITXOR;
"&^"           if (g_tokens){printf("tBITCLEAR\n");} return tBITCLEAR;

"<<"           if (g_tokens){printf("tLEFTSHIFT\n");} return tLEFTSHIFT;
">>"           if (g_tokens){printf("tRIGHTSHIFT\n");} return tRIGHTSHIFT;
 
  /* Boolean Operators */
"&&"           if (g_tokens){printf("tAND\n");} return tAND;
"||"           if (g_tokens){printf("tOR\n");} return tOR;
"!"            if (g_tokens){printf("tBANG\n");} return tBANG;

  /* Assignment */
"="            if (g_tokens){printf("tASSIGN\n");} return tASSIGN;
":="           if (g_tokens){printf("tDECL\n");} return tDECL;
"+="           if (g_tokens){printf("tPLUSEQ\n");} return tPLUSEQ;
"-="           if (g_tokens){printf("tMINUSEQ\n");} return tMINUSEQ;
"*="           if (g_tokens){printf("tTIMESEQ\n");} return tTIMESEQ;
"/="           if (g_tokens){printf("tDIVEQ\n");} return tDIVEQ;
"%="           if (g_tokens){printf("tMODEQ\n");} return tMODEQ;
"&="           if (g_tokens){printf("tBITANDEQ\n");} return tBITANDEQ;
"|="           if (g_tokens){printf("tBITOREQ\n");} return tBITOREQ;
"^="           if (g_tokens){printf("tBITXOREQ\n");} return tBITXOREQ;
"<<="          if (g_tokens){printf("tLEFTSHIFTEQ\n");} return tLEFTSHIFTEQ;
">>="          if (g_tokens){printf("tRIGHTSHIFTEQ\n");} return tRIGHTSHIFTEQ;
"&^="          if (g_tokens){printf("tBITCLEAREQ\n");} return tBITCLEAREQ;

"++"           if (g_tokens){printf("tINC\n");} return tINC;
"--"           if (g_tokens){printf("tDEC\n");} return tDEC;

"<-"           if (g_tokens){printf("tCHANNEL\n");} return tCHANNEL;
  
  /* Logical Comparisons */
"=="           if (g_tokens){printf("tEQ\n");} return tEQ;
"<"            if (g_tokens){printf("tLESS\n");} return tLESS;
">"            if (g_tokens){printf("tGREATER\n");} return tGREATER;
"!="           if (g_tokens){printf("tNOTEQ\n");} return tNOTEQ;
"<="           if (g_tokens){printf("tLTEQ\n");} return tLTEQ;
">="           if (g_tokens){printf("tGTEQ\n");} return tGTEQ;

"("            if (g_tokens){printf("tLPAREN\n");} return tLPAREN;
")"            if (g_tokens){printf("tRPAREN\n");} return tRPAREN;
"["            if (g_tokens){printf("tLSBRACE\n");} return tLSBRACE;
"]"            if (g_tokens){printf("tRSBRACE\n");} return tRSBRACE;
"{"            if (g_tokens){printf("tLCBRACE\n");} return tLCBRACE;
"}"            if (g_tokens){printf("tRCBRACE\n");} return tRCBRACE;

","            if (g_tokens){printf("tCOMMA\n");} return tCOMMA;
"."            if (g_tokens){printf("tPERIOD\n");} return tPERIOD;
":"            if (g_tokens){printf("tCOLON\n");} return tCOLON;
";"            if (g_tokens){printf("tSEMICOLON\n");} return tSEMICOLON;
"..."          if (g_tokens){printf("tELIP\n");} return tELIP;

  /* Valid Integers, includes octal and hexadecimal*/
0|([1-9]{DIGIT}*)|(0{OCTALDIGITS}+)|(0(x|X){HEXDECSYMBOLS}+) { 
  if (g_tokens){printf("tINTVAL(%s)\n", yytext); }
  // Note: for strtol if base is 0, then it assumes the base based on if it starts with 0 or 0(x|X)
  yylval.intval = strtol(yytext, NULL, 0);
  return tINTVAL;
}

  /* Invalid Integers */
{DIGIT}* { 
  fprintf(stderr, "Error: (line %d) invalid integer '%s'\n", yylineno, yytext); 
  exit(1);
}

  /* Valid Floating Points */
({DIGIT}*\.{DIGIT}*) {
  if (g_tokens){printf("tFLOATVAL(%s)\n", yytext); }
  yylval.floatval = atof(yytext);
  return tFLOATVAL;
}

  /* Valid Identifiers */
[a-zA-Z_][a-zA-Z0-9_]* { 
  if (g_tokens){printf("tIDENTIFIER(%s)\n", yytext); }
  yylval.identifier = strdup(yytext);
  return tIDENTIFIER;
}

  /* Comments */
"//".* { /* empty because comment */ }

  /* Valid Strings */
\"([a-zA-Z0-9]|{STRINGSYMBOLS}|{ESCAPESYMBOLS})*\" {
  if (g_tokens){printf("tSTRINGVAL(%s)\n", yytext); }
  yylval.stringval = strdup(yytext);
  return tSTRINGVAL;
}

  /* Invalid Strings - Commented out because causing errors in case "string" + "string" 
\".*\" {
  fprintf(stderr, "error: (line %d) string with invalid characters %s\n", yylineno, yytext);
  exit(1);
} 
  */

  /* Everything else */
. { 
  fprintf(stderr, "Error: (line %d) unexpected character ‘%s’\n", yylineno, yytext);
  exit(1); 
}
%%
