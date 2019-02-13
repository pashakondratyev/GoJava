#include <stdio.h>
#include <string.h>

#include "tree.h"

void yyparse();
int yylex();
int g_tokens;
int lineno;

int main(int argc, char *argv[]){
  // Input starts at first line
  lineno = 1;
  // Checks that the number of arguments passed is valid
  if (argc != 2 && argc != 3){
    fprintf(stderr, "Error: invalid number of arguments expected 2\n");
    return 1;
  }
  // Checks that the mode of the compiler is valid, returns an error 
  if (strcmp(argv[1], "scan") == 0){
    while(yylex()){};
    printf("OK\n");
  }
  else if(strcmp(argv[1], "tokens") == 0){
    g_tokens = 1;
    while(yylex()){};
  }
  else if(strcmp(argv[1], "parse") == 0){
    yyparse();
    printf("OK\n");
  }
  else if(strcmp(argv[1], "pretty") == 0){
    yyparse();
    //prettyPrint(root);
  }
  else{
    fprintf(stderr, "Error: invalid argument for compiler mode, valid options are (scan | tokens | parse | pretty)\n");
    return 1;
  }
  return 0;
}
