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

#include "af/af_state_space.h"
#include "af/af_step_response.h"

#include "af/af_block.h"
#include "af/af_block_step.h"
#include "af/af_block_feedback.h"
#include "af/af_block_state_space.h"
#include "af/af_block_step_response.h"

#define TIME (20)
#define STEP (0.01)

af_block *create_print_block(af_router *router);

int main() {
	const double A[] = { 0, -5,
						 1, -10 };

	const double B[] = { 1,
				   	   	 1 };

	const double C[] = { 0, 1 };

	double X0[] = { 0, 0 };

	af_router *router = af_router_alloc(STEP, 0);

	af_state_space *state_space = af_state_space_alloc(2, 1, 1);

	af_state_space_set_state_matrix(state_space, A);
	af_state_space_set_input_matrix(state_space, B);
	af_state_space_set_output_matrix(state_space, C);
	af_state_space_set_state_vector(state_space, X0);

	af_step_response *response = af_step_response_alloc(TIME/STEP, 0.01);

	af_block *state_space_block = af_block_state_space_alloc(router, state_space);
	af_block *feedback_block 	= af_block_feedback_alloc(router, 2);
	af_block *step_block 		= af_block_step_alloc(router, 0, 1);
	af_block *response_block 	= af_block_step_response_alloc(router, response);
	af_block *print_block 		= create_print_block(router);

	af_block_add_source_at(feedback_block, step_block, 0);
	af_block_add_source_at(feedback_block, state_space_block, 1);

	af_block_add_source_at(state_space_block, feedback_block, 0);

	af_block_add_source_at(print_block, state_space_block, 0);
	af_block_add_source_at(response_block, state_space_block, 0);

	af_router_add_block(router, step_block);
	af_router_add_block(router, feedback_block);
	af_router_add_block(router, state_space_block);
	af_router_add_block(router, print_block);
	af_router_add_block(router, response_block);

	af_router_process(router, TIME);

	printf("%f\n", af_step_response_get_steady_value(response));
	printf("%f\n", af_step_response_get_overshoot(response));
	printf("%f\n", af_step_response_get_settling_time(response));

	return EXIT_SUCCESS;
}

int print_fucntion(af_block *block) {
	const gsl_vector * input = af_block_input_get_vector_at(block->input, 0);
	size_t i = 0;

	printf("[time]\t%f\t[out] ", block->router->time);

	while (i < input->size) {
		printf("%d: %f\t", i, gsl_vector_get(input, i));
		i++;
	}

	printf("\n");

	return GSL_SUCCESS;
}

af_block *create_print_block(af_router *router) {
	af_block *block = af_block_alloc(1, 0);

	af_block_set_handler(block, print_fucntion);
	af_block_set_router(block, router);

	return block;
}
