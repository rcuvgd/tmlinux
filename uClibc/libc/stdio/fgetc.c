/* Copyright (C) 2004       Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

#include "_stdio.h"

#undef fgetc
#undef fgetc_unlocked
#undef getc
#undef getc_unlocked

#ifdef __TCS__
/*
 * FIXME: to avoid symbol duplication with TCS
 * */
#define getc    tmlinux_getc
#define fgetc   tmlinux_fgetc
#endif 

#ifdef __DO_UNLOCKED

int attribute_hidden __fgetc_unlocked_internal(FILE *stream)
{
	__STDIO_STREAM_VALIDATE(stream);

	/* First the fast path.  We're good to go if getc macro enabled. */
	if (__STDIO_STREAM_CAN_USE_BUFFER_GET(stream)) {
		return __STDIO_STREAM_BUFFER_GET(stream);
	}

	/* Next quickest... reading and narrow oriented, but macro
	 * disabled and/or buffer is exhausted. */
	if (__STDIO_STREAM_IS_NARROW_READING(stream)
		|| !__STDIO_STREAM_TRANS_TO_READ(stream, __FLAG_NARROW)
		) {
		if (stream->__modeflags & __FLAG_UNGOT) { /* Use ungots first. */
			unsigned char uc = stream->__ungot[(stream->__modeflags--) & 1];
			stream->__ungot[1] = 0;
			__STDIO_STREAM_VALIDATE(stream);
			return uc;
		}

		if (__STDIO_STREAM_BUFFER_RAVAIL(stream)) {	/* Have buffered? */
			return __STDIO_STREAM_BUFFER_GET(stream);
		}

		/* Is this a fake stream for *sscanf? */
		if (__STDIO_STREAM_IS_FAKE_VSSCANF(stream)) {
			__STDIO_STREAM_SET_EOF(stream);
			return EOF;
		}

		/* We need to read from the host environment, so we must
		 * flush all line buffered streams if the stream is not
		 * fully buffered. */
		if (!__STDIO_STREAM_IS_FBF(stream)) {
			__STDIO_FLUSH_LBF_STREAMS;
		}

		if (__STDIO_STREAM_BUFFER_SIZE(stream)) { /* Do we have a buffer? */
			__STDIO_STREAM_DISABLE_GETC(stream);
			if(__STDIO_FILL_READ_BUFFER(stream)) {	/* Refill succeeded? */
				__STDIO_STREAM_ENABLE_GETC(stream);	/* FBF or LBF */
				return __STDIO_STREAM_BUFFER_GET(stream);
			}
		} else {
			unsigned char uc;
			if (__stdio_READ(stream, &uc, 1)) {
				return uc;
			}
		}
	}

	return EOF;
}

#ifndef __TCS__
strong_alias(__fgetc_unlocked_internal,__fgetc_unlocked)
weak_alias(__fgetc_unlocked_internal,fgetc_unlocked)
weak_alias(__fgetc_unlocked_internal,getc_unlocked)
#ifndef __UCLIBC_HAS_THREADS__
weak_alias(__fgetc_unlocked_internal,fgetc)
weak_alias(__fgetc_unlocked_internal,getc)
#endif
#else
int attribute_hidden __fgetc_unlocked(FILE *stream)
{
	return __fgetc_unlocked_internal(stream);
}
int attribute_hidden fgetc_unlocked(FILE *stream)
{
	return __fgetc_unlocked_internal(stream);
}
int attribute_hidden getc_unlocked(FILE *stream)
{
	return __fgetc_unlocked_internal(stream);
}
#ifndef __UCLIBC_HAS_THREADS__
int attribute_hidden fgetc(FILE *stream)
{
	return __fgetc_unlocked_internal(stream);
}
int attribute_hidden getc(FILE *stream)
{
	return __fgetc_unlocked_internal(stream);
}
#endif

#endif 

#elif defined __UCLIBC_HAS_THREADS__

int fgetc(register FILE *stream)
{
	if (stream->__user_locking != 0) {
		return __GETC_UNLOCKED_MACRO(stream);
	} else {
		int retval;
		__STDIO_ALWAYS_THREADLOCK(stream);
		retval = __GETC_UNLOCKED_MACRO(stream);
		__STDIO_ALWAYS_THREADUNLOCK(stream);
		return retval;
	}
}

#ifndef __TCS__
weak_alias(fgetc,getc);
#else
/*NOTE: do not need to implement getc, for getc is a macro which is redirected to fgetc*/
#endif 

#endif
