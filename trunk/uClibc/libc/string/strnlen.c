/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strnlen
#define Wstrnlen __strnlen

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strnlen, strnlen)
#else
size_t strnlen(const char *s, size_t maxlen)
{
	return __strnlen(s,maxlen);
}
#endif 

#undef L_strnlen
