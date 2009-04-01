/* vi: set sw=4 ts=4: */
/*
 * setgroups() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#define sysconf __sysconf

#include "syscalls.h"
#include <unistd.h>
#include <grp.h>

#define __NR___syscall_setgroups __NR_setgroups
static inline _syscall2(int, __syscall_setgroups,
		size_t, size, const __kernel_gid_t *, list);

int setgroups(size_t n, const gid_t * groups)
{
	if (n > (size_t) sysconf(_SC_NGROUPS_MAX)) {
		__set_errno(EINVAL);
		return -1;
	} else {
		size_t i;
#ifndef __TCS__
		__kernel_gid_t kernel_groups[n];
#else
		__kernel_gid_t* kernel_groups = NULL;
		int ret = 0;

		if(n > 0) {
			kernel_groups = malloc(sizeof(__kernel_gid_t)*n);
			if(kernel_groups==NULL){
				__set_errno(ENOMEM);
				return -1;
			}
			memset(kernel_groups,0,sizeof(__kernel_gid_t)*n);
		}
#endif

		for (i = 0; i < n; i++) {
			kernel_groups[i] = (groups)[i];
			if (groups[i] != (gid_t) ((__kernel_gid_t) groups[i])) {
				__set_errno(EINVAL);
				return -1;
			}
		}
#ifndef __TCS__
		return (__syscall_setgroups(n, kernel_groups));
#else
		ret = __syscall_setgroups(n, kernel_groups);
		free(kernel_groups);
		return ret;
#endif 
	}
}
