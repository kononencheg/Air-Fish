/**
 * Air-Fish
 *
 * @file af_signal_router.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_SIGNAL_ROUTER_H_
#define AF_SIGNAL_ROUTER_H_

#include "af_signal_block.h"

struct _af_signal_block;

typedef struct _af_signal_router {
    double time;
    double step_size;
    
    struct _af_signal_block ** blocks;
    struct _af_signal_block ** sinks;
    
    size_t blocks_count;
    size_t sinks_count;
} af_signal_router;

af_signal_router *af_signal_router_alloc(double step_size, double time);

size_t af_signal_router_add_block(af_signal_router * const router,
                                  struct _af_signal_block * const block);

void af_signal_router_process(af_signal_router * const router,
							  const double time);

void af_signal_router_free(af_signal_router *router);

#endif /* AF_SIGNAL_ROUTER_H_ */
