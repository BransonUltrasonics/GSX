/* 40vxbEqep.cdf - Component Definition file for PWMSS */

/*
 * Copyright : To Be Done
 */

Component       BRANSON_DRV_EQEP {
    NAME        EQEP vxBus driver
    SYNOPSIS    EQEP vxBus driver
    MODULES     vxbTiEqep.o
    LINK_SYMS   vxbOfAm437xeqepDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
