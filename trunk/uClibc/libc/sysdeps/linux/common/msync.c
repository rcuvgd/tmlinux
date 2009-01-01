/* vi: set sw=4 ts=4: */
/*
 * msync() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>
#include <sys/mman.h>

#define __NR___libc_msync __NR_msync
_syscall3(int, __libc_msync, void *, addr, size_t, length, int, flags);
#ifndef __TCS__
weak_alias(__libc_msync, msync);
#else
int msync(void* addr,size_t length,int flags)
{
	return __libc_msync(addr,length,flags);
}
#endif 
