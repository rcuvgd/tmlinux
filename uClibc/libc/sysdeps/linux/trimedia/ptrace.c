
#include <errno.h>
#include <asm/ptrace.h>
#include <asm/unistd.h>

/*
 *FIXME: I don't know how to implement it for trimedia
 */
int
ptrace(int request, int pid, int addr, int data)
{
	errno=ENOSYS;
	return -1;
}
