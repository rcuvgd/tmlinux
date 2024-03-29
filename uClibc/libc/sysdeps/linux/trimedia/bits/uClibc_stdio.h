/* Copyright (C) 2002-2004   Manuel Novoa III    <mjn3@codepoet.org>
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  The GNU C Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with the GNU C Library; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA.
 */

#ifndef _STDIO_H
#error Always include <stdio.h> rather than <bits/uClibc_stdio.h>
#endif

/**********************************************************************/

#define __STDIO_BUFFERS
/* ANSI/ISO mandate at least 256. */
#if defined(__UCLIBC_HAS_STDIO_BUFSIZ_NONE__)
/* Fake this because some apps use stdio.h BUFSIZ. */
#define __STDIO_BUFSIZ			256
#undef __STDIO_BUFFERS
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_256__)
#define __STDIO_BUFSIZ			256
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_512__)
#define __STDIO_BUFSIZ			512
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_1024__)
#define __STDIO_BUFSIZ		   1024
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_2048__)
#define __STDIO_BUFSIZ		   2048
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_4096__)
#define __STDIO_BUFSIZ		   4096
#elif defined(__UCLIBC_HAS_STDIO_BUFSIZ_8192__)
#define __STDIO_BUFSIZ		   8192
#else
#error config seems to be out of sync regarding bufsiz options
#endif

#ifdef __UCLIBC_HAS_STDIO_BUFSIZ_NONE__
#define __STDIO_BUILTIN_BUF_SIZE		0
#else  /* __UCLIBC_HAS_STDIO_BUFSIZ_NONE__ */
#if defined(__UCLIBC_HAS_STDIO_BUILTIN_BUFFER_NONE__)
#define __STDIO_BUILTIN_BUF_SIZE		0
#elif defined(__UCLIBC_HAS_STDIO_BUILTIN_BUFFER_4__)
#define __STDIO_BUILTIN_BUF_SIZE		4
#elif defined(__UCLIBC_HAS_STDIO_BUILTIN_BUFFER_8__)
#define __STDIO_BUILTIN_BUF_SIZE		8
#else
#error config seems to be out of sync regarding builtin buffer size
#endif
#endif

#if defined(__STDIO_BUFFERS) || defined(__UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__) || defined(__UCLIBC_HAS_THREADS__)
#define __STDIO_HAS_OPENLIST 1
#else
#undef __STDIO_HAS_OPENLIST
#endif

/**********************************************************************/
/* Make sure defines related to large files are consistent. */
#ifdef _LIBC

#ifdef __UCLIBC_HAS_LFS__
#undef __USE_LARGEFILE
#undef __USE_LARGEFILE64
#undef __USE_FILE_OFFSET64
/* If we're actually building uClibc with large file support, only define... */
#define __USE_LARGEFILE64	1
#endif /* __UCLIBC_HAS_LFS__ */

#else  /* not _LIBC */

#ifndef __UCLIBC_HAS_LFS__
#if defined(__LARGEFILE64_SOURCE) || defined(__USE_LARGEFILE64) || defined(__USE_FILE_OFFSET64)
#error Sorry... uClibc was built without large file support!
#endif
#endif /* __UCLIBC_HAS_LFS__ */

#endif /* _LIBC */
/**********************************************************************/
#ifdef __UCLIBC_HAS_WCHAR__

#define __need_wchar_t
#include <stddef.h>

/* Note: we don't really need mbstate for 8-bit locales.  We do for UTF-8.
 * For now, always use it. */
#define __STDIO_MBSTATE
#define __need_mbstate_t
#include <wchar.h>

#endif
/**********************************************************************/
/* Currently unimplemented/untested */
/* #define __STDIO_FLEXIBLE_SETVBUF */

#ifdef __UCLIBC_HAS_STDIO_GETC_MACRO__
#define __STDIO_GETC_MACRO
#endif

#ifdef __UCLIBC_HAS_STDIO_PUTC_MACRO__
#define __STDIO_PUTC_MACRO
#endif


/* These are consistency checks on the different options */

#ifndef __STDIO_BUFFERS
#undef __STDIO_GETC_MACRO
#undef __STDIO_PUTC_MACRO
#endif

#ifdef __BCC__
#undef __UCLIBC_HAS_LFS__
#endif

#ifndef __UCLIBC_HAS_LFS__
#undef __UCLIBC_HAS_FOPEN_LARGEFILE_MODE__
#endif

