#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __TCS__
#include <tmlib/dprintf.h>
#define printf _dp
#endif 

extern int foo(void);

int
main(void)
{
	void* result=NULL;

	result=malloc(20);
	if(result==NULL)
	{
		printf("malloc(20) failed, errno is %d\n",errno);
		return -1;
	}

	printf("malloc(20) success, return %p!\n",result);

	foo();

	return 0;
}
