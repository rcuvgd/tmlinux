/* brk system call for Linux/MIPS.
   Copyright (C) 2000 Free Software Foundation, Inc.
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

#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>

void *__curbrk = 0;

int brk (void *addr)
{
  void *newbrk;

  {
    register long int res __asm__ ("$2");

    asm ("move\t$4,%2\n\t"
	 "syscall"		/* Perform the system call.  */
	 : "=r" (res)
	 : "0" (__NR_brk), "r" (addr)
	 : "$1", "$3", "$4", "$7", "$8", "$9", "$10", "$11",
	   "$12", "$13", "$14", "$15", "$24", "$25", "memory");
    newbrk = (void *) res;
  }
  __curbrk = newbrk;

  if (newbrk < addr)
    {
      __set_errno (ENOMEM);
      return -1;
    }

  return 0;
}
