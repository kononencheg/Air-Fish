/**
 * Air-Fish
 *
 * @file af_source_signals.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_SOURCE_SIGNALS_H_
#define AF_SOURCE_SIGNALS_H_

#include <gsl/gsl_vector.h>

#include "af_signal_processor.h"
#include "af_signal_router.h"

typedef struct {
	double shift;
} af_heaviside_step_signal_core;

af_heaviside_step_signal_core *af_heaviside_step_signal_core_alloc(double shift);
void af_heaviside_step_signal_core_free(af_heaviside_step_signal_core *core);

int af_heaviside_step_function(const gsl_vector *input,
							   af_signal_processor *processor);


#endif /* AF_SOURCE_SIGNALS_H_ */
