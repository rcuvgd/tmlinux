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
 *    TEST IDENTIFIER	: timer_settime02
 *
 *    EXECUTED BY	: anyone
 *
 *    TEST TITLE	: Basic test for timer_settime(2)
 *
 *    TEST CASE TOTAL	: 4
 *
 *    AUTHOR		: Aniruddha Marathe <aniruddha.marathe@wipro.com>
 *
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    DESCRIPTION
 *     This is a Phase I test for the timer_settime(2) system call.
 *     It is intended to provide a limited exposure of the system call.
 *
 * 	Setup:
 *	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 *
 * 	Test:
 *	 Loop if the proper options are given.
 *	 setup individual test
 *	 Execute system call
 *	 Check return code, if system call failed (return=-1)
 *		Log the errno and Issue a FAIL message.
 *	 Otherwise, Issue a PASS message.
 *
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 *
 * USAGE:  <for command-line>
 * timer_settime02 [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-p]
 * where:
 * 	-c n : Run n copies simultaneously.
 *	-e   : Turn on errno logging.
 *	-i n : Execute test n times.
 *	-I x : Execute test for x seconds.
 *	-p   : Pause for SIGUSR1 before starting
 *	-P x : Pause for x seconds between iterations.
 *	-t   : Turn on syscall timing.
 *
 *RESTRICTIONS:
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
static int setup_test(int option);

char *TCID = "timer_settime02";	/* Test program identifier.    */
int TST_TOTAL = 4;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

static struct itimerspec new_set, old_set, *old_temp;
static timer_t timer;
static int flag;

int
main(int ac, char **av)
{
	int lc, i;	/* loop counter */
	char *msg;	/* message returned from parse_opts */

	if ((msg = parse_opts(ac, av, (option_t *) NULL, NULL)) !=
			(char *) NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	/* perform global setup for test */
	setup();

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count = 0;

		for (i = 0; i < TST_TOTAL; i++) {

			/* Set up individual test */
			if (setup_test(i) < 0) {
				continue;	/* We are skipping this test */
			}

			TEST(timer_settime(timer, flag, &new_set, old_temp));

			if (TEST_ERRNO == ENOSYS) {
				/* system call is not implemented */
				Tst_count = TST_TOTAL;
				perror("timer_settime");
				tst_brkm(TBROK, cleanup, "");
			}
			if (TEST_RETURN == -1) {
				TEST_ERROR_LOG(TEST_ERRNO);
				tst_resm(TFAIL, "timer_settime(2) Failed and"
						" set errno to %d", TEST_ERRNO);
			} else {
				tst_resm(TPASS, "timer_settime(2) Passed");
			}
		}	/* End of TEST CASE LOOPING */
	}		/* End for TEST_LOOPING */

	/* Clean up and exit */
	cleanup();

	/* NOTREACHED */
	return 0;
}

/* This function does set up for individual tests */
int
setup_test(int option)
{
	struct timespec timenow;	/* Used to obtain current time */

	switch (option) {
		case 0:
			/* This is general initialization.
			 * make old_setting NULL
			 * make flags equal to zero
			 * use one-shot timer
			 */
			old_temp = (struct itimerspec *) NULL;
			new_set.it_interval.tv_sec = 0;
			new_set.it_interval.tv_nsec = 0;
			new_set.it_value.tv_sec = 5;
			new_set.it_value.tv_nsec = 0;
			flag = 0;
			break;
		case 1:
			/* get the old_setting in old_set
			 * This test case also takes care
			 * of situation where the timerid is
			 * already armed
			 */
			old_temp = &old_set;
			break;
		case 2:
			/* Use the periodic timer */
			new_set.it_interval.tv_sec = 5;
		        new_set.it_value.tv_sec = 0;
			break;
		case 3:
			/* Use TIMER_ABSTIME flag for setting
			 * absolute time for timer
			 */
			flag = TIMER_ABSTIME;
			if (clock_gettime(CLOCK_REALTIME, &timenow) < 0) {
				tst_resm(TWARN, "clock_gettime failed"
						" skipping the test");
				return -1;
			}
			new_set.it_value.tv_sec = timenow.tv_sec + 25;
			break;
	}
	return 0;
}

/* setup() - performs all ONE TIME setup for this test */
void
setup()
{
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	if (timer_create(CLOCK_REALTIME, NULL, &timer) < 0) {
		if (errno == ENOSYS) {
			/* system call is not implemented */
			TST_TOTAL = 1;
			perror("timer_create");
			tst_brkm(TBROK, tst_exit, "");
		}
		tst_brkm(TBROK, tst_exit, "timer_create(2) failed");
	}
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
