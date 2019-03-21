#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "type.h"

void typeProgram(PROG *prog, SymbolTable *st) {
  if (prog != NULL) typeDeclarations(prog->root_decl, st);
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

void typeVarDecl(VAR_SPECS *vs, SymbolTable *st) {
  if (vs != NULL) {
    // If rhs is not null
    if (vs->exp != NULL) {
      typeExp(vs->exp, st);
      if (vs->exp->type == NULL) {
        fprintf(stderr, "Error: (line %d) void cannot be used as a value in a declaration\n", vs->exp->lineno);
        exit(1);
      }

      if (vs->type != NULL) {
        vs->type = fixType(st, vs->type);
      } else if (strcmp(vs->id, "_") != 0) {
        // We need to resolve the type
        SYMBOL *s = getSymbol(st, vs->id);
        s->val.type = vs->exp->type;
        vs->type = vs->exp->type;
      }

      if (strcmp(vs->id, "_") != 0) {
        // SYMBOL *s = getSymbol(st, vs->type->val.name);
        // TYPE *t = vs->type->kind == tk_ref ? getSymbol(st, vs->type->val.name)->val.type : vs->type;
        if (!typeCompare(vs->exp->type, vs->type, st)) {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, vs->type);
          getTypeString(buffer2, vs->exp->type);
          fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n",
                  vs->exp->lineno, buffer1, buffer2);
          exit(1);
        }
      }
    } else {
      if (vs->type != NULL) {
        vs->type = fixType(st, vs->type);
      }
    }
    typeVarDecl(vs->next, st);
  }
}

void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st) {
  if (ss != NULL) {
    typeExp(ss->rhs, st);
    if (strcmp(ss->lhs->val.id, "_") != 0) {
      SYMBOL *s = getSymbol(st, ss->lhs->val.id);

      // If the lhs's type is null we can se the type to the type of the exp
      if (s->val.type == NULL) {
        s->val.type = ss->rhs->type;
      }

      if (ss->rhs->type == NULL) {
        fprintf(stderr, "Error: (line %d) void cannot be used as a value in a short declaration\n", ss->rhs->lineno);
        exit(1);
      }

      if (s->val.type != NULL && s->val.type != ss->rhs->type) {
        char buffer1[1024];
        char buffer2[1024];

        getTypeString(buffer1, s->val.type);
        getTypeString(buffer2, ss->rhs->type);
        fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n",
                ss->rhs->lineno, buffer1, buffer2);
        exit(1);
      }
    }
    typeShortDecl(ss->next, st);
  }
}

void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st) {
  if (ts != NULL) {
    ts->type = fixType(st, ts->type);
    typeTypeDecl(ts->next, st);
  }
}

void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st) {
  if (fd != NULL) {
    // TODO: Typecheck return statements
    if (fd->returnType != NULL) {
      fd->returnType = fixType(st, fd->returnType);
    }

    ReturnStatus returns = typeStmt(fd->body, st, fd->returnType);
    if ((returns == Breaks || returns == NoReturn) && fd->returnType != NULL) {
      fprintf(stderr, "Error: (line %d) function %s does not have a terminating statement\n", fd->body->lineno,
              fd->name);
      exit(1);
    }
  }
}

/* Takes a statement, symbol table and return type and checks if the stmt type checks and returns
 * Returns 0 if the statement does not return, -1 if it contains a break, and 1 if it returns
 */
