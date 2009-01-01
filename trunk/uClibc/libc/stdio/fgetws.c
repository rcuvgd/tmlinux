/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

extern wchar_t *__fgetws_unlocked(wchar_t *__restrict ws, int n,
						FILE *__restrict stream) attribute_hidden;

#ifdef __DO_UNLOCKED

wchar_t attribute_hidden *__fgetws_unlocked(wchar_t *__restrict ws, int n,
						   FILE *__restrict stream)
{
	register wchar_t *p = ws;
	wint_t wi;

	__STDIO_STREAM_VALIDATE(stream);

	while ((n > 1)
		   && ((wi = __fgetwc_unlocked(stream)) != WEOF)
		   && ((*p++ = wi) != '\n')
		   ) {
		--n;
	}
	if (p == ws) {
		/* TODO -- should we set errno? */
/*  		if (n <= 0) { */
/*  			errno = EINVAL; */
/*  		} */
		return NULL;
	}
	*p = 0;
	return ws;
}

#ifndef __TCS__
weak_alias(__fgetws_unlocked,fgetws_unlocked);
#else
wchar_t attribute_hidden *fgetws_unlocked(wchar_t *__restrict ws, int n,
						   FILE *__restrict stream)
{
	return __fgetws_unlocked(ws,n,stream);
}
#endif 

#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fgetws_unlocked,fgetws);
#else
wchar_t attribute_hidden *fgetws(wchar_t *__restrict ws, int n,
						   FILE *__restrict stream)
{
	return __fgetws_unlocked(ws,n,stream);
}
#endif 
#endif

#elif defined __UCLIBC_HAS_THREADS__

wchar_t *fgetws(wchar_t *__restrict ws, int n, FILE *__restrict stream)
{
	wchar_t *retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fgetws_unlocked(ws, n, stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
