/* vi: set sw=4 ts=4: */
/*
 * read() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>

#define __NR___read __NR_read
attribute_hidden _syscall3(ssize_t, __read, int, fd, __ptr_t, buf, size_t, count);
#ifndef __TCS__
strong_alias(__read,read)
weak_alias(__read,__libc_read)
#else
ssize_t read(int fd,__ptr_t buf,size_t count)
{
	return __read(fd,buf,count);
}
ssize_t __libc_read(int fd,__ptr_t buf,size_t count)
{
	return __read(fd,buf,count);
}
#endif 
