#ifndef AF_BLOCK_MUX_H_
#define AF_BLOCK_MUX_H_

#include "af_block.h"

int af_block_mux_function(af_block * const block);

af_block *af_block_mux_alloc(const af_router * const router,
							 size_t input_size, size_t output_dim);

void af_block_mux_free(af_block * const block);

#endif /* AF_BLOCK_MUX_H_ */
