/**
 * Air-Fish
 *
 * @file af_signal_router.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_SIGNAL_ROUTER_H_
#define AF_SIGNAL_ROUTER_H_

//#include "af_signal_processor.h"

//typedef struct _af_signal_processor af_signal_processor;

typedef struct {
	//af_signal_processor *base;

	double time;
	double step_size;
} af_signal_router;


af_signal_router *af_signal_router_alloc(double step_size, double time);

void af_signal_router_free(af_signal_router *router);

/*
void af_signal_router_set_base(af_signal_router *router, af_signal_processor *base);

void af_signal_router_process(double time);

*/

#endif /* AF_SIGNAL_ROUTER_H_ */
