/**
 * Air-Fish
 *
 * @file af_input_heaviside_step.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_BLOCK_STEP_H_
#define AF_BLOCK_STEP_H_

#include <gsl/gsl_vector.h>

#include "af_block.h"
#include "af_router.h"

typedef struct {
	double shift;
	double value;
} af_block_step_params;

af_block_step_params *af_block_step_params_alloc(double shift, double value);

void af_block_step_params_free(af_block_step_params *params);

int af_block_step_function(af_block * const block);

af_block *af_block_step_alloc(const af_router * const router,
							  double shift, double value);

void af_block_step_free(af_block * const block);


#endif /* AF_BLOCK_STEP_H_ */
