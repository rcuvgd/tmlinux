/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strchr
#define WANT_WIDE
#define Wstrchr __wcschr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcschr, wcschr)
#else
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc)
{
	return __wcschr(wcs,wc);
}
#endif 

#undef L_strchr
