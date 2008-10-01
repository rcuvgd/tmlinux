#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/syscall.h>

int setegid(gid_t gid)
{
    int result;

    if (gid == (gid_t) ~0)
    {
	__set_errno (EINVAL);
	return -1;
    }

#ifdef __NR_setresgid
#ifndef __TCS__
    result = setresgid(-1, gid, -1);
#else
    result = setresgid((__gid_t)-1, gid,(__gid_t)-1);
#endif 
    if (result == -1 && errno == ENOSYS)
	/* Will also set the saved group ID if egid != gid,
	 * making it impossible to switch back...*/
#endif
#ifndef __TCS__
	result = setregid(-1, gid);
#else
	result = setregid((__gid_t)-1, gid);
#endif 

    return result;
}
