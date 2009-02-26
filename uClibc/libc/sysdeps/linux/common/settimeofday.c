/* vi: set sw=4 ts=4: */
/*
 * settimeofday() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <sys/time.h>
#define __NR___settimeofday __NR_settimeofday
attribute_hidden _syscall2(int, __settimeofday, const struct timeval *, tv,
		  const struct timezone *, tz);
#ifndef __TCS__
strong_alias(__settimeofday,settimeofday)
#else
int settimeofday(const struct timeval* tv, const struct timezone* tz)
{
	return __settimeofday(tv,tz);
}
#endif 
