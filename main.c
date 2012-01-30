/**
 * Air-Fish
 *
 * @file main.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "af/af_router.h"
#include "af/af_block.h"

#include "af/af_state_space.h"

#include "af/af_block_step.h"
#include "af/af_block_state_space.h"

af_state_space *create_state_space(const double A[], const double B[],
								   const double C[], double X0[]);

af_block *create_print_block(af_router *router);
af_block *create_feedback_block(af_router *router);

int main() {
	const double A[] = { 1, 10,
						-1, -5 };

	const double B[] = { 1,
				   	   	 0 };

	const double C[] = { 0, 1,
						 1, 0 };

	double X0[] = { 0, 0 };


	af_router *router = af_router_alloc(0.001, 0);

	af_state_space *st = create_state_space(A, B, C, X0);

	af_block *print_block = create_print_block(router);
	af_block *step_block = af_block_step_alloc(router, 0, 2);
	af_block *state_space_block = af_block_state_space_alloc(router, st);
	//af_block *feedback_block = create_feedback_block(router);

	//af_block_add_source_at(feedback_block, step_block, 0);
	//af_block_add_source_at(feedback_block, state_space_block, 1);
	af_block_add_source_at(state_space_block, step_block, 0);
	af_block_add_source_at(print_block, state_space_block, 0);

	af_router_add_block(router, step_block);
	//af_router_add_block(router, feedback_block);
	af_router_add_block(router, state_space_block);
	af_router_add_block(router, print_block);

	af_router_process(router, 10);

	return EXIT_SUCCESS;
}

int print_fucntion(af_block *block) {
	gsl_vector_fprintf(stdout, block->input->vectors[0], "%.5f");
	fprintf(stdout, "----\n");

	return GSL_SUCCESS;
}

af_block *create_print_block(af_router *router) {
	af_block *block = af_block_alloc(1, 0);

	af_block_set_handler(block, print_fucntion);
	af_block_set_router(block, router);

	return block;
}

int feedback_fucntion(af_block *block) {
	gsl_vector_set_all(block->output->vector, 0);

	gsl_vector_add(block->output->vector,
				   af_block_input_get_vector_at(block->input, 0));

	gsl_vector_add(block->output->vector,
				   af_block_input_get_vector_at(block->input, 1));

	return GSL_SUCCESS;
}

af_block *create_feedback_block(af_router *router) {
	af_block *block = af_block_alloc(2, 1);

	af_block_set_handler(block, feedback_fucntion);
	af_block_set_router(block, router);

	return block;
}


af_state_space *create_state_space(const double A[], const double B[],
								   const double C[], double X0[]) {
    
	af_state_space *state_space = af_state_space_alloc(2, 1, 2);

	af_state_space_set_state_matrix(state_space, A);
	af_state_space_set_input_matrix(state_space, B);
	af_state_space_set_output_matrix(state_space, C);
	af_state_space_set_state_vector(state_space, X0);

	return state_space;
};
