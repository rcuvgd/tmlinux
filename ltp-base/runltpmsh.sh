#!/bin/sh

export CMDFILE="syscalls02345"
export LTPROOT="/opt/ltp-base"
export TMPBASE="/tmp"
export TMP="${TMPBASE}/ltp-$$"
export PATH="${PATH}:${LTPROOT}/testcases/bin"

mkdir -p $TMP || \
{
	echo "FATAL: Unable to make temporary directory $TMP"
	exit 1
}

cd $TMP || \
{
	echo "could not cd ${TMP} ... exiting"
	exit 1
}

if   [ -f $CMDFILE ] || \
	CMDFILE="$LTPROOT/runtest/$CMDFILE"
then
	cat $CMDFILE > ${TMP}/alltests || \
	{
		echo "FATAL: Unable to create command file"
		exit 1
	}
fi

LOGFILE="-p -l testlog -C cmd.failed"

${LTPROOT}/pan/pan $QUIET_MODE -e -S $INSTANCES $DURATION -a $$ -n $$ $PRETTY_PRT -f ${TMP}/alltests $LOGFILE $OUTPUTFILE

#if [ $? -eq 0 ]; then
	#echo "INFO: pan reported all tests PASS"
	#VALUE=0
#else
	#echo "INFO: pan reported some tests FAIL"
	#VALUE=1
#fi

#exit $VALUE
