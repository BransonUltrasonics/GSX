/* 40usb_host.cdf - usb host core component description file */

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
03jun14,j_x  move to FOLDER_NOT_VISIBLE folder (V7CON-140)
05mar14,ten  Added hdr file usbd.h to INCLUDE_USB_ASYN_MSG
11feb14,j_x  move INCLUDE_USB_ASYN_MSG component from USB CORE to USBD
16jan14,j_x  change INCLUDE_USBD
10jan14,s_z  move INCLUDE_USB to 40usb.cdf
06jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB host core components.
*/

/* host core components */

Folder    FOLDER_USB_HOST {
    NAME            USB Host
    SYNOPSIS        USB Host Components
    _CHILDREN       FOLDER_USB
}

Folder    FOLDER_USB_HOST_INIT {
    NAME            USB Host Controller Init
    SYNOPSIS        USB Host Component Initialization
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component    INCLUDE_USBD {
    NAME            USBD Module
    SYNOPSIS        USBD Module used by USB host
    _CHILDREN       FOLDER_NOT_VISIBLE
    MODULES         usbd.o
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_ASYN_MSG
}

Component    INCLUDE_USB_ASYN_MSG {
    NAME            USB Message Asyn Lib Init
    SYNOPSIS        USB Message Asynchronous Library Initialization
    REQUIRES        INCLUDE_ERF
    INIT_RTN        usbMsgAsynLibInit();
    _INIT_ORDER     usrRoot
    _CHILDREN       FOLDER_NOT_VISIBLE
    INIT_AFTER      usrScInit
   HDR_FILES	    usbd.h
}

/* debug infromation */

Folder    FOLDER_USB_TRACK_USBD {
    NAME            USBD
    SYNOPSIS        USB Driver
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_USBD
}

Component    INCLUDE_USB_TRACK_USBD {
    NAME            USBD
    SYNOPSIS        USB Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_USBD
}

Parameter    USB_TRACK_USBD {
    NAME            USB Driver Debug Level BitMap
    SYNOPSIS        USB Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}
