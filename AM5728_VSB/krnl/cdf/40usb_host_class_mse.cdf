/* 40usb_host_class_mse.cdf - USB host class mouse component description file */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03sep14,wyy  Init Keyboard and Mouse driver before network driver (V7CON-179)
07jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB host class mouse components.
*/

/* USB host class mouse configuration */

Component INCLUDE_USB_GEN2_MOUSE {
    NAME            USB GEN2 Mouse
    SYNOPSIS        USB Generation 2 Mouse Class Driver
    #ifdef          _WRS_CONFIG_EVDEV_LIB
    _CHILDREN       FOLDER_EVDEV_DRV
    MODULES         usb2Mse.o
    #else
    _CHILDREN       FOLDER_USB_GEN2_DEVICES
    MODULES         usb2Mse_raw.o
    #endif
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_GEN2_HELPER
}

#ifdef    _WRS_CONFIG_EVDEV_LIB
Component INCLUDE_USB_GEN2_MOUSE {
    REQUIRES        += INCLUDE_EVDEV_LIB_PTR    \
                       INCLUDE_USB_GEN2_MOUSE_INIT
}
#endif 

Component INCLUDE_USB_GEN2_MOUSE_INIT {
    NAME            USB GEN2 Mouse Init
    SYNOPSIS        USB GEN2 Mouse Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_DEVICES_INIT
    REQUIRES        INCLUDE_USB_GEN2_MOUSE \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    PROTOTYPE       STATUS usrUsb2MseInit (void);
    INIT_RTN        usrUsb2MseInit ();
    _INIT_ORDER     usrIosExtraInit
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      USB_GEN2_MOUSE_NAME
}

Parameter USB_GEN2_MOUSE_NAME {
    NAME            USB GEN2 Mouse driver base name
    SYNOPSIS        Device base name associated to the GEN2 mouse device
    DEFAULT         "/usb2Mse/"
}

/* debug information */

Component INCLUDE_USB_TRACK_GEN2_MSE {
    NAME            MOUSE
    SYNOPSIS        USB GEN2 MOUSE Driver Tracking
    _CHILDREN       FOLDER_USB_TRACK_GEN2
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_MOUSE
    CFG_PARAMS      USB_TRACK_GEN2_MSE
}

Parameter USB_TRACK_GEN2_MSE {
    NAME            USB GEN2 Mouse Debug Level BitMap
    SYNOPSIS        USB GEN2 Mouse Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}