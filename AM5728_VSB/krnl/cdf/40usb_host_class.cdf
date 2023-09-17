/* 40usb_host_class.cdf - USB host class helper component description file */

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
07may14,j_x  add U/O/XHCI vxBus GEN2 support (US22049 US22048 US25159)
17apr14,s_z  add USB target stack support on VxBus new mode and
             legacy mode (US23307 and US34223), add usb init group
10jan14,s_z  make USB component enable on both new VxBus model and vxBus
             legacy model
07jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB host class helper components.
*/

/* class helper configuration */

Folder  FOLDER_USB_HOST_DEVICES {
    NAME            USB Host Devices
    SYNOPSIS        USB Devices
    _CHILDREN       FOLDER_USB_HOST
    CHILDREN        FOLDER_USB_GEN2_DEVICES    \
                    INCLUDE_USB_HOST_CLASS_INIT
}

Folder  FOLDER_USB_GEN2_DEVICES {
    NAME            USB GEN2 Class drivers
    SYNOPSIS        USB Generation 2 Class Device Drivers
    CHILDREN        FOLDER_USB_GEN2_DEVICES_INIT
}

Folder  FOLDER_USB_GEN2_DEVICES_INIT {
    NAME            USB GEN2 Device Init
    SYNOPSIS        USB Generation 2 Device Component Initialization
    CHILDREN        INCLUDE_USB_GEN2_HELPER
}

Component INCLUDE_USB_HOST_CLASS_INIT {
    NAME            USB Host Class Driver Init
    SYNOPSIS        USB Host Class Driver Initialization
    INIT_RTN        usbHstClassInit();
    REQUIRES        INCLUDE_USBD
    _INIT_ORDER     usrUsbGroupInit 
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       void usbHstClassInit (void);
}

Component INCLUDE_USB_GEN2_HELPER {
    NAME            USB GEN2 Helper Init
    SYNOPSIS        USB GEN2 Helper Task Init (needed to delete device structs)
    REQUIRES        INCLUDE_USB_HOST_CLASS_INIT
    CFG_PARAMS      USB_GEN2_HELPER_LATENCY     \
                    USB_GEN2_HELPER_PRIORITY    \
                    USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE
}

Parameter USB_GEN2_HELPER_LATENCY {
    NAME            USB GEN2 Helper Task Latency (Milliseconds)
    SYNOPSIS        USB GEN2 Helper Task Latency between 2 attempts to delete devices (Milliseconds)
    DEFAULT         1000
}

Parameter USB_GEN2_HELPER_PRIORITY {
    NAME            USB GEN2 Helper Task priority
    SYNOPSIS        USB GEN2 Helper Task priority
    DEFAULT         150
}
Parameter USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE {
    NAME            USB GEN2 Helper queue size
    SYNOPSIS        USB GEN2 Helper queue size (max elems in the queue)
    DEFAULT         100
}

/* debug information */

Folder    FOLDER_USB_TRACK_CLASS_DRIVERS {
    NAME            Class Drivers
    SYNOPSIS        USB Stack Track Class Drivers
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        FOLDER_USB_TRACK_HUB          \
                    FOLDER_USB_TRACK_GEN2
}

Folder    FOLDER_USB_TRACK_HUB {
    NAME            HUB
    SYNOPSIS        USB Stack Track HUB
    _CHILDREN       FOLDER_USB_TRACK_CLASS_DRIVERS
    CHILDREN        INCLUDE_USB_TRACK_HUB
}

Folder    FOLDER_USB_TRACK_GEN2 {
    NAME            GEN2
    SYNOPSIS        USB Stack Track GEN2 Class Drivers
    _CHILDREN       FOLDER_USB_TRACK_CLASS_DRIVERS
    CHILDREN        INCLUDE_USB_TRACK_GEN2_HLP
}

Component INCLUDE_USB_TRACK_HUB {
    NAME            HUB
    SYNOPSIS        USB HUB Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_HUB
}

Parameter USB_TRACK_HUB {
    NAME            USB HUB Debug Level BitMap
    SYNOPSIS        USB HUB Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_HLP {
    NAME            HELPER
    SYNOPSIS        USB GEN2 Helper Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_HELPER
    CFG_PARAMS      USB_TRACK_GEN2_HLP
}

Parameter USB_TRACK_GEN2_HLP {
    NAME            USB GEN2 Helper Debug Level BitMap
    SYNOPSIS        USB GEN2 Helper Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}