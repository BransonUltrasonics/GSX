/* 40vxbSdMmc.cdf - SDMMC bus Driver */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05nov14,j_x  add connectivity folder (V7CON-198)
07may14,e_d  add vxbus Gen2 model support. (US29286)
*/

/* connectivity folder */

Folder FOLDER_CONNECTIVITY {
    NAME       connectivity components
    SYNOPSIS   connectivity components
    _CHILDREN  FOLDER_ROOT
}

Folder    FOLDER_SDMMC {
    NAME            SDMMC
    SYNOPSIS        SDMMC Bus (SDMMC)
    _CHILDREN       FOLDER_CONNECTIVITY
}
#ifdef _WRS_CONFIG_VXBUS
Component       INCLUDE_SD_BUS {
        NAME            SD Bus
        SYNOPSIS        SD Bus
        _CHILDREN       FOLDER_SDMMC 
        MODULES         vxbSdMmcLib.o
        REQUIRES        INCLUDE_VXBUS
}
#else
Component       INCLUDE_SD_BUS {
        NAME            SD Bus
        SYNOPSIS        SD Bus
        CONFIGLETTES    hwif/vxbSd.bc
        MODULES         vxbSdMmcLib.o
        _CHILDREN       FOLDER_SDMMC 
        _INIT_ORDER hardWareInterFaceBusInit
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_PARAM_SYS
        INIT_RTN        sdRegister();
}
#endif
