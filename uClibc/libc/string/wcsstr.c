/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strstr
#define WANT_WIDE
#define Wstrstr __wcsstr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsstr, wcsstr)
#else
wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle)
{
	return __wcsstr(haystack,needle);
}
#endif 

#ifndef __TCS__
weak_alias(wcsstr, wcswcs)
#else
wchar_t *wcswcs(const wchar_t *haystack, const wchar_t *needle)
{
	return wcsstr(haystack, needle);
}
#endif 

#undef L_strstr
