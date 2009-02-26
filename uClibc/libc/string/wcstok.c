/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strtok_r
#define WANT_WIDE
#define Wstrtok_r __wcstok

#undef Wstrspn
#define Wstrspn __wcsspn
#undef Wstrpbrk
#define Wstrpbrk __wcspbrk

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcstok, wcstok)
#else
wchar_t *wcstok(wchar_t *wcs, const wchar_t *delim, wchar_t **ptr)
{
	return __wcstok(wcs,delim,ptr);
}
#endif 

#undef L_strtok_r
