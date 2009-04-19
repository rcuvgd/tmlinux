#!/bin/bash

TCS_VERSION='5.1.0'
CORE='tm3260'
ENDIAN='el'
FP='fp64'
TARGET_FUNDAMENTAL_CFLAGS=' -target pnx1500 -host ttisim -el -fp64'
TARGET_FUNDAMENTAL_AFLAGS=' -target pnx1500 -host ttisim -el -fp64'

filename="$0"
dir_filename="${filename%/*}"
if [ "$dir_filename" = "$filename" ]; then
  dir_filename="."
fi

base_filename="${filename##*/}"

# Parse our tool name, splitting it at '-' characters.
IFS=- read wrp_toolchain_arch wrp_vendor wrp_os wrp_abi wrp_cpu wrp_platform wrp_binary << EOF
$base_filename
EOF

#
# We add the directory this was executed from to the PATH
# The toolchains (links) should be in this directory or in the users
# PATH.
#
wrp_bin_dir=""
if [ "$base_filename" = "$filename" ]; then
	OIFS=$IFS
	IFS=:
	for each_path in $PATH; do
		if [ -x "$each_path/$filename" ]; then
			wrp_bin_dir="$each_path"
			break
		fi
	done
	IFS=$OIFS
else
	wrp_bin_dir="$dir_filename"
	unset CDPATH
	wrp_bin_dir=$(cd "$wrp_bin_dir" && pwd)
fi

if [ -z "$wrp_bin_dir" ]; then
	echo "Error: Unable to determine execution path!" >&2
	exit 1
fi  

export PATH="$wrp_bin_dir":$PATH

wrp_sysroot="$wrp_bin_dir/../"
if [ ! -d "$wrp_sysroot" ]; then
	echo "Error: Unable to determine sysroot (looking for $wrp_sysroot)!" >&2
	exit 1
fi
TMCONFIG_FLAG="-tmconfig=$wrp_sysroot/tmconfig"
STDINC_FLAG="-nostdinc -I$wrp_sysroot/compiler/$TCS_VERSION/include -I$wrp_sysroot/sysroot/usr/include/ -D__signed__=signed"
STDLIB_FLAG="-L$wrp_sysroot/sysroot/usr/lib/ -lc -lcstub"

DYNAPP_FLAG="-btype app"
APPMAIN="$wrp_sysroot/sysroot/usr/lib/crt0.o"

