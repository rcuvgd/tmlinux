/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.14.7-tiny1
 * Mon Jun 23 11:14:29 2008
 */
#define AUTOCONF_INCLUDED
#define CONFIG_TRIMEDIA 1
#undef CONFIG_MMU
#undef CONFIG_FPU
#define CONFIG_UID16 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#undef CONFIG_RWSEM_XCHGADD_ALGORITHM
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#define CONFIG_KTIME_SCALAR 1
#define CONFIG_SEMAPHORE_SLEEPERS 1
#define CONFIG_GENERIC_HARDIRQS 1

/*
 * Code maturity level options
 */
#undef CONFIG_EXPERIMENTAL
#define CONFIG_CLEAN_COMPILE 1
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_INIT_ENV_ARG_LIMIT 32

/*
 * General setup
 */
#define CONFIG_LOCALVERSION ""
#undef CONFIG_LOCALVERSION_AUTO
#undef CONFIG_BSD_PROCESS_ACCT
#undef CONFIG_SYSCTL
#undef CONFIG_HOTPLUG
#undef CONFIG_IKCONFIG
#define CONFIG_INITRAMFS_SOURCE "/home/guohr/workdir/gulessoft/initramfs_list /home/guohr/workdir/gulessoft/export"
#define CONFIG_INITRAMFS_ROOT_UID 0
#define CONFIG_INITRAMFS_ROOT_GID 0
#define CONFIG_EMBEDDED 1
#undef CONFIG_KALLSYMS
#undef CONFIG_PRINTK
#define CONFIG_BUG 1
#undef CONFIG_KMALLOC_ACCOUNTING
#undef CONFIG_AUDIT_BOOTMEM
#define CONFIG_PANIC 1
#define CONFIG_FULL_PANIC 1
#define CONFIG_ELF_CORE 1
#define CONFIG_BASE_FULL 1
#undef CONFIG_NET_SMALL
#define CONFIG_FUTEX 1
#define CONFIG_EPOLL 1
#define CONFIG_AIO 1
#define CONFIG_XATTR 1
#define CONFIG_FILE_LOCKING 1
#define CONFIG_INETPEER 1
#undef CONFIG_MEASURE_INLINES
#undef CONFIG_CC_OPTIMIZE_FOR_SIZE
#define CONFIG_CC_ALIGN_FUNCTIONS 0
#define CONFIG_CC_ALIGN_LABELS 0
#define CONFIG_CC_ALIGN_LOOPS 0
#define CONFIG_CC_ALIGN_JUMPS 0
#undef CONFIG_NEST_ALTSTACK
#undef CONFIG_OOM_PROTECT
#undef CONFIG_TINY_CFLAGS
#undef CONFIG_SLAB
#define CONFIG_MEMPOOL 1
#define CONFIG_CONSOLE_TRANSLATIONS 1
#define CONFIG_BINFMT_SCRIPT 1
#define CONFIG_MAX_SWAPFILES_SHIFT 5
#define CONFIG_NR_LDISCS 16
#define CONFIG_MAX_USER_RT_PRIO 100
#define CONFIG_PCI_QUIRKS 1
#define CONFIG_CPU_SUP_INTEL 1
#define CONFIG_CPU_SUP_CYRIX 1
#define CONFIG_CPU_SUP_NSC 1
#define CONFIG_CPU_SUP_AMD 1
#define CONFIG_CPU_SUP_CENTAUR 1
#define CONFIG_CPU_SUP_TRANSMETA 1
#define CONFIG_CPU_SUP_RISE 1
#define CONFIG_CPU_SUP_NEXGEN 1
#define CONFIG_CPU_SUP_UMC 1
#define CONFIG_CRC32_CALC 1
#define CONFIG_TINY_SHMEM 1
#define CONFIG_BASE_SMALL 0
#define CONFIG_SLOB 1

/*
 * Processor type and features
 */
#define CONFIG_TMSIM 1
#undef CONFIG_WINNT
#undef CONFIG_NOHOST
#undef CONFIG_TM1300
#define CONFIG_PNX1500 1
#undef CONFIG_PNX1700
#define CONFIG_TTISIM 1
#define CONFIG_LITTLE_ENDIAN 1
#undef CONFIG_BIG_ENDIAN
#undef CONFIG_FP32
#define CONFIG_FP64 1
#undef CONFIG_TRIMEDIA_DP
#undef CONFIG_4KSTACKS
#define CONFIG_FLATMEM 1
#define CONFIG_FLAT_NODE_MEM_MAP 1
#undef CONFIG_SPARSEMEM_STATIC

