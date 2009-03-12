#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static pid_t
/*run_child(unsigned char *colle, unsigned char *active, int quiet_mode)*/
run_child(unsigned long colle, unsigned long active, int quiet_mode)
{
    int cpid;

    printf("run_child before vfork(%p,%p,%x)\n",colle,active,quiet_mode);

    if ((cpid = vfork()) < 0) {
	return -1;
    } else if (cpid == 0) {
	/* child */
	printf("run_child child(%p,%p,%x)\n",colle,active,quiet_mode);
	exit(errno);
    }
    /* parent */
    printf("run_child of parent (%p,%p,%x)\n",colle,active,quiet_mode);

    return cpid;
}

int 
main(void)
{
	run_child((unsigned long)0x11111111,(unsigned long)0x22222222,0x33333333);
	while(1);
	return 0;
}

