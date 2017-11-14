#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct stack
{
	size_t size;
	size_t max_size;
	int *stack;
};

int stack_init(struct stack*);
void stack_free(struct stack*);

int stack_push(struct stack*, int);
int stack_top(struct stack*, int*);
int stack_pop(struct stack*, int*);

size_t stack_size(struct stack*);

#endif
