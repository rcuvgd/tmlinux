/* Copyright (C) 1997,1998,1999,2000,2002,2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <signal.h>
#include <string.h>

#include <sys/syscall.h>


/* The difference here is that the sigaction structure used in the
   kernel is not the same as we use in the libc.  Therefore we must
   translate it here.  */
#include <bits/kernel_sigaction.h>

#if defined __NR_rt_sigaction

/* If ACT is not NULL, change the action for SIG to *ACT.
   If OACT is not NULL, put the old action for SIG in *OACT.  */
int attribute_hidden
__sigaction_internal (int sig, const struct sigaction *act, struct sigaction *oact)
{
	int result;
	struct kernel_sigaction kact, koact;

	if (act) {
		kact.k_sa_handler = act->sa_handler;
		__memcpy (&kact.sa_mask, &act->sa_mask, sizeof (kact.sa_mask));
		kact.sa_flags = act->sa_flags;
# ifdef HAVE_SA_RESTORER
		kact.sa_restorer = act->sa_restorer;
# endif
	}
	
	/* XXX The size argument hopefully will have to be changed to the
	   real size of the user-level sigset_t.  */
	result = __syscall_rt_sigaction(sig,
			       act ? __ptrvalue (&kact) : NULL,
			       oact ? __ptrvalue (&koact) : NULL, _NSIG / 8);

	if (oact && result >= 0) {
		oact->sa_handler = koact.k_sa_handler;
		__memcpy (&oact->sa_mask, &koact.sa_mask, sizeof (oact->sa_mask));
		oact->sa_flags = koact.sa_flags;
# ifdef HAVE_SA_RESTORER
		oact->sa_restorer = koact.sa_restorer;
# endif
	}

	return result;
}

#else

/* If ACT is not NULL, change the action for SIG to *ACT.
   If OACT is not NULL, put the old action for SIG in *OACT.  */
int attribute_hidden
__sigaction_internal (int sig, const struct sigaction *act, struct sigaction *oact)
{
	int result;
	struct old_kernel_sigaction kact, koact;

	if (act) {
		kact.k_sa_handler = act->sa_handler;
		kact.sa_mask = act->sa_mask.__val[0];
		kact.sa_flags = act->sa_flags;
# ifdef HAVE_SA_RESTORER
		kact.sa_restorer = act->sa_restorer;
# endif
	}

	result = __syscall_sigaction(sig,
			       act ? __ptrvalue (&kact) : NULL,
			       oact ? __ptrvalue (&koact) : NULL);

	if (oact && result >= 0) {
		oact->sa_handler = koact.k_sa_handler;
		oact->sa_mask.__val[0] = koact.sa_mask;
		oact->sa_flags = koact.sa_flags;
# ifdef HAVE_SA_RESTORER
		oact->sa_restorer = koact.sa_restorer;
# endif
	}

	return result;
}

#endif

#ifndef __TCS__
strong_alias(__sigaction_internal,__libc_sigaction)
weak_alias(__sigaction_internal,__sigaction)
weak_alias(__sigaction_internal,sigaction)
#else
int attribute_hidden
__libc_sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return __sigaction_internal(sig,act,oact);
}
#if 0
/*for trimedia, the following functions are moved to libcstub.a*/
int attribute_hidden
__sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return __sigaction_internal(sig,act,oact);
}
int attribute_hidden
sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return __sigaction_internal(sig,act,oact);
}
#endif 
#endif
