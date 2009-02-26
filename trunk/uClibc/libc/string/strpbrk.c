/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strpbrk
#define Wstrpbrk __strpbrk

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strpbrk, strpbrk)
#else
char *strpbrk(const char *s, const char *accept)
{
	return __strpbrk(s,accept);
}
#endif 

#undef L_strpbrk
