/**
 * Air-Fish
 *
 * @file af_block.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_block.h"
#include "af_router.h"

// signal block

af_block *af_block_alloc(size_t input_size, size_t output_dim) {

	af_block *block =
			(af_block *) malloc(sizeof(af_block));

	block->params = NULL;
	block->handler = NULL;
	block->router = NULL;
	block->is_clear = 1;

	block->input = af_block_input_alloc(input_size);

	if (output_dim == 0) {
		block->output = NULL;
	} else {
		block->output = af_block_output_alloc(output_dim);
	}

	return block;
}

void af_block_set_handler(af_block * const block,
						  const af_block_handler handler) {
	block->handler = handler;
}

void af_block_set_router(af_block * const block,
						 const af_router * const router) {
	block->router = router;
}

void af_block_set_params(af_block * const block,
								const void * params) {
	block->params = params;
}
void af_block_add_source_at(const af_block * const block,
	 							   af_block * const source,
	 							   const size_t index) {
	size_t size = block->input->size;			
	 							   
	if (index < size) {
		block->input->sources[index] = source;
	} else {
		fprintf(stderr, "Index is out of bounds: %d of %d\n", index, size);
	}
}

void af_block_process(af_block * const block) {
	af_block_input_update(block->input);

	block->handler(block);
}

short int af_block_is_clear(af_block * const block) {
	int is_clear = block->is_clear;
	block->is_clear = 0;

	return is_clear;
}

void af_block_reset(af_block * const block) {
	af_block_input_reset(block->input);
	block->is_clear = 1;
}

void af_block_free(af_block *block) {
	af_block_output_free(block->output);
	af_block_input_free(block->input);
	free(block);
}

// signal block input

af_block_input *af_block_input_alloc(size_t size) {
	size_t i = 0;

	af_block_input *input =
		(af_block_input *) malloc(sizeof(af_block_input));

	input->vectors = (const gsl_vector **) malloc(size * sizeof(gsl_vector));
	input->sources = (af_block **) malloc(size * sizeof(af_block));

	input->size = size;

	while (i < size) {
		input->vectors[i] = NULL;
		input->sources[i] = NULL;

		i++;
	}

	return input;
}

const gsl_vector * const af_block_input_get_vector_at
	(const af_block_input * const input, size_t index) {
	return input->vectors[index];
}

void af_block_input_update(af_block_input * const input) {
	const af_block * source = NULL;

	size_t i = 0;
	while (i < input->size) {
		source = input->sources[i];

		if (source != NULL) {
			input->vectors[i] = source->output->vector;
		} else {
			fprintf(stderr, "Source doesn't set: %d\n", i);
		}

		i++;
	}
}

void af_block_input_reset(af_block_input * const input) {
	size_t i = 0;
	while (i < input->size) {
		input->vectors[i] = NULL;

		i++;
	}
}

void af_block_input_free(af_block_input *input) {
	free(input);
}

af_block_output *af_block_output_alloc(size_t output_dim) {
	af_block_output *output =
		(af_block_output *) malloc(sizeof(af_block_output));

	output->vector = gsl_vector_alloc(output_dim);
	gsl_vector_set_all(output->vector, 0);

	return output;
}

void af_block_output_free(af_block_output *output) {
	gsl_vector_free(output->vector);
	free(output);
}
