#CC = trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc
CC = tmcc
AR = tmar
CFLAGS = -host ttisim -target pnx1500
ROOTFS_DIR = /home/guohr/workdir/tmlinux/export
APPS = libpthread.a libmystub.a math 

all: $(APPS)

math: main.c 
	$(CC) $(CFLAGS) $^ -o $@ -L. -lpthread -lmystub 

libpthread.a: libpthread.o
	$(AR) r $@ $^

libmystub.a: libmystub.o
	$(AR) r $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -fr $(APPS) *.o

install:
	chmod a+x $(APPS)
	cp -f math $(ROOTFS_DIR)/
