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
 *    TEST IDENTIFIER	: getdomainname01 
 * 
 *    EXECUTED BY	: anyone
 * 
 *    TEST TITLE	: Basic test for getdomainname(2)
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
 *	This is a Phase I test for the getdomainname(2) system call.
 *	It is intended to provide a limited exposure of the system call.
 * 
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 * 
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  Execute system call
 *	  Check return code, if system call failed (return=-1)
 *		Log the errno and Issue a FAIL message.
 *	  Otherwise, Issue a PASS message.
 * 
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 * 
 * USAGE:  <for command-line>
 *  getdomainname01  [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-h] [-f] [-p]
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
#include <linux/utsname.h>
#include "test.h"
#include "usctest.h"

#define MAX_NAME_LEN __NEW_UTS_LEN

static void setup();
static void cleanup();

char *TCID = "getdomainname01";	/* Test program identifier.    */
int TST_TOTAL = 1;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

static char domain_name[MAX_NAME_LEN];

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

	/* check looping state if -c option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count = 0;

		/* 
		 * Call getdomainname(2)
		 */
		TEST(getdomainname(domain_name, sizeof(domain_name)));
	
		/* check return code */
		if (TEST_RETURN == -1) {
			tst_resm(TFAIL, "getdomainname() Failed, errno = %d :"
				" %s", TEST_ERRNO, strerror(TEST_ERRNO));
		} else {
			tst_resm(TPASS, "getdomainname() returned %d ",
				 TEST_RETURN);
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
