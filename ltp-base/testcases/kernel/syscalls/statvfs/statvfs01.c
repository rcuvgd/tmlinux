/*
 * Copyright (c) Wipro Technologies Ltd, 2005.  All Rights Reserved.
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
 *    TEST IDENTIFIER   : statvfs01
 *
 *    EXECUTED BY       : root / superuser
 *
 *    TEST TITLE        : Basic tests for statvfs(2)
 *
 *    TEST CASE TOTAL   : 1
 *
 *    AUTHOR            : Prashant P Yendigeri 
 *                        <prashant.yendigeri@wipro.com> 
 *
 *    DESCRIPTION
 *      This is a Phase I test for the statvfs(2) system call.
 *      It is intended to provide a limited exposure of the system call.
 *	This call behaves similar to statfs.
 *
 **********************************************************/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/statvfs.h>

#include "test.h"
#include "usctest.h"

#define TEST_PATH "/" /* Should be a mounted FS */

void setup();
void cleanup();

char *TCID="statvfs01";     /* Test program identifier.    */
int TST_TOTAL=1;                /* Total number of test cases. */
extern int Tst_count;           /* Test Case counter for tst_* routines */

int exp_enos[]={0};             /* must be a 0 terminated list */


int  main(int ac, char **av)
{
	struct statvfs buf;
    	int lc;             /* loop counter */
	char *msg;          /* message returned from parse_opts */

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
         	* TEST CASE:
         	* statvfs
         	*/
        	;

        	/* Call statvfs(2) */
        	TEST(statvfs(TEST_PATH,&buf));

        /* check return code */
        if ( TEST_RETURN == -1 ) {
            TEST_ERROR_LOG(TEST_ERRNO);
            tst_resm(TFAIL, "statvfs - Basic sanity test,failed, errno=%d : %s",
                     TEST_ERRNO, strerror(TEST_ERRNO));
        }
	else
	{
            TEST_ERROR_LOG(TEST_ERRNO);
            tst_resm(TPASS, "statvfs - Basic sanity test,PASS");
	}

    }   /* End for TEST_LOOPING */

	tst_resm(TINFO,"This call is similar to statfs");
	tst_resm(TINFO,"Extracting info about the '%s' file system",TEST_PATH);
        tst_resm(TINFO, "file system block size = %u bytes",buf.f_bsize);
        tst_resm(TINFO, "file system fragment size = %u bytes",buf.f_frsize);
        tst_resm(TINFO, "file system free blocks = %d",buf.f_bfree);
        tst_resm(TINFO, "file system total inodes = %u",buf.f_files);
        tst_resm(TINFO, "file system free inodes = %u",buf.f_ffree);
        tst_resm(TINFO, "file system id = %u",buf.f_fsid);
        tst_resm(TINFO, "file system max filename length = %u",buf.f_namemax);

    /***************************************************************
     * cleanup and exit
     ***************************************************************/
    cleanup();

    return 0;
}       /* End main */

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
}       /* End setup() */



/***************************************************************
 * cleanup() - performs all ONE TIME cleanup for this test at
 *              completion or premature exit.
 ***************************************************************/
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
}       /* End cleanup() */

