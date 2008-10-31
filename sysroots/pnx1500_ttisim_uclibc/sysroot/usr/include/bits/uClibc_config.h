/*
 * Automatically generated C config: don't edit
 */
#if !defined __FEATURES_H && !defined __need_uClibc_config_h
#error Never include <bits/uClibc_config.h> directly; use <features.h> instead
#endif

/*
 * Version Number
 */
#define __UCLIBC_MAJOR__ 0
#define __UCLIBC_MINOR__ 9
#define __UCLIBC_SUBLEVEL__ 28
#undef __TARGET_alpha__
#undef __TARGET_arm__
#undef __TARGET_bfin__
#undef __TARGET_cris__
#undef __TARGET_e1__
#undef __TARGET_frv__
#undef __TARGET_h8300__
#undef __TARGET_hppa__
#undef __TARGET_i386__
#undef __TARGET_i960__
#undef __TARGET_m68k__
#undef __TARGET_microblaze__
#undef __TARGET_mips__
#undef __TARGET_nios__
#undef __TARGET_nios2__
#undef __TARGET_powerpc__
#define __TARGET_trimedia__ 1
#undef __TARGET_sh__
#undef __TARGET_sh64__
#undef __TARGET_sparc__
#undef __TARGET_v850__
#undef __TARGET_x86_64__

/*
 * Target Architecture Features and Options
 */
#undef __HAVE_ELF__
#define __ARCH_SUPPORTS_LITTLE_ENDIAN__ 1
#define __TARGET_ARCH__ "trimedia"
#define __ARCH_SUPPORTS_BIG_ENDIAN__ 1
#define __CROSS__ "tm"
#define __ARCH_TRIMEDIA__ 1
#define __TCS_PATH__ "/home/guohr/workdir/gulessoft/tcs/5.1.0"
#define __ARCH_LITTLE_ENDIAN__ 1
#undef __ARCH_BIG_ENDIAN__
#define __ARCH_HAS_NO_MMU__ 1
#define __UCLIBC_HAS_FLOATS__ 1
#undef __HAS_FPU__
#define __UCLIBC_HAS_SOFT_FLOAT__ 1
#undef __DO_C99_MATH__
#define __KERNEL_SOURCE__ "/home/guohr/workdir/gulessoft/linux/trunk"
#define __UCLIBC_UCLINUX_BROKEN_MUNMAP__ 1
#define __EXCLUDE_BRK__ 1
#define __C_SYMBOL_PREFIX__ ""
#define __HAVE_DOT_CONFIG__ 1

/*
 * General Library Settings
 */
#define __HAVE_NO_PIC__ 1
#define __HAVE_NO_SHARED__ 1
#define __ARCH_HAS_NO_LDSO__ 1
#undef __UCLIBC_CTOR_DTOR__
#undef __HAS_NO_THREADS__
#define __UCLIBC_HAS_THREADS__ 1
#undef __PTHREADS_DEBUG_SUPPORT__
#define __LINUXTHREADS_OLD__ 1
#define __UCLIBC_HAS_LFS__ 1
#define __UCLIBC_STATIC_LDCONFIG__ 1
#undef __MALLOC__
#define __MALLOC_SIMPLE__ 1
#undef __MALLOC_STANDARD__
#undef __MALLOC_GLIBC_COMPAT__
#define __UCLIBC_DYNAMIC_ATEXIT__ 1
#undef __COMPAT_ATEXIT__
#define __HAS_SHADOW__ 1
#define __UNIX98PTY_ONLY__ 1
#define __ASSUME_DEVPTS__ 1
#define __UCLIBC_HAS_TM_EXTENSIONS__ 1
#define __UCLIBC_HAS_TZ_CACHING__ 1
#define __UCLIBC_HAS_TZ_FILE__ 1
#define __UCLIBC_HAS_TZ_FILE_READ_MANY__ 1
#define __UCLIBC_TZ_FILE_PATH__ "/etc/TZ"

/*
 * Networking Support
 */
#undef __UCLIBC_HAS_IPV6__
#undef __UCLIBC_HAS_RPC__

/*
 * String and Stdio Support
 */
#define __UCLIBC_HAS_STRING_GENERIC_OPT__ 1
#define __UCLIBC_HAS_STRING_ARCH_OPT__ 1
#define __UCLIBC_HAS_CTYPE_TABLES__ 1
#define __UCLIBC_HAS_CTYPE_SIGNED__ 1
#define __UCLIBC_HAS_CTYPE_UNSAFE__ 1
#undef __UCLIBC_HAS_CTYPE_CHECKED__
#undef __UCLIBC_HAS_CTYPE_ENFORCED__
#undef __UCLIBC_HAS_WCHAR__
#undef __UCLIBC_HAS_LOCALE__
#undef __UCLIBC_HAS_HEXADECIMAL_FLOATS__
#undef __UCLIBC_HAS_GLIBC_CUSTOM_PRINTF__
#undef __USE_OLD_VFPRINTF__
#define __UCLIBC_PRINTF_SCANF_POSITIONAL_ARGS__ 9
#undef __UCLIBC_HAS_SCANF_GLIBC_A_FLAG__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_NONE__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_256__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_512__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_1024__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_2048__
#define __UCLIBC_HAS_STDIO_BUFSIZ_4096__ 1
#undef __UCLIBC_HAS_STDIO_BUFSIZ_8192__
#define __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_NONE__ 1
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_4__
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_8__
#undef __UCLIBC_HAS_STDIO_SHUTDOWN_ON_ABORT__
#undef __UCLIBC_HAS_STDIO_GETC_MACRO__
#undef __UCLIBC_HAS_STDIO_PUTC_MACRO__
#define __UCLIBC_HAS_STDIO_AUTO_RW_TRANSITION__ 1
#undef __UCLIBC_HAS_FOPEN_LARGEFILE_MODE__
#undef __UCLIBC_HAS_FOPEN_EXCLUSIVE_MODE__
#undef __UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__
#undef __UCLIBC_HAS_PRINTF_M_SPEC__
#define __UCLIBC_HAS_ERRNO_MESSAGES__ 1
#undef __UCLIBC_HAS_SYS_ERRLIST__
#define __UCLIBC_HAS_SIGNUM_MESSAGES__ 1
#undef __UCLIBC_HAS_SYS_SIGLIST__
#define __UCLIBC_HAS_GNU_GETOPT__ 1

/*
 * Big and Tall
 */
#undef __UCLIBC_HAS_REGEX__
#undef __UCLIBC_HAS_WORDEXP__
#undef __UCLIBC_HAS_FTW__
#define __UCLIBC_HAS_GLOB__ 1

/*
 * Library Installation Options
 */
#define __RUNTIME_PREFIX__ "/home/guohr/workdir/gulessoft/sysroots/pnx1500_ttisim_uclibc/sysroot/"
#define __DEVEL_PREFIX__ "/home/guohr/workdir/gulessoft/sysroots/pnx1500_ttisim_uclibc/sysroot/usr/"

/*
 * Security options
 */
#undef __HAVE_NO_SSP__
#undef __UCLIBC_HAS_SSP__
#define __UCLIBC_BUILD_NOEXECSTACK__ 1

/*
 * uClibc development/debugging options
 */
#define __CROSS_COMPILER_PREFIX__ ""
#undef __DODEBUG__
#undef __DODEBUG_PT__
#undef __DOASSERTS__
#define __WARNINGS__ ""
#undef __DOMULTI__
#undef __UCLIBC_MJN3_ONLY__