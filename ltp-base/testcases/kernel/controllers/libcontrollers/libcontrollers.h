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
/* File:        libcontrollers.h                                              */
/*                                                                            */
/* Description: This file contains the declarations for the functions and     */
/*              variables used by the library and the test files.             */
/*                                                                            */
/* Author:      Sudhir Kumar skumar@linux.vnet.ibm.com                        */
/*                                                                            */
/* History:                                                                   */
/* Created-     15/02/2008 -Sudhir Kumar <skumar@linux.vnet.ibm.com>          */
/*                                                                            */
/******************************************************************************/

/* Standard Include Files */
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef PATH_MAX
char fullpath[PATH_MAX];
#else
char fullpath[1024]; /* Guess */
#endif

int FLAG, timer_expired;

int retval;

unsigned int num_line;//??

unsigned int current_shares;

unsigned int total_shares;

unsigned int *shares_pointer;//??

char target[LINE_MAX];

struct dirent 	*dir_pointer;

enum{
	GET_SHARES	=1,
	GET_TASKS
};

inline int error_function(char *msg1, char *msg2);

unsigned int read_shares_file (char *filepath);

int read_file(char *filepath, int action, unsigned int *value);

int scan_shares_files(unsigned int *shares_pointer);

int write_to_file (char * file, const char* mode, unsigned int value);

void signal_handler_alarm (int signal );

void signal_handler_sigusr2 (int signal);
