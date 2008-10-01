
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>

/*
 * FIXME: I don't know how to implement it for trimedia
 */
int brk (void *addr)
{
    return -ENOMEM;
}
