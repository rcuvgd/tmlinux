/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_stpcpy
#define WANT_WIDE
#define Wstpcpy __wcpcpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcpcpy, wcpcpy)
#else
wchar_t *wcpcpy(wchar_t *dest, const wchar_t *src)
{
	return __wcpcpy(dest,src);
}
#endif 

#undef L_stpcpy
