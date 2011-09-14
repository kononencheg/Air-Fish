/**
 * Air-Fish
 *
 * @file af_source_signals.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_source_signals.h"
#include "af_signal_router.h"

af_heaviside_step_signal_core *af_heaviside_step_signal_core_alloc(double shift) {
	af_heaviside_step_signal_core *core = (af_heaviside_step_signal_core *)
			malloc(sizeof(af_heaviside_step_signal_core));

	core->shift = shift;

	return core;
}

void af_heaviside_step_signal_core_free(af_heaviside_step_signal_core *core) {
	free(core);
}

int af_heaviside_step_function(const gsl_vector *input,
							   af_signal_processor *processor) {

	af_heaviside_step_signal_core *core =
			(af_heaviside_step_signal_core *) processor->signal_core;

	gsl_vector_set(
		processor->output_vector, 0,
		processor->router->time >= core->shift ? 1 : 0
	);

	return GSL_SUCCESS; //GSL_ERANGE
}




