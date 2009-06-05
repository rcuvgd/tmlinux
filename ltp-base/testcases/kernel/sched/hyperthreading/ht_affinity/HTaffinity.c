/***************************************************************************
                          HTaffinity.c  -  description
                             -------------------
    email                : sonic,zhang@intel.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "HTutils.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

char *TCID = "ht_affinity";
int TST_TOTAL = 3;

/************************************************************************************
int set_affinity(pid_t pid, unsigned int len, unsigned long *mask_ptr)
pid - pid of the process whose affinity is desired to be set.
mask_ptr - pointer to the new cpu_affinity_mask.
len - length in bytes of the bitmask pointed to by user_mask_ptr.

int get_affinity(pid_t pid, unsigned int len, unsigned long *mask_ptr)
pid - pid of the process whose affinity is being read.
mask_ptr pointer to store the current affinity information.
len - length in bytes of the bitmask pointed to by user_mask_ptr.
************************************************************************************/

//Any application program can invoke these system call using sched_setaffinity() and sched_getaffinity(),
//with the syntax mentioned in the previous section, after declaring the interface as:

#define sched_setaffinity(pid, cpusetsize, mask) syscall(__NR_sched_setaffinity, pid, cpusetsize, mask)
#define sched_getaffinity(pid, cpusetsize, mask) syscall(__NR_sched_getaffinity, pid, cpusetsize, mask)

#define AFFINITY_NAME "affinity"
#define PROCFS_PATH "/proc/"

int HT_SetAffinity()
{
	unsigned mask;
	pid_t pid;
	int result=1;
	int cpu_count, i, j, k, cpuid;

	pid=getpid();

	tst_resm(TINFO, "Set affinity through system call.\n");

	cpu_count=get_cpu_count();
	if(cpu_count==0)
	{
	 	return 0;
	}
	else if(cpu_count>32)
		cpu_count=32;

	for(i=0, mask=0x1;i<cpu_count;i++, mask=mask<<1)
	{
		tst_resm(TINFO, "Set test process affinity.");
		printf("mask: %x\n",mask);

		sched_setaffinity(pid, sizeof(unsigned long), &mask);

		for(j=0;j<10;j++)
		{
			for(k=0;k<10;k++)
			{
				if(fork()==0)
				{
					system("ps > /dev/null");
					exit(0);
				}
			}

			sleep(1);

			if(get_current_cpu(pid)!=i)
				break;
		}

		if(j<10)
		{
			tst_resm(TINFO, "...Error\n");
			result=0;
		}
		else
			tst_resm(TINFO, "...OK\n");

	}

	for(i=0, mask=0x3;i<cpu_count-1;i++, mask=mask<<1)
	{
		tst_resm(TINFO, "Set test process affinity.");
		printf("mask: %x\n",mask);

		sched_setaffinity(pid, sizeof(unsigned long), &mask);

		for(j=0;j<10;j++)
		{
			for(k=0;k<10;k++)
			{
				if(fork()==0)
				{
					system("ps > /dev/null");
					exit(0);
				}
			}

			sleep(1);

			cpuid=get_current_cpu(pid);
			if(cpuid!=i&&cpuid!=i+1)
				break;
		}

		if(j<10)
		{
			tst_resm(TINFO, "...Error\n");
			result=0;
		}
		else
			tst_resm(TINFO, "...OK\n");

	}

	if(result)
		return 1;
	else
		return 0;
}

unsigned long get_porc_affinity(pid_t pid)
{
	FILE *pfile;

	sprintf(buf, "%s%d/%s%c", PROCFS_PATH, pid, AFFINITY_NAME, 0);

	if((pfile=fopen(buf, "r"))==NULL)
		return 0;

	if(fgets(buf, 255, pfile)==NULL)
	{
		fclose(pfile);
		return 0;
	}

	fclose(pfile);

	return atol(buf);
}

int HT_GetAffinity()
{
	unsigned long mask,mask1,mask2;
	pid_t pid;

	mask=0x1;
	pid=getpid();

	tst_resm(TINFO, "Get affinity through system call.\n");

	sched_setaffinity(pid, sizeof(unsigned int), &mask);

	sleep(1);

	mask1=get_porc_affinity(pid);
	sched_getaffinity(pid, sizeof(unsigned int), &mask2);

	if(mask==0x1 && mask==mask1 && mask==mask2)
	{
		mask=0x2;
		sched_setaffinity(pid, sizeof(unsigned long), &mask);

		sleep(1);

		mask1=get_porc_affinity(pid);
		sched_getaffinity(pid, sizeof(unsigned int), &mask2);

		if(mask==0x2 && mask==mask1 && mask==mask2)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

// return 0 means Pass, return 1 means Fail
int HT_InheritAffinity()
{
	unsigned long mask;
	pid_t pid;

	mask=0x2;
	pid=getpid();

	tst_resm(TINFO, "Inherit affinity from parent process.\n");

	sched_setaffinity(pid, sizeof(unsigned int), &mask);

	sleep(1);

	if((pid=fork())==0)
	{
		sleep(1);
		system("ps > /dev/null");
		exit(0);
	}

	mask=get_porc_affinity(pid);

	if(mask==0x2)
		return 1;
	else
		return 0;
}

// return 0 means Pass, return 1 means Fail
int main(int argc, char *argv[])
{
	tst_resm(TINFO, "Begin: HyperThreading Affinity");

#ifndef ARCH_i386
	tst_brkm(TCONF, NULL, "This test suite can only excute on i386 architecture.");
#else
	if (!check_ht_capability())
	{

		if(HT_GetAffinity())
			tst_resm(TPASS, "System call getaffinity() is OK.");
		else
			tst_resm(TFAIL, "System call getaffinity() is error.");

		printf("\n");

		if(HT_InheritAffinity())
			tst_resm(TPASS, "Inheritance of affinity is OK.");
		else
			tst_resm(TFAIL, "Inheritance of affinity is error.");

		printf("\n");

		if(HT_SetAffinity())
			tst_resm(TPASS, "System call setaffinity() is OK.");
		else
			tst_resm(TFAIL, "System call setaffinity() is error.");
	}
	else
	{
		tst_brkm(TCONF, NULL, "HT is not enabled or not supported.");
	}
#endif

	tst_resm(TINFO, "End: HyperThreading Affinity");

	return 0;
}
