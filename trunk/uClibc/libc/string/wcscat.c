/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcat
#define WANT_WIDE
#define Wstrcat __wcscat

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcscat, wcscat)
#else
wchar_t *wcscat(wchar_t *dest, const wchar_t *src)
{
	return __wcscat(dest,src);
}
#endif 

#undef L_strcat
