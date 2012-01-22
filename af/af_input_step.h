/**
 * Air-Fish
 *
 * @file af_input_heaviside_step.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_INPUT_STEP_H_
#define AF_INPUT_STEP_H_

#include <gsl/gsl_vector.h>

#include "af_signal_block.h"

typedef struct {
	double shift;
} af_input_step_params;

af_input_step_params *af_input_step_params_alloc(double shift);

void af_input_step_params_free(af_input_step_params *params);

int af_input_step_function(af_signal_block *block);


#endif /* AF_INPUT_HEAVISIDE_STEP_H_ */
