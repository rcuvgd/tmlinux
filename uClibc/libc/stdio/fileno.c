/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

int attribute_hidden __fileno_unlocked(register FILE *stream)
{
	__STDIO_STREAM_VALIDATE(stream);

	if ((!__STDIO_STREAM_IS_CUSTOM(stream)) && (stream->__filedes >= 0)) {
		return stream->__filedes;
	}

	__set_errno(EBADF);
	return -1;
}

#ifndef __TCS__
weak_alias(__fileno_unlocked,fileno_unlocked);
#else
int attribute_hidden fileno_unlocked(register FILE *stream)
{
	return __fileno_unlocked(stream);
}
#endif 
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fileno_unlocked,fileno);
#else
int attribute_hidden fileno(register FILE *stream)
{
	return __fileno_unlocked(stream);
}
#endif 
#endif

#elif defined __UCLIBC_HAS_THREADS__

int fileno(register FILE *stream)
{
	int retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fileno_unlocked(stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
