/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memchr
#define Wmemchr __memchr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__memchr, memchr)
#else
void *memchr(const void* s, int c, size_t n)
{
	return __memchr(s,c,n);
}
#endif 

#undef L_memchr
