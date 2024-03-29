/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

wint_t __getwchar_unlocked(void)
{
	return __fgetwc_unlocked(stdin);
}

#ifndef __TCS__
weak_alias(__getwchar_unlocked,getwchar_unlocked);
#else
wint_t getwchar_unlocked(void)
{
	return __getwchar_unlocked();
}
#endif 
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__getwchar_unlocked,getwchar);
#else
wint_t getwchar(void)
{
	return __getwchar_unlocked();
}
#endif
#endif

#elif defined __UCLIBC_HAS_THREADS__

wint_t getwchar(void)
{
	return fgetwc(stdin);
}

#endif
