/* 40vxbbransonAm437xqspi.cdf - Component Definition file for QSPI Controller*/

/*
 * Copyright To be Done
 */
 
Component       BRANSON_DRV_AM437x_QSPI {
    NAME        AM437x QSPI Controller Driver
    SYNOPSIS    vxBus Driver for AM437x QSPI Controller
    MODULES     vxbTiAm437xQspi.o
    LINK_SYMS   vxbOfAm437xSpiDrv
    REQUIRES    INCLUDE_VXBUS 
    _CHILDREN   FOLDER_DRIVERS
}
