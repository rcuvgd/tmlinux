/* Macros to swap the order of bytes in integer values.
   Copyright (C) 1997,1998,2000,2001,2002,2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#if !defined _BYTESWAP_H && !defined _NETINET_IN_H
# error "Never use <bits/byteswap.h> directly; include <byteswap.h> instead."
#endif

#ifndef _BITS_BYTESWAP_H
#define _BITS_BYTESWAP_H 1

/* Swap bytes in 16 bit value.  */
#define __bswap_constant_16(x) \
     ((((x) >> 8) & 0xffu) | (((x) & 0xffu) << 8))

#ifdef __GNUC__
# define __bswap_16(x) \
    (__extension__							      \
     ({ unsigned short int __bsx = (x); __bswap_constant_16 (__bsx); }))
#else
static inline unsigned short int
__bswap_16 (unsigned short int __bsx)
{
  return __bswap_constant_16 (__bsx);
}
#endif

/* Swap bytes in 32 bit value.  */
#define __bswap_constant_32(x) \
     ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) |	      \
      (((x) & 0x0000ff00u) <<  8) | (((x) & 0x000000ffu) << 24))

#ifdef __GNUC__
# define __bswap_32(x) \
  (__extension__							      \
   ({ register unsigned int __bsx = (x); __bswap_constant_32 (__bsx); }))
#else
static inline unsigned int
__bswap_32 (unsigned int __bsx)
{
  return __bswap_constant_32 (__bsx);
}
#endif

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

#endif /* _BITS_BYTESWAP_H */
