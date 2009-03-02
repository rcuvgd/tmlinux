CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CFLAGS =
ROOTFS_DIR = /home/guohr/export/
APPS = init

all: $(APPS)

init: main.o foo.o
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(CLFAGS) -c $^ -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f init $(ROOTFS_DIR)/sbin/
