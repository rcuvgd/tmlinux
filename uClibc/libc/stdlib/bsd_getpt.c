/* Copyright (C) 1998, 1999 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Zack Weinberg <zack@rabi.phys.columbia.edu>, 1998.

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

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


/* Prefix for master pseudo terminal nodes.  */
#define _PATH_PTY "/dev/pty"


/* Letters indicating a series of pseudo terminals.  */
#ifndef PTYNAME1
#define PTYNAME1 "pqrsPQRS"
#endif
const char _ptyname1[] = PTYNAME1;

/* Letters indicating the position within a series.  */
#ifndef PTYNAME2
#define PTYNAME2 "0123456789abcdefghijklmnopqrstuv";
#endif
const char _ptyname2[] = PTYNAME2;


/* Open a master pseudo terminal and return its file descriptor.  */
int
__getpt (void)
{
  char buf[sizeof (_PATH_PTY) + 2];
  const char *p, *q;
  char *s;

  __memcpy (buf, _PATH_PTY, sizeof (_PATH_PTY));
  s = buf + __strlen (buf);

  /* s[0] and s[1] will be filled in the loop.  */
  s[2] = '\0';

  for (p = _ptyname1; *p != '\0'; ++p)
    {
      s[0] = *p;

      for (q = _ptyname2; *q != '\0'; ++q)
	{
	  int fd;

	  s[1] = *q;

	  fd = __open (buf, O_RDWR);
	  if (fd != -1)
	    return fd;

	  if (errno == ENOENT)
	    return -1;
	}
    }

  errno = ENOENT;
  return -1;
}
