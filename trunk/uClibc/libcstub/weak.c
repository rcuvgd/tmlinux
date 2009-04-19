
#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static int __pthread_return_0 __P ((void));
static int __pthread_return_0 (void) { return 0; }

int __pthread_mutex_init(void)
{
	return __pthread_return_0();
}
int __pthread_mutex_lock(void)
{
	return __pthread_return_0();
}
int __pthread_mutex_trylock(void)
{
	return __pthread_return_0();
}
int __pthread_mutex_unlock(void)
{
	return __pthread_return_0();
}

