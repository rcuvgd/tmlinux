
/******************************************************************************
 *          			 fallocate02.c                                  
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
  *    TEST IDENTIFIER	: fallocate02
  *
  *    EXECUTED BY		: anyone
  *
  *    TEST TITLE		: Checks for Errors from fallocate()
  *
  *    TEST CASE TOTAL	: 7
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
  *    (Tests fallocate() for different test cases as reported in map page)
  *
  *	INPUT SPECIFICATIONS
  * 		No input needs to be specified
  *		  fallocate() in-puts are specified through test_data
  *
  *	OUTPUT SPECIFICATIONS
  *		fallocate Error message matches with the expected error message.
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
  *		This test suite tests various error messages from fallocate
  *		If the error message received matches with the expected
  *		test is considered passed else test fails
  *		Provided TEST_DEFAULT to switch b/w modes
  *
  *		Total 7 Test Cases :-
  *		Various error messages from the man page
  *
  *	Setup:
  *		Setup files on which fallocate is to be called
  *
  *	Test:
  *		Loop if the proper options are given.
  *		Execute system call
  *		Check return code.
  *		If error obtained matches with the expected error
  *		PASS the test, otherwise TEST FAILS
  *		Provided TEST_DEFAULT to switch b/w modes
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
#include <sys/types.h>
#include <fcntl.h>
#include <sys/utsname.h>

/* Harness Specific Include Files. */
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"

#define BLOCKS_WRITTEN 12

#ifdef TEST_DEFAULT
	#define DEFAULT_TEST_MODE 0   //DEFAULT MODE
#else
	#define DEFAULT_TEST_MODE 1  //FALLOC_FL_KEEP_SIZE MODE
#endif

#ifndef __NR_fallocate
#	define __NR_fallocate -1  //DUMMY VALUE
	int arch_support = 0;
#else
	int arch_support = 1;
#endif

#define OFFSET 12

/*Local Functions*/
static inline long fallocate();
void populate_file();
void create_fifo();
void create_pipe();
void get_blocksize(int fd);

/* Extern Global Variables */
extern int  Tst_count;               /* counter for tst_xxx routines.         */
/* Global Variables */
char *TCID     = "fallocate02"; /* test program identifier.          */
char fnamew[255];	/* Files used for testing  */
char fnamer[255]; /* Files used for testing  */
int fdw;
int fdr;
enum {RFILE,WFILE,PIPE,FIFO};
struct test_data_t
{
	int file;
	int mode;
	loff_t offset;
	loff_t len;
	int error; 
} test_data[] = { 
                {RFILE, DEFAULT_TEST_MODE, 0, 1, EBADF},
                {WFILE, DEFAULT_TEST_MODE, -1, 1, EINVAL},
                {WFILE, DEFAULT_TEST_MODE, 1, -1, EINVAL},
                {WFILE, DEFAULT_TEST_MODE, BLOCKS_WRITTEN, 0, EINVAL},
                {WFILE, DEFAULT_TEST_MODE, BLOCKS_WRITTEN, -1, EINVAL},
                {WFILE, DEFAULT_TEST_MODE,-( BLOCKS_WRITTEN + OFFSET ), 1 , EINVAL},
                {WFILE, DEFAULT_TEST_MODE, BLOCKS_WRITTEN - OFFSET, 1 , 0}
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
	if(close(fdw) == -1)
	{	
		tst_resm(TWARN, "close(%s) Failed, errno=%d : %s",
		fnamew, errno, strerror(errno));
	}
	
	if(close(fdr) == -1)
	{	
		tst_resm(TWARN, "close(%s) Failed, errno=%d : %s",
		fnamer, errno, strerror(errno));
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
 	/* capture signals */
    	tst_sig(FORK, DEF_HANDLER, cleanup); 

    	/* Pause if that option was specified */
    	TEST_PAUSE;

	 /* make a temp directory and cd to it */
    	tst_tmpdir();

	sprintf(fnamer,"tfile_read_%d",getpid());
	sprintf(fnamew,"tfile_write_%d",getpid());

	if ((fdr = open(fnamer,O_RDONLY|O_CREAT,S_IRUSR)) == -1) {
 	       tst_brkm(TBROK, cleanup,   "open(%s,O_RDONLY|O_CREAT,S_IRUSR) Failed, errno=%d : %s",
                fnamer, errno, strerror(errno));
    	}
	if ((fdw = open(fnamew,O_RDWR|O_CREAT,S_IRWXU)) == -1) {
 	       tst_brkm(TBROK, cleanup,   "open(%s,O_RDWR|O_CREAT,S_IRWXU) Failed, errno=%d : %s",
                fnamew, errno, strerror(errno));
    	}
	get_blocksize(fdr);
	populate_file();
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
 * Writes data into the file
 ******************************************************************************/
void
populate_file()
{
        int index;
        int blocks;
        int data;

	char* buf=malloc(buf_size+1);
	
	if(buf==NULL)  exit(-1);

        for (blocks = 0; blocks < BLOCKS_WRITTEN ; blocks++)
        {
                for (index = 0; index < buf_size; index++)
                                buf[index] = 'A' + (index % 26);
                buf[buf_size]='\0';
                if (( data = write(fdw, buf, buf_size)) < 0 )
                {
                        tst_brkm(TBROK, cleanup, "Unable to write to %s. Error: %d, %s", \
                        fnamew,errno, strerror(errno));
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
	int fd;
	char fname[255], *msg;
	
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
			switch(test_data[test_index].file){
			case RFILE: fd = fdr;
				strcpy(fname,fnamer);
				break;
			case WFILE: fd = fdw;
				strcpy(fname,fnamew);
				break;
			} 
	        	
		TEST(fallocate( fd, test_data[test_index].mode, 
		test_data[test_index].offset * block_size, test_data[test_index].len * block_size));
			 /* check return code */
	        if ( TEST_ERRNO != test_data[test_index].error ) {
			if ( TEST_ERRNO == EOPNOTSUPP ) {
				tst_brkm( TCONF, cleanup, " fallocate system call" \
				" is not implemented");
			}
	            TEST_ERROR_LOG(TEST_ERRNO);
	            tst_resm(TFAIL, "fallocate(%s:%d, %d, %lld, %lld) Failed, expected errno:%d \
				instead errno=%d : %s", fname, fd,test_data[test_index].mode, 
				test_data[test_index].offset * block_size, test_data[test_index].len * block_size, 
				test_data[test_index].error, TEST_ERRNO, strerror(TEST_ERRNO));
		 } else {
		        /* No Verification test, yet... */
		        tst_resm(TPASS, "fallocate(%s:%d, %d, %lld, %lld) returned %d ",
			fname, fd,test_data[test_index].mode, test_data[test_index].offset * block_size, 
			test_data[test_index].len * block_size, TEST_ERRNO);
        		}
		}
	}
	cleanup();
	return(0);
}

