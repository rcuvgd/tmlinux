#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char** argv, char** env)
{
	char* cwd=NULL;

	printf("getcwd test in!\n");

	cwd=getcwd(NULL,0);

	if(cwd==NULL){
		printf("getcwd failed, with errno = %d\n",errno);
		return -1;
	}

	printf("cwd = '%s'\n",cwd);
	
	while(1);
	return 0;
}
