#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int seteuid(uid_t uid)
{
    int result;

    if (uid == (uid_t) ~0)
    {
	__set_errno (EINVAL);
	return -1;
    }

#ifdef __NR_setresuid
#ifndef __TCS__
    result = setresuid(-1, uid, -1);
#else
    result = setresuid((__gid_t)-1, uid, (__gid_t)-1);
#endif 
    if (result == -1 && errno == ENOSYS)
	/* Will also set the saved user ID if euid != uid,
	 * making it impossible to switch back...*/
#endif
#ifndef __TCS__
	result = setreuid(-1, uid);
#else
	result = setreuid((__gid_t)-1, uid);
#endif 

    return result;
}
