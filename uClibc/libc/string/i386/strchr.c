/*
 * This string-include defines all string functions as inline
 * functions. Use gcc. It also assumes ds=es=data space, this should be
 * normal. Most of the string-functions are rather heavily hand-optimized,
 * see especially strtok,strstr,str[c]spn. They should work, but are not
 * very easy to understand. Everything is done entirely within the register
 * set, making the functions fast and clean. String instructions have been
 * used through-out, making for "slightly" unclear code :-)
 *
 *		NO Copyright (C) 1991, 1992 Linus Torvalds,
 *		consider these trivial functions to be PD.
 */

/*
 * Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

/*
 * Modified for uClibc by Erik Andersen <andersen@codepoet.org>
 * These make no attempt to use nifty things like mmx/3dnow/etc.
 * These are not inline, and will therefore not be as fast as
 * modifying the headers to use inlines (and cannot therefore
 * do tricky things when dealing with const memory).  But they
 * should (I hope!) be faster than their generic equivalents....
 *
 * More importantly, these should provide a good example for
 * others to follow when adding arch specific optimizations.
 *  -Erik
 */

#include <string.h>

#undef strchr
char attribute_hidden *__strchr(const char *s, int c)
{
    int d0;
    register char * __res;
    __asm__ __volatile__(
	    "movb %%al,%%ah\n"
	    "1:\tlodsb\n\t"
	    "cmpb %%ah,%%al\n\t"
	    "je 2f\n\t"
	    "testb %%al,%%al\n\t"
	    "jne 1b\n\t"
	    "movl $1,%1\n"
	    "2:\tmovl %1,%0\n\t"
	    "decl %0"
	    :"=a" (__res), "=&S" (d0) : "1" (s),"0" (c));
    return __res;
}

strong_alias(__strchr, strchr)

weak_alias(strchr, index)
