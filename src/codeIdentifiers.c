#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codeIdentifiers.h"
#include "symbol.h"

IdentifierTable *initIdentifierTable() {
  IdentifierTable *newIdentifierTable = (IdentifierTable *)malloc(sizeof(IdentifierTable));
  for (int i = 0; i < HashSize; i++) {
    newIdentifierTable->table[i] = NULL;
  }
  return newIdentifierTable;
}

IdentifierTable *scopeIdentifierTable(IdentifierTable *identifierTable) {
  IdentifierTable *t = initIdentifierTable();
  t->parent = identifierTable;
  return t;
}

IDENTIFIER *addToIdentifierTable(char *identifier, int scopeCount, IdentifierTable *it) {
  if (strcmp(identifier, "_") == 0) {
    return NULL;
  }
  int i = Hash(identifier);
  for (IDENTIFIER *s = it->table[i]; s; s = s->next) {
    if (strcmp(s->identifier, identifier) == 0) {
      // Potentially temporary fix
      return s;

      fprintf(stderr, "Error adding to identifier table! Logical error! Shouldn't happen!\n");
      exit(1);
    }
  }

  IDENTIFIER *s = (IDENTIFIER *)malloc(sizeof(IDENTIFIER));
  s->identifier = strdup(identifier);
  s->scopeCount = scopeCount;
  s->next = it->table[i];
  it->table[i] = s;
  return s;
}

IDENTIFIER *getFromIdentifierTable(char *id, IdentifierTable *it) {
  int i = Hash(id);
  // Check the current scope
  for (IDENTIFIER *s = it->table[i]; s; s = s->next) {
    if (strcmp(s->identifier, id) == 0) return s;
  }
  // Check for existence of a parent scope
  if (it->parent == NULL) return NULL;
  // Check the parent scopes
  return getFromIdentifierTable(id, it->parent);
}

IDENTIFIER *addIfNotInTable(char *id, IdentifierTable *it) {
  IDENTIFIER *i = getFromIdentifierTable(id, it);
  if (i != NULL) {
    int scopeCount = i->scopeCount + 1;
    i = addToIdentifierTable(id, scopeCount, it);
  } else {
    i = addToIdentifierTable(id, 1, it);
  }
  return i;
}