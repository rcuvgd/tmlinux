/* Copyright (C) 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2003.

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

#ifndef _BITS_ATOMIC_H
#define _BITS_ATOMIC_H	1

/* We have by default no support for atomic operations.  So define
   them non-atomic.  If this is a problem somebody will have to come
   up with real definitions.  */

/* The only basic operation needed is compare and exchange.  */
static inline unsigned long
__atomic_compare_and_exchange_val_acq(unsigned long* mem,
		unsigned long newval,unsigned long oldval)
{
#pragma TCS_atomic
	if((*mem) == oldval){
		(*mem) = newval;
	}

	return (*mem);
}

static inline int
__atomic_compare_and_exchange_bool_acq(unsigned long* mem,
		unsigned long newval,unsigned long oldval)
{
#pragma TCS_atomic
	if((*mem) == oldval){
		(*mem) = newval;
		return 0;
	}

	return 1;
}

#define atomic_compare_and_exchange_bool_acq(mem,newval,oldval)  	\
	__atomic_compare_and_exchange_bool_acq((unsigned long*)(mem),(unsigned long)(newval),(unsigned long)(oldval))
#define atomic_compare_and_exchange_val_acq(mem,newval,oldval) 	\
	__atomic_compare_and_exchange_val_acq((unsigned long*)(mem),(unsigned long)(newval),(unsigned long)(oldval))

#endif	/* bits/atomic.h */
