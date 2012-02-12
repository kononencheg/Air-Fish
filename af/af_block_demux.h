#ifndef AF_BLOCK_DEMUX_H_
#define AF_BLOCK_DEMUX_H_

#include "af_block.h"

int af_block_demux_function(af_block * const block);

af_block *af_block_demux_alloc(const af_router * const router, size_t shift,
							   size_t length);

void af_block_demux_free(af_block * const block);

#endif /* AF_BLOCK_DEMUX_H_ */
