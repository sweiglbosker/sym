#ifndef _SYM_H
#define _SYM_H

#include "stack.h"
#include "ht.h"

typedef enum { TYPE_INVALID, TYPE_INT, TYPE_CHAR, TYPE_STRING } Type;

typedef struct {
	Ht *ht;
	Stack *stack;
} SymbolTable;

typedef struct {
	int type;
	void *val;
} Var;

typedef struct {
	char *ident; 
	Var *var;
} Binding;

SymbolTable *symboltable_new(void);
void symboltable_free(SymbolTable *table);

Binding *mkbind(const char *ident, Var *var);
void freebind(Binding *b);

Var *mkvar(int type, void *val);
void freevar(Var *v);

Binding *mkscope(void);
// mayhaps bool?
void symboltable_bind(SymbolTable *table, Binding *binding);
Var *symboltable_lookup(SymbolTable *table, const char *ident);
// i don't think we need specific markers for scopes bc it is a stack anyway right?
void scope_enter(SymbolTable *table);
void scope_exit(SymbolTable *table);

#endif /* _SYM_H */
