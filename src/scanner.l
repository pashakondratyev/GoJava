%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
int g_tokens;

enum yytokentype lastToken;

void saveToken(enum yytokentype type){
  lastToken = type;
}

bool semicolonRequired(){
  switch(lastToken){
    case tINTVAL:
    case tFLOATVAL:
    case tBOOLVAL:
    case tRUNEVAL:
    case tSTRINGVAL:
    case tIDENTIFIER:
    case tBREAK:
    case tCONTINUE:
    case tFALLTHROUGH:
    case tRETURN:
    case tINC:
    case tDEC:
    case tRPAREN:
    case tRCBRACE:
    case tRSBRACE:
      return true;
    default:
      return false;
  }
}

char getEscaped(char c){
  char rune;
  switch(c){
    case 'a':
      rune = '\a';
      break;
    case 'b':
      rune = '\b';
      break;
    case 'f':
      rune = '\f';
      break;
    case 'n':
      rune = '\n';
      break;
    case 'r':
      rune = '\r';
      break;
    case 't':
      rune = '\t';
      break;
    case 'v':
      rune = '\v';
      break;
    case '\\':
      rune = '\\';
      break;
    case '\'':
      rune = '\'';
      break;
    default:
      
      rune = c;
  }
  return rune;
}
%}
DIGIT [0-9]
OCTALDIGITS [0-7]
HEXDECSYMBOLS [0-9a-f]

