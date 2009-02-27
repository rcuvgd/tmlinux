#define _GNU_SOURCE
#include <features.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <endian.h>
#include <unistd.h>
#include <sys/mman.h>

#ifdef __NR_mmap

#define __NR___syscall_mmap	    __NR_mmap
static inline _syscall6(__ptr_t, __syscall_mmap, __ptr_t, addr, 
	size_t, len, int, prot, int, flags, int, fd, off_t, offset);

/* This is always 12, even on architectures where PAGE_SHIFT != 12.  */
# ifndef MMAP_PAGE_SHIFT
#  define MMAP_PAGE_SHIFT 12
# endif

__ptr_t mmap(__ptr_t addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    if (offset & ((1 << MMAP_PAGE_SHIFT) - 1)) {
	__set_errno (EINVAL);
	return MAP_FAILED;
    }
    return(__syscall_mmap(addr, len, prot, flags, fd, (off_t) (offset >> MMAP_PAGE_SHIFT)));
}

#endif 
