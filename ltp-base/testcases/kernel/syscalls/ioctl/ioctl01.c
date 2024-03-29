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
 *	ioctl01.c
 *
 * DESCRIPTION
 *	Testcase to check the errnos set by the ioctl(2) system call.
 *
 * ALGORITHM
 *	1. EBADF: Pass an invalid fd to ioctl(fd, ..) and expect EBADF.
 *	2. EFAULT: Pass an invalid address of arg in ioctl(fd, .., arg)
 *	3. EINVAL: Pass invalid cmd in ioctl(fd, cmd, arg)
 *	4. ENOTTY: Pass an non-streams fd in ioctl(fd, cmd, arg)
 *	5. EFAULT: Pass a NULL address for termio
 *
 * USAGE:  <for command-line>
 *  ioctl01 -D /dev/tty[0-9] [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *	04/2002 Fixes by wjhuie
 *
 * RESTRICTIONS
 *      test must be run with the -D option
 *      test may have to be run as root depending on the tty permissions
 */

#include <stdio.h>
#include <sys/termios.h>
#include <termio.h>
#include <errno.h>
#include <fcntl.h>
#include <test.h>
#include <usctest.h>

char *TCID = "ioctl01";
int TST_TOTAL = 5;
extern int Tst_count;

#define	INVAL_IOCTL	9999999

void setup(void);
void cleanup(void);
void help(void);

int exp_enos[] = {EBADF, EFAULT, EINVAL, ENOTTY, EFAULT, 0};

int fd, fd1;
int bfd = -1;

char *tty;
struct termio termio;

struct test_case_t {
        int *fd;
        int request;
        struct termio *s_tio;
        int error;
} TC[] = {
	/* file descriptor is invalid */
        {&bfd, TCGETA, &termio, EBADF},

	/* termio address is invalid */
        {&fd, TCGETA, (struct termio *)-1, EFAULT},

	/* command is invalid */
        {&fd, INVAL_IOCTL, &termio, EINVAL},

	/* file descriptor is for a regular file */
        {&fd1, TCGETA, &termio, ENOTTY},

	/* termio is NULL */
        {&fd, TCGETA, NULL, EFAULT}
};

int Devflag = 0;
char *devname;

/* for test specific parse_opts options - in this case "-D" */
option_t options[] = {
	{"D:", &Devflag, &devname},
	{NULL, NULL, NULL}
};

int main(int ac, char **av)
{
	int lc;				/* loop counter */
	int i;
	char *msg;			/* message returned from parse_opts */
	
	/* parse standard options */
	if ((msg = parse_opts(ac, av, options, &help)) != (char *)NULL) {
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
	}

	if (!Devflag) {
		tst_resm(TWARN, "You must specify a tty device with "
			 "the -D option.");
		tst_resm(TWARN, "Run '%s -h' for option information.", TCID);
		cleanup();
        }

        if (geteuid() != 0) { 	 
	                 tst_brkm(TBROK, tst_exit, "Test must be run as root"); 	 
	}

	setup();

	if ((fd = open(devname, O_RDWR, 0777)) < 0) {
		tst_brkm(TBROK, cleanup, "Couldn't open %s, errno = %d",
			 tty, errno);
	}

	TEST_EXP_ENOS(exp_enos);

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* reset Tst_count in case we are looping. */
		Tst_count = 0;

		/* loop through the test cases */
		for (i = 0; i < TST_TOTAL; i++) {

			TEST(ioctl(*(TC[i].fd), TC[i].request, TC[i].s_tio));

                        if (TEST_RETURN != -1) {
                                tst_resm(TFAIL, "call succeeded unexpectedly");
                                continue;
                        }

                        TEST_ERROR_LOG(TEST_ERRNO);

                        if (TEST_ERRNO == TC[i].error) {
                                tst_resm(TPASS, "expected failure - "
                                         "errno = %d : %s", TEST_ERRNO,
                                         strerror(TEST_ERRNO));
                        } else {
                                tst_resm(TFAIL, "unexpected error - %d : %s - "
                                         "expected %d", TEST_ERRNO,
                                         strerror(TEST_ERRNO), TC[i].error);
			}
		}
	}
	cleanup();

	/*NOTREACHED*/
	return(0);
}

/*
 * help() - Prints out the help message for the -D option defined
 *          by this test.
 */
void
help()
{
	printf("  -D <tty device> : for example, /dev/tty[0-9]\n");
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

	/* make a temporary directory and cd to it */
	tst_tmpdir();

	/* create a temporary file */
	if ((fd1 = open("x", O_CREAT, 0777)) < 0) {
		tst_resm(TFAIL, "Could not open test file, errno = %d", errno);
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

	close(fd1);

	/* delete the test directory created in setup() */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}

