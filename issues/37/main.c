#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <ops/custom_ops.h>

static unsigned long child_r4 = 0;
static unsigned long father_r4 = 0;

static pid_t
run_child(unsigned long colle, unsigned long active, int quiet_mode)
{
    int cpid;

    if ((cpid = vfork()) < 0) {
	return -1;
    } else if (cpid == 0) {
	/* child */
	    child_r4 = RDREG(4);
	exit(errno);
    }
    /* parent */
    father_r4 = RDREG(4);

    return cpid;
}

int 
main(void)
{
	run_child((unsigned long)0x11111111,(unsigned long)0x22222222,0x33333333);
	printf("child_r4 = %x, father_r4 = %x\n",child_r4, father_r4);
	while(1);
	return 0;
}

