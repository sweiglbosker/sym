#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sym.h"
#include "stack.h"
#include "ht.h"

static inline bool SCOPEMARKER(Binding *b) {
	return (b->ident == NULL);
}

Binding *mkbind(const char *ident, Var *var) {
	Binding *b = calloc(1, sizeof(Binding));
	b->ident = calloc(strlen(ident + 1), sizeof(char));
	b->var = var;

	strcpy(b->ident, ident);

	if (b->ident == NULL || b->var == NULL)
		return NULL;
	return b;
} 

void freebind(Binding *b) {
	if (b->ident == NULL) { // this is a scope marker
		free(b);
		return;
	}

	free(b->ident);
	freevar(b->var);
	free(b);
}

void freevar(Var *v) {
	if (v->type == TYPE_STRING) 
		free(v->val);
	free(v);
}

Binding *mkscope(void) {
	return (Binding *)calloc(1, sizeof(Binding)); // using ident = NULL to tell if stack item is a scope marker
}

SymbolTable *symboltable_new(void) {
	SymbolTable *st = calloc(1, sizeof(SymbolTable));
	st->stack = stack_new();
	st->ht = ht_new();

	stack_push(st->stack, mkscope());

	return st;
}

void symboltable_free(SymbolTable *st) {
	stack_free(st->stack);
	ht_free(st->ht);
	free(st);
}

void symboltable_bind(SymbolTable *st, Binding *binding) {
	BindingList *bl = ht_lookup(st->ht, binding->ident);

	if (!bl) {
		ht_bind(st->ht, binding->ident, binding->var);
	}

	stack_push(st->stack, binding);
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
		if (stack_empty(st->stack)) {
			fprintf(stderr, "error: trying to exist scope that does not exist");
			return;
		}
		ht_unbind(st->ht, b->ident);
		freebind(b);
		stack_pop(st->stack);
	}
}
