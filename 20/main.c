#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __TCS__ 
#include <tmlib/dprintf.h>
#define printf _dp
#endif 

static char* filename="noexist";

int
main(void)
{
	int ret=0;

	printf("main in\n");

	ret=open(filename,O_RDWR,0);
	if(ret<0){
		printf("open '%s' failed, returns %d, errno=%d!\n",filename,ret,errno);
		return ret;
	}

	printf("open '%s' success, returns %d\n",filename,ret);

	return 0;
}
