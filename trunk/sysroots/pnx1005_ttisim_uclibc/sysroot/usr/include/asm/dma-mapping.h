#ifndef _ASM_TRIMEDIA_DMA_MAPPING_H__
#define _ASM_TRIMEDIA_DMA_MAPPING_H__

#include <linux/config.h>

/*
 *FIXME: need to implement it in furture.
 */
#ifdef CONFIG_PCI
#include <asm-generic/dma-mapping.h>
#else
#include <asm-generic/dma-mapping-broken.h>
#endif

#endif /* _ASM_TRIMEDIA_DMA_MAPPING_H__ */

