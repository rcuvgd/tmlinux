/*
 * setegid.c
 */

#include <unistd.h>

int setegid(gid_t egid)
{
	return setregid((gid_t)-1, egid);
}
