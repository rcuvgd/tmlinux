
/******************************************************************************
 *          			 fallocate03.c                                   
 *	Mon Dec 24 2007
 *  	Copyright (c) International Business Machines  Corp., 2007             
 *	Emali : sharyathi@in.ibm.com
 ******************************************************************************/


/***************************************************************************
  * This program is free software;  you can redistribute it and/or modify      
  * it under the terms of the GNU General Public License as published by       
  * the Free Software Foundation; either version 2 of the License, or          
  * (at your option) any later version.                                        
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU Library General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
***************************************************************************/ 	

/*****************************************************************************
  *
  *    OS Test - International Business Machines Corp. 2007.
  *
  *    TEST IDENTIFIER	: fallocate03
  *
  *    EXECUTED BY		: anyone
  *
  *    TEST TITLE		: fallocate 
  *
  *    TEST CASE TOTAL	: 8
  *
  *    CPU ARCHITECTURES	: PPC,X86, X86_64
  *
  *    AUTHOR			: Sharyathi Nagesh
  *
  *    CO-PILOT			:
  *
  *    DATE STARTED		: 24/12/2007
  *
  *    TEST CASES
  *    (Working of fallocate on a sparse file)
  * 	 
  *
  *	INPUT SPECIFICATIONS
  * 		No input needs to be specified
  *		  fallocate() in-puts are specified through test_data
  *
  *	OUTPUT SPECIFICATIONS
  *		Output describing whether test cases passed or failed.
  *
  *	ENVIRONMENTAL NEEDS
  *		Test Needs to be executed on file system supporting ext4
  *   LTP {TMP} Needs to be set to such a folder
  *   
  *	SPECIAL PROCEDURAL REQUIREMENTS
  * 		None
  *
  *	DETAILED DESCRIPTION
  *		This is a test case for fallocate() system call.
  *		This test suite tests working of fallocate on sparse file
  *		fallocate is tested for different offsets
  *
  *		Total 8 Test Cases :-
  *			Different offsets with in a sparse file is tested
  *
  *	Setup:
  *		Setup file on which fallocate is to be called
  *		Set up a file with hole, created through lseek
  *
  *	Test:
  *		Loop if the proper options are given 
  *		Execute system call
  *		Check return code, if system call failed 
  *		TEST fails, PASS the test otherwise
  *
  *	Cleanup:
  *		Cleanup the temporary folder
  *
*************************************************************************/

/* Standard Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h> //Can be done with out
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>

/* Harness Specific Include Files. */
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"

#define BLOCKS_WRITTEN 12
#define HOLE_SIZE_IN_BLOCKS 12
#define DEFAULT_MODE 0
#define FALLOC_FL_KEEP_SIZE 1 //Need to be removed once the glibce support is provided  
#define TRUE 0

#ifndef __NR_fallocate
#	define __NR_fallocate -1  //DUMMY VALUE
	int arch_support = 0;
#else
	int arch_support = 1;
#endif

/*Local Functions*/
static inline long fallocate();
void get_blocksize(int);
void populate_file();
void file_seek(off_t); 

/* Extern Global Variables */
extern int  Tst_count;               /* counter for tst_xxx routines.         */

/* Global Variables */
char *TCID     = "fallocate03"; /* test program identifier.          */
char fname[255];
int fd;
struct test_data_t
{
	int mode;
	loff_t offset;
	loff_t len;
	int error; 
}test_data[] = { 
	{DEFAULT_MODE, 2, 1, TRUE},
	{DEFAULT_MODE, BLOCKS_WRITTEN, 1, TRUE},
	{DEFAULT_MODE, BLOCKS_WRITTEN + HOLE_SIZE_IN_BLOCKS/2 -1 , 1, TRUE},
	{DEFAULT_MODE, BLOCKS_WRITTEN + HOLE_SIZE_IN_BLOCKS + 1 , 1, TRUE},
	{FALLOC_FL_KEEP_SIZE,2, 1, TRUE},
	{FALLOC_FL_KEEP_SIZE, BLOCKS_WRITTEN , 1, TRUE},
	{FALLOC_FL_KEEP_SIZE, BLOCKS_WRITTEN + HOLE_SIZE_IN_BLOCKS/2 + 1 , 1, TRUE},
	{FALLOC_FL_KEEP_SIZE, BLOCKS_WRITTEN + HOLE_SIZE_IN_BLOCKS + 2 , 1, TRUE}
};
int  TST_TOTAL  = sizeof(test_data)/sizeof(test_data[0]);                  /* total number of tests in this file.   */
int block_size;
int buf_size;

/******************************************************************************
 * Performs all one time clean up for this test on successful    
 * completion,  premature exit or  failure. Closes all temporary 
 * files, removes all temporary directories exits the test with  
 * appropriate return code by calling tst_exit() function.       
******************************************************************************/
extern void
cleanup()
{
	/* Close all open file descriptors. */
	if(close(fd) == -1)
	{	
		tst_resm(TWARN, "close(%s) Failed, errno=%d : %s",
		fname, errno, strerror(errno));
	}

    	/* Remove tmp dir and all files in it */
    	tst_rmdir();

	/* Exit with appropriate return code. */
	tst_exit();
}


