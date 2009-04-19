#include <errno.h>
#include <signal.h>
#include <string.h>

#include <sys/syscall.h>

/* The difference here is that the sigaction structure used in the
   kernel is not the same as we use in the libc.  Therefore we must
   translate it here.  */
#include <bits/kernel_sigaction.h>

extern int __sigaction_internal (int sig, const struct sigaction *act, struct sigaction *oact);

int 
__sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return __sigaction_internal(sig,act,oact);
}

int
sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return __sigaction_internal(sig,act,oact);
}
