/* vi: set sw=4 ts=4: */
/* syscall for arm/uClibc
 *
 * Copyright (C) 2002 by Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <features.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>


long syscall(long sysnum, long a, long b, long c, long d, long e, long f)
{
#if !defined(__thumb__)
	register long _r0 asm("r0")=(long)(sysnum);
	register long _r6 asm("r6")=(long)(f);
	register long _r5 asm("r5")=(long)(e);
	register long _r4 asm("r4")=(long)(d);
	register long _r3 asm("r3")=(long)(c);
	register long _r2 asm("r2")=(long)(b);
	register long _r1 asm("r1")=(long)(a);
	asm volatile(
			"swi %1"
			: "=r"(_r0)
			: "i"(__NR_syscall), "r"(_r0), "r"(_r1),
			"r"(_r2), "r"(_r3), "r"(_r4), "r"(_r5),
			"r"(_r6)
			: "memory");
#else
	register long _r7 asm("r7")=(long)(sysnum);
	register long _r5 asm("r5")=(long)(f);
	register long _r4 asm("r4")=(long)(e);
	register long _r3 asm("r3")=(long)(d);
	register long _r2 asm("r2")=(long)(c);
	register long _r1 asm("r1")=(long)(b);
	register long _r0 asm("r0")=(long)(a);
	asm volatile(
			"swi 0"
			: "=r"(_r0)
			: "r"(_r0), "r"(_r1), "r"(_r2), "r"(_r3),
			"r"(_r4), "r"(_r5), "r"(_r7)
			: "memory");
#endif
	if(_r0 >=(unsigned long) -4095) {
		long err = _r0;
		(*__errno_location())=(-err);
		_r0=(unsigned long) -1;
	}
	return (long) _r0;
}
