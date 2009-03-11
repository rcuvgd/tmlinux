#include <errno.h>
#include <sys/syscall.h>

typedef unsigned long arg_t;

/* Invoke `system call' NUM, passing it the remaining arguments.
   This is completely system-dependent, and not often useful.  */
long
syscall (long num, arg_t a1, arg_t a2, arg_t a3, arg_t a4, arg_t a5, arg_t a6)
{
	register long __ret;

	if(num==0){
		/*syscall 0 is reserved for restart_syscall, should never be called from userspace.*/
		__ret = -1;
		errno = EFAULT;
	}else{
		__ret= __syscall(num, a1, a2, a3, a4, a5, a6);
		__syscall_return (long, __ret);
	}
	return (long)__ret;
}

