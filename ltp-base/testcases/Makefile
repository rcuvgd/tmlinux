	SUBDIRS = `ls */Makefile | sed "s/Makefile//g" | grep -v open | grep -v pounder | grep -v DOTS | grep -v kdump | grep -v realtime | grep -v audit-test` 
UCLINUX_SUBDIRS = misc kernel
all:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i ; done

install:
	mkdir -p bin
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install ; done

uclinux:
	@set -e; for i in $(UCLINUX_SUBDIRS); do $(MAKE) -C $$i uclinux; done

uclinux_install:
	mkdir -p bin
	@set -e; for i in $(UCLINUX_SUBDIRS); \
		do $(MAKE) -C $$i uclinux_install ; done

clean:
	rm -rf bin/
	@set -e; for i in $(SUBDIRS) ; do $(MAKE) -C $$i clean ; done

