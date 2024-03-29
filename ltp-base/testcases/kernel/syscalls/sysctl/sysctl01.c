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
 * NAME
 *	sysctl01.c
 *
 * DESCRIPTION
 *	Testcase for testing the basic functionality of sysctl(2) system call.
 *	This testcase attempts to read the kernel parameters using
 *	sysctl({CTL_KERN, KERN_* }, ...) and compares it with the known
 *	values.
 *
 * USAGE:  <for command-line>
 *  sysctl01 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS
 *	None
 */
#include "test.h"
#include "usctest.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <linux/version.h>
#include <sys/utsname.h>
#include <linux/unistd.h>
#include <linux/sysctl.h>

char *TCID = "sysctl01";
int TST_TOTAL = 3;
extern int Tst_count;

int sysctl(int *name, int nlen, void *oldval, size_t *oldlenp,
           void *newval, size_t newlen)
{
	struct __sysctl_args args={name,nlen,oldval,oldlenp,newval,newlen};
	return syscall(__NR__sysctl, &args);
}


#define SIZE(x) sizeof(x)/sizeof(x[0])

struct utsname buf;
char osname[BUFSIZ];
size_t osnamelth;

void setup(void);
void cleanup(void);

struct test_case_t {
	char *desc;
	int name[2];
	int size;
	void *oldval;
	size_t *oldlen;
	void *newval;
	int newlen;
	int (*cleanup)();
	int exp_retval;
} TC[] = {
	{ "Test for KERN_OSTYPE", { CTL_KERN, KERN_OSTYPE } , 2, osname,
		&osnamelth, NULL, 0, NULL, 0 },
	{ "Test for KERN_OSRELEASE", { CTL_KERN, KERN_OSRELEASE }, 2,
		osname, &osnamelth, NULL, 0, NULL, 0 },
	{ "Test for KERN_VERSION", { CTL_KERN, KERN_VERSION }, 2,
		osname, &osnamelth, NULL, 0, NULL, 0 }
};

int main(int ac, char **av)
{
	int lc;
	char *msg;
	int i;
	char *comp_string;

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	}

	setup();

	/* check looping state if -i option is given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		for (i = 0; i < TST_TOTAL; ++i) {

			osnamelth = SIZE(osname);

			switch (i) {
			case 0:
				comp_string = buf.sysname;
				break;
			case 1:
				comp_string = buf.release;
				break;
			case 2:
				comp_string = buf.version;
				break;
			}

			TEST(sysctl(TC[i].name, TC[i].size, TC[i].oldval,
					  TC[i].oldlen, TC[i].newval,
					  TC[i].newlen));

			if (TEST_RETURN != 0) {
				tst_resm(TFAIL, "sysctl(2) failed unexpectedly "
					 "errno:%d", errno);
				continue;
			}

			if (!STD_FUNCTIONAL_TEST) {
				tst_resm(TPASS, "call succeeded");
				continue;
			}

			if (strcmp(TC[i].oldval, comp_string) != 0) {
				tst_resm(TFAIL, "strings don't match - %s : %s",
					 TC[i].oldval, comp_string);
			} else {
				tst_resm(TPASS, "%s is correct", TC[i].desc);
			}
			if (TC[i].cleanup) {
				(void)TC[i].cleanup();
			}
		}
	}
	cleanup();

	/*NOTREACHED*/
	return(0);
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void
setup()
{
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* get kernel name and information */
	if (uname(&buf) == -1) {
		tst_brkm(TBROK, cleanup, "uname() failed");
	}
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
