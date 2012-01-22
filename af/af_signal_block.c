/**
 * Air-Fish
 *
 * @file af_signal_block.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_signal_block.h"

// signal block

af_signal_block *af_signal_block_alloc(size_t input_size, size_t output_dim) {

	af_signal_block *block =
			(af_signal_block *) malloc(sizeof(af_signal_block));

	block->params = NULL;
	block->handler = NULL;
	block->router = NULL;

	block->input = af_signal_block_input_alloc(input_size);
	block->output_vector = gsl_vector_alloc(output_dim);

	gsl_vector_set_all(block->output_vector, 0);

	return block;
}

void af_signal_block_set_handler(af_signal_block * const block,
								 af_signal_handler handler) {
	block->handler = handler;
}

void af_signal_block_set_router(af_signal_block * const block,
								af_signal_router * const router) {
	block->router = router;
}

void af_signal_block_set_params(af_signal_block * const block,
								const void * params) {
	block->params = params;
}

void af_signal_block_apply(af_signal_block * const block) {
	size_t length = block->input->length;
	size_t size = block->input->size;

	if (length == size) {
		block->handler(block);
	} else {
		fprintf(stderr, "Incomplete block input: has %d of %d\n", length, size);
	}

	af_signal_block_input_reset(block->input);
}

af_signal_block_input * const af_signal_block_get_input
	(af_signal_block * const block) {
	return block->input;
}

void af_signal_block_free(af_signal_block *block) {
	af_signal_block_input_free(block->input);
	gsl_vector_free(block->output_vector);
	free(block);
}

// signal block input

af_signal_block_input *af_signal_block_input_alloc(size_t size) {
	af_signal_block_input *input =
		(af_signal_block_input *) malloc(sizeof(af_signal_block_input));

	input->input_vectors =
			(const gsl_vector **) malloc(size * sizeof(gsl_vector));

	input->size = size;
	input->length = 0;

	af_signal_block_input_reset(input);

	return input;
}

void af_signal_block_input_add_vector(af_signal_block_input * const input,
									  const gsl_vector * const vector) {

	if (input->length < input->size) {
		input->input_vectors[input->length] = vector;
		input->length++;
	} else {
		fprintf(stderr, "Block input overflow: add %d of %d\n",
				input->length, input->size);
	}
}

const gsl_vector * const af_signal_block_input_get_vector_at
	(const af_signal_block_input * const input, size_t index) {
	return input->input_vectors[index];
}

void af_signal_block_input_reset(af_signal_block_input *input) {
	while (input->length > 0) {
		input->input_vectors[--input->length] = NULL;
	}
}

void af_signal_block_input_free(af_signal_block_input *input) {
	free(input);
}
