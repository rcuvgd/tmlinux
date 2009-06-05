#!/bin/sh

output="linux_syscall_numbers.h"

rm -f ${output}

cat << EOF > ${output}
/************************************************
 * GENERATED FILE: DO NOT EDIT/PATCH THIS FILE  *
 *  change your arch specific .in file instead  *
 ************************************************/

/*
 * Here we stick all the ugly *fallback* logic for linux
 * system call numbers (those __NR_ thingies).
 *
 * Licensed under the GPLv2 or later, see the COPYING file.
 */

#ifndef __LINUX_SYSCALL_NUMBERS_H__
#define __LINUX_SYSCALL_NUMBERS_H__

#include <sys/syscall.h>

#ifndef __TCS__
static void cleanup(void);

#define syscall(NR, ...) ({ \\
	int __ret; \\
	if (NR == 0) { \\
		tst_brkm(TCONF, cleanup, "syscall " #NR " not supported on your arch"); \\
		errno = ENOSYS; \\
		__ret = -1; \\
	} else \\
		__ret = syscall(NR, ##__VA_ARGS__); \\
	__ret; \\
})
#endif 
EOF

for arch in `cat order` ; do
	echo -n "Generating data for arch $arch ... "

	echo "" >> ${output}
	echo "#ifdef __${arch}__" >> ${output}
	while read line ; do
		set -- $line
		nr=$1
		shift
		if [ -z "$*" ] ; then
			echo "invalid line found"
			exit 1
		fi
		cat <<-EOF >> ${output}
		# ifndef $nr
		#  define $nr $*
		# endif
		EOF
	done < ${arch}.in
	echo "#endif" >> ${output}
	echo "" >> ${output}

	echo "OK!"
done

echo -n "Generating stub list ... "
echo "" >> ${output}
for nr in $(awk '{print $1}' *.in | sort -u) ; do
	cat <<-EOF >> ${output}
	# ifndef $nr
	#  define $nr 0
	# endif
	EOF
done
echo "" >> ${output}
echo "OK!"

echo "" >> ${output}
echo "#endif" >> ${output}
