/*
 *
 */
#include <bits/uClibc_page.h>

#include <ops/custom_ops.h>

#ifndef NULL
#define NULL (void*)0
#endif 

#define USERSPACE_STACK_SIZE  (2*PAGE_SIZE)

extern int main(int argc, char** argv, char** env);
extern void __uClibc_main(int (*main)(int, char **, char **), 
		int argc, char **argv, void (*app_init)(void), void (*app_fini)(void),
		void (*rtld_fini)(void), void *stack_end);

extern void __rtld_fini(void);

/*
 * The following symbols are used by libcstub.a, 
 * Due to the tmcc bug, they must be resolved at first.
   _errno
   _h_errno
   ___libc_longjmp
   ___raise
   ___sigaction_internal
   ___sigwait
*/
extern int errno;
extern int h_errno;
extern int __libc_longjmp;
extern int __raise;
extern int __sigaction_internal;
extern int __sigwait;

void* errno_addr = &errno;
void* h_errno_addr = &h_errno;
void* __libc_longjmp_addr = &__libc_longjmp;
void* __raise_addr = &__raise;
void* __sigaction_internal_addr = &__sigaction_internal;
void* __sigwait_addr = &__sigwait;

#ifdef CONFIG_DYNLOAD
extern void _application_main(void)
{
	unsigned long sp=RDREG(4);
	unsigned long argc=0;
	char** argv=NULL;
	void* stack_end=NULL;

	argc=*((unsigned long*)sp);
	argv=(char**)(*(unsigned long*)(sp+sizeof(unsigned long)));
	stack_end=(void*)(sp+USERSPACE_STACK_SIZE); 

	__uClibc_main(main,argc,argv,NULL,NULL,NULL,stack_end);
}
#else
extern void userapp_start(void* dummy,int (*user_main)(int,char**,char**))
{
	unsigned long sp=RDREG(4);
	unsigned long argc=0;
	char** argv=NULL;
	void* stack_end=NULL;

	argc=*((unsigned long*)sp);
	argv=(char**)(*(unsigned long*)(sp+sizeof(unsigned long)));
	stack_end=(void*)(sp+USERSPACE_STACK_SIZE); 

	__uClibc_main(user_main,argc,argv,NULL,NULL,NULL,stack_end);
}
#endif
