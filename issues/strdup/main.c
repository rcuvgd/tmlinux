#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printf _dp

int
main(void)
{
	char* str=NULL;

	str=strdup("hello string!");
	if(str==NULL){
		printf("strdup returns NULL\n");
	}else{
		printf("strdup returns '%s'\n",str);
		free(str);
		str=NULL;
	}
	return 0;
}
