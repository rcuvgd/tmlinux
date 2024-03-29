# See toplevel INSTALL for build info

CROSS_COMPILE = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-
CROSS_CFLAGS = -D__UCLIBC__ -DUCLINUX  -Dlinux -lpthread

ifdef CROSS_COMPILE
CROSS_COMPILER = $(CROSS_COMPILE)
endif
ifdef CROSS_COMPILER
CC=$(CROSS_COMPILER)gcc
AR=$(CROSS_COMPILER)ar
RANLIB=$(CROSS_COMPILER)ranlib
endif

HAS_NUMA=$(shell sh tools/scripts/numa_test.sh)

export CFLAGS += $(CROSS_CFLAGS)
export CC AR RANLIB CPPFLAGS LDFLAGS HAS_NUMA

-include config.mk

VPATH += include m4
all: uclinux

linux_all: config.h config.mk uclinux_libltp.a 
	@$(MAKE) -C pan $@
	@$(MAKE) -C testcases $@
	@$(MAKE) -C tools $@
	@echo
	@echo "***********************************************"
	@echo "** You now need to do a make install as root **"
	@echo "***********************************************"

install: all
	@$(MAKE) -C testcases install
	@$(MAKE) -C tools install
	@$(MAKE) -C lib install
	@$(MAKE) -C include install
	@$(MAKE) -C pan install
	@$(MAKE) -C m4 install
	@$(MAKE) -C doc/man1 install
	@$(MAKE) -C doc/man3 install

	@./IDcheck.sh

libltp.a: config.h
	@$(MAKE) -C lib $@

uclinux: uclinux_libltp.a
	@$(MAKE) -C pan all
	@$(MAKE) -C testcases uclinux
	@$(MAKE) -C tools all
	@echo
	@echo "*******************************************************"
	@echo "** You now need to do a make uclinux_install as root **"
	@echo "*******************************************************"

uclinux_install: uclinux
	@$(MAKE) -C testcases uclinux_install
	@$(MAKE) -C tools install
	@./IDcheck.sh

uclinux_libltp.a: config.h
	@$(MAKE) -C lib UCLINUX=1 libltp.a

menuconfig:
	@./ltpmenu

clean: ac-clean
	@$(MAKE) -C lib $@
	@$(MAKE) -C pan $@
	@$(MAKE) -C tools $@
	@$(MAKE) -C testcases $@

distclean: clean ac-distclean
	@$(MAKE) -C include $@

maintainer-clean: distclean ac-maintainer-clean
	@$(MAKE) -C include $@

package: 
	rpmbuild -ba ltp-devel.spec


#
# Autotools related
#
.PHONY: autotools
autotools: aclocal autoconf autoheader automake

.PHONY: aclocal
aclocal: aclocal.m4
aclocal.m4: $(wildcard m4/*.m4)
	aclocal -I m4

.PHONY: autoconf
autoconf: configure
configure: configure.ac aclocal.m4
	autoconf

.PHONY: autoheader
autoheader: config.h.in
config.h.in: configure.ac $(wildcard m4/*.m4)
	autoheader
	touch include/$@
config.h: config.h.default
	cp include/config.h.default include/config.h
config.mk:
	touch $@

.PHONY: automake
AUTOMAKE_FILES = config.guess config.sub install-sh missing
automake: aclocal $(AUTOMAKE_FILES)
$(AUTOMAKE_FILES): m4/Makefile.in
m4/Makefile.in: m4/Makefile.am
	automake -c -a

.PHONY: ac-clean ac-distclean ac-maintainer-clean
ac-clean:
	rm -rf autom4te.cache
	rm -f config.log config.status
ac-distclean: ac-clean
ac-maintainer-clean: ac-distclean
	rm -f aclocal.m4 configure $(AUTOMAKE_FILES) m4/Makefile.in

#
# Help
#
.PHONY: help
help:
	@echo
	@echo 'About configuration'
	@echo '-------------------'
	@echo 'If you want to use auto configuration,   '
	@echo 'be sure autoconf and automake are installed. Then run: '
	@echo '	$$ make autotools '
	@echo '	$$ ./configure   '
	@echo '	$$ make all      '
	@echo
	@echo 'If you want to use default configuration,  '
	@echo 'autoconf is not needed. Just run:          '
	@echo '	$$ touch include/config.h.default '
	@echo '	$$ make config.h                  '
	@echo '	$$ make all                       '
	@echo
	@echo 'If make all is failed even if you use the '
	@echo 'auto configuration, please, report it to  '
	@echo 'ltp developers with config.log, generated '
	@echo 'by running the configure script.          '
	@echo