/**********************************************************************/
#ifdef __UCLIBC_HAS_THREADS__
/* Need this for pthread_mutex_t. */
#include <bits/pthreadtypes.h>

/* user_locking
 * 0 : do auto locking/unlocking
 * 1 : user does locking/unlocking
 * 2 : initial state prior to thread initialization
 *     with no auto locking/unlocking
 *
 * When threading is initialized, walk the stdio open stream list
 * and do  "if (user_locking == 2) user_locking = 0;".
 *
 * This way, we avoid calling the weak lock/unlock functions.
 */

#define __STDIO_AUTO_THREADLOCK_VAR			int __infunc_user_locking

#define __STDIO_AUTO_THREADLOCK(__stream)								\
	if ((__infunc_user_locking = (__stream)->__user_locking) == 0) {	\
		__pthread_mutex_lock(&(__stream)->__lock);						\
	}

#define __STDIO_AUTO_THREADUNLOCK(__stream)				\
	if (__infunc_user_locking == 0) {					\
		__pthread_mutex_unlock(&(__stream)->__lock);		\
	}

#define __STDIO_SET_USER_LOCKING(__stream)	((__stream)->__user_locking = 1)

#define __STDIO_ALWAYS_THREADLOCK(__stream)	\
		__pthread_mutex_lock(&(__stream)->__lock)

#define __STDIO_ALWAYS_THREADTRYLOCK(__stream)	\
		__pthread_mutex_trylock(&(__stream)->__lock)

#define __STDIO_ALWAYS_THREADUNLOCK(__stream) \
		__pthread_mutex_unlock(&(__stream)->__lock)

#else  /* __UCLIBC_HAS_THREADS__ */

#define __STDIO_AUTO_THREADLOCK_VAR				((void)0)

#define __STDIO_AUTO_THREADLOCK(__stream)		((void)0)
#define __STDIO_AUTO_THREADUNLOCK(__stream)		((void)0)

#define __STDIO_SET_USER_LOCKING(__stream)		((void)0)

#define __STDIO_ALWAYS_THREADLOCK(__stream)		((void)0)
#define __STDIO_ALWAYS_THREADTRYLOCK(__stream)	(0)	/* Always succeed. */
#define __STDIO_ALWAYS_THREADUNLOCK(__stream)	((void)0)

#endif /* __UCLIBC_HAS_THREADS__ */
/**********************************************************************/

#define __STDIO_IOFBF 0		/* Fully buffered.  */
#define __STDIO_IOLBF 1		/* Line buffered.  */
#define __STDIO_IONBF 2		/* No buffering.  */

typedef struct {
	__off_t __pos;
#ifdef __STDIO_MBSTATE
	__mbstate_t __mbstate;
#endif
#ifdef __UCLIBC_HAS_WCHAR__
	int __mblen_pending;
#endif
} __STDIO_fpos_t;

#ifdef __UCLIBC_HAS_LFS__
typedef struct {
	__off64_t __pos;
#ifdef __STDIO_MBSTATE
	__mbstate_t __mbstate;
#endif
#ifdef __UCLIBC_HAS_WCHAR__
	int __mblen_pending;
#endif
} __STDIO_fpos64_t;
#endif

/**********************************************************************/
#ifdef __UCLIBC_HAS_LFS__
typedef __off64_t __offmax_t;	/* TODO -- rename this? */
#else
typedef __off_t __offmax_t;		/* TODO -- rename this? */
#endif

/**********************************************************************/
#ifdef __UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__

typedef __ssize_t __io_read_fn(void *__cookie, char *__buf, size_t __bufsize);
typedef __ssize_t __io_write_fn(void *__cookie,
								__const char *__buf, size_t __bufsize);
/* NOTE: GLIBC difference!!! -- fopencookie seek function
 * For glibc, the type of pos is always (__off64_t *) but in our case
 * it is type (__off_t *) when the lib is built without large file support.
 */
typedef int __io_seek_fn(void *__cookie, __offmax_t *__pos, int __whence);
typedef int __io_close_fn(void *__cookie);

typedef struct {
	__io_read_fn  *read;
	__io_write_fn *write;
	__io_seek_fn  *seek;
	__io_close_fn *close;
} _IO_cookie_io_functions_t;

#if defined(_LIBC) || defined(_GNU_SOURCE)

