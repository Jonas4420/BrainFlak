#include <stdio.h>
#include <stdlib.h>

#include "brainflak.h"

int
main(int argc, const char *argv[])
{
	int ret = EXIT_FAILURE, res = 0;
	struct brainflak_ctx *ctx = NULL;

	// Check that we have at least a program to run
	if ( 2 > argc ) { goto cleanup; }

	// Create the context 
	ctx = malloc(sizeof(struct brainflak_ctx));
	if ( NULL == ctx ) { goto cleanup; }

	res = brainflak_init(ctx);
	if ( 0 != res ) { goto cleanup; }

	// Push arguments to the stack
	for ( int i = 2 ; i < argc ; ++i ) {
		res = brainflak_add_arg(ctx, atoi(argv[i]));
		if ( 0 != res ) { goto cleanup; }
	}

	// Run the program
	res = brainflak_run(ctx, argv[1]);
	if ( 0 != res ) { goto cleanup; }

	ret = EXIT_SUCCESS;
cleanup:
	// Free the context
	if ( NULL != ctx ) { brainflak_free(ctx); free(ctx); }
	return ret;
}
