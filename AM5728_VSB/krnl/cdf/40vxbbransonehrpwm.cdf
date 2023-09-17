/* 40vxbehrpwm.cdf - Component Definition file for PWMSS */

/*
 * Copyright : To Be Done
 */

Component       BRANSON_DRV_EHRPWM {
    NAME        EHRPWM vxBus driver
    SYNOPSIS    EHRPWM vxBus driver
    MODULES     vxbTiehrpwm.o
    LINK_SYMS   vxbOfAm437xehrpwmDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
