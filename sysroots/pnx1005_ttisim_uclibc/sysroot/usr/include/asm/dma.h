#ifndef _ASM_TRIMEDIA_DMA_H__
#define _ASM_TRIMEDIA_DMA_H__

/*
 *FIXME: need to implement it in furture
 */
/* What should this be?  */
#define MAX_DMA_ADDRESS	0xFFFFFFFF

/* reserve a DMA channel */
extern int request_dma (unsigned int dmanr, const char * device_id);
/* release it again */
extern void free_dma (unsigned int dmanr);

#ifdef CONFIG_PCI
extern int isa_dma_bridge_buggy;
#else
#define isa_dma_bridge_buggy    (0)
#endif

#endif /* _ASM_TRIMEDIA_DMA_H__ */

