#!/bin/bash

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

source $wrp_sysroot/machine.conf

TMCONFIG_FLAG="-tmconfig=$wrp_sysroot/../config/tmconfig"
STDINC_FLAG="-nostdinc -I$wrp_sysroot/compiler/$TCS_VERSION/include -I$wrp_sysroot/sysroot/usr/include/ -D__signed__=signed"
STDLIB_FLAG="-L$wrp_sysroot/sysroot/usr/lib/ -lc -lcstub"

DYNAPP_FLAG="-btype app"
APPMAIN="$wrp_sysroot/sysroot/usr/lib/crt0.o"

source $wrp_sysroot/../config/symrename.txt

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
