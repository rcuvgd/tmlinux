

# Introduction #

This page describes how to compile the whole platform of tmlinux (includding linux kernel, uclibc and so on) and how to populate sysroot for application developer.

# Details #

## Preparation ##
### 0. Environment setup ###
In the following section, let's suppose:<br>
<ol><li>the workdir is <i><b>/home/guohr/worktest</b></i>;<br>
</li><li>the TCS installation directory is <i><b>/opt/NXP/TCS520</b></i>;<br>
</li><li>the NDK installation directory is <i><b>/opt/NXP/NDK6.0/</b></i>;</li></ol>

<h3>1. Get and Install TCS Linux edition</h3>
First of all, you should install TCS Linux edition on your Linux machines. Please goto <a href='http://www.tcshelp.com'>http://www.tcshelp.com</a> to get it. Of course, you should have the license for it.<br>
In the following section, let's suppose the installation directory of your TCS is <i><b>/opt/NXP/TCS520/</b></i>

<b>Note that</b><br>
To run tmlinux on mbe1500 board or pnx1500 simulator, you need TCS 5.10 or above; to run tmlinux on pnx1005 simulator, you need TCS 5.20 or above.<br>
<br>
To make sure the TCS tools(such as tmcc, tmld) are in the PATH environment<br>
<pre><code>export PATH=$PATH:/opt/NXP/TCS520/bin/<br>
</code></pre>
<pre><code>guohr@ghr-notebook-02:~/worktest&gt; tmcc -V<br>
tmcc: V7.1.0 of TCS V5.1(0042rc6_Linux)<br>
tmcc warning : no work<br>
</code></pre>

<h3>2. Get and install NDK edition</h3>
Then, you should install NDK on your windows machine. And then pack the whole NDK directory and copy it to your linux machine (work machine of tmlinux). On your linux machine, unpack it to <i><b>/opt/NXP/NDK6.0</b></i>. <br>
If you don't have NDK, please goto <a href='http://www.tcshelp.com'>http://www.tcshelp.com</a> to get it.<br>
<br>
<b>Note that</b><br>
To run tmlinux on mbe1500 board or pnx1500 simulator, you need NDK 5.8 or above; to run tmlinux on pnx1005 simulator, you need NDK 6.0 or above.<br>
<br>
<h3>3. Install CIL (C intermedia language)</h3>
Note that: before continuing, you need to make sure you have installed OCaml language compiler. OCaml can be downloaded from <a href='http://caml.inria.fr/ocaml/'>http://caml.inria.fr/ocaml/</a>. After downloading and unpacking the source distribution, in the ocaml directory, do:<br>
<pre><code>./configure<br>
make world<br>
make opt<br>
make install<br>
</code></pre>

Before building Linux kernel image, you need to setup and install cil in your Linux machine, which is used to process GNU C extension options when compiling Linux kernel.<br>
<br>
Use the following command to check it out and compile, setup it.<br>
<pre><code>svn checkout http://tmlinux.googlecode.com/svn/trunk/cil<br>
cd cil/<br>
./configure<br>
make  <br>
export PATH=$PATH:/home/guohr/worktest/cil/obj/x86_LINUX/<br>
</code></pre>

To validate the environment, type the following command and you will get the following message:<br>
<pre><code>guohr@ghr-notebook-01:~/worktest/cil&gt; cilly.asm.exe<br>
<br>
Error: No arguments for CIL<br>
<br>
Fatal error: exception Errormsg.Error<br>
</code></pre>

<h2>Build Linux Kernel</h2>
<h3>1. checkout Linux kernel of tmlinux and setup environment</h3>
checkout Linux kernel of tmlinux using the following command (at <b><i>/home/guohr/worktest/</i></b>):<br>
<pre><code>git clone git://github.com/camelguo/linux-2.6-trimedia.git linux<br>
</code></pre>

and run setup.sh to setup the compilation enviroment using the following commands:<br>
<pre><code>cd /home/guohr/worktest/linux/<br>
export NDK=/opt/NXP/NDK6.0/<br>
./setup.sh<br>
</code></pre>

the evironment NDK should be set to your NDK directory.<br>
<br>
<h3>2. Configure Linux kernel</h3>
First configure the linux kernel using default configuration files by the following command (at <b><i>/home/guohr/worktest/linux/</i></b> directory).<br>
<br>
<b>To compile kernel for PNX1500 simulator</b>
<pre><code>make sim1500_defconfig<br>
</code></pre>
<b>To compile kernel for PNX1005 simulator</b>
<pre><code>make sim1005_defconfig<br>
</code></pre>
<b>To compile kernel for MBE1500</b>
<pre><code>make mbe1500_defconfig<br>
</code></pre>

Then adjust the configuration if necessary by the following command<br>
<pre><code>make menuconfig<br>
</code></pre>

<h3>3. Build Linux kernel</h3>
To build linux kernel image, use the following command at <b><i>/home/guohr/worktest/linux</i></b>:<br>
<pre><code>make<br>
</code></pre>

After building kernel, the <code>TMLinux-pnx1500-*-el.out</code> at <i>/home/guohr/worktest/linux/arch/trimedia/boot/</i> is the executable Linux kernel image.<br>
<br>
<h2>Build uClibc and generate sysroot</h2>
<h3>1. Checkout sysroots and uClibc</h3>
Checkout sysroots and uClibc using the following commands:<br>
<pre><code>svn checkout http://tmlinux.googlecode.com/svn/trunk/sysroots<br>
svn checkout http://tmlinux.googlecode.com/svn/trunk/uClibc<br>
</code></pre>

