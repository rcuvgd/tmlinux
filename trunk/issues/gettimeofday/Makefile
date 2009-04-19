CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/workdir/tmlinux/export
APPS = gettimeofday.o

all: $(APPS)

gettimeofday.o: main.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f gettimeofday $(ROOTFS_DIR)/sbin/init
