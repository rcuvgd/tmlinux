#!/bin/bash
# usage ./runcpuctl_latency_test.sh

###############################################################################
#  Copyright (c) International Business Machines  Corp., 2008                 #
#                                                                             #
#  This program is free software;  you can redistribute it and/or modify      #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation; either version 2 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY;  without even the implied warranty of            #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See                  #
#  the GNU General Public License for more details.                           #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program;  if not, write to the Free Software               #
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA    #
#                                                                             #
###############################################################################
# Name Of File: run_cpuctl_test.sh                                            #
#                                                                             #
# Description: This file runs the setup for testing latency under heavy load  #
#              when group scheduling is enabled                               #
#              After setup it runs diff test cases in diff setup.             #
#                                                                             #
# Test 01:     Tests latency when no cgroup is mounted                        #
# Test 02:     Tests latency when cgroup is mounted                           #
#                                                                             #
#                                                                             #
# Author:       Sudhir Kumar   <skumar@linux.vnet.ibm.com>                    #
#                                                                             #
# History:                                                                    #
#                                                                             #
#  DATE         NAME           EMAIL                         DESC             #
#                                                                             #
#  26/11/08  Sudhir Kumar <skumar@linux.vnet.ibm.com>   Created this test     #
#                                                                             #
###############################################################################


NUM_TASKS=100;
NUM_GROUPS=2;	#for default
check_task="";
declare -a pid;

export TCID="cpuctl_latency_test";
export TST_COUNT=1;
export TST_TOTAL=2;

. parameters.sh

latency_cleanup()
{
	local i;

	if [ -n "${pid[1]}" ]; then
		for i in $(seq 1 $NUM_TASKS)
		do
			kill -s SIGUSR1 ${pid[$i]} 2>/dev/null;
		done
	fi

	sleep 2;
	if [ $TEST_NUM -eq 2 ]; then
		# move any remaining task
		sleep 2;
		for task in `cat /dev/cpuctl/group*/tasks`; do
	                echo $task > /dev/cpuctl/tasks #2>/dev/null 1>&2;
	        done
	        rmdir /dev/cpuctl/group* #2> /dev/null
        	umount /dev/cpuctl #2> /dev/null
	        rmdir /dev/cpuctl #2> /dev/null
		rm -f myfifo;
	fi;
	echo "Cleanup done for latency test $TEST_NUM"
	echo
}

PWD=`pwd`

################################
# Start
################################

	TEST_NUM=$1;
	if [ -z $TEST_NUM  ]; then
		echo "Invalid test no passed to test script"
		echo "Exiting the test..."
		exit 1;
	fi;

	cd $LTPROOT/testcases/bin/ #Bad idea????????

	# Check if sources are compiled
	if [ ! -f $cpuctl_latency_test ] || [ ! -f $cpuctl_latency_check_task ]
	then
		echo "TBROK The sources are not compiled...."
		cd $PWD;
		exit 1;
	fi

	# Keep the signal handler ready
	trap 'echo "Doing cleanup"; latency_cleanup;' 0;

	case $TEST_NUM in
	"1")	#without creating any groups
		# Run the load creating tasks
		echo TINFO "Running cpuctl Latency Test 1"
		for i in $(seq 1 $NUM_TASKS)
		do
			# Execute the load tasks
			./cpuctl_latency_test $TEST_NUM &
			if [ $? -ne 0 ]
			then
				echo "TBROK Failed to execute binary"
				exit;
			else
				pid[$i]=$!;
			fi
		done

		# Run the latency checking task
		./cpuctl_latency_check_task $TEST_NUM $$ &
		if [ $? -ne 0 ]
		then
			echo "TBROK Failed to execute main binary"
			exit;
		else
			check_task=$!;
		fi;
		;;

	"2")	# With group scheduling
		echo TINFO "Running cpuctl Latency Test 2"

		NUM_CPUS=`cat /proc/cpuinfo | grep -w processor | wc -l`;
		get_num_groups;	# NUM_GROUPS is set now
		do_setup;

		for num in $(seq 1 $NUM_TASKS)
		do
			group=/dev/cpuctl/group_`expr $num % $NUM_GROUPS + 1`;
			./cpuctl_latency_test $TEST_NUM $group &
			if [ $? -ne 0 ]
			then
				echo "TBROK Failed to execute binary"
				exit;
			else
				pid[$num]=$!;
			fi;
		done;

		# Run the latency checking task in any group
		./cpuctl_latency_check_task $TEST_NUM $$ $group &
		if [ $? -ne 0 ]
		then
			echo "TBROK Failed to execute main binary";
			exit;
		else
			check_task=$!;
		fi;
		;;

	* )	# wrong test num
		echo TBROK "Invalid test number passed to the script"
		exit;
		;;
	esac;

	wait $check_task;
	RC=$?;  # Return status of the task being waited
	if [ $RC -ne 0 ]
	then
		echo "Task $check_task exited abnormaly with return value: $RC";
		echo TBROK "Test could'nt execute for expected duration";
	fi
	cd $PWD;
