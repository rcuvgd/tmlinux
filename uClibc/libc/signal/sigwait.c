/* vi: set sw=4 ts=4: */
/* sigwait
 *
 * Copyright (C) 2003 by Erik Andersen <andersen@uclibc.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The GNU C Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.  */

#include <errno.h>
#include <signal.h>
#include <string.h>

#undef sigwait
int attribute_hidden __sigwait (const sigset_t *set, int *sig)
{
	int ret = 1;
	if ((ret = sigwaitinfo(set, NULL)) != -1) {
		*sig = ret;
		return 0;
	}
	return 1;
}

/* psm: keep this weak, because the one in libpthread.so could overwrite it */
#ifndef __TCS__
weak_alias(__sigwait, sigwait)
#else
/*for trimedia, sigwait is moved to libcstub.a*/
#endif
