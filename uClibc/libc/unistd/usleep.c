#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int usleep (__useconds_t usec)
{
#ifndef __TCS__
    const struct timespec ts = {
	.tv_sec = (long int) (usec / 1000000),
	.tv_nsec = (long int) (usec % 1000000) * 1000ul
    };
#else
    struct timespec ts = { 0 };
#endif 

#ifdef __TCS__
    ts.tv_sec= (long int) (usec / 1000000);
    ts.tv_nsec= (long int) (usec % 1000000) * 1000ul;
#endif 
    
    return(nanosleep(&ts, NULL));
}
