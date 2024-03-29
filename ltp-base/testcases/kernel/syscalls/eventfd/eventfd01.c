/*
 *   Copyright (c) 2008 Vijay Kumar B. <vijaykumar@bravegnu.org>
 *
 *   Based on testcases/kernel/syscalls/waitpid/waitpid01.c
 *   Original copyright message:
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
 *	eventfd01.c
 *
 * DESCRIPTION
 *      Test cases for eventfd syscall.
 *
 * USAGE:  <for command-line>
 *      eventfd01 [-c n] [-i n] [-I x] [-P x] [-t]
 *      where,  -c n : Run n copies concurrently.
 *              -i n : Execute test n times.
 *              -I x : Execute test for x seconds.
 *              -P x : Pause for x seconds between iterations.
 *              -t   : Turn on syscall timing.
 *
 * History
 *	07/2008 Vijay Kumar
 *		Initial Version.
 *
 * Restrictions
 *	None
 */

#include "config.h"

#include <sys/select.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <poll.h>

#include <test.h>
#include <usctest.h>
#include <linux_syscall_numbers.h>

#ifdef HAVE_LIBAIO_H
#include <libaio.h>
#endif

static void setup(void);
static void cleanup(void);

TCID_DEFINE(eventfd01);
int TST_TOTAL = 15;
extern int Tst_count;

static int
myeventfd(unsigned int initval, int flags)
{
  /* eventfd2 uses FLAGS but eventfd doesn't take FLAGS. */
#if defined (__NR_eventfd)
    return syscall(__NR_eventfd, initval);
#else
    errno = ENOSYS;
    return -1;
#endif
}

/*
 * clear_counter() - clears the counter by performing a dummy read
 * @fd: the eventfd
 *
 * RETURNS:
 * 0 on success, and -1 on failure
 */
static int
clear_counter(int fd)
{
	uint64_t dummy;
	int ret;

	ret = read(fd, &dummy, sizeof(dummy));
	if (ret == -1) {
		if (errno != EAGAIN) {
			tst_resm(TINFO, "error clearing counter: %s",
				 strerror(errno));
			return -1;
		}
	}

	return 0;
}

/*
 * set_counter() - sets the count to specified value
 * @fd: the eventfd
 * @val: the value to be set
 *
 * Clears the counter and sets the counter to @val.
 *
 * RETURNS:
 * 0 on success, -1 on failure
 */
static int
set_counter(int fd, uint64_t val)
{
	int ret;

	ret = clear_counter(fd);
	if (ret == -1) {
		return -1;
	}

	ret = write(fd, &val, sizeof(val));
	if (ret == -1) {
		tst_resm(TINFO, "error setting counter value: %s",
			 strerror(errno));
		return -1;
	}

	return 0;
}

/*
 * Test whether the current value of the counter matches @required.
 */
static void
read_test(int fd, uint64_t required)
{
	int ret;
	uint64_t val;

	ret = read(fd, &val, sizeof(val));
	if (ret == -1) {
		tst_resm(TBROK, "error reading eventfd: %s",
			 strerror(errno));
		return;
	}

	if (val == required)
		tst_resm(TPASS, "counter value matches required");
	else
		tst_resm(TFAIL, "counter value mismatch: "
			 "required: %llu, got: %llu", required, val);
}

/*
 * Test whether read returns with error EAGAIN when counter is at 0.
 */
static void
read_eagain_test(int fd)
{
	int ret;
	uint64_t val;

	ret = clear_counter(fd);
	if (ret == -1) {
		tst_resm(TBROK, "error clearing counter");
		return;
	}

	ret = read(fd, &val, sizeof(val));
	if (ret == -1) {
		if (errno == EAGAIN)
			tst_resm(TPASS, "read failed with EAGAIN as expected");
		else
			tst_resm(TFAIL, "read failed with unexpected "
				 "error: %s", strerror(errno));
	} else {
		tst_resm(TFAIL, "read returned with %d");
	}
}

