/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strncmp
#define Wstrncmp __strncmp

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strncmp, strncmp)
#else
int strncmp(const char *s1, const char *s2, size_t n)
{
	return __strncmp(s1,s2,n);
}
#endif 

#undef L_strncmp
