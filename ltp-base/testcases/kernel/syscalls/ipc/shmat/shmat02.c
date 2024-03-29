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
 *	shmat02.c
 *
 * DESCRIPTION
 *	shmat02 - check for EINVAL and EACCES errors
 *
 * ALGORITHM
 *	loop if that option was specified
 *	  call shmat() using three invalid test cases
 *	  check the errno value
 *	    issue a PASS message if we get EINVAL or EACCES
 *	  otherwise, the tests fails
 *	    issue a FAIL message
 *	call cleanup
 *
 * USAGE:  <for command-line>
 *  shmat02 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *	       -i n : Execute test n times.
 *	       -I x : Execute test for x seconds.
 *	       -P x : Pause for x seconds between iterations.
 *	       -t   : Turn on syscall timing.
 *
 * HISTORY
 *	03/2001 - Written by Wayne Boyer
 *
 *      27/02/2008 Renaud Lottiaux (Renaud.Lottiaux@kerlabs.com)
 *      - Fix concurrency issue. The second key used for this test could
 *        conflict with the key from another task.
 *
 * RESTRICTIONS
 *	Must be ran as non-root
 */

#include "ipcshm.h"
#include <pwd.h>

char *TCID = "shmat02";
extern int Tst_count;
char nobody_uid[] = "nobody";
struct passwd *ltpuser;

int exp_enos[] = {EINVAL, EACCES, 0};	/* 0 terminated list of */
					/* expected errnos      */

int shm_id_1 = -1;
int shm_id_2 = -1;
int shm_id_3 = -1;

void   *base_addr;	/* By probing this address first, we can make
			 * non-aligned addresses from it for different
			 * architectures without explicitly code it.
			 */

void	*addr;		/* for result of shmat-call */

struct test_case_t {
	int *shmid;
	int offset;
	int error;
};

int TST_TOTAL = 3;

#undef SHMLBA
#define SHMLBA  4096

static void setup_tc( int i, struct test_case_t *tc){

	struct test_case_t TC[] = {
		/* EINVAL - the shared memory ID is not valid */
		{&shm_id_1, 0, EINVAL},
		/* EINVAL - the address is not page aligned and SHM_RND is not given */
		{&shm_id_2, SHMLBA - 1, EINVAL},
		/* EACCES - the shared memory resource has no read/write permission */
		{&shm_id_3, 0, EACCES}
	};

	if( i > TST_TOTAL || i < 0)
		return;

	*tc = TC[i];
}

int main(int ac, char **av)
{
	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */
	int i;
	struct test_case_t tc;

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
	}

	setup();			/* global setup */

	/* The following loop checks looping state if -i option given */

	for (lc = 0; TEST_LOOPING(lc); lc++) {
		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		/* setup test case paremeters */
		setup_tc( lc, &tc);

		/* loop through the test cases */
		for (i=0; i<TST_TOTAL; i++) {
			/*
			 * make the call using the TEST() macro - attempt
			 * various invalid shared memory attaches
			 */
			errno = 0;
			addr = shmat(*(tc.shmid), (unsigned char*)base_addr + tc.offset, 0);
			TEST_ERRNO = errno;

                      	if (addr != (void *)-1) {
				tst_resm(TFAIL, "call succeeded unexpectedly");
				continue;
			}
	
			TEST_ERROR_LOG(TEST_ERRNO);

			if (TEST_ERRNO == tc.error) {
				tst_resm(TPASS, "expected failure - errno = "
					 "%d : %s", TEST_ERRNO,
					 strerror(TEST_ERRNO));
			} else {
				tst_resm(TFAIL, "call failed with an "
					 "unexpected error - %d : %s",
					 TEST_ERRNO, strerror(TEST_ERRNO));
			
			}
		}
	}

	cleanup();

	/*NOTREACHED*/
	return(0);
}

/*
 * setup() - performs all the ONE TIME setup for this test.
 */
void
setup(void)
{
	key_t shmkey2;

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

	/* Set up the expected error numbers for -e option */
	TEST_EXP_ENOS(exp_enos);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/*
	 * Create a temporary directory and cd into it.
	 * This helps to ensure that a unique msgkey is created.
	 * See ../lib/libipc.c for more information.
	 */
	tst_tmpdir();

	/* get an IPC resource key */
	shmkey = getipckey();

	/* create a shared memory resource with read and write permissions */
	/* also post increment the shmkey for the next shmget call */
	if ((shm_id_2 = shmget(shmkey, INT_SIZE, SHM_RW | IPC_CREAT |
	     IPC_EXCL)) == -1) {
		tst_brkm(TBROK, cleanup, "Failed to create shared memory "
			 "resource #1 in setup()");
	}

	/* Get an new IPC resource key. */
	shmkey2 = getipckey();

	/* create a shared memory resource without read and write permissions */
	if ((shm_id_3 = shmget(shmkey2, INT_SIZE, IPC_CREAT | IPC_EXCL)) == -1) {
		tst_brkm(TBROK, cleanup, "Failed to create shared memory "
			 "resource #2 in setup()");
	}

	/* Probe an available linear address for attachment */
	if( (base_addr = shmat(shm_id_2, NULL, 0)) == (void *)-1 ){
		tst_brkm(TBROK, cleanup,
				"Couldn't attach shared memory");
	}

	if (shmdt((const void *)base_addr) == -1) {
		tst_brkm(TBROK, cleanup,
				"Couldn't detach shared memory");
	}

	/* some architectures (e.g. parisc) are strange, so better always align to
	 * next SHMLBA address. */
	base_addr = (void *)( ((unsigned long)(base_addr) & ~(SHMLBA-1)) + SHMLBA );
}

/*
 * cleanup() - performs all the ONE TIME cleanup for this test at completion
 * 	       or premature exit.
 */
void
cleanup(void)
{
	/* if they exist, remove the shared memory resources */
	rm_shm(shm_id_2);
	rm_shm(shm_id_3);

	/* Remove the temporary directory */
	tst_rmdir();

	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

	/* exit with return code appropriate for results */
	tst_exit();
}

