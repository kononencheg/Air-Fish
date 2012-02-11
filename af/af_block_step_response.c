/**
 * Air-Fish
 *
 * @file af_block_step_response.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <gsl/gsl_vector.h>

#include "af_block_step_response.h"

int af_block_step_response_function(af_block * const block) {
	af_step_response *response = (af_step_response *) block->params;
	const gsl_vector *input = af_block_input_get_vector_at(block->input, 0);

	af_step_response_add_value
		(response, block->router->time, gsl_vector_get(input, 0));

	return GSL_SUCCESS;
}

af_block *af_block_step_response_alloc(const af_router * const router,
									   af_step_response * const response) {
	af_block *block = af_block_alloc(1, 0);

	af_block_set_router(block, router);
	af_block_set_params(block, response);
	af_block_set_handler(block, af_block_step_response_function);

	return block;
}

void af_block_step_response_free(af_block * const block) {
	af_block_free(block);
}
