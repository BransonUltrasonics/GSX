/* 40gfxFbConsole.cdf - Frame buffer console CDF file */

/*
 * Copyright (c) 2012, 2014, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
22jan17.jbm  Editing, removed tabs, fixing bad characters.
24jan14,mgc  Modified for VxWorks 7 release
11may12,rfm  Created.
*/

Folder FOLDER_FBDEV_CONSOLE {
    NAME            Frame buffer console components 
    SYNOPSIS        This folder includes components and parameters that allow \
                    you to configure and display a graphical console on your \
                    target hardware display.
    DEFAULTS        INCLUDE_FBDEV_CONSOLE
    _CHILDREN       FOLDER_FBDEV
}

Component INCLUDE_FBDEV_CONSOLE  {
    NAME            Frame buffer console
    SYNOPSIS        This component enables a graphical console.
    HDR_FILES       gfxConsoleInit.h
    CONFIGLETTES    gfxConsoleInit.c
    _CHILDREN       FOLDER_FBDEV_CONSOLE
    REQUIRES        INCLUDE_FBDEV SELECT_FB_CONSOLE_FONT
    CFG_PARAMS      FB_CONSOLE_REDIRECT \
                    FB_CONSOLE_BGCOLOR  \
                    FB_CONSOLE_FGCOLOR  \
                    FB_CONSOLE_TAB      \
                    FB_CONSOLE_WRITE
}

Parameter FB_CONSOLE_REDIRECT {
    NAME            Standard output redirection
    SYNOPSIS        This Boolean value allows you to redirect your keyboard \
                    input to the graphical console.
    TYPE            bool
    DEFAULT         TRUE
}

Parameter FB_CONSOLE_BGCOLOR {
    NAME            Background color
    SYNOPSIS        This parameter is a 24-bit RGB parameter that indicates the \
                    console background color. The default color is dark blue \
                    (0x1d3753).
    TYPE            uint
    DEFAULT         0x1d3753
}

Parameter FB_CONSOLE_FGCOLOR {
    NAME            Foreground color
    SYNOPSIS        This parameter is a 24-bit RGB parameter that indicates the \
                    console foreground (text) color. The default color is white \
                    (0xffffff).
    TYPE            uint
    DEFAULT         0xffffff
}

Parameter FB_CONSOLE_TAB {
    NAME            Tab size
    SYNOPSIS        This parameter modifies the size of tab spaces on the \
                    graphical console. Tab sizes are given as the number of \
                    white space characters. The default tab size is 4.
    TYPE            uint
    DEFAULT         4
}

Parameter FB_CONSOLE_WRITE {
    NAME            Write function
    SYNOPSIS        This parameter specifies a function pointer to handle the \
                    underlying console rendering functionality. By default, \
                    fbdev drivers provide a function that renders text to a \
                    target's display based on the parameters specified in \
                    FOLDER_FBDEV_CONSOLE. You can overwrite the default \
                    console rendering behavior by providing a function \
                    pointer.
    TYPE            funcptr
    DEFAULT         FB_DEFAULT_CONSOLE_WRITE
}

Selection SELECT_FB_CONSOLE_FONT {
    NAME            Font
    SYNOPSIS        This selection provides a choice of fonts that can be used \
                    when rendering the graphical console. The inclusion of any \
                    font requires the presence of INCLUDE_FBDEV_CONSOLE. \
                    CP437 Large is the font by default.
    _CHILDREN       FOLDER_FBDEV_CONSOLE
    COUNT           1-1 
    CHILDREN        INCLUDE_FB_CONSOLE_CP437_L  \
                    INCLUDE_FB_CONSOLE_CP437_S
    DEFAULTS        INCLUDE_FB_CONSOLE_CP437_L
    REQUIRES        INCLUDE_FBDEV_CONSOLE
}

Component INCLUDE_FB_CONSOLE_CP437_L {
    NAME            CP437 large - 8 x 16
    SYNOPSIS        This component provides CP473 large as the font for the \
                    graphical console.
}

Component INCLUDE_FB_CONSOLE_CP437_S {
    NAME            CP437 small - 8 x 8
    SYNOPSIS        This component provides CP473 small as the font for the \
                    graphical console.
}
