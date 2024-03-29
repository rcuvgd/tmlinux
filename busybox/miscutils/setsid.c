/*
 * setsid.c -- execute a command in a new session
 * Rick Sladkey <jrs@world.std.com>
 * In the public domain.
 *
 * 1999-02-22 Arkadiusz Mi�kiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 *
 * 2001-01-18 John Fremlin <vii@penguinpowered.com>
 * - fork in case we are process group leader
 *
 * 2004-11-12 Paul Fox
 * - busyboxed
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "busybox.h"

#if ( defined(__UCLIBC__) || defined(__uClinux__) ) && !defined(__ARCH_HAS_MMU__)
#undef fork
#define fork vfork
#endif 

int
setsid_main(int argc, char *argv[]) {

	if (argc < 2) {
		bb_show_usage();
	}

	if (getpgrp() == getpid()) {
		switch(fork()){
		case -1:
			bb_perror_msg_and_die("fork");
		case 0:
			break;
		default:	/* parent */
			exit(0);
		}
		/* child falls through */
	}

	setsid();  /* no error possible */

	execvp(argv[1], argv + 1);

	bb_perror_msg_and_die(argv[1]);

}
