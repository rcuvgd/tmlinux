TESTS = clone errno fork getcwd getopt getopt_long preadwrite tstgetopt vfork

include ../Test.mak

CFLAGS_preadwrite_uclibc = -D__UCLIBC_HAS_LFS__

export OPTS_getopt = -abcXXX -9
export OPTS_getopt_long = --add XXX --delete YYY --verbose
export OPTS_tstgetopt = -a -b -cfoobar --required foobar --optional=bazbug --none random --col --color --colour
