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

struct _af_signal_router;
struct _af_signal_block;
struct _af_signal_block_input;

typedef int (* af_signal_handler) (struct _af_signal_block * const block);

// signal block

typedef struct _af_signal_block {
    const void * params;

    af_signal_handler handler;

    struct _af_signal_router * router;

    gsl_vector * output_vector;

    struct _af_signal_block_input * input;

    short int is_clear;

} af_signal_block;

af_signal_block *af_signal_block_alloc(size_t input_size, size_t output_dim);

void af_signal_block_set_params(af_signal_block * const block,
								const void * params);

void af_signal_block_set_handler(af_signal_block * const block,
								 af_signal_handler handler);

void af_signal_block_set_router(af_signal_block * const block,
								struct _af_signal_router * const router);

void af_signal_block_add_source_at(const af_signal_block * const block,
	 							   af_signal_block * const source,
	 							   const size_t index);
	 							   
void af_signal_block_process(af_signal_block * const block);

short int af_signal_block_is_clear(af_signal_block * const block);

void af_signal_block_reset(af_signal_block * const block);


void af_signal_block_free(af_signal_block * block);


// signal block input

typedef struct _af_signal_block_input {
    const gsl_vector ** input_vectors;
    af_signal_block ** sources;

    size_t size;

} af_signal_block_input;

af_signal_block_input *af_signal_block_input_alloc(size_t size);

const gsl_vector * const af_signal_block_input_get_vector_at
	(const af_signal_block_input * const input, const size_t index);

void af_signal_block_input_update(af_signal_block_input * const input);

void af_signal_block_input_reset(af_signal_block_input * const input);

void af_signal_block_input_free(af_signal_block_input *input);


#endif /* AF_SIGNAL_BLOCK_H_ */
