/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strchrnul
#define WANT_WIDE
#define Wstrchrnul __wcschrnul

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcschrnul, wcschrnul)
#else
wchar_t *wcschrnul(const wchar_t *s, wchar_t c)
{
	return __wcschrnul(s,c);
}
#endif 

#undef L_strchrnul
