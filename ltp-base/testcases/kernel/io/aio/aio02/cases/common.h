#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include <libaio.h>
#include <sys/uio.h>
#include <assert.h>

/* Fatal error handler */
static void io_error(const char *func, int rc)
{
	if (rc == -ENOSYS)
		fprintf(stderr, "AIO not in this kernel\n");
	else if (rc < 0)
		fprintf(stderr, "%s: %s\n", func, strerror(-rc));
	else
		fprintf(stderr, "%s: error %d\n", func, rc);
	
	exit(1);
}
