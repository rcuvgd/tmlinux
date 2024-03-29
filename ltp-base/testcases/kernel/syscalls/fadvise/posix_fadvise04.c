/*
 *
 *   Copyright (c) Red Hat Inc., 2007
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
 *	posix_fadvise04.c
 *
 * DESCRIPTION
 *	Check the value that posix_fadvise returns for pipe descriptor.
 *
 * USAGE
 * 	posix_fadvise04
 *
 * HISTORY
 *	11/2007 Initial version by Masatake YAMATO <yamato@redhat.com>
 *
 * RESTRICTIONS
 * 	None
 */


#define _XOPEN_SOURCE 600
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 32
#endif

#ifndef __NR_fadvise64
#define __NR_fadvise64 0
#endif

void setup();
void cleanup();


TCID_DEFINE(posix_fadvise04);	/* Test program identifier.    */
extern int Tst_count;		/* Test Case counter for tst_* routines */

#define GIVEN_IN_SETUP 42	/* No mean. Just used as padding.
				   This is overwritten by setup(). */

struct test_case_t {
	int   fd;
	off_t offset;
	off_t len;
	int   advice;
	int   error;
} TC[] = {
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_NORMAL,     ESPIPE},
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_SEQUENTIAL, ESPIPE},
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_RANDOM,     ESPIPE},
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_NOREUSE,    ESPIPE},
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_WILLNEED,   ESPIPE},
	{GIVEN_IN_SETUP, 0, 0, POSIX_FADV_DONTNEED,   ESPIPE},
};

int TST_TOTAL = sizeof(TC) / sizeof(TC[0]);

int
main(int ac, char **av)
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */
	int i;

        /* Check this system has fadvise64 system which is used 
          in posix_fadvise. */
        if ((_FILE_OFFSET_BITS != 64) && (__NR_fadvise64 == 0)) {
               tst_resm(TWARN, "This test can only run on kernels that implements ");
               tst_resm(TWARN, "fadvise64 which is used from posix_fadvise");
               exit(0);
        }

        /* Disable test if the version of the kernel is less than 2.6.16 */
        if((tst_kvercmp(2,6,16)) < 0) {
          tst_resm(TWARN, "This test can only run on kernels that are ");
          tst_resm(TWARN, "2.6.16 and higher");
          exit(0);
        }

	/*
	 * parse standard options
	 */
	if ( (msg = parse_opts(ac, av, (option_t *) NULL, NULL)) != (char *) NULL )
	  tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);

	/*
	 * perform global setup for test
	 */
	setup();

	/*
	 * check looping state if -i option given on the command line
	 */
	for (lc=0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping. */
		Tst_count=0;

		/* loop through the test cases */
		for (i = 0; i < TST_TOTAL; i++) {

			TEST(posix_fadvise(TC[i].fd, TC[i].offset, TC[i].len, TC[i].advice));

			if (TEST_RETURN == 0) {
				tst_resm(TFAIL, "call succeeded unexpectedly");
				continue;
			}
	  
			/* Man page says:
			   "On error, an error number is returned." */
			if (TEST_RETURN == TC[i].error) {
				tst_resm(TPASS, "expected failure - "
					 "returned value = %d : %s", TEST_RETURN,
					 strerror(TEST_RETURN));
			} else {
				tst_resm(TFAIL, "unexpected return value - %d : %s - "
					 "expected %d", TEST_RETURN,
					 strerror(TEST_RETURN), TC[i].error);
			}
		}
	}	/* End for TEST_LOOPING */

	/*
	 * cleanup and exit
	 */
	cleanup();

	return(0);
}	/* End main */

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void 
setup()
{
	int pipedes[2];
	
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* Make a pipe */
	if (pipe(pipedes) != 0) {
		tst_brkm(TBROK, cleanup,
			 "Untable to make a pipe: %s\n",
			 strerror(errno));
	} else {
		int i;
		
		/* Close write side first. 
		   I don't use it in test. */
		close(pipedes[1]);

		/* Fill fd field of all test cases 
		   with read side of pipe. */
		for (i = 0; i < TST_TOTAL; i++) {
			TC[i].fd = pipedes[0];
		}
	}
}	/* End setup() */


/*
 * cleanup() - performs all ONE TIME cleanup for this test at
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

	/* Close pipe of read side */
	close(TC[0].fd);

	/* exit with return code appropriate for results */
	tst_exit();
}	/* End cleanup() */
