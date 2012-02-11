/**
 * Air-Fish
 *
 * @file af_block_feedback.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_BLOCK_FEEDBACK_H_
#define AF_BLOCK_FEEDBACK_H_

#include "af_block.h"

int af_block_feedback_function(af_block * const block);

af_block *af_block_feedback_alloc(const af_router * const router,
							  	  size_t input_size);

void af_block_feedback_free(af_block * const block);

#endif /* AF_BLOCK_FEEDBACK_H_ */
