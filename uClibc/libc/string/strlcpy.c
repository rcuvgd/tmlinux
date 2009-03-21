/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strlcpy
#define Wstrlcpy __strlcpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strlcpy, strlcpy)
#else
size_t strlcpy(char *dst,const char *src, size_t n)
{
	return __strlcpy(dst,src,n);
}
#endif 

#ifdef __LOCALE_C_ONLY
#ifndef __TCS__
weak_alias(strlcpy, strxfrm)
#else
size_t strxfrm(char *dst,const char *src, size_t n)
{
	return strlcpy(dst, src, n);
}
#endif 
#endif

#undef L_strlcpy
