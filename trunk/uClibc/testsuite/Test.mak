# Common makefile rules for tests
#
# Copyright (C) 2000,2001 Erik Andersen <andersen@uclibc.org>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Library General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Library General Public License for more
# details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

include ../Rules.mak

U_TARGETS := $(patsubst %,%_uclibc,$(TESTS))
G_TARGETS := $(patsubst %,%_glibc,$(TESTS))
U_TARGETS += $(U_TESTS)
G_TARGETS += $(G_TESTS)

TARGETS    = 
ifeq ($(GLIBC_ONLY),)
TARGETS   += $(U_TARGETS)
endif
ifeq ($(UCLIBC_ONLY),)
TARGETS   += $(G_TARGETS)
endif

all: $(TARGETS)

$(TARGETS): Makefile $(TESTDIR)Rules.mak $(TESTDIR)Test.mak
$(U_TARGETS): $(patsubst %_uclibc,%.c,$(U_TARGETS))
$(G_TARGETS): $(patsubst %_glibc,%.c,$(G_TARGETS))

$(U_TARGETS):
	-@ echo "Compiling $@ vs uClibc: "
	@$(CC) $(CFLAGS) $(CFLAGS_$@) -c $(patsubst %_uclibc,%,$@).c -o $@.o
	@$(CC) $(LDFLAGS) $@.o -o $@ $(EXTRA_LIBS)
	@chmod a+x $@

$(G_TARGETS):
	-@ echo "Compiling $@ vs glibc: "
	@$(HOSTCC) $(GLIBC_CFLAGS) $(GLIBC_CFLAGS_$@) -c $(patsubst %_glibc,%,$@).c -o $@.o
	@$(HOSTCC) $(GLIBC_LDFLAGS) $@.o -o $@ $(EXTRA_LIBS)

clean:
	@$(RM) *.[oa] *~ core $(TARGETS)

install:
	cp $(U_TARGETS) ../_install/uclibc
	cp $(G_TARGETS) ../_install/glibc
