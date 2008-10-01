#include <signal.h>

#ifdef __TCS__

extern int
__sigemptyset(void* set)
{
	int __cnt = _SIGSET_NWORDS;
	sigset_t* __set = (sigset_t*)set;
	while(--__cnt >= 0) __set->__val[__cnt] = 0;
	return 0;
}

extern int
__sigfillset(void* set)
{
	int __cnt = _SIGSET_NWORDS;
	sigset_t* __set= (sigset_t*)set;
	while(--__cnt >= 0) __set->__val[__cnt] = ~0UL;
	return 0;
}

#  ifdef __USE_GNU
/* The POSIX does not specify for handling the whole signal set in one
   command.  This is often wanted and so we define three more functions
   here.  */
extern int 
__sigisemptyset(void* set)
{
	int __cnt = _SIGSET_NWORDS;
	const sigset_t* __set = (sigset_t*)set;
	int __ret = __set->__val[--__cnt];

	while(!__ret && --__cnt >=0)
		__ret = __set->__val[__cnt];

	return __ret == 0;
	
}

extern int 
__sigandset(void* dest,void* left, void* right)
{
	int __cnt = _SIGSET_NWORDS;				      
	sigset_t *__dest = (sigset_t*)(dest);				      
	const sigset_t *__left = (sigset_t*)(left);			      
	const sigset_t *__right = (sigset_t*)(right);			      
	while (--__cnt >= 0)				      
		__dest->__val[__cnt] = (__left->__val[__cnt]	      
				& __right->__val[__cnt]);	      
	return 0; 

}	

extern int 
__sigorset(void* dest,void* left,void* right)
{
	int __cnt = _SIGSET_NWORDS;				      
	sigset_t *__dest = (sigset_t*)(dest);				      
	const sigset_t *__left = (sigset_t*)(left);			      
	const sigset_t *__right = (sigset_t*)(right);			      
	while (--__cnt >= 0)				      
		__dest->__val[__cnt] = (__left->__val[__cnt]	      
				| __right->__val[__cnt]);	      
	return 0;

}
#  endif
#endif 
