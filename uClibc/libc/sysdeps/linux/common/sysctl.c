/* vi: set sw=4 ts=4: */
/*
 * _sysctl() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
struct __sysctl_args {
	int *name;
	int nlen;
	void *oldval;
	size_t *oldlenp;
	void *newval;
	size_t newlen;
	unsigned long __unused[4];
};

static inline
_syscall1(int, _sysctl, struct __sysctl_args *, args);

int sysctl(int *name, int nlen, void *oldval, size_t * oldlenp,
		   void *newval, size_t newlen)
{
#ifndef __TCS__
	struct __sysctl_args args = {
	  name:name,
	  nlen:nlen,
	  oldval:oldval,
	  oldlenp:oldlenp,
	  newval:newval,
	  newlen:newlen
	};
#else
	struct __sysctl_args args = { 0 };

	args.name=name;
	args.nlen=nlen;
	args.oldval=oldval;
	args.oldlenp=oldlenp;
	args.newval=newval;
	args.newlen=newlen;
#endif

	return _sysctl(&args);
}