ReturnStatus typeStmt(STMT *stmt, SymbolTable *st, TYPE *returnType) {
  ReturnStatus returns = NoReturn;
  STMT_LIST *sl;
  ASSIGN *al;
  EXP_LIST *el;
  CASE_CLAUSE_LIST *ccl;
  if (stmt != NULL) {
    switch (stmt->kind) {
      case sk_block:
        // Change scope to the block
        st = stmt->val.block.scope != NULL ? stmt->val.block.scope : st;
        sl = stmt->val.block.blockStatements;
        while (sl != NULL) {
          // If a statement contained a break or a return we propogate that up to the calling block
          // For infinite for loops, if there is no break we can effectively treat the for
          // loop as a statement which typechecks correctly as it never returns
          // However if there is a break statement then the block is not considered as a returning block
          ReturnStatus temp = typeStmt(sl->stmt, st, returnType);
          if (temp == Breaks) {
            returns = Breaks;
          } else if (returns != Breaks) {
            returns = temp;
          }

          sl = sl->next;
        }
        break;
      case sk_exp:
        typeExp(stmt->val.exp, st);
        break;
      case sk_assign:
        al = stmt->val.assign;
        while (al != NULL) {
          typeExp(al->rhs, st);
          // If we have a blank identifier we only need to type check the rhs;
          if (!isLValue(al->lhs, st)) {
            fprintf(stderr, "Error: (line %d) expected only an assignable field\n", stmt->lineno);
            exit(1);
          }

          if (strcmp(al->lhs->val.id, "_") == 0) break;
          typeExp(al->lhs, st);
          // TODO : compare types when they are compound @Kabilan
          if (!typeCompare(al->lhs->type, al->rhs->type, st)) {
            char buffer1[1024];
            char buffer2[1024];

            getTypeString(buffer1, al->lhs->type);
            getTypeString(buffer2, al->rhs->type);
            fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n",
                    stmt->lineno, buffer1, buffer2);
            exit(1);
          }
          al = al->next;
        }
        break;
      case sk_assignOp:
        typeExp(stmt->val.assignOp.lhs, st);
        typeExp(stmt->val.assignOp.rhs, st);
        if (!typeCompare(stmt->val.assignOp.lhs->type, stmt->val.assignOp.rhs->type, st)) {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, stmt->val.assignOp.lhs->type);
          getTypeString(buffer2, stmt->val.assignOp.rhs->type);
          fprintf(stderr, "Error: (line %d) Both sides of assignment mus be the same [received %s and %s]\n",
                  stmt->lineno, buffer1, buffer2);
          exit(1);
        }
        AssignOpKind kind = stmt->val.assignOp.kind;
        switch (kind) {
          case aok_plus:
            if (!resolvesToNumeric(stmt->val.assignOp.lhs->type, st) &&
                !resolvesToString(stmt->val.assignOp.lhs->type, st)) {
              char buffer[1024];
              getTypeString(buffer, stmt->val.assignOp.lhs->type);
              fprintf(stderr, "Error: (line %d) operator += only works with numeric or string types [received %s]\n",
                      stmt->lineno, buffer);
              exit(1);
            }
            break;
          case aok_minus:
          case aok_times:
          case aok_div:
            if (!resolvesToNumeric(stmt->val.assignOp.lhs->type, st)) {
              char buffer[1024];
              getTypeString(buffer, stmt->val.assignOp.lhs->type);
              fprintf(stderr, "Error: (line %d) Operation %s only works with numeric types [received %s]\n",
                      stmt->lineno, getAssignOpString(kind), buffer);
              exit(1);
            }
            break;
          case aok_mod:
          case aok_bitAnd:
          case aok_bitOr:
          case aok_bitXor:
          case aok_bitLeftShift:
          case aok_bitRightShift:
          case aok_bitClear:
            if (!resolvesToInteger(stmt->val.assignOp.lhs->type, st)) {
              char buffer[1024];
              getTypeString(buffer, stmt->val.assignOp.lhs->type);
              fprintf(stderr, "Error: (line %d) Operation %s, only works with integer types [received %s]\n",
                      stmt->lineno, getAssignOpString(kind), buffer);
              exit(1);
            }
            break;
        }

        break;
      case sk_decl:
        typeDeclarations(stmt->val.decl, st);
        break;
      case sk_shortDecl:
        typeShortDecl(stmt->val.decl->val.shortSpecs, st);
        break;
      case sk_incr:
      case sk_decr:
        typeExp(stmt->val.exp, st);
        if(!isLValue(stmt->val.exp, st)){
          fprintf(stderr, "Error: (line %d) increment/decrement expected lvalue\n", stmt->lineno);
          exit(1);
        }
        if (!resolvesToNumeric(stmt->val.exp->type, st)) {
          fprintf(stderr, "Error: (line %d) cannot increment non numeric type\n", stmt->lineno);
          exit(1);
        }
        break;
      case sk_print:
      case sk_println:
        el = stmt->val.printExps;
        while (el != NULL) {
          typeExp(el->exp, st);
          if (!resolvesToBase(el->exp->type, st)) {
            fprintf(stderr, "Error: (line %d) print statment expected base type\n", stmt->lineno);
            exit(1);
          }
          el = el->next;
        }
        break;
      case sk_return:
        if (returnType == NULL && stmt->val.exp == NULL) {
          return Returns;
        }

        if (returnType != NULL && stmt->val.exp == NULL) {
          fprintf(stderr, "Error: (line %d) invalid return [function has void return type]\n", stmt->lineno);
          exit(1);
        }

        if (returnType == NULL && stmt->val.exp != NULL) {
          fprintf(stderr, "Error: (line %d) invalid return [function has non-void return type]\n", stmt->lineno);
          exit(1);
        }

        if (stmt->val.exp != NULL) {
          typeExp(stmt->val.exp, st);
        }

        if (!typeCompare(returnType, stmt->val.exp->type, st)) {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, returnType);
          getTypeString(buffer2, stmt->val.exp->type);
          fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in return statement\n",
                  stmt->lineno, buffer1, buffer2);
          exit(1);
        }

        returns = Returns;
        break;
      case sk_if:
        st = stmt->val.ifStmt.scope;
        if (stmt->val.ifStmt.simpleStmt != NULL) {
          typeStmt(stmt->val.ifStmt.simpleStmt, st, returnType);
        }
        typeExp(stmt->val.ifStmt.cond, st);
        if (!resolvesToBool(stmt->val.ifStmt.cond->type, st)) {
          char buffer[1024];
          getTypeString(buffer, stmt->val.ifStmt.cond->type);
          fprintf(stderr, "Error: (line %d) Condition evaluates to %s instead of boolean\n", stmt->lineno, buffer);
          exit(1);
        }
        returns = typeStmt(stmt->val.ifStmt.body, st, returnType);
        if (stmt->val.ifStmt.elseStmt != NULL) {
          ReturnStatus temp = typeStmt(stmt->val.ifStmt.elseStmt, st, returnType);
          if (temp == Breaks) {
            returns = Breaks;
          } else if (temp == Returns && returns == Returns) {
            returns = Returns;
          } else {
            returns = NoReturn;
          }
        } else if (returns == Breaks) {
          // if the if statement has a break, we want that information to propogate up
          returns = Breaks;
        } else {
          returns = NoReturn;
        }
        break;
      case sk_else:
        returns = typeStmt(stmt->val.elseBody, st, returnType);
        break;
      case sk_switch:
        st = stmt->val.switchStmt.scope;
        if (stmt->val.switchStmt.simpleStmt != NULL) {
          typeStmt(stmt->val.switchStmt.simpleStmt, st, returnType);
        }
        // typeExp(stmt->val.switchStmt.exp, st);
        ccl = stmt->val.switchStmt.caseClauses;
        int containsDefault = 0;
        if (stmt->val.switchStmt.exp == NULL) {
          returns = ccl == NULL ? NoReturn : Returns;
          // if no expression then all case expressions must be booleans
          while (ccl != NULL) {
            ReturnStatus clauseReturns = NoReturn;
            switch (ccl->clause->kind) {
              case ck_case:
                el = ccl->clause->val.caseClause.cases;
                while (el != NULL) {
                  typeExp(el->exp, st);
                  if (!typeBool(el->exp->type)) {
                    char buffer[1024];
                    getTypeString(buffer, el->exp->type);
                    fprintf(stderr, "Error: (line %d) Expression must evaluate to bool but received %s\n", stmt->lineno,
                            buffer);
                    exit(1);
                  }
                  el = el->next;
                }
                sl = ccl->clause->val.caseClause.clauses;
                while (sl != NULL) {
                  ReturnStatus temp = typeStmt(sl->stmt, st, returnType);

                  if (temp == Breaks) {
                    clauseReturns = Breaks;
                  } else if (clauseReturns != Breaks) {
                    // If there is no break, it checks the last statement
                    clauseReturns = temp;
                  }
                  sl = sl->next;
                }
                break;
              case ck_default:
                containsDefault = 1;
                sl = ccl->clause->val.defaultClauses;
                while (sl != NULL) {
                  ReturnStatus temp = typeStmt(sl->stmt, st, returnType);
                  if (temp == Breaks) {
                    clauseReturns = Breaks;
                  } else if (clauseReturns != Breaks) {
                    clauseReturns = temp;
                  }
                  sl = sl->next;
                }
                break;
            }
            // If the clause has a break
            if (clauseReturns == Breaks) {
              returns = Breaks;
            } else if (returns != Breaks) {
              returns = clauseReturns == returns ? clauseReturns : NoReturn;
            }

            ccl = ccl->next;
          }

        } else {
          // if expression is not empty then all case expressions must have the same type as it
          typeExp(stmt->val.switchStmt.exp, st);
          if (stmt->val.switchStmt.exp->type == NULL) {
            fprintf(stderr, "Error: (line %d) Switch expression must have a type\n", stmt->lineno);
            exit(1);
          } else if (!resolvesToComparable(stmt->val.switchStmt.exp->type, st)) {
            char buffer[1024];
            getTypeString(buffer, stmt->val.switchStmt.exp->type);
            fprintf(stderr, "Error: (line %d) Expression type must be comparable [received %s]\n", stmt->lineno, buffer);
            exit(1);
          }
          returns = ccl == NULL ? NoReturn : Returns;
          while (ccl != NULL) {
            ReturnStatus clauseReturns = NoReturn;
            switch (ccl->clause->kind) {
              case ck_case:
                el = ccl->clause->val.caseClause.cases;
                while (el != NULL) {
                  typeExp(el->exp, st);

                  if (!typeCompare(stmt->val.switchStmt.exp->type, el->exp->type, st)) {
                    char buffer1[1024];
                    char buffer2[1024];
                    getTypeString(buffer1, stmt->val.switchStmt.exp->type);
                    getTypeString(buffer2, el->exp->type);
                    fprintf(stderr,
                            "Error: (line %d) Switch expression and case expression must have matching types, got %s "
                            "and %s instead\n",
                            el->exp->lineno, buffer1, buffer2);
                    exit(1);
                  }
                  el = el->next;
                }
                sl = ccl->clause->val.caseClause.clauses;
                while (sl != NULL) {
                  ReturnStatus temp = typeStmt(sl->stmt, st, returnType);
                  if (temp == Breaks) {
                    clauseReturns = Breaks;
                  } else if (clauseReturns != Breaks) {
                    clauseReturns = temp;
                  }
                  sl = sl->next;
                }
                break;
              case ck_default:
                containsDefault = 1;
                sl = ccl->clause->val.defaultClauses;
                while (sl != NULL) {
                  ReturnStatus temp = typeStmt(sl->stmt, st, returnType);
                  if (temp == Breaks) {
                    clauseReturns = Breaks;
                  } else if (clauseReturns != Breaks) {
                    clauseReturns = temp;
                  }
                  sl = sl->next;
                }
                break;
            }
            if (clauseReturns == Breaks) {
              returns = Breaks;
            } else if (returns != Breaks) {
              returns = clauseReturns == returns ? clauseReturns : NoReturn;
            }
            ccl = ccl->next;
          }
        }
        if (!containsDefault) {
          returns = NoReturn;
        }
        break;
      case sk_for:
        st = stmt->val.forStmt.scope;
        if (stmt->val.forStmt.forClause != NULL) {
          FOR_CLAUSE *fc = stmt->val.forStmt.forClause;
          if (fc->init != NULL) {
            typeStmt(fc->init, st, returnType);
          }
          if (fc->cond != NULL) {
            typeExp(fc->cond, st);
            if (!resolvesToBool(fc->cond->type, st)) {
              char buffer[1024];
              getTypeString(buffer, fc->cond->type);
              fprintf(stderr, "Error: (line %d) Condition evaluates to %s instead of boolean\n", stmt->lineno, buffer);
              exit(1);
            }
          }
          if (fc->post != NULL) {
            typeStmt(fc->post, st, returnType);
          }
        } else if (stmt->val.forStmt.whileExp != NULL) {
          typeExp(stmt->val.forStmt.whileExp, st);
          if (!resolvesToBool(stmt->val.forStmt.whileExp->type, st)) {
            char buffer[1024];
            getTypeString(buffer, stmt->val.forStmt.whileExp->type);
            fprintf(stderr, "Error: (line %d) Condition evaluates to %s instead of boolean\n", stmt->lineno, buffer);
            exit(1);
          }
        }
        returns = typeStmt(stmt->val.forStmt.body, st, returnType);

        // If there is a break we cannot say this for loop returns;
        if (returns == NoReturn &&
            ((stmt->val.forStmt.forClause == NULL && stmt->val.forStmt.whileExp == NULL) ||
             (stmt->val.forStmt.forClause != NULL && stmt->val.forStmt.forClause->cond == NULL))) {
          returns = Returns;
        } else {
          returns = NoReturn;
        }
        break;
      case sk_break:
        return Breaks;
      case sk_continue:
      case sk_fallthrough:
      case sk_empty:
        break;
    }
  }
  return returns;
}

