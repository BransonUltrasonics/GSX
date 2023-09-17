/* 40vxbbransonAm572xqspi.cdf - Component Definition file for QSPI Controller*/

/*
 * Copyright To be Done
 */
 
Component       BRANSON_DRV_AM572x_QSPI {
    NAME        AM572x QSPI Controller Driver
    SYNOPSIS    vxBus Driver for AM572x QSPI Controller
    MODULES     vxbTiAm572xQspi.o
    LINK_SYMS   vxbOfAm572xSpiDrv
    REQUIRES    INCLUDE_VXBUS 
    _CHILDREN   FOLDER_DRIVERS
}