SYMRENAME="-Dcalloc=tmlinux_calloc  	\
	-Dfree=tmlinux_free  		\
	-Dmalloc=tmlinux_malloc  	\
	-Drealloc=tmlinux_realloc 	\
	-Dlongjmp=tmlinux_longjmp 	\
	-Dmemmove=tmlinux_memmove 	\
	-Dmemset=tmlinux_memset 	\
	-Dmemchr=tmlinux_memchr 	\
	-Dmemcmp=tmlinux_memcmp 	\
	-Dmemcpy=tmlinux_memcpy 	\
	-Drename=tmlinux_rename 	\
	-Dexit=tmlinux_exit 		\
	-Datexit=tmlinux_atexit 	\
	-Dstrlen=tmlinux_strlen		\
	-Datoi=tmlinux_atoi  		\
	-Datol=tmlinux_atol  		\
	-Datoll=tmlinux_atoll  		\
	-Dfflush=tmlinux_fflush  	\
	-Dfprintf=tmlinux_fprintf 	\
	-Dfputs=tmlinux_fputs 		\
	-Dputc=tmlinux_putc 		\
	-Dfseek=tmlinux_fseek 		\
	-Dputs=tmlinux_puts  		\
	-Dsprintf=tmlinux_sprintf  	\
	-Dsscanf=tmlinux_sscanf 	\
	-Dstrcat=tmlinux_strcat  	\
	-Dstrchr=tmlinux_strchr 	\
	-Dstrcmp=tmlinux_strcmp  	\
	-Dstrcpy=tmlinux_strcpy  	\
	-Dstrncat=tmlinux_strncat  	\
	-Dstrncmp=tmlinux_strncmp  	\
	-Dstrncpy=tmlinux_strncpy  	\
	-Dstrrchr=tmlinux_strrchr  	\
	-Dstrtol=tmlinux_strtol  	\
	-Dstrtoll=tmlinux_strtoll  	\
	-Dstrtod=tmlinux_strtod  	\
	-Dremove=tmlinux_remove 	\
	-Dfread=tmlinux_fread 		\
	-Dfwrite=tmlinux_fwrite 	\
	-Datof=tmlinux_atof 		\
	-Dstrerror=tmlinux_strerror  	\
	-Dstrcoll=tmlinux_strcoll 	\
	-Dstrpbrk=tmlinux_strpbrk 	\
	-Dstrstr=tmlinux_strstr 	\
	-Dstrspn=tmlinux_strspn 	\
	-Dstrcspn=tmlinux_strcspn 	\
	-Dtolower=tmlinux_tolower  	\
	-Dtoupper=tmlinux_toupper  	\
	-Dtmpnam=tmlinux_tmpnam  	\
	-Dqsort=tmlinux_qsort 		\
	-Dvfprintf=tmlinux_vfprintf  	\
	-Dvsprintf=tmlinux_vsprintf  	\
	-Dprintf=tmlinux_printf  	\
	-Dbsearch=tmlinux_bsearch  	\
	-Dsignal=tmlinux_signal  	\
	-Dsystem=tmlinux_system  	\
	-Dsleep=tmlinux_sleep           \
	-Dgetenv=tmlinux_getenv  	\
	-Dputenv=tmlinux_putenv  	\
	-Dclose=tmlinux_close  		\
	-Dclosedir=tmlinux_closedir  	\
	-Disatty=tmlinux_isatty  	\
	-Dopendir=tmlinux_opendir  	\
	-Dread=tmlinux_read  		\
	-Dreaddir=tmlinux_readdir  	\
	-Dperror=tmlinux_perror 	\
	-Dfclose=tmlinux_fclose 	\
	-Dfopen=tmlinux_fopen 		\
	-Dftell=tmlinux_ftell 		\
	-Dfcntl=tmlinux_fcntl 		\
	-Dfreopen=tmlinux_freopen 	\
	-Dlink=tmlinux_link 		\
	-Dunlink=tmlinux_unlink 	\
	-Dmkdir=tmlinux_mkdir 		\
	-Dfsync=tmlinux_fsync 		\
	-Dopen=tmlinux_open 		\
	-Dfgets=tmlinux_fgets 		\
	-Dgets=tmlinux_gets 		\
	-Dungetc=tmlinux_ungetc 	\
	-Dstat=tmlinux_stat 		\
	-Dfstat=tmlinux_fstat 		\
	-Dlseek=tmlinux_lseek 		\
	-Dabort=tmlinux_abort 		\
	-Denviron=__environ 		\
	-Draise=tmlinux_raise 		\
	-Dstrtoul=tmlinux_strtoul 	\
	-Dstrtoull=tmlinux_strtoull 	\
	-Drand=tmlinux_rand 		\
	-Dsrand=tmlinux_srand 		\
	-Dsetvbuf=tmlinux_setvbuf 	\
	-Dsetbuf=tmlinux_setbuf 	\
	-Drewind=tmlinux_rewind 	\
	-Daccess=tmlinux_access 	\
	-Dstrtok=tmlinux_strtok 	\
	-Dfscanf=tmlinux_fscanf 	\
	-Dscanf=tmlinux_scanf 		\
	-Dasctime=tmlinux_asctime 	\
	-Ddifftime=tmlinux_difftime 	\
	-Dtime=tmlinux_time 		\
	-Dstrftime=tmlinux_strftime 	\
	-Dgmtime=tmlinux_gmtime 	\
	-Dlocaltime=tmlinux_localtime 	\
	-Dctime=tmlinux_ctime 		\
	-Dsigprocmask=tmlinux_sigprocmask \
	-Dwrite=tmlinux_write "

#
# Run the cross-tool.
#
case $wrp_binary in

cc|gcc|g++|c++)
    exec tmcc -tmld $APPMAIN -- $DYNAPP_FLAG $SYMRENAME $STDINC_FLAG $TMCONFIG_FLAG $TARGET_FUNDAMENTAL_CFLAGS "$@" $STDLIB_FLAG 
    ;;

cpp)
    exec tmcc $APPMAIN $DYNAPP_FLAG $STDINC_FLAG $TMCONFIG_FLAG $TARGET_FUNDAMENTAL_CFLAGS "$@" $SYMRENAME $STDLIB_FLAG 
    ;;

ld)
    exec tmld "$@"
    ;;

as)
    exec tmcc $APPMAIN $DYNAPP_FLAG $SYMRENAME $STDLIB_FLAG $STDINC_FLAG $TMCONFIG_FLAG $TARGET_FUNDAMENTAL_AFLAGS "$@"
    ;;

strip)
    ;;

*)
    exec tm$wrp_binary "$@"
    ;;
esac

exit 0
