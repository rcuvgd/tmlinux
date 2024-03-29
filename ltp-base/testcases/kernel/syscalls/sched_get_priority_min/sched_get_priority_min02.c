/*
 * Copyright (c) Wipro Technologies Ltd, 2002.  All Rights Reserved.
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
/**********************************************************
 * 
 *    TEST IDENTIFIER	: sched_get_priority_min02
 * 
 *    EXECUTED BY	: anyone
 * 
 *    TEST TITLE	: Test for error conditions
 * 
 *    TEST CASE TOTAL	: 1
 * 
 *    AUTHOR		: Saji Kumar.V.R <saji.kumar@wipro.com>
 * 
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    DESCRIPTION
 *	Verify that given an invalid scheduling policy,
 *	sched_get_priority_min() returns -1 with errno EINVAL
 * 
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 * 
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  Execute system call
 *	  Check return code, if system call failed (return=-1) & errno=EINVAL
 *		Test Passed
 *	  Otherwise
 *		Test Failed
 * 
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 * 
 * USAGE:  <for command-line>
 *  sched_get_priority_min02 [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-h] [-f]
 * 			     [-p]
 *			where,  -c n : Run n copies concurrently.
 *				-e   : Turn on errno logging.
 *				-h   : Show help screen
 *				-f   : Turn off functional testing
 *				-i n : Execute test n times.
 *				-I x : Execute test for x seconds.
 *				-p   : Pause for SIGUSR1 before starting
 *				-P x : Pause for x seconds between iterations.
 *				-t   : Turn on syscall timing.
 *
 ****************************************************************/

#include <errno.h>
#include <sched.h>
#include "test.h"
#include "usctest.h"

#define SCHED_INVALID 1000

static void setup();
static void cleanup();

char *TCID = "sched_get_priority_min02"; /* Test program identifier.    */
extern int Tst_count;		/* Test Case counter for tst_* routines */

int TST_TOTAL = 1;

int
main(int ac, char **av)
{

	int lc;		/* loop counter */
	char *msg;	/* message returned from parse_opts */
    
	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL))
	     != (char *)NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	/* perform global setup for test */
	setup();

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count = 0;

		/* 
		 * Call sched_get_priority_min(2)
		 */
		TEST(sched_get_priority_min(SCHED_INVALID));
	
		if ((TEST_RETURN == -1) && (TEST_ERRNO == EINVAL)) {
			tst_resm(TPASS, "Test Passed, Got EINVAL");
		} else {
			tst_resm(TFAIL, "Test Failed, sched_get_priority_min()"
					" returned %d, errno = %d : %s",	
					TEST_RETURN, TEST_ERRNO,
					strerror(TEST_ERRNO));
		}
	}	/* End for TEST_LOOPING */

	/* cleanup and exit */
	cleanup();

	/*NOTREACHED*/
	return 0;

}	/* End main */

/* setup() - performs all ONE TIME setup for this test */
void 
setup()
{
	
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

}	/* End setup() */


/* 
 *cleanup() -  performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
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
