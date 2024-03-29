/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

/* Note: The standard says fputs returns a nonnegative number on
 * success.  In this implementation, we return the length of the
 * string written on success.
 */

#ifdef __DO_UNLOCKED

int attribute_hidden __fputs_unlocked(register const char * __restrict s,
					 FILE * __restrict stream)
{
	size_t n = __strlen(s);

	return ((__fwrite_unlocked(s, 1, n, stream) == n) ? n : EOF);
}

#ifndef __TCS__
weak_alias(__fputs_unlocked,fputs_unlocked);
#else
int attribute_hidden fputs_unlocked(register const char * __restrict s,
					 FILE * __restrict stream)
{
	return __fputs_unlocked(s,stream);
}
#endif
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fputs_unlocked,fputs);
#else
int attribute_hidden fputs(register const char * __restrict s,
					 FILE * __restrict stream)
{
	return __fputs_unlocked(s,stream);
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

int fputs(const char * __restrict s, register FILE * __restrict stream)
{
	int retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fputs_unlocked(s, stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
