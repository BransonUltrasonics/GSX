/* 40evdevLibKbd.cdf - Keyboard Library Component Bundles */

/*
 * Copyright (c) 2013, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
22jan17,jbm  Fixed spelling error and alignment issues.
12nov13,j_x  abstract kbd attach to shell to evdev kbd lib
19jul13,j_x  create
*/

Folder FOLDER_EVDEV_KBD
    {
    NAME            Keyboard library components
    SYNOPSIS        This folder contains components and parameters related to \
                    the keyboard library.
    _CHILDREN       FOLDER_EVDEV_LIB
    }

Parameter EVDEV_KBD_TYPE_MATIC_PERIOD
    {
    NAME            Typematic period
    SYNOPSIS        This parameter defines the period for checking a key long \
                    press in milliseconds.

    TYPE            uint
    DEFAULT         200
    }

Component   INCLUDE_EVDEV_LIB_KBD
    {
    NAME            Keyboard library
    SYNOPSIS        This component is required when using keyboard devices. It \
                    includes the event devices keyboard library.
    MODULES         evdevLibKbd.o
    CONFIGLETTES    evdevLibKbdCfg.c
    _CHILDREN       FOLDER_EVDEV_KBD
    PROTOTYPE       STATUS evdevKbdInit (UINT32 typeMaticRate);
    _INIT_ORDER     usrIosExtraInit
    INIT_AFTER      INCLUDE_EVDEV_LIB_CORE
    INIT_RTN        evdevKbdInit (EVDEV_KBD_TYPE_MATIC_PERIOD);
    CFG_PARAMS      EVDEV_KBD_TYPE_MATIC_PERIOD
    REQUIRES        INCLUDE_EVDEV_LIB_CORE
    }
    
Component INCLUDE_EVDEV_LIB_KBD_SHELL_ATTACH
    {
    NAME            Attach to VxWorks shell
    SYNOPSIS        This component attaches the evdev keyboard to the VxWorks \
                    target shell.
    _CHILDREN       FOLDER_EVDEV_KBD
    REQUIRES        INCLUDE_EVDEV_LIB_KBD
    }
