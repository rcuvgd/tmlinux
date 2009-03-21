/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strrchr
#define Wstrrchr __strrchr

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strrchr, strrchr)
#else
char *strrchr(const char *s, int c)
{
	return __strrchr(s,c);
}
#endif 

#ifndef __TCS__
weak_alias(strrchr, rindex)
#else
char *rindex(const char *s, int c)
{
	return strrchr(s, c);
}
#endif 

#undef L_strrchr
