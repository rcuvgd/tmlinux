/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memcmp
#define Wmemcmp __memcmp

#include "wstring.c"

#ifndef __TCS__
strong_alias(__memcmp, memcmp)
#else
int memcmp(const void *s1, const void *s2, size_t n)
{
	return __memcmp(s1,s2,n);
}
#endif 

weak_alias(memcmp, bcmp)

#undef L_memcmp
