#
#  Copyright (c) International Business Machines  Corp., 2001
#
#  This program is free software;  you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY;  without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
#  the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program;  if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

DEBUG_CFLAGS=-O
DEBUG_LDFLAGS=-s
CFLAGS+=-I../../include
LDLIBS+=
EXCLUDE_DIR=epoll

#
# Commented this out since there are directories here we don't want built by default
#
SUBDIR = `ls */Makefile | sed "s/Makefile//g"` 
UCLINUX_SUBDIR = `ls */Makefile | sed "s/Makefile//g" | grep -vE "^fork|epoll|nftw|profil|getcontext|getcpu|mallopt|move_pages|pselect|remap_file_pages|signalfd|sysctl|sysfs|syslog|vmsplice|swapon|swapoff|msync|mremap|mlock|munlock|madvise"`

all:
	@set -e; for i in $(SUBDIR); do \
		$(MAKE) -C $$i  all; \
	done

install:
	@set -e; for i in $(SUBDIR); do \
		$(MAKE) -C $$i  install; \
	done

uclinux:  
	@set -e; for i in $(UCLINUX_SUBDIR); do \
		$(MAKE) -C $$i UCLINUX=1 all; \
	done

uclinux_install:
	@set -e; for i in $(UCLINUX_SUBDIR); do \
		$(MAKE) -C $$i UCLINUX=1 install; \
	done

clean:
	@set -e; for i in $(SUBDIR); do \
		$(MAKE) -C $$i  clean; \
	done

