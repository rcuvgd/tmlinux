#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv, char** env)
{
	int i=0;

	if(argv!=NULL){
		if(argc<=0){
			printf("invalid argc!\n");
		}

		i=0;
		while(argv[i]!=NULL){
			printf("argv[%d]='%s'\n",i,argv[i]);
			i++;
		}

		if(i!=argc){
			printf("argc is not correct!\n");
		}
	}

	if(env!=NULL){
		i =0;
		while(env[i]!=NULL){
			printf("env[%d]='%s'\n",i,env[i]);
			i++;
		}
	}

	return 0;
}
