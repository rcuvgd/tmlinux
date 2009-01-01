/* vi: set sw=4 ts=4: */
/*
 * execve() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>
#include <string.h>
#include <sys/param.h>

#define __NR___syscall_execve __NR_execve
static inline _syscall3(int, __syscall_execve, const char *, filename,
		  char *const *, argv, char *const *, envp);

int attribute_hidden __execve(const char * filename, char *const * argv, char *const * envp)
{
	return __syscall_execve(filename, argv, envp);
}
#ifndef __TCS__
strong_alias(__execve,execve)
#else
int attribute_hidden execve(const char * filename, char *const * argv, char *const * envp)
{
	return __execve(filename,argv,envp);
}
#endif
