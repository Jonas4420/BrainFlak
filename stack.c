#include "stack.h"

# define STACK_SIZE 1024

int
stack_init(struct stack *ctx)
{
	int ret = 1;
	if ( NULL == ctx ) { goto cleanup; }

	ctx->size = 0;

	ctx->stack = calloc(STACK_SIZE, sizeof(int));
	if ( NULL == ctx->stack ) { goto cleanup; }

	ctx->max_size = STACK_SIZE;

	ret = 0;
cleanup:
	return ret;
}

void
stack_free(struct stack *ctx)
{
	if ( NULL == ctx ) { return; }

	if ( NULL != ctx->stack ) {
		free(ctx->stack);
	}

	ctx->stack    = NULL;
	ctx->size     = 0;
	ctx->max_size = 0;
}

int
stack_push(struct stack *ctx, int val)
{
	int ret = 1;

	if ( NULL == ctx )        { goto cleanup; }
	if ( NULL == ctx->stack ) { goto cleanup; }
	if ( 0 == ctx->max_size ) { goto cleanup; }

	if ( ctx->size == ctx->max_size ) {
		ctx->stack = realloc(ctx->stack, ctx->max_size + STACK_SIZE);
	}

	ctx->stack[ctx->size] = val;
	++ctx->size;

	ret = 0;
cleanup:
	return ret;
}

int
stack_top(struct stack *ctx, int *val)
{
	int ret = 1;

	if ( NULL == ctx )    { goto cleanup; }
	if ( 0 >= ctx->size ) { goto cleanup; }
	if ( NULL == val )    { goto cleanup; }

	*val = ctx->stack[ctx->size - 1];
	
	ret = 0;
cleanup:
	return ret;
}

int
stack_pop(struct stack *ctx, int *val)
{
	int ret = 1;

	if ( NULL == ctx )    { goto cleanup; }
	if ( 0 >= ctx->size ) { goto cleanup; }

	if ( NULL != val ) {
		stack_top(ctx, val);
	}

	--ctx->size;

	ret = 0;
cleanup:
	return ret;
}

size_t
stack_size(struct stack *ctx)
{
	return ctx ? ctx->size : 0;
}
