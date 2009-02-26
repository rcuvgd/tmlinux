/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_stpncpy
#define WANT_WIDE
#define Wstpncpy __wcpncpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcpncpy, wcpncpy)
#else
wchar_t *wcpncpy(wchar_t *dest, const wchar_t *src, size_t n)
{
	return __wcpncpy(dest,src,n);
}
#endif 

#undef L_stpncpy
