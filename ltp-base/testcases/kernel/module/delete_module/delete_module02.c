/*
 * Copyright (c) Wipro Technologies Ltd, 2002.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/**********************************************************
 * 
 *    TEST IDENTIFIER   : delete_module02
 * 
 *    EXECUTED BY       : root / superuser
 * 
 *    TEST TITLE        : Checking error conditions for delete_module(2)
 * 
 *    TEST CASE TOTAL   : 5
 * 
 *    AUTHOR            : Madhu T L <madhu.tarikere@wipro.com>
 * 
 *    SIGNALS
 *      Uses SIGUSR1 to pause before test if option set.
 *      (See the parse_opts(3) man page).
 *
 *    DESCRIPTION
 *      Verify that,
 *      1. delete_module(2) returns -1 and sets errno to ENOENT for non-existing
 *		    module entry.
 *      2. delete_module(2) returns -1 and sets errno to EINVAL, if module
 *         name parameter is null terminated (zero length) string.
 *      3. delete_module(2) returns -1 and sets errno to EFAULT, if 
 *         module name parameter is outside program's accessible address space.
 *      4. delete_module(2) returns -1 and sets errno to ENAMETOOLONG, if 
 *         module name parameter is too long.
 *      5. delete_module(2) returns -1 and sets errno to EPERM, if effective
 *         user id of the caller is not superuser.
 * 
 *      Setup:
 *        Setup signal handling.
 *        Test caller is super user
 *        Check existances of "nobody" user id.
 *        Initialize  long module name
 *        Set expected errnos for logging
 *        Pause for SIGUSR1 if option specified.
 *		   Initialize modname for each child process
 * 
 *      Test:
 *       Loop if the proper options are given.
 *		   Perform testcase specific setup (if needed)
 *        Execute system call
 *        Check return code and error number, if matching,
 *                   Issue PASS message
 *        Otherwise,
 *                   Issue FAIL message
 *		   Perform testcase specific cleanup (if needed)
 * 
 *      Cleanup:
 *        Print errno log and/or timing stats if options given
 * 
 * USAGE:  <for command-line>
 *  delete_module02 [-c n] [-e] [-f] [-h] [-i n] [-I x] [-p] [-P x] [-t]
 *		 		 where,  -c n : Run n copies concurrently.
 *		 		 		 -e   : Turn on errno logging.
 *		 		 		 -f   : Turn off functional testing
 *		 		 		 -h   : Show help screen
 *		 		 		 -i n : Execute test n times.
 *		 		 		 -I x : Execute test for x seconds.
 *		 		 		 -p   : Pause for SIGUSR1 before starting
 *		 		 		 -P x : Pause for x seconds between iterations.
 *		 		 		 -t   : Turn on syscall timing.
 * 
 ****************************************************************/

#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
//#include <linux/module.h>
#include <sys/mman.h>
#include "test.h"
#include "usctest.h"

#ifndef PAGE_SIZE 
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#endif

extern int Tst_count;

#define NULLMODNAME ""
#define BASEMODNAME "dummy"
#define LONGMODNAMECHAR 'm'		 		 /* Arbitrarily selected */
#define MODNAMEMAX (PAGE_SIZE + 1)
#define EXP_RET_VAL -1

struct test_case_t {		 		 		 /* test case structure */
		 char 		 *modname;
		 int		 experrno;		 		 /* expected errno */
		 char		 *desc;
		 int		 (*setup)(void);		 		 /* Individual setup routine */
		 void		 (*cleanup)(void);		 /* Individual cleanup routine */
};

char *TCID = "delete_module02";
static int exp_enos[] = {EPERM, EINVAL, ENOENT, EFAULT, ENAMETOOLONG, 0};
static char nobody_uid[] = "nobody";
struct passwd *ltpuser;
static char longmodname[MODNAMEMAX];
static int testno;
static char modname[20];		 		 /* Name of the module */

char * bad_addr = 0;

static void setup(void);
static void cleanup(void);
static int setup1(void);
static void cleanup1(void);

static struct test_case_t  tdat[] = {
		 { modname, ENOENT,
		 		 "non-existing module", NULL, NULL},
		 { NULLMODNAME, ENOENT,
		 		 "null terminated module name", NULL, NULL},
		 { (char *) -1, EFAULT, "module name outside program's "
		 		 "accessible address space", NULL, NULL},
		 { longmodname, ENOENT,
		 		 "long module name", NULL, NULL},
		 { modname, EPERM,
		 		 "non-superuser", setup1, cleanup1},
};

int TST_TOTAL = sizeof(tdat) / sizeof(tdat[0]);

