#include <stdio.h>

extern void pthread_one(void);

void pthread_create(void* a, void* b, void* c, void* d)
{
	printf("this is libmystub pthread_create speaking!\n");
	pthread_one();
	return;
}

