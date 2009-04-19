#include <stdio.h>
#include <stdlib.h>
/*#include <pthread.h>*/

extern void pthread_create(void* , void* , void*, void*);
extern void pthread_one(void);

int
main(void)
{
	pthread_one();
	pthread_create(NULL, NULL, NULL,NULL);
	return 0;
}
