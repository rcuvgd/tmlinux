/******************************************************************************/
/*                                                                            */
/* Copyright (c) International Business Machines  Corp., 2008                 */
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
/*
 * File: print_caps.c
 * Author: Serge Hallyn
 * Purpose: print out the POSIX capabilities with which it runs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFOFILE "caps_fifo"

int main(int argc, char *argv[])
{
	cap_t cap = cap_get_proc();
	int fd;
	int seqno = 0;
	char buf[2000];

	if (argc>1)
		seqno = atoi(argv[1]);

	if (!cap) {
		perror("print_caps - cap_get_proc");
		exit(1);
	}

	fd = open(FIFOFILE, O_WRONLY);
	if (!fd) {
		perror("print_caps: open fifo");
		exit(2);
	}

	snprintf(buf, 2000, "%d.%s", seqno, cap_to_text(cap, NULL));
	write(fd, buf, strlen(buf)+1);
	close(fd);

	cap_free(cap);

	return 0;
}
