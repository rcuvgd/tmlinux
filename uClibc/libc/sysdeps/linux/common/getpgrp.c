/* vi: set sw=4 ts=4: */
/*
 * getpgrp() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>

#ifdef __NR_getpgrp
_syscall0(pid_t, getpgrp);
#endif