/*****************************************************************************
 * Performs all one time setup for this test. This function is   
 * used to create temporary dirs and temporary files
 * that may be used in the course of this test
 ******************************************************************************/

void
setup()
{

	/* Create temporary directories */
	TEST_PAUSE;

	tst_tmpdir();
	
	sprintf(fname,"tfile_sparse_%d",getpid());
	if ((fd = open(fname, O_RDWR|O_CREAT, 0700)) == -1 )
	{	
		tst_brkm(TBROK, cleanup, "Unable to open %s for read/write.  Error:%d, %s\n", \
		fname, errno, strerror(errno));
	}
	get_blocksize(fd);	
	populate_file();
	file_seek(BLOCKS_WRITTEN + HOLE_SIZE_IN_BLOCKS); //create holes
	populate_file();
	file_seek(0); //Rewind
}

/*****************************************************************************
 * Gets the block size for the file system
 ******************************************************************************/
void
get_blocksize(int fd)
{
         struct stat file_stat;

        if( fstat(fd, &file_stat) < 0 )
           tst_resm(TFAIL, "fstat failed while getting block_size errno=%d : %s",
                TEST_ERRNO, strerror(TEST_ERRNO));

        block_size = (int) file_stat.st_blksize;
        buf_size = block_size;
}

/*****************************************************************************
 * Create a Hole in the file
 ******************************************************************************/
void 
file_seek(off_t offset)
{
	offset *= block_size;
	lseek(fd,offset,SEEK_SET);
}

	
/*****************************************************************************
 * Writes data into the file
 ******************************************************************************/
void
populate_file()
{
	int index;
	int blocks;
	int data;
	char* buf=malloc(buf_size+1);

	if(buf==NULL) exit(-1);

	for (blocks = 0; blocks < BLOCKS_WRITTEN ; blocks++)
	{
		for (index = 0; index < buf_size; index++) 
                	        buf[index] = 'A' + (index % 26);
		buf[buf_size]='\0';
		if (( data = write(fd, buf, buf_size)) < 0 )
		{
			tst_brkm(TBROK, cleanup, "Unable to write to %s. Error: %d, %s", \
			fname,errno, strerror(errno));
		}
	}
}



/*****************************************************************************
 * Wraper function to call fallocate system call
 ******************************************************************************/
static inline long fallocate(int fd, int mode, loff_t offset, loff_t len)
{
            #if __WORDSIZE == 32
            struct utsname buf;
            if ( uname(&buf) == 0 ){
                        if(!strcmp(buf.machine,"ppc64") || !strcmp(buf.machine,"x86_64"))
                            return syscall(__NR_fallocate, fd, mode, (int)(offset>>32), (int)offset ,(int)(len>>32), (int)len);
            }
            else{
                        perror("uname:");
                        return -1;
                        }
            #endif
                        return syscall(__NR_fallocate, fd, mode,
offset,len);
}

/*****************************************************************************
 * Main function that calls the system call with the  appropriate parameters
 ******************************************************************************/
int
main(int   ac,    /* number of command line parameters                      */
        char **av)   /* pointer to the array of the command line parameters.   */
{
	int test_index = 0;
	int lc;
	char *msg;

    	/***************************************************************
    	 * parse standard options
     	***************************************************************/
    	if ( (msg=parse_opts(ac, av, (option_t *) NULL, NULL)) != (char *) NULL )
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
	
	/* This test needs kernel version > 2.6.23 and
	 * either of x86, x86_64 or ppc architecture 
	 */
	if ( !arch_support || (tst_kvercmp(2,6,23) < 0)) {
		tst_resm(TWARN," System doesn't support execution of the test");		
		exit(0);
	}

	/* perform global test setup, call setup() function. */
	setup();

	 for (lc=0; TEST_LOOPING(lc); lc++) {
	        /* reset Tst_count in case we are looping. */
        	Tst_count=0;
		for(test_index = 0 ; test_index < TST_TOTAL; test_index ++)
		{
	        	TEST(fallocate(fd, test_data[test_index].mode, 
				test_data[test_index].offset * block_size, test_data[test_index].len * block_size));

			 /* check return code */
		        if ( TEST_RETURN != test_data[test_index].error ) {
				if ( TEST_ERRNO == EOPNOTSUPP ) {
					tst_resm( TCONF, " fallocate system call" \
					" is not implemented");
					cleanup(); /* calls tst_exit */
				}
		            TEST_ERROR_LOG(TEST_ERRNO);
		            tst_resm(TFAIL, "fallocate(%s, %d, %lld, %lld) Failed, errno=%d : %s",
				fname,test_data[test_index].mode, test_data[test_index].offset * block_size, 
				test_data[test_index].len * block_size, TEST_ERRNO, strerror(TEST_ERRNO));
		        } else {
            		if ( STD_FUNCTIONAL_TEST ) {
		        /* No Verification test, yet... */
		        tst_resm(TPASS, "fallocate(%s, %d, %lld, %lld) returned %d ",
				fname,test_data[test_index].mode, test_data[test_index].offset * block_size, 
				test_data[test_index].len * block_size, TEST_RETURN); }	
        		}
		}
	}
	cleanup();
	return(0);
}
