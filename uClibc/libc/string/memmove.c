/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_memmove
#define Wmemmove __memmove

#include "wstring.c"

#ifndef __TCS__
strong_alias(__memmove, memmove)
#else
void *memmove(void *dest, const void *src, size_t n)
{
	return __memmove(dest,src,n);
}
#endif 

#undef L_memmove
