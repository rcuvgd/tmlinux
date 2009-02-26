/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memmove
#define WANT_WIDE
#define Wmemmove __wmemmove

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wmemmove, wmemmove)
#else
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n)
{
	return __wmemmove(dest,src,n);
}
#endif 

#undef L_memmove
