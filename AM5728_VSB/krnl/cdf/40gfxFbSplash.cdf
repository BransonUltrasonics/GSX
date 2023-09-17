/* 40gfxFbSplash.cdf - Frame buffer splash CDF file */

/*
 * Copyright (c) 2014, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
22jan17,jbm  Fixing bad characters, editing
24jan14,mgc  Modified for VxWorks 7 release
*/

Component INCLUDE_FBDEV_SPLASH {
    NAME            Frame buffer splash screen
    SYNOPSIS        This component renders a splash screen onto the \
                    display following the boot up of the target hardware. 
    HDR_FILES       gfxSplashInit.h
    CONFIGLETTES    gfxSplashInit.c
    ARCHIVE         libgfxSplash.a
    _CHILDREN       FOLDER_FBDEV
    REQUIRES        INCLUDE_FBDEV
    CFG_PARAMS      FB_SPLASH_WR \
                    FB_SPLASH_BLIT
}

Parameter FB_SPLASH_WR {
    NAME            Wind River splash screen
    SYNOPSIS        This Boolean value, when set to TRUE, configures the fbdev \
                    driver to render the splash screen specified in FB_SPLASH_BLIT. \
                    If the value is set to FALSE, the display renders a solid color.
    TYPE            bool
    DEFAULT         TRUE
}

Parameter FB_SPLASH_BLIT {
    NAME            Blit function
    SYNOPSIS        This parameter allows you to provide a function pointer \
                    to overwrite the default splash screen behavior. Use this \
                    parameter to render your own splash screen.
    TYPE            funcptr
    DEFAULT         FB_DEFAULT_SPLASH_BLIT
}

