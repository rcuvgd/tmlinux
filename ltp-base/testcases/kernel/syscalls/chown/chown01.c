/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 *
 * http://www.sgi.com
 *
 * For further information regarding this notice, see:
 *
 * http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */
/* $Id: chown01.c,v 1.2 2006/05/26 06:26:38 vapier Exp $ */
/**********************************************************
 * 
 *    OS Test - Silicon Graphics, Inc.
 * 
 *    TEST IDENTIFIER	: chown01
 * 
 *    EXECUTED BY	: anyone
 * 
 *    TEST TITLE	: Basic test for chown(2)
 * 
 *    PARENT DOCUMENT	: usctpl01
 * 
 *    TEST CASE TOTAL	: 1
 * 
 *    WALL CLOCK TIME	: 1
 * 
 *    CPU TYPES		: ALL
 * 
 *    AUTHOR		: William Roske
 * 
 *    CO-PILOT		: Dave Fenner
 * 
 *    DATE STARTED	: 03/30/92
 * 
 *    INITIAL RELEASE	: UNICOS 7.0
 * 
 *    TEST CASES
 * 
 * 	1.) chown(2) returns...(See Description)
 *	
 *    INPUT SPECIFICATIONS
 * 	The standard options for system call tests are accepted.
 *	(See the parse_opts(3) man page).
 * 
 *    OUTPUT SPECIFICATIONS
 * 	
 *    DURATION
 * 	Terminates - with frequency and infinite modes.
 * 
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    RESOURCES
 * 	None
 * 
 *    ENVIRONMENTAL NEEDS
 *      No run-time environmental needs.
 * 
 *    SPECIAL PROCEDURAL REQUIREMENTS
 * 	None
 * 
 *    INTERCASE DEPENDENCIES
 * 	None
 * 
 *    DETAILED DESCRIPTION
 *	This is a Phase I test for the chown(2) system call.  It is intended
 *	to provide a limited exposure of the system call, for now.  It
 *	should/will be extended when full functional tests are written for
 *	chown(2).
 * 
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 * 
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  Execute system call
 *	  Check return code, if system call failed (return=-1)
 *		Log the errno and Issue a FAIL message.
 *	  Otherwise, Issue a PASS message.
 * 
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 * 
 * 
 *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#**/

#include <sys/types.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "test.h"
#include "usctest.h"

void setup();
void cleanup();



char *TCID="chown01"; 		/* Test program identifier.    */
int TST_TOTAL=1;    		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

int exp_enos[]={0, 0};

char fname[255];
int fd, uid, gid;
char *buf = "davef";

int
main(int ac, char **av)
{
    int lc;		/* loop counter */
    char *msg;		/* message returned from parse_opts */
    
    /***************************************************************
     * parse standard options
     ***************************************************************/
    if ( (msg=parse_opts(ac, av, (option_t *) NULL, NULL)) != (char *) NULL )
	tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);

    /***************************************************************
     * perform global setup for test
     ***************************************************************/
    setup();

    /* set the expected errnos... */
    TEST_EXP_ENOS(exp_enos);

    /***************************************************************
     * check looping state if -c option given
     ***************************************************************/
    for (lc=0; TEST_LOOPING(lc); lc++) {

	/* reset Tst_count in case we are looping. */
	Tst_count=0;

	/* 
	 * Call chown(2) 
	 */
	TEST(chown(fname, uid,gid));
	
	/* check return code */
	if ( TEST_RETURN == -1 ) {
	    TEST_ERROR_LOG(TEST_ERRNO);
	    tst_resm(TFAIL, "chown(%s, %d,%d) Failed, errno=%d : %s", fname, uid, gid,
		     TEST_ERRNO, strerror(TEST_ERRNO));
	} else {

	    /***************************************************************
	     * only perform functional verification if flag set (-f not given)
	     ***************************************************************/
	    if ( STD_FUNCTIONAL_TEST ) {
		/* No Verification test, yet... */
		tst_resm(TPASS, "chown(%s, %d,%d) returned %d", fname, uid, gid, TEST_RETURN);
	    }
	}	/* end else */

    }	/* End for TEST_LOOPING */

    /***************************************************************
     * cleanup and exit
     ***************************************************************/
    cleanup();

    return 0;
}	/* End main */

/***************************************************************
 * setup() - performs all ONE TIME setup for this test.
 ***************************************************************/
void 
setup()
{
    /* capture signals */
    tst_sig(NOFORK, DEF_HANDLER, cleanup);

    /* Pause if that option was specified */
    TEST_PAUSE;

    /* create a temp dir and cd to it. */
    tst_tmpdir();

    /* set uid and gid */
    uid=geteuid();
    gid=getegid();

    sprintf(fname,"t_%d",getpid());
    if ((fd = open(fname,O_RDWR|O_CREAT,0700)) == -1) {
       tst_brkm(TBROK, cleanup, "open(%s, O_RDWR|O_CREAT,0700) Failed, errno=%d : %s", fname, errno, strerror(errno));
    } else if (write(fd, &buf, strlen(buf)) == -1) {
       tst_brkm(TBROK, cleanup, "write(%s, &buf, strlen(buf)) Failed, errno=%d : %s", fname, errno, strerror(errno));
    } else if (close(fd) == -1) {
       tst_brkm(TBROK, cleanup, "close(%s) Failed, errno=%d : %s", fname, errno, strerror(errno));
    }
}	/* End setup() */


/***************************************************************
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 ***************************************************************/
void 
cleanup()
{
    /*
     * print timing stats if that option was specified.
     * print errno log if that option was specified.
     */
    TEST_CLEANUP;

    /* remove temp dir and files */
    tst_rmdir();

    /* exit with return code appropriate for results */
    tst_exit();
}	/* End cleanup() */


