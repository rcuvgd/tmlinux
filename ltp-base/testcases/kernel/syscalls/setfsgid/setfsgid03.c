/*
 *
 *   Copyright (C) International Business Machines  Corp., 2001
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
 * NAME
 *	setfsgid03.c
 *
 * DESCRIPTION
 *	Testcase to check the basic functionality of setfsgid(2) system
 *	call fails when called by a non-root user.
 *
 * ALGORITHM
 *	Call setfsgid() and test the return value.
 *
 * USAGE:  <for command-line>
 *  setfsgid01 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *      04/2003 Adapted by Dustin Kirkland (k1rkland@us.ibm.com)
 *
 * RESTRICTIONS
 *	None
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#ifdef __GLIBC__
#include <sys/fsuid.h>
#endif
#include "test.h"
#include "usctest.h"

char *TCID = "setfsgid03";
int TST_TOTAL = 1;
extern int Tst_count;
char nobody_uid[] = "nobody";
struct passwd *ltpuser;

void setup(void);
void cleanup(void);

int main(int ac, char **av)
{
	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */

	gid_t gid;
	
	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	setup();

	/* Check for looping state if -i option is given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* reset Tst_count in case we are looping */
		Tst_count = 0;

                gid = 1;
                while (!getgrgid(gid)) {
                  gid++;
                }

		TEST(setfsgid(gid));

		if (TEST_RETURN == -1) {
			tst_resm(TFAIL, "call failed unexpectedly - errno %d",
				 TEST_ERRNO);
			continue;
		}

		if (!STD_FUNCTIONAL_TEST) {
			tst_resm(TPASS, "call succeeded");
			continue;
		}

		if (TEST_RETURN == gid) {
			tst_resm(TFAIL, "setfsgid() returned %d, expeceted anything but %d",
				 TEST_RETURN, gid);
		} else {
			tst_resm(TPASS, "setfsgid() returned expected value : "
				 "%d", TEST_RETURN);
		}
	}
	cleanup();

	/*NOTREACHED*/
	return EXIT_SUCCESS;
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void
setup()
{
        /* Switch to nobody user for correct error code collection */
        if (geteuid() != 0) {
                tst_brkm(TBROK, tst_exit, "Test must be run as root");
        }
         ltpuser = getpwnam(nobody_uid);
         if (setuid(ltpuser->pw_uid) == -1) {
                tst_resm(TINFO, "setuid failed to "
                         "to set the effective uid to %d",
                         ltpuser->pw_uid);
                perror("setuid");
         }

	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;
}

/*
 * cleanup() - performs all ONE TIME cleanup for this test at
 *	       completion or premature exit.
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
