#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int
main(void)
{
	jmp_buf buf;
	int ret=0;

	ret=setjmp(buf);
	if(ret!=0){
		printf("longjmp to here, ret=%d\n",ret);
		return 0;
	}

	printf("begin to longjmp\n");

	longjmp(buf,3);

	return 0;
}
