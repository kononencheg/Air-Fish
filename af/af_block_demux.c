
#include "af_block_demux.h"

int af_block_demux_function(af_block * const block) {
	size_t i = 0, shift = (size_t) block->params;

	gsl_vector * output = block->output->vector;

	const gsl_vector * input = af_block_input_get_vector_at(block->input, 0);

	while (i < output->size) {
		gsl_vector_set(output, i, gsl_vector_get(input, shift + i));

		i++;
	}

	return GSL_SUCCESS;
}

af_block *af_block_demux_alloc(const af_router * const router, size_t shift,
							   size_t length) {

	af_block *block = af_block_alloc(1, length);

	af_block_set_router(block, router);
	af_block_set_params(block, (void *) shift);
	af_block_set_handler(block, af_block_demux_function);

	return block;
}

void af_block_demux_free(af_block * const block) {
	af_block_free(block);
}
