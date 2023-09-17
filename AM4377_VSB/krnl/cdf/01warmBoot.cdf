/* 01warmBoot.cdf - warm boot library component description file */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
08apr15,l_z  created. (F3387)
*/

#if (!defined SIMLINUX) && (!defined  SIMNT)
Component INCLUDE_WARM_BOOT {
    NAME            initialize warm boot library
    SYNOPSIS        initialize warm boot library
    CONFIGLETTES    usrWarmBootLib.c
    MODULES         warmBootLib.o
    HDR_FILES       warmBootLib.h
    PROTOTYPE       void usrWarmBootLibInit ();
    INIT_RTN        usrWarmBootLibInit();
    _CHILDREN       FOLDER_BOARD_LIB
    _INIT_ORDER     usrRoot
    INIT_AFTER      INCLUDE_BOARD_INIT
    INIT_BEFORE     INCLUDE_VXBUS
}

Component INCLUDE_WARM_BOOT_PROTECT {
    NAME            protect the warm boot data
    SYNOPSIS        protect the warm boot data
    REQUIRES        INCLUDE_WARM_BOOT \
    				INCLUDE_MMU_BASIC
    _CHILDREN       FOLDER_BOARD_LIB
}
#endif


