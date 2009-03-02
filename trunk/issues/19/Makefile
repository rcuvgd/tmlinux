CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
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
