# Introduction #

This page describes how to test tmlinux using LTP. The Linux™ Test Project (LTP) is a joint project started by SGI™ and maintained by IBM®, that has a goal to deliver test suites to the open source community that validate the reliability, robustness, and stability of Linux. The LTP testsuite contains a collection of tools for testing the Linux kernel and related features. For more information, please goto http://ltp.sourceforge.net


# Details #

## 1. checkout LTP ##
checkout LTP using the following command:
```
svn checkout http://tmlinux.googlecode.com/svn/trunk/ltp-base
```

Let's suppose you checkout ltp-base at **_/home/guohr/workdir/tmlinux/ltp-base_**

## 2. Set compiler and PATH environment ##
Edit **_Makefile_** at **_/home/guohr/workdir/tmlinux/ltp-base/_** and set CROSS\_COMPILE as the compiler prefix you are using.

For PNX1500 nohost, you should set CROSS\_COMPIL as **_trimedia-gss-linux-tcs-pnx1500el\_fp64-uclibc-_**

And then set PATH environment variable, so that shell can find the compiler you are using.
For PNX1500 nohost, you should set PATH using the following command:
```
export PATH=$PATH:/home/guohr/workdir/tmlinux/sysroots/pnx1500_nohost_uclibc/x86-linux2/
```

## 3. Build and Install LTP ##
Now build and install ltp using the following commands:
```
cd /home/guohr/workdir/tmlinux/ltp-base
make uclinux
sudo make uclinux_install
```

## 4. Install LTP to target root filesystem ##
let's suppose tmlinux target root filesystem in your machine is **_/home/guohr/workdir/
tmlinux/export_**, you should install LTP using the following commands:

```
cd /home/guohr/workdir/tmlinux/ltp-base/
```

make ltp directory structures
```
mkdir -p /home/guohr/workdir/tmlinux/export/opt/ltp-base/testcases/
mkdir -p /home/guohr/workdir/tmlinux/export/opt/ltp-base/runtest
mkdir -p /home/guohr/workdir/tmlinux/export/opt/ltp-base/pan
```

install testcases of LTP to target root filesystem.
```
sudo chmod a+x testcases/bin/*
sudo cp testcases/bin/ /home/guohr/workdir/tmlinux/export/opt/ltp-base/testcases/ -rf
```

copy test cases file of LTP to target root filesystem
```
cp runtest/syscalls* /home/guohr/workdir/tmlinux/export/opt/ltp-base/runtest/
```

copy pan to target root filesystem
```
chmod a+x pan/pan
cp pan/pan /home/guohr/workdir/tmlinux/export/opt/ltp-base/pan/
```

copy test script to target root filesystem
```
cp runltpmsh.sh /home/guohr/workdir/tmlinux/export/opt/ltp-base/
```

## 5. execute LTP ##
Now boot up trimedia machine.

Before run LTP testcases, you need to tell **runltpmsh.sh** which testcase set you want to execute. To do that, you need to set **CMDFILE** in **runltpmsh.sh**. For example, you want to run testcases in syscalls01, you should set **CMDFILE="syscalls01"**

To execute ltp, at your trimedia machine's console
```
cd /opt/ltp-base/
./runltpmsh.sh
```

the raw testresult will print on the screen,  and test result file will be located at /**tmp/ltp-$(pid)/** directory.