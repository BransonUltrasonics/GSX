/* 40vxbAm5728ehrpwm.cdf - Component Definition file for PWMSS */

/*
 * Copyright : To Be Done
 */

Component       BRANSON_DRV_AM5728_EHRPWM {
    NAME        EHRPWM vxBus driver
    SYNOPSIS    EHRPWM vxBus driver
    MODULES     vxbTiAm5728ehrpwm.o
    LINK_SYMS   vxbOfAm572xehrpwmDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
