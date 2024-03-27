#include <stdio.h>
#include <stdlib.h>

#include "sym.h"

int main(void) {
	SymbolTable *st = symboltable_new();

	int a = 45;
	int b = 7;
	int c = 32;

	Var *a_var = mkvar(TYPE_INT, &a);
	Var *b_var = mkvar(TYPE_INT, &b);
	Var *c_var = mkvar(TYPE_INT, &c);

	printf("var a = %i\nvar b = %i\n", a, b);

	symboltable_bind(st, mkbind("a", a_var));
	symboltable_bind(st, mkbind("b", b_var)); 

	Var *a_lookup = symboltable_lookup(st, "a");
	Var *b_lookup = symboltable_lookup(st, "b");

	printf("st[\"a\"].val: %i\n", *(int*)(a_lookup->val));
	printf("st[\"b\"].val: %i\n", *(int*)(b_lookup->val));

	printf("{\n var c = %i\n", c);
	scope_enter(st);
	symboltable_bind(st, mkbind("c", c_var));
	Var *c_lookup = symboltable_lookup(st, "c");
	printf("st[\"c\"].val: %i\n}\n", *(int*)(c_lookup->val));
	scope_exit(st);

	Var *c_lookup2 = symboltable_lookup(st, "c");
	if (c_lookup2 == NULL) {
		printf("st[\"c\"].val: NULL\n");
	} else {
		printf("something is broken\n");
	}
	
}
