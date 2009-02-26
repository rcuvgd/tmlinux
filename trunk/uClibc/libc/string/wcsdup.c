/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strdup
#define WANT_WIDE
#define Wstrdup __wcsdup

#undef Wstrlen
#undef Wstrcpy
#define Wstrlen __wcslen
#define Wstrcpy __wcscpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsdup, wcsdup)
#else
wchar_t *wcsdup(const wchar_t *s)
{
	return __wcsdup(s);
}
#endif 

#undef L_strdup
