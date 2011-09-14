/**
 * Air-Fish
 *
 * @file af_signal_router.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>

#include "af_signal_router.h"

af_signal_router *af_signal_router_alloc(double step_size, double time) {
	af_signal_router *router =
			(af_signal_router *) malloc(sizeof(af_signal_router));

	router->step_size = step_size;
	router->time = time;

	return router;
}

void af_signal_router_free(af_signal_router *router) {
	free(router);
}


