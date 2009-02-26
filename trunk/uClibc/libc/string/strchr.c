/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strchr
#define Wstrchr __strchr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strchr, strchr)
#else
char *strchr(const char *s, int c)
{
	return __strchr(s,c);
}
#endif 

weak_alias(strchr, index)

#undef L_strchr
