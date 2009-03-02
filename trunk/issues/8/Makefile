CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/export/
APPS = init ifconfig server

all: $(APPS)

init: initapp.c
	$(CC) $(CFLAGS) $^ -o $@

ifconfig: ifconfig.c
	$(CC) $(CFLAGS) $^ -o $@

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f init $(ROOTFS_DIR)/sbin/
	cp -f ifconfig $(ROOTFS_DIR)/sbin/
	cp -f server $(ROOTFS_DIR)/sbin/

