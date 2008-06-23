/*
 * root_group.c
 */

#include "userdb.h"

#ifndef __TCS__ 
const struct group __root_group = {
	.gr_name   = "root",
	.gr_passwd = "",
	.gr_gid    = 0,
	.gr_mem    = NULL
};
#else
const struct group __root_group = {
	"root",
	"",
	0,
	NULL
};
#endif 
