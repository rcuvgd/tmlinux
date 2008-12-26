CC= trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc

TARGET_LOCATION=/home/guohr/workdir/tmlinux/export/

SRC  = clone.c  	\
       fork.c    	\
       getopt.c   	\
       hello.c  	\
       preadwrite.c    	\
       setjmp-test.c  	\
       tstgetopt.c      \
       errno.c  	\
       getcwd.c  	\
       getopt_long.c    \
       setjmp-simple.c  \
       setjmptest.c   \
       server.c   \
       client.c   \
       vfork.c

APPS = $(patsubst %.c,%.out,$(SRC)) 

all: $(APPS)

%.out: %.c
	$(CC) $^ -o $@

clean:
	rm -fr *.o *.out init

install:
	cp $(APPS) $(TARGET_LOCATION) -fr 
	chmod a+x $(TARGET_LOCATION)/*.out

