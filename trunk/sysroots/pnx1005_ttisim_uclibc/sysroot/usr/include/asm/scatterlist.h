
#ifndef _ASM_TRIMEDIA_SCATTERLIST_H__
#define _ASM_TRIMEDIA_SCATTERLIST_H__

#include <asm/types.h>
#include <asm/page.h>

struct scatterlist {
	struct page	*page;
	unsigned	offset;
	dma_addr_t	dma_address;
	unsigned	length;
};

#define ISA_DMA_THRESHOLD	(~0UL)

#endif /* _ASM_TRIMEDIA_SCATTERLIST_H__ */

