#include <stdio.h>
#include <stdlib.h>

#include "ht.h"
#include "stack.h"

int main(void) {
	Ht *ht = ht_new();

	int a = 54, b = 32;
	
	ht_bind(ht, "joe", &a);
	ht_bind(ht, "bob", &b);

	int *joe = ht_lookup(ht, "joe");
	int *bob = ht_lookup(ht, "bob");

	printf("ht[\"joe\"]: %i\n", *joe);
	printf("ht[\"bob\"]: %i\n", *bob);

	ht_bind(ht, "joe", &b);
	int *joe2 = ht_lookup(ht, "joe");

	printf("ht[\"joe\"]: %i\n", *joe2);

	Stack *stack = stack_new();

	int items[5] = {1, 2, 3, 4, 5};

	for (int i = 0; i < 5; i++) {
		printf("value %i pushed to stack\n", items[i]);
		stack_push(stack, &items[i]);
	}

	for (int *val = stack_pop(stack); val != NULL; val = stack_pop(stack)) {
		printf("value %i popped from stack!\n", *val);
	}
}