/*
 * Test whether writing to counter works.
 */
static void
write_test(int fd)
{
	int ret;
	uint64_t val;

	val = 12;

	ret = set_counter(fd, val);
	if (ret == -1) {
		tst_resm(TBROK, "error setting counter value to %lld", val);
		return;
	}

	read_test(fd, val);
}

/*
 * Test whether write returns with error EAGAIN when counter is at
 * (UINT64_MAX - 1).
 */
static void
write_eagain_test(int fd)
{
	int ret;
	uint64_t val;

	ret = set_counter(fd, UINT64_MAX - 1);
	if (ret == -1) {
		tst_resm(TBROK, "error setting counter value to UINT64_MAX-1");
		return;
	}

	val = 1;
	ret = write(fd, &val, sizeof(val));
	if (ret == -1) {
		if (errno == EAGAIN)
			tst_resm(TPASS, "write failed with EAGAIN as "
				 "expected");
		else
			tst_resm(TFAIL, "write returned with unexpected "
				 "error: %s", strerror(errno));
	} else {
		tst_resm(TFAIL, "write returned with %d", ret);
	}
}

/*
 * Test whether read returns with error EINVAL, if buffer size is less
 * than 8 bytes.
 */
static void
read_einval_test(int fd)
{
	uint32_t invalid;
	int ret;

	ret = read(fd, &invalid, sizeof(invalid));
	if (ret == -1) {
		if (errno == EINVAL) {
			tst_resm(TPASS, "read failed with EINVAL as expected");
		} else {
			tst_resm(TFAIL, "read returned with unexpected "
				 "error: %s", strerror(errno));
		}
	} else {
		tst_resm(TFAIL, "read returned with %d", ret);
	}
}

/*
 * Test whether write returns with error EINVAL, if buffer size is
 * less than 8 bytes.
 */
static void
write_einval_test(int fd)
{
	uint32_t invalid;
	int ret;

	ret = write(fd, &invalid, sizeof(invalid));
	if (ret == -1) {
		if (errno == EINVAL) {
			tst_resm(TPASS, "write failed with EINVAL as "
				 "expected");
		} else {
			tst_resm(TFAIL, "write returned with unexpected "
				 "error: %s", strerror(errno));
		}
	} else {
		tst_resm(TFAIL, "write returned with %d", ret);
	}
}

/*
 * Test wheter write returns with error EINVAL, when the written value
 * is 0xFFFFFFFFFFFFFFFF.
 */
static void
write_einval2_test(int fd)
{
	int ret;
	uint64_t val;

	ret = clear_counter(fd);
	if (ret == -1) {
		tst_resm(TBROK, "error clearing counter");
		return;
	}

	val = 0xffffffffffffffffLL;
	ret = write(fd, &val, sizeof(val));
	if (ret == -1) {
		if (errno == EINVAL)
			tst_resm(TPASS, "write failed with EINVAL as "
				 "expected");
		else
			tst_resm(TFAIL, "write returned with unexpected "
				"error: %s", strerror(errno));
	} else {
		tst_resm(TFAIL, "write returned with %d", ret);
	}
}

/*
 * Test whether readfd is set by select when counter value is
 * non-zero.
 */
static void
readfd_set_test(int fd)
{
	int ret;
	fd_set readfds;
	struct timeval timeout = { 0, 0 };
	uint64_t non_zero = 10;

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

	ret = set_counter(fd, non_zero);
	if (ret == -1) {
		tst_resm(TBROK, "error setting counter value to %lld",
			 non_zero);
		return;
	}

	ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
	if (ret == -1) {
		/* EINTR cannot occur, since we don't block. */
		tst_resm(TBROK, "select: error getting fd status: %s",
			 strerror(errno));
		return;
	}

	if (FD_ISSET(fd, &readfds))
		tst_resm(TPASS, "fd is set in readfds");
	else
		tst_resm(TFAIL, "fd is not set in readfds");
}

