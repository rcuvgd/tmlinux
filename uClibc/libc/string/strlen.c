/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strlen
#define Wstrlen __strlen

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strlen, strlen)
#else
size_t strlen(const char *s)
{
	return __strlen(s);
}
#endif 

#undef L_strlen
