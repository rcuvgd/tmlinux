#ifndef _ASM_TRIMEDIA_MMU_H
#define _ASM_TRIMEDIA_MMU_H

/* Copyright (C) 2002, David McCullough <davidm@snapgear.com> */

typedef struct {
	struct vm_list_struct	*vmlist;
	unsigned long		end_brk;
} mm_context_t;

#endif /*_ASM_TRIMEDIA_MMU_H*/