/*
 * Test whether readfd is not set by select when counter value is
 * zero.
 */
static void
readfd_not_set_test(int fd)
{
	int ret;
	fd_set readfds;
	struct timeval timeout = { 0, 0 };

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

	ret = clear_counter(fd);
	if (ret == -1) {
		tst_resm(TBROK, "error clearing counter");
		return;
	}

	ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
	if (ret == -1) {
		/* EINTR cannot occur, since we don't block. */
		tst_resm(TBROK, "select: error getting fd status: %s",
			 strerror(errno));
		return;
	}

	if (!FD_ISSET(fd, &readfds))
		tst_resm(TPASS, "fd is not set in readfds");
	else
		tst_resm(TFAIL, "fd is set in readfds");
}

/*
 * Test whether writefd is set by select when counter value is not the
 * maximum counter value.
 */
static void
writefd_set_test(int fd)
{
	int ret;
	fd_set writefds;
	struct timeval timeout = { 0, 0 };
	uint64_t non_max = 10;

	FD_ZERO(&writefds);
	FD_SET(fd, &writefds);

	ret = set_counter(fd, non_max);
	if (ret == -1) {
		tst_resm(TBROK, "error setting counter value to %lld", 
			 non_max);
		return;
	}

	ret = select(fd + 1, NULL, &writefds, NULL, &timeout);
	if (ret == -1) {
		/* EINTR cannot occur, since we don't block. */
		tst_resm(TBROK, "select: error getting fd status: %s",
			 strerror(errno));
		return;
	}

	if (FD_ISSET(fd, &writefds))
		tst_resm(TPASS, "fd is set in writefds");
	else
		tst_resm(TFAIL, "fd is not set in writefds");
}

/*
 * Test whether writefd is not set by select when counter value is at
 * (UINT64_MAX - 1).
 */
static void
writefd_not_set_test(int fd)
{
	int ret;
	fd_set writefds;
	struct timeval timeout = { 0, 0 };

	FD_ZERO(&writefds);
	FD_SET(fd, &writefds);

	ret = set_counter(fd, UINT64_MAX - 1);
	if (ret == -1) {
		tst_resm(TBROK, "error setting counter value to UINT64_MAX-1");
		return;
	}

	ret = select(fd + 1, NULL, &writefds, NULL, &timeout);
	if (ret == -1) {
		/* EINTR cannot occur, since we don't block. */
		tst_resm(TBROK, "select: error getting fd status: %s",
			 strerror(errno));
		return;
	}

	if (!FD_ISSET(fd, &writefds))
		tst_resm(TPASS, "fd is not set in writefds");
	else
		tst_resm(TFAIL, "fd is set in writefds");
}

/*
 * Test whether counter update in child is reflected in the parent.
 */
static void
child_inherit_test(int fd)
{
	uint64_t val;
	pid_t cpid;
	int ret;
	int status;
	uint64_t to_parent = 0xdeadbeef;
	uint64_t dummy;

	cpid = FORK_OR_VFORK();
	if (cpid == -1)
		tst_resm(TBROK, "error while forking child: %s",
			 strerror(errno));
	if (cpid != 0) {
		/* Parent */
		ret = wait(&status);
		if (ret == -1) {
			tst_resm(TBROK, "error getting child exit status");
			return;
		}

		if (WEXITSTATUS(status) == 1) {
			tst_resm(TBROK, "counter value write not "
				 "succesful in child");
			return;
		}

		ret = read(fd, &val, sizeof(val));
		if (ret == -1) {
			tst_resm(TBROK, "error reading eventfd: %s",
				 strerror(errno));
			return;
		}

		if (val == to_parent)
			tst_resm(TPASS, "counter value write from "
				 "child successful");
		else
			tst_resm(TFAIL, "counter value write in child "
				 "failed");
	} else {
		/* Child */
		ret = read(fd, &dummy, sizeof(dummy));
		if (ret == -1 && errno != EAGAIN) {
			tst_resm(TWARN, "error clearing counter: %s",
				 strerror(errno));
			exit(1);
		}

		ret = write(fd, &to_parent, sizeof(to_parent));
		if (ret == -1) {
			tst_resm(TWARN, "error writing eventfd: %s",
				 strerror(errno));
			exit(1);
		}

		exit(0);
	}
}

