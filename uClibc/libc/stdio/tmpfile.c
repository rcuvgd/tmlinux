/* Copyright (C) 1991, 1993, 1996, 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <features.h>
#include <stdio.h>
#include <unistd.h>
#include "../misc/internals/tempname.h"

/* This returns a new stream opened on a temporary file (generated
   by tmpnam).  The file is opened with mode "w+b" (binary read/write).
   If we couldn't generate a unique filename or the file couldn't
   be opened, NULL is returned.  */
FILE * tmpfile (void)
{
    char buf[FILENAME_MAX];
    int fd;
    FILE *f;

    if (__path_search (buf, FILENAME_MAX, NULL, "tmpf", 0))
	return NULL;
    fd = __gen_tempname (buf, __GT_FILE);
    if (fd < 0)
	return NULL;

    /* Note that this relies on the Unix semantics that
       a file is not really removed until it is closed.  */
    (void) remove (buf);

    if ((f = fdopen (fd, "w+b")) == NULL)
	__close (fd);

    return f;
}
#ifdef __UCLIBC_HAS_LFS__
#ifndef __TCS__
weak_alias(tmpfile, tmpfile64);
#else
FILE * tmpfile64 (void)
{
	return tmpfile();
}
#endif 
#endif
