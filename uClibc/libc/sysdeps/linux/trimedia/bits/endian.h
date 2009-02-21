
#ifndef _ENDIAN_H
# error "Never use <bits/endian.h> directly; include <endian.h> instead."
#endif

#if defined(__LITTLE_ENDIAN__)
#define __BYTE_ORDER __LITTLE_ENDIAN
#elif defined(__BIG_ENDIAN__)
#define __BYTE_ORDER __BIG_ENDIAN
#else
#error Can not figure out the endianess of your machine
#endif