typedef __io_read_fn cookie_read_function_t;
typedef __io_write_fn cookie_write_function_t;
typedef __io_seek_fn cookie_seek_function_t;
typedef __io_close_fn cookie_close_function_t;

typedef _IO_cookie_io_functions_t cookie_io_functions_t;

#endif

#endif
/**********************************************************************/

struct __STDIO_FILE_STRUCT {
	unsigned short __modeflags;
	/* There could be a hole here, but modeflags is used most.*/
#ifdef __UCLIBC_HAS_WCHAR__
	unsigned char __ungot_width[2]; /* 0: current (building) char; 1: scanf */
	/* Move the following futher down to avoid problems with getc/putc
	 * macros breaking shared apps when wchar config support is changed. */
	/* wchar_t ungot[2]; */
#else  /* __UCLIBC_HAS_WCHAR__ */
	unsigned char __ungot[2];
#endif /* __UCLIBC_HAS_WCHAR__ */
	int __filedes;
#ifdef __STDIO_BUFFERS
	unsigned char *__bufstart;	/* pointer to buffer */
	unsigned char *__bufend;	/* pointer to 1 past end of buffer */
	unsigned char *__bufpos;
	unsigned char *__bufread; /* pointer to 1 past last buffered read char */

#ifdef __STDIO_GETC_MACRO
	unsigned char *__bufgetc_u;	/* 1 past last readable by getc_unlocked */
#endif /* __STDIO_GETC_MACRO */
#ifdef __STDIO_PUTC_MACRO
	unsigned char *__bufputc_u;	/* 1 past last writeable by putc_unlocked */
#endif /* __STDIO_PUTC_MACRO */

#endif /* __STDIO_BUFFERS */

#ifdef __STDIO_HAS_OPENLIST
	struct __STDIO_FILE_STRUCT *__nextopen;
#endif
#ifdef __UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__
	void *__cookie;
	_IO_cookie_io_functions_t __gcs;
#endif
#ifdef __UCLIBC_HAS_WCHAR__
	wchar_t __ungot[2];
#endif
#ifdef __STDIO_MBSTATE
	__mbstate_t __state;
#endif
#ifdef __UCLIBC_HAS_XLOCALE__
	void *__unused;				/* Placeholder for codeset binding. */
#endif
#ifdef __UCLIBC_HAS_THREADS__
	int __user_locking;
	pthread_mutex_t __lock;
#endif
/* Everything after this is unimplemented... and may be trashed. */
#if __STDIO_BUILTIN_BUF_SIZE > 0
	unsigned char __builtinbuf[__STDIO_BUILTIN_BUF_SIZE];
#endif /* __STDIO_BUILTIN_BUF_SIZE > 0 */
};


/***********************************************************************/
/* Having ungotten characters implies the stream is reading.
 * The scheme used here treats the least significant 2 bits of
 * the stream's modeflags member as follows:
 *   0 0   Not currently reading.
 *   0 1   Reading, but no ungetc() or scanf() push back chars.
 *   1 0   Reading with one ungetc() char (ungot[1] is 1)
 *         or one scanf() pushed back char (ungot[1] is 0).
 *   1 1   Reading with both an ungetc() char and a scanf()
 *         pushed back char.  Note that this must be the result
 *         of a scanf() push back (in ungot[0]) _followed_ by
 *         an ungetc() call (in ungot[1]).
 *
 * Notes:
 *   scanf() can NOT use ungetc() to push back characters.
 *     (See section 7.19.6.2 of the C9X rationale -- WG14/N897.)
 */

#define __MASK_READING		0x0003U /* (0x0001 | 0x0002) */
#define __FLAG_READING		0x0001U
#define __FLAG_UNGOT    	0x0002U
#define __FLAG_EOF			0x0004U
#define __FLAG_ERROR		0x0008U
#define __FLAG_WRITEONLY  	0x0010U
#define __FLAG_READONLY  	0x0020U /* (__FLAG_WRITEONLY << 1) */
#define __FLAG_WRITING		0x0040U
#define __FLAG_NARROW       0x0080U

