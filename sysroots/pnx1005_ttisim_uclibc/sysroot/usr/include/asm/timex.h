#ifndef _ASM_TRIMEDIA_TIMEX_H
#define _ASM_TRIMEDIA_TIMEX_H

#include <ops/custom_ops.h>

#define CLOCK_TICK_RATE   CONFIG_CPU_FREQ 

typedef unsigned long cycles_t;

static inline cycles_t get_cycles(void)
{
	return (cycles_t)CYCLES();
}

#endif
