
#include "af_block_mux.h"

int af_block_mux_function(af_block * const block) {
	const gsl_vector * input;// =
	gsl_vector * output = block->output->vector;

	size_t i = 0, k = 0, j;
	while (i < block->input->size && k < output->size) {
		input = af_block_input_get_vector_at(block->input, i);

		j = 0;
		while (j < input->size && k < output->size) {
			gsl_vector_set(output, k, gsl_vector_get(input, j));
			j++; k++;
		}

		i++;
	}

	return GSL_SUCCESS;
}

af_block *af_block_mux_alloc(const af_router * const router,
							 size_t input_size, size_t output_dim) {

	af_block *block = af_block_alloc(input_size, output_dim);

	af_block_set_router(block, router);
	af_block_set_handler(block, af_block_mux_function);

	return block;
}

void af_block_mux_free(af_block * const block) {
	af_block_free(block);
}