STRINGSYMBOLS [~@#$%^&*\-+/`<>=_|'.,;:!?{}\[\]() ]
RUNESYMBOLS [~@#$%^&*\-+/`<>=_|.,;:!?{}\[\]() ]
ALPHNUM [a-zA-Z0-9]
ESCAPESYMBOLS \\[abfnrtv\\]
%option yylineno
%option noyywrap

%x IN_COMMENT
%%
  /* White Space */
([\n\r]) {
  if(semicolonRequired()){
    lastToken = tSEMICOLON;
    if(g_tokens) printf("tSEMICOLON\n");
    return tSEMICOLON;
  }
}


[ \t\r]+

  /* Keywords */
"break"        if (g_tokens){printf("tBREAK\n");} saveToken(tBREAK); return tBREAK;
"case"         if (g_tokens){printf("tCASE\n");} saveToken(tCASE); return tCASE;
"chan"         if (g_tokens){printf("tCHAN\n");} saveToken(tCHAN); return tCHAN;
"const"        if (g_tokens){printf("tCONST\n");} saveToken(tCONST); return tCONST;
"continue"     if (g_tokens){printf("tCONTINUE\n");} saveToken(tCONTINUE); return tCONTINUE;
"default"      if (g_tokens){printf("tDEFAULT\n");} saveToken(tDEFAULT); return tDEFAULT;
"defer"        if (g_tokens){printf("tDEFER\n");} saveToken(tDEFER); return tDEFER;
"else"         if (g_tokens){printf("tELSE\n");} saveToken(tELSE); return tELSE;
"fallthrough"  if (g_tokens){printf("tFALLTHROUGH\n");} saveToken(tFALLTHROUGH); return tFALLTHROUGH;
"for"          if (g_tokens){printf("tFOR\n");} saveToken(tFOR); return tFOR;
"func"         if (g_tokens){printf("tFUNC\n");} saveToken(tFUNC); return tFUNC;
"go"           if (g_tokens){printf("tGO\n");} saveToken(tGO); return tGO;
"goto"         if (g_tokens){printf("tGOTO\n");} saveToken(tGOTO); return tGOTO;
"if"           if (g_tokens){printf("tIF\n");} saveToken(tIF); return tIF;
"import"       if (g_tokens){printf("tIMPORT\n");} saveToken(tIMPORT); return tIMPORT;
"interface"    if (g_tokens){printf("tINTERFACE\n");} saveToken(tINTERFACE); return tINTERFACE;
"map"          if (g_tokens){printf("tMAP\n");} saveToken(tMAP); return tMAP;
"package"      if (g_tokens){printf("tPACKAGE\n");} saveToken(tPACKAGE); return tPACKAGE;
"range"        if (g_tokens){printf("tRANGE\n");} saveToken(tRANGE); return tRANGE;
"return"       if (g_tokens){printf("tRETURN\n");} saveToken(tRETURN); return tRETURN;
"select"       if (g_tokens){printf("tSELECT\n");} saveToken(tSELECT); return tSELECT;
"struct"       if (g_tokens){printf("tSTRUCT\n");} saveToken(tSTRUCT); return tSTRUCT;
"switch"       if (g_tokens){printf("tSWITCH\n");} saveToken(tSWITCH); return tSWITCH;
"type"         if (g_tokens){printf("tTYPE\n");} saveToken(tTYPE); return tTYPE;
"var"          if (g_tokens){printf("tVAR\n");} saveToken(tVAR); return tVAR;
"print"        if (g_tokens){printf("tPRINT\n");} saveToken(tPRINT); return tPRINT;
"println"      if (g_tokens){printf("tPRINTLN\n");} saveToken(tPRINTLN); return tPRINTLN;
"append"       if (g_tokens){printf("tAPPEND\n");} saveToken(tAPPEND); return tAPPEND;
"len"          if (g_tokens){printf("tLEN\n");} saveToken(tLEN); return tLEN;
"cap"          if (g_tokens){printf("tCAP\n");} saveToken(tCAP); return tCAP;

  /* Arithmetic Operators */
"+"            if (g_tokens){printf("tPLUS\n");} saveToken(tPLUS); return tPLUS;
"-"            if (g_tokens){printf("tMINUS\n");} saveToken(tMINUS); return tMINUS;
"*"            if (g_tokens){printf("tTIMES\n");} saveToken(tTIMES); return tTIMES;
"/"            if (g_tokens){printf("tDIV\n");} saveToken(tDIV); return tDIV;
"%"            if (g_tokens){printf("tMOD\n");} saveToken(tMOD); return tMOD;
"&"            if (g_tokens){printf("tBITAND\n");} saveToken(tBITAND); return tBITAND;
"|"            if (g_tokens){printf("tBITOR\n");} saveToken(tBITOR); return tBITOR;
"^"            if (g_tokens){printf("tBITXOR\n");} saveToken(tBITXOR); return tBITXOR;
"&^"           if (g_tokens){printf("tBITCLEAR\n");} saveToken(tBITCLEAR); return tBITCLEAR;
"<<"           if (g_tokens){printf("tLEFTSHIFT\n");} saveToken(tLEFTSHIFT); return tLEFTSHIFT;
">>"           if (g_tokens){printf("tRIGHTSHIFT\n");} saveToken(tRIGHTSHIFT); return tRIGHTSHIFT;

  /* Boolean Operators */
"&&"           if (g_tokens){printf("tAND\n");} saveToken(tAND); return tAND;
"||"           if (g_tokens){printf("tOR\n");} saveToken(tOR); return tOR;
"!"            if (g_tokens){printf("tBANG\n");} saveToken(tBANG); return tBANG;

  /* Assignment */
"="            if (g_tokens){printf("tASSIGN\n");} saveToken(tASSIGN); return tASSIGN;
":="           if (g_tokens){printf("tDECL\n");} saveToken(tDECL); return tDECL;
"+="           if (g_tokens){printf("tPLUSEQ\n");} saveToken(tPLUSEQ); return tPLUSEQ;
"-="           if (g_tokens){printf("tMINUSEQ\n");} saveToken(tMINUSEQ); return tMINUSEQ;
"*="           if (g_tokens){printf("tTIMESEQ\n");} saveToken(tTIMESEQ); return tTIMESEQ;
"/="           if (g_tokens){printf("tDIVEQ\n");} saveToken(tDIVEQ); return tDIVEQ;
"%="           if (g_tokens){printf("tMODEQ\n");} saveToken(tMODEQ); return tMODEQ;
"&="           if (g_tokens){printf("tBITANDEQ\n");} saveToken(tBITANDEQ); return tBITANDEQ;
"|="           if (g_tokens){printf("tBITOREQ\n");} saveToken(tBITOREQ); return tBITOREQ;
"^="           if (g_tokens){printf("tBITXOREQ\n");} saveToken(tBITXOREQ); return tBITXOREQ;
"<<="          if (g_tokens){printf("tLEFTSHIFTEQ\n");} saveToken(tLEFTSHIFTEQ); return tLEFTSHIFTEQ;
">>="          if (g_tokens){printf("tRIGHTSHIFTEQ\n");} saveToken(tRIGHTSHIFTEQ); return tRIGHTSHIFTEQ;
"&^="          if (g_tokens){printf("tBITCLEAREQ\n");} saveToken(tBITCLEAREQ); return tBITCLEAREQ;
"++"           if (g_tokens){printf("tINC\n");} saveToken(tINC); return tINC;
"--"           if (g_tokens){printf("tDEC\n");} saveToken(tDEC); return tDEC;
"<-"           if (g_tokens){printf("tCHANNEL\n");} saveToken(tCHANNEL); return tCHANNEL;

  /* Logical Comparisons */
"=="           if (g_tokens){printf("tEQ\n");} saveToken(tEQ); return tEQ;
"<"            if (g_tokens){printf("tLESS\n");} saveToken(tLESS); return tLESS;
">"            if (g_tokens){printf("tGREATER\n");} saveToken(tGREATER); return tGREATER;
"!="           if (g_tokens){printf("tNOTEQ\n");} saveToken(tNOTEQ); return tNOTEQ;
"<="           if (g_tokens){printf("tLTEQ\n");} saveToken(tLTEQ); return tLTEQ;
">="           if (g_tokens){printf("tGTEQ\n");} saveToken(tGTEQ); return tGTEQ;
"("            if (g_tokens){printf("tLPAREN\n");} saveToken(tLPAREN); return tLPAREN;
")"            if (g_tokens){printf("tRPAREN\n");} saveToken(tRPAREN); return tRPAREN;
"["            if (g_tokens){printf("tLSBRACE\n");} saveToken(tLSBRACE); return tLSBRACE;
"]"            if (g_tokens){printf("tRSBRACE\n");} saveToken(tRSBRACE); return tRSBRACE;
"{"            if (g_tokens){printf("tLCBRACE\n");} saveToken(tLCBRACE); return tLCBRACE;
"}"            if (g_tokens){printf("tRCBRACE\n");} saveToken(tRCBRACE); return tRCBRACE;
","            if (g_tokens){printf("tCOMMA\n");} saveToken(tCOMMA); return tCOMMA;
"."            if (g_tokens){printf("tPERIOD\n");} saveToken(tPERIOD); return tPERIOD;
":"            if (g_tokens){printf("tCOLON\n");} saveToken(tCOLON); return tCOLON;
";"            if (g_tokens){printf("tSEMICOLON\n");} saveToken(tSEMICOLON); return tSEMICOLON;
"..."          if (g_tokens){printf("tELIP\n");} saveToken(tELIP); return tELIP;

  /* Valid Integers, includes octal and hexadecimal*/
0|([1-9]{DIGIT}*)|(0{OCTALDIGITS}+)|(0(x|X){HEXDECSYMBOLS}+) { 
  if (g_tokens){printf("tINTVAL(%s)\n", yytext); }
  // Note: for strtol if base is 0, then it assumes the base based on if it starts with 0 or 0(x|X)
  yylval.intval = strtol(yytext, NULL, 0);
  saveToken(tINTVAL);
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
  saveToken(tFLOATVAL);
  return tFLOATVAL;
}

  /* Valid Identifiers */
[a-zA-Z_][a-zA-Z0-9_]* { 
  if (g_tokens){printf("tIDENTIFIER(%s)\n", yytext); }
  yylval.identifier = strdup(yytext);
  saveToken(tIDENTIFIER);
  return tIDENTIFIER;
}

  /* Comments */
"//".* { /* empty because comment */ }

  /* Multiline C-Style comments, from FLEX documentation */
"/*"      BEGIN(IN_COMMENT);
<IN_COMMENT><<EOF>>   fprintf(stderr, "Error: (line %d) block comment not closed\n", yylineno); exit(1);
<IN_COMMENT>"*/"      BEGIN(INITIAL);
<IN_COMMENT>[^*\n]+   // eat comment in chunk
<IN_COMMENT>"*"       // eat the lone star
<IN_COMMENT>\n        yylineno++; if(semicolonRequired()){saveToken(tSEMICOLON); return tSEMICOLON;} //TODO: check if this is necessary , insert semicolon

"'"({ALPHNUM}|{RUNESYMBOLS}|{ESCAPESYMBOLS}|"\\\'")"'" {
  char rune;
  if(yytext[1] == '\\'){
    rune = getEscaped(yytext[2]);
  }
  else {
    rune = yytext[1];
  }

  if (g_tokens){printf("tRUNEVAL(%s)\n", yytext);}
  yylval.runeval = rune;
  saveToken(tRUNEVAL);
  return tRUNEVAL;
}

  /* Valid Strings */
\"({ALPHNUM}|{STRINGSYMBOLS}|{ESCAPESYMBOLS}|\\\")*\" {
  // Make one pass to count how long the string is if we count escapes
  int len = 0;
  for(int i = 0; yytext[i] != '\0'; i++){
    len++; //Escapes are not counted in the length
    if(yytext[i] == '\\'){
      //So we count the escaped characters as one character and not two
      i++;
    }
  }
  char *newString = (char*)malloc((len)*sizeof(char*));
  int escapes = 0;
  for(int i = 0; i < len; i++){
    if(yytext[i + escapes] == '\\'){
      escapes++;
      newString[i] = getEscaped(yytext[i+escapes]);
    }
    else{
      newString[i] = yytext[i+escapes];
    }
  }

  if (g_tokens){printf("tSTRINGVAL(%s)\n", yytext); }
  // Don't need to call strdup because for each string we are mallocing more space
  yylval.stringval = newString;
  saveToken(tIDENTIFIER);
  return tSTRINGVAL;
}

`[^`]*` {
  if (g_tokens){printf("tSTRINGVAL(%s)\n", yytext);}
  yylval.stringval = strdup(yytext);
  saveToken(tSTRINGVAL);
  return tSTRINGVAL;
}
  /* Invalid Strings - Commented out because causing errors in case "string" + "string" 
\".*\" {
  fprintf(stderr, "Error: (line %d) string with invalid characters %s\n", yylineno, yytext);
  exit(1);
} 
  */

<<EOF>> {
  if(semicolonRequired()){
    lastToken = tSEMICOLON;
    if(g_tokens) printf("tSEMICOLON\n");
    return tSEMICOLON;
  }
  yyterminate();
}

  /* Everything else */
. { 
  fprintf(stderr, "Error: (line %d) unexpected character ‘%s’\n", yylineno, yytext);
  exit(1); 
}
%%
