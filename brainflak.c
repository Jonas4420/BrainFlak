#include "brainflak.h"

#include <stdio.h>

static int brainflak_exec(struct brainflak_ctx*, const char*, int*, const char**);
static bool brainflak_alphabet(char);
static int (*brainflak_getfunc(char))(struct brainflak_ctx*, const char*, int*, const char**);
static int brainflak_parenthesis(struct brainflak_ctx*, const char*, int*, const char**);
static int brainflak_bracket(struct brainflak_ctx*, const char*, int*, const char**);
static int brainflak_brace(struct brainflak_ctx*, const char*, int*, const char**);
static int brainflak_angle(struct brainflak_ctx*, const char*, int*, const char**);


int
brainflak_init(struct brainflak_ctx *ctx)
{
	int ret = 1;

	if ( NULL == ctx ) { goto cleanup; }

	ctx->active_stack = 0;
	ctx->stacks[0]     = NULL;
	ctx->stacks[1]     = NULL;

	ctx->stacks[0] = malloc(sizeof(struct stack));
	if ( NULL == ctx->stacks[0] ) { goto cleanup; }

	ret = stack_init(ctx->stacks[0]);
	if ( 0 != ret ) { goto cleanup; }

	ctx->stacks[1] = malloc(sizeof(struct stack));
	if ( NULL == ctx->stacks[1] ) { goto cleanup; }

	ret = stack_init(ctx->stacks[1]);
	if ( 0 != ret ) { goto cleanup; }

	ret = 0;
cleanup:
	if ( 0 != ret ) {
		brainflak_free(ctx);
	}

	return ret;
}

void
brainflak_free(struct brainflak_ctx *ctx)
{
	if ( NULL == ctx ) { return; }

	if ( NULL != ctx->stacks[0] ) {
		stack_free(ctx->stacks[0]);
		free(ctx->stacks[0]);
		ctx->stacks[0] = NULL;
	}

	if ( NULL != ctx->stacks[1] ) {
		stack_free(ctx->stacks[1]);
		free(ctx->stacks[0]);
		ctx->stacks[1] = NULL;
	}
}

int
brainflak_add_arg(struct brainflak_ctx *ctx, int arg)
{
	int ret = 1;

	if ( NULL == ctx )            { goto cleanup; }
	if ( NULL == ctx->stacks[0] ) { goto cleanup; }
	if ( NULL == ctx->stacks[1] ) { goto cleanup; }

	ret = stack_push(ctx->stacks[ctx->active_stack], arg);
	if ( 0 != ret ) { goto cleanup; }

	ret = 0;
cleanup:
	return ret;
}

int
brainflak_run(struct brainflak_ctx *ctx, const char *prog)
{
	int ret = 1;
	const char *ret_code;

	if ( NULL == ctx )  { goto cleanup; }
	if ( NULL == prog ) { goto cleanup; }

	// Check that every character is in the alphabet
	for ( const char *c = prog ; *c ; ++c ) {
		if ( ! brainflak_alphabet(*c) ) { goto cleanup; }
	}

	// Execute program
	ret = brainflak_exec(ctx, prog, NULL, &ret_code);
	if ( 0 != ret )          { goto cleanup; }
	if ( '\0' != *ret_code ) { goto cleanup; }

	// Print active stack value
	int val;
	while ( stack_size(ctx->stacks[ctx->active_stack]) > 0 ) {
		stack_pop(ctx->stacks[ctx->active_stack], &val);
		printf("%d\n", val);
	}

	ret = 0;
cleanup:
	if ( 0 != ret ) {
		fprintf(stderr, "Program error...\n");
	}

	return ret;
}

bool
brainflak_alphabet(char c)
{
	bool result = false;

	switch ( c ) {
		case '(': case ')':
		case '[': case ']':
		case '{': case '}':
		case '<': case '>':
			result = true;
			break;
		default:
			result = false;
			break;
	}

	return result;
}

int
(*brainflak_getfunc(char c))(struct brainflak_ctx*, const char*, int*, const char**)
{
	int (*result)(struct brainflak_ctx*, const char*, int*, const char**);

	result = NULL;
	
	switch ( c ) {
		case '(':
			result = brainflak_parenthesis;
			break;
		case '[':
			result = brainflak_bracket;
			break;
		case '{':
			result = brainflak_brace;
			break;
		case '<':
			result = brainflak_angle;
			break;
		default:
			result = NULL;
			break;
	}

	return result;
}

