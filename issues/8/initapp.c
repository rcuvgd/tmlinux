#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
main(void)
{
	pid_t pid=0;

	printf("goto initapp test case!\n");

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
		execve("ifconfig",argv,NULL);
		exit(errno);
	}
	/*father process*/
	printf("create process for ifconfig %d\n",pid);

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
		exit(errno);

	}

	/*father process*/
	printf("create process for server %d\n",pid);

	while(1);	

	return 0;
}