void typeExp(EXP *exp, SymbolTable *st) {
  TYPE *type;
  SYMBOL *s;
  if (exp != NULL) {
    switch (exp->kind) {
      case ek_id:
        if (strcmp(exp->val.id, "_") == 0) {
          exp->type = NULL;
          break;
        }
        s = getSymbol(st, exp->val.id);
        if (s == NULL) {
          fprintf(stderr, "Error: (line %d) use of undeclared identifier \"%s\"\n", exp->lineno, exp->val.id);
          exit(1);
        }
        switch (s->kind) {
          case dk_func:
            // TODO: check if this is enough
            exp->type = s->val.functionDecl.returnType;
            break;
          case dk_short:
            if (s->val.type != NULL) {
              exp->type = s->val.type;
            } else {
              exp->type = NULL;
            }
            break;
          case dk_type:
            fprintf(stderr, "Error: (line %d) improper usage of type\n", exp->lineno);
            exit(1);
            /* if (s->val.typeDecl.type != NULL) {
              exp->type = s->val.typeDecl.type;
            } else {
              exp->type = NULL;
            }
            break; */
          case dk_var:
            exp->type = s->val.type;
            break;
        }
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
        typeExp(exp->val.binary.rhs, st);
        if (typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st)) {
          if (resolvesToNumeric(exp->val.binary.lhs->type, st) || resolvesToString(exp->val.binary.lhs->type, st)) {
            exp->type = exp->val.binary.lhs->type;
          } else {
            char buffer[1024];
            getTypeString(buffer, exp->val.binary.lhs->type);
            fprintf(stderr, "Error: (line %d) Operator + only applies to numeric or string types [received %s]\n",
                    exp->lineno, buffer);
            exit(1);
          }
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr,
                  "Error: (line %d) Both sides of algebraic operation must be the same type [received %s and %s]\n",
                  exp->lineno, buffer1, buffer2);
          exit(1);
        }
        break;
      case ek_minus:
      case ek_times:
      case ek_div:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st)) {
          if (resolvesToNumeric(exp->val.binary.lhs->type, st)) {
            exp->type = exp->val.binary.lhs->type;
          } else {
            char buffer[1024];
            getTypeString(buffer, exp->val.binary.lhs->type);
            fprintf(stderr, "Error: (line %d) Cannot apply algebraic operation to two %ss\n", exp->lineno, buffer);
            exit(1);
          }
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr, "Error: (line %d) Cannot apply algebraic operation to %s and %s\n", exp->lineno, buffer1,
                  buffer2);
          exit(1);
        }
        break;
      case ek_eq:
      case ek_ne:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);

        if (typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st)) {
          if (resolvesToComparable(exp->val.binary.lhs->type, st)) {
            exp->type = baseBool;
          } else {
            char buffer[1024];
            getTypeString(buffer, exp->val.binary.lhs->type);
            fprintf(stderr, "Error: (line %d) Cannot compare two %ss\n", exp->lineno, buffer);
            exit(1);
          }
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr, "Error: (line %d) Cannot compare %s with %s\n", exp->lineno, buffer1, buffer2);
          exit(1);
        }
        break;
      case ek_ge:
      case ek_le:
      case ek_gt:
      case ek_lt:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st)) {
          if (resolvesToOrdered(exp->val.binary.lhs->type, st)) {
            exp->type = baseBool;
          } else {
            char buffer[1024];
            getTypeString(buffer, exp->val.binary.lhs->type);
            fprintf(stderr, "Error: (line %d) Cannot compare two %ss\n", exp->lineno, buffer);
            exit(1);
          }
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr, "Error: (line %d) Cannot compare %s with %s\n", exp->lineno, buffer1, buffer2);
          exit(1);
        }
        break;
      case ek_and:
      case ek_or:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (resolvesToBool(exp->val.binary.lhs->type, st) &&
            typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st)) {
          // For logical && and || operations the type evaluates to left hand side
          exp->type = exp->val.binary.lhs->type;
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr, "Error: (line %d) Cannot compare %s with %s\n", exp->lineno, buffer1, buffer2);
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
        if (typeCompare(exp->val.binary.lhs->type, exp->val.binary.rhs->type, st) &&
            resolvesToInteger(exp->val.binary.lhs->type, st)) {
          exp->type = exp->val.binary.lhs->type;
        } else {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.binary.lhs->type);
          getTypeString(buffer2, exp->val.binary.rhs->type);
          fprintf(stderr, "Error: (line %d) Cannot compare %s with %s\n", exp->lineno, buffer1, buffer2);
          exit(1);
        }
        break;
      case ek_uplus:
      case ek_uminus:
        typeExp(exp->val.unary.exp, st);
        if (resolvesToNumeric(exp->val.unary.exp->type, st)) {
          exp->type = exp->val.unary.exp->type;
        } else {
          char buffer[1024];
          getTypeString(buffer, exp->val.unary.exp->type);
          fprintf(stderr, "Error: (line %d) Unary plus/minus only accepts numeric types, received %s\n", exp->lineno,
                  buffer);
          exit(1);
        }
        break;
      case ek_bang:
        typeExp(exp->val.unary.exp, st);
        if (resolvesToBool(exp->val.unary.exp->type, st)) {
          exp->type = baseBool;
        } else {
          char buffer[1024];
          getTypeString(buffer, exp->val.unary.exp->type);
          fprintf(stderr, "Error: (line %d) Negation operator only accepts booleans, received %s\n", exp->lineno,
                  buffer);
          exit(1);
        }
        break;
      case ek_ubitXor:
        typeExp(exp->val.unary.exp, st);
        if (resolvesToInteger(exp->val.unary.exp->type, st)) {
          exp->type = exp->val.unary.exp->type;
        } else {
          char buffer[1024];
          getTypeString(buffer, exp->val.unary.exp->type);
          fprintf(stderr, "Error: (line %d) Bitwise negation operator only accepts integer types, received %s\n",
                  exp->lineno, buffer);
          exit(1);
        }
        break;
      case ek_func:
        s = getSymbol(st, exp->val.funcCall.funcId);

        // If Type decl
        if (s->kind == dk_type) {
          // This will only be a type decl if it's a compound type
          EXP_LIST *el = exp->val.funcCall.args;
          if (el->next != NULL) {
            fprintf(stderr, "Error: (line %d) type cast only expected 1 argument\n", exp->lineno);
            exit(1);
          }
          typeExp(el->exp, st);

          // Type casts only work on types which resolve to base types
          if (!resolvesToBase(s->val.typeDecl.type, st)) {
            char buffer[1024];
            getTypeString(buffer, s->val.typeDecl.type);
            fprintf(stderr, "Error: (line %d) type %s must resolve to a base\n", exp->lineno, buffer);
            exit(1);
          }

          // The three conditions for typecasts are that they must
          // 1. Resolve to the same type
          // 2. Both resolve to a numeric
          // 3. If it is being cast to a string it must resolve to an integer and
          if (!resolvesToSame(el->exp->type, s->val.typeDecl.type, st) &&
              !(resolvesToNumeric(el->exp->type, st) && resolvesToNumeric(s->val.typeDecl.type, st)) &&
              !(resolvesToString(s->val.typeDecl.type, st) && resolvesToInteger(el->exp->type, st))) {
            char buffer1[1024];
            char buffer2[1024];
            getTypeString(buffer1, el->exp->type);
            getTypeString(buffer2, s->val.typeDecl.type);
            fprintf(stderr, "Error: (line %d) conversion between incompatible types [%s <- %s]\n", exp->lineno, buffer2,
                    buffer1);
            exit(1);
          }
          exp->type = s->val.typeDecl.type;

        } else if (s->kind == dk_func) {  // Function call (not type cast)
          EXP_LIST *passedParams = exp->val.funcCall.args;
          PARAM_LIST *pl = s->val.functionDecl.paramList;

          int expectedArgumentCount = 0;
          {
            PARAM_LIST *temp = pl;
            while (temp != NULL) {
              expectedArgumentCount++;
              temp = temp->next;
            }
          }

          if (strcmp(s->name, "init") == 0) {
            fprintf(stderr, "Error: (line %d) init may not be called\n", exp->lineno);
            exit(1);
          }

          EXP_LIST *curr = passedParams;
          int received = 0;
          while (curr != NULL) {
            received++;

            if (pl == NULL) {
              fprintf(stderr,
                      "Error: (line %d) function %s called with incorrect number of parameters [expected %d, received "
                      "%d]\n",
                      exp->lineno, s->name, expectedArgumentCount, received);
              exit(1);
            }

            typeExp(curr->exp, st);

            if (!typeCompare(curr->exp->type, pl->type, st)) {
              char buffer1[1024];
              char buffer2[1024];
              getTypeString(buffer1, curr->exp->type);
              getTypeString(buffer2, pl->type);
              fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in function call \n",
                      exp->lineno, buffer1, buffer2);
              exit(1);
            }

            pl = pl->next;
            curr = curr->next;
          }

          // If there are leftover arguments that weren't called
          if (pl != NULL) {
            fprintf(
                stderr,
                "Error: (line %d) function %s called with incorrect number of parameters [expected %d, received %d]\n",
                exp->lineno, s->name, expectedArgumentCount, received);
            exit(1);
          }
          exp->type = s->val.functionDecl.returnType;
        } else {
          fprintf(stderr, "Error: (line %d) %s is not a function type\n", exp->lineno, s->name);
          exit(1);
        }
        break;
      case ek_append:
        typeExp(exp->val.append.sliceExp, st);
        typeExp(exp->val.append.elem, st);
        if (!resolvesToTSlice(exp->val.append.sliceExp->type, exp->val.append.elem->type, st)) {
          char buffer1[1024];
          char buffer2[1024];
          getTypeString(buffer1, exp->val.append.sliceExp->type);
          getTypeString(buffer2, exp->val.append.elem->type);
          fprintf(stderr,
                  "Error: (line %d) slice expression should resolve to a slice of the element type, received %s slice "
                  "and %s element\n",
                  exp->lineno, buffer1, buffer2);
          exit(1);
        }
        exp->type = exp->val.append.sliceExp->type;
        break;
      case ek_len:
        typeExp(exp->val.lenExp, st);
        if (typeList(exp->val.lenExp->type) || typeString(exp->val.lenExp->type)) {
          exp->type = baseInt;
        } else {
          char buffer[1024];
          getTypeString(buffer, exp->val.lenExp->type);
          fprintf(stderr,
                  "Error: (line %d) Length builtin expects slice, array, or string types as argument [received %s]\n",
                  exp->lineno, buffer);
          exit(1);
        }
        break;
      case ek_cap:
        typeExp(exp->val.capExp, st);
        if (typeList(exp->val.capExp->type)) {
          exp->type = baseInt;
        } else {
          char buffer[1024];
          getTypeString(buffer, exp->val.capExp->type);
          fprintf(stderr, "Error: (line %d) Capacity builtin expects slice or array types [received %s]\n", exp->lineno,
                  buffer);
          exit(1);
        }
        break;
      case ek_indexExp:
        typeExp(exp->val.indexExp.indexExp, st);
        if (!resolvesToSame(exp->val.indexExp.indexExp->type, baseInt, st)) {
          char buffer[1024];
          getTypeString(buffer, exp->val.indexExp.indexExp->type);
          fprintf(stderr, "Error: (line %d) index must be well typed and resolve to type int [received %s]\n",
                  exp->lineno, buffer);
          exit(1);
        }
        typeExp(exp->val.indexExp.objectExp, st);
        EXP *e = exp->val.indexExp.objectExp;
        TYPE *resolvesTo = typeOfList(e->type, st);
        if (resolvesTo == NULL) {
          char buffer[1024];
          getTypeString(buffer, resolvesTo);
          fprintf(stderr, "Error: (line %d) expression must resolve to an array or slice [received %s]\n", e->lineno,
                  buffer);
          exit(1);
        } else {
          exp->type = resolvesTo;
        }
        break;
      case ek_structField:
        typeExp(exp->val.structField.structExp, st);
        TYPE *t = exp->val.structField.structExp->type;
        t = fixType(st, t);
        if (t == NULL || !resolvesToStruct(t, st)) {
          char buffer[1024];
          getTypeString(buffer, t);
          fprintf(stderr, "Error: (line %d) %s is not a struct type\n", exp->lineno, buffer);
          exit(1);
        }

        if(t->kind == tk_ref){
          SYMBOL *s = getSymbol(st, t->val.name);
          t = s->val.typeDecl.resolvesTo;
        }
        FIELD_DECLS *d = t->val.structFields;

        while (d != NULL) {
          if (strcmp(d->id, exp->val.structField.fieldName) == 0) {
            exp->type = d->type;
            return;
          }
          d = d->next;
        }
        // If the code left this while loop then  the struct does not  contain the field accessed
        char buffer[1024];
        getTypeString(buffer, t);
        fprintf(stderr, "Error: (line %d) %s does not contain field %s\n", exp->lineno, buffer,
                exp->val.structField.fieldName);
        exit(1);
        break;
      case ek_paren:
        typeExp(exp->val.parenExp, st);
        exp->type = exp->val.parenExp->type;
        break;
      case ek_conv:
        printf("conv not implemented\n");
        break;
    }
  }
}

