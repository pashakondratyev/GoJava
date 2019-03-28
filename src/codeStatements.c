#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeStatements.h"

#include "code.h"
#include "codeStructs.h"
#include "symbol.h"
#include "type.h"

#define DEBUG 0

void codeStmt(STMT *stmt, SymbolTable *st, int tabCount) {
  // TODO: implement
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
        fprintf(stderr, "Error: fallthough not supported.\n");
        break;
      case sk_empty:
        // empty statement
        break;
    }
  }
}