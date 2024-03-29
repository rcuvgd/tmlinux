/*
 *
 *   Copyright (c) International Business Machines  Corp., 2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * NAME
 *	pipe03.c
 *
 * DESCRIPTION
 * 	Make sure that writing to the read end of a pipe and reading from 
 * 	the write end of a pipe both fail.
 *
 * ALGORITHM
 * 	1. Open a pipe
 * 	2. Attempt to write to the [0] descriptor (expect -1)
 * 	3. Attempt to read from the [1] descriptor (expect -1)
 *
 * USAGE:  <for command-line>
 *  pipe03 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	11/2002 Ported by Paul Larson
 */
#include <unistd.h>
#include <errno.h>
#include "test.h"
#include "usctest.h"

char *TCID = "pipe03";
int TST_TOTAL = 1;
extern int Tst_count;

int exp_enos[] = {EBADF, 0};

void setup(void);
void cleanup(void);

ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

int main(int ac, char **av)
{
	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */

	int fildes[2];			/* fds for pipe read and write */
	char rbuf[BUFSIZ];

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
		/*NOTREACHED*/
	}

	setup();

	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		TEST(pipe(fildes));

		if (TEST_RETURN == -1)
			tst_brkm(TBROK, cleanup, "pipe() failed unexpectedly "
					"- errno %d", TEST_ERRNO);

		TEST(write(fildes[0], "A", 1));
		if (TEST_RETURN == -1 && TEST_ERRNO == EBADF)
			tst_resm(TPASS, "expected failure writing to "
					"read end of pipe");
		else
			tst_resm(TFAIL, "success when writing to read "
					"end of pipe ret=%d, errno=%d", 
					TEST_RETURN, TEST_ERRNO);

		TEST(safe_read(fildes[1], rbuf, 1));
		if (TEST_RETURN == -1 && TEST_ERRNO == EBADF)
			tst_resm(TPASS, "expected failure reading from "
					"write end of pipe");
		else
			tst_resm(TFAIL, "success when reading from "
					"write end of pipe ret=%d, "
					"errno=%d", TEST_RETURN, 
					TEST_ERRNO);
	}
	cleanup();

	/*NOTREACHED*/
	return 0;
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void setup()
{
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;
}

/*
 * cleanup() - performs all ONE TIME cleanup for this test at
 *	       completion or premature exit.
 */
void cleanup()
{
	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

	/* exit with return code appropriate for results */
	tst_exit();
}
