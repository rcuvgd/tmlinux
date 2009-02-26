/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcmp
#define WANT_WIDE
#define Wstrcmp __wcscmp

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcscmp, wcscmp)
#else
int wcscmp(const wchar_t *s1, const wchar_t *s2)
{
	return __wcscmp(s1,s2);
}
#endif 

#ifdef __LOCALE_C_ONLY
#ifndef __TCS__
weak_alias(__wcscmp, __wcscoll)
#else
int __wcscoll(const wchar_t *s1, const wchar_t *s2)
{
	return __wcscmp(s1,s2);
}
#endif 
#ifndef __TCS__
strong_alias(__wcscoll, wcscoll)
#else
int wcscoll(const wchar_t *s1, const wchar_t *s2)
{
	return __wcscoll(s1,s2);
}
#endif 
#endif

#undef L_strcmp
