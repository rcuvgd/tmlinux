
PREFIX=/opt/ltp

CFLAGS+= 
CFLAGS+= -D_USC_LIB_
CPPFLAGS+= -I../include 
ifeq ($(shell uname -s),HP-UX)
CFLAGS+=-Ae -D_LARGEFILE64_SOURCE +DA1.1
endif
CFLAGS+=$(if $(UCLINUX),-DUCLINUX)
LDFLAGS+=
TARGET=libltp.a
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(AR) -cr $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

install: $(TARGET)
	install -D -m 644 $(TARGET) $(DESTDIR)/$(PREFIX)/lib$(LIBSUFFIX)/$(TARGET)
	install -D -m 644 ltp.pc $(DESTDIR)/$(PREFIX)/share/pkgconfig/ltp.pc

