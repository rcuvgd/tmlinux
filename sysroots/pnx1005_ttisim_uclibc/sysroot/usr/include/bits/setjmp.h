#ifndef _BITS_SETJMP_H
#define _BITS_SETJMP_H	1

#if !defined _SETJMP_H && !defined _PTHREAD_H
# error "Never include <bits/setjmp.h> directly; use <setjmp.h> instead."
#endif

#ifndef	_ASM

typedef struct
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

} __jmp_buf[1];

#endif /* _ASM */

/* Test if longjmp to JMPBUF would unwind the frame
   containing a local variable at ADDRESS.  */
#define _JMPBUF_UNWINDS(jmpbuf, address) \
  ((void *) (address) < (void *) (jmpbuf)->rp)

#endif	/* bits/setjmp.h */
