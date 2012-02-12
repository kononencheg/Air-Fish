/**
 * Air-Fish
 *
 * @file af_router.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_ROUTER_H_
#define AF_ROUTER_H_

#include "af_block.h"

struct _af_block;

typedef struct _af_router {
    double time;
    double step_size;
    
    struct _af_block ** blocks;
    struct _af_block ** sinks;
    
    size_t blocks_count;
    size_t sinks_count;
} af_router;

af_router *af_router_alloc(double step_size);

size_t af_router_add_block(af_router * const router,
						   struct _af_block * const block);

void af_router_process(af_router * router, const double time);

void af_router_free(af_router *router);

#endif /* AF_ROUTER_H_ */
