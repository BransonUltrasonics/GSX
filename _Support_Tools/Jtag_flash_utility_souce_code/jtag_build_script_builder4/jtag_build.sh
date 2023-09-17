#!/bin/bash
#rm *.bin
mv -T ./jtag_flasher_L2_* ./jtag_flasher_L2_$1
cp /media/samba_share/BuildArchive/NextGen/$2/*.bin .

mv AC_vxWorks_$1.bin vxWorks.bin
./bin2header vxWorks.bin
rm vxWorks.bin
rm /home/builder4/Workspace_Jtag/Jtag_tool/flash_writer/vxWorks.h
mv vxWorks.h /home/builder4/Workspace_Jtag/Jtag_tool/flash_writer/vxWorks.h
/home/builder4/ti/ccsv8/eclipse/eclipse -noSplash -data "/home/builder4/Workspace_Jtag" -application com.ti.ccstudio.apps.projectBuild -ccs.projects Jtag_tool -ccs.clean
/home/builder4/ti/ccsv8/eclipse/eclipse -noSplash -data "/home/builder4/Workspace_Jtag" -application com.ti.ccstudio.apps.projectBuild -ccs.projects Jtag_tool -ccs.configuration Debug
rm ./jtag_flasher_L2_$1/jtag_flasher/flasher_AC/flash_tool_ac.out
cp -v /home/builder4/Workspace_Jtag/Jtag_tool/Debug/Jtag_tool.out ./jtag_flasher_L2_$1/jtag_flasher/flasher_AC/flash_tool_ac.out
echo "AC jtag binary generated successfully"

mv PC_vxWorks_$1.bin vxWorks.bin
./bin2header vxWorks.bin
rm vxWorks.bin
rm /home/builder4/Workspace_Jtag/Jtag_tool/flash_writer/vxWorks.h
mv vxWorks.h /home/builder4/Workspace_Jtag/Jtag_tool/flash_writer/vxWorks.h
/home/builder4/ti/ccsv8/eclipse/eclipse -noSplash -data "/home/builder4/Workspace_Jtag" -application com.ti.ccstudio.apps.projectBuild -ccs.projects Jtag_tool -ccs.clean
/home/builder4/ti/ccsv8/eclipse/eclipse -noSplash -data "/home/builder4/Workspace_Jtag" -application com.ti.ccstudio.apps.projectBuild -ccs.projects Jtag_tool -ccs.configuration Debug
rm ./jtag_flasher_L2_$1/jtag_flasher/flasher_AC/flash_tool_pc.out
cp -v /home/builder4/Workspace_Jtag/Jtag_tool/Debug/Jtag_tool.out ./jtag_flasher_L2_$1/jtag_flasher/flasher_PC/flash_tool_pc.out
echo "PC jtag binary generated successfully"

mv SC_vxWorks_$1_20khz.bin vxWorks.bin
./bin2header vxWorks.bin
rm vxWorks.bin
cp /home/builder4/jtag_build/AM5728/driver/qspi_flash.c /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/src/idkAM572x/device/qspi_flash.c
cp /home/builder4/jtag_build/AM5728/driver/qspi_flash.h /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/src/idkAM572x/device/qspi_flash.h
cp /home/builder4/jtag_build/AM5728/driver/QSPI_v1.c /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/drv/spi/src/v1/QSPI_v1.c

rm -rf /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/diag/qspi
cp -rf /home/builder4/jtag_build/AM5728/flash_tool/qspi /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/diag/qspi
mv vxWorks.h /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/diag/qspi/src/vxWorks.h
cd /home/builder4/ti/pdk_am57xx_1_0_10/packages/

./pdksetupenv.sh

make LIMIT_BOARDS="idkAM572x" LIMIT_SOCS="am572x" LIMIT_CORES="a15_0" diag clean
make LIMIT_BOARDS="idkAM572x" LIMIT_SOCS="am572x" LIMIT_CORES="a15_0" diag
cd -
rm ./jtag_flasher_L2_$1/jtag_flasher/flasher_SC/qspi_idkAM572x_armv7.out
cp -v /home/builder4/ti/pdk_am57xx_1_0_10/packages/ti/board/bin/idkAM572x/armv7/qspi_diagExample_idkAM572x_armv7.out ./jtag_flasher_L2_$1/jtag_flasher/flasher_SC/qspi_idkAM572x_armv7.out
echo "SC jtag binary generated successfully"

