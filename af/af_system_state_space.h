/**
 * Air-Fish
 *
 * @file af_system_state_space.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_SYSTEM_STATE_SPACE_H_
#define AF_SYSTEM_STATE_SPACE_H_

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_vector.h>

#include "af_signal_block.h"
#include "af_state_space.h"

typedef struct {
	gsl_odeiv_step *step;
	gsl_odeiv_control *control;
	gsl_odeiv_evolve *evolve;
	gsl_odeiv_system *system;

	af_state_space *state_space;
} af_system_state_space_params;

af_system_state_space_params *af_system_state_space_params_alloc
	(size_t state_dim, double step_size, af_state_space * const state_space);

void af_system_state_space_params_free(af_system_state_space_params *params);

int af_system_state_space_function(af_signal_block * const block);


#endif /* AF_SYSTEM_STATE_SPACE_H_ */
