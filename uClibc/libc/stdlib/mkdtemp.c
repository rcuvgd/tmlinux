/* vi: set sw=4 ts=4: */
/* Copyright (C) 1998 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include "../misc/internals/tempname.h"

/* Generate a unique temporary directory name from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the filename unique.
   The directory is created, mode 700, and its name is returned.
   (This function comes from OpenBSD.) */
#if 0
char * mkdtemp (char *template)
{
    if (__gen_tempname (template, __GT_DIR))
	/* We return the null string if we can't find a unique file name.  */
	template[0] = '\0';

    return template;
}
#else
/*
 * FIXME: I think the above implement is incorrect, really??.
 * */
char * mkdtemp (char *template)
{
    if (__gen_tempname (template, __GT_DIR) < 0)
		return NULL;

	return template;
}

#endif 
