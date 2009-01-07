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
