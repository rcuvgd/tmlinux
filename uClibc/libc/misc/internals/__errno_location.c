/*
 * For trimedia, the following is move to libcstub.a
 */

#ifndef __TCS__
#include <errno.h>
#undef errno

int * weak_const_function __errno_location (void)
{
    return &errno;
}
#endif 

