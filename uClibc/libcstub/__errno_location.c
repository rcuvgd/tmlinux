#include <errno.h>
#undef errno

int * __errno_location (void)
{
    return &errno;
}

