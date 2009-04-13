# Makefile for uClibc
#
# Copyright (C) 2000-2005 Erik Andersen <andersen@uclibc.org>
#
# Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
#

top_builddir=../
-include $(top_builddir).config

.EXPORT_ALL_VARIABLES:


ALL_SUBDIRS = args assert crypt ctype malloc misc mmap rpc pwd_grp setjmp stat signal silly stdlib string termios unistd

DIRS := $(ALL_SUBDIRS)

ifeq ($(HAVE_SHARED),y)
	DIRS += dlopen
endif
ifeq ($(UCLIBC_HAS_THREADS),y)
	DIRS += pthread
endif

ALL_SUBDIRS += pthread dlopen

all: subdirs

tags:
	ctags -R

install: subdirs install_prepare subdirs_install

install_prepare: dummy
	@mkdir -p _install/uclibc
	@mkdir -p _install/glibc
	@cp crypt/crypt.input _install/uclibc/
	@cp crypt/crypt.input _install/glibc/
	@cat uclibc_test.sh test.sh > _install/uclibc/test.sh
	@chmod a+x _install/uclibc/test.sh
	@cat glibc_test.sh test.sh > _install/glibc/test.sh
	@chmod a+x _install/glibc/test.sh

clean: install_clean subdirs_clean

install_clean: dummy
	rm -fr _install

subdirs: $(patsubst %, _dir_%, $(DIRS))
subdirs_clean: $(patsubst %, _dirclean_%, $(DIRS))
subdirs_install: $(patsubst %, _install_%, $(DIRS))

$(patsubst %, _dir_%, $(DIRS)) : dummy
	@echo "Making test in $(patsubst _dir_%,%,$@)"
	$(MAKE) -C $(patsubst _dir_%, %, $@)

$(patsubst %, _dirclean_%, $(ALL_SUBDIRS)) : dummy
	$(MAKE) -C $(patsubst _dirclean_%, %, $@) clean

$(patsubst %, _install_%, $(DIRS)) : dummy
	$(MAKE) -C $(patsubst _install_%, %, $@) install 

.PHONY: dummy
