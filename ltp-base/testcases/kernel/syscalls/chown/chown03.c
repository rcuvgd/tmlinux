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
 * Test Name: chown03
 *
 * Test Description:
 *  Verify that, chown(2) succeeds to change the group of a file specified
 *  by path when called by non-root user with the following constraints,
 *	- euid of the process is equal to the owner of the file.	 
 *	- the intended gid is either egid, or one of the supplementary gids
 *	  of the process.
 *  Also, verify that chown() clears the setuid/setgid bits set on the file.
 *
 * Expected Result:
 *  chown(2) should return 0 and the ownership set on the file should match 
 *  the numeric values contained in owner and group respectively.
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
 *  chown03 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <grp.h>
#include <pwd.h>

#include "test.h"
#include "usctest.h"

#define FILE_MODE	S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
#define NEW_PERMS       S_IFREG | S_IRWXU | S_IRWXG | S_ISUID | S_ISGID
#define TESTFILE	"testfile"

char *TCID="chown03";		/* Test program identifier.    */
int TST_TOTAL=1;		/* Total number of test conditions */
extern int Tst_count;		/* Test Case counter for tst_* routines */
char nobody_uid[] = "nobody";
struct passwd *ltpuser;

void setup();			/* setup function for the test */
void cleanup();			/* cleanup function for the test */

int
main(int ac, char **av)
{
	struct stat stat_buf;	/* stat(2) struct contents */
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */
	uid_t User_id;		/* Owner id of the test file. */
	gid_t Group_id;		/* Group id of the test file. */

	STD_FUNCTIONAL_TEST=0;
 
	/* Parse standard options given to run the test. */
	msg = parse_opts(ac, av, (option_t *) NULL, NULL);
	if (msg != (char *) NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
		tst_exit();
	}

	/* Perform global setup for test */
	setup();

	/* Check looping state if -c option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* Reset Tst_count in case we are looping. */
		Tst_count = 0;

		/* Get the euid/egid of the process */
		User_id = geteuid();
		Group_id = getegid();

		/* 
		 * Call chown(2) with different user id and
		 * group id (numeric values) to set it on testfile.
		 */

		seteuid(0);
		setegid(0);
		TEST(chown(TESTFILE, -1, Group_id));

		/* check return code of chown(2) */
		if (TEST_RETURN == -1) {
			tst_resm(TFAIL, "chown() on %s Fails, errno=%d",
				 TESTFILE, TEST_ERRNO);
			continue;
		}

		/*
		 * Perform functional verification if test
		 * executed without (-f) option.
		 */
		if (STD_FUNCTIONAL_TEST) {
			/*
		 	 * Get the testfile information using stat(2).
			 */
			if (stat(TESTFILE, &stat_buf) < 0) {
				tst_brkm(TFAIL, cleanup, "stat(2) of %s failed,"
					 " errno:%d", TESTFILE, TEST_ERRNO);
			}

			/*
			 * Check for expected Ownership ids set on testfile.
			 */
			if ((stat_buf.st_uid != User_id) ||
				    (stat_buf.st_gid != Group_id)) {
				tst_resm(TFAIL, "%s: Incorrect "
					 "ownership set to %d %d, Expected %d %d",
					 TESTFILE, stat_buf.st_uid, stat_buf.st_gid, User_id, Group_id);
			}

			/*
			 * Verify that setuid/setgid bits set on the
			 * testfile in setup() are cleared by chown()
			 */
			if (stat_buf.st_mode & (S_ISUID | S_ISGID)) {
				tst_resm(TFAIL, "%s: Incorrect mode permissions"
					 " %#o, Expected %#o", TESTFILE,
					 stat_buf.st_mode, NEW_PERMS);
			} else {
				tst_resm(TPASS, "chown() on %s succeeds, "
					 "clears setuid/gid bits", TESTFILE);
			}
		} else {
			tst_resm(TPASS, "call succeeded");
		}
	}	/* End for TEST_LOOPING */

	/* Call cleanup() to undo setup done for the test. */
	cleanup();

	return 0;
	/*NOTREACHED*/
}	/* End main */

