/* vi: set sw=4 ts=4: */
/*
 * modify_ldt() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"

#ifdef __NR_modify_ldt
_syscall3(int, modify_ldt, int, func, void *, ptr, unsigned long, bytecount);
#ifndef __TCS__
weak_alias(modify_ldt, __modify_ldt);
#else
int __modify_ldt(int func,void* ptr,unsigned long bytecount)
{
	return modify_ldt(func,ptr,bytecount);
}
#endif
#endif
