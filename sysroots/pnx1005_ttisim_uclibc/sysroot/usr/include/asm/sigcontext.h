#ifndef _ASM_TRIMEDIA_SIGCONTEXT_H
#define _ASM_TRIMEDIA_SIGCONTEXT_H

#include <asm/ptrace.h>

struct sigcontext {
	unsigned long oldmask;
	struct pt_regs regs;
};

#endif /*_ASM_TRIMEDIA_SIGCONTEXT_H*/