int resolvesToInteger(TYPE *type, SymbolTable *st) {
  return resolvesToSame(type, baseInt, st) || resolvesToSame(type, baseRune, st);
}

int resolvesToString(TYPE *type, SymbolTable *st) { return resolvesToSame(type, baseString, st); }

int resolvesToBool(TYPE *type, SymbolTable *st) { return resolvesToSame(type, baseBool, st); }

int resolvesToNumeric(TYPE *type, SymbolTable *st) {
  return resolvesToSame(type, baseInt, st) || resolvesToSame(type, baseRune, st) || resolvesToSame(type, baseFloat, st);
}

int resolvesToOrdered(TYPE *type, SymbolTable *st) {
  return resolvesToSame(type, baseInt, st) || resolvesToSame(type, baseRune, st) ||
         resolvesToSame(type, baseFloat, st) || resolvesToSame(type, baseString, st);
}

int resolvesToBase(TYPE *type, SymbolTable *st) {
  return resolvesToBool(type, st) || resolvesToNumeric(type, st) || resolvesToString(type, st);
}

int resolvesToStruct(TYPE *type, SymbolTable *st) {
  if (type->kind == tk_struct) return 1;
  if (type->kind != tk_ref) return 0;
  SYMBOL *s = getSymbol(st, type->val.name);
  if (s->kind != dk_type) {
    fprintf(stderr, "Error: (line %d) declared value %s is not a type\n", type->lineno, type->val.name);
    exit(1);
  }
  return s->val.typeDecl.resolvesTo->kind == tk_struct;
}

