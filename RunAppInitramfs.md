# Introduction #

This page describes how to run tmlinux application on initramfs filesystem.

# Details #
To run tmlinux application on initramfs filesystem, you are advised to read PlatformDeveloperGettingStart first, for you need the linux kernel build evironment. To run it, you need to:
  1. put your binary applications into the target root file system.
  1. convert the target root file system to initramfs cpio file.
  1. Build your Linux kernel with this initramfs cpio file embedded.

### 1. Arrange target root file system and create initramfs cpio file ###
Before running tmlinux applications on initramfs, you need to build linux kernel yourself. For how to build it, please refer PlatformDeveloperGettingStart. In this page, let's suppose the linux build directory is **_/home/guohr/worktest/linux_** which is usually the directory of your linux kernel source tree.

Now make a directory as the target root filesystem directory (suppose it is **_/home/guohr/worktest/export_**). The kernel of tmlinux will load its first application, which name is **_init_** and located at the target root directory. So you need to copy the application you want to run to the target root directory and rename it to **_init_**.
> E.g:
```
mkdir export
cp app-test/hello.out ./export/init -fr
```
> Do not forget to assign runnable permission to the binary applications
```
chmod a+x ./export/init
```

> Because regular users can not make device node files and the basic target root filesystem needs them, I suggest that you write a initramfs list file (in this example, it is named **_initramfs\_list_** and located at **_/home/guohr/worktest directory_**) which contains the device node files information:
```
# This is a very simple, default initramfs

dir /dev 0755 0 0
nod /dev/console 0600 0 0 c 5 1
nod /dev/zero 0600 0 0 c 1 5
dir /root 0700 0 0
```

> Now change to **_/home/guohr/worktest/linux_** directory at the work directory. And use `make menuconfig` to change the **CONFIG\_INITRAMFS\_SOURCE** option.
```
cd /home/guohr/worktest/linux
make menuconfig
select "General setup -> Initramfs source file(s)"
set "/home/guohr/worktest/initramfs_list /home/guohr/worktest/export"
save and quite
```
> while `/home/guohr/worktest/initramfs_list` is the full path of that initramfs list file and `/home/guohr/worktest/export` is the target root directory in which contains your applications.

> Tips: you can edit **.config** file located at **_/home/guohr/worktest/linux_** directly. Just set
```
CONFIG_INITRAMFS_SOURCE="/home/guohr/worktest/initramfs_list /home/guohr/worktest/export"
```
> and then save and quite.

### 2. Build Linux kernel image ###

> At **_/home/guohr/worktest/linux_** directory, type `make` and you will get the linux runnable image with target root file system embedded.
```
guohr@ghr-notebook-01:~/worktest/linux$ make
  CHK     include/linux/version.h
  GEN     include/asm-trimedia/asm-offsets.h
  CHK     include/linux/compile.h
  CHK     usr/initramfs_list
  GEN     usr/initramfs_data.h
  GEN     usr/initramfs_len.h
  CC      usr/initramfs_data.o
  LD      usr/built-in.o
  GEN     .version
  CHK     include/linux/compile.h
  UPD     include/linux/compile.h
  CC      init/version.o
  LD      init/built-in.o
  LD      vmlinux
  SYSMAP  System.map
```

> If success, the Linux image is `arch/trimedia/boot/TMLinux-*-fp64-el.out`

### 3. Run this Linux system and see your application running ###

> Congratulations, you now get the runnable Linux image. ON real hardware board, you just need to download it into the board and run it. On simulator, at shell, type `./run.sh` to run it.

> The information below traces the tasks switching track. the `(1)` is the previous task process ID and the `(13)` is the next task process ID. It tells you that Linux kernel switch from `task(1)` to `task(13)`.
```
switch_to: from (1) to (13)
```