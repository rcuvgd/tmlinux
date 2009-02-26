/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strncat
#define Wstrncat __strncat

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strncat, strncat)
#else
char *strncat(char *dest, const char *src, size_t n)
{
	return __strncat(dest,src,n);
}
#endif 

#undef L_strncat
