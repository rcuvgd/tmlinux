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
 * Test Name: fchmod05
 * 
 * Test Description:
 *  Verify that, fchmod(2) will succeed to change the mode of a directory
 *  but fails to set the setgid bit on it if invoked by non-root (uid != 0)
 *  process with the following constraints,
 *	- the process is the owner of the directory.
 *	- the effective group ID or one of the supplementary group ID's of the
 *	  process is not equal to the group ID of the directory.
 *
 * Expected Result:
 *  fchmod() should return value 0 on success and though succeeds to change
 *  the mode of a directory but fails to set setgid bit on it.
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
 *  fchmod05 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *	       -i n : Execute test n times.
 *	       -I x : Execute test for x seconds.
 *	       -P x : Pause for x seconds between iterations.
 *	       -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS:
 *  This test should be run by 'non-super-user' only.
 *
 */

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

#include "test.h"
#include "usctest.h"

#define MODE_RWX 	S_IRWXU | S_IRWXG | S_IRWXO
#define PERMS		043777
#define TESTDIR		"testdir"

int fd;				/* file descriptor for test directory */
char *TCID="fchmod05"; 		/* Test program identifier.    */
int TST_TOTAL=1;    		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

void setup();			/* Main setup function for test */
void cleanup();			/* Main cleanup function for test */

int
main(int ac, char **av)
{
	struct stat stat_buf;	/* stat struct */
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */
	mode_t dir_mode;	/* mode permissions set on test directory */
    
	/* Parse standard options given to run the test. */
	msg = parse_opts(ac, av, (option_t *) NULL, NULL);
	if (msg != (char *) NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
		tst_exit();
	}

	/* Perform global setup for test */
	setup();

	/* Check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* Reset Tst_count in case we are looping. */
		Tst_count = 0;

		/* 
	 	 * Call fchmod(2) with mode argument 
		 * to set setgid bit on TESTDIR.
	 	 */

		TEST(fchmod(fd, PERMS));
	
		/* check return code of fchmod(2) */
		if (TEST_RETURN == -1) {
			tst_resm(TFAIL, "fchmod(%d, %#o) Failed, errno=%d : %s",
				 fd, PERMS, TEST_ERRNO, strerror(TEST_ERRNO));
			continue;
		}
		/*
		 * Perform functional verification if test
		 * executed without (-f) option.
		 */
		if (STD_FUNCTIONAL_TEST) {
			/*
		 	 * Get the directory information using
			 * fstat(2).
			 */
			if (fstat(fd, &stat_buf) < 0) {
				 tst_brkm(TFAIL, cleanup,
					  "fstat(2) of %s failed, errno:%d",
					  TESTDIR, TEST_ERRNO);
			}
			dir_mode = stat_buf.st_mode;
			if (PERMS != dir_mode) {
				tst_resm(TFAIL, "%s: Incorrect modes 0%03o, "
					 "Expected 0%03o",
					 TESTDIR, PERMS, MODE_RWX);
			} else {
				tst_resm(TPASS, "Functionality of fchmod(%d, "
					 "%#o) successful", fd, PERMS);
			}
		} else {
			tst_resm(TPASS, "call succeeded");
		}
	}	/* End for TEST_LOOPING */

	/* Call cleanup() to undo setup done for the test. */
	cleanup();

	/*NOTREACHED*/
	return(0);
}	/* End main */

/*
 * void
 * setup() - performs all ONE TIME setup for this test.
 *  Create a temporary directory and cd to it.
 *  Create a test directory under temporary directory.
 *  Invoke setuid to root program to modify group ownership 
 *  on test directory.
 *  Open the test directory for reading.
 */
void 
setup()
{
	char *test_home;		/* variable to hold TESTHOME env */
	static char Path_name[PATH_MAX];	/* Buffer to hold command string */
	static char Cmd_buffer[BUFSIZ];        /* Buffer to hold command string */

	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	/* Check that the test process id is not super/root  */
	if (geteuid() != 0) {
		tst_brkm(TBROK, NULL, "Must be super/root for this test!");
		tst_exit();
	}

	test_home = get_current_dir_name();

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temp directory and cd to it */
	tst_tmpdir();

	/*
	 * Create a test directory under temporary directory with specified
	 * mode permissions and change the gid of test directory to that of
	 * guest user.
	 */
	if (mkdir(TESTDIR, MODE_RWX) < 0) {
		tst_brkm(TBROK, cleanup, "mkdir(2) of %s failed", TESTDIR);
	}

	/* Get the current working directory of the process */
	if (getcwd(Path_name, sizeof(Path_name)) == NULL) {
		tst_brkm(TBROK, cleanup,
			 "getcwd(3) fails to get working directory of process");
	}

	/*
	 * Get the complete path of TESTDIR created
	 * under temporary directory
	 */
	strcat(Path_name, "/"TESTDIR);

	/* Get the command name to be executed as setuid to root */
	strcpy((char *)Cmd_buffer, (const char *)test_home);
	strcat((char *)Cmd_buffer, (const char *)"/change_owner ");
	strcat((char *)Cmd_buffer, TCID);
	strcat((char *)Cmd_buffer, " ");
	strcat((char *)Cmd_buffer, Path_name);

	if (system((const char *)Cmd_buffer) != 0) {
		tst_brkm(TBROK, cleanup,
			 "Fail to modify %s group ownerships", TESTDIR);
	}

	/* Open the test directory for reading */
	if ((fd = open(TESTDIR, O_RDONLY)) == -1) {
		tst_brkm(TBROK, cleanup,
			 "open(%s, O_RDONLY) failed, errno=%d : %s",
			 TESTDIR, errno, strerror(errno));
	}
}	/* End setup() */


/*
 * void
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 *  Close the test directory opened in the setup().
 *  Remove the test directory and temporary directory created in
 *  in the setup().
 */
void 
cleanup()
{
	/*
	 * print timing stats if that option was specified.
	 */
	TEST_CLEANUP;

	/* Close the test directory opened in the setup() */
	if (close(fd) == -1) {
		tst_brkm(TBROK, NULL,
			 "close(%s) Failed, errno=%d : %s",
			 TESTDIR, errno, strerror(errno));
	}

	/* Remove tmp dir and all files in it */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}	/* End cleanup() */
