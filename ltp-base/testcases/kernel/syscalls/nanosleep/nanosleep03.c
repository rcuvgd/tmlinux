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
 * Test Name: nanosleep03
 *
 * Test Description:
 *  Verify that nanosleep() will fail to suspend the execution
 *  of a process for a specified time if interrupted by a non-blocked signal.
 *
 * Expected Result:
 *  nanosleep() should return with -1 value and sets errno to EINTR.
 *
 * Algorithm:
 *  Setup:
 *   Setup signal handling.
 *   Pause for SIGUSR1 if option specified.
 *
 *  Test:
 *   Loop if the proper options are given.
 *   Execute system call
 *   Check return code, if system call failed (return=-1)
 *   	if errno set == expected errno
 *   		Issue sys call fails with expected return value and errno.
 *   	Otherwise,
 *		Issue sys call fails with unexpected errno.
 *   Otherwise,
 *	Issue sys call returns unexpected value.
 *
 *  Cleanup:
 *   Print errno log and/or timing stats if options given
 *
 * Usage:  <for command-line>
 *  nanosleep03 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *	       -i n : Execute test n times.
 *	       -I x : Execute test for x seconds.
 *	       -P x : Pause for x seconds between iterations.
 *	       -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS:
 *  None.
 */
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

#include "test.h"
#include "usctest.h"

char *TCID="nanosleep03";	/* Test program identifier.    */
int TST_TOTAL=1;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

struct timespec timereq;	/* time struct. buffer for nanosleep() */
struct timespec timerem;	/* time struct. buffer for nanosleep() */


int exp_enos[]={EINTR, 0};

void do_child();		/* Child process */
void setup();			/* Main setup function of test */
void cleanup();			/* cleanup function for the test */
void sig_handler();		/* signal catching function */

int
main(int ac, char **av)
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */
	pid_t cpid;		/* Child process id */
	int status;		/* child exit status */
    
	/* Parse standard options given to run the test. */
	msg = parse_opts(ac, av, (option_t *)NULL, NULL);
	if (msg != (char *)NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

#ifdef UCLINUX
	maybe_run_child(&do_child, "dddd", &timereq.tv_sec, &timereq.tv_nsec,
			&timerem.tv_sec, &timerem.tv_nsec);
#endif

	/* Perform global setup for test */
	setup();

	/* set the expected errnos... */
	TEST_EXP_ENOS(exp_enos);

	/* Check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* Reset Tst_count in case we are looping. */
		Tst_count=0;

		/*
		 * Creat a child process and suspend its
		 * execution using nanosleep()
		 */
		if ((cpid = FORK_OR_VFORK()) == -1) {
			tst_brkm(TBROK, cleanup, "fork() failed");
		}

		if (cpid == 0) {		/* Child process */
#ifdef UCLINUX
			if (self_exec(av[0], "dddd",
				      timereq.tv_sec, timereq.tv_nsec,
				      timerem.tv_sec, timerem.tv_nsec) < 0) {
				tst_brkm(TBROK, cleanup, "self_exec failed");
			}
#else
			do_child();
#endif
		}

		/* wait for child to time slot for execution */
		sleep(1);

		/* Now send signal to child */
		if (kill(cpid, SIGINT) < 0) {
			tst_brkm(TBROK, cleanup,
				 "kill() fails send signal to child");
		}

		/* Wait for child to execute */
		wait(&status);
		if (WEXITSTATUS(status) == 0) {
			tst_resm(TPASS, "nanosleep() fails, interrupted"
				 " by signal, error:%d", EINTR);
		} else if (WEXITSTATUS(status) == 1) {
			tst_resm(TFAIL, "child process exited abnormally");
		}
	}	/* End for TEST_LOOPING */

	/* Call cleanup() to undo setup done for the test. */
	cleanup();

	/*NOTREACHED*/	
	return(0);

}	/* End main */

/*
 * do_child()
 */
void
do_child()
{
	/* 
	 * Call nanosleep() to suspend child process
	 * for specified time 'tv_sec'.
	 * Call should return before suspending execution
	 * for the specified time due to receipt of signal
	 * from Parent.
	 */
	TEST(nanosleep(&timereq, &timerem));

	/* check return code of nanosleep() */
	if (TEST_RETURN == -1) {

		TEST_ERROR_LOG(TEST_ERRNO);

		/* Check for expected errno is set */
		if (TEST_ERRNO != EINTR) {
			tst_resm(TFAIL, "nanosleep() failed, "
				 "got errno:%d, expected errno"
				 ":%d", TEST_ERRNO, EINTR);
			exit(1);
		}
	} else {
		tst_resm(TFAIL, "nanosleep() returns %d, "
			 "expected -1, errno:%d",
			 TEST_RETURN, EINTR);
		exit(1);
	}

	/* Everything is fine, exit normally */
	exit(0);
}

/*
 * setup() - performs all ONE TIME setup for this test.
 *  Setup signal handler to catch the interrupt signal sent by parent
 *  to child process.
 *  Initialise time structure elements.
 */
void 
setup()
{
	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* Setup signal handler */
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		tst_brkm(TBROK, cleanup,
			 "signal() fails to setup signal handler");
	}

	/* Initialise time variables which used to suspend child execution */
	timereq.tv_sec = 5;
	timereq.tv_nsec = 9999;

}

/*
 * sig_handler() - signal catching function.
 *   This function gets executed when a parnet sends a signal 'SIGINT'
 *   to child to awake it from sleep. 
 *   This function just returns without doing anything.
 */
void
sig_handler()
{
}

/*
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
}
