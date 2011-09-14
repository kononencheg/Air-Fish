/**
 * Air-Fish
 *
 * @file af_signal_processor.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "af_signal_processor.h"

af_signal_processor *af_signal_processor_alloc(size_t output_dim) {

	af_signal_processor *signal_processor =
			(af_signal_processor *) malloc(sizeof(af_signal_processor));

	signal_processor->output_vector = gsl_vector_alloc(output_dim);
	gsl_vector_set_all(signal_processor->output_vector, 0);

	return signal_processor;
}

void af_signal_processor_set_handler(af_signal_processor *processor,
									 af_signal_handler handler) {
	processor->handler = handler;
}

void af_signal_processor_set_router(af_signal_processor *processor,
									af_signal_router *router) {
	processor->router = router;
}

void af_signal_processor_set_core(af_signal_processor *processor,
								  void *signal_core) {
	processor->signal_core = signal_core;
}


void af_signal_processor_apply(af_signal_processor *processor,
							   const gsl_vector *input) {

	processor->handler(input, processor);
}


void af_signal_processor_free(af_signal_processor *processor) {
	gsl_vector_free(processor->output_vector);

	free(processor);
}

