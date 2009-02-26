/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strnlen
#define WANT_WIDE
#define Wstrnlen __wcsnlen

#include "wstring.c"

#ifndef __TCS__
strong_alias(__wcsnlen, wcsnlen)
#else
size_t wcsnlen(const wchar_t *s, size_t maxlen)
{
	return __wcsnlen(s,maxlen);
}
#endif 

#undef L_strnlen
