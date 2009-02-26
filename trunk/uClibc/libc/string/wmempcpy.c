/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_mempcpy
#define WANT_WIDE
#define Wmempcpy __wmempcpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wmempcpy, wmempcpy)
#else
wchar_t *wmempcpy(wchar_t *dest, const wchar_t *src, size_t n)
{
	return __wmempcpy(dest,src,n);
}
#endif 

#undef L_mempcpy
