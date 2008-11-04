/* Copyright (C) 2005       Manuel Novoa III    <mjn3@codepoet.org>
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

#ifndef _UCLIBC_VA_COPY_H
#define _UCLIBC_VA_COPY_H 1

#include <stdarg.h>

/*
 * FIXME: TCS has not va_copy, I don't know whether or not it's suitable.
 */
#define va_copy(A,B)	A = B

#endif /* _UCLIBC_VA_COPY_H */
