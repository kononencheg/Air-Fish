/**
 * Air-Fish
 *
 * @file af_signal_router.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>

#include "af_signal_router.h"
#include "af_signal_block.h"

void process_blocks(af_signal_block ** blocks, const size_t count);
void reset_blocks(af_signal_block ** blocks, const size_t count);

af_signal_router *af_signal_router_alloc(double step_size, double time) {
	af_signal_router *router =
			(af_signal_router *) malloc(sizeof(af_signal_router));

	router->step_size = step_size;
	router->time = time;

    router->blocks = NULL;
    router->blocks_count = 0;

    router->sinks = NULL;
    router->sinks_count = 0;

	return router;
}

void af_signal_router_free(af_signal_router *router) {
	free(router);
}

size_t af_signal_router_add_block(af_signal_router * const router,
                                  af_signal_block * const block) {

    size_t block_index = router->blocks_count;
    size_t sink_index = router->sinks_count;

    router->blocks_count++;
    router->blocks = (af_signal_block **) realloc
            (router->blocks, router->blocks_count * sizeof(af_signal_block **));

    router->blocks[block_index] = block;

    if (block->output_vector == NULL) {
        router->sinks_count++;
        router->sinks = (af_signal_block **) realloc
			 (router->sinks, router->sinks_count * sizeof(af_signal_block **));

        router->sinks[sink_index] = block;
    }

    return block_index;
}

void af_signal_router_process(af_signal_router * const router,
		  	  	  	  	  	  const double time) {
	if (router->sinks_count == 0) {
		fprintf(stderr, "There is no sinks!");
	} else {
		while (router->time < time) {
			process_blocks(router->sinks, router->sinks_count);
			reset_blocks(router->blocks, router->blocks_count);

			router->time += router->step_size;
		}
	}
}

void process_blocks(af_signal_block ** blocks, const size_t count) {
	af_signal_block *block = NULL;

	size_t i = 0;
	while (i < count) {
		block = blocks[i];

		if (af_signal_block_is_clear(block)) {
			process_blocks(block->input->sources, block->input->size);
			af_signal_block_process(block);
		}

		i++;
	}
}

void reset_blocks(af_signal_block ** blocks, const size_t count) {
	size_t i = 0;
	while (i < count) {
		af_signal_block_reset(blocks[i]);
		i++;
	}
}
