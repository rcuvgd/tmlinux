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
 * Test Name: sigsuspend01
 *
 * Test Description:
 *  Verify that sigsuspend() succeeds to change process's current signal
 *  mask with the specified signal mask and suspends the process execution
 *  until the delivery of a signal.
 *
 * Expected Result:
 *  sigsuspend() should return after the execution of signal catching
 *  function and the previous signal mask should be restored.
 *
 * Algorithm:
 *  Setup:
 *   Setup signal handling.
 *   Create temporary directory.
 *   Pause for SIGUSR1 if option specified.
 *
 *  Test:
 *   Loop if the proper options are given.
 *   Execute system call
 *   Check return code, if system call failed (return=-1)
 *   	Log the errno and Issue a FAIL message.
 *   Otherwise,
 *   	Verify the Functionality of system call	
 *      if successful,
 *      	Issue Functionality-Pass message.
 *      Otherwise,
 *		Issue Functionality-Fail message.
 *  Cleanup:
 *   Print errno log and/or timing stats if options given
 *   Delete the temporary directory created.
 *
 * Usage:  <for command-line>
 *  sigsuspend01 [-c n] [-e] [-f] [-i n] [-I x] [-p x] [-t]
 *	where,  -c n : Run n copies concurrently.
 *		-e   : Turn on errno logging.
 *		-f   : Turn off functionality Testing.
 *		-i n : Execute test n times.
 *		-I x : Execute test for x seconds.
 *		-P x : Pause for x seconds between iterations.
 *		-t   : Turn on syscall timing.
 *
 * History
 *	07/2001 John George
 *		-Ported
 *
 * Restrictions:
 *  None.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>

#include "test.h"
#include "usctest.h"

char *TCID="sigsuspend01";	/* Test program identifier.    */
int TST_TOTAL=1;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */
int exp_enos[]={EINTR, 0};

struct sigaction sa_new;	/* struct to hold signal info */
sigset_t sigset;		/* signal set to hold signal lists */
sigset_t sigset1;
sigset_t sigset2;

void setup();			/* Main setup function of test */
void cleanup();			/* cleanup function for the test */
void sig_handler(int sig);	/* signal catching function */

int
main(int ac, char **av)
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */

	/* Parse standard options given to run the test. */
	msg = parse_opts(ac, av, (option_t *) NULL, NULL);
	if (msg != (char *) NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
		tst_exit();
	}

	/* Perform global setup for test */
	setup();

	/* set the expected errnos... */
	TEST_EXP_ENOS(exp_enos);

	/* Check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* Reset Tst_count in case we are looping. */
		Tst_count=0;

		/* Set the alarm timer */
		alarm(5);

		/* 
		 * Call sigsuspend() to replace current signal mask
		 * of the process and suspend process execution till
		 * receipt of a signal 'SIGALRM'.
		 */
		TEST(sigsuspend(&sigset));

		/* Reset the alarm timer */
		alarm(0);

		/* check return code of sigsuspend() */
		if ((TEST_RETURN == -1) && (TEST_ERRNO == EINTR)) {
			TEST_ERROR_LOG(TEST_ERRNO);
			/*
			 * Perform functional verification if test
			 * executed without (-f) option.
			 */
			if (STD_FUNCTIONAL_TEST) {
				/* 
				 * Read the current signal mask of process,
				 * Check whether previous signal mask preserved
				 */
				if (sigprocmask(SIG_UNBLOCK, 0, &sigset2)
								== -1) {
					tst_resm(TFAIL, "sigprocmask() Failed "
						"to get previous signal mask "
						"of process");
				} else if (sigset2.__val[0] != 
						sigset1.__val[0]) {
					tst_resm(TFAIL, "sigsuspend failed to "
						"preserve signal mask");
				} else {
					tst_resm(TPASS, "Functionality of "
						"sigsuspend() successful");
				}
			} else {
				tst_resm(TPASS,
					"Received expected return value.");
			}
		} else {
			tst_resm(TFAIL,
				 "sigsuspend() returned value %d, error:%d",
				 TEST_RETURN, TEST_ERRNO);
		}

		Tst_count++;			/* incr TEST_LOOP counter */
	}	/* End for TEST_LOOPING */

	/* Call cleanup() to undo setup done for the test. */
	cleanup();
	/*NOTREACHED*/


  return(0);

}	/* End main */

/*
 * void
 * setup() - performs all ONE TIME setup for this test.
 * Initialise signal set with the list that includes/excludes
 * all system-defined signals.
 * Set the signal handler to catch SIGALRM signal.
 * Get the current signal mask of test process using sigprocmask().
 */
void 
setup()
{
	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/*
	 * Initialise the signal sets with the list that
	 * excludes/includes  all system-defined signals.
	 */
	if (sigemptyset(&sigset) == -1) {
		tst_brkm(TFAIL, cleanup, 
			 "sigemptyset() failed, errno=%d : %s",
			 errno, strerror(errno));
		/*NOTREACHED*/
	}
	if (sigfillset(&sigset2) == -1) {
		tst_brkm(TFAIL, cleanup,
			 "sigfillset() failed, errno=%d : %s",
			 errno, strerror(errno));
		/*NOTREACHED*/
	}

	/* Set the signal handler function to catch the signal */
	sa_new.sa_handler = sig_handler;
	if (sigaction(SIGALRM, &sa_new, 0) == -1) {
		tst_brkm(TFAIL, cleanup,
			 "sigaction() failed, errno=%d : %s",
			 errno, strerror(errno));
		/*NOTREACHED*/
	}

	/* Read the test process's current signal mask. */
	if (sigprocmask(SIG_UNBLOCK, 0, &sigset1) == -1) {
		tst_brkm(TFAIL, cleanup,
			 "sigprocmask() Failed, errno=%d : %s",
			 errno, strerror(errno));
		/*NOTREACHED*/
	}
}	/* End setup() */

/*
 * void
 * sig_handler(int sig) - Signal catching function.
 *   This function gets executed when the signal SIGALRM is delivered
 *   to the test process after the expiry of alarm time and the signal was 
 *   trapped by sigaction() to execute this function.
 *
 *   This function simply returns without doing anything.
 */
void
sig_handler(int sig)
{
}

/*
 * void
 * cleanup() - performs all ONE TIME cleanup for this test at
 *             completion or premature exit.
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
