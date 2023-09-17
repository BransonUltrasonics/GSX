/* mibway.cdf - component description file */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,13may13,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_MIBWAY {
    NAME  MIBWay
    SYNOPSIS MIBWay
    REQUIRES INCLUDE_WEBCLI_COMMON INCLUDE_WINDMANAGE_SNMP_CORE   
    _CHILDREN FOLDER_WEBCLI
}

