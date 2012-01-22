/**
 * Air-Fish
 *
 * @file af_system_state_space.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_system_state_space.h"

#include "af_signal_block.h"

#include <gsl/gsl_blas.h>

af_system_state_space_params *af_system_state_space_params_alloc
	(size_t state_dim, double step_size, af_state_space *state_space) {

	af_system_state_space_params *params = (af_system_state_space_params *)
			malloc(sizeof(af_system_state_space_params));

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

void af_system_state_space_params_free(af_system_state_space_params *params) {

}

int af_system_state_space_function(af_signal_block *block) {
	int result = GSL_FAILURE;

	double time = block->router->time, // TODO: Try 0!
		   step = block->router->step_size;

	af_system_state_space_params *params =
			(af_system_state_space_params *) block->params;

	af_state_space *system = params->state_space;
	system->input_vector = af_signal_block_input_get_vector_at(block->input, 0);

	result = gsl_odeiv_evolve_apply(params->evolve, params->control,
									params->step, params->system,
									&time, time + step, &step,
									af_state_space_get_state_vector(system));

	gsl_vector_set_all(system->output_vector, 0);

	gsl_blas_dgemv(CblasNoTrans, 1, system->output_matrix,
				   system->state_vector, 0, system->output_vector);

	block->output_vector = system->output_vector;

	return result;
}


