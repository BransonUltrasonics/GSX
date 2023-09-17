/* 40usb_host_class_kbd.cdf - USB host class keyboard component description file */

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
This file contains descriptions for the USB host class keyborard components.
*/

/* USB host class keyboard configuration */

Component INCLUDE_USB_GEN2_KEYBOARD {
    NAME            USB GEN2 Keyboard
    SYNOPSIS        USB Generation 2 Keyboard Class Driver
    #ifdef          _WRS_CONFIG_EVDEV_LIB
    _CHILDREN       FOLDER_EVDEV_DRV
    MODULES         usb2Kbd.o
    #else
    _CHILDREN       FOLDER_USB_GEN2_DEVICES
    MODULES         usb2Kbd_raw.o
    #endif
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_GEN2_HELPER
}

#ifdef    _WRS_CONFIG_EVDEV_LIB
Component INCLUDE_USB_GEN2_KEYBOARD {
    REQUIRES        += INCLUDE_EVDEV_LIB_KBD        \
                       INCLUDE_EVDEV_LIB_KBD_MAP_US \
                       INCLUDE_USB_GEN2_KEYBOARD_INIT
}
#endif 

Folder  FOLDER_USB_GEN2_KEYBOARD_INIT {
    NAME            USB GEN2 Keyboard Initialization
    SYNOPSIS        USB GEN2 Keyboard Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_DEVICES_INIT
    CHILDREN        INCLUDE_USB_GEN2_KEYBOARD_INIT    
}

Component INCLUDE_USB_GEN2_KEYBOARD_INIT {
    NAME            USB GEN2 Keyboard Init
    SYNOPSIS        USB GEN2 Keyboard Driver Initialization
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    PROTOTYPE       STATUS usrUsb2KbdInit (void);
    INIT_RTN        usrUsb2KbdInit ();
    _INIT_ORDER     usrIosExtraInit
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      USB_GEN2_KEYBOARD_NAME USB_GEN2_KEYBOARD_TYPEMATIC_MS
}

Parameter USB_GEN2_KEYBOARD_NAME {
    NAME            USB GEN2 keyboard driver base name
    SYNOPSIS        Device base name associated to the GEN2 keyboard device
    DEFAULT         "/usb2Kbd/"
}

Parameter USB_GEN2_KEYBOARD_TYPEMATIC_MS {
    NAME            USB GEN2 keyboard Typematic Rate in ms
    SYNOPSIS        USB GEN2 keyboard Typematic Rate in ms\
                    Defines how often the current pressed key will be sent    \
                    to the application (ASCII MODE). -1 is FOREVER, any other \
                    integer is the time in ms
    DEFAULT         128
}

#ifdef    _WRS_CONFIG_EVDEV_LIB
#else
Component INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH {
    NAME            USB GEN2 keyboard attaching to vxWorks Shell
    _CHILDREN       FOLDER_USB_GEN2_KEYBOARD_INIT
    SYNOPSIS        Attaches the USB keyboard to the vxWorks target shell
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD_INIT   \
                    INCLUDE_SHELL
    EXCLUDES        INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH
}

Component INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH {
    NAME            USB GEN2 keyboard attaching to bootAppShell (only for bootrom)
    _CHILDREN       FOLDER_USB_GEN2_KEYBOARD_INIT
    SYNOPSIS        Attaches the USB keyboard to the bootAppShell
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD_INIT \
                    INCLUDE_PC_CONSOLE
    EXCLUDES        INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH
}
#endif

/* debug information */

Component INCLUDE_USB_TRACK_GEN2_KBD {
    NAME            KEYBOARD
    SYNOPSIS        USB GEN2 Keyboard Driver Tracking
    _CHILDREN       FOLDER_USB_TRACK_GEN2
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD
    CFG_PARAMS      USB_TRACK_GEN2_KBD
}

Parameter USB_TRACK_GEN2_KBD {
    NAME            USB GEN2 Keyboard Debug Level BitMap
    SYNOPSIS        USB GEN2 Keyboard Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}