@echo off
REM echo "Converting the MLO to header"
REM python "bin2header_modified.py" MLO MLO.h
REM echo "Converting the uboot bin to header" 
REM python "bin2header_modified.py" u-boot.img u-boot.h
REM echo "Converting the vxWorks bin to header" 
REM python "bin2header_modified.py" vxWorks.bin vxWorks.h
REM Copy "MLO.h" "C:\Users\builder5\Downloads\flash_tool_no_console\flash_tool\flash_writer\MLO.h"
REM Copy "u-boot.h" "C:\Users\builder5\Downloads\flash_tool_no_console\flash_tool\flash_writer\u-boot.h"
REM Copy "vxWorks.h" "C:\Users\builder5\Downloads\flash_tool_no_console\flash_tool\flash_writer\vxWorks.h"
REM echo "reBuilding flash utility tool.." 
REM C:\ti\ccsv7\eclipse\eclipsec.exe -noSplash -data "C:\Sasken\Dhanya\Flash_Trial_CCS1" -application com.ti.ccstudio.apps.projectBuild -ccs.projects flash_tool -ccs.configuration Debug
echo "Launching debug server script for flash"
C:\ti\ccsv7\ccs_base\scripting\bin\dss.bat LoadProgram-baremetal.js & TIMEOUT /T 1740 /NOBREAK & echo "Flashed Successfully"
pause