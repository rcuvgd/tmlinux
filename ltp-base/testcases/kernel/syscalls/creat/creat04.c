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
 *	creat04.c
 *
 * DESCRIPTION
 *	testcase to check creat(2) fails with EACCES
 *
 * ALGORITHM
 *	1.	A parent spawns a child, which creates a test-directory, and
 *		makes it 700. Then the parent spawns another child, does a
 *		setreuid to ltpuser1, and attempts to creat() a file (which
 *		does not exist) in the directory created by the first child.
 *	2.	Repeat 1, for a file (to be created by ltpuser1) which exists.
 *		That is the first child should creat() a file before exiting.
 *
 * USAGE:  <for command-line>
 *  creat04 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS
 *	Test must be run as root.
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "test.h"
#include "usctest.h"

char *TCID = "creat04";
int TST_TOTAL = 2;
extern int Tst_count;

void setup(void);
void cleanup(void);

#define FMODE	0444
#define DMODE	00700

int exp_enos[] = {EACCES, 0};

char user1name[] = "nobody";
char good_dir[40] = "testdir";
char fname[40], fname1[40];
extern struct passwd * my_getpwnam(char *);
struct passwd *ltpuser1;

struct test_case_t {
	char *fname;
} TC[] = {
	/* comment */
	{fname},

	/* comment */
	{fname1}
};

int
main(int ac, char **av)
{
	int lc;				/* loop counter */
	int retval=0;
	char *msg;			/* message returned from parse_opts */

	pid_t pid, pid1;
	int i, e_code, status, fd;

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	setup();

	TEST_EXP_ENOS(exp_enos);

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		if ((pid = FORK_OR_VFORK()) == -1) {
			tst_brkm(TBROK, cleanup, "fork() #1 failed");
		}

		if (pid == 0) {		/* first child */
			if (mkdir(good_dir, DMODE) != 0) {
				tst_resm(TINFO, "mkdir() failed");
				exit(1);
			}
			if ((fd = open(fname1, O_RDWR | O_CREAT, 0444))
			    == -1) {
				tst_resm(TINFO, "open() failed");
				exit(1);
			}
			exit(0);
		}
		wait(&status);

		/* make sure the child returned a good exit status */
		e_code = status >> 8;
		if (e_code != 0) {
			tst_brkm(TBROK, cleanup, "child #1 failed");
		}

		if ((pid1 = FORK_OR_VFORK()) == -1) {
			tst_brkm(TBROK, cleanup, "fork() #2 failed");
		}

		if (pid1 == 0) {		/* second child */

			ltpuser1 = my_getpwnam(user1name);

			if (seteuid(ltpuser1->pw_uid) == -1) {
				tst_resm(TINFO, "setreuid failed to "
					 "to set the effective uid to %d",
					 ltpuser1->pw_uid);
				perror("setreuid");
				continue;
			}

			/* loop through the test cases */
			for (i=0; i<TST_TOTAL; i++) {

				TEST(creat(TC[i].fname, FMODE));

				if (TEST_RETURN != -1) {
					retval=1;
					tst_resm(TFAIL, "call succeeded "
						 "unexpectedly");
					continue;
				}

				TEST_ERROR_LOG(TEST_ERRNO);

				if (TEST_ERRNO != EACCES) {
					retval=1;
					tst_resm(TFAIL, "Expected EACCES got "
						 "%d : %s", TEST_ERRNO,
						 strerror(TEST_ERRNO));
				} else {
					tst_resm(TPASS, "call failed with "
						 "expected EACCES error");
				}
			}

			/* reset our ID back to the saved ID - root */
			seteuid(0);

			/* clean up things in case we are looping */
			unlink(fname);
			unlink(fname1);
			rmdir(good_dir);
			exit(retval);

		} else {			/* parent */
			/* wait for the child to finish */
			wait(&status);
			/* make sure the child returned a good exit status */
			e_code = status >> 8;
			if (e_code != 0) {
				tst_resm(TFAIL, "Failures reported above");
			}
		}
	}
	cleanup();

	return 0;
	/*NOTREACHED*/
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void
setup()
{
	/* The test must be run as root */
	if (geteuid() != 0) {
		tst_brkm(TBROK, tst_exit, "Test must be run as root");
	}

	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temporary directory and cd to it */
	tst_tmpdir();

	sprintf(good_dir, "%s.%d", good_dir, getpid());
	sprintf(fname1, "%s/file1.%d", good_dir, getpid());
	sprintf(fname, "%s/file.%d", good_dir, getpid());
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

	/* delete the test directory created in setup() */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}
