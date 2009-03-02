#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	pid_t pid=0;

	printf("goto initapp test case!\n");

#if 1
	pid=vfork();
	if(pid<0){
		printf("Error failed to create child process.\n");
		return 0;
	}
	if(pid==0){
		/*child process*/
		char* argv[]={
			"/sbin/ifconfig",
			"lo",
			"127.0.0.1",
			NULL,
		};
		execve("/sbin/ifconfig",argv,NULL);
	}
	/*father process*/
	printf("create process for ifconfig %d\n",pid);
#endif 

#if 1
	pid=vfork();
	if(pid<0){
		printf("Error failed to create child process.\n");
		return 0;
	}
	if(pid==0){
		/*child process*/
		int ret=0;
		char* argv[]={
			NULL,
		};
		char* env[]={
			NULL,
		};
		ret=execve("/server",argv,env);
		printf("failed to execve, return value is %d\n",ret);

	}

	/*father process*/
	printf("create process for server %d\n",pid);
#endif 

	while(1);	

	return 0;
}
