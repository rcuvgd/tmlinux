/* vi: set sw=4 ts=4: */
/*
 * adjtimex() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <sys/timex.h>

#define __NR___adjtimex __NR_adjtimex
attribute_hidden _syscall1(int, __adjtimex, struct timex *, buf);

#ifndef __TCS__
strong_alias(__adjtimex, adjtimex)
#else
int adjtimex(struct timex* buf)
{
	return __adjtimex(buf);
}
#endif 
#ifndef __TCS__
weak_alias(__adjtimex, ntp_adjtime)
#else
int ntp_adjtime(struct timex* buf)
{
	return __adjtimex(buf);
}
#endif 
