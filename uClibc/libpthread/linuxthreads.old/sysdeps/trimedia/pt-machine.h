/*
 * sysdeps/trimedia/pt-machine.h -- trimedia-specific pthread definitions
 *
 *  Copyright (C) 2007  Guo Hongruan <guo.hongruan@gulessoft.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License.  See the file COPYING.LIB in the main
 * directory of this archive for more details.
 *
 */
#ifndef _PT_MACHINE_H
#define _PT_MACHINE_H

#include <asm/page.h>
#include <ops/custom_ops.h>

#ifndef PT_EI
# define PT_EI static inline
#endif

/* Spinlock implementation; required.  */
PT_EI long int
testandset (int *spinlock)
{
#pragma TCS_atomic
	if (*spinlock)
		return 1;
	else
	{
		*spinlock=1;
		return 0;
	}
}

#define HAS_COMPARE_AND_SWAP

PT_EI int
__compare_and_swap (long int *p, long int oldval, long int newval)
{
#pragma TCS_atomic
  if((*p ^ oldval) == 0) {
	*p = newval;
	return 1;
  }
  else
	return 0;
}

/* Get some notion of the current stack.  Need not be exactly the top
   of the stack, just something somewhere in the current frame.  */
#define CURRENT_STACK_FRAME  (char*)RDREG(4)

#define THREAD_MANAGER_STACK_SIZE  (2 * PAGE_SIZE - 32)

/*
 * FIXME: I don't know how to implement it for trimedia
 */
#define MEMORY_BARRIER() 
#define READ_MEMORY_BARRIER() 
#define WRITE_MEMORY_BARRIER() 

#endif 
