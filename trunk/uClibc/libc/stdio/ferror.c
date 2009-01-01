/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

int __ferror_unlocked(register FILE *stream)
{
	__STDIO_STREAM_VALIDATE(stream);

	return __FERROR_UNLOCKED(stream);
}

#ifndef __TCS__
weak_alias(__ferror_unlocked,ferror_unlocked);
#else
/*int ferror_unlocked(register FILE *stream)*/
/*{*/
/*return __ferror_unlocked(stream);*/
/*}*/
#endif 
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__ferror_unlocked,ferror);
#else
int tmlinux_ferror(register FILE *stream)
{
	return __ferror_unlocked(stream);
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

int tmlinux_ferror(register FILE *stream)
{
	int retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	__STDIO_STREAM_VALIDATE(stream);

	retval = __FERROR_UNLOCKED(stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
