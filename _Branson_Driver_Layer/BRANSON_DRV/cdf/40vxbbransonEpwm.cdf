/* 40vxbEpwm.cdf - Component Definition file for PWMSS */

/*
 * Copyright :To Be Done
 */

Component       BRANSON_DRV_EPWM {
    NAME        PWM vxBus driver
    SYNOPSIS    PWM vxBus driver
    MODULES     vxbTiEpwm.o
    LINK_SYMS   vxbOfAm437xEpwmDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
