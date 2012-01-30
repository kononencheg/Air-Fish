/**
 * Air-Fish
 *
 * @file af_signal_processor.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_BLOCK_H_
#define AF_BLOCK_H_

#include <gsl/gsl_vector.h>

#include "af_router.h"

struct _af_router;
struct _af_block;
struct _af_block_input;
struct _af_block_output;

typedef int (* af_block_handler) (struct _af_block * const block);

// signal block

typedef struct _af_block {
    const void * params;

    af_block_handler handler;

    const struct _af_router * router;

    struct _af_block_output * output;
    struct _af_block_input * input;

    short int is_clear;

} af_block;

af_block *af_block_alloc(size_t input_size, size_t output_dim);

void af_block_set_params(af_block * const block, const void * params);

void af_block_set_handler(af_block * const block,
						  const af_block_handler handler);

void af_block_set_router(af_block * const block,
						 const struct _af_router * const router);

void af_block_add_source_at(const af_block * const block,
	 							   af_block * const source,
	 							   const size_t index);
	 							   
void af_block_process(af_block * const block);

short int af_block_is_clear(af_block * const block);

void af_block_reset(af_block * const block);


void af_block_free(af_block * block);



typedef struct _af_block_input {
    const gsl_vector ** vectors;
    af_block ** sources;

    size_t size;

} af_block_input;

af_block_input *af_block_input_alloc(size_t size);

const gsl_vector * const af_block_input_get_vector_at
	(const af_block_input * const input, const size_t index);

void af_block_input_update(af_block_input * const input);

void af_block_input_reset(af_block_input * const input);

void af_block_input_free(af_block_input *input);



typedef struct _af_block_output {
    gsl_vector * vector;
} af_block_output;

af_block_output *af_block_output_alloc(size_t output_dim);

void af_block_output_free(af_block_output *output);

#endif /* AF_BLOCK_H_ */
