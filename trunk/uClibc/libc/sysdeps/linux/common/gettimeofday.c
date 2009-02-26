/* vi: set sw=4 ts=4: */
/*
 * gettimeofday() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <sys/time.h>
#undef gettimeofday
#define __NR___gettimeofday __NR_gettimeofday
attribute_hidden _syscall2(int, __gettimeofday, struct timeval *, tv, struct timezone *, tz);
#ifndef __TCS__
strong_alias(__gettimeofday,gettimeofday)
#else
int gettimeofday(struct timeval* tv,struct timezone* tz)
{
	return __gettimeofday(tv,tz);
}
#endif 