/*
 * Boot options
 */
#define CONFIG_CMDLINE ""
#define CONFIG_MEMSIZE 56
#define CONFIG_CPU_FREQ 300000000
#define CONFIG_DYNLOAD 1
#define CONFIG_PAGE_OFFSET 0x0
#define CONFIG_ISA_DMA_API 1

/*
 * Bus options (PCI, PCMCIA, EISA, MCA, ISA)
 */
#undef CONFIG_PCI

/*
 * PCCARD (PCMCIA/CardBus) support
 */
#undef CONFIG_PCCARD

/*
 * PCI Hotplug Support
 */

/*
 * Executable file formats
 */
#undef CONFIG_BINFMT_FLAT
#undef CONFIG_BINFMT_MISC

/*
 * Power management options
 */
#undef CONFIG_PM

/*
 * Networking
 */
#undef CONFIG_NET

/*
 * Device Drivers
 */

/*
 * Generic Driver Options
 */
#define CONFIG_STANDALONE 1
#undef CONFIG_PREVENT_FIRMWARE_BUILD
#undef CONFIG_FW_LOADER

/*
 * Connector - unified userspace <-> kernelspace linker
 */

/*
 * Memory Technology Devices (MTD)
 */
#undef CONFIG_MTD

/*
 * Parallel port support
 */
#undef CONFIG_PARPORT

/*
 * Plug and Play support
 */

/*
 * Block devices
 */
#undef CONFIG_BLK_DEV_COW_COMMON
#undef CONFIG_BLK_DEV_LOOP
#undef CONFIG_BLK_DEV_RAM
#define CONFIG_BLK_DEV_RAM_COUNT 16
#undef CONFIG_CDROM_PKTCDVD

/*
 * IO Schedulers
 */
#define CONFIG_IOSCHED_NOOP 1
#undef CONFIG_IOSCHED_AS
#undef CONFIG_IOSCHED_DEADLINE
#undef CONFIG_IOSCHED_CFQ

/*
 * ATA/ATAPI/MFM/RLL support
 */
#undef CONFIG_IDE

/*
 * SCSI device support
 */
#undef CONFIG_RAID_ATTRS
#undef CONFIG_SCSI

/*
 * Multi-device support (RAID and LVM)
 */
#undef CONFIG_MD

/*
 * Fusion MPT device support
 */
#undef CONFIG_FUSION

/*
 * IEEE 1394 (FireWire) support
 */

/*
 * I2O device support
 */

/*
 * Network device support
 */
#undef CONFIG_NETPOLL
#undef CONFIG_NET_POLL_CONTROLLER

/*
 * ISDN subsystem
 */

/*
 * Telephony Support
 */
#undef CONFIG_PHONE

/*
 * Input device support
 */
#define CONFIG_INPUT 1

/*
 * Userland interfaces
 */
#define CONFIG_INPUT_MOUSEDEV 1
#undef CONFIG_INPUT_MOUSEDEV_PSAUX
#define CONFIG_INPUT_MOUSEDEV_SCREEN_X 1024
#define CONFIG_INPUT_MOUSEDEV_SCREEN_Y 768
#undef CONFIG_INPUT_JOYDEV
#undef CONFIG_INPUT_TSDEV
#undef CONFIG_INPUT_TSLIBDEV
#undef CONFIG_INPUT_EVDEV
#undef CONFIG_INPUT_EVBUG

/*
 * Input Device Drivers
 */
#undef CONFIG_INPUT_KEYBOARD
#undef CONFIG_INPUT_MOUSE
#undef CONFIG_INPUT_JOYSTICK
#undef CONFIG_INPUT_TOUCHSCREEN
#undef CONFIG_INPUT_MISC

/*
 * Hardware I/O ports
 */
#undef CONFIG_SERIO
#undef CONFIG_GAMEPORT

/*
 * Character devices
 */
#define CONFIG_VT 1
#undef CONFIG_PL031
#define CONFIG_VT_CONSOLE 1
#define CONFIG_HW_CONSOLE 1
#undef CONFIG_SERIAL_NONSTANDARD

/*
 * Serial drivers
 */
#undef CONFIG_SERIAL_8250

/*
 * Non-8250 serial port support
 */
#define CONFIG_UNIX98_PTYS 1
#undef CONFIG_LEGACY_PTYS

/*
 * IPMI
 */
#undef CONFIG_IPMI_HANDLER

