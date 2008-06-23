/*
 * include/arch/trimedia/klibc/archconfig.h
 *
 * See include/klibc/sysconfig.h for the options that can be set in
 * this file.
 *
 */

#ifndef _KLIBC_ARCHCONFIG_H
#define _KLIBC_ARCHCONFIG_H

/*
 * We only support trimedia without mmu
 */
#define _KLIBC_NO_MMU 		1

/*
 * TriMedia has its own vfork
 */
#define _KLIBC_REAL_VFORK 	1

#define _KLIBC_USE_MMAP2 	1

#define _KLIBC_MMAP2_SHIFT  	12

#define _KLIBC_MALLOC_USES_SBRK 0

#define _KLIBC_USE_RT_SIG 	0

#define _KLIBC_NEEDS_SA_RESTORER 0

#define _KLIBC_STATFS_F_TYPE_64 0

#define _KLIBC_STATFS_F_TYPE_32B 0

#define _KLIBC_HAS_ARCHSOCKET_H  0

#endif				/* _KLIBC_ARCHCONFIG_H */
