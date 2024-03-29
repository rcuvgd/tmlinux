/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strncpy
#define Wstrncpy __strncpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strncpy, strncpy)
#else
char *strncpy(char *dest, const char *src, size_t n)
{
	return __strncpy(dest,src,n);
}
#endif 

#undef L_strncpy
