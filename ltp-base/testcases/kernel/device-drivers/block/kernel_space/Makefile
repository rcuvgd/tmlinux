
EXTRA_CFLAGS += -Wall -Wextra -Wno-unused-parameter

ifneq ($(KERNELRELEASE),)

obj-m	:= test_block.o test_genhd.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f modules.order

help:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) help

modules_install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install

endif
