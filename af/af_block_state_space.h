/**
 * Air-Fish
 *
 * @file af_block_state_space.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_BLOCK_STATE_SPACE_H_
#define AF_BLOCK_STATE_SPACE_H_

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_vector.h>

#include "af_state_space.h"
#include "af_block.h"

typedef struct {
	gsl_odeiv_step *step;
	gsl_odeiv_control *control;
	gsl_odeiv_evolve *evolve;
	gsl_odeiv_system *system;

	af_state_space *state_space;
} af_block_state_space_params;

af_block_state_space_params *af_block_state_space_params_alloc
	(double step_size, af_state_space * const state_space);

void af_block_state_space_params_free(af_block_state_space_params *params);

int af_block_state_space_function(af_block * const block);

af_block *af_block_state_space_alloc(const af_router * const router,
									 af_state_space *state_space);

void af_block_state_space_free(af_block * const block);

#endif /* AF_BLOCK_STATE_SPACE_H_ */
