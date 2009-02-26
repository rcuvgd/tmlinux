/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strlen
#define WANT_WIDE
#define Wstrlen __wcslen

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcslen, wcslen)
#else
size_t wcslen(const wchar_t *s)
{
	return __wcslen(s);
}
#endif 

#undef L_strlen
