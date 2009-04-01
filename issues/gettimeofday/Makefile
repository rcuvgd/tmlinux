CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/workdir/tmlinux-ttisim/export
APPS = gettimeofday

all: $(APPS)

gettimeofday: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f gettimeofday $(ROOTFS_DIR)/gettimeofday
