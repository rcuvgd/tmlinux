/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#define wcslen __wcslen

#include "_stdio.h"

extern int __fputws_unlocked(const wchar_t *__restrict ws,
					FILE *__restrict stream) attribute_hidden;

#ifdef __DO_UNLOCKED

int attribute_hidden __fputws_unlocked(const wchar_t *__restrict ws,
					  register FILE *__restrict stream)
{
	size_t n = wcslen(ws);

	return (_wstdio_fwrite(ws, n, stream) == n) ? 0 : -1;
}

#ifndef __TCS__
weak_alias(__fputws_unlocked,fputws_unlocked)
#else
int fputws_unlocked(const wchar_t *__restrict ws, register FILE *__restrict stream)
{
	return __fputws_unlocked(ws, stream);
}
#endif 
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fputws_unlocked,fputws)
#else
int fputws(const wchar_t *__restrict ws, register FILE *__restrict stream)
{
	return __fputws_unlocked(ws, stream);
}
#endif 
#endif

#elif defined __UCLIBC_HAS_THREADS__

int fputws(const wchar_t *__restrict ws, register FILE *__restrict stream)
{
	int retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fputws_unlocked(ws, stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
