#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "type.h"

SymbolTable *st;

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

// TODO: Implement
void typeVarDecl(VAR_SPECS *vs, SymbolTable *st) {
  if (vs != NULL) {
    // If rhs is not null
    if (vs->exp != NULL) {
      typeExp(vs->exp, st);
      if(strcmp(vs->id, "_") != 0){
        //SYMBOL *s = getSymbol(st, vs->type->val.name);
        //TYPE *t = vs->type->kind == tk_ref ? getSymbol(st, vs->type->val.name)->val.type : vs->type;
        if(!typeCompare(vs->exp->type, vs->type, st)){
          fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n", vs->exp->lineno, vs->type->val.name, vs->exp->type->val.name);
          exit(1);
        }
      }
    }
    typeVarDecl(vs->next, st);
  }
}

// TODO: Implement
void typeShortDecl(SHORT_SPECS *ss, SymbolTable *st) {
  if (ss != NULL) {
    typeExp(ss->rhs, st);
    if(strcmp(ss->lhs->val.id, "_") != 0){
      SYMBOL *s = getSymbol(st, ss->lhs->val.id);
      if (s->val.type != NULL && s->val.type != ss->rhs->type) {
        fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n", ss->rhs->lineno,
                s->val.type->val.name, ss->rhs->type->val.name);
        exit(1);
      }
    }
    typeShortDecl(ss->next, st);
  }
}

// TODO: Implement
void typeTypeDecl(TYPE_SPECS *ts, SymbolTable *st) {
  if (ts != NULL) {
    typeTypeDecl(ts->next, st);
  }
}

// TODO: Implement
void typeFuncDecl(FUNC_DECL *fd, SymbolTable *st) {
  if (fd != NULL) {
    // TODO: Typecheck return statements
    typeStmt(fd->body, st);
  }
}

