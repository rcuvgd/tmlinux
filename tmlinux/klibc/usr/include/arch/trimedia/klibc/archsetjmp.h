/*
 * arch/trimedia/include/klibc/archsetjmp.h
 */

#ifndef _KLIBC_ARCHSETJMP_H
#define _KLIBC_ARCHSETJMP_H

struct __jmp_buf
{
	unsigned long rp;
	unsigned long fp;
	unsigned long sp;
	unsigned long dpc;
	unsigned long spc;
	unsigned long pcsw;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long callee_save_regs[32-9+1];
};

typedef struct __jmp_buf jmp_buf[1];

#endif				/* _KLIBC_ARCHSETJMP_H */
