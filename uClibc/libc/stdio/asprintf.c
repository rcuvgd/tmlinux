/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"
#include <stdarg.h>

#ifndef __STDIO_HAS_VSNPRINTF
#warning Skipping asprintf and __asprintf since no vsnprintf!
#else

int __asprintf(char **__restrict buf, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vasprintf(buf, format, arg);
	va_end(arg);

	return rv;
}

#ifndef __TCS__
weak_alias(__asprintf,asprintf)
#else
int asprintf(char **__restrict buf, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vasprintf(buf, format, arg);
	va_end(arg);

	return rv;
}
#endif 

#endif
