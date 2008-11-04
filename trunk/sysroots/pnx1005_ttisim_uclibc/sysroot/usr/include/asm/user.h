#ifndef _ASM_TRIMEDIA_USER_H__
#define _ASM_TRIMEDIA_USER_H__

#ifdef __KERNEL__

#include <linux/ptrace.h>

/*
 * FIXME: used in coredump, so maybe it is not useful for trimedia
 */
struct user {
	char		u_comm[32];		/* user command name */
};


#endif /* __KERNEL__ */

#endif /* _ASM_TRIMEDIA_USER_H__ */

