#ifndef _STACK_H
#define _STACK_H

#include <stddef.h>
#include <stdint.h>

#define STACK_CAP 256

typedef struct {
	size_t cap;
	uintptr_t top; // this is an index idk what type to use
	void **buf;
} Stack;

Stack *stack_new(void);
void stack_free(Stack *stack);

void stack_push(Stack *stack, void *item);
void *stack_pop(Stack *stack);

#endif /* _STACK_H */
