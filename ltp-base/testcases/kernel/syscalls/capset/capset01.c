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
 *    TEST IDENTIFIER	: capset01
 * 
 *    EXECUTED BY	: anyone
 * 
 *    TEST TITLE	: Basic test for capset(2)
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
 *	This is a Phase I test for the capset(2) system call.
 *	It is intended to provide a limited exposure of the system call.
 * 
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 *	  call capget() to save the current capability data
 * 
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  call capset() with the saved data
 *	  if return value == 0
 *		Test passed
 *	  Otherwise
 *		Test failed
 * 
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 * 
 * USAGE:  <for command-line>
 * capset01 [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-h] [-f] [-p]
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
 * CHANGES:
 *  2005/01/01: add an hint to a possible solution when test fails
 *              - Ricky Ng-Adam <rngadam@yahoo.com>
 ****************************************************************/
#include <unistd.h>
#include <errno.h>
#include "test.h"
#include "usctest.h"
/**************************************************************************/
/*                                                                        */
/*   Some archs do not have the manpage documented sys/capability.h file, */
/*   and require the use of the line below                                */

#include <linux/capability.h>

/*   If you are having issues with including this file and have the sys/  */
/*   version, then you may want to try switching to it. -Robbie W.        */
/**************************************************************************/


extern int capget(cap_user_header_t, cap_user_data_t);
extern int capset(cap_user_header_t, const cap_user_data_t);
static void setup();
static void cleanup();

char *TCID = "capset01";	/* Test program identifier.    */
int TST_TOTAL = 1;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */


static struct __user_cap_header_struct header; /* cap_user_header_t is a pointer
						  to __user_cap_header_struct */

static struct __user_cap_data_struct data; /* cap_user_data_t is a pointer to
					      __user_cap_data_struct */


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

		TEST(capset(&header, &data));

		if (TEST_RETURN == 0) {
			tst_resm(TPASS, "capset() returned %d", TEST_RETURN);
		} else {
			tst_resm(TFAIL, "Test Failed, capset()"
				 " returned %d, errno = %d : %s."
				 " Maybe you need to do `modprobe capability`?",
				 TEST_RETURN, TEST_ERRNO, strerror(TEST_ERRNO)
				 );
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

	/*
	 * Save current capability data.
	 * header.version must be _LINUX_CAPABILITY_VERSION
	 */
	header.version = _LINUX_CAPABILITY_VERSION;
	header.pid = 0;
	if ((capget(&header, &data)) == -1) {
		tst_brkm(TBROK, tst_exit, "capget() failed");
	}

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
	/* End cleanup() */
}
