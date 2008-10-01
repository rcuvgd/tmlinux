#include <setjmp.h>

extern int __setjmp(__jmp_buf buf);

extern int
__sigsetjmp(jmp_buf __env,int __savemask)
{
	__sigjmp_save(__env,__savemask);
	return __setjmp(__env[0].__jmpbuf);
}

#ifdef setjmp
#undef setjmp
#endif

int setjmp(jmp_buf __env)
{
	return __sigsetjmp(__env,1);
}

int _setjmp(jmp_buf __env)
{
	return __sigsetjmp(__env,0);
}
