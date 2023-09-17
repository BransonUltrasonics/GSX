ECHO OFF
title This is your first batch script!
:Ask
echo Enter BRANSON_DRV_LAYER Path (IE: C:\Users\Me\MyWorkspaces\_Branson_Driver_Layer)
set INPUT=
set /P INPUT=Type location: %=%

echo Enter WindRiver Path to vxWorks-7 (IE: C:\WindRiver\vxWorks-7)
set INPUTWR=
set /P INPUTWR=Type Location: %=%

%SystemRoot%\system32\setx WIND_LAYER_PATHS %INPUT% /m
SET PATH = %WIND_LAYER_PATHS%

::::::::::::::::::::::::::::::::::::::::::::SETTING PATH FOR BSPs:::::::::::::::::::::::::::::::::::::::::::::::::::::::

echo Settings Bsp path variables for 4377 and 5728...! 
set SRC_BSP=%INPUT%\BRANSON_BSP\BSP_4377;%INPUT%\BRANSON_BSP\BSP_5728;%INPUT%\BRANSON_BSP\BSP_IDK;%INPUT%\BRANSON_BSP\BSP_LBC;
:: Set Path variable
%SystemRoot%\system32\setx WIND_BSP_PATHS %SRC_BSP% /m
:: Set Path variable
SET PATH = %WIND_BSP_PATHS%

move %INPUTWR%\pkgs\os\drv\vxbus-1.0.5.0\core-1.0.8.1\h\hwif\vxBus.h %INPUTWR%\..\vxBus.original.h
copy vxBus.h %INPUTWR%\pkgs\os\drv\vxbus-1.0.5.0\core-1.0.8.1\h\hwif\vxBus.h

echo Done!
echo Please Reboot your workstation now.
PAUSE 
exit
