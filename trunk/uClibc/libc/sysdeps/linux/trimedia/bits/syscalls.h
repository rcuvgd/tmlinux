/* Unlike the asm/unistd.h kernel header file (which this is partly based on),
 * this file must be able to cope with PIC and non-PIC code.  For some arches
 * there is no difference.  For x86 (which has far too few registers) there is
 * a difference.   Regardless, including asm/unistd.h is hereby officially
 * forbidden.  Don't do it.  It is bad for you. 
 */ 
#ifndef _SYSCALL_H
# error "Never use <bits/syscalls.h> directly; include <sys/syscall.h> instead."
#endif

/* This includes the `__NR_<name>' syscall numbers taken from the Linux kernel
 * header files.  It also defines the traditional `SYS_<name>' macros for older
 * programs.  */
#include <bits/sysnum.h>

#define __syscall_return(type, res) 					\
do { 									\
	if ((unsigned long)(res) >= (unsigned long)(-125)) { 		\
	/* avoid using res which is declared to be in register d0; 	\
	   errno might expand to a function call and clobber it.  */ 	\
		int __err = -(res); 					\
		errno = __err; 						\
		res = -1; 						\
	} 								\
	return (type) (res); 						\
} while (0)

#define _syscall0(type, name)						\
type name(void)								\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,0,0,0,0,0,0); 				\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall1(type, name, atype, a)					\
type name(atype a)							\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,0,0,0,0,0); 		\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall2(type, name, atype, a, btype, b)			\
type name(atype a, btype b)						\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,(unsigned long)b, 	\
		  0,0,0,0); 						\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall3(type, name, atype, a, btype, b, ctype, c)		\
type name(atype a, btype b, ctype c)					\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,(unsigned long)b, 	\
		  (unsigned long)c,0,0,0); 				\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall4(type, name, atype, a, btype, b,			\
                  ctype, c, dtype, d)					\
type name(atype a, btype b, ctype c, dtype d)				\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,(unsigned long)b, 	\
		  (unsigned long)c,(unsigned long)d,0,0); 		\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall5(type, name, atype, a, btype, b,			\
                  ctype, c, dtype, d, etype, e)				\
type name(atype a, btype b, ctype c, dtype d, etype e)			\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,(unsigned long)b, 	\
		  (unsigned long)c,(unsigned long)d,(unsigned long)e,0);\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}

#define _syscall6(type, name, atype, a, btype, b,			\
                  ctype, c, dtype, d, etype, e, ftype, f)		\
type name(atype a, btype b, ctype c, dtype d, etype e, ftype f)		\
{									\
  register long __res=0; 						\
  __res=__syscall(__NR_##name,(unsigned long)a,(unsigned long)b,      \
		  (unsigned long)c,(unsigned long)d, 			\
		  (unsigned long)e,(unsigned long)f); 			\
  __syscall_return(type, __res);					\
  return (type)__res; 							\
}