#ifdef HAVE_IO_SET_EVENTFD
/*
 * Test whether counter overflow is detected and handled correctly. 
 *
 * It is not possible to directly overflow the counter using the
 * write() syscall. Overflows occur when the counter is incremented
 * from kernel space, in an irq context, when it is not possible to
 * block the calling thread of execution.
 *
 * The AIO subsystem internally uses eventfd mechanism for
 * notification of completion of read or write requests. In this test
 * we trigger a counter overflow, by setting the counter value to the
 * max possible value initially. When the AIO subsystem notifies
 * through the eventfd counter, the counter overflows.
 *
 * NOTE: If the the counter starts from an initial value of 0, it will
 * take decades for an overflow to occur. But since we set the initial
 * value to the max possible counter value, we are able to cause it to
 * overflow with a single increment.
 *
 * When the counter overflows, the following are tested
 *   1. Check whether POLLERR event occurs in poll() for the eventfd.
 *   2. Check whether readfd_set/writefd_set is set in select() for the
        eventfd.
 *   3. The counter value is UINT64_MAX.
 */
static int 
trigger_eventfd_overflow(int evfd, int *fd, io_context_t *ctx)
{
	int ret;
	struct iocb iocb;
	struct iocb *iocbap[1];
	static char buf[4 * 1024];

	*ctx = 0;
	ret = io_setup(16, ctx);
	if (ret < 0) {
		tst_resm(TINFO, "io_setup error: %s", strerror(-ret));
		return -1;
	}

	*fd = open("testfile", O_RDWR | O_CREAT, 0644);
	if (*fd == -1) {
		tst_resm(TINFO, "error creating tmp file: %s", 
			 strerror(errno));
		goto err_io_destroy;
	}

	ret = set_counter(evfd, UINT64_MAX - 1);
	if (ret == -1) {
		tst_resm(TINFO, "error setting counter to UINT64_MAX-1");
		goto err_close_file;
	}

	io_prep_pwrite(&iocb, *fd, buf, sizeof(buf), 0);
	io_set_eventfd(&iocb, evfd);
	
	iocbap[0] = &iocb;
	ret = io_submit(*ctx, 1, iocbap);
	if (ret < 0) {
		tst_resm(TINFO, "error submitting iocb: %s", strerror(-ret));
		goto err_close_file;
	}

	return 0;

 err_close_file:
	close(*fd);

 err_io_destroy:	
	io_destroy(*ctx);

	return -1;
}

static void
cleanup_overflow(int fd, io_context_t ctx)
{
	close(fd);
	io_destroy(ctx);
}

static void
overflow_select_test(int evfd)
{
	struct timeval timeout = { 10, 0 };
	fd_set readfds;
	int fd;
	io_context_t ctx;
	int ret;

	ret = trigger_eventfd_overflow(evfd, &fd, &ctx);
	if (ret == -1) {
		tst_resm(TBROK, "error triggering eventfd overflow");
		return;
	}
	
	FD_ZERO(&readfds);
	FD_SET(evfd, &readfds);
	ret = select(evfd + 1, &readfds, NULL, NULL, &timeout);
	if (ret == -1) {
		tst_resm(TBROK, "error getting evfd status with select: %s", 
			 strerror(errno));
		goto err_cleanup;
	}

	if (FD_ISSET(evfd, &readfds))
		tst_resm(TPASS, "read fd set as expected");
	else
		tst_resm(TFAIL, "read fd not set");

 err_cleanup:
	cleanup_overflow(fd, ctx);
}

