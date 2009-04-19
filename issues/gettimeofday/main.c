#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <asm/unistd.h>

extern int gtod_count;

int
user_main(void)
{
	struct timeval tv2;
	int ret =0;
	int i = 0;

	printf("compare gtod_count!\n");

	while(1) {
		if(i!=gtod_count){
			printf("gtod_count is incorrect!\n");
			break;
		}
		ret = gettimeofday(&tv2,NULL);
		if(ret!=0){
			printf("gettimeofday failed, errno = %d, ret = %d, i = %d\n", 
					errno, ret, i);
			break;
		}
		i++;
		{
			int j =0 ;
			for(j=0;j<40000000;j++){;}
		}	
	}

	printf("goto out!\n");
	while(1);

	return 0;
}
