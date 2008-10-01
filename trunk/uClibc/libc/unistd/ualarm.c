#define setitimer __setitimer

#define _GNU_SOURCE
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

useconds_t ualarm(useconds_t value, useconds_t interval)
{
    struct itimerval otimer;
#ifndef __TCS__
    const struct itimerval itimer = {
	{ 0, interval },
	{ 0, value}
    };
#else
    struct itimerval itimer={0};

    itimer.it_interval.tv_sec=0;
    itimer.it_interval.tv_usec=interval;

    itimer.it_value.tv_sec=0;
    itimer.it_value.tv_usec=value;

#endif 

    if (setitimer(ITIMER_REAL, &itimer, &otimer) < 0) {
#ifndef __TCS__
	return -1;
#else
	return (unsigned int)-1;
#endif 
    }
    return((otimer.it_value.tv_sec * 1000000) + otimer.it_value.tv_usec);
}
