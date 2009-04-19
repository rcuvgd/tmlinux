#include <stddef.h>
#include <setjmp.h>
#include <signal.h>


extern void __longjmp (__jmp_buf __env, int val);


/* Set the signal mask to the one specified in ENV, and jump
   to the position specified in ENV, causing the setjmp
   call there to return VAL, or 1 if VAL is 0.  */
extern void __libc_longjmp (sigjmp_buf env, int val);

void longjmp (sigjmp_buf env, int val)
{
	__libc_longjmp(env,val);
}

void siglongjmp (sigjmp_buf env, int val)
{
	__libc_longjmp(env,val);
}
