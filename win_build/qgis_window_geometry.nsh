############################### reg2nsis begin #################################
# This NSIS-script was generated by the Reg2Nsis utility                       #
# Author  : Artem Zankovich                                                    #
# URL     : http://aarrtteemm.nm.ru                                            #
# Usage   : You can freely inserts this into your setup script as inline text  #
#           or include file with the help of !include directive.               #
#           Please don't remove this header.                                   #
################################################################################

WriteRegBin HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "state" 40004200790074006500410072007200610079002800000000000000ff000000000000000000fc000000000000000200000000000000020000000000000005000000000000001600000046000000690000006c00000065000000540000006f0000006f0000006c00000042000000610000007200010000000000000000000000000000001a00000000000000be00000000000000270000000000000000000000000000000000000000000000000000000000000000000000000000000800000048000000650000006c0000007000000000000000020027000000000000001a00ff00ff00ff00ff00ff00ff00ff00ff000000000000000000000000000000000000000000020027000000000000001a000000000000000e000000500000006c0000007500000067000000690000006e0000007300000000000000020026000000000000001a00ff00ff00ff00ff00ff00ff00ff00ff000000000000000000000000000000000000000000020026000000000000001a00000000000000180000004c00000061000000790000006500000072000000540000006f0000006f0000006c000000420000006100000072000100000000000000be000000000000001a000000000001009000000000000000270000000000000000000000000000000000000000000000000000000000000000000000000000000a00000047000000520000004100000053000000530001000000000002004e000000000000001a00000000000200b20000000000000027000000000000000000000000000000000000000000000000000000000000000000000000000000020000000000000003000000000000001c0000004d0000006100000070000000200000004e000000610000007600000069000000670000006100000074000000690000006f0000006e00010000000000000000000000000000004100000000000100270000000000000027000000000000000000000000000000000000000000000000000000000000000000000000000000140000004100000074000000740000007200000069000000620000007500000074000000650000007300010000000000010027000000000000004100000000000100040000000000000027000000000000000000000000000000000000000000000000000000000000000000000000000000140000004400000069000000670000006900000074000000690000007a000000690000006e000000670001000000000002002b000000000000004100000000000200d50000000000000027000000000000000000000000000000000000000000000000000000000000000000fd0000000000000000000000000005000000000000000200610000000000000004000000000000000400000000000000080000000000000008002900
WriteRegStr HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "maximized" "false"
WriteRegDWORD HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "x" 0xfffffffc
WriteRegDWORD HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "y" 0xfffffffc
WriteRegDWORD HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "w" 0x500
WriteRegDWORD HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "h" 0x2e6
WriteRegBin HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "canvasSplitterState" 40004200790074006500410072007200610079002800000000000000ff00000000000000000000000000000002000000000001000900000000000300e10001000000000000000600010000000000000001002900
WriteRegBin HKEY_CURRENT_USER "Software\QuantumGIS\qgis\Geometry" "legendSplitterState" 40004200790074006500410072007200610079002800000000000000ff0000000000000000000000000000000200000000000100ed000000000000005e0001000000000000000600010000000000000002002900

###############################  reg2nsis end  #################################