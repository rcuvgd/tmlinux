/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcspn
#define WANT_WIDE
#define Wstrcspn __wcscspn

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcscspn, wcscspn)
#else
size_t wcscspn(const wchar_t *wcs, const wchar_t *reject)
{
	return __wcscspn(wcs,reject);
}
#endif 

#undef L_strcspn
