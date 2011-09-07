/**
 * Air-Fish
 *
 * @file af_state_space.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "af_state_space.h"

///////////////////////////////////////////////////////////////////////////////

void populate_matrix(gsl_matrix *matrix, const double data[]) {
	size_t i, j;

	for (i = 0; i < matrix->size1; i++) {
		for (j = 0; j < matrix->size2; j++) {
			gsl_matrix_set(matrix, i, j, data[i * matrix->size2 + j]);
		}
	}
}

void populate_vector(gsl_vector *vector, const double data[]) {
	size_t i;

	for (i = 0; i < vector->size; i++) {
		gsl_vector_set(vector, i, data[i]);
	}
}


double *extract_vector(gsl_vector *vector) {
	double *data = calloc(vector->size, sizeof(double));
	size_t i;

	for (i = 0; i < vector->size; i++) {
		 data[i] = gsl_vector_get(vector, i);
	}

	return data;
}



///////////////////////////////////////////////////////////////////////////////

af_state_space *af_state_space_alloc(size_t state_dim,
									 size_t input_dim,
									 size_t output_dim) {

	af_state_space *state_space = (af_state_space *) malloc(sizeof(af_state_space));

	state_space->state_dim  = state_dim;
	state_space->input_dim  = input_dim;
	state_space->output_dim = output_dim;

	state_space->state_matrix  = gsl_matrix_alloc(state_dim, state_dim);
	state_space->input_matrix  = gsl_matrix_alloc(state_dim, input_dim);
	state_space->output_matrix = gsl_matrix_alloc(output_dim, state_dim);

	state_space->state_vector = gsl_vector_alloc(state_dim);
	state_space->input_vector = gsl_vector_alloc(input_dim);
	state_space->output_vector = gsl_vector_alloc(output_dim);

	return state_space;
}

int af_state_space_function(double time,
							const double f[],
							double dfdt[],
							void *params) {

	af_state_space *state = (af_state_space *) params;
	gsl_vector *temp = gsl_vector_alloc(state->state_dim);

	gsl_vector_set_all(temp, 0.0);
	af_state_space_set_state_vector(state, f);

	gsl_blas_dgemv(CblasNoTrans, 1.0,
				   state->input_matrix,
				   state->input_vector,
				   1.0, temp);

	gsl_blas_dgemv(CblasNoTrans, 1.0,
				  state->state_matrix,
				  state->state_vector,
				  1.0, temp);

	dfdt[0] = gsl_vector_get(temp, 0);
	dfdt[1] = gsl_vector_get(temp, 1);

	return GSL_SUCCESS;
}

void af_state_space_set_state_vector(af_state_space *state_space, const double data[]) {
	populate_vector(state_space->state_vector, data);
}

double *af_state_space_get_state_vector(af_state_space *state_space) {
	return extract_vector(state_space->state_vector);
}


void af_state_space_set_state_matrix(af_state_space *state_space, const double data[]) {
	populate_matrix(state_space->state_matrix, data);
}

void af_state_space_set_input_matrix(af_state_space *state_space, const double data[]) {
	populate_matrix(state_space->input_matrix, data);

}

void af_state_space_set_output_matrix(af_state_space *state_space, const double data[]) {
	populate_matrix(state_space->output_matrix, data);
}

void af_state_space_free(af_state_space* state_space) {
	gsl_matrix_free(state_space->state_matrix);
	gsl_matrix_free(state_space->input_matrix);
	gsl_matrix_free(state_space->output_matrix);

	gsl_vector_free(state_space->state_vector);
	gsl_vector_free(state_space->input_vector);
	gsl_vector_free(state_space->output_vector);

	free(state_space);
}


