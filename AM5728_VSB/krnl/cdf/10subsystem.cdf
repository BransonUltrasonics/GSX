/* 10subsystem.cdf - Component configuration file */

/*
 * Copyright (c) 2013, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------
15arp16,l_z  fix typo for FOLDER_SUBSYSTEM. (V7PRO-3016)
16jul13,l_z  created for VxBus GEN2
*/

Folder FOLDER_SUBSYSTEM {
    NAME       Driver subsystem
    SYNOPSIS   Device subsystem
    _CHILDREN  FOLDER_HARDWARE
}
