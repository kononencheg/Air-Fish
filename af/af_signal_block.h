/**
 * Air-Fish
 *
 * @file af_signal_processor.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_SIGNAL_BLOCK_H_
#define AF_SIGNAL_BLOCK_H_

#include <gsl/gsl_vector.h>

#include "af_signal_router.h"

typedef struct _af_signal_block af_signal_block;
typedef struct _af_signal_block_input af_signal_block_input;

typedef int (* af_signal_handler) (af_signal_block * const block);

// signal block

struct _af_signal_block {
    const void * params;

    af_signal_handler handler;

    af_signal_router * router;

    gsl_vector * output_vector;

    af_signal_block_input * input;
};

af_signal_block *af_signal_block_alloc(size_t input_size, size_t output_dim);

void af_signal_block_set_params(af_signal_block * const block,
								const void * params);

void af_signal_block_set_handler(af_signal_block * const block,
								 af_signal_handler handler);

void af_signal_block_set_router(af_signal_block * const block,
								af_signal_router * const router);

void af_signal_block_apply(af_signal_block * const block);

af_signal_block_input * const af_signal_block_get_input
	(af_signal_block * const block);

void af_signal_block_free(af_signal_block *block);


// signal block input

struct _af_signal_block_input {
    const gsl_vector **input_vectors;

    size_t length;
    size_t size;
};

af_signal_block_input *af_signal_block_input_alloc(size_t size);

void af_signal_block_input_add_vector(af_signal_block_input * const input,
									  const gsl_vector * const vector);

const gsl_vector * const af_signal_block_input_get_vector_at
	(const af_signal_block_input * const input, size_t index);

void af_signal_block_input_reset(af_signal_block_input * const input);

void af_signal_block_input_free(af_signal_block_input *input);


#endif /* AF_SIGNAL_BLOCK_H_ */