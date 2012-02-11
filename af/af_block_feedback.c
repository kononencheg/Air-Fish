/**
 * Air-Fish
 *
 * @file af_block_feedback.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_block_feedback.h"

int af_block_feedback_function(af_block * const block) {
	gsl_vector_set_all(block->output->vector, 0);
	size_t input_size = (size_t) block->params;
	size_t i = 0;

	while (i < input_size) {
		gsl_vector_add(block->output->vector,
					   af_block_input_get_vector_at(block->input, i));

		i++;
	}

	return GSL_SUCCESS;
}

af_block *af_block_feedback_alloc(const af_router * const router,
							  	  size_t input_size) {

	af_block *block = af_block_alloc(input_size, 1);

	af_block_set_router(block, router);
	af_block_set_params(block, (void *) input_size);
	af_block_set_handler(block, af_block_feedback_function);

	return block;
}

void af_block_feedback_free(af_block * const block) {
	af_block_free(block);
}

