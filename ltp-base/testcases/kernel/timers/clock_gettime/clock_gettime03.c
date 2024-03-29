/*
 * Copyright (c) Wipro Technologies Ltd, 2003.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/**************************************************************************
 *
 *    TEST IDENTIFIER	: clock_gettime03
 *
 *    EXECUTED BY	: anyone
 *
 *    TEST TITLE	: Test checking for basic error conditions for
 *    			  clock_gettime(2)
 *
 *    TEST CASE TOTAL	: 7
 *
 *    AUTHOR		: Aniruddha Marathe <aniruddha.marathe@wipro.com>
 *
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    DESCRIPTION
 *    	This test case check whether clock_gettime(2) returns appropriate error
 *    	value for invalid parameter
 *
 * 	Setup:
 *	 Setup signal handling.
 *	 Pause for SIGUSR1 if option specified.
 *
 * 	Test:
 *	 Loop if the proper options are given.
 *	 If it is the first test case
 *	 	make temp a bad pointer
 *	 Otherwise pass defined struct timespec variable to temp
 *	 Execute system call with invalid parameter
 *	 Check return code, if system call fails with errno == expected errno
 * 	 	Issue syscall passed with expected errno
 *	 Otherwise, Issue syscall failed to produce expected errno
 *
 * 	Cleanup:
 * 	 Print errno log and/or timing stats if options given
 *
 * USAGE:  <for command-line>
 * clock_gettime03 [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-p]
 * where:
 * 	-c n : run n copies simultaneously
 *	-e   : Turn on errno logging.
 *	-i n : Execute test n times.
 *	-I x : Execute test for x seconds.
 *	-p   : Pause for SIGUSR1 before starting
 *	-P x : Pause for x seconds between iterations.
 *	-t   : Turn on syscall timing.
 *
 * RESTRICTIONS:
 * None
 *****************************************************************************/

#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#include "test.h"
#include "usctest.h"
#include "common_timers.h"

static void setup();
static void cleanup();

char *TCID = "clock_gettime03"; /* Test program identifier.    */
int TST_TOTAL;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */
static int exp_enos[] = {EINVAL, EFAULT, 0};

static struct test_case_t {
	char *err_desc;		/* error description */
	int  exp_errno;		/* expected error number*/
	char *exp_errval;	/* Expected errorvalue string*/
} testcase[6] = {
	{"Bad address", EFAULT, "EFAULT"},			/* Bad timespec   */
	{"Bad address", EFAULT, "EFAULT"},			/* Bad timespec   */
	{"Invalid parameter", EINVAL, "EINVAL"},	/* MAX_CLOCKS	  */
	{"Invalid parameter", EINVAL, "EINVAL"}		/* MAX_CLOCKS + 1 */
};

int
main(int ac, char **av)
{
	int lc, i;	/* loop counter */
	char *msg;	/* message returned from parse_opts */
	struct timespec spec, *temp;

	clockid_t clocks[] = {
		CLOCK_REALTIME,
		CLOCK_MONOTONIC,
		MAX_CLOCKS,
		MAX_CLOCKS + 1,
		CLOCK_PROCESS_CPUTIME_ID,
		CLOCK_THREAD_CPUTIME_ID
	};

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL))
		!= (char *)NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	TST_TOTAL = sizeof(testcase) / sizeof(testcase[0]);

	/* PROCESS_CPUTIME_ID & THREAD_CPUTIME_ID are not supported on
	 * kernel versions lower than 2.6.12
	 */
	if((tst_kvercmp(2, 6, 12)) < 0) {
		testcase[4].err_desc = "Invalid parameter";	/* PROCESS_CPUTIME_ID */
		testcase[4].exp_errno = EINVAL;
		testcase[4].exp_errval = "EINVAL";
		testcase[5].err_desc = "Invalid parameter";	/* THREAD_CPUTIME_ID */
		testcase[5].exp_errno = EINVAL;
		testcase[5].exp_errval = "EINVAL";
	}
	else {
		testcase[4].err_desc = "Bad address";	/* Bad timespec pointer */
		testcase[4].exp_errno = EFAULT;
		testcase[4].exp_errval = "EFAULT";
		testcase[5].err_desc = "Bad address";	/* Bad timespec pointer */
		testcase[5].exp_errno = EFAULT;
		testcase[5].exp_errval = "EFAULT";
	}


	/* perform global setup for test */
	setup();

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count = 0;

		for (i = 0; i < TST_TOTAL; i++) {
			temp = &spec;

			if (i == 0) {
				temp = (struct timespec *) -1;
			} else if (i == 1) {
				temp = (struct timespec *) NULL;
			} else if ((i >= 4) && (tst_kvercmp(2, 6, 12) >= 0)) {
				temp = (struct timespec *) NULL;
			}

			TEST(clock_gettime(clocks[i], temp));

			if (TEST_ERRNO == ENOSYS) {
				/* System call is not implemented */
				Tst_count = TST_TOTAL;
				perror("clock_gettime");
				tst_brkm(TBROK, cleanup, "");
			}

			/* check return code */
			if ((TEST_RETURN == -1) && (TEST_ERRNO == testcase[i].
						exp_errno)) {
				tst_resm(TPASS, "clock_gettime(2) expected"
						" failure; Got errno - %s : %s"
						, testcase[i].exp_errval,
						testcase[i].err_desc);
			} else {
				tst_resm(TFAIL, "clock_gettime(2) failed to"
						" produce expected error; %d"
						" , errno : %s and got %d",
						testcase[i].exp_errno,
						testcase[i].exp_errval,
						TEST_ERRNO);
			} /* end of else */

			TEST_ERROR_LOG(TEST_ERRNO);
		}	/*End of TEST CASE LOOPING*/
	}	/*End for TEST_LOOPING*/

	/* Clean up and exit */
	cleanup();

	/* NOTREACHED */
	return 0;
}

/* setup() - performs all ONE TIME setup for this test */
void
setup()
{
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* set the expected errnos... */
	TEST_EXP_ENOS(exp_enos);

	/* Pause if that option was specified */
	TEST_PAUSE;
}	/* End setup() */

/*
 * cleanup() - Performs one time cleanup for this test at
 * completion or premature exit
 */

void
cleanup()
{
	/*
	* print timing stats if that option was specified.
	* print errno log if that option was specified.
	*/
	TEST_CLEANUP;

	/* exit with return code appropriate for results */
	tst_exit();
}	/* End cleanup() */
