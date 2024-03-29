/* vi: set sw=4 ts=4: */
/*
 * close() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>

#define __NR___close __NR_close
attribute_hidden _syscall1(int, __close, int, fd);
#ifndef __TCS__
strong_alias(__close,close)
weak_alias(__close,__libc_close)
#else
extern int close(int fd)
{
	return __close(fd);
}
extern int __libc_close(int fd)
{
	return __close(fd);
}
#endif 
