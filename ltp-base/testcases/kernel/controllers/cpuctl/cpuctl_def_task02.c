/******************************************************************************/
/*                                                                            */
/* Copyright (c) International Business Machines  Corp., 2007                 */
/*                                                                            */
/* This program is free software;  you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation; either version 2 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY;  without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See                  */
/* the GNU General Public License for more details.                           */
/*                                                                            */
/* You should have received a copy of the GNU General Public License          */
/* along with this program;  if not, write to the Free Software               */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA    */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* File:        cpuctl_task_def02.c                                           */
/*                                                                            */
/* Description: This is a c program that runs as a default task in a default  */
/*              group to create an ideal scenario. In this default group all  */
/*              the system tasks will be running along with this spinning task*/
/*              The file is to be used by tests 4-5.                          */
/*                                                                            */
/* Total Tests: 2                                                             */
/*                                                                            */
/* Test 04:     Nice value effect on group scheduling                         */
/* Test 05:     Task migration test                                           */
/*                                                                            */
/* Test Name:   cpu_controller_test02                                         */
/*                                                                            */
/* Test Assertion                                                             */
/*              Please refer to the file cpuctl_testplan.txt                  */
/*                                                                            */
/* Author:      Sudhir Kumar skumar@linux.vnet.ibm.com                        */
/*                                                                            */
/* History:                                                                   */
/* Created-     18/11/2008 -Sudhir Kumar <skumar@linux.vnet.ibm.com>          */
/*                                                                            */
/******************************************************************************/

/* Standard Include Files */
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "../libcontrollers/libcontrollers.h"
#include "test.h"		/* LTP harness APIs*/

#define TIME_INTERVAL	30	/* Time interval in seconds*/
#define NUM_INTERVALS	3       /* How many iterations of TIME_INTERVAL */

extern int Tst_count;
char *TCID = "cpu_controller_test04";
int TST_TOTAL = 1;
pid_t scriptpid;
char path[] = "/dev/cpuctl";

extern void
cleanup()
{
	kill(scriptpid, SIGUSR1);/* Inform the shell to do cleanup*/
	tst_exit();		/* Report exit status*/
}

int timer_expired = 0;

