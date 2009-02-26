/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memset
#define WANT_WIDE
#define Wmemset __wmemset

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wmemset, wmemset)
#else
wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n)
{
	return __wmemset(wcs,wc,n);
}
#endif 

#undef L_memset
