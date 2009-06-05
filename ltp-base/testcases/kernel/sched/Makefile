SUBDIRS = cfs-scheduler clisrv hyperthreading nptl process_stress pthreads sched_stress tool 
UCLINUX_SUBDIRS = pthreads nptl

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


