#ifndef AF_BLOCK_PRINT_H_
#define AF_BLOCK_PRINT_H_

#include "af_block.h"
#include "af_router.h"

int af_block_print_function(af_block * const block);

af_block *af_block_print_alloc(const af_router * const router);

void af_block_print_free(af_block * const block);

#endif /* AF_BLOCK_PRINT_H_ */
