/* Copyright (C) 1995,1996 Robert de Bath <rdebath@cix.compulink.co.uk>
 * This file is part of the Linux-8086 C library and is distributed
 * under the GNU Library General Public License.
 */

#define kill __kill

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#undef raise
extern int __raise(int signo);

int raise(int signo)
{
	return __raise(signo);
}
