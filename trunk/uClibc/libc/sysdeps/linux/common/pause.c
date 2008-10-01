/* vi: set sw=4 ts=4: */
/*
 * pause() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>

#ifdef __NR_pause

#define __NR___libc_pause __NR_pause
_syscall0(int, __libc_pause);
#ifndef __TCS__
weak_alias(__libc_pause, pause);
#else
int pause(void)
{
	return __libc_pause();
}
#endif 

#else

#include <signal.h>
int __libc_pause(void)
{
	return (__sigpause(sigblock(0), 0));
}
#ifndef __TCS__
weak_alias(__libc_pause, pause);
#else
int pause(void)
{
	return __libc_pause();
}
#endif 

#endif
