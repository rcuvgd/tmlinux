/*  Copyright (C) 2003     Manuel Novoa III
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *  Supply some weaks for use by strerror*(), etc.
 *
 *  Aug 30, 2003
 *  Add some hidden names to support locale-enabled libstd++.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#undef __OPTIMIZE__
#include <libintl.h>

/**********************************************************************/
#ifdef L___uClibc_dgettext

char *__uClibc_dgettext(const char *domainname,
						const char *msgid)
{
	return (char *) msgid;
}

#ifndef __TCS__
weak_alias(__uClibc_dgettext, __dgettext)
#else
char *__dgettext(const char *domainname,
						const char *msgid)
{
	return __uClibc_dgettext(domainname,msgid);
}
#endif

#endif
/**********************************************************************/
#ifdef L___uClibc_dcgettext

char *__uClibc_dcgettext(const char *domainname,
						 const char *msgid, int category)
{
	return (char *) msgid;
}

#ifndef __TCS__
weak_alias(__uClibc_dcgettext, __dcgettext)
#else
char *__dcgettext(const char *domainname,
						 const char *msgid, int category)
{
	return __uClibc_dcgettext(domainname,msgid,category);
}
#endif

#endif
/**********************************************************************/
#ifdef L___uClibc_textdomain

char *__uClibc_textdomain(const char *domainname)
{
	static const char default_str[] = "messages";

	if (domainname && *domainname && __strcmp(domainname, default_str)) {
		__set_errno(EINVAL);
		return NULL;
	}
	return (char *) default_str;
}

#ifndef __TCS__
weak_alias(__uClibc_textdomain, __textdomain)
#else
char *__textdomain(const char *domainname)
{
	return __uClibc_textdomain(domainname);
}
#endif

#endif
/**********************************************************************/
#ifdef L___uClibc_bindtextdomain

char *__uClibc_bindtextdomain(const char *domainname, const char *dirname)
{
	static const char dir[] = "/";

	if (!domainname || !*domainname
		|| (dirname
#if 1
			&& ((dirname[0] != '/') || dirname[1])
#else
			&& __strcmp(dirname, dir)
#endif
			)
		) {
		__set_errno(EINVAL);
		return NULL;
	}

	return (char *) dir;
}

#ifndef __TCS__
weak_alias(__uClibc_bindtextdomain, __bindtextdomain)
#else
char *__bindtextdomain(const char *domainname, const char *dirname)
{
	return __uClibc_bindtextdomain(domainname,dirname);
}
#endif

#endif
/**********************************************************************/
