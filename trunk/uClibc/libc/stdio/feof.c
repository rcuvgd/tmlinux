/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __TCS__
#undef feof
#define feof   tmlinux_feof
#endif 

#ifdef __DO_UNLOCKED

int __feof_unlocked(register FILE *stream)
{
	__STDIO_STREAM_VALIDATE(stream);

	return __FEOF_UNLOCKED(stream);
}

#ifndef __TCS__
weak_alias(__feof_unlocked,feof_unlocked);
#else
/*int feof_unlocked(register FILE *stream)*/
/*{*/
/*return __feof_unlocked(stream);*/
/*}*/
#endif 
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__feof_unlocked,feof);
#else
int feof(register FILE *stream)
{
	return __feof_unlocked(stream);
}
#endif 
#endif

#elif defined __UCLIBC_HAS_THREADS__

int feof(register FILE *stream)
{
	int retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	__STDIO_STREAM_VALIDATE(stream);

	retval = __FEOF_UNLOCKED(stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
