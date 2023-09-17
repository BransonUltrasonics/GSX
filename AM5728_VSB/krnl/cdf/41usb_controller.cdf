/* 41usb_controller.cdf - USB controller component description file */

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
11feb14,j_x  move INCLUDE_USBD to different controllers
16jan14,s_z  code clean up to align 
10jan14,s_z  add INCLUDE_USBD dependence
07jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB controller components.
*/

/* controller configuration */

Folder    FOLDER_USB_OTG_CONTROLLERS {
    NAME            USB OTG Controllers
    SYNOPSIS        USB OTG Controller Components
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Folder    FOLDER_USB_OTG_CONTROLLER_INIT {
    NAME            USB OTG Controller Init
    SYNOPSIS        USB OTG Controller Initialization Components
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Folder FOLDER_USB_GEN2_TARGET_CONTROLLERS {
    NAME            USB GEN2 Target Controllers
    SYNOPSIS        USB GEN2 Target Controllers
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Folder FOLDER_USB_GEN2_TARGET_CONTROLLERS_INIT {
    NAME            USB GEN2 Target Controllers Init
    SYNOPSIS        USB GEN2 Target Controllers Initialization
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_HCD_BUS {
    NAME            USB Host Controller Start
    SYNOPSIS        Register USB Host Controller to vxBus
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_USB
    PROTOTYPE       void vxbUsbHciRegister (void);
    INIT_RTN        vxbUsbHciRegister();
    _INIT_ORDER     hardWareInterFaceBusInit
}

Component INCLUDE_USB_PCI_STUB_CUSTOM {
    NAME            USB PCI Stub Custom
    SYNOPSIS        USB PCI Stub Custom
    _CHILDREN       FOLDER_NOT_VISIBLE
}

/* debug information */

Folder    FOLDER_USB_TRACK_HCD {
    NAME            HCD
    SYNOPSIS        USB Stack Track HCD
    _CHILDREN       FOLDER_USB_TRACK
}