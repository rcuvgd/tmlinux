/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcspn
#define Wstrcspn __strcspn

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strcspn, strcspn)
#else
size_t strcspn(const char *s, const char *reject)
{
	return __strcspn(s,reject);
}
#endif 

#undef L_strcspn
