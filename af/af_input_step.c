/**
 * Air-Fish
 *
 * @file af_source_signals.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_input_step.h"

#include "af_signal_block.h"
#include "af_signal_router.h"

af_input_step_params *af_input_step_params_alloc(double shift) {
	af_input_step_params *params = (af_input_step_params *)
			malloc(sizeof(af_input_step_params));

	params->shift = shift;

	return params;
}

void af_input_step_params_free(af_input_step_params *params) {
	free(params);
}

int af_input_step_function(af_signal_block *block) {
	af_input_step_params *params = (af_input_step_params *) block->params;

	gsl_vector_set(block->output_vector, 0,
				   block->router->time >= params->shift ? 1 : 0);

	return GSL_SUCCESS; //GSL_ERANGE
}




