/* Copyright (C) 1991,1992,1995-1997,2000,2002 Free Software Foundation, Inc.
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

#include <unistd.h>
#include <features.h>
#include <sys/param.h>
extern size_t __pagesize;

/* Return the system page size.  */
/* couldn't make __getpagesize hidden, because shm.h uses it in a macro */
int attribute_hidden __getpagesize_internal(void)
{
  if (__pagesize != 0)
    return __pagesize;

#ifdef	EXEC_PAGESIZE
  return EXEC_PAGESIZE;
#else	/* No EXEC_PAGESIZE.  */
#ifdef	NBPG
#ifndef	CLSIZE
#define	CLSIZE	1
#endif	/* No CLSIZE.  */
  return NBPG * CLSIZE;
#else	/* No NBPG.  */
  return NBPC;
#endif	/* NBPG.  */
#endif	/* EXEC_PAGESIZE.  */
}
#ifndef __TCS__
strong_alias(__getpagesize_internal, __getpagesize)
#else
int __getpagesize(void)
{
	return __getpagesize_internal();
}
#endif 
#ifndef __TCS__
weak_alias(__getpagesize_internal, getpagesize)
#else
int getpagesize(void)
{
	return __getpagesize_internal();
}
#endif 