/*
 * Watchdog Cards
 */
#undef CONFIG_WATCHDOG
#undef CONFIG_RTC
#undef CONFIG_GEN_RTC
#undef CONFIG_DTLK
#undef CONFIG_R3964

/*
 * Ftape, the floppy tape device driver
 */
#undef CONFIG_RAW_DRIVER

/*
 * TPM devices
 */

/*
 * I2C support
 */
#undef CONFIG_I2C

/*
 * Dallas's 1-wire bus
 */
#undef CONFIG_W1

/*
 * Hardware Monitoring support
 */
#undef CONFIG_HWMON
#undef CONFIG_HWMON_VID

/*
 * Misc devices
 */

/*
 * Multimedia Capabilities Port drivers
 */

/*
 * Multimedia Capabilities Port drivers
 */

/*
 * Multimedia devices
 */
#undef CONFIG_VIDEO_DEV

/*
 * Digital Video Broadcasting Devices
 */

/*
 * Graphics support
 */
#undef CONFIG_FB

/*
 * Console display driver support
 */
#undef CONFIG_VGA_CONSOLE
#define CONFIG_DUMMY_CONSOLE 1

/*
 * Sound
 */
#undef CONFIG_SOUND

/*
 * USB support
 */
#undef CONFIG_USB_ARCH_HAS_HCD
#undef CONFIG_USB_ARCH_HAS_OHCI

/*
 * USB Gadget Support
 */
#undef CONFIG_USB_GADGET

/*
 * MMC/SD Card support
 */
#undef CONFIG_MMC

/*
 * InfiniBand support
 */

/*
 * SN Devices
 */

/*
 * File systems
 */
#undef CONFIG_EXT2_FS
#undef CONFIG_EXT3_FS
#undef CONFIG_JBD
#undef CONFIG_REISERFS_FS
#undef CONFIG_JFS_FS
#undef CONFIG_FS_POSIX_ACL
#undef CONFIG_XFS_FS
#undef CONFIG_MINIX_FS
#undef CONFIG_ROMFS_FS
#undef CONFIG_INOTIFY
#undef CONFIG_QUOTA
#define CONFIG_DNOTIFY 1
#undef CONFIG_AUTOFS_FS
#undef CONFIG_AUTOFS4_FS
#undef CONFIG_FUSE_FS

/*
 * CD-ROM/DVD Filesystems
 */
#undef CONFIG_ISO9660_FS
#undef CONFIG_UDF_FS

/*
 * DOS/FAT/NT Filesystems
 */
#undef CONFIG_MSDOS_FS
#undef CONFIG_VFAT_FS
#undef CONFIG_NTFS_FS

/*
 * Pseudo filesystems
 */
#undef CONFIG_PROC_FS
#define CONFIG_SYSFS 1
#undef CONFIG_TMPFS
#undef CONFIG_HUGETLB_PAGE
#define CONFIG_RAMFS 1
#undef CONFIG_RELAYFS_FS

/*
 * Miscellaneous filesystems
 */
#undef CONFIG_HFSPLUS_FS
#undef CONFIG_YAFFS_FS
#undef CONFIG_CRAMFS
#undef CONFIG_SQUASHFS
#undef CONFIG_VXFS_FS
#undef CONFIG_HPFS_FS
#undef CONFIG_QNX4FS_FS
#undef CONFIG_SYSV_FS
#undef CONFIG_UFS_FS

/*
 * Partition Types
 */
#undef CONFIG_PARTITION_ADVANCED
#define CONFIG_MSDOS_PARTITION 1

/*
 * Native Language Support
 */
#undef CONFIG_NLS

/*
 * Kernel hacking
 */
#undef CONFIG_PRINTK_TIME
#undef CONFIG_DEBUG_KERNEL
#define CONFIG_LOG_BUF_SHIFT 14
#undef CONFIG_WANT_EXTRA_DEBUG_INFORMATION
#undef CONFIG_FULLDEBUG
#undef CONFIG_BOOTPARAM
#undef CONFIG_NO_KERNEL_MSG

/*
 * Security options
 */
#undef CONFIG_KEYS
#undef CONFIG_SECURITY

/*
 * Cryptographic options
 */
#undef CONFIG_CRYPTO

/*
 * Hardware crypto devices
 */

/*
 * Library routines
 */
#undef CONFIG_CRC_CCITT
#undef CONFIG_CRC16
#undef CONFIG_CRC32
#undef CONFIG_LIBCRC32C
