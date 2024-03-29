/* vi: set sw=4 ts=4: */
/*
 * getrlimit() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>
#include <sys/resource.h>

#ifdef __NR_ugetrlimit
#define __NR___ugetrlimit __NR_ugetrlimit
static inline
_syscall2(int, __ugetrlimit, enum __rlimit_resource, resource,
		  struct rlimit *, rlim);
int attribute_hidden __getrlimit(__rlimit_resource_t resource, struct rlimit *rlimits)
{
	return (__ugetrlimit(resource, rlimits));
}

#else							/* __NR_ugetrlimit */

/* Only include the old getrlimit if the new one (ugetrlimit) is not around */
#define __NR___syscall_getrlimit __NR_getrlimit
static inline
_syscall2(int, __syscall_getrlimit, int, resource, struct rlimit *, rlim);

int attribute_hidden __getrlimit(__rlimit_resource_t resource, struct rlimit *rlimits)
{
	int result;

	result = __syscall_getrlimit(resource, rlimits);

	if (result == -1)
		return result;

	/* We might have to correct the limits values.  Since the old values
	 * were signed the infinity value is too small.  */
	if (rlimits->rlim_cur == RLIM_INFINITY >> 1)
		rlimits->rlim_cur = RLIM_INFINITY;
	if (rlimits->rlim_max == RLIM_INFINITY >> 1)
		rlimits->rlim_max = RLIM_INFINITY;
	return result;
}
#endif

#ifndef __TCS__
strong_alias(__getrlimit,getrlimit)
#else
int getrlimit(__rlimit_resource_t resource, struct rlimit *rlimits)
{
	return __getrlimit(resource,rlimits);
}
#endif 
