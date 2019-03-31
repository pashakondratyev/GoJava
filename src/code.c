#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "code.h"
#include "codeIdentifiers.h"
#include "codeDeclarations.h"
#include "codeStatements.h"
#include "codeStructs.h"
#include "codeTypes.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

void writeTab(int tabCount) {
  for (int i = 0; i < tabCount; i++) {
    fprintf(outputFile, "\t");
  }
}

// prefix all identifiers
char *prefix(char *str) {
  char *prefix = "__golite__";
  char *result = malloc((strlen(str) + strlen(prefix) + 1) * sizeof(char));
  sprintf(result, "%s%s", prefix, str);
  return result;
}

int indexLastForwardSlash(char *str) {
  int index = -1;
  for (int i = 0; i <= strlen(str); i++) {
    if (str[i] == '/') index = i;
  }
  return index;
}

void removeNonAlphaNum(char *str, int startIndex) {
    unsigned long i = startIndex;
    unsigned long j = startIndex;
    char c;
    while ((c = str[i++]) != '\0') {
        if (isalnum(c)) {
            str[j++] = c;
        }
    }
    str[j] = '\0';
}

void codeProgram(PROG *prog, SymbolTable *st, char *inputFileName) {
  // create file name and open it
  numInitFunc = 0;
  identifierCount = 0;
  blankVar = 0;
  removeNonAlphaNum(inputFileName, indexLastForwardSlash(inputFileName)+1);
  char *outputFileName = malloc((strlen(inputFileName) + 6) * sizeof(char));
  strncpy(outputFileName, inputFileName, strlen(inputFileName));
  sprintf(outputFileName, "%s.java", inputFileName);
  outputFile = fopen(outputFileName, "w");
  free(outputFileName);
  if (outputFile == NULL) {
    fprintf(stderr, "Can't open output file!\n");
    exit(1);
  }

  // ignore package declaration

  // import for array equality
  fprintf(outputFile, "import java.util.Arrays;\n\n");  

  // set class name as the file name excluding the path
  int index = indexLastForwardSlash(inputFileName);
  identifierTable = initIdentifierTable();
  initStructTable();
  // Creates all the structs necessary as java objects that can be reused later
  makeStructTable(prog->root_decl, st);
  // Outputs the created java objects into the outtput file by traversing
  //printStructTable();
  codeStructTable();
  codeSetup(&inputFileName[index + 1]);

  codeDeclarations(prog->root_decl, st, identifierTable, 1);
  codeComplete();
  fclose(outputFile);
}

// setup classes and other defaults
void codeSetup(char *className) {
  // TODO: complete
  // class name must match file name
  fprintf(outputFile, "public class %s {\n", className);
  // define Go boolean variables
  fprintf(outputFile, "\tpublic static Boolean __golite__true = Boolean.TRUE;\n");
  fprintf(outputFile, "\tpublic static Boolean __golite__false = Boolean.FALSE;\n\n");
  // utility class to handle casts/type conversions
  fprintf(outputFile, "\tpublic static Cast castUtil = new Cast();\n");
}

// complete class definition
void codeComplete() {
  // TODO: complete
  fprintf(outputFile, "\tpublic static void main(String[] args) {\n");
  for (int i = 0; i < numInitFunc; i++) {
    fprintf(outputFile, "\t\t__golite__init_%d();\n", i);
  }
  fprintf(outputFile, "\t\t__golite__main();\n");
  fprintf(outputFile, "\t}\n");
  fprintf(outputFile, "}\n");
}
