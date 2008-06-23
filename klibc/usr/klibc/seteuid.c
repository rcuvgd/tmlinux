/*
 * seteuid.c
 */

#include <unistd.h>

int seteuid(uid_t euid)
{
	return setreuid((uid_t)-1, euid);
}
