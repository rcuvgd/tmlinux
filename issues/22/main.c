#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#ifdef __TCS__ 
#include <tmlib/dprintf.h>
#define printf _dp
#endif 

	int
main(void)
{
	int ret=0;
	uid_t uid,euid,suid;

	ret=getresuid(&uid,&euid,&suid);

	return 0;
}