static void
overflow_poll_test(int evfd)
{
	struct pollfd pollfd;
	int fd;
	io_context_t ctx;
	int ret;

	ret = trigger_eventfd_overflow(evfd, &fd, &ctx);
	if (fd == -1) {
		tst_resm(TBROK, "error triggering eventfd overflow");
		return;
	}

	pollfd.fd = evfd;
	pollfd.events = POLLIN;
	pollfd.revents = 0;
	ret = poll(&pollfd, 1, 10000);
	if (ret == -1) {
		tst_resm(TBROK, "error getting evfd status with poll: %s", 
			 strerror(errno));
		goto err_cleanup;
	}
	if (pollfd.revents & POLLERR)
		tst_resm(TPASS, "POLLERR occurred as expected");
	else
		tst_resm(TFAIL, "POLLERR did not occur");

 err_cleanup:
	cleanup_overflow(fd, ctx);
}

static void
overflow_read_test(int evfd)
{
	uint64_t count;
	io_context_t ctx;
	int fd;
	int ret;

	ret = trigger_eventfd_overflow(evfd, &fd, &ctx);
	if (ret == -1) {
		tst_resm(TBROK, "error triggering eventfd overflow");
		return;
	}
	
	ret = read(evfd, &count, sizeof(count)); 
	if (ret == -1) {
		tst_resm(TBROK, "error reading eventfd: %s", strerror(errno));
		goto err_cleanup;
	}

	if (count == UINT64_MAX)
		tst_resm(TPASS, "overflow occurred as expected");
	else
		tst_resm(TFAIL, "overflow did not occur");

 err_cleanup:
	cleanup_overflow(fd, ctx);
}
#else
static void
overflow_select_test(int evfd)
{
	tst_resm(TCONF, "eventfd support is not available in AIO subsystem");
}

static void
overflow_poll_test(int evfd)
{
	tst_resm(TCONF, "eventfd support is not available in AIO subsystem");
}

static void
overflow_read_test(int evfd)
{
	tst_resm(TCONF, "eventfd support is not available in AIO subsystem");
}
#endif

int
main(int argc, char **argv)
{
	int lc;				/* loop counter */
	char *msg;			/* message returned from parse_opts */
	int fd;

	/* parse standard options */
	msg = parse_opts(argc, argv, (option_t *) NULL, NULL);
	if (msg != NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
		tst_exit();
		/* NOTREACHED */
	}

	setup();

	/* check for looping state if -i option is given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		int ret;
		uint64_t einit = 10;

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		fd = myeventfd(einit, 0);
		if (fd == -1)
			tst_brkm(TBROK, cleanup, "error creating eventfd: %s",
				 strerror(errno));

		ret = fcntl(fd, F_SETFL, O_NONBLOCK);
		if (ret == -1)
			tst_brkm(TBROK, cleanup,
				 "error setting non-block mode: %s", strerror);

		read_test(fd, einit);
		read_eagain_test(fd);
		write_test(fd);
		write_eagain_test(fd);
		read_einval_test(fd);
		write_einval_test(fd);
		write_einval2_test(fd);
		readfd_set_test(fd);
		readfd_not_set_test(fd);
		writefd_set_test(fd);
		writefd_not_set_test(fd);
		child_inherit_test(fd);
		overflow_select_test(fd);
		overflow_poll_test(fd);
		overflow_read_test(fd);

		close(fd);
	}

	cleanup();
	/* NOT REACHED */

	return 0;
}

/*
 * setup() - performs all ONE TIME setup for this test
 */
static void
setup(void)
{
	/* capture signals */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	if (tst_kvercmp(2, 6, 22) < 0)
		tst_brkm(TCONF, cleanup, "2.6.22 or greater kernel required");

	/* Pause if that option was specified
	 * TEST_PAUSE contains the code to fork the test with the -c option.
	 */
	TEST_PAUSE;
}

/*
 * cleanup() - performs all ONE TIME cleanup for this test
 */
static void
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
