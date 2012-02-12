
#include "af_block_print.h"

int af_block_print_function(af_block * const block) {
	const gsl_vector * input = af_block_input_get_vector_at(block->input, 0);
	size_t i = 0;

	printf("[time]\t%f  [out] ", block->router->time);

	while (i < input->size) {
		printf("%d: %f\t", i, gsl_vector_get(input, i));
		i++;
	}

	printf("\n");

	return GSL_SUCCESS;
}

af_block *af_block_print_alloc(const af_router * const router) {
	af_block *block = af_block_alloc(1, 0);

	af_block_set_handler(block, af_block_print_function);
	af_block_set_router(block, router);

	return block;
}

void af_block_print_free(af_block * const block) {
	af_block_free(block);
}
