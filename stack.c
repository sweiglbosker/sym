#include <stdlib.h>

#include "stack.h"

Stack *stack_new(void) {
	Stack *stack = calloc(1, sizeof(Stack));
	stack->buf = (void**)calloc(STACK_CAP, sizeof(void *));
	stack->cap = STACK_CAP;
	stack->top = 0;

	return stack;
}

void stack_free(Stack *stack) {
	free(stack->buf);
	free(stack);
}

void stack_push(Stack *stack, void *val) {
	if (++stack->top >= stack->cap + 1) {
		stack->cap = stack->cap * 2;
		stack->buf = reallocarray(stack->buf, stack->cap, sizeof(void *));
		// can error check here
	}
	stack->buf[stack->top] = val;
}

void *stack_pop(Stack *stack) {
	if (stack->top == 0) { /* doing it like this bc idk how overflow works for uintptr_t */
		return NULL;
	}

	return stack->buf[(stack->top)--];
}
