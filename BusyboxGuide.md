# Introduction #

This page describes how to build busybox for tmlinux. To completely understand this how to, please read ApplicationDeveloperGettingStart first.

# Details #

## 1. Checkout busybox ##
Checkout busybox using the following command:
```
svn checkout http://tmlinux.googlecode.com/svn/trunk/busybox
```
## 2. Configure busybox ##
Configure busybox using the following command:
```
make menuconfig
```

In most cases, you have to set the following configuration options:
```
Busybox Settings -> Installation Options -> Busybox installation prefix
```
This option determines where to install the busybox binaries. It should be set to the target root filesystem directory.

## 3. Build and install busybox ##
To build and install busybox, use the following commands:
```
make 
make install
```

Now you can find the busybox binaries at the target rootfs directory setted by Busybox installation prefix.