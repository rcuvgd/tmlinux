/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#undef putchar_unlocked
#undef putchar

#ifdef __DO_UNLOCKED

int __putchar_unlocked(int c)
{
	register FILE *stream = stdout;

	return __PUTC_UNLOCKED_MACRO(c, stream);
}

#ifndef __TCS__
weak_alias(__putchar_unlocked,putchar_unlocked);
#else
int putchar_unlocked(int c)
{
	return __putchar_unlocked(c);
}
#endif
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__putchar_unlocked,putchar);
#else
int putchar(int c)
{
	return __putchar_unlocked(c);
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

int putchar(int c)
{
	register FILE *stream = stdout;

	if (stream->__user_locking != 0) {
		return __PUTC_UNLOCKED_MACRO(c, stream);
	} else {
		int retval;
		__STDIO_ALWAYS_THREADLOCK(stream);
		retval = __PUTC_UNLOCKED_MACRO(c, stream);
		__STDIO_ALWAYS_THREADUNLOCK(stream);
		return retval;
	}
}

#endif
