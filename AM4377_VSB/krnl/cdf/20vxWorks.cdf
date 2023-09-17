/* 20vxWorks.cdf - VxWorks components */

/*
 * Copyright (c) 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
14mar12,cwl  add component INCLUDE_HW_FP_SHOW. (WIND00274266)
27sep10,jdw  created

DESCRIPTION

This file contains updated descriptions for some vxWorks components
defined in 00vxWorks.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

Component INCLUDE_COPROCESSOR_SHOW {
        NAME            coprocessor show routine
        SYNOPSIS        task coprocessor registers
        MODULES         coprocShow.o
        INIT_RTN        coprocShowInit ();
        HDR_FILES       private/coprocLibP.h
        REQUIRES        INCLUDE_COPROCESSOR
        _CHILDREN       FOLDER_SHOW_ROUTINES
        _INIT_ORDER     usrShowInit
}

Component INCLUDE_HW_FP_SHOW {
        NAME            floating point show routine
        SYNOPSIS        task floating point registers
        MODULES         vfpArchLibShow.o
        INIT_RTN
        HDR_FILES
        REQUIRES        INCLUDE_COPROCESSOR_SHOW
}

Parameter MAX_REBOOT_HOOKS {
        NAME            Maximum number of reboot hooks
        TYPE            int
        DEFAULT         8
}

Parameter ENV_VAR_USE_HOOKS {
        NAME            install environment variable task create/delete hooks
        TYPE            bool
        DEFAULT         TRUE
}
