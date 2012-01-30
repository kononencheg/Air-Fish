/**
 * Air-Fish
 *
 * @file af_source_signals.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_block_step.h"

#include "af_block.h"
#include "af_router.h"

af_block_step_params *af_block_step_params_alloc(double shift, double value) {
	af_block_step_params *params = (af_block_step_params *)
			malloc(sizeof(af_block_step_params));

	params->shift = shift;
	params->value = value;

	return params;
}

void af_block_step_params_free(af_block_step_params *params) {
	free(params);
}

int af_block_step_function(af_block * const block) {
	af_block_step_params *params = (af_block_step_params *) block->params;

	gsl_vector_set(block->output->vector, 0,
				   block->router->time >= params->shift ? params->value : 0);

	return GSL_SUCCESS;
}

af_block *af_block_step_alloc(const af_router * const router,
							  double shift, double value) {

	af_block *block = af_block_alloc(0, 1);

	af_block_set_params(block, af_block_step_params_alloc(shift, value));
	af_block_set_handler(block, af_block_step_function);
	af_block_set_router(block, router);

	return block;
}

void af_block_step_free(af_block * const block) {
	af_block_step_params_free((af_block_step_params *) block->params);
	af_block_free(block);
}
