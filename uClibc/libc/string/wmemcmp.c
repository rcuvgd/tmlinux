/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memcmp
#define WANT_WIDE
#define Wmemcmp __wmemcmp

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wmemcmp, wmemcmp)
#else
int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	return __wmemcmp(s1,s2,n);
}
#endif 

#undef L_memcmp
