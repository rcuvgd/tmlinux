/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strstr
#define Wstrstr __strstr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strstr, strstr)
#else
char *strstr(const char *haystack, const char *needle)
{
	return __strstr(haystack,needle);
}
#endif 

#undef L_strstr
