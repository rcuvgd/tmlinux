#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

extern int __internal_clone(int (*fn)(void *arg), void *child_stack, int flags, void *arg);

extern int 
clone(int (*fn)(void *arg), void *child_stack, int flags, void *arg)
{
	int retval = -EINVAL;

	retval=__internal_clone(fn,child_stack,flags,arg);

	__syscall_return(int,retval);
}