int main(int argc, char *argv[])
{

	int test_num, task_num, len, num_cpus;
	int migrate = 0;		/* For task migration*/
	char mygroup[FILENAME_MAX], mytaskfile[FILENAME_MAX];
	char mysharesfile[FILENAME_MAX], ch;
	/* Following variables are to capture parameters from script*/
	char *group_num_p, *mygroup_p, *script_pid_p, *num_cpus_p;
	char *test_num_p, *task_num_p;
	pid_t pid;
	int mygroup_num,	        /* A number attached with a group*/
		fd,          	        /* to open a fifo to synchronize*/
		counter = 0; 	 	/* To take n number of readings*/
	double total_cpu_time,  	/* Accumulated cpu time*/
		delta_cpu_time,  	/* Time the task could run on cpu(s)*/
		prev_cpu_time = 0;
	double exp_cpu_time;            /* Exp time in % by shares calculation*/

	struct rusage cpu_usage;
	time_t current_time, prev_time, delta_time;
	unsigned int fmyshares, num_tasks;
	struct sigaction newaction, oldaction;
	/* Signal handling for alarm*/
	sigemptyset(&newaction.sa_mask);
	newaction.sa_handler = signal_handler_alarm;
	newaction.sa_flags = 0;
	sigaction(SIGALRM, &newaction, &oldaction);

	/* Collect the parameters passed by the script */
	group_num_p	= getenv("GROUP_NUM");
	mygroup_p	= getenv("MYGROUP");
	script_pid_p 	= getenv("SCRIPT_PID");
	num_cpus_p 	= getenv("NUM_CPUS");
	test_num_p 	= getenv("TEST_NUM");
	task_num_p 	= getenv("TASK_NUM");
	/* Check if all of them are valid */
	if ((test_num_p != NULL) && (((test_num = atoi(test_num_p)) == 4) || \
					((test_num = atoi(test_num_p)) == 5))) {
		if ((group_num_p != NULL) && (mygroup_p != NULL) && \
			(script_pid_p != NULL) && (num_cpus_p != NULL) && \
				 (task_num_p != NULL)) {
			mygroup_num	 = atoi(group_num_p);
			scriptpid	 = atoi(script_pid_p);
			num_cpus	 = atoi(num_cpus_p);
			task_num	 = atoi(task_num_p);
			sprintf(mygroup, "%s", mygroup_p);
		} else {
			tst_brkm(TBROK, cleanup,
					 "Invalid other input parameters\n");
		}
	} else {
		tst_brkm(TBROK, cleanup, "Invalid test number passed\n");
	}

	sprintf(mytaskfile, "%s", mygroup);
	sprintf(mysharesfile, "%s", mygroup);
	strcat(mytaskfile, "/tasks");
	strcat(mysharesfile, "/cpu.shares");
	pid = getpid();
	write_to_file(mytaskfile, "a", pid);    /* Assign task to it's group*/

	fd = open("./myfifo", 0);
	if (fd == -1)
		tst_brkm(TBROK, cleanup,
				 "Could not open fifo for synchronization");

	read(fd, &ch, 1);	/* Block task here to synchronize */

	/*
	 * We now calculate the expected % cpu time of this task by getting
	 * it's group's shares, the total shares of all the groups and the
	 * number of tasks in this group.
	 */
	FLAG = 0;
	total_shares = 0;
	shares_pointer = &total_shares;
	len = strlen(path);
	if (!strncpy(fullpath, path, len))
		tst_brkm(TBROK, cleanup,
				 "Could not copy directory path %s ", path);

	if (scan_shares_files(shares_pointer) != 0)
		tst_brkm(TBROK, cleanup,
			 "From function scan_shares_files in %s ", fullpath);

	/* return val -1 in case of function error, else 2 is min share value */
	if ((fmyshares = read_shares_file(mysharesfile)) < 2)
		tst_brkm(TBROK, cleanup,
				 "in reading shares files  %s ", mysharesfile);

	if ((read_file(mytaskfile, GET_TASKS, &num_tasks)) < 0)
		tst_brkm(TBROK, cleanup,
				 "in reading tasks files  %s ", mytaskfile);

	exp_cpu_time = (double)(fmyshares * 100) / (total_shares * num_tasks);

	prev_time = time(NULL);	 /* Note down the time*/

	while (1) {
		/*
		 * Need to run some cpu intensive task, which also
		 * frequently checks the timer value
		 */
		double f = 274.345, mytime;	/*just a float number for sqrt*/
		alarm(TIME_INTERVAL);
		timer_expired = 0;
		/*
		 * Let the task run on cpu for TIME_INTERVAL. Time of this
		 * operation should not be high otherwise we can exceed the
		 * TIME_INTERVAL to measure cpu usage
		 */
		while (!timer_expired)
			f = sqrt(f * f);

		current_time = time(NULL);
		/* Duration in case its not exact TIME_INTERVAL*/
		delta_time = current_time - prev_time;

		getrusage(0, &cpu_usage);
		/* total_cpu_time = total user time + total sys time */
		total_cpu_time = (cpu_usage.ru_utime.tv_sec +
				cpu_usage.ru_utime.tv_usec * 1e-6 +
				cpu_usage.ru_stime.tv_sec +
				cpu_usage.ru_stime.tv_usec * 1e-6) ;
		delta_cpu_time = total_cpu_time - prev_cpu_time;

		prev_cpu_time = total_cpu_time;
		prev_time = current_time;

		/* calculate % cpu time each task gets */
		if (delta_time > TIME_INTERVAL)
			mytime = (delta_cpu_time * 100) /
					 (delta_time * num_cpus);
		else
			mytime = (delta_cpu_time * 100) /
					 (TIME_INTERVAL * num_cpus);

		fprintf(stdout, "Grp:-%3dDEF task-%3d: CPU TIME{calc:-%6.2f(s)"
			"i.e. %6.2f(%%)exp:-%6.2f(%%)} with %u(shares) in %lu"
			" (s)\n", mygroup_num, task_num, delta_cpu_time, mytime,
			exp_cpu_time, fmyshares, delta_time);

		counter++;

		if (counter >= NUM_INTERVALS) {
			switch (test_num) {
			case 4:			/* Test04 */
				exit(0);	/* This task is done its job*/
				break;
			case 5:			/* Test 05 */
				if (migrate == 0) {
					counter = 0;
					migrate = 1;
				} else {
					exit(0);
				}
				break;
			default:
				tst_brkm(TBROK, cleanup,
					 "Invalid test number passed\n");
				break;

			}	/* end switch*/
		}
	}	/* end while*/
}	/* end main*/

