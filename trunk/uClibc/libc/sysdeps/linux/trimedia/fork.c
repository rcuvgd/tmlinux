#include <errno.h>

int fork(void)
{
	errno = EINVAL;
	return -1;
}
