/* vi: set sw=4 ts=4: */
/*
 * kill() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <signal.h>

#undef kill
#define __NR___syscall_kill __NR_kill
static inline _syscall2(int, __syscall_kill, __kernel_pid_t, pid, int, sig);

int attribute_hidden __kill(pid_t pid, int sig)
{
	return (__syscall_kill(pid, sig));
}
#ifndef __TCS__
strong_alias(__kill,kill)
#else
int attribute_hidden kill(pid_t pid, int sig)
{
	return __kill(pid,sig);
}
#endif 
