/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strpbrk
#define WANT_WIDE
#define Wstrpbrk __wcspbrk

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcspbrk, wcspbrk)
#else
wchar_t *wcspbrk(const wchar_t *wcs, const wchar_t *accept)
{
	return __wcspbrk(wcs,accept);
}
#endif 

#undef L_strpbrk