#define __FLAG_FBF          0x0000U /* must be 0 */
#define __FLAG_LBF          0x0100U
#define __FLAG_NBF          0x0200U /* (__FLAG_LBF << 1) */
#define __MASK_BUFMODE      0x0300U /* (__FLAG_LBF|__FLAG_NBF) */
#define __FLAG_APPEND       0x0400U /* fixed! == O_APPEND for linux */
#define __FLAG_WIDE			0x0800U
/* available slot           0x1000U */
#define __FLAG_FREEFILE		0x2000U
#define __FLAG_FREEBUF		0x4000U
#define __FLAG_LARGEFILE    0x8000U /* fixed! == 0_LARGEFILE for linux */

/* Note: In no-buffer mode, it would be possible to pack the necessary
 * flags into one byte.  Since we wouldn't be buffering and there would
 * be no support for wchar, the only flags we would need would be:
 *   2 bits : ungot count
 *   2 bits : eof + error
 *   2 bits : readonly + writeonly
 *   1 bit  : freefile
 *   1 bit  : appending
 * So, for a very small system (< 128 files) we might have a
 * 4-byte FILE struct of:
 *   unsigned char flags;
 *   signed char filedes;
 *   unsigned char ungot[2];
 */
/**********************************************************************
 * PROTOTYPES OF INTERNAL FUNCTIONS
 **********************************************************************/
#ifdef _LIBC

extern void _stdio_init(void);
extern void _stdio_term(void);

#ifdef __STDIO_HAS_OPENLIST

extern struct __STDIO_FILE_STRUCT *_stdio_openlist;

#ifdef __UCLIBC_HAS_THREADS__
extern pthread_mutex_t _stdio_openlist_lock;
extern int _stdio_openlist_delflag;
extern int _stdio_user_locking;
#ifdef _LIBC
extern void __stdio_init_mutex(pthread_mutex_t *m) attribute_hidden;
#endif
#endif

#endif

#endif
/**********************************************************************/

#define __CLEARERR_UNLOCKED(__stream) \
	((void)((__stream)->__modeflags &= ~(__FLAG_EOF|__FLAG_ERROR)))
#define __FEOF_UNLOCKED(__stream)	((__stream)->__modeflags & __FLAG_EOF)
#define __FERROR_UNLOCKED(__stream)	((__stream)->__modeflags & __FLAG_ERROR)

/*
 * FIXME: to avoid symbole duplication with TCS
 */
#ifdef __UCLIBC_HAS_THREADS__
/* Clear the error and EOF indicators for STREAM.  */
extern void tmlinux_clearerr (FILE *__stream) __THROW;
/* Return the EOF indicator for STREAM.  */
extern int tmlinux_feof (FILE *__stream) __THROW;
/* Return the error indicator for STREAM.  */
extern int tmlinux_ferror (FILE *__stream) __THROW;
# define __CLEARERR(__stream)		(tmlinux_clearerr)(__stream)
# define __FERROR(__stream)		(tmlinux_ferror)(__stream)
# define __FEOF(__stream)		(tmlinux_feof)(__stream)
#else
# define __CLEARERR(__stream)		__CLEARERR_UNLOCKED(__stream)
# define __FERROR(__stream)		__FERROR_UNLOCKED(__stream)
# define __FEOF(__stream)		__FEOF_UNLOCKED(__stream)
#endif

extern int __fgetc_unlocked(FILE *__stream);
extern int __fputc_unlocked(int __c, FILE *__stream);
extern int tmlinux_fgetc(FILE *stream);
extern int tmlinux_fputc(int c, FILE *stream);

/* First define the default definitions.  They overriden below as necessary. */
#define __FGETC_UNLOCKED(__stream) 	(__fgetc_unlocked)((__stream))
#define __FGETC(__stream)		(tmlinux_fgetc)((__stream))
#define __GETC_UNLOCKED_MACRO(__stream)	(__fgetc_unlocked)((__stream))
#define __GETC_UNLOCKED(__stream)	(__fgetc_unlocked)((__stream))
#define __GETC(__stream)		(tmlinux_fgetc)((__stream))

#define __FPUTC_UNLOCKED(__c, __stream)	(__fputc_unlocked)((__c),(__stream))
#define __FPUTC(__c, __stream)		(tmlinux_fputc)((__c),(__stream))
#define __PUTC_UNLOCKED_MACRO(__c, __stream) (__fputc_unlocked)((__c),(__stream))
#define __PUTC_UNLOCKED(__c, __stream)	(__fputc_unlocked)((__c),(__stream))
#define __PUTC(__c, __stream)		(tmlinux_fputc)((__c),(__stream))

