/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strdup
#define Wstrdup __strdup

#undef Wstrlen
#undef Wstrcpy
#define Wstrlen __strlen
#define Wstrcpy __strcpy

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strdup, strdup)
#else
extern char *strdup (__const char *__s) 
{
	return __strdup(__s);
}
#endif 

#undef L_strdup
