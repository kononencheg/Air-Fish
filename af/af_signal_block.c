/**
 * Air-Fish
 *
 * @file af_signal_block.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_signal_block.h"
#include "af_signal_router.h"

// signal block

af_signal_block *af_signal_block_alloc(size_t input_size, size_t output_dim) {

	af_signal_block *block =
			(af_signal_block *) malloc(sizeof(af_signal_block));

	block->params = NULL;
	block->handler = NULL;
	block->router = NULL;
	block->is_clear = 1;

	block->input = af_signal_block_input_alloc(input_size);

	if (output_dim == 0) {
		block->output_vector = NULL;
	} else {
		block->output_vector = gsl_vector_alloc(output_dim);
		gsl_vector_set_all(block->output_vector, 0);
	}

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
void af_signal_block_add_source_at(const af_signal_block * const block,
	 							   af_signal_block * const source,
	 							   const size_t index) {
	size_t size = block->input->size;			
	 							   
	if (index < size) {
		block->input->sources[index] = source;
	} else {
		fprintf(stderr, "Index is out of bounds: %d of %d\n", index, size);
	}
}

void af_signal_block_process(af_signal_block * const block) {
	af_signal_block_input_update(block->input);

	block->handler(block);
}

short int af_signal_block_is_clear(af_signal_block * const block) {
	int is_clear = block->is_clear;
	block->is_clear = 0;

	return is_clear;
}

void af_signal_block_reset(af_signal_block * const block) {
	af_signal_block_input_reset(block->input);
	block->is_clear = 1;
}

void af_signal_block_free(af_signal_block *block) {
	af_signal_block_input_free(block->input);
	gsl_vector_free(block->output_vector);
	free(block);
}

// signal block input

af_signal_block_input *af_signal_block_input_alloc(size_t size) {
	size_t i = 0;

	af_signal_block_input *input =
		(af_signal_block_input *) malloc(sizeof(af_signal_block_input));

	input->input_vectors =
			(const gsl_vector **) malloc(size * sizeof(gsl_vector));

	input->sources =
			(af_signal_block **) malloc(size * sizeof(af_signal_block));

	input->size = size;

	while (i < size) {
		input->input_vectors[i] = NULL;
		input->sources[i] = NULL;

		i++;
	}

	return input;
}

const gsl_vector * const af_signal_block_input_get_vector_at
	(const af_signal_block_input * const input, size_t index) {
	return input->input_vectors[index];
}

void af_signal_block_input_update(af_signal_block_input * const input) {
	const af_signal_block * source = NULL;

	size_t i = 0;
	while (i < input->size) {
		source = input->sources[i];

		if (source != NULL) {
			input->input_vectors[i] = source->output_vector;
		} else {
			fprintf(stderr, "Source doesn't set: %d\n", i);
		}

		i++;
	}
}

void af_signal_block_input_reset(af_signal_block_input * const input) {
	size_t i = 0;
	while (i < input->size) {
		input->input_vectors[i] = NULL;

		i++;
	}
}

void af_signal_block_input_free(af_signal_block_input *input) {
	free(input);
}
