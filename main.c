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

#include "af/af_signal_block.h"
#include "af/af_signal_router.h"

#include "af/af_state_space.h"

#include "af/af_input_step.h"
#include "af/af_system_state_space.h"


#define TIME_STEP (0.01)

af_state_space *create_state_space(const double A[], const double B[],
								   const double C[], double X0[]);

af_signal_block *create_step_block(af_signal_router *router);
af_signal_block *create_state_space_block(af_signal_router *router,
										  af_state_space *state_space);

af_signal_block *create_feedback_block(af_signal_router *router);

int main() {
	const double A[] = { 1, 10,
						-1, -5 };

	const double B[] = { 0,
				   	   	 1 };

	const double C[] = { 0, 1 };

	double X0[] = { 0, 0 };

	af_signal_router *router = af_signal_router_alloc(0.00001, 0);

	af_state_space *st = create_state_space(A, B, C, X0);

	af_signal_block *step_block = create_step_block(router);
	af_signal_block *state_space_block = create_state_space_block(router, st);
	af_signal_block *feedback_block = create_feedback_block(router);

	while (router->time < 10.0) {

		af_signal_block_apply(step_block);

		af_signal_block_input_add_vector(feedback_block->input, step_block->output_vector);
		af_signal_block_input_add_vector(feedback_block->input, state_space_block->output_vector);

		af_signal_block_apply(feedback_block);

		af_signal_block_input_add_vector
			(state_space_block->input, feedback_block->output_vector);

		af_signal_block_apply(state_space_block);


		printf("%f: ", router->time);
		gsl_vector_fprintf(stdout, state_space_block->output_vector, "%f");

		router->time += router->step_size;
	}

	return EXIT_SUCCESS;
}

int feedback_fucntion(af_signal_block *block) {
	gsl_vector_set_all(block->output_vector, 0);

	gsl_vector_add(block->output_vector,
				   af_signal_block_input_get_vector_at(block->input, 0));

	gsl_vector_add(block->output_vector,
				   af_signal_block_input_get_vector_at(block->input, 1));

	return GSL_SUCCESS;
}

af_signal_block *create_feedback_block(af_signal_router *router) {
	af_signal_block *block = af_signal_block_alloc(2, 1);

	af_signal_block_set_handler(block, feedback_fucntion);
	af_signal_block_set_router(block, router);

	return block;
}

af_signal_block *create_step_block(af_signal_router *router) {
	af_signal_block *block = af_signal_block_alloc(0, 1);

	af_signal_block_set_params(block, af_input_step_params_alloc(0));
	af_signal_block_set_handler(block, af_input_step_function);
	af_signal_block_set_router(block, router);

	return block;
};

af_signal_block *create_state_space_block(af_signal_router *router,
										  af_state_space *state_space) {

	af_signal_block *block = af_signal_block_alloc(1, 1);

	af_signal_block_set_params
		(block, af_system_state_space_params_alloc(2, 0.01, state_space));

	af_signal_block_set_handler(block, af_system_state_space_function);
	af_signal_block_set_router(block, router);

	return block;
}

af_state_space *create_state_space(const double A[], const double B[],
								   const double C[], double X0[]) {

	af_state_space *state_space = af_state_space_alloc(2, 1, 1);
	af_state_space_set_state_matrix(state_space, A);
	af_state_space_set_input_matrix(state_space, B);
	af_state_space_set_output_matrix(state_space, C);
	af_state_space_set_state_vector(state_space, X0);

	return state_space;
};
