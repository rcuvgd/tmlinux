/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strchrnul
#define Wstrchrnul __strchrnul

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strchrnul, strchrnul)
#else
char *strchrnul(const char *s, int c)
{
	return __strchrnul(s,c);
}
#endif 

#undef L_strchrnul
