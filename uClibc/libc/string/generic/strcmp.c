/* Copyright (C) 1991, 1996, 1997, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <string.h>
#include <locale.h>

#include "memcopy.h"

#undef strcmp

/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int attribute_hidden __strcmp (const char *p1, const char *p2)
{
  register const unsigned char *s1 = (const unsigned char *) p1;
  register const unsigned char *s2 = (const unsigned char *) p2;
  unsigned reg_char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
	return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}

#ifndef __TCS__
strong_alias(__strcmp, strcmp)
#else
int strcmp(const char *p1, const char *p2)
{
	return __strcmp(p1,p2);
}
#endif 

#ifdef __LOCALE_C_ONLY
#ifndef __TCS__
weak_alias(__strcmp, __strcoll)
#else
int __strcoll(const char *p1, const char *p2)
{
	return __strcmp(p1,p2);
}
#endif 
#ifndef __TCS__
strong_alias(__strcoll, strcoll)
#else
int strcoll(const char *p1, const char *p2)
{
	return __strcoll(p1,p2);
}
#endif 
#endif /* __LOCALE_C_ONLY */
