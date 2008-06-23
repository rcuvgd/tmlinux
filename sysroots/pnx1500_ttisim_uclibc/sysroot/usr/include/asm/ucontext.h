#ifndef _ASM_TRIMEDIA_UCONTEXT_H__
#define _ASM_TRIMEDIA_UCONTEXT_H__


/*
 *FIXME: Implement it according TriMedia
 */
#include <asm/sigcontext.h>

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
};

#endif /* _ASM_TRIMEDIA_UCONTEXT_H__ */
