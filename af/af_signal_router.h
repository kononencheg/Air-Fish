/**
 * Air-Fish
 *
 * @file af_signal_router.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */


#ifndef AF_SIGNAL_ROUTER_H_
#define AF_SIGNAL_ROUTER_H_


typedef struct {
	double time;
	double step_size;
} af_signal_router;


af_signal_router *af_signal_router_alloc(double step_size, double time);

void af_signal_router_free(af_signal_router *router);

#endif /* AF_SIGNAL_ROUTER_H_ */
