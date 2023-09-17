/* 30tilconDemo.cdf - Tilcon Demo Component */

/*
 * Copyright (c) 2013, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
Modification history
--------------------
14mar16,jnl  support drawing dash lines (V7GFX-305)
15oct13,yya  created
*/

Component INCLUDE_TILCON_DEMO
    {
    NAME        Tilcon Library Demo
    SYNOPSIS    Tilcon Library Demo
    _CHILDREN   FOLDER_TILCON
    REQUIRES    INCLUDE_TILCON_KERNEL

    LINK_SYMS   tilconBasic         \
                tilconOscilloscope  \
                tilconMineSweeper   \
                tilconTwd           \
                tilconRadar
    }
