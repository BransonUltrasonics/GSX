/* 41usb_controller_xhci.cdf - USB controller xhci component description file */

/*
 * Copyright (c) 2014, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
13feb17,whu  add INCLUDE_CACHE_DMA32_LIB for IA32_PAE
03jun14,j_x  move to FOLDER_USB_CONTROLLER folder (V7CON-140)
11feb14,j_x  move INCLUDE_USBD to different controllers
07jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB controller xhci components.
*/

/* xhci configuration */

Folder    FOLDER_USB_XHCI {
    NAME            USB XHCI Controller
    SYNOPSIS        USB XHCI Controller Components
    _CHILDREN       FOLDER_USB_CONTROLLER
}

Component INCLUDE_USB_XHCI_HCD {
    NAME            USB 3.0 xHCI HCD
    SYNOPSIS        USB 3.0 xHCI HCD for eXtensible Host Controller Interface
    _CHILDREN       FOLDER_USB_XHCI
    MODULES         usbXhcd.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_DMA_SYS  \
                    INCLUDE_USBD
    LINK_SYMS       usbXhcdInit
}

#if defined (_WRS_CONFIG_LP64) || defined (_WRS_CONFIG_IA32_PAE)
Component INCLUDE_USB_XHCI_HCD {
    REQUIRES        += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 || _WRS_CONFIG_IA32_PAE */

Component INCLUDE_USB_XHCI_HCD_INIT {
    NAME            xHCI HCD Init
    SYNOPSIS        Initializes USB 3.0 xHCI HCD Driver
    _CHILDREN       FOLDER_USB_XHCI
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_USB_XHCI_HCD
}

/* debug information */

Component INCLUDE_USB_TRACK_XHCD {
    NAME            xHCI HCD
    SYNOPSIS        USB xHCI HCD Tracking
    _CHILDREN       FOLDER_USB_TRACK_HCD
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_XHCI_HCD
    CFG_PARAMS      USB_TRACK_XHCD
}

Parameter USB_TRACK_XHCD {
    NAME            USB xHCI HCD Debug Level
    SYNOPSIS        USB xHCI HCD Debug Level (0:OFF, 1:ERROR, 2:WARNING, 3:NORMAL, 4:VERBOSE)
    DEFAULT         1
}