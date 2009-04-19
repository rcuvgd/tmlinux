#include <errno.h>
#include <signal.h>
#include <string.h>

#undef sigwait
extern int __sigwait (const sigset_t *set, int *sig);

int sigwait (const sigset_t *set, int *sig)
{
	return __sigwait(set,sig);
}