/*
 * void
 * setup() - performs all ONE TIME setup for this test.
 *  Create a temporary directory and change directory to it.
 *  Create a test file under temporary directory and close it
 *  Change the group ownership on testfile.
 */
void 
setup()
{
	static char test_home[PATH_MAX];	/* variable to hold TESTHOME env */
	int fd;				/* file handler for testfile */
	static char Path_name[PATH_MAX];       /* Buffer to hold command string */
	static char Cmd_buffer[BUFSIZ];        /* Buffer to hold command string */
	char *change_owner_path;

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temp directory and cd to it */
	tst_tmpdir();

	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	/* Check that the test process id is super/root  */
	if (geteuid() != 0) {
		tst_brkm(TBROK, cleanup,
			 "Must be super/root for this test!");
		tst_exit();
	}

	ltpuser = getpwnam(nobody_uid);
         if (setegid(ltpuser->pw_uid) == -1) {
                tst_resm(TINFO, "setegid failed to "
                         "to set the effective uid to %d",
                         ltpuser->pw_uid);
                perror("setegid");
         }
         if (seteuid(ltpuser->pw_uid) == -1) {
                tst_resm(TINFO, "seteuid failed to "
                         "to set the effective uid to %d",
                         ltpuser->pw_uid);
                perror("seteuid");
         }


	if (getcwd(test_home, sizeof(test_home)) == NULL) {
		tst_brkm(TBROK, cleanup,
			"getcwd(3) fails to get working directory of process");
        }

	/* Create a test file under temporary directory */
	if ((fd = open(TESTFILE, O_RDWR|O_CREAT, FILE_MODE)) == -1) {
		tst_brkm(TBROK, cleanup,
			 "open(%s, O_RDWR|O_CREAT, %o) Failed, errno=%d : %s",
			 TESTFILE, FILE_MODE, errno, strerror(errno));
	}
	/* Close the test file created above */
	if (close(fd) == -1) {
		tst_brkm(TBROK, cleanup,
			 "close(%s) Failed, errno=%d : %s",
			 TESTFILE, errno, strerror(errno));
	}


	/*
	 * Change mode permissions on testfile such that 
	 * setuid/setgid bits are set on the testfile.
	 */
	if (chmod(TESTFILE, NEW_PERMS) < 0) {
		tst_brkm(TBROK, cleanup,
			 "chmod(2) on %s Failed, errno=%d : %s",
			 TESTFILE, errno, strerror(errno));
	}


	/* Get the current working directory of the process */
	if (getcwd(Path_name, sizeof(Path_name)) == NULL) {
		tst_brkm(TBROK, cleanup,
			 "getcwd(3) fails to get working directory of process");
	}
	/* Get the path of TESTFILE under temporary directory */
	strcat(Path_name, "/"TESTFILE);

	/* Set the environment variable change_owner if not already set */
	setenv("change_owner",strcat(test_home,"/change_owner"),0);

	/* Get the command name to be executed as setuid to root */
	if ((change_owner_path = getenv("change_owner")) == NULL) {
	    tst_brkm(TBROK, cleanup,
	    "getenv() failed to get the cmd to be execd as setuid to root");
        }

	strcpy((char *)Cmd_buffer, (const char *)change_owner_path);
	strcat((char *)Cmd_buffer, " ");
	strcat((char *)Cmd_buffer, TCID);
	strcat((char *)Cmd_buffer, " ");
	strcat((char *)Cmd_buffer, Path_name);

	

	if (system((const char *)Cmd_buffer) != 0) {
		tst_brkm(TBROK, cleanup,
			 "Fail to modify Ownership of %s", TESTFILE);
	}
}	/* End setup() */

/*
 * void
 * cleanup() - performs all ONE TIME cleanup for this test at
 *	       completion or premature exit.
 *  Remove the test directory and testfile created in the setup.
 */
void 
cleanup()
{
	/*
	 * print timing stats if that option was specified.
	 */
	TEST_CLEANUP;

	/* Remove tmp dir and all files in it */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}	/* End cleanup() */
