#ifndef _ASM_TRIMEDIA_DELAY_H
#define _ASM_TRIMEDIA_DELAY_H

#include <asm/timex.h>

#define cycles_after(a,b) (((long)(b)-(long)(a))<0)

static inline void 
udelay(int usecs)
{
	cycles_t start=get_cycles();
	cycles_t to=start+(usecs*CLOCK_TICK_RATE/1000000);

	while(cycles_after(to,get_cycles()));
}

static inline void 
ndelay(int nsecs)
{
	cycles_t start=get_cycles();
	cycles_t to=start+(nsecs*CLOCK_TICK_RATE/1000000000);

	while(cycles_after(to,get_cycles()));
}

extern void __delay(unsigned long loops);

#endif /*_ASM_TRIMEDIA_DELAY_H */

