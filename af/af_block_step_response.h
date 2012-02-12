
#ifndef AF_BLOCK_RESPONSE_STEP_H_
#define AF_BLOCK_RESPONSE_STEP_H_

#include "af_block.h"
#include "af_router.h"

#include "af_step_response.h"

int af_block_step_response_function(af_block * const block);

af_block *af_block_step_response_alloc(const af_router * const router,
									   af_step_response * const response);

void af_block_step_response_free(af_block * const block);


#endif /* AF_BLOCK_RESPONSE_STEP_H_ */