int resolvesToComparable(TYPE *type, SymbolTable *st) {
  SYMBOL *s;
  FIELD_DECLS *fd;
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
      case tk_float:
      case tk_rune:
      case tk_boolean:
      case tk_string:
        return 1;
      case tk_struct:
        fd = type->val.structFields;
        while (fd != NULL) {
          if (!resolvesToComparable(fd->type, st)) {
            return 0;
          }
          fd = fd->next;
        }
        return 1;
        break;
      case tk_array:
        return resolvesToComparable(type->val.array.elemType, st);
      case tk_slice:
        return 0;
      case tk_ref:
        s = getSymbol(st, type->val.name);
        return resolvesToComparable(s->val.typeDecl.resolvesTo, st);
        break;
      case tk_res:  // shouldn't happen
        return 0;
    }
  }
  return 0;
}

int typeInteger(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
      case tk_int:
      case tk_rune:
        return 1;
      default:
        return 0;
    }
  }
  return 0;
}

int typeBool(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
      case tk_boolean:
        return 1;
      default:
        return 0;
    }
  }
  return 0;
}

int typeString(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
      case tk_string:
        return 1;
      default:
        return 0;
    }
  }
  return 0;
}

int typeNumeric(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
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

int typeComparable(TYPE *type) {
  if (type != NULL) {
    FIELD_DECLS *fd;
    switch (type->kind) {
      case tk_int:
      case tk_float:
      case tk_rune:
      case tk_string:
      case tk_boolean:
        return 1;
      case tk_struct:
        fd = type->val.structFields;
        while (fd != NULL) {
          if (!typeComparable(fd->type)) {
            return 0;
          }
          fd = fd->next;
        }
        return 1;
      case tk_array:
        return typeComparable(type->val.array.elemType);
      case tk_slice:
      case tk_ref:
      case tk_res:
        return 0;
    }
  }
  return 0;
}

int typeCompare(TYPE *type1, TYPE *type2, SymbolTable *st) {
  if (type1 == NULL || type2 == NULL || type1->kind != type2->kind) return 0;
  FIELD_DECLS *fd1;
  FIELD_DECLS *fd2;
  if (type1 == type2) return 1;
  switch (type1->kind) {
    case tk_res:
      fprintf(stderr, "Error: (line %d) shouldn't be reached\n", type1->lineno);
      exit(1);
    case tk_int:
    case tk_float:
    case tk_rune:
    case tk_string:
    case tk_boolean:
    case tk_ref:
      return type1 == type2;
    case tk_slice:
      return typeCompare(type1->val.sliceType, type2->val.sliceType, st);
    case tk_array:
      if (type1->val.array.size != type2->val.array.size) {
        fprintf(stderr, "Error: (line %d) size of arrays is different\n", type1->lineno);
        exit(1);
      }
      return typeCompare(type1->val.array.elemType, type2->val.array.elemType, st);
    case tk_struct:
      fd1 = type1->val.structFields;
      fd2 = type2->val.structFields;

      while (fd1 != NULL && fd2 != NULL) {
        if (strcmp(fd1->id, fd2->id) != 0) return 0;
        if (!typeCompare(fd1->type, fd2->type, st)) {
          return 0;
        }
        fd1 = fd1->next;
        fd2 = fd2->next;
      }

      if (fd1 != NULL || fd2 != NULL) return 0;
      return 1;
  }
}

int typeOrdered(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
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

int typeList(TYPE *type) {
  if (type != NULL) {
    switch (type->kind) {
      case tk_slice:
      case tk_array:
        return 1;
      default:
        return 0;
    }
  }
  return 0;
}

int resolvesToList(TYPE *type, SymbolTable *st) {
  if (type != NULL) {
    SYMBOL *s;
    switch (type->kind) {
      case tk_slice:
      case tk_array:
        return 1;
      case tk_ref:
        s = getSymbol(st, type->val.name);
        if (s->val.typeDecl.resolvesToKind == tk_slice || s->val.typeDecl.resolvesToKind == tk_array) {
          return 0;
        } else if (s->val.typeDecl.resolvesToKind == tk_ref) {
          return resolvesToList(s->val.typeDecl.type, st);
        }
      default:
        return 0;
    }
  }
  return 0;
}

TYPE *typeOfList(TYPE *type, SymbolTable *st) {
  SYMBOL *s;
  if (type == NULL) return NULL;
  switch (type->kind) {
    case tk_slice:
      return type->val.sliceType;
    case tk_array:
      return type->val.array.elemType;
    case tk_ref:
      s = getSymbol(st, type->val.name);
      if (s->val.typeDecl.resolvesToKind == tk_slice || s->val.typeDecl.resolvesToKind == tk_array) {
        return 0;
      } else if (s->val.typeDecl.resolvesToKind == tk_ref) {
        return typeOfList(s->val.typeDecl.type, st);
      }
    case tk_res:
      return fixType(st, type);
    default:
      return NULL;
  }
  return NULL;
}

/* Returns wheter s resolves to a slice of type t */
int resolvesToTSlice(TYPE *s, TYPE *t, SymbolTable *st) {
  if (s == NULL || t == NULL) return 0;
  SYMBOL *sym;
  switch (s->kind) {
    case tk_slice:
      sym = getSymbol(st, s->val.sliceType->val.name);
      return sym->val.typeDecl.type == t;
    case tk_ref:
      sym = getSymbol(st, s->val.name);
      if (sym != NULL && sym->kind != dk_type) {
        fprintf(stderr, "Error: (line %d) does not resolve to a type declaration", s->lineno);
        exit(1);
      }
      if (s == sym->val.typeDecl.resolvesTo) {
        return 0;
      }
      return resolvesToTSlice(sym->val.typeDecl.resolvesTo, t, st);
    default:
      return 0;
  }
  return 0;
}

int isLValue(EXP *exp, SymbolTable *st) {
  SYMBOL *s;
  switch (exp->kind) {
    case ek_id:
      return 1;
    case ek_indexExp:
      return isLValue(exp->val.indexExp.objectExp, st);
    case ek_structField:
      return isLValue(exp->val.structField.structExp, st);
    case ek_paren:
      return isLValue(exp->val.parenExp, st);
    case ek_func:
      s = getSymbol(st, exp->val.funcCall.funcId);
      TYPE *returnType = s->val.functionDecl.returnType;
      switch(returnType->kind){
        case tk_slice:
          return 1;
        case tk_struct:
        default:
          return 0;
      }
    default:
      return 0;
  }
}

TYPE *typeResolve(TYPE *type, SymbolTable *st) {
  if (type == NULL) return NULL;
  SYMBOL *s;
  switch (type->kind) {
    case tk_res:
      fprintf(stderr, "Error: (line %d) shouldn't be reached\n", type->lineno);
      exit(1);
    case tk_int:
    case tk_float:
    case tk_rune:
    case tk_string:
    case tk_boolean:
      return type;
    case tk_ref:
      s = getSymbol(st, type->val.name);
      return typeResolve(s->val.typeDecl.resolvesTo, st);
    default:
      return NULL;
  }
}

int resolvesToSame(TYPE *type1, TYPE *type2, SymbolTable *st) {
  type1 = typeResolve(type1, st);
  type2 = typeResolve(type2, st);
  return type1 == type2;
}

char *getAssignOpString(AssignOpKind kind) {
  switch (kind) {
    case aok_plus:
      return "+=";
    case aok_minus:
      return "-=";
    case aok_times:
      return "*=";
    case aok_div:
      return "/=";
    case aok_mod:
      return "\%=";
    case aok_bitAnd:
      return "&=";
    case aok_bitOr:
      return "|=";
    case aok_bitXor:
      return "^=";
    case aok_bitClear:
      return "&^=";
    case aok_bitLeftShift:
      return "<<=";
    case aok_bitRightShift:
      return ">>=";
  }
}