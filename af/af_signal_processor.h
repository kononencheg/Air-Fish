/**
 * Air-Fish
 *
 * @file af_signal_processor.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_SIGNAL_PROCESSOR_H_
#define AF_SIGNAL_PROCESSOR_H_

#include <gsl/gsl_vector.h>

#include "af_signal_router.h"

typedef struct _af_signal_processor af_signal_processor;
typedef struct _af_signal_processor_subject af_signal_processor_subject;
typedef struct _af_signal_processor_source af_signal_processor_source;

typedef int (* af_signal_handler) (const gsl_vector *input,
								   af_signal_processor *processor);

typedef struct _af_signal_processor {
	af_signal_handler handler;

	void *signal_core;

	gsl_vector *output_vector;

	af_signal_router *router;

	//af_signal_processor_subject **subjects;
	//af_signal_processor_source **sources;

} af_signal_processor;

af_signal_processor *af_signal_processor_alloc(size_t output_dim);

void af_signal_processor_set_core(af_signal_processor *signal_processor, void *signal_core);
void af_signal_processor_set_handler(af_signal_processor *signal_processor, af_signal_handler handler);
void af_signal_processor_set_router(af_signal_processor *signal_processor, af_signal_router *router);

void af_signal_processor_apply(af_signal_processor *processor,
							   const gsl_vector *input);

void af_signal_processor_free(af_signal_processor *signal_processor);

/*
void af_signal_processor_add_source(af_signal_processor *signal_processor,
									af_signal_processor *source,
									size_t fetch_index,
									size_t fetch_length);
*/
/*
typedef struct _af_signal_processor_source {
	size_t fetch_index;
	size_t fetch_length;

	af_signal_processor *source;
} af_signal_processor_source;

typedef struct _af_signal_processor_subject{
	af_signal_processor *source;
} af_signal_processor_subject;
*/


#endif /* AF_SIGNAL_PROCESSOR_H_ */
