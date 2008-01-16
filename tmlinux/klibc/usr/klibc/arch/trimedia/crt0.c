/*
 *FIXME: too simpile for standard libc
 */
#include <stdio.h>
#include <ops/custom_ops.h>

#undef DEBUG

void _start(void)
{
	unsigned long sp=RDREG(4);
	unsigned long argc=0;
#ifdef DEBUG 
	int i=0;
#endif
	int ret=0;
	char** argv=NULL;
	char** env=NULL;

	argc=*((unsigned long*)sp);
	argv=(char**)(*(unsigned long*)(sp+sizeof(unsigned long)));
	env=(char**)(*(unsigned long*)(sp+2*sizeof(unsigned long)));

#ifdef DEBUG 
	printf("sp is %x\n",sp);
	printf("argc is %d\n",argc);
	
	for(i=0;argv[i]!=NULL;i++){
		printf("argv[%d] is '%s'\n",i,argv[i]);
	}

	for(i=0;env[i]!=NULL;i++){
		printf("env[%d] is '%s'\n",i,env[i]);	
	}
#endif

	ret=main(argc,argv,env);
#ifdef DEBUG 
	printf("main returns %d\n",ret);
#endif 
	exit(ret);
}
