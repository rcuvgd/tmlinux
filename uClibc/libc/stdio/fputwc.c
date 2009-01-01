/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

wint_t attribute_hidden __fputwc_unlocked(wchar_t wc, FILE *stream)
{
	return _wstdio_fwrite(&wc, 1, stream) ? wc : WEOF;
}

#ifndef __TCS__
weak_alias(__fputwc_unlocked,fputwc_unlocked);
#else
wint_t attribute_hidden fputwc_unlocked(wchar_t wc, FILE *stream)
{
	return __fputwc_unlocked(wc,stream);
}
#endif
#ifndef __TCS__
weak_alias(__fputwc_unlocked,putwc_unlocked);
#else
wint_t attribute_hidden putwc_unlocked(wchar_t wc, FILE *stream)
{
	return __fputwc_unlocked(wc,stream);
}
#endif
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fputwc_unlocked,fputwc);
#else
wint_t attribute_hidden fputwc(wchar_t wc, FILE *stream)
{
	return __fputwc_unlocked(wc,stream);
}
#endif
#ifndef __TCS__
weak_alias(__fputwc_unlocked,putwc);
#else
wint_t attribute_hidden putwc(wchar_t wc, FILE *stream)
{
	return __fputwc_unlocked(wc,stream);
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

wint_t fputwc(wchar_t wc, register FILE *stream)
{
	wint_t retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fputwc_unlocked(wc, stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#ifndef __TCS__
weak_alias(fputwc,putwc);
#else
wint_t putwc(wchar_t wc, register FILE *stream)
{
	return fputwc(wc,stream);
}
#endif

#endif
