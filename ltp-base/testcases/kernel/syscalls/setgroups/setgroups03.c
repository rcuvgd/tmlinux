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
 * Test Name: setgroups03
 *
 * Test Description:
 *  Verify that,
 *   1. setgroups() fails with -1 and sets errno to EINVAL if the size
 *      argument value is > NGROUPS
 *   2. setgroups() fails with -1 and sets errno to EPERM if the
 *	calling process is not super-user.
 *
 * Expected Result:
 *  setgroups() should fail with return value -1 and set expected errno.
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
 *  setgroups03 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
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
 *  This test should be executed by 'non-super-user' only.
 *
 */
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

#include "test.h"
#include "usctest.h"

#include "compat_16.h"

#define TESTUSER	"nobody"

char nobody_uid[] = "nobody";
struct passwd *ltpuser;


TCID_DEFINE(setgroups03);	/* Test program identifier.    */
int TST_TOTAL=2;		/* Total number of test conditions */
extern int Tst_count;		/* Test Case counter for tst_* routines */

int exp_enos[] = {EINVAL, EPERM, 0};

GID_T *groups_list; 		/* Array to hold gids for getgroups() */
	
int setup1();			/* setup function to test error EPERM */
void setup();			/* setup function for the test */
void cleanup();			/* cleanup function for the test */

struct test_case_t {		/* test case struct. to hold ref. test cond's*/
	size_t gsize_add;
	int list;
	char *desc;
	int exp_errno;
	int (*setupfunc)();
} Test_cases[] = {
	{1, 1, "Size is > sysconf(_SC_NGROUPS_MAX)", EINVAL, NULL},
	{0, 2, "Permission denied, not super-user", EPERM, setup1}
};

int
main(int ac, char **av)
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */
	int gidsetsize;		/* total no. of groups */
	int i;			/* counter to test different test conditions */
	char *test_desc;        /* test specific error message */
	int ngroups_max = sysconf(_SC_NGROUPS_MAX);	/* max no. of groups in the current system */

	/* Parse standard options given to run the test. */
	msg = parse_opts(ac, av, (option_t *)NULL, NULL);
	if (msg != (char *)NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}
	
	groups_list = malloc(ngroups_max * sizeof(GID_T));
	if (groups_list == NULL) {
		tst_brkm(TBROK, NULL, "malloc failed to alloc %d errno "
			 " %d ", ngroups_max * sizeof(GID_T), errno);
	}

	/* Perform global setup for test */
	setup();

	/* set the expected errnos... */
	TEST_EXP_ENOS(exp_enos);

	/* Check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) { 

		/* Reset Tst_count in case we are looping. */
		Tst_count = 0;

		for (i = 0; i < TST_TOTAL; i++) {
			if(Test_cases[i].setupfunc != NULL) {
				Test_cases[i].setupfunc();
			}

			gidsetsize = ngroups_max + Test_cases[i].gsize_add;
			test_desc = Test_cases[i].desc;
			
			/*
			 * Call setgroups() to test different test conditions
			 * verify that it fails with -1 return value and
			 * sets appropriate errno.
			 */ 
			 TEST(SETGROUPS(gidsetsize, groups_list));
	
			/* check return code of setgroups(2) */
			if (TEST_RETURN != -1) {
				tst_resm(TFAIL, "setgroups(%d) returned %d, "
					 "expected -1, errno=%d", gidsetsize, TEST_RETURN,
					 Test_cases[i].exp_errno);
				continue;
			}

			TEST_ERROR_LOG(TEST_ERRNO);

			if (TEST_ERRNO == Test_cases[i].exp_errno) {
				tst_resm(TPASS,
					 "setgroups(%d) fails, %s, errno=%d", gidsetsize,
					 test_desc, TEST_ERRNO);
			} else {
				tst_resm(TFAIL, "setgroups(%d) fails, %s, "
					 "errno=%d, expected errno=%d", gidsetsize,
					 test_desc, TEST_ERRNO,
					 Test_cases[i].exp_errno);
			}
		}	/* End of TEST CASE LOOPING. */

	}	/* End for TEST_LOOPING */

	/* Call cleanup() to undo setup done for the test. */
	cleanup();

	/*NOTREACHED*/
	return(0);
}

/*
 * setup() - performs all ONE TIME setup for this test.
 *
 *  Call individual test specific setup functions.
 */
void 
setup()
{

        if (geteuid() != 0) {
                tst_brkm(TBROK, tst_exit, "Test must be run as root");
        }

	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

}

/*
 * setup1 -  Setup function to test setgroups() which returns -1
 *	     and sets errno to EPERM.
 *
 *  Get the user info. from /etc/passwd file.
 *  This function returns 0 on success.
 */
int
setup1()
{
	struct passwd *user_info;	/* struct. to hold test user info */

/* Switch to nobody user for correct error code collection */
         ltpuser = getpwnam(nobody_uid);
         if (seteuid(ltpuser->pw_uid) == -1) {
                tst_resm(TINFO, "setreuid failed to "
                         "to set the effective uid to %d",
                         ltpuser->pw_uid);
                perror("setreuid");
         }

	if ((user_info = getpwnam(TESTUSER)) == NULL) {
		tst_brkm(TFAIL, cleanup, "getpwnam(2) of %s Failed", TESTUSER);
	}

	if (!GID_SIZE_CHECK(user_info->pw_gid)) {
		tst_brkm(TBROK,
			 cleanup,
			 "gid returned from getpwnam is too large for testing setgroups16");
	}
	groups_list[0] = user_info->pw_gid;
	return 0;
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
	 */
	TEST_CLEANUP;

	/* exit with return code appropriate for results */
	tst_exit();
}
