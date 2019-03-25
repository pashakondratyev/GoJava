#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "symbol.h"
#include "type.h"
#include "code.h"

FILE *outputFile;
int numInitFunc = 0;

void writeTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    fprintf(outputFile, "\t");
  }
}

// prefix all identifiers
char* prefix(char* str) {
	char* prefix = "__golite__";
	char* result = malloc((strlen(str)+strlen(prefix)+1)*sizeof(char));
	sprintf(result, "%s%s", prefix, str);
	return result;
}


int indexLastForwardSlash(char *str){
	int index = -1;
  for(int i = 0; i <= strlen(str); i++) {
  	if(str[i] == '/')  
  		index = i;	
	}
   	return index;
 }

void codeProgram(PROG *prog, SymbolTable *st, char *inputFileName) {
	// create file name and open it
	char *outputFileName = malloc((strlen(inputFileName)+6) *sizeof(char));
	strncpy(outputFileName, inputFileName, strlen(inputFileName));
	sprintf(outputFileName, "%s.java", inputFileName);
	outputFile = fopen(outputFileName, "w");
	free(outputFileName);
	if (outputFile == NULL) {
 		fprintf(stderr, "Can't open output file!\n");
  		exit(1);
	}

	// ignore package declaration

	// set class name as the file name excluding the path
  	int index = indexLastForwardSlash(inputFileName);
  	codeSetup(&inputFileName[index+1]);

  	codeDeclarations(prog->root_decl, st, 1);
  	codeComplete();
  	fclose(outputFile);
}



// setup classes and other defaults
void codeSetup(char *className) {
	// TODO: complete
	// class name must match file name
	fprintf(outputFile, "public class %s {\n", className);
	// define Go boolean variables
	fprintf(outputFile, "\tpublic boolean __golite__true = true;\n");
	fprintf(outputFile, "\tpublic boolean __golite__false = false;\n");
}

// complete class definition
void codeComplete() {
	// TODO: complete
	fprintf(outputFile, "\tpublic static void main(String[] args) {\n");
	for (int i=0; i<numInitFunc; i++) {
		fprintf(outputFile, "\t\t__golite__init_%d();\n", numInitFunc);
	}
	fprintf(outputFile, "\t\t__golite__main();\n" );
	fprintf(outputFile, "\t}\n");
	fprintf(outputFile, "}\n");
}

void codeDeclarations(DECL *dcl, SymbolTable *st, int tabCount) {
	if (dcl != NULL) {
    switch (dcl->kind) {
      case dk_var:
        codeVarDecl(dcl->val.varSpecs, st, tabCount);
        break;
      case dk_short:
        codeShortDecl(dcl->val.shortSpecs, st, tabCount);
        break;
      case dk_type:
        codeTypeDecl(dcl->val.typeSpecs, st, tabCount);
        break;
      case dk_func:
        codeFuncDecl(dcl->val.funcDecl, st, tabCount);
        break;
    }
    codeDeclarations(dcl->next, st, tabCount);
  }
}

void codeVarDecl(VAR_SPECS *vs, SymbolTable *st, int tabCount) {
	// TODO: implement
}

void codeShortDecl(SHORT_SPECS *ss, SymbolTable *st, int tabCount) {
	// TODO: implement
}

void codeTypeDecl(TYPE_SPECS *ts, SymbolTable *st, int tabCount) {
	// TODO: implement
}

char *getJavaTypeString(char *buffer, TYPE *type){
	return buffer;
}

void codeFuncDecl(FUNC_DECL *fd, SymbolTable *st, int tabCount) {
	char buffer[1024];
	//If this is not a reference we need to handle this specially
	char *returnTypeString = fd->returnType == NULL ? "void" : getTypeString(buffer, fd->returnType);
	fprintf(outputFile, "public %s %s (", returnTypeString, fd->name);
	//print args
	fprintf(outputFile, ") {\n");
	// TODO: implement
}

void codeStmt(STMT *stmt, SymbolTable *st, TYPE *returnType, int tabCount) {
	// TODO: implement
}

