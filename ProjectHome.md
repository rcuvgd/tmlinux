**tmlinux** is a port of linux to NXP's TriMedia Processors. It now well supports PNX1500 and PNX1005, and it will support TM1300 and PNX1700. Now it can run on trimedia simulator, MBE1500 board (A LCP1500 in standalone mode) and HTC EDVR board. For more details, please refer DevicesValidation. <br>
<b>tmlinux</b> includes a standard linux kernel (based on linux-2.6.14 and applied with some celinux patches), a uclibc and a busybox. All of them are well tested using uclibc testsuite and LTP testsuite. You can get uclibc, busybox and toolchain from this project's SVN repository, and you can get linux kernel source tree from github.com using the following command:<br>
<pre><code>git clone git://github.com/camelguo/linux-2.6-trimedia.git<br>
</code></pre>

<a href='http://www.pledgie.com/campaigns/4503'><img src='http://www.pledgie.com/campaigns/4503.png?skin_name=chrome' alt='Click here to lend your support to: linux-2.6-trimedia and make a donation at www.pledgie.com !' border='0' /></a>

<h1>Need your help</h1>
<b>tmlinux</b> now passes LTP testsuite on PNX1005 simulator. And in theory, it should work OK on PNX1005 real hardware board. But due to the lack of hardware, we can not validate it. So tmlinux project group is looking forward help on PNX1005 hardware board. Who can help me?<br>
<br>
<br>
<h1>News</h1>
<ul><li>tmlinux passed LTP project on PNX1005 simulator.<br>
</li><li>tmlinux passed LTP project on MBE1500 board. And now it is available for application developers to develop their applications on it.<br>
</li><li>We are trying to port tmlinux on  MBE-1500 board. It is sponsored by NXP.<br>
</li><li>We have released an SDK for application developers based on tmlinux of PNX1005. Application developers can build and run tmlinux pnx1005 applications on ttisim using this SDK. We will release the complete source code after complete testing.<br>
</li><li>We are trying to port tmlinux onto pnx1005 (strictly speaking only tm3282 -- the core to PNX1005) and have done most development jobs. We are trying our best to release it as soon as possible.</li></ul>

<h1>Feature</h1>
<ul><li>Complete compatibility with standard linux interface.<br>
</li><li>Support dynamically loading applications.</li></ul>

<h1>BSP Matrix</h1>
<table><thead><th>           </th><th> <b>TM1300</b> </th><th> <b>PNX1500</b> </th><th> <b>PNX1700</b> </th><th> <b>PNX1005</b> </th></thead><tbody>
<tr><td> <b>simulator</b> </td><td> No            </td><td> Yes            </td><td> No             </td><td> Yes            </td></tr>
<tr><td> <b><a href='http://www.mds.com'>MDS MBE</a></b>   </td><td> No            </td><td> Yes            </td><td> No             </td><td> No             </td></tr>
<tr><td> <b><a href='http://www.fjhtc.com'>HTC EDVR</a></b>  </td><td> No            </td><td> Yes            </td><td> No             </td><td> No             </td></tr></tbody></table>

Please refer DevicesValidation for more details. If you want to support your board, please contact <a href='mailto:camelguo@gmail.com'>camelguo@gmail.com</a>.<br>
<br>
<h1>Getting Start</h1>
For Application Development, please go to ApplicationDeveloperGettingStart.<br>
<br>
For Platform Development, please go to PlatformDeveloperGettingStart.<br>
<br>
<h1>Test and validation</h1>

For how to test and validate tmlinux using LTP, please goto TestWithLtp.<br>
To get to know which devices we have validated on trimedia board, please goto DevicesValidation.<br>

<h1>Attend this project</h1>
<ul><li>Need help on this project? please attend <a href='http://groups.google.com/group/tmlinux'>TriMedia on Linux Discuss Group</a>.<br>
</li><li>Want to become a project member? please make sure you have a valid google.com account and then write email (English or Chinese) to <a href='mailto:camelguo@gmail.com'>camelguo@gmail.com</a>.<br>
</li><li>For new members, please check your permission as in MembershipChecklist.</li></ul>

<h1>sponsor</h1>
<ul><li><a href='http://www.gulessoft.com'>Gulessoft</a>
<blockquote>Gulessoft is a professional service provider of embedded Linux, which can also provide professional service for this project.</blockquote></li></ul>

