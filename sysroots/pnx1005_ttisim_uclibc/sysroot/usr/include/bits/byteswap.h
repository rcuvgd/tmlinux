
#if !defined _BYTESWAP_H && !defined _NETINET_IN_H
# error "Never use <bits/byteswap.h> directly; include <byteswap.h> instead."
#endif

/* Swap bytes in 16 bit value.  We don't provide an assembler version
   because GCC is smart enough to generate optimal assembler output, and
   this allows for better cse.  */
#define __bswap_16(x) \
  ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

/* Swap bytes in 32 bit value.  */
#define __bswap_constant_32(x) \
  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
   (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

# define __bswap_32(x) __bswap_constant_32 (x)

#ifndef __bswap_64
static inline unsigned long long 
__bswap_64_internal(unsigned long long x)
{
	union {
		unsigned long long int __ll;
		unsigned long int __l[2];
	} __v,__r;

	__v.__ll = x;
	__r.__l[0] = __bswap_32(__v.__l[1]);
	__r.__l[1] = __bswap_32(__v.__l[0]);

	return __r.__ll;
}

#define __bswap_64(x)   __bswap_64_internal((unsigned long long)(x))
#endif 

