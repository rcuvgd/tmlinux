#Decribes which devices have been validated on which board.

# Introduction #

This page describes which devices have been validated on which board.


# Details #

## [MDS MBE1500](http://www.mds.com) ##

|  **device name**  |  **vendor ID** | **device ID** | **chip description** | **note**    |
|:------------------|:---------------|:--------------|:---------------------|:------------|
|  [rtl8139](http://www.realtek.com.tw/products/productsView.aspx?Langid=1&PNid=14&PFid=6&Level=5&Conn=4&ProdID=16) |  0x10EC        | 0x8139        |   Realtek RTL8139 Family PCI Fast Ethernet NIC      |             |
|  [MCS9835](http://www.moschip.com/mcs9835.php)      | 0x9710         | 0x9835        |  2 serial, 1 LPT port PCI Card         | see note 1,note 2    |
|  [EN5251B/EN1207F](http://www.accton.com/) | 0x1113         | 0x1216        |   Fast Ethernet Adapter                  |             |

Note 1: LPT port of MCS9835 has not been validated.<br>
Note 2: MCS9835 can not work on interrupt mode but poll mode. <br>

<h2><a href='http://www.fjhtc.com'>HTC EDVR1500</a></h2>
<table><thead><th>  <b>device name</b>  </th><th>  <b>vendor ID</b> </th><th> <b>device ID</b> </th><th> <b>chip description</b> </th><th> <b>note</b>    </th></thead><tbody>
<tr><td>  <a href='http://www.realtek.com.tw/products/productsView.aspx?Langid=1&PNid=14&PFid=6&Level=5&Conn=4&ProdID=16'>rtl8139</a> </td><td>  0x10EC           </td><td> 0x8139           </td><td>   Realtek RTL8139 Family PCI Fast Ethernet NIC      </td><td>                </td></tr>
<tr><td>  AN983B              </td><td>  0x1317           </td><td> 0x0985           </td><td>  ADMTek  PCI Fast Ethernet NIC      </td><td> see note 2     </td></tr>
<tr><td>  VT6105              </td><td>  0x1106           </td><td> 0x3106           </td><td> VIA  PCI Fast Ethernet NIC      </td><td>see note 2      </td></tr>
<tr><td>  DP83815             </td><td>  0x100b           </td><td> 0x0020           </td><td>  National PCI Fast Ethernet NIC      </td><td>see note 2      </td></tr>
<tr><td>  I82559              </td><td>  0x8086           </td><td> 0x1209           </td><td>  Intel PCI Fast Ethernet NIC      </td><td>see note 3      </td></tr>
<tr><td>  <a href='http://www.moschip.com/mcs9835.php'>MCS9835</a>      </td><td> 0x9710            </td><td> 0x9835           </td><td>  2 serial, 1 LPT port PCI Card         </td><td> see note 1     </td></tr>
<tr><td>  <a href='http://www.ite.com.tw'>IT8212F</a>      </td><td> 0x1283            </td><td> 0x8212           </td><td> 2 IDE channels PCI Card         </td><td>                </td></tr>
<tr><td>  CMD649              </td><td> 0x1095            </td><td> 0x0649           </td><td> 2 IDE channels PCI Card         </td><td>                </td></tr>
<tr><td>  siimage 680         </td><td> 0x1095            </td><td> 0x0680           </td><td> 2 IDE channels PCI Card         </td><td>                </td></tr></tbody></table>

Note 1: LPT port of MCS9835 has not been validated.<br>
Note 2: You must set rx_copybreak>1518 to close zero-copy.<br>
Note 3: If you use the eepro100.c driver, must close zero-copy. It's nothing to do if you use the e100.c drier.<br>