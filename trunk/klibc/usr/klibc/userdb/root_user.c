/*
 * root_user.c
 *
 */

#include "userdb.h"
#include <paths.h>

#ifndef __TCS__
const struct passwd __root_user = {
	.pw_name    = "root",
	.pw_passwd  = "",
	.pw_uid     = 0,
	.pw_gid     = 0,
	.pw_gecos   = "root",
	.pw_dir     = "/",
	.pw_shell   = _PATH_BSHELL
};
#else
const struct passwd __root_user = {
	"root",
	"",
	0,
	0,
	"root",
	"/",
	_PATH_BSHELL
};
#endif 
