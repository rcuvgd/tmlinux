#include <errno.h>

#if 0
int fork(void)
{
	errno = EINVAL;
	return -1;
}
#endif 
