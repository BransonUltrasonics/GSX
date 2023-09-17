/* 04qspi.cdf - eeprom device component description */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22dec15,y_y  move it from vxbus core layer cdf folder to here. (V7PRO-2620)
07feb12,y_y  written.
*/

Component BRANSON_INCLUDE_QSPI {
    NAME        QSPI device driver
    SYNOPSIS    Attach the QSPI device to standard I/O 
	MODULES     vxbTiQspi.o
    LINK_SYMS   vxbOfAm437xSpiDrv
    REQUIRES    INCLUDE_VXBUS 
    _CHILDREN   FOLDER_PERIPHERALS
}
