/* common.cdf - common component description file */

/*
 * Copyright (c) 2013-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
13feb15,f_f  add the dependence on pipe
13may13,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the common components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_COMMON {
    NAME  Common
    SYNOPSIS Common utilities for WebCLI
    REQUIRES INCLUDE_SOCKLIB INCLUDE_PIPES
    _CHILDREN FOLDER_WEBCLI
}
