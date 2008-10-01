/*
 * sysdeps/trimedia/pt-machine.h -- trimedia-specific pthread definitions
 *
 *  Copyright (C) 2002  NEC Electronics Corporation
 *  Copyright (C) 2002  Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License.  See the file COPYING.LIB in the main
 * directory of this archive for more details.
 *
 */

#include <ops/custom_ops.h>

#ifndef PT_EI
# define PT_EI extern inline
#endif

/* Get some notion of the current stack.  Need not be exactly the top
   of the stack, just something somewhere in the current frame.  */
#define CURRENT_STACK_FRAME  (char*)RDREG(4)

#undef HAS_COMPARE_AND_SWAP

/*
 * FIXME: I don't know how to implement it for trimedia
 */
#define MEMORY_BARRIER() 
#define READ_MEMORY_BARRIER() 
#define WRITE_MEMORY_BARRIER() 
