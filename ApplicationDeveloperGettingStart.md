

# Introduction #

> This page describes how to compile your applications and run them on tmlinux. For more details on application development, please refer ApplicationDevelopmentNote.


# Details #


## Preparation: ##
> ### 1. Get and install TCS Linux edition. ###

> First of all, you should install TCS Linux edition on your Linux machines. Please goto http://www.tcshelp.com to get it. Of course, you should have the license for it.
> In the following section, let's suppose the installation directory of your TCS is _**/opt/NXP/TCS510/**_

> ### 2. Checkout sysroots from this project's svn. ###
> Now make your work directory (suppose it is _**/home/guohr/worktest/**_), at this directory, use the following command to get the sysroots:
```
cd /home/guohr/worktest/
svn checkout http://tmlinux.googlecode.com/svn/trunk/sysroots/
```
> ### 3. Setup application building environment. ###
> Make sure the TCS tools(such as tmcc, tmld) in the PATH environment and the compiler for your application building in the PATH environment.

**For PNX1500 Simulator:**
```
export PATH=$PATH:/opt/NXP/TCS510/bin/
export PATH=$PATH:/home/guohr/worktest/sysroots/pnx1500_ttisim_uclibc/x86-linux2/
```

**For PNX1500 standalone board (e.g: MBE1500 board):**
```
export PATH=$PATH:/opt/NXP/TCS510/bin/
export PATH=$PATH:/home/guohr/worktest/sysroots/pnx1500_nohost_uclibc/x86-linux2/
```

**For PNX1005 Simulator**:
```
export PATH=$PATH:/opt/NXP/TCS510/bin/
export PATH=$PATH:/home/guohr/worktest/sysroots/pnx1005_ttisim_uclibc/x86-linux2/
```

**For PNX1005 standalone board**:
```
export PATH=$PATH:/opt/NXP/TCS510/bin/
export PATH=$PATH:/home/guohr/worktest/sysroots/pnx1005_nohost_uclibc/x86-linux2/
```

> To validate the environment, at shell, use the following command, the result will like this:
```
guohr@ghr-notebook-02:~/worktest> tmcc -V
tmcc: V7.1.0 of TCS V5.1(0042rc6_Linux)
tmcc warning : no work
```

**For PNX1500:**
```
guohr@ghr-notebook-02:~/worktest>trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc -V
tmcc: V7.1.0 of TCS V5.1(0042rc6_Linux)
tmcc warning : no work
```

**For PNX1005:**
```
guohr@ghr-notebook-02:~/worktest>trimedia-gss-linux-tcs-pnx1005el_fp64-uclibc-gcc -V
tmcc: V7.1.0 of TCS V5.1(0042rc6_Linux)
tmcc warning : no work
```

> ### 4. Create TCS environment for building your applications. ###
> To create the whole TCS environment for building tmlinux applications, you need to goto **_sysroots/sysroots/pnx1500\_ttisim\_uclibc/tcs/5.1.0_** (for building pnx1500 application) or **_sysroots/sysroots/pnx1005\_ttisim\_uclibc/tcs/5.1.0_** (for building pnx1005 application) and then use the following command:
```
cd /home/guohr/worktest/sysroots/tcs/5.1.0
TCS=/opt/NXP/TCS510 ./creat-tcs.sh
```

> Please set TCS with your TCS installation path (In the above example, it is **_/opt/NXP/TCS510_**).

## Build your application: ##
> ### 1. Note about tmlinux applications ###
> Now, you can compile your exist application. BUT if you run tmlinux system on **simulator**, then before compiling them, please note that:
  * ON SIMULATOR INPUT/OUTPUT FUNCTIONS CANNOT WORK
> Because of lacking real output devices on simulator, this linux kernel directs stdout/stderr to /dev/null, which means that these output functions (such as `printf()`, `puts()`) will output nothing. Because of lacking real input devices, this linux kernel will wait forever when application calls input functions (such as `gets()`, `getchar()`)

  * ON SIMULATOR WITHOUT `PRINTF()`, HOW CAN I SEE APPLICATION RUNNING
> As I said before, tmlinux's `printf()` just output information to /dev/null. To see application running, I use **TCS**'s `printf()` instead of tmlinux's `printf()`. So, in the following code,
```
#include <stdio.h>

int
main(void)
{
   printf("Hello world!\n");
   return 0;
}
```
> the **printf()** here is provided by TCS but not tmlinux. So it still can output information when using ttisim.
> But **PLEASE DO NOTE THAT:** high frequent `printf()` may cause the TCS system suspend, so try to avoid this situation.

> ### 2. Build your applications ###
> Ok, you can compile your applications. It is as simple as compiling the simplest 'hello world' application, Just replace your compiler with **trimedia-gss-linux-tcs-pnx1500el\_fp64-uclibc-gcc** (for building pnx1500 applications) or **trimedia-gss-linux-tcs-pnx1005el\_fp64-uclibc-gcc** (for building pnx1005 applications)
**For building pnx1500 applications:**
```
trimedia-gss-linux-tcs-pnx1500el_fp64-uclibc-gcc hello.c -o hello.out
```
**For building pnx1005 applications:**
```
trimedia-gss-linux-tcs-pnx1005el_fp64-uclibc-gcc hello.c -o hello.out
```

> There are some test cases for tmlinux application in svn. Please use the following command to check them out and just type make to compile them.
```
/*suppose you are in /home/guohr/worktest directory*/
svn checkout http://tmlinux.googlecode.com/svn/trunk/app-test/
cd app-test
make
```

> If successful, you will get some .out binary applications in **_app-test_** directory.

> Please see TmlinuxAppExample for examples.

## Run your applications ##
> Now you can put your binary applications into the target root file system and run it. For trimedia simulator, tmlinux only supports initramfs file system, please refer RunAppInitramfs to get to know how to run it. For trimedia standalone hardware board (e.g: MBE1500), tmlinux supports lots of file systems, e.g: ext2 (based on harddisk), NFS. But you need to configure linux kernel properly before running application.