void codeExp(EXP *exp, SymbolTable *st, int tabCount) {
	// TODO: complete
  	if (exp != NULL) {
    	switch (exp->kind) {
	      case ek_id:
	       	if (strcmp(exp->val.id, "_") == 0) {
	       		// TODO: do something with blank id
	       		break;
	       	}
	       	fprintf(outputFile, "%s", prefix(exp->val.id));
	       	break;
	    	case ek_float:
	    		fprintf(outputFile, "%f", exp->val.floatval);
	        break;
	      case ek_int:
	        fprintf(outputFile, "%i", exp->val.intval);
	        break;
	      case ek_string:
	        fprintf(outputFile, "\"%s\"", exp->val.stringval);
	        break;
	      case ek_boolean:
	      	// TODO: check if want to use GoLite specific boolean values
	        fprintf(outputFile, "%s", exp->val.booleanval ? "__golite__true" : "__golite__false");
	        break;
	      case ek_rune:
	        fprintf(outputFile, "'%c'", exp->val.runeval);
	        break;
      	case ek_plus:
      		// TODO: if char cast to char at the end. May not be needed in Java
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " + ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
      	case ek_minus:
      		fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " - ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
      	case ek_times:
      		fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " * ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
      	case ek_div:
      		fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " / ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
      	case ek_eq:
      		// TODO: fix for strings, arrays, slices, and structs
      		fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " == ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
     	 	case ek_ne:
     	 		// TODO: fix for strings, arrays, slices, and structs
        	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " != ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_ge:
	      	// TODO: fix for strings
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " >= ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_le:
	      	// TODO: fix for strings
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " <= ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_gt:
	      	// TODO: fix for strings
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " > ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_lt:
	      	// TODO: fix for strings
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " < ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_and:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " && ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_or:
	     		fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " || ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_mod:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " %% ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitAnd:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " & ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitOr:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " | ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitXor:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " ^ ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitLeftShift:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " << ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitRightShift:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " >> ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, ")");
	        break;
	      case ek_bitClear:
	      	fprintf(outputFile, "(");
	        codeExp(exp->val.binary.lhs, st, tabCount);
	        fprintf(outputFile, " & (~ ");
	        codeExp(exp->val.binary.rhs, st, tabCount);
	        fprintf(outputFile, "))");
	      	break;
	      case ek_uplus:
	      	TYPE *type = exp->val.unary.exp.type;
	      	while (type->kind = tk_ref) {
	      		type = resolveType(type, st);
	      	}
	      	if (type->kind == tk_rune) {	// rune
	      		fprintf(outputFile, "((char)(+");
	        	codeExp(exp->val.unary.exp, st, tabCount);
	        	fprintf(outputFile, "))");
	      	} else {	// int, float64
	      		fprintf(outputFile, "(+");
	        	codeExp(exp->val.unary.exp, st, tabCount);
	        	fprintf(outputFile, ")");
	        }
	        break;
	      case ek_uminus:
	      	TYPE *type = exp->val.unary.exp.type;
	      	while (type->kind = tk_ref) {
	      		type = resolveType(type, st);
	      	}
	      	if (type->kind == tk_rune) {	// rune
	      		fprintf(outputFile, "((char)(-");
	        	codeExp(exp->val.unary.exp, st, tabCount);
	        	fprintf(outputFile, "))");
	      	} else {	// int, float64
	      		fprintf(outputFile, "(-");
	        	codeExp(exp->val.unary.exp, st, tabCount);
	        	fprintf(outputFile, ")");
	        }
	        break;
     	 	case ek_bang:
        	fprintf(outputFile, "(!");
	        codeExp(exp->val.unary.exp, st, tabCount);
	        fprintf(outputFile, ")");
	        break;	
      	case ek_ubitXor:
        	fprintf(outputFile, "(~");
	        codeExp(exp->val.unary.exp, st, tabCount);
	        fprintf(outputFile, ")");
      		break;
      	case ek_func:
        	// TODO: complete
       		break;
     		case ek_append:
        	// TODO: complete
       		break;
      	case ek_len:
        	// TODO: complete
       		break;
      	case ek_cap:
       		// TODO: complete
       		break;
      	case ek_indexExp:
        	// TODO: complete
       		break;
      	case ek_structField:
        	// TODO: complete
       		break;
      	case ek_paren:
	        fprintf(outputFile, "(");
	        codeExp(exp->val.parenExp, st, tabCount);
	        fprintf(outputFile, ")");
       		break;
      	case ek_conv:
        	// TODO: complete
       		break;
    }
  }

}