<h3>2. Setup TCS for tmlinux build environment</h3>
To create the whole TCS environment for building tmlinux uClibc and applications, you need to goto <b><i>sysroots/sysroots/tcs/5.1.0</i></b> and then use the following command:<br>
<pre><code>cd /home/guohr/worktest/sysroots/tcs/5.1.0<br>
TCS=/opt/NXP/TCS510 ./creat-tcs.sh<br>
</code></pre>
Please set TCS using your TCS installation path (In the above example, it is <b><i>/opt/NXP/TCS510</i></b>).<br>
<br>
<h3>3. Configure, build and install uClibc</h3>
To build uClibc, you need to set the following configuration options correctly using the following command:<br>
<pre><code>make menuconfig<br>
</code></pre>

<h4>TCS include search path</h4>
<pre><code>Target Architecture Features and Options -&gt; tmcc compiler include search path<br>
</code></pre>
In this example, it is <b><i>/home/guohr/worktest/sysroots/tcs/5.1.0</i></b>

<h4>Linux location</h4>
<pre><code>Target Architecture Features and Options -&gt; Linux Kernel header location<br>
</code></pre>
In this example, it is <b><i>/home/guohr/worktest/linux</i></b>

<h4>uClibc installation directories</h4>
<pre><code>Libary Installation Options -&gt; uClibc runtime library directory<br>
</code></pre>
for pnx1500 simulator, it is <b><i>/home/guohr/worktest/sysroots/pnx1500_ttisim_uclibc/sysroot</i></b>; for pnx1005 simulator, it is <b><i>/home/guohr/worktest/sysroots/pnx1005_ttisim_uclibc/sysroot</i></b>; for pnx1500 standalone board, it is <b><i>/home/guohr/worktest/sysroots/pnx1500_nohost_uclibc/sysroot</i></b>;<br>
<pre><code>Libary Installation Options -&gt; uClibc development environment directory<br>
</code></pre>
for pnx1500 simulator, it is <b><i>/home/guohr/worktest/sysroots/pnx1500_ttisim_uclibc/sysroot/usr</i></b>; for pnx1005 simulator, it is <b><i>/home/guohr/worktest/sysroots/pnx1005_ttisim_uclibc/sysroot/usr</i></b>; for pnx1500 standalone board, it is <b><i>/home/guohr/worktest/sysroots/pnx1500_nohost_uclibc/sysroot/usr</i></b>;<br>
<br>
Build and install uClibc using the following commands:<br>
<pre><code>make <br>
make install<br>
</code></pre>


After installing uClibc, it is the time to deliver sysroots to application developers:<br>
For how to build applications, please refer ApplicationDeveloperGettingStart;<br>
For how to build busybox, please refer BusyboxGuide.<br>
<br>
<h2>Generate target root filesystem</h2>
The target root filesystem is formed by a set of basic files (device node files and system configuration files) and lots of optional software package. This section describes how to form the target root filesystem for tmlinux.<br>
<br>
<h3>1. Install the basic files for the filesystem</h3>
In SVN repository, there is a package call <b>filesystem</b>, which contains the basic files for target root filesystem of tmlinux.<br>
<br>
First, check it out from svn (suppose the directory of <b>filesystem</b> is <b><i>/home/guohr/workdir/tmlinux/filesystem</i></b>):<br>
<pre><code>cd /home/guohr/workdir/tmlinux/<br>
svn co http://tmlinux.googlecode.com/svn/trunk/filesystem<br>
</code></pre>
Second, install it to the target root filesystem directory (Let suppose the directory <b><i>/home/guohr/workdir/tmlinux/export</i></b> contains the whole content of the target root filesystem):<br>
<pre><code>mkdir /home/guohr/workdir/tmlinux/export/<br>
cd /home/guohr/workdir/tmlinux/filesystem/<br>
vi Makefile<br>
</code></pre>

And then, set <b><i>TARGET_ROOTFS=/home/guohr/workdir/tmlinux/export/</i></b>. At directory <b><i>/home/guohr/workdir/tmlinux/filesystem/</i></b>
<pre><code>make<br>
</code></pre>


Now the basic files should be installed into the target root filesystem (<b><i>/home/guohr/workdir/tmlinux/export</i></b>)<br>
<br>
<h3>2. Install busybox for the filesystem</h3>
BusyBox combines tiny versions of many common UNIX utilities into a single small executable. It is optional for the target root filesystem, but you are well advised to install it into target root fs.<br>
To Build it, please refer BusyboxGuide. To install it, you need to set<br>
<pre><code>Busybox Settings -&gt; Installation Options -&gt; Busybox installation prefix<br>
</code></pre>
to the target root filesystem directory, in the above example, it is <b><i>/home/guohr/workdir/tmlinux/export</i></b>. And then type <b>make install</b>.<br>
<br>
<h3>3. pack the target root filesystem and deliver it to application developer</h3>
After install your software package for the target root filesystem, it is the time to deliver it to application developers, so that they can boot the whole linux system using it.<br>
<br>
To pack the target root filesystem, please use the following commands:<br>
<pre><code>tar -C /home/guohr/workdir/tmlinux/export -cpf - . | bzip2 -9f &gt; trimedia-uclibc_small-small-dist.tar.bz2<br>
</code></pre>
Above, the <b><i>/home/guohr/workdir/tmlinux/export</i></b> is the target root filesystem directory, the <b><i>trimedia-uclibc_small-small-dist.tar.bz2</i></b> contains the whole target root fs.<br>
<br>
The application developers should unpack it using the following commands:<br>
<pre><code>tar xvfjp trimedia-uclibc_small-small-dist.tar.bz2 -C /home/guohr/export <br>
</code></pre>
Above, the <b><i>/home/guohr/export</i></b> is the directory which contains the target root filesystem content.<br>
<br>
<h1>references</h1>

For more information about booting linux on trimedia, please refer BootingLinuxOnTriMedia.