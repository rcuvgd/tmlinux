#
#  Copyright (c) Red Hat Inc., 2008
#
#  This program is free software;  you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY;  without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
#  the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program;  if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

# Author: Masatake YAMATO <yamato@redhat.com>
# Technique used here is suggested by Garrett Cooper <yanegomi@gmail.com>

# Usage:
# 
# This makefile snippet is for writing test cases
# for foo16 system calls. Here I assume you already have 
# test cases for foo like foo01, foo02.. fooN and I also 
# assume the source file name for fooN is fooN.c.
# On the above assumption, this file does:
# 
# * adding fooN_16 as TARGETS, 
# * making *.c depend on compat_16.h if the header file exists,
# * adding rules to build fooN_16 from fooN.c (and compat_16.h), and
# * passing a cpp symbol TST_USE_COMPAT16_SYSCALL to 
#   CC when building fooN_16.
#
#
# You can use this file in following procedures:
# 
# 1. write fooN.c.
# 2. add the code for 16 bit syscall and wrap
#    it #ifdef TST_USE_COMPAT16_SYSCALL/endif.
# 3. introduce your own compat_16.h if the ifdef 
#    block is too large.
# 4. don't forget putting compat_16.h in all fooN.c
#    if you introduced compat_16.h.
# 5. include this file compat_16.mk in your Makefile.
# 6. use `+=' instead of `=' as assignment operator for TARGETS.
# 7. Added extra definitions to CFLAGS in %_16 target if needed.
#
# See Makefile of setuid test case.
#
TARGETS_16 = $(patsubst %.c,%_16,$(SRCS))
ifneq ($(TST_COMPAT_16_SYSCALL),no)
TARGETS  +=  $(TARGETS_16)
endif

DEF_16 = TST_USE_COMPAT16_SYSCALL
COMPAT_16_H = compat_16.h
HAS_COMPAT_16 := $(shell if [ -f $(COMPAT_16_H) ]; then	\
	  			echo yes;		\
			else				\
				echo no;		\
			fi)


ifeq ($(HAS_COMPAT_16),yes)
%.c: $(COMPAT_16_H)
endif

%_16.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
%_16: CFLAGS += -D$(DEF_16)=1
