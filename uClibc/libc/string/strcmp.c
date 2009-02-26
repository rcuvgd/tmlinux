/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define L_strcmp
#define Wstrcmp __strcmp

#include "wstring.c"

#ifndef __TCS__
strong_alias(__strcmp, strcmp)
#else
int strcmp(const char *s1, const char *s2)
{
	return __strcmp(s1,s2);
}
#endif 

#ifdef __LOCALE_C_ONLY
#ifndef __TCS__
weak_alias(__strcmp, __strcoll)
#else
int __strcoll(const char *s1, const char *s2)
{
	return __strcmp(s1,s2);
}
#endif 
#ifndef __TCS__
strong_alias(__strcoll, strcoll)
#else
int strcoll(const char *s1, const char *s2)
{
	return __strcoll(s1,s2);
}
#endif 
#endif

#undef L_strcmp
