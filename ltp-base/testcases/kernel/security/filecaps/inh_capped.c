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
 * File: inh_capped
 * Author: Serge Hallyn
 * Purpose: test that CAP_SETPCAP is needed to add bits to pI
 * Uses no command line arguments.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/capability.h>
#include <test.h>

char *TCID = "filecaps";
int TST_TOTAL=1;

#define DEBUG 1

void debug_print_caps(char *when)
{
#ifdef DEBUG
	char buf[2000];
	tst_resm(TINFO, "%s", when);
	snprintf(buf, 2000, "%s", cap_to_text(cap_get_proc(), NULL));
	tst_resm(TINFO, "%s", buf);
#endif
}

int set_caps_from_text(char *capstr)
{
	cap_t caps = cap_from_text(capstr);
	int ret;

	if (!caps) {
		tst_resm(TFAIL, "Bad capability name: %s\n", capstr);
		return 1;
	}
	ret = cap_set_proc(caps);
	cap_free(caps);
	return ret;
}

int main()
{
	int ret;

	debug_print_caps("start");
	ret = set_caps_from_text("all=eip");
	debug_print_caps("after raising all caps");
	if (ret) {
		tst_resm(TFAIL, "failed to raise all caps");
		tst_exit(ret);
	}

	ret = set_caps_from_text("all=iep cap_sys_admin-iep");
	debug_print_caps("after first drop cap_sys_admin");
	if (ret) {
		tst_resm(TFAIL, "failed to drop capsysadmin from pI");
		tst_exit(ret);
	}

	/* we can't regain cap_sys_admin in pE or pP, only pI */
	ret = set_caps_from_text("all=eip cap_sys_admin-ep+i");
	debug_print_caps("after first raise cap_sys_admin");
	if (ret) {
		tst_resm(TFAIL, "failed to raise capsysadmin in pI");
		tst_exit(ret);
	}

	ret = set_caps_from_text("all=ip cap_setpcap-e+ip cap_sys_admin+i-ep");
	debug_print_caps("after drop cappset");
	if (ret) {
		tst_resm(TFAIL, "failed to drop cappset from pE");
		tst_exit(ret);
	}

	ret = set_caps_from_text("all=iep cap_sys_admin-iep cap_setpcap-e+ip");
	debug_print_caps("after second drop cap_sys_admin");
	if (ret) {
		tst_resm(TFAIL, "failed to drop capsysadmin from pI "
				"after dropping cappset from pE");
		tst_exit(ret);
	}

	ret = set_caps_from_text("all=iep cap_sys_admin-ep+i cap_setpcap-e+ip");
	debug_print_caps("final");
	if (ret) {
		tst_resm(TPASS, "pI is properly capped\n");
		tst_exit(0);
	}

	tst_resm(TFAIL, "succeeded raising capsysadmin in pI "
			"without having setpcap");
	tst_exit(ret);
}
