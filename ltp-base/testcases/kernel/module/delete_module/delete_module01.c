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
 *    TEST IDENTIFIER   : delete_module01
 * 
 *    EXECUTED BY       : root / superuser
 * 
 *    TEST TITLE        : Basic tests for delete_module(2)
 * 
 *    TEST CASE TOTAL   : 1
 * 
 *    AUTHOR            : Madhu T L <madhu.tarikere@wipro.com>
 * 
 *    SIGNALS
 *		 Uses SIGUSR1 to pause before test if option set.
 *		 (See the parse_opts(3) man page).
 *
 *    DESCRIPTION
 *		 This is a Phase I test for the delete_module(2) system call.
 *		 It is intended to provide a limited exposure of the system call.
 * 
 *		 Setup:
 *		   Setup signal handling.
 *		   Test caller is superuser
 *		   Pause for SIGUSR1 if option specified.
 *		   Initialize modname for each child process.
 * 
 *		 Test:
 *		  Loop if the proper options are given.
 *		   Create module entry
 *		   Execute system call
 *		   Check return code, if system call failed (return=-1)
 *		 		 Issue FAIL message with errno.
 *		   Otherwise, Issue PASS message.
 * 
 *		 Cleanup:
 *		   Print errno log and/or timing stats if options given
 * 
 * USAGE:  <for command-line>
 *  delete_module01 [-c n] [-e] [-f] [-h] [-i n] [-I x] [-p] [-P x] [-t]
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
//#include <linux/module.h>
#include "test.h"
#include "usctest.h"

extern int Tst_count;		 		 /* Test Case counter for tst_* routines */

static void setup(void);
static void cleanup(void);

char *TCID = "delete_module01";		 /* Test program identifier.    */
int TST_TOTAL=1;		 		 /* Total number of test cases. */
//static char modname[20];		 /* Name of the module */
		 

int
main(int argc, char **argv)
{
		 int lc;		 		 		 /* loop counter */
		 char *msg;		 		 /* message returned from parse_opts */
		 char cmd[PATH_MAX];
		 char *module_name = "dummy_del_mod";

		 /* parse standard options */
		 if ((msg = parse_opts(argc, argv, (option_t *)NULL, NULL)) !=
		 		 		 (char *)NULL) {
		 		 tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
		 }

		 /* perform global setup for test */
		 setup();

		 /* check looping state if -i option is given */
		 for (lc = 0; TEST_LOOPING(lc); lc++) {

		 		 /* reset Tst_count in case we are looping */
		 		 Tst_count = 0;

		 		 // Execute system call
		 		 sprintf(cmd, "/sbin/insmod /tmp/%s.ko", module_name);
                                                                              
		 		 // Insmod the module
		 		 if ( (system( cmd )) != 0 ) {
						 tst_resm(TBROK, "Failed to load %s module", module_name);
		 		 		 printf("system() failed; cannot test init_module: errno=%i\n",
                                errno);
		 		 		 goto EXIT;
		 		 }

		 		 /* Test the system call */
		 		 TEST(delete_module(module_name));

		 		 /* check return code */
		 		 if (TEST_RETURN == -1) {
		 		 		 tst_resm(TFAIL, "delete_module() failed to remove"
		 		 		 		 " module entry for %s, errno=%d : %s",
		 		 		 		 module_name, TEST_ERRNO, strerror(TEST_ERRNO));
		 		 } else {
		 		 		 tst_resm(TPASS, "delete_module() successful, returned"
		 		 		 		 " %d", TEST_RETURN);
		 		 }
		 }

		 /* perform global cleanup and exit */
EXIT:
		 cleanup();

		 /*NOTREACHED*/
		 return 0;

}		 /* End main */

/* setup() - performs all ONE TIME setup for this test */
void
setup(void)
{
		 /* capture signals */
		 tst_sig(FORK, DEF_HANDLER, cleanup);

		 /* Check whether we are root  */
		 if (geteuid() != 0) {
		 		 tst_brkm(TBROK, tst_exit, "Must be root for this test!");
		 		 /*NOTREACHED*/
		 }

		 //if (tst_kvercmp(2,5,48) >= 0)
		 		 //tst_brkm(TCONF, tst_exit, "This test will not work on "
		 		 		 		 //"kernels after 2.5.48");

		 /* Pause if that option was specified
		  * TEST_PAUSE contains the code to fork the test with the -c option.
		  */
		 TEST_PAUSE;

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

