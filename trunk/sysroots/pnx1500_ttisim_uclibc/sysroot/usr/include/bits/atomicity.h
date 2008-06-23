#ifndef _ATOMICITY_H
#define _ATOMICITY_H	1

#include <inttypes.h>

static inline int
exchange_and_add (volatile uint32_t *mem, int val)
{
#pragma TCS_atomic
  int result = *mem;
  *mem += val;
  return result;
}

static inline void
atomic_add (volatile uint32_t *mem, int val)
{
#pragma TCS_atomic
  *mem += val;
}

static inline int
compare_and_swap (volatile long int *p, long int oldval, long int newval)
{
#pragma TCS_atomic
  if (*p != oldval)
    return 0;

  *p = newval;
  return 1;
}

#endif /* atomicity.h */

