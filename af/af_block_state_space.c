/**
 * Air-Fish
 *
 * @file af_block_state_space.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#include "af_router.h"
#include "af_block.h"

#include "af_block_state_space.h"

#include <gsl/gsl_blas.h>

af_block_state_space_params *af_block_state_space_params_alloc
	(double step_size, af_state_space * const state_space) {

	af_block_state_space_params *params = (af_block_state_space_params *)
			malloc(sizeof(af_block_state_space_params));

	size_t state_dim = state_space->state_matrix->size1;

	params->system = (gsl_odeiv_system *) malloc(sizeof(gsl_odeiv_system));
	params->system->function = af_state_space_function;
	params->system->dimension = state_dim;
	params->system->params = state_space;

	params->step = gsl_odeiv_step_alloc(gsl_odeiv_step_rk8pd, state_dim);
	params->control = gsl_odeiv_control_y_new(step_size, 0.0);
	params->evolve = gsl_odeiv_evolve_alloc(state_dim);

	params->state_space = state_space;

	return params;
}

void af_block_state_space_params_free(af_block_state_space_params *params) {

}

int af_block_state_space_function(af_block * const block) {
	int result = GSL_FAILURE;

	double time = block->router->time, // TODO: Try 0!
		   step = block->router->step_size;

	af_block_state_space_params *params =
			(af_block_state_space_params *) block->params;

	af_state_space *system = params->state_space;
	system->input_vector = af_block_input_get_vector_at(block->input, 0);

	result = gsl_odeiv_evolve_apply(params->evolve, params->control,
									params->step, params->system,
									&time, time + step, &step,
									af_state_space_get_state_vector(system));

	gsl_vector_set_all(block->output->vector, 0);

	gsl_blas_dgemv(CblasNoTrans, 1, system->output_matrix,
				   system->state_vector, 0, block->output->vector);

	//block->output->vector = system->output_vector;

	return result;
}

af_block *af_block_state_space_alloc(const af_router * const router,
									 af_state_space *state_space) {

	af_block *block = af_block_alloc(
		state_space->input_matrix->size2,
		state_space->output_matrix->size1
	);

	af_block_set_params(
		block, af_block_state_space_params_alloc(router->step_size, state_space)
	);

	af_block_set_handler(block, af_block_state_space_function);
	af_block_set_router(block, router);

	return block;
}

void af_block_state_space_free(af_block * const block) {
	af_block_state_space_params_free
		((af_block_state_space_params *) block->params);

	af_block_free(block);
}
