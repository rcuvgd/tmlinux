/*
 * Copyright (c) 2008 Parallels.  All Rights Reserved.
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
 * Started by Andrew Vagin <avagin@gmail.com>
 *
 */
/*
 * NAME
 *	inotify03
 *
 * DESCRIPTION
 *	Check that inotify get IN_UNMOUNT event and
 *	don't block the umount command.
 *
 * ALGORITHM
 *	Execute sequence file's operation and check return events
 *
 */
#include "config.h"

#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <signal.h>
#include "test.h"
#include "usctest.h"

#if defined(HAVE_SYS_INOTIFY) && defined(__NR_inotify_init)
#include <sys/inotify.h>

#define EVENT_MAX 1024
/* size of the event structure, not counting name */
#define EVENT_SIZE (sizeof (struct inotify_event))
/* reasonable guess as to size of 1024 events */
#define EVENT_BUF_LEN		(EVENT_MAX * (EVENT_SIZE + 16))

void help(void);
void setup();
void cleanup();

char *TCID="inotify03";		/* Test program identifier.	*/
int TST_TOTAL = 3;		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

#define BUF_SIZE 1024
char fname[BUF_SIZE];
char buf[BUF_SIZE];
int fd, fd_notify;
int wd;

int event_set[EVENT_MAX];

char event_buf[EVENT_BUF_LEN];

static long myinotify_init()
{
	return syscall(__NR_inotify_init);
}

static long myinotify_add_watch(int fd, const char *pathname, int mask)
{
	return syscall(__NR_inotify_add_watch, fd, pathname, mask);
}

static long myinotify_rm_watch(int fd, int wd)
{
	return syscall(__NR_inotify_rm_watch, fd, wd);
}

#define DEFAULT_FSTYPE	"ext2"
#define DIR_MODE	S_IRWXU | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP

static char *Fstype;
static char mntpoint[20];
static int mount_flag = 0;
static char *fstype;
static char *device;
static int Tflag = 0;
static int Dflag = 0;

static option_t options[] = {		/* options supported by mount01 test */
	{ "T:", &Tflag, &fstype },	/* -T type of filesystem	*/
	{ "D:", &Dflag, &device },	/* -D device used for mounting	*/
	{ NULL, NULL, NULL }
};

