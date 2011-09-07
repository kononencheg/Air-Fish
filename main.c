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
#include "af/af_signal_router.h"

int step_source(double t, const gsl_vector *input,
			  	gsl_vector *output, af_signal_processor *processor) {

	double treshold = *(double *) processor->signal_core;

	gsl_vector_set(output, 0, t >= treshold ? 1 : 0);

	return GSL_SUCCESS; //GSL_ERANGE
}

int state_space(double t,
				const gsl_vector *input,
	  			gsl_vector *output,
	  			af_signal_processor *processor,
	  			af_signal_router *router) {

	double time = t, step = router->step_size;

	af_state_space_signal_core *state_space_core =
			(af_state_space_signal_core *) processor->signal_core;

	af_state_space *state = state_space_core->state_space;

	gsl_odeiv_evolve_apply(state_space_core->evolve,
						   state_space_core->control,
						   state_space_core->step,
						   state_space_core->system,
						   &time, time + step, &step,
						   af_state_space_get_state_vector(state));

	gsl_vector_set_all(state->output_vector, 0.0);

	gsl_blas_dgemv(CblasNoTrans, 1.0,
				   state->output_matrix,
				   state->state_vector,
				   1.0, state->output_vector);

	output = state->output_vector;

	return GSL_SUCCESS;
}

int main() {

	const double A[] = { 1, 10,
						-1, -5 };

	const double B[] = { 1,
				   	   	 0 };

	const double C[] = { 0, 1 };

	double X0[] = { 0, 0 };

	double h = 1e-6;
	double t = 0, t1 = 10.0;

	af_state_space *state = af_state_space_alloc(2, 1, 1);

	gsl_odeiv_step *step = gsl_odeiv_step_alloc(gsl_odeiv_step_rk8pd, 2);
	gsl_odeiv_control *control = gsl_odeiv_control_y_new(h, 0.0);
	gsl_odeiv_evolve *evolve = gsl_odeiv_evolve_alloc(2);

	gsl_odeiv_system sys = { af_state_space_function, NULL, 2, state };

	af_state_space_set_state_matrix(state, A);
	af_state_space_set_input_matrix(state, B);
	af_state_space_set_output_matrix(state, C);

	af_state_space_set_state_vector(state, X0);

	printf("%f x %f\n", gsl_matrix_get(state->output_matrix, 0, 0), gsl_matrix_get(state->output_matrix, 0, 1));


	while (t < t1) {
		gsl_vector_set(state->input_vector, 0, 1);

		h = 0.01;
		int status = gsl_odeiv_evolve_apply(evolve, control, step, &sys, &t, t+h, &h, af_state_space_get_state_vector(state));

		if (status != GSL_SUCCESS) {
			break;
		}

		gsl_vector_set_all(state->output_vector, 0.0);


		gsl_blas_dgemv(CblasNoTrans, 1.0,
					   state->output_matrix,
					   state->state_vector,
					   1.0, state->output_vector);

		printf("%.5f %.5f %.5f\n", t, h,
				gsl_vector_get(state->output_vector, 0));
	}

	gsl_odeiv_evolve_free(evolve);
	gsl_odeiv_control_free(control);
	gsl_odeiv_step_free(step);

	af_state_space_free(state);

	return EXIT_SUCCESS;
}

