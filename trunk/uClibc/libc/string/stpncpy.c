/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_stpncpy
#define Wstpncpy __stpncpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__stpncpy, stpncpy)
#else
char *stpncpy(char *dest, const char *src, size_t n)
{
	return __stpncpy(dest,src,n);
}
#endif 

#undef L_stpncpy
