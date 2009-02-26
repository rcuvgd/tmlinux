/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcpy
#define WANT_WIDE
#define Wstrcpy __wcscpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcscpy, wcscpy)
#else
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src)
{
	return __wcscpy(dest,src);
}
#endif 

#undef L_strcpy
