/* backplane.cdf - backplane component description file */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27may14,r_w  add dependent on MIBway for V7MAN-45
13may13,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the backplane components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_BACKPLANE {
    NAME  Backplane
    SYNOPSIS Data backplane for WebCLI
    REQUIRES INCLUDE_WEBCLI_COMMON INCLUDE_WEBCLI_MIBWAY
    _CHILDREN FOLDER_WEBCLI
}

