#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <asm/unistd.h>

#define gettimeofday(a,b)  syscall(__NR_gettimeofday,a,b)

static int done = 0;

void breakout(int sig)
{
	printf("breakout!!!\n");
        done = 1;
}

int
main(void)
{
	struct timeval tv1;
	struct timeval tv2;
	int ret =0;
	int i = 0;

	memset(&tv1,0,sizeof(tv1));
	memset(&tv2,0,sizeof(tv2));

	signal(SIGALRM, breakout);
	alarm(30);

	while(!done) {
		ret = gettimeofday(&tv2,NULL);
		if(ret!=0){
			printf("gettimeofday failed, errno = %d\n", errno);
			return ret;
		}else{
			/*printf("gettimeofday succeed, tv.tv_sec = %d, tv.tv_usec = %d!\n",*/
			/*tv1.tv_sec, tv1.tv_usec);*/
		}
		
		if((tv2.tv_sec < tv1.tv_sec) || 
				((tv2.tv_sec == tv1.tv_sec) && (tv2.tv_usec < tv1.tv_usec))){
			printf("timeval error, %d!\n", i);
			return 0;
		}

		tv1 = tv2;
		/*sleep(1);*/
	}

	printf("goto out!\n");

	return 0;
}
