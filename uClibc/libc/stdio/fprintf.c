/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"
#include <stdarg.h>

#ifdef __TCS_nohost__
int fprintf(FILE * __restrict stream, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	rv = vfprintf(stream, format, arg);
	va_end(arg);

	return rv;
}
#else
int fprintf(FILE * __restrict stream, const char * __restrict format, ...)
{
	va_list arg;
	int rv;

	va_start(arg, format);
	if(( stream==stdout ) || ( stream==stderr ) || ( stream==stdin )) {
		rv = vprintf(format, arg);
	} else {
		rv = vfprintf(stream, format, arg);
	}
	va_end(arg);

	return rv;
}
#endif 
