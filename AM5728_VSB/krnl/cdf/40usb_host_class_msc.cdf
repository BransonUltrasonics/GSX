/* 40usb_host_class_prn.cdf - USB host class storage component description file */

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
05nov14,j_x  extern tUsb2Msc task size to 8192 (V7CON-195)
20oct14,ten  ifdef for _WRS_CONFIG_VXBUS_LEGACY removed (V7PRO-1236)
27feb14,j_x  add init before boot line (VXW7-2156)
24feb14,j_x  remove init order dependency (VXW7-2129)
07jan14,j_x  create
*/

/*
DESCRIPTION
This file contains descriptions for the USB host class storage components.
*/

/* USB host class storage configuration */

Component INCLUDE_USB_GEN2_STORAGE {
    NAME            USB GEN2 Mass Storage
    SYNOPSIS        USB Generation 2 Mass Storage Class Driver
    _CHILDREN       FOLDER_USB_GEN2_DEVICES
    MODULES         usb2Msc.o usb2MscBBB.o usb2MscCBI.o usb2MscBLK.o usb2MscXBD.o \
                    usb2MscCommandSet.o usb2MscDirectAccess.o usb2MscCompliance.o
    REQUIRES        INCLUDE_USB             \
                    INCLUDE_USB_GEN2_HELPER \
                    INCLUDE_ERF             \
                    INCLUDE_XBD             \
                    INCLUDE_FS_MONITOR
    CFG_PARAMS      USB2_MSC_FS_INTERFACE_MODE \
                    USB2_MSC_DIRECT_ACCESS_ENABLE \
                    USB2_MSC_DYNAMIC_CONFIG \
                    USB2_MSC_COMPLIANCE_ENABLE \
                    USB2_MSC_CBI_TRANSPORT_ENABLE \
                    USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL
}

