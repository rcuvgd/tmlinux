/* vi: set sw=4 ts=4: */
/*
 * fstat() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>
#define _SYS_STAT_H
#include <bits/stat.h>
#include <bits/kernel_stat.h>
#include "xstatconv.h"

#define __NR___syscall_fstat __NR_fstat
static inline _syscall2(int, __syscall_fstat, int, fd, struct kernel_stat *, buf);

int fstat(int fd, struct stat *buf)
{
	int result;
	struct kernel_stat kbuf;

	result = __syscall_fstat(fd, &kbuf);
	if (result == 0) {
		__xstat_conv(&kbuf, buf);
	}
	return result;
}

#if ! defined __NR_fstat64 && defined __UCLIBC_HAS_LFS__
#ifndef __TCS__
weak_alias(fstat, fstat64);
#else
int fstat64(int fd, struct stat *buf)
{
	return fstat(fd,buf);
}
#endif
#endif