int main(int ac, char **av){
	char *msg;		/* message returned from parse_opts */
	int ret;
	int len, i, test_num;

	/* parse standard options */
	msg = parse_opts(ac, av, (option_t *) options, &help);
	if ( msg != (char *) NULL )
		tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);

	/* Check for mandatory option of the testcase */
	if (!Dflag) {
		tst_brkm(TBROK, NULL, "You must specifiy the device used for "
			" mounting with -D option, Run '%s  -h' for option "
			" information.", TCID);
		tst_exit();
	}

	if (Tflag) {
		Fstype = (char *) malloc(strlen(fstype)+1);
		if(Fstype == NULL) {
			tst_brkm(TBROK, NULL, "malloc - failed to alloc %d"
				"errno %d", strlen(fstype), errno);
		}
		strncpy(Fstype, fstype, strlen(fstype)+1);
	} else {
		Fstype = (char *) malloc(strlen(DEFAULT_FSTYPE)+1);
		if(Fstype == NULL) {
			tst_brkm(TBROK, NULL, "malloc - failed to alloc %d"
				"errno %d", strlen(DEFAULT_FSTYPE), errno);
		}
		strncpy(Fstype, DEFAULT_FSTYPE, strlen(DEFAULT_FSTYPE)+1);
	}

	/* perform global setup for test */
	setup();

	Tst_count = 0;

	event_set[Tst_count] = IN_UNMOUNT;
	Tst_count++;
	event_set[Tst_count] = IN_IGNORED;
	Tst_count++;

	/*check exit code from inotify_rm_watch*/
	Tst_count++;

	if (TST_TOTAL != Tst_count) {
            tst_brkm(TBROK, cleanup,
                    "TST_TOTAL and Tst_count are not equal");
        }
        Tst_count = 0;

	tst_resm(TINFO, "umount %s", device);
	TEST(umount(mntpoint));
	if (TEST_RETURN != 0) {
		TEST_ERROR_LOG(TEST_ERRNO);
		tst_brkm(TBROK, cleanup, "umount(2) Failed "
			"while unmounting errno = %d : %s",
			TEST_ERRNO, strerror(TEST_ERRNO));
	}
	mount_flag = 0;

	len = read(fd_notify, event_buf, EVENT_BUF_LEN);
	if (len < 0) {
		tst_brkm(TBROK, cleanup,
			"read(%d, buf, %d) Failed, errno=%d : %s",
			fd_notify, EVENT_BUF_LEN, errno,
			strerror(errno));
	}

	/* check events */
	test_num = 0;
	i = 0;
	while (i < len) {
		struct inotify_event *event;
		event = (struct inotify_event *) &event_buf[i];
		if (test_num >= (TST_TOTAL - 1)) {
			tst_resm(TFAIL,
				"get unnecessary event: wd=%d mask=%x "
				"cookie=%u len=%u",
				event->wd, event->mask,
				event->cookie, event->len);
		} else if (event_set[test_num] == event->mask){
			tst_resm(TPASS, "get event: wd=%d mask=%x"
				" cookie=%u len=%u",
				event->wd, event->mask,
				event->cookie, event->len);

		} else {
			tst_resm( TFAIL, "get event: wd=%d mask=%x "
				"(expected %x) cookie=%u len=%u",
				event->wd, event->mask,
				event_set[test_num],
				event->cookie, event->len);
		}
		test_num++;
		i += EVENT_SIZE + event->len;
	}
	for (; test_num<TST_TOTAL - 1; test_num++){
		tst_resm(TFAIL, "don't get event: mask=%x ",
				event_set[test_num]);

	}
	ret = myinotify_rm_watch(fd_notify, wd);
	if (ret != -1 || errno != EINVAL)
		tst_resm(TFAIL,	"inotify_rm_watch (%d, %d) return %d "
			"errno=%d : %s (instead of %d)",
			fd_notify, wd, ret, errno, EINVAL, strerror(errno));
	else
		tst_resm(TPASS,	"inotify_rm_watch (%d, %d) return %d "
			"errno=%d : %s",
			fd_notify, wd, ret, errno, strerror(errno));

	/* cleanup and exit */
	cleanup();

	return 0;
}	/* End main */

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void setup()
{
	int ret;
	/* capture signals */
	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temp directory and cd to it */
	tst_tmpdir();

	(void)sprintf(mntpoint, "mnt_%d", getpid());

	if (mkdir(mntpoint, DIR_MODE) < 0) {
		tst_brkm(TBROK, cleanup, "mkdir(%s, %#o) failed; "
			"errno = %d: %s", mntpoint, DIR_MODE, errno,
			strerror(errno));
	}

	/* Call mount(2) */
	tst_resm(TINFO, "mount %s to %s fstype=%s", device, mntpoint, Fstype);
	TEST(mount(device, mntpoint, Fstype, 0, NULL));

	/* check return code */
	if (TEST_RETURN != 0) {
		TEST_ERROR_LOG(TEST_ERRNO);
		tst_brkm(TBROK, cleanup, "mount(2) Failed errno = %d : %s",
			TEST_ERRNO, strerror(TEST_ERRNO));
	}
	mount_flag = 1;

	sprintf(fname,"%s/tfile_%d", mntpoint, getpid());
	fd = open(fname,O_RDWR|O_CREAT,0700);
	if (fd == -1) {
		tst_brkm(TBROK, cleanup,
			"open(%s, O_RDWR|O_CREAT,0700) Failed, errno=%d : %s",
			fname, errno, strerror(errno));
	}

	ret = write(fd, fname, 1);
	if (ret == -1) {
		tst_brkm(TBROK, cleanup,
				"write(%d, %s, 1) Failed, errno=%d : %s",
				fd, fname, errno, strerror(errno));
	}

	/* close the file we have open */
	if (close(fd) == -1) {
		tst_brkm(TBROK, cleanup,
				"close(%s) Failed, errno=%d : %s",
				fname, errno, strerror(errno));
	}

	fd_notify = myinotify_init();

	if (fd_notify < 0) {
		if( errno == ENOSYS ){
			tst_resm(TCONF, "inotify is not configured in this kernel.");
			tst_resm(TCONF, "Test will not run.");
			cleanup();
			tst_exit();
		}else{
			tst_brkm(TBROK, cleanup,
				"inotify_init () Failed, errno=%d : %s",
				errno, strerror(errno));
		}
	}

	wd = myinotify_add_watch (fd_notify, fname, IN_ALL_EVENTS);
	if (wd < 0) {
		tst_brkm(TBROK, cleanup,
				"inotify_add_watch (%d, %s, IN_ALL_EVENTS)"
				"Failed, errno=%d : %s",
				fd_notify, fname, errno, strerror(errno));
	};

}	/* End setup() */


/*
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 */
void cleanup()
{
	free(Fstype);
	if (close(fd_notify) == -1) {
		tst_resm(TWARN, "close(%d) Failed, errno=%d : %s",
				fd_notify, errno, strerror(errno));
	}

	if (mount_flag) {
		TEST(umount(mntpoint));
		if (TEST_RETURN != 0) {
			TEST_ERROR_LOG(TEST_ERRNO);
			tst_resm(TWARN, "umount(2) Failed "
				"while unmounting errno = %d : %s",
				TEST_ERRNO, strerror(TEST_ERRNO));
		}
	}

	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

	/* Remove tmp dir and all files in it */
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}	/* End cleanup() */

/*
 * issue a help message
 */
void help()
{
	printf("-T type : specifies the type of filesystem to be mounted."
		" Default ext2. \n");
	printf("-D device : device used for mounting \n");
}

#else

char *TCID="inotify03";	/* Test program identifier.	*/
int TST_TOTAL = 0;		/* Total number of test cases. */

int
main()
{
#ifndef __NR_inotify_init
	tst_resm(TCONF, "This test needs a kernel that has inotify syscall.");
	tst_resm(TCONF, "Inotify syscall can be found at kernel 2.6.13 or higher.");
	return 0;
#endif
#ifndef HAVE_SYS_INOTIFY
	tst_resm(TBROK, "can't find header sys/inotify.h");
	return 1;
#endif
	return 0;
}

#endif
