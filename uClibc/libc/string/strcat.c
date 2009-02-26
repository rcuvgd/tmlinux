/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcat
#define Wstrcat __strcat

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strcat, strcat)
#else
char *strcat(char *dest, const char *src)
{
	return __strcat(dest,src);
}
#endif 

#undef L_strcat
