/**
 * Air-Fish
 *
 * @file af_state_space.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef STATE_SPACE_H_
#define STATE_SPACE_H_

#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

typedef struct {
	const gsl_vector *input_vector;
	gsl_vector *state_vector;
	gsl_vector *output_vector;

	gsl_matrix *input_matrix;
	gsl_matrix *state_matrix;
	gsl_matrix *output_matrix;

	size_t state_dim;
	size_t input_dim;
	size_t output_dim;

} af_state_space;

af_state_space *af_state_space_alloc(size_t state_dim,
									 size_t input_dim,
									 size_t output_dim);

int af_state_space_function(double time, const double f[],
							double dfdt[], void *params);

void af_state_space_set_state_vector(af_state_space * const state_space,
									 const double data[]);

double *af_state_space_get_state_vector(const af_state_space * const state_space);

void af_state_space_set_state_matrix(af_state_space * const state_space,
									 const double data[]);

void af_state_space_set_input_matrix(af_state_space * const state_space,
									 const double data[]);

void af_state_space_set_output_matrix(af_state_space * const state_space,
									  const double data[]);

void af_state_space_free(af_state_space* state_space);


#endif /* STATE_SPACE_H_ */
