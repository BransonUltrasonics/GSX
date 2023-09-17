/* 40vxbeMmc.cdf - EMMC bus Driver */

/*
 * Copyright (c) Branson.
 *
 */

/*
modification history
--------------------
06Apr18,r_r  add eMMC support - core
*/

Component	INCLUDE_EMMC_BUS {
        NAME            EMMC Bus
        SYNOPSIS        EMMC Bus
        MODULES         vxbeMmcLib.o
        REQUIRES        INCLUDE_VXBUS
        _CHILDREN       FOLDER_DRIVERS 
}
