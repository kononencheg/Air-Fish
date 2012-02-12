
#ifndef AF_BLOCK_GAIN_H_
#define AF_BLOCK_GAIN_H_

#include "af_block.h"

int af_block_gain_function(af_block * const block);

af_block *af_block_gain_alloc(const af_router * const router,
							  size_t input_dim, const double *gain);

af_block *af_block_inverse_alloc(const af_router * const router,
								 size_t input_dim);

void af_block_gain_free(af_block * const block);

#endif /* AF_BLOCK_GAIN_H_ */