int
brainflak_exec(struct brainflak_ctx *ctx, const char *prog, int *ret_value, const char **ret_code)
{
	int ret = 1;
	int my_ret_value, result;
	const char *my_ret_code;
	bool end_parsing;

	my_ret_value = 0;
	end_parsing  = false;

	while ( ! end_parsing ) {
		switch ( *prog ) {
			case '(':
			case '[':
			case '{':
			case '<':
				ret = brainflak_getfunc(*prog)(ctx, prog, &result, &my_ret_code);
				if ( 0 != ret ) { goto cleanup; }

				my_ret_value += result;
				prog = my_ret_code + 1;
				break;
			default:
				end_parsing = true;
				break;
		}
	}

	if ( NULL != ret_value ) { *ret_value = my_ret_value; }
	if ( NULL != ret_code )  { *ret_code  = prog; }

	ret = 0;
cleanup:
	return ret;
}

int
brainflak_parenthesis(struct brainflak_ctx *ctx, const char *prog, int *ret_value, const char **ret_code)
{
	int ret = 1;
	int my_ret_value;
	const char *my_ret_code;

	if ( '(' != *prog ) { goto cleanup; }

	ret = brainflak_exec(ctx, prog + 1, &my_ret_value, &my_ret_code);
	if ( 0   != ret )          { goto cleanup; }
	if ( ')' != *my_ret_code ) { ret = 1; goto cleanup; }

	if ( prog + 1 == my_ret_code ) {
		my_ret_value = 1;
	} else {
		ret = stack_push(ctx->stacks[ctx->active_stack], my_ret_value);
		if ( 0 != ret ) { goto cleanup; }
	}

	if ( NULL != ret_value ) { *ret_value = my_ret_value; }
	if ( NULL != ret_code )  { *ret_code  = my_ret_code; }

	ret = 0;
cleanup:
	return ret;
}

int
brainflak_bracket(struct brainflak_ctx *ctx, const char *prog, int *ret_value, const char **ret_code)
{
	int ret = 1;
	int my_ret_value;
	const char *my_ret_code;

	if ( '[' != *prog ) { goto cleanup; }

	ret = brainflak_exec(ctx, prog + 1, &my_ret_value, &my_ret_code);
	if ( 0   != ret )          { goto cleanup; }
	if ( ']' != *my_ret_code ) { ret = 1; goto cleanup; }

	if ( prog + 1 == my_ret_code ) {
		my_ret_value = stack_size(ctx->stacks[ctx->active_stack]);
	} else {
		my_ret_value *= -1;
	}

	if ( NULL != ret_value ) { *ret_value = my_ret_value; }
	if ( NULL != ret_code )  { *ret_code  = my_ret_code; }

	ret = 0;
cleanup:
	return ret;
}

int
brainflak_brace(struct brainflak_ctx *ctx, const char *prog, int *ret_value, const char **ret_code)
{
	int ret = 1;
	int my_ret_value, top, result;
	const char *my_ret_code;
	bool stop_loop;

	result = 0;

	do {
		stop_loop = true;

		if ( '{' != *prog ) { goto cleanup; }

		ret = brainflak_exec(ctx, prog + 1, &my_ret_value, &my_ret_code);
		if ( 0   != ret )          { goto cleanup; }
		if ( '}' != *my_ret_code ) { ret = 1; goto cleanup; }

		if ( prog + 1 == my_ret_code ) {
			if ( stack_size(ctx->stacks[ctx->active_stack]) > 0 ) {
				ret = stack_pop(ctx->stacks[ctx->active_stack], &my_ret_value);
			} else {
				my_ret_value = 0;
			}
		} else {
			result += my_ret_value;

			if ( stack_size(ctx->stacks[ctx->active_stack]) > 0 ) {
				ret = stack_top(ctx->stacks[ctx->active_stack], &top);
			} else {
				top = 0;
			}

			if ( 0 != top ) {
				stop_loop = false;
			} else {
				my_ret_value = result;
			}
		}
	} while ( ! stop_loop );

	if ( NULL != ret_value ) { *ret_value = my_ret_value; }
	if ( NULL != ret_code )  { *ret_code  = my_ret_code; }

	ret = 0;
cleanup:
	return ret;
}

int
brainflak_angle(struct brainflak_ctx *ctx, const char *prog, int *ret_value, const char **ret_code)
{
	int ret = 1;
	int my_ret_value;
	const char *my_ret_code;

	if ( '<' != *prog ) { goto cleanup; }

	ret = brainflak_exec(ctx, prog + 1, &my_ret_value, &my_ret_code);
	if ( 0   != ret )          { goto cleanup; }
	if ( '>' != *my_ret_code ) { ret = 1; goto cleanup; }

	my_ret_value = 0;

	if ( prog + 1 == my_ret_code ) {
		ctx->active_stack = 1 - ctx->active_stack;
	}

	if ( NULL != ret_value ) { *ret_value = my_ret_value; }
	if ( NULL != ret_code )  { *ret_code  = my_ret_code; }

	ret = 0;
cleanup:
	return ret;
}
