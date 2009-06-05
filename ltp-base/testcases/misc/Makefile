SUBDIRS = crash f00f math tcore_patch_test_suites
UCLINUX_SUBDIRS = math

all:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i ; done

uclinux:
	@set -e; for i in $(UCLINUX_SUBDIRS); do $(MAKE) -C $$i ; done

install:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install ; done

uclinux_install:
	@set -e; for i in $(UCLINUX_SUBDIRS); do $(MAKE) -C $$i install ; done

clean:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean ; done

