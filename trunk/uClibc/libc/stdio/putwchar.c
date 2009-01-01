/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

wint_t __putwchar_unlocked(wchar_t wc)
{
	return __fputwc_unlocked(wc, stdout);
}

#ifndef __TCS__
weak_alias(__putwchar_unlocked,putwchar_unlocked);
#else
wint_t putwchar_unlocked(wchar_t wc)
{
	return __putwchar_unlocked(wc);
}
#endif
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__putwchar_unlocked,putwchar);
#else
wint_t putwchar(wchar_t wc)
{
	return __putwchar_unlocked(wc);
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

wint_t putwchar(wchar_t wc)
{
	return fputc(wc, stdout);
}

#endif
