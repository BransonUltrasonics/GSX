/* cli.cdf - cli component description file */

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
13may13,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the cli components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_CLI {
    NAME  CLI
    SYNOPSIS Wind River CLI
    REQUIRES INCLUDE_WEBCLI_BACKPLANE INCLUDE_WEBCLI_COMMON
    _CHILDREN FOLDER_WEBCLI
}