int
main(int argc, char **argv)
{
		 int lc;		 		 		 		 /* loop counter */
		 char *msg;		 		 		 /* message returned from parse_opts */

		 /* parse standard options */
		 if ((msg = parse_opts(argc, argv, (option_t *)NULL, NULL)) !=
		     (char *)NULL) {
		 		 tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
		 }

		 setup();

		 /* check looping state if -i option is given */
		 for (lc = 0; TEST_LOOPING(lc); lc++) {
		 		 /* reset Tst_count in case we are looping */
		 		 Tst_count = 0;

		 		 for (testno = 0; testno < TST_TOTAL; ++testno) {
		 		 		 if( (tdat[testno].setup) && (tdat[testno].setup()) ) {
		 		 		 		 /* setup() failed, skip this test */
		 		 		 		 continue;
		 		 		 }

		 		 		 /* Test the system call */
		 		 		 TEST(delete_module(tdat[testno].modname));
		 		 		 TEST_ERROR_LOG(TEST_ERRNO);
		 		 		 printf("TEST_RETURN is %d, TEST_ERRNO is %d\n", TEST_RETURN, TEST_ERRNO);
		 		 		 if ( (TEST_RETURN == EXP_RET_VAL) &&
		 		 		 		 (TEST_ERRNO == tdat[testno].experrno) ) {
		 		 		 		 tst_resm(TPASS, "Expected results for %s, "
		 		 		 		 		 "errno: %d", tdat[testno].desc,
		 		 		 		 		 TEST_ERRNO);
		 		 		 } else {
		 		 		 		 tst_resm(TFAIL, "Unexpected results for %s ; "
		 		 		 		 		 "returned %d (expected %d), errno %d "
		 		 		 		 		 "(expected %d)", tdat[testno].desc,
		 		 		 		 		 TEST_RETURN, EXP_RET_VAL,
		 		 		 		 		 TEST_ERRNO, tdat[testno].experrno);
		 		 		 }
		 		 		 if(tdat[testno].cleanup) {
		 		 		 		 tdat[testno].cleanup();
		 		 		 }
		 		 }
		 }
		 cleanup();

		 /*NOTREACHED*/
		 return 0;
}

int
setup1(void)
{
		 /* Change effective user id to nodody */
		 if (seteuid(ltpuser->pw_uid) == -1) {
		 		 tst_resm(TBROK, "seteuid failed to set the effective"
		 		 		 		 " uid to %d", ltpuser->pw_uid);
		 		 return 1;
		 }
		 return 0;
}

void
cleanup1(void)
{
		  /* Change effective user id to root */
         if (seteuid(0) == -1) {
		 		 tst_brkm(TBROK, tst_exit, "seteuid failed to set the effective"
		 		 		 " uid to root");
         }
}


/*
 * setup()
 *		 performs all ONE TIME setup for this test
 */
void
setup(void)
{
		 /* capture signals */
		 tst_sig(NOFORK, DEF_HANDLER, cleanup);

		 /* Check whether it is root  */
		 if (geteuid() != 0) {
		 		 tst_brkm(TBROK, tst_exit, "Must be root for this test!");
		 		 /*NOTREACHED*/
		 }

		 /*if (tst_kvercmp(2,5,48) >= 0)
		 		 tst_brkm(TCONF, tst_exit, "This test will not work on "
		 		 		 		 "kernels after 2.5.48");
		 */

        /* Check for nobody_uid user id */
		  if( (ltpuser = getpwnam(nobody_uid)) == NULL) {
		 		 tst_brkm(TBROK, tst_exit, "Required user %s doesn't exists",
		 		 		 		 nobody_uid);
		 		 /*NOTREACHED*/
		  }

		 /* Initialize longmodname to LONGMODNAMECHAR character */
		 memset(longmodname, LONGMODNAMECHAR, MODNAMEMAX - 1);

		 /* set the expected errnos... */
		 TEST_EXP_ENOS(exp_enos);

		 /* Pause if that option was specified
		  * TEST_PAUSE contains the code to fork the test with the -c option.
		  */
		 TEST_PAUSE;

		 /* Get unique module name for each child process */
		 if( sprintf(modname, "%s_%d",BASEMODNAME, getpid()) <= 0) {
		 		 tst_brkm(TBROK, tst_exit, "Failed to initialize module name");
		 }
        bad_addr = mmap(0, 1, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
        if (bad_addr == MAP_FAILED) {
                tst_brkm(TBROK, cleanup, "mmap failed");
    		 }
		 tdat[2].modname = bad_addr;

}

/*
 * cleanup()
 *		 performs all ONE TIME cleanup for this test at
 *		 completion or premature exit
 */
void
cleanup(void)
{
		 /*
		  * print timing stats if that option was specified.
		  * print errno log if that option was specified.
		  */
		 TEST_CLEANUP;

		 /* exit with return code appropriate for results */
		 tst_exit();
		 /*NOTREACHED*/
}

