/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strncat
#define WANT_WIDE
#define Wstrncat __wcsncat

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsncat, wcsncat)
#else
wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n)
{
	return __wcsncat(dest,src,n);
}
#endif 

#undef L_strncat
