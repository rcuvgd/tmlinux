/* Wrapper arpund system calls to provide cancelation points.
   Copyright (C) 1996,1997,1998,1999,2000,2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1996.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#define __FORCE_GLIBC
#include <features.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/syscall.h>


/*
 * TriMedia does not support weak symbols
 */
#ifndef __TCS__

#ifndef __PIC__
/* We need a hook to force this file to be linked in when static
   libpthread is used.  */
const int __pthread_provide_wrappers = 0;
#endif


#define CANCELABLE_SYSCALL(res_type, name, param_list, params) \
res_type __libc_##name param_list;					      \
res_type								      \
__attribute__ ((weak))							      \
name param_list								      \
{									      \
  res_type result;							      \
  int oldtype;								      \
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);	      \
  result = __libc_##name params;					      \
  pthread_setcanceltype (oldtype, NULL);				      \
  return result;							      \
}

#define CANCELABLE_SYSCALL_VA(res_type, name, param_list, params, last_arg) \
res_type __libc_##name param_list;					      \
res_type								      \
__attribute__ ((weak))							      \
name param_list								      \
{									      \
  res_type result;							      \
  int oldtype;								      \
  va_list ap;								      \
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);	      \
  va_start (ap, last_arg);						      \
  result = __libc_##name params;					      \
  va_end (ap);								      \
  pthread_setcanceltype (oldtype, NULL);				      \
  return result;							      \
}


/* close(2).  */
CANCELABLE_SYSCALL (int, close, (int fd), (fd))


/* fcntl(2).  */
CANCELABLE_SYSCALL_VA (int, fcntl, (int fd, int cmd, ...),
		       (fd, cmd, va_arg (ap, long int)), cmd)


/* fsync(2).  */
CANCELABLE_SYSCALL (int, fsync, (int fd), (fd))


/* lseek(2).  */
CANCELABLE_SYSCALL (off_t, lseek, (int fd, off_t offset, int whence),
		    (fd, offset, whence))

#ifdef __UCLIBC_HAS_LFS__
/* lseek64(2).  */
CANCELABLE_SYSCALL (off64_t, lseek64, (int fd, off64_t offset, int whence),
		    (fd, offset, whence))
#endif

/* msync(2).  */
CANCELABLE_SYSCALL (int, msync, (__ptr_t addr, size_t length, int flags),
		    (addr, length, flags))


/* nanosleep(2).  */
CANCELABLE_SYSCALL (int, nanosleep, (const struct timespec *requested_time,
				     struct timespec *remaining),
		    (requested_time, remaining))


/* open(2).  */
CANCELABLE_SYSCALL_VA (int, open, (const char *pathname, int flags, ...),
		       (pathname, flags, va_arg (ap, mode_t)), flags)


#ifdef __UCLIBC_HAS_LFS__
/* open64(3).  */
CANCELABLE_SYSCALL_VA (int, open64, (const char *pathname, int flags, ...),
		       (pathname, flags, va_arg (ap, mode_t)), flags)
#endif

/* pause(2).  */
CANCELABLE_SYSCALL (int, pause, (void), ())


/* Enable this if enabling these in syscalls.c */
/* pread(3).  */
CANCELABLE_SYSCALL (ssize_t, pread, (int fd, void *buf, size_t count,
				     off_t offset),
		    (fd, buf, count, offset))


#if defined __UCLIBC_HAS_LFS__ && defined __NR_pread64
/* pread64(3).  */
CANCELABLE_SYSCALL (ssize_t, pread64, (int fd, void *buf, size_t count,
				       off64_t offset),
		    (fd, buf, count, offset))
#endif

/* pwrite(3).  */
CANCELABLE_SYSCALL (ssize_t, pwrite, (int fd, const void *buf, size_t n,
				      off_t offset),
		    (fd, buf, n, offset))


#if defined __UCLIBC_HAS_LFS__ && defined __NR_pwrited64
/* pwrite64(3).  */
CANCELABLE_SYSCALL (ssize_t, pwrite64, (int fd, const void *buf, size_t n,
					off64_t offset),
		    (fd, buf, n, offset))
#endif

/* read(2).  */
CANCELABLE_SYSCALL (ssize_t, read, (int fd, void *buf, size_t count),
		    (fd, buf, count))


/* system(3).  */
CANCELABLE_SYSCALL (int, system, (const char *line), (line))


/* tcdrain(2).  */
CANCELABLE_SYSCALL (int, tcdrain, (int fd), (fd))


/* wait(2).  */
CANCELABLE_SYSCALL (__pid_t, wait, (__WAIT_STATUS_DEFN stat_loc), (stat_loc))


/* waitpid(2).  */
CANCELABLE_SYSCALL (__pid_t, waitpid, (__pid_t pid, int *stat_loc,
				       int options),
		    (pid, stat_loc, options))


/* write(2).  */
CANCELABLE_SYSCALL (ssize_t, write, (int fd, const void *buf, size_t n),
		    (fd, buf, n))


/* The following system calls are thread cancellation points specified
   in XNS.  */

/* accept(2).  */
CANCELABLE_SYSCALL (int, accept, (int fd, __SOCKADDR_ARG addr,
				  socklen_t *addr_len),
		    (fd, addr, addr_len))

/* connect(2).  */
CANCELABLE_SYSCALL (int, connect, (int fd, __CONST_SOCKADDR_ARG addr,
				     socklen_t len),
		    (fd, addr, len))

/* recv(2).  */
CANCELABLE_SYSCALL (ssize_t, recv, (int fd, __ptr_t buf, size_t n, int flags),
		    (fd, buf, n, flags))

/* recvfrom(2).  */
CANCELABLE_SYSCALL (ssize_t, recvfrom, (int fd, __ptr_t buf, size_t n, int flags,
					__SOCKADDR_ARG addr, socklen_t *addr_len),
		    (fd, buf, n, flags, addr, addr_len))

/* recvmsg(2).  */
CANCELABLE_SYSCALL (ssize_t, recvmsg, (int fd, struct msghdr *message, int flags),
		    (fd, message, flags))

/* send(2).  */
CANCELABLE_SYSCALL (ssize_t, send, (int fd, const __ptr_t buf, size_t n,
				    int flags),
		    (fd, buf, n, flags))

/* sendmsg(2).  */
CANCELABLE_SYSCALL (ssize_t, sendmsg, (int fd, const struct msghdr *message,
				       int flags),
		    (fd, message, flags))

/* sendto(2).  */
CANCELABLE_SYSCALL (ssize_t, sendto, (int fd, const __ptr_t buf, size_t n,
				      int flags, __CONST_SOCKADDR_ARG addr,
				      socklen_t addr_len),
		    (fd, buf, n, flags, addr, addr_len))

#endif 
