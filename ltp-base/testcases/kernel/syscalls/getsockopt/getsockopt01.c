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
 * Test Name: getsockopt01
 *
 * Test Description:
 *  Verify that getsockopt() returns the proper errno for various failure cases
 *
 * Usage:  <for command-line>
 *  getsockopt01 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *	       -i n : Execute test n times.
 *	       -I x : Execute test for x seconds.
 *	       -P x : Pause for x seconds between iterations.
 *	       -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS:
 *  None.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/ioctl.h>

#include <netinet/in.h>

#include "test.h"
#include "usctest.h"

char *TCID="getsockopt01";		/* Test program identifier.    */
int testno;

int	s;	/* socket descriptor */
struct sockaddr_in sin0, fsin1;
int	sinlen;
int	optval;
socklen_t	optlen;

void setup(void), setup0(void), setup1(void),
	cleanup(void), cleanup0(void), cleanup1(void);

struct test_case_t {		/* test case structure */
	int	domain;	/* PF_INET, PF_UNIX, ... */
	int	type;	/* SOCK_STREAM, SOCK_DGRAM ... */
	int	proto;	/* protocol number (usually 0 = default) */
	int	level;	/* IPPROTO_* */
	int	optname;
	void	*optval;
	socklen_t	*optlen;
	struct sockaddr	*sin;
	int	salen;
	int	retval;		/* syscall return value */
	int	experrno;	/* expected errno */
	void	(*setup)(void);
	void	(*cleanup)(void);
	char *desc;
} tdat[] = {
	{ PF_INET, SOCK_STREAM, 0, SOL_SOCKET, SO_OOBINLINE, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EBADF, setup0, cleanup0, "bad file descriptor" },
	{ PF_INET, SOCK_STREAM, 0, SOL_SOCKET, SO_OOBINLINE, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		ENOTSOCK, setup0, cleanup0, "bad file descriptor" },
#ifndef UCLINUX
	{ PF_INET, SOCK_STREAM, 0, SOL_SOCKET, SO_OOBINLINE, 0, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EFAULT, setup1, cleanup1, "invalid option buffer" },
	{ PF_INET, SOCK_STREAM, 0, SOL_SOCKET, SO_OOBINLINE, &optval, 0,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EFAULT, setup1, cleanup1, "invalid optlen" },
#endif /* UCLINUX */
	{ PF_INET, SOCK_STREAM, 0, 500, SO_OOBINLINE, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EOPNOTSUPP, setup1, cleanup1, "invalid level" },
	{ PF_INET, SOCK_STREAM, 0, IPPROTO_UDP, SO_OOBINLINE, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EOPNOTSUPP, setup1, cleanup1, "invalid option name" },
	{ PF_INET, SOCK_STREAM, 0, IPPROTO_UDP, SO_OOBINLINE, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		EOPNOTSUPP, setup1, cleanup1, "invalid option name (UDP)" },
	{ PF_INET, SOCK_STREAM, 0, IPPROTO_IP, -1, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		ENOPROTOOPT, setup1, cleanup1, "invalid option name (IP)" },
	{ PF_INET, SOCK_STREAM, 0, IPPROTO_TCP, -1, &optval, &optlen,
		(struct sockaddr *)&fsin1, sizeof(fsin1), -1,
		ENOPROTOOPT, setup1, cleanup1, "invalid option name (TCP)" },
};

int TST_TOTAL=sizeof(tdat)/sizeof(tdat[0]); /* Total number of test cases. */

int exp_enos[] = {EBADF, ENOTSOCK, EFAULT, EOPNOTSUPP, ENOPROTOOPT, 0};

extern int Tst_count;

int
main(int argc, char *argv[])
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */

	/* Parse standard options given to run the test. */
	msg = parse_opts(argc, argv, (option_t *) NULL, NULL);
	if (msg != (char *) NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
		tst_exit();
	}

	setup();

	/* Check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); ++lc) {
		Tst_count = 0;
		for (testno=0; testno < TST_TOTAL; ++testno) {
			tdat[testno].setup();

			TEST(getsockopt(s, tdat[testno].level,
				tdat[testno].optname, tdat[testno].optval,
				tdat[testno].optlen));
			TEST_ERROR_LOG(TEST_ERRNO);
			if (TEST_RETURN != tdat[testno].retval ||
			    (TEST_RETURN < 0 &&
			     TEST_ERRNO != tdat[testno].experrno)) {
				tst_resm(TFAIL, "%s ; returned"
					" %d (expected %d), errno %d (expected"
					" %d)", tdat[testno].desc,
					TEST_RETURN, tdat[testno].retval,
					TEST_ERRNO, tdat[testno].experrno);
			} else {
				tst_resm(TPASS, "%s successful",
					tdat[testno].desc);
			}
			tdat[testno].cleanup();
		}
	}
	cleanup();

	/*NOTREACHED*/
	return(0);
}	/* End main */

void
setup(void)
{
	TEST_PAUSE;	/* if -P option specified */

	/* set up expected error numbers */
	TEST_EXP_ENOS(exp_enos);

	/* initialize local sockaddr */
	sin0.sin_family = AF_INET;
	sin0.sin_port = 0;
	sin0.sin_addr.s_addr = INADDR_ANY;
}

void
cleanup(void)
{
	TEST_CLEANUP;
	tst_exit();
}


void 
setup0(void)
{
	if (tdat[testno].experrno == EBADF)
		s = 400;	/* anything not an open file */
	else
		if((s = open("/dev/null", O_WRONLY)) == -1)
			tst_brkm(TBROK, cleanup, "error opening /dev/null - "
			"errno: %s", strerror(errno));
}

void
cleanup0(void)
{
	s = -1;
}

void
setup1(void)
{
	s = socket(tdat[testno].domain, tdat[testno].type, tdat[testno].proto);
	if (s < 0) {
		tst_brkm(TBROK, cleanup,"socket setup failed for getsockopt: "
			"%s", strerror(errno));
	}
	if (bind(s, (struct sockaddr *)&sin0, sizeof(sin0)) < 0) {
		tst_brkm(TBROK, cleanup, "socket bind failed for getsockopt: "
			"%s", strerror(errno));
	}
	sinlen = sizeof(fsin1);
	optlen = sizeof(optval);
}

void
cleanup1(void)
{
	(void) close(s);
	s = -1;
}
