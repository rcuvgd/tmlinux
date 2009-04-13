
export PATH=$PATH:./

ENVVAR=123 SOMETHING=sldajfasdf arg_test$LABEL a b c d e f g h
assert$LABEL
crypt$LABEL < crypt.input > crypt.out 2>&1
ctype$LABEL
md5c-test$LABEL
#malloc$LABEL
#mallocbug$LABEL
realloc0$LABEL
testmalloc$LABEL
#time_malloc$LABEL
#sem$LABEL
fdopen$LABEL
dirent$LABEL dirent$LABEL > dirent.out
dirent64$LABEL dirent64$LABEL > dirent64.out
getgroups$LABEL 2>&1 >getgroups.out
grcat$LABEL 2>&1 >grcat.out
pwcat$LABEL 2>&1 >pwcat.out
test_grp$LABEL 2>&1 >test_grp.out
test_pwd$LABEL 2>&1 >test_pwd.out
getrpcent$LABEL > getrpcent.out
setjmp_test$LABEL
sigchld$LABEL
signal$LABEL
hello$LABEL
tiny$LABEL
stat$LABEL stat$LABEL > stat.out
stat64$LABEL stat64$LABEL > stat64.out
teststrtol$LABEL > teststrtol.out
qsort$LABEL > qsort.out
teston_exit$LABEL > teston_exit.out
testatexit$LABEL > testatexit.out
ptytest$LABEL
string$LABEL
testcopy$LABEL > testcopy.out
termios$LABEL
#clone$LABEL
errno$LABEL
fork$LABEL
getcwd$LABEL
getopt$LABEL -abcXXX -9
getopt_long$LABEL --add XXX --delete YYY --verbose
preadwrite$LABEL 
tstgetopt$LABEL -a -b -cfoobar --required foobar --optional=bazbug --none random --col --color --colour
vfork$LABEL
