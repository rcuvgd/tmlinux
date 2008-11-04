#ifndef _ASM_TRIMEDIA_UNALIGNED_H
#define _ASM_TRIMEDIA_UNALIGNED_H

#include <linux/config.h>

/*
 *FIXME: TriMedia supports unaligned access memory, need to implement it.
 */
/* Use memmove here, so gcc does not insert a __builtin_memcpy. */

#define get_unaligned(ptr) \
  ({ __typeof__(*(ptr)) __tmp; memmove(&__tmp, (ptr), sizeof(*(ptr))); __tmp; })

#define put_unaligned(val, ptr)				\
  ({ __typeof__(*(ptr)) __tmp = (val);			\
     memmove((ptr), &__tmp, sizeof(*(ptr)));		\
     (void)0; })

#endif /*_ASM_TRIMEDIA_UNALIGNED_H */
