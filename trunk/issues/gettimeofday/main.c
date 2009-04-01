#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <asm/unistd.h>

int
main(void)
{
	struct timeval tv2;
	int ret =0;
	int i = 0;

	while(1) {
		ret = gettimeofday(&tv2,NULL);
		if(ret!=0){
			printf("gettimeofday failed, errno = %d, ret = %d, i = %d\n", 
					errno, ret, i);
			break;
		}
		i++;
		
	}

	printf("goto out!\n");
	while(1);

	return 0;
}
