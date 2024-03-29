/* vi: set sw=4 ts=4: */
/* vfork for uClibc
 *
 * Copyright (C) 2000 by Lineo, inc. and Erik Andersen
 * Copyright (C) 2000,2001 by Erik Andersen <andersen@uclibc.org>
 * Written by Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define _ERRNO_H
#include <bits/errno.h>
#include <sys/syscall.h>

#ifdef __NR_fork
.text
.global vfork
.type vfork,%function
.align 4
vfork:

#ifdef __NR_vfork
	DO_CALL (vfork)
	cmn	r0, #4096
	bxcc	lr

	/* Check if vfork even exists.  */
	ldr     r1, =-ENOSYS
	teq     r0, r1
	bne     __error
#endif

	/* If we don't have vfork, use fork.  */
	DO_CALL (fork)
	cmn     r0, #4096

	/* Syscal worked.  Return to child/parent */
	bxcc   lr

__error:
	b	__syscall_error

.size vfork,.-vfork
#endif
