/* 40vxbAm5728Epwm.cdf - Component Definition file for PWMSS */

/*
 * Copyright :To Be Done
 */

Component       BRANSON_DRV_AM5728EPWM {
    NAME        PWM vxBus driver
    SYNOPSIS    PWM vxBus driver
    MODULES     vxbAm5728TiEpwm.o
    LINK_SYMS   vxbOfAm572xEpwmDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
