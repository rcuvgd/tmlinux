/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#ifdef __DO_UNLOCKED

size_t attribute_hidden __fwrite_unlocked(const void * __restrict ptr, size_t size,
						 size_t nmemb, register FILE * __restrict stream)
{
	__STDIO_STREAM_VALIDATE(stream);

	/* Note: If nmbem * size > SIZE_MAX then there is an application
	 * bug since no array can be larger than SIZE_MAX in size. */

	if ((__STDIO_STREAM_IS_NARROW_WRITING(stream)
		 || !__STDIO_STREAM_TRANS_TO_WRITE(stream, __FLAG_NARROW))
		&& size && nmemb
		) {

		if (nmemb <= (SIZE_MAX / size)) {
			return __stdio_fwrite((const unsigned char *) ptr,
								  size*nmemb, stream) / size;
		}

		__STDIO_STREAM_SET_ERROR(stream);
		__set_errno(EINVAL);
	}

	return 0;
}

#ifndef __TCS__
weak_alias(__fwrite_unlocked,fwrite_unlocked);
#else
size_t attribute_hidden fwrite_unlocked(const void * __restrict ptr, size_t size,
						 size_t nmemb, register FILE * __restrict stream)
{
	return __fwrite_unlocked(ptr,size,nmemb,stream);
}
#endif
#ifndef __UCLIBC_HAS_THREADS__
#ifndef __TCS__
weak_alias(__fwrite_unlocked,fwrite);
#else
size_t attribute_hidden fwrite(const void * __restrict ptr, size_t size,
						 size_t nmemb, register FILE * __restrict stream)
{
	return __fwrite_unlocked(ptr,size,nmemb,stream);
}
#endif 
#endif

#elif defined __UCLIBC_HAS_THREADS__

size_t fwrite(const void * __restrict ptr, size_t size,
			  size_t nmemb, register FILE * __restrict stream)
{
	size_t retval;
	__STDIO_AUTO_THREADLOCK_VAR;

	__STDIO_AUTO_THREADLOCK(stream);

	retval = __fwrite_unlocked(ptr, size, nmemb, stream);

	__STDIO_AUTO_THREADUNLOCK(stream);

	return retval;
}

#endif
