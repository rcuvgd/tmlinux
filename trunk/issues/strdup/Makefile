#CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CC = arm-wrs-linux-gnueabi-armv5tel_vfp-uclibc_small-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/export/
APPS = init

all: $(APPS)

init: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f init $(ROOTFS_DIR)/sbin/
