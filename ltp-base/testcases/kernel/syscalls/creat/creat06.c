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
 *	creat06.c
 *
 * DESCRIPTION
 *	Testcase to check creat(2) sets the following errnos correctly:
 *	1.	EISDIR
 *	2.	ENAMETOOLONG
 *	3.	ENOENT
 *	4.	ENOTDIR
 *	5.	EFAULT
 *	6.	EACCES
 *
 * ALGORITHM
 *	1.	Attempt to creat(2) an existing directory, and test for
 *		EISDIR
 *	2.	Attempt to creat(2) a file whose name is more than
 *		VFS_MAXNAMLEN and test for ENAMETOOLONG.
 *	3.	Attempt to creat(2) a file inside a directory which doesn't
 *		exist, and test for ENOENT
 *	4.	Attempt to creat(2) a file, the pathname of which comprises
 *		a component which is a file, test for ENOTDIR.
 *	5.	Attempt to creat(2) a file with a bad address
 *		and test for EFAULT
 *	6.	Attempt to creat(2) a file in a directory with no
 *		execute permission and test for EACCES
 *
 * USAGE:  <for command-line>
 *  creat06 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
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

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "test.h"
#include "usctest.h"

void setup(void);
void cleanup(void);

char user1name[] = "nobody";

char *TCID = "creat06";
int fileHandle = 0;
extern int Tst_count;

int exp_enos[] = {EISDIR, ENAMETOOLONG, ENOENT, ENOTDIR, EFAULT, EACCES, 0};

#define	MODE1	0444
#define	MODE2	0666
#define NSIZE	1000

char long_name[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyzabcdefghijklmnopqrstmnopqrstuvwxyz";

char good_dir[NSIZE];
char no_dir[] = "testfile/testdir";
char not_dir[] = "file1/testdir";
char test6_file[] = "dir6/file6";
char nobody_uid[] = "nobody";

struct passwd *ltpuser;
struct test_case_t {
	char *fname;
	int mode;
	int error;
} TC[] = {
	/* The file name is an existing directory */
	{good_dir, MODE1, EISDIR},

	/* The file name is too long - ENAMETOOLONG */
	{long_name, MODE1, ENAMETOOLONG},

	/* Attept to create a file in a directory that doesn't exist - ENOENT */
	{no_dir, MODE1, ENOENT},

	/* a compent of the file's path is not a directory - ENOTDIR */
	{not_dir, MODE1, ENOTDIR},

#if !defined(UCLINUX)
	/* The file address is bad - EFAULT */
	{(char *)-1, MODE1, EFAULT},
#endif

	/* The directory lacks execute permission - EACCES */
	{test6_file, MODE1, EACCES}
};

int TST_TOTAL = (sizeof(TC) / sizeof(*TC));

char * bad_addr = 0;

int
main(int ac, char **av)
{
	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */
	int i;

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
	}

	setup();

	/* set up the expected errnos */
	TEST_EXP_ENOS(exp_enos);

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		/* loop through the test cases */
		for (i = 0; i < TST_TOTAL; i++) {

			TEST(creat(TC[i].fname, TC[i].mode));

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

	return 0;
	return 0;
	/*NOTREACHED*/
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void
setup()
{
	char *cur_dir = NULL;

	/* Switch to nobody user for correct error code collection */
        if (geteuid() != 0) {
                tst_brkm(TBROK, tst_exit, "Test must be run as root");
        }
         ltpuser = getpwnam(nobody_uid);
         if (seteuid(ltpuser->pw_uid) == -1) {
                tst_resm(TINFO, "seteuid failed to "
                         "to set the effective uid to %d",
                         ltpuser->pw_uid);
                perror("seteuid");
         }


	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temporary directory and cd to it */
	tst_tmpdir();

	/* get the current directory for the first test */
	if ((cur_dir = getcwd(cur_dir, 0)) == NULL) {
		tst_brkm(TBROK, cleanup, "getcwd failed");
	}

	if (strncpy(good_dir, cur_dir, NSIZE) == NULL) {
		tst_brkm(TBROK, cleanup, "couldn't copy currect directory");
	}

	/* create a file that will be used in test #3 */
	if ((fileHandle = creat("file1", MODE1)) == -1) {
		tst_brkm(TBROK, cleanup, "couldn't create a test file");
	}

#if !defined(UCLINUX)
	bad_addr = mmap(0, 1, PROT_NONE,
			MAP_PRIVATE_EXCEPT_UCLINUX|MAP_ANONYMOUS, 0, 0);
	if (bad_addr == MAP_FAILED) {
		tst_brkm(TBROK, cleanup, "mmap failed");
	}
	TC[4].fname = bad_addr;
#endif

	/* create a directory that will be used in test #6 */
	if (mkdir("dir6", MODE2) == -1) {
		tst_brkm(TBROK, cleanup, "couldn't creat a test directory");
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
    close(fileHandle);

	TEST_CLEANUP;

	/* delete the test directory created in setup() */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}