// TODO: Implement
void typeStmt(STMT *stmt, SymbolTable *st) {
  STMT_LIST *sl;
  ASSIGN *al;
  CASE_CLAUSE_LIST *ccl;
  if (stmt != NULL) {
    switch (stmt->kind) {
      case sk_block:
        // Change scope to the block
        st = stmt->val.block.scope != NULL ? stmt->val.block.scope : st;
        sl = stmt->val.block.blockStatements;
        while (sl != NULL) {
          typeStmt(sl->stmt, st);
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
          if(strcmp(al->lhs->val.id,"_") == 0) break;
          typeExp(al->lhs, st);
          // TODO : compare types when they are compound @Kabilan
          if (!typeCompare(al->lhs->type, al->rhs->type, st)){
            fprintf(stderr, "Error: (line %d) %s is not assignment compatible with %s in assign statement\n",
                    stmt->lineno, al->lhs->type->val.name, al->rhs->type->val.name);
            exit(1);
          }
          al = al->next;
        }
        break;
      case sk_assignOp:
        typeExp(stmt->val.assignOp.lhs, st);
        typeExp(stmt->val.assignOp.rhs, st);
        if (stmt->val.assignOp.lhs->type != stmt->val.assignOp.rhs->type){
          fprintf(stderr, "Error: (line %d) Both sides of assignment mus be the same\n", stmt->lineno);
          exit(1);
        }
        switch (stmt->val.assignOp.kind) {
          case aok_plus:
            if (!typeNumeric(stmt->val.assignOp.lhs->type) && !typeString(stmt->val.assignOp.lhs->type)) {
              fprintf(stderr, "Error: (line %d) Plus assign operator only works with numeric or string types\n", stmt->lineno);
              exit(1);
            }
            break;
          case aok_minus:
          case aok_times:
          case aok_div:
            if (!typeNumeric(stmt->val.assignOp.lhs->type)) {
              fprintf(stderr, "Error: (line %d) Operation only works with numeric types\n", stmt->lineno);
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
            if (!typeInteger(stmt->val.assignOp.lhs->type)) {
              fprintf(stderr, "Error: (line %d) Operation only works with integer types\n", stmt->lineno);
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
        break;
      case sk_print:
        break;
      case sk_println:
        break;
      case sk_return:
        break;
      case sk_if:
        st = stmt->val.ifStmt.scope;
        if (stmt->val.ifStmt.simpleStmt != NULL) {
          typeStmt(stmt->val.ifStmt.simpleStmt, st);
        }
        typeExp(stmt->val.ifStmt.cond, st);
        if (!typeBool(stmt->val.ifStmt.cond->type)) {
          fprintf(stderr, "Error: (line %d) if statement condition must evaluate to a boolean", stmt->lineno);
          exit(1);
        }
        typeStmt(stmt->val.ifStmt.body, st);
        if (stmt->val.ifStmt.elseStmt != NULL) {
          typeStmt(stmt->val.ifStmt.elseStmt, st);
        }
        break;
      case sk_else:
        typeStmt(stmt->val.elseBody, st);
        break;
      case sk_switch:
        st = stmt->val.switchStmt.scope;
        if (stmt->val.switchStmt.simpleStmt != NULL) {
          typeStmt(stmt->val.switchStmt.simpleStmt, st);
        }
        typeExp(stmt->val.switchStmt.exp, st);
        ccl = stmt->val.switchStmt.caseClauses;
        while (ccl != NULL) {
          if (ccl->clause->kind == ck_case) {
            // TODO: Kabilan do the rest
          } else {
          }
          ccl = ccl->next;
        }
        break;
      case sk_for:
        st = stmt->val.forStmt.scope;
        if (stmt->val.forStmt.forClause != NULL) {
          FOR_CLAUSE *fc = stmt->val.forStmt.forClause;
          if (fc->init != NULL) {
            typeStmt(fc->init, st);
          }
          if (fc->cond != NULL) {
            typeExp(fc->cond, st);
          }
          if (fc->post != NULL) {
            typeStmt(fc->post, st);
          }
        } else {
          typeExp(stmt->val.forStmt.whileExp, st);
        }
        typeStmt(stmt->val.forStmt.body, st);
        break;
      case sk_break:
      case sk_continue:
      case sk_fallthrough:
      case sk_empty:
        break;
    }
  }
}

// TODO: Implement
void typeExp(EXP *exp, SymbolTable *st) {
  TYPE *type;
  SYMBOL *s;
  if (exp != NULL) {
    switch (exp->kind) {
      case ek_id:
        if(strcmp(exp->val.id, "_") == 0){
          exp->type = NULL;
          break;
        }

        s = getSymbol(st, exp->val.id);
        if (getSymbol(st, exp->val.id) == NULL) {
          fprintf(stderr, "Error: (line %d) use of undeclared identifier \"%s\"", exp->lineno, exp->val.id);
          exit(1);
        }
        switch (s->kind) {
          case dk_func:
            // TODO: check if this is enough
            exp->type = s->val.functionDecl.returnType;
            break;
          case dk_short:
            exp->type = NULL;
            break;
          case dk_type:
            exp->type = s->val.typeDecl.type;
            break;
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
        if (exp->val.binary.lhs->type == exp->val.binary.rhs->type) {
          if (typeNumeric(exp->val.binary.lhs->type) || typeString(exp->val.binary.lhs->type)) {
            exp->type = exp->val.binary.lhs->type;
          } else {
            fprintf(stderr, "Error: (line %d) Operation only works with numeric or string types", exp->lineno);
            exit(1);
          }
        } else {
          fprintf(stderr, "Error: (line %d) Both sides of algebraic operation must be numeric", exp->lineno);
          exit(1);
        }
        break;
      case ek_minus:
      case ek_times:
      case ek_div:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (exp->val.binary.rhs->type == exp->val.binary.lhs->type) {
          if (typeNumeric(exp->val.binary.lhs->type)) {
            exp->type = exp->val.binary.lhs->type;
          } else {
            fprintf(stderr, "Error: (line %d) Operation only works with numeric types", exp->lineno);
            exit(1);
          }
        } else {
          fprintf(stderr, "Error: (line %d) Both sides of algebraic operation must be numeric", exp->lineno);
          exit(1);
        }
        break;
      case ek_eq:
      case ek_ne:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (exp->val.binary.lhs->type == exp->val.binary.rhs->type) {
          if (typeComparable(exp->val.binary.lhs->type)) {
            exp->type = baseBool;
          } else {
            fprintf(stderr, "Error: (line %d) Types not comparable", exp->lineno);
            exit(1);
          }
        } else {
          fprintf(stderr, "Error: (line %d) Both sides of equality test must be of same type", exp->lineno);
          exit(1);
        }
        break;
      case ek_ge:
      case ek_le:
      case ek_gt:
      case ek_lt:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (exp->val.binary.lhs->type == exp->val.binary.rhs->type) {
          if (typeOrdered(exp->val.binary.lhs->type)) {
            exp->type = baseBool;
          } else {
            fprintf(stderr, "Error: (line %d) Types not ordered", exp->lineno);
            exit(1);
          }
        } else {
          fprintf(stderr, "Error: (line %d) Both sides of comparison must be of same type", exp->lineno);
          exit(1);
        }
        break;
      case ek_and:
      case ek_or:
        typeExp(exp->val.binary.lhs, st);
        typeExp(exp->val.binary.rhs, st);
        if (typeBool(exp->val.binary.lhs->type) && typeBool(exp->val.binary.rhs->type)) {
          exp->type = baseBool;
        } else {
          fprintf(stderr, "Error: (line %d) Logical operators and/or only accept two bools", exp->lineno);
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
        if (typeInteger(exp->val.binary.lhs->type) && typeInteger(exp->val.binary.rhs->type)) {
          exp->type = exp->val.binary.lhs->type;
        } else {
          fprintf(stderr, "Error: (line %d) Logical operators and/or only accept two bools\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_uplus:
      case ek_uminus:
        typeExp(exp->val.unary.exp, st);
        if (typeNumeric(exp->val.unary.exp->type)) {
          exp->type = exp->val.unary.exp->type;
        } else {
          fprintf(stderr, "Error: (line %d) Unary plus/minus only accepts numeric types\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_bang:
        typeExp(exp->val.unary.exp, st);
        if (typeBool(exp->val.unary.exp->type)) {
          exp->type = baseBool;
        } else {
          fprintf(stderr, "Error: (line %d) Negation operator only accepts booleans\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_ubitXor:
        typeExp(exp->val.unary.exp, st);
        if (typeInteger(exp->val.unary.exp->type)) {
          exp->type = exp->val.unary.exp->type;
        } else {
          fprintf(stderr, "Error: (line %d) Bitwise negation operator only accepts integer types\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_func:
        s = getSymbol(st, exp->val.funcCall.funcId);
        // If Type decl
        if (s->kind == dk_type) {
          // TODO: resolve types
          exp->type = s->val.typeDecl.type;
        } else if (s->kind == dk_func) {
          exp->type = s->val.functionDecl.returnType;
        }
        break;
      case ek_append:
        typeExp(exp->val.append.sliceExp, st);
        typeExp(exp->val.append.elem, st);
        if(!resolvesToTSlice(exp->val.append.sliceExp->type, exp->val.append.elem->type, st)){
          fprintf(stderr, "Error: (line %d) slice expression should resolve to a slice of the elemnt type\n", exp->lineno);
          exit(1);
        }
        exp->type = exp->val.append.sliceExp->type;
        break;
      case ek_len:
        typeExp(exp->val.lenExp, st);
        if (typeList(exp->val.lenExp->type) || typeString(exp->val.lenExp->type)) {
          exp->type = baseInt;
        } else {
          fprintf(stderr, "Error: (line %d) Length only applies to slice, array or string types\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_cap:
        typeExp(exp->val.capExp, st);
        if (typeList(exp->val.capExp->type)) {
          exp->type = baseInt;
        } else {
          fprintf(stderr, "Error: (line %d) Capacity only applies to slice or array types\n", exp->lineno);
          exit(1);
        }
        break;
      case ek_indexExp:
        break;
      case ek_structField:
        typeExp(exp->val.structField.structExp, st);
        
        break;
      case ek_paren:
        typeExp(exp->val.parenExp, st);
        exp->type = exp->val.parenExp->type;
        break;
      case ek_conv:
        break;
    }
  }
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
        return 0;
    }
  }
  return 0;
}

int typeCompare(TYPE *type1, TYPE *type2, SymbolTable *st){
  SYMBOL *s1;
  SYMBOL *s2;
  if(type1->kind != type2->kind) return 0;
  switch (type1->kind){
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
      if(type1->val.array.size != type2->val.array.size){
        fprintf(stderr, "Error: (line %d) size of arrays is different", type1->lineno);
        exit(1);
      }
      return typeCompare(type1->val.array.elemType, type2->val.array.elemType, st);
    case tk_struct:
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

int typeIsBase(TYPE *type) {
  return type == baseBool || type == baseFloat || type == baseInt || type == baseRune || type == baseString;
}

/* Returns wheter s resolves */
int resolvesToTSlice(TYPE *s, TYPE *t, SymbolTable *st){
  SYMBOL *sym;
  while(1){
    switch(s->kind){
      case tk_slice:
        sym = getSymbol(st, s->val.sliceType->val.name);
        return sym->val.typeDecl.type == t;
      case tk_ref:
        sym = getSymbol(st, s->val.name);
        if(sym->kind != dk_type){
          fprintf(stderr, "Error: (line %d) does not resolve to a type declaration", s->lineno);
          exit(1);
        }
        resolvesToTSlice(sym->val.typeDecl.type, t, st);
      default:
        return 0;
    }
  }
  return 0;
}

int resolvesToBase(TYPE *type, SymbolTable *st) {
  while (1) {
    break;
  }
  return 0;
}
