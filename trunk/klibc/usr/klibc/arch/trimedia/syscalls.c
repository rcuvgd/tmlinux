#include <asm/unistd.h>

int errno;

#define __NR__exit __NR_exit
extern _syscall0(int,fork);
extern _syscall0(int,vfork);
extern _syscall1(int,_exit,int,status);
extern _syscall3(int,execve,unsigned long,fn,char**,argv,char**,envp);
extern _syscall0(int,getpid);
extern _syscall2(int,nanosleep,unsigned long*,tv,unsigned long*,ts);
extern _syscall4(int,wait4,unsigned long,pid,unsigned long*,status,unsigned long, option,unsigned long*,usage);
extern _syscall6(int,mmap,unsigned long*,addr,unsigned long,len,int,prot,int,flags,int,fildes,unsigned long,off);
extern _syscall2(int,munmap,unsigned long*,addr,unsigned long,len);
extern _syscall0(int,getppid);
extern _syscall2(int,kill,int,pid,int,signum);
#define __NR___sigaction __NR_sigaction
extern _syscall3(int,__sigaction,int,signum,unsigned long*,newaction,unsigned long*,oldaction);
