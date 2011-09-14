/**
 * Air-Fish
 *
 * @file main.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "af/af_state_space.h"
#include "af/af_signal_processor.h"
#include "af/af_source_signals.h"
#include "af/af_signal_router.h"

#define TIME_STEP (0.01)

/*
int state_space(const gsl_vector *input,
	  			gsl_vector *output,
	  			af_signal_processor *processor,
	  			af_signal_router *router) {

	int result = GSL_FAILURE;

	double time = router->time,
		   step = router->step_size;

	af_state_space_signal_core *state_space_core =
			(af_state_space_signal_core *) processor->signal_core;

	af_state_space *state = state_space_core->state_space;
	state->input_vector = input;

	result = gsl_odeiv_evolve_apply(state_space_core->evolve,
						   	   	    state_space_core->control,
						   	   	    state_space_core->step,
						   	   	    state_space_core->system,
						   	   	    &time, time + step, &step,
						   	   	    af_state_space_get_state_vector(state));

	gsl_vector_set_all(state->output_vector, 0);

	gsl_blas_dgemv(CblasNoTrans, 1,
				   state->output_matrix,
				   state->state_vector,
				   0, state->output_vector);

	output = state->output_vector;

	return result;
}*/

int main() {

	/*double u[1] = { 1 };

	const double A[] = { 1, 10,
						-1, -5 };

	const double B[] = { 1,
				   	   	 0 };

	const double C[] = { 0, 1 };

	double X0[] = { 0, 0 };

	double t = 0, t1 = 10.0;*/

	af_signal_router *router = af_signal_router_alloc(0.01, 0);
	af_signal_processor *step_signal = af_signal_processor_alloc(1);

	af_heaviside_step_signal_core *step_core =
			af_heaviside_step_signal_core_alloc(0);

	af_signal_processor_set_core(step_signal, step_core);
	af_signal_processor_set_handler(step_signal, af_heaviside_step_function);
	af_signal_processor_set_router(step_signal, router);

	af_signal_processor_apply(step_signal, NULL);

	printf("%.5f\n", gsl_vector_get(step_signal->output_vector, 0));

	/*af_state_space *state = af_state_space_alloc(2, 1, 1);
	af_state_space_signal_core *core = af_state_space_signal_core_alloc(2, 0.01, state);

	af_state_space_set_state_matrix(state, A);
	af_state_space_set_input_matrix(state, B);
	af_state_space_set_output_matrix(state, C);

	af_state_space_set_state_vector(state, X0);

	gsl_vector_view in = gsl_vector_view_array(u, 1);
	gsl_vector *out = gsl_vector_alloc(1);

	while (t < t1) {
		state_space(t, &in.vector, out, core);
		printf("%.5f %.5f\n", t, gsl_vector_get(state->output_vector, 0));
		t += 0.01;
	}

	af_state_space_free(state);
	*/

	return EXIT_SUCCESS;
}