Parameter USB2_MSC_DYNAMIC_CONFIG {
    NAME            USB GEN2 Mass Storage driver dynamic configuration
    SYNOPSIS        USB GEN2 Mass Storage driver dynamic configuration support
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_FS_INTERFACE_MODE {
    NAME            USB GEN2 Mass Storage driver filesystem interface mode
    SYNOPSIS        Filesystem interface mode (USB2_MSC_FS_INTERFACE_XBD or USB2_MSC_FS_INTERFACE_BLK_DEV) to use for the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         USB2_MSC_FS_INTERFACE_XBD
}

Parameter USB2_MSC_DIRECT_ACCESS_ENABLE {
    NAME            USB GEN2 Mass Storage driver direct access enable flag
    SYNOPSIS        Flag to enable or disable direct access module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_COMPLIANCE_ENABLE {
    NAME            USB GEN2 Mass Storage driver compliance test module enable flag
    SYNOPSIS        Flag to enable or disable compliance test module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_CBI_TRANSPORT_ENABLE {
    NAME            USB GEN2 Mass Storage driver CB[I] transport module enable flag
    SYNOPSIS        Flag to enable or disable CB[I] transport module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL {
    NAME            USB GEN2 Mass Storage quirky device global support level
    SYNOPSIS        USB GEN2 Mass Storage quirky device global support level(0:No support, 1:Relaxed support, 2:Support all)
    TYPE            int
    DEFAULT         1
}

Component INCLUDE_USB_GEN2_STORAGE_INIT {
    NAME            USB GEN2 Mass Storage Init
    SYNOPSIS        USB GEN2 Mass Storage Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_DEVICES_INIT
    CONFIGLETTES    usrUsb2MscInit.c
    REQUIRES        INCLUDE_USB_GEN2_STORAGE \
                    INCLUDE_USB_INIT
    PROTOTYPE       STATUS usrUsb2MscInit (void);
    INIT_RTN        usrUsb2MscInit ();
    _INIT_ORDER     usrIosExtraInit
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      USB2_MSC_DRIVE_NAME \
                    USB2_MSC_MAX_NAME_SZ \
                    USB2_MSC_SERVICE_TASK_NAME \
                    USB2_MSC_MAX_DEVICES \
                    USB2_MSC_MAX_DRIVES \
                    USB2_MSC_DIRECT_ACCESS_BASE_NAME \
                    USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME \
                    USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE \
                    USB2_MSC_WRITE_PROTECTION_CHECK_ENABLE \
                    USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL \
                    USB2_MSC_STATUS_CHECK_ENABLE \
                    USB2_MSC_RESET_TIMEOUT \
                    USB2_MSC_COMMAND_TIMEOUT \
                    USB2_MSC_STATUS_TIMEOUT \
                    USB2_MSC_MAX_TRANSFER_BLOCKS \
                    USB2_MSC_SERVICE_TASK_PRIORITY \
                    USB2_MSC_SERVICE_TASK_OPTIONS \
                    USB2_MSC_SERVICE_TASK_STACK_SIZE
}

Parameter USB2_MSC_DRIVE_NAME {
    NAME            USB GEN2 Mass Storage driver drive base name
    SYNOPSIS        Drive base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "/bd"
}

Parameter USB2_MSC_MAX_NAME_SZ {
    NAME            USB GEN2 Mass Storage driver drive base name max length
    SYNOPSIS        Max length of drive base name associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         50
}

Parameter USB2_MSC_SERVICE_TASK_NAME {
    NAME            USB GEN2 Mass Storage driver XBD service task base name
    SYNOPSIS        XBD service task base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "tUsb2Msc"
}

Parameter USB2_MSC_MAX_DEVICES {
    NAME            USB GEN2 Mass Storage driver max devices
    SYNOPSIS        Max number of devices supported by the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         10
}

Parameter USB2_MSC_MAX_DRIVES {
    NAME            USB GEN2 Mass Storage driver max LUNs per device
    SYNOPSIS        Max count of LUNs to be supported by the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         16
}

Parameter USB2_MSC_DIRECT_ACCESS_BASE_NAME {
    NAME            USB GEN2 Mass Storage driver direct access base name
    SYNOPSIS        Direct access base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "/usb2Msc"
}

Parameter USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME {
    NAME            USB GEN2 Mass Storage driver compliance test output file name
    SYNOPSIS        Compliance test log file name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "stdout"
}

Parameter USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver medium hotplug check enable flag
    SYNOPSIS        Flag to enable or disable medium hotplug check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL {
    NAME            USB GEN2 Mass Storage driver medium hotplug check interval (in ms)
    SYNOPSIS        Medium hotplug check interval(in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_RESET_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver reset timeout (in ms)
    SYNOPSIS        Reset timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_COMMAND_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver command timeout (in ms)
    SYNOPSIS        Command timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_STATUS_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver status timeout (in ms)
    SYNOPSIS        Status timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_STATUS_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver status check enable flag
    SYNOPSIS        Flag to enable or disable status check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_WRITE_PROTECTION_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver write protection check enable flag
    SYNOPSIS        Flag to enable or disable write protection check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_MAX_TRANSFER_BLOCKS {
    NAME            USB GEN2 Mass Storage driver maximum blocks per transfer
    SYNOPSIS        Maximum blocks per transfer for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         64
}

Parameter USB2_MSC_SERVICE_TASK_PRIORITY {
    NAME            USB GEN2 Mass Storage driver XBD service task priority
    SYNOPSIS        XBD service task priority associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         150
}

Parameter USB2_MSC_SERVICE_TASK_OPTIONS {
    NAME            USB GEN2 Mass Storage driver XBD service task options
    SYNOPSIS        XBD service task options associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         0
}

Parameter USB2_MSC_SERVICE_TASK_STACK_SIZE {
    NAME            USB GEN2 Mass Storage driver XBD service task stack size
    SYNOPSIS        XBD service task stack size associated to the GEN2 Mass Storage device
    TYPE            int
#ifndef _WRS_CONFIG_LP64
    DEFAULT         4096
#else
    DEFAULT         8192
#endif
}

/* debug information */

Component INCLUDE_USB_TRACK_GEN2_MSC {
    NAME            MASS STORAGE
    SYNOPSIS        USB GEN2 Mass Storage Driver Tracking
    _CHILDREN       FOLDER_USB_TRACK_GEN2
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_STORAGE
    CFG_PARAMS      USB_TRACK_GEN2_MSC
}

Parameter USB_TRACK_GEN2_MSC {
    NAME            USB GEN2 Mass Storage Debug Level BitMap
    SYNOPSIS        USB GEN2 Mass Storage Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}
