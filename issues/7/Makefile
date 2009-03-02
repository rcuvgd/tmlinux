CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/export/

all: init

init: clone.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -fr init *.o

install:
	chmod a+x init
	cp -f init $(ROOTFS_DIR)/sbin/
