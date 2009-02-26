/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strspn
#define WANT_WIDE
#define Wstrspn __wcsspn

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsspn, wcsspn)
#else
size_t wcsspn(const wchar_t *wcs, const wchar_t *accept)
{
	return __wcsspn(wcs,accept);
}
#endif 

#undef L_strspn
