/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memchr
#define WANT_WIDE
#define Wmemchr __wmemchr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wmemchr, wmemchr)
#else
wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n)
{
	return __wmemchr(s,c,n);
}
#endif 

#undef L_memchr
