/******************************************************************************/
/*                                                                            */
/* Copyright (c) Ulrich Drepper <drepper@redhat.com>                          */
/* Copyright (c) International Business Machines  Corp., 2009                 */
/*                                                                            */
/* This program is free software;  you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation; either version 2 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY;  without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See                  */
/* the GNU General Public License for more details.                           */
/*                                                                            */
/* You should have received a copy of the GNU General Public License          */
/* along with this program;  if not, write to the Free Software               */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA    */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/* File:        inotify_init1_02.c                                            */
/*                                                                            */
/* Description: This Program tests the new system call introduced in 2.6.27.  */
/*              Ulrich´s comment as in:                                       */
/* http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=commit;h=510df2dd482496083e1c3b1a8c9b6afd5fa4c7d7 */
/* which says:                                                                */
/* This patch adds non-blocking support for inotify_init1.  The additional    */
/* changes needed are minimal. The following test must be adjusted for        */
/* architectures other than x86 and x86-64 and in case the syscall numbers    */
/* changed.                                                                   */
/*                                                                            */
/* Usage:  <for command-line>                                                 */
/* inotify_init1_02 [-c n] [-e][-i n] [-I x] [-p x] [-t]                      */
/*      where,  -c n : Run n copies concurrently.                             */
/*              -e   : Turn on errno logging.                                 */
/*              -i n : Execute test n times.                                  */
/*              -I x : Execute test for x seconds.                            */
/*              -P x : Pause for x seconds between iterations.                */
/*              -t   : Turn on syscall timing.                                */
/*                                                                            */
/* Total Tests: 1                                                             */
/*                                                                            */
/* Test Name:   inotify_init1_02                                              */
/*                                                                            */
/* Author:      Ulrich Drepper <drepper@redhat.com>                           */
/*                                                                            */
/* History:     Created - Jan 13 2009 - Ulrich Drepper <drepper@redhat.com>   */
/*              Ported to LTP                                                 */
/*                      - Jan 13 2009 - Subrata <subrata@linux.vnet.ibm.com>  */
/******************************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

/* Harness Specific Include Files. */
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"

#ifndef O_CLOEXEC
# define O_CLOEXEC 02000000
#endif

#ifndef __NR_inotify_init1
# ifdef __x86_64__
#  define __NR_inotify_init1 294
# elif defined __i386__
#  define __NR_inotify_init1 332
# else
#  error "need __NR_inotify_init1"
# endif
#endif

#define IN_NONBLOCK O_NONBLOCK

/* Extern Global Variables */
extern int  Tst_count;               /* counter for tst_xxx routines.         */
extern char *TESTDIR;                /* temporary dir created by tst_tmpdir() */

/* Global Variables */
char *TCID     = "inotify_init1_02"; /* test program identifier.              */
int  testno;
int  TST_TOTAL = 1;                  /* total number of tests in this file.   */

/* Extern Global Functions */
/******************************************************************************/
/*                                                                            */
/* Function:    cleanup                                                       */
/*                                                                            */
/* Description: Performs all one time clean up for this test on successful    */
/*              completion,  premature exit or  failure. Closes all temporary */
/*              files, removes all temporary directories exits the test with  */
/*              appropriate return code by calling tst_exit() function.       */
/*                                                                            */
/* Input:       None.                                                         */
/*                                                                            */
/* Output:      None.                                                         */
/*                                                                            */
/* Return:      On failure - Exits calling tst_exit(). Non '0' return code.   */
/*              On success - Exits calling tst_exit(). With '0' return code.  */
/*                                                                            */
/******************************************************************************/
extern void cleanup() {
  /* Remove tmp dir and all files in it */
  TEST_CLEANUP;
  tst_rmdir();

  /* Exit with appropriate return code. */
  tst_exit();
}

/* Local  Functions */
/******************************************************************************/
/*                                                                            */
/* Function:    setup                                                         */
/*                                                                            */
/* Description: Performs all one time setup for this test. This function is   */
/*              typically used to capture signals, create temporary dirs      */
/*              and temporary files that may be used in the course of this    */
/*              test.                                                         */
/*                                                                            */
/* Input:       None.                                                         */
/*                                                                            */
/* Output:      None.                                                         */
/*                                                                            */
/* Return:      On failure - Exits by calling cleanup().                      */
/*              On success - returns 0.                                       */
/*                                                                            */
/******************************************************************************/
void setup() {
  /* Capture signals if any */
  /* Create temporary directories */
  TEST_PAUSE;
  tst_tmpdir();
}

int main (int argc, char *argv[]) {
    int fd, fl;
    int lc;                 /* loop counter */
    char *msg;              /* message returned from parse_opts */

    /* Parse standard options given to run the test. */
    msg = parse_opts(argc, argv, (option_t *) NULL, NULL);
    if (msg != (char *) NULL) {
        tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
        tst_exit();
    }
    if((tst_kvercmp(2, 6, 27)) < 0) {
        tst_resm(TCONF, "This test can only run on kernels that are 2.6.27 and higher");
        tst_exit();
    }
    setup();

    /* Check looping state if -i option given */
    for (lc = 0; TEST_LOOPING(lc); ++lc) {
         Tst_count = 0;
         for (testno=0; testno < TST_TOTAL; ++testno) {
              fd = syscall (__NR_inotify_init1, 0);
              if (fd == -1) {
                  tst_resm(TFAIL, "inotify_init1(0) failed");
                  cleanup();
                  tst_exit();
              }
              fl = fcntl (fd, F_GETFL);
              if (fl == -1) {
                  tst_brkm(TBROK, cleanup, "fcntl failed");
                  tst_exit();
              }
              if (fl & O_NONBLOCK) {
                  tst_resm(TFAIL, "inotify_init1(0) set non-blocking mode");
                  cleanup();
                  tst_exit();
              }
              close (fd);

              fd = syscall (__NR_inotify_init1, IN_NONBLOCK);
              if (fd == -1) {
                  tst_resm(TFAIL, "inotify_init1(IN_NONBLOCK) failed");
                  cleanup();
                  tst_exit();
              }
              fl = fcntl (fd, F_GETFL);
              if (fl == -1) {
                  tst_brkm(TBROK, cleanup, "fcntl failed");
                  tst_exit();
              }
              if ((fl & O_NONBLOCK) == 0) {
                   tst_resm(TFAIL, "inotify_init1(IN_NONBLOCK) set non-blocking mode");
                   cleanup();
                   tst_exit();
              }
              close (fd);
              tst_resm(TPASS, "inotify_init1(IN_NONBLOCK) PASSED");
              cleanup();
         }
    }
    tst_exit();
}
