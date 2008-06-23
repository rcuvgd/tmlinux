#ifndef _ASM_TRIMEDIA_CACHE_H
#define _ASM_TRIMEDIA_CACHE_H

#include <sys/callconv.h>

#define  L1_CACHE_BYTES  TCS_CACHE_BLOCK_SIZE	

/*
 * FIXME: I don't know how to define them, this will cause poor performance.
 */
#define __cacheline_aligned
#define ____cacheline_aligned

#endif /*_ASM_TRIMEDIA_CACHE_H*/
