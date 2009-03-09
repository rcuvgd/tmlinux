#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char test_name6[1000];

int
main(int ac, char **av)
{
	int lc;				/* loop counter */
	int fd;

	/* creat() and close a zero length file with executeable permission */
	sprintf(test_name6, "tst_execve03");

	if ((fd = creat(test_name6, 0755)) == -1) {
		printf("creat() failed!\n");
		return -1;
	}
	if (close(fd) == -1) {
		printf("close() failed!\n");
		return -1;
	}

	/* check looping state if -i option given */
	for (lc = 0; lc < 5; lc++) {
		int ret=0;

		errno = 0;

		ret = execve(test_name6, NULL, NULL);

		if (ret != -1) {
			printf("FAIL %d: call succeeded unexpectedly\n", lc);
			continue;
		}

		if (errno == ENOEXEC) {
			printf("PASS %d: expected failure - "
					"errno = %d : %s\n", 
					lc , 
					errno,
					strerror(errno));
		} else {
			printf("FAIL %d: unexpected error - %d : %s - "
					"expected %d\n", 
					lc ,
					errno,
					strerror(errno), 
					ENOEXEC);
		}
	}

	return(0);
}
