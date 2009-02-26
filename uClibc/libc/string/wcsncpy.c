/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strncpy
#define WANT_WIDE
#define Wstrncpy __wcsncpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsncpy, wcsncpy)
#else
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n)
{
	return __wcsncpy(dest,src,n);
}
#endif 

#undef L_strncpy
