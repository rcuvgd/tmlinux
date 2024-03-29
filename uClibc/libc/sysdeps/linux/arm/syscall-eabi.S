/* Copyright (C) 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sys/syscall.h>

/* In the EABI syscall interface, we don't need a special syscall to
   implement syscall().  It won't work reliably with 64-bit arguments
   (but that is true on many modern platforms).  */

.text
.global syscall
.type syscall,%function
.align 4
syscall:
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7}
	mov	r7, r0
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	ldmfd	ip, {r3, r4, r5, r6}
	swi	0x0
	ldmfd	sp!, {r4, r5, r6, r7}
	cmn	r0, #4096
	bxcc	lr
	b	__syscall_error

.size syscall,.-syscall
