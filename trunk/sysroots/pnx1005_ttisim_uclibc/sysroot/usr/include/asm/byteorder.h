#ifndef _ASM_TRIMEDIA_BYTEORDER_H
#define _ASM_TRIMEDIA_BYTEORDER_H

#include <linux/config.h>
#include <asm/types.h>

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#ifdef CONFIG_BIG_ENDIAN 
#include <linux/byteorder/big_endian.h>
#else

#ifdef CONFIG_LITTLE_ENDIAN
#include <linux/byteorder/little_endian.h>
#else
#error "Please tell me the byte order!!" 
#endif /*CONFIG_LITTLE_ENDIAN*/

#endif /*CONFIG_BIG_ENDIAN*/

#endif

