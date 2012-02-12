
#include "af_block_gain.h"

int af_block_gain_function(af_block * const block) {
	size_t i = 0;
	double * gain = (double *) block->params;
	gsl_vector * output = block->output->vector;
	const gsl_vector * input = af_block_input_get_vector_at(block->input, 0);

	while (i < output->size) {
		gsl_vector_set(output, i, gsl_vector_get(input, i) * gain[i]);

		i++;
	}

	return GSL_SUCCESS;
}

af_block *af_block_gain_alloc(const af_router * const router,
							  size_t input_dim, const double * gain) {
	af_block *block = af_block_alloc(1, input_dim);

	af_block_set_router(block, router);
	af_block_set_params(block, gain);
	af_block_set_handler(block, af_block_gain_function);

	return block;
}

af_block *af_block_inverse_alloc(const af_router * const router, size_t input_dim) {
	double * gain = (double *) malloc(sizeof(double) * input_dim);
	size_t i = 0;
	while (i < input_dim) {
		gain[i] = -1;

		i++;
	}

	return af_block_gain_alloc(router, input_dim, gain);
}

void af_block_gain_free(af_block * const block) {
	af_block_free(block);
}
