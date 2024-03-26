#include <stdbool.h>
#include <stdlib.h>

#include "sym.h"
#include "stack.h"
#include "ht.h"

static inline bool SCOPEMARKER(Binding *b) {
	return (b->ident == NULL);
}

Binding *mkbind(const char *ident, Var *var) {
	Binding *b = calloc(1, sizeof(Binding));
	b->ident = ident;
	b->var = var;

	if (b->ident == NULL || b->var == NULL)
		return NULL;
	return b;
} 

Binding *mkscope(void) {
	return (Binding *)calloc(1, sizeof(Binding)); // using ident = NULL to tell if stack item is a scope marker
}

SymbolTable *symboltable_new(void) {
	SymbolTable *st = calloc(1, sizeof(SymbolTable));
	st->stack = stack_new();
	st->ht = ht_new();

	return st;
}

void symboltable_free(SymbolTable *st) {
	stack_free(st->stack);
	ht_free(st->ht);
	free(st);
}

void symboltable_bind(SymbolTable *st, Binding *binding) {
	stack_push(st->stack, binding);
	ht_bind(st->ht, binding->ident, binding->var);
}

Var *symboltable_lookup(SymbolTable *st, const char *ident) {
	Var *var = ht_lookup(st->ht, ident);
	return var;
}

void scope_enter(SymbolTable *st) {
	stack_push(st->stack, mkscope());
}

void scope_exit(SymbolTable *st) {
	for (Binding *b = stack_pop(st->stack); !SCOPEMARKER(b); b = stack_pop(st->stack)) {
		// pop binding
	}
}
