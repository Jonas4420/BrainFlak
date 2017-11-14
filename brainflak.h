#ifndef BRAINFLAK_H
#define BRAINFLAK_H

#include "stack.h"

#include <stdbool.h>

struct brainflak_ctx
{
	int active_stack;
	struct stack* stacks[2];
};

int brainflak_init(struct brainflak_ctx*);
void brainflak_free(struct brainflak_ctx*);

int brainflak_add_arg(struct brainflak_ctx*, int);
int brainflak_run(struct brainflak_ctx*, const char*);

#endif
