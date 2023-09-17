/* 30tilconKernel.cdf - Tilcon CDF file */

/*
 * Copyright (c) 2013-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
28may14,yaa  add explicit dependency to INCLUDE_POSIX_TIMERS (V7GFX-168)
15may14,yya  rename API_FBDEV to INCLUDE_FBDEV
26may13,yya  create
*/

Component INCLUDE_TILCON_KERNEL
    {
    NAME            Tilcon Library Kernel
    SYNOPSIS        Tilcon Library Kernel
    _CHILDREN       FOLDER_TILCON
    ARCHIVE         libtilcon.a
    REQUIRES        INCLUDE_PNG                     \
                    INCLUDE_JPEG                    \
                    INCLUDE_FREETYPE2               \
                    INCLUDE_EVDEV_LIB_CORE          \
                    INCLUDE_EVDEV_LIB_KBD           \
                    INCLUDE_EVDEV_LIB_PTR           \
                    INCLUDE_SEM_READ_WRITE_CREATE   \
                    INCLUDE_POSIX_TIMERS            \
                    INCLUDE_FBDEV
    }
