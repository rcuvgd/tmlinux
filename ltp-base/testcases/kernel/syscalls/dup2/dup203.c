/*
 *
 *   Copyright (c) International Business Machines  Corp., 2001
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
 *	dup203.c
 *
 * DESCRIPTION
 *	Testcase to check the basic functionality of dup2().
 *
 * ALGORITHM
 *	1.	Attempt to dup2() on an open file descriptor.
 *	2.	Attempt to dup2() on a close file descriptor.
 *
 * USAGE:  <for command-line>
 *  dup203 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS
 *	NONE
 */

#include <fcntl.h>
#include <sys/param.h>
#include <errno.h>
#include <string.h>
#include <test.h>
#include <usctest.h>

void setup(void);
void cleanup(void);

char *TCID = "dup203";
int TST_TOTAL = 1;
extern int Tst_count;

int main(int ac, char **av)
{
	int fd0, fd1, fd2, rval;
	char filename0[40], filename1[40];
	char buf[40];

	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
	}

	setup();

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count = 0;
//block1:
		tst_resm(TINFO, "Enter block 1");
		tst_resm(TINFO, "Test duping over an open fd");

		sprintf(filename0, "dup202.file0.%d\n", getpid());
		sprintf(filename1, "dup202.file1.%d\n", getpid());
		unlink(filename0);
		unlink(filename1);

		if ((fd0 = creat(filename0, 0666)) == -1) {
			tst_brkm(TBROK, cleanup, "cannot create first file");
			/*NOTREACHED*/
		}
		if (write(fd0, filename0, strlen(filename0)) == -1) {
			tst_brkm(TBROK, cleanup, "filename0: write(2) failed");
			/*NOTREACHED*/
		}

		if ((fd1 = creat(filename1, 0666)) == -1) {
			tst_brkm(TBROK, cleanup, "Cannot create second file");
			/*NOTREACHED*/
		}
		if (write(fd1, filename1, strlen(filename1)) == -1) {
			tst_brkm(TBROK, cleanup, "filename1: write(2) failed");
			/*NOTREACHED*/
		}

		if (close(fd0) == -1) {
			tst_brkm(TBROK, cleanup, "close(2) fd0 failed");
			/*NOTREACHED*/
		}
		if ((fd0 = open(filename0, O_RDONLY)) == -1) {
			tst_brkm(TBROK, cleanup, "open(2) on filename0 failed");
			/*NOTREACHED*/
		}

		if (close(fd1) == -1) {
			tst_brkm(TBROK, cleanup, "close(2) fd1 failed");
			/*NOTREACHED*/
		}
		if ((fd1 = open(filename1, O_RDONLY)) == -1) {
			tst_brkm(TBROK, cleanup, "open(2) on filename1 failed");
			/*NOTREACHED*/
		}

		TEST(dup2(fd0, fd1));

		if ((fd2 = TEST_RETURN) == -1) {
			tst_resm(TFAIL, "call failed unexpectedly");
		} else if (STD_FUNCTIONAL_TEST) {
			if (fd1 != fd2) {
				tst_resm(TFAIL, "file descriptors don't match");
				break;
			}

			memset(buf, 0, sizeof(buf));
			if (read(fd2, buf, sizeof(buf)) == -1) {
				tst_brkm(TBROK, cleanup, "read(2) failed");
				/*NOTREACHED*/
			}
			if (strcmp(buf, filename0) != 0) {
				tst_resm(TFAIL, "read from file got bad data");
			}
			tst_resm(TPASS, "dup2 test 1 functionality is correct");
		} else {
			tst_resm(TPASS, "call succeeded");
		}

		close(fd0);
		close(fd1);
		close(fd2);
		unlink(filename0);
		unlink(filename1);

		tst_resm(TINFO, "Exit block 1");

//block2:
		tst_resm(TINFO, "Enter block 2");
		tst_resm(TINFO, "Test close on exec flag");

		sprintf(filename0, "dup02.%d\n", getpid());
		unlink(filename0);

		if ((fd0 = creat(filename0, 0666)) == -1) {
			tst_brkm(TBROK, cleanup, "Cannot create first file");
			/*NOTREACHED*/
		}
		if (fcntl(fd0, F_SETFD, 1) == -1) {
			tst_brkm(TBROK, cleanup, "setting close on exec flag "
				 "on fd0 failed");
			/*NOTREACHED*/
		}

		if ((fd2 = creat(filename1, 0666)) == -1) {
			tst_brkm(TBROK, cleanup, "Cannot create second file");
			/*NOTREACHED*/
		}

		if (close(fd2) == -1) {
			tst_brkm(TBROK, cleanup, "close(2) fd_closed failed");
			/*NOTREACHED*/
		}

		TEST(dup2(fd0, fd2));

		if ((fd1 = TEST_RETURN) == -1) {
			tst_resm(TFAIL, "call failed unexpectedly");
		} else if (STD_FUNCTIONAL_TEST) {
			if (fd1 != fd2) {
				tst_resm(TFAIL, "bad dup2 descriptor %d", fd1);
				break;
			}

			if ((rval = fcntl(fd1, F_GETFD, 0)) != 0) {
				tst_resm(TFAIL, "fcntl F_GETFD on fd1 failed - "
					 "Expected rval of 0, got %d", rval);
				break;
				/*NOTREACHED*/
			}
			if (!((rval = fcntl(fd0, F_GETFL, 0)) && O_WRONLY)) {
				tst_resm(TFAIL, "fctnl F_GETFL bad rval on fd0 "
					 "Expected 1 got %#x", rval);
				/*NOTREACHED*/
			}
			tst_resm(TPASS, "dup2 test 2 functionality is correct");
		} else {
			tst_resm(TPASS, "call succeeded");
		}
		close(fd0);
		close(fd1);

		unlink(filename0);
		unlink(filename1);
		tst_resm(TINFO, "Exit block 2");
	}
	cleanup();

	/*NOTREACHED*/
	return(0);
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void
setup()
{
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temporary directory and cd to it */
	tst_tmpdir();
}


/*
 * cleanup() - performs all ONE TIME cleanup for this test at
 *	       completion or premature exit.
 */
void
cleanup()
{
	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

	/* Remove tmp dir and all files in it */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}
