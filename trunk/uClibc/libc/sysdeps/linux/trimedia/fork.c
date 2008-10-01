#include <errno.h>

int fork(void)
{
	return -EINVAL;
}
