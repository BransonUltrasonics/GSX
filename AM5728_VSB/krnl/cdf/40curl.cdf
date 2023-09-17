/* curl.cdf - curl component description file */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
29sep16,r_w  update to 7.50.3.0 (V7MAN-197)
27jun16,r_w  add LINK_SYMS (V7MAN-200)
13may13,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the curl components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_CURL {
    NAME  cURL
    SYNOPSIS cURL library
    MODULES   easy.o
    REQUIRES INCLUDE_SOCKLIB INCLUDE_IPSSL
    LINK_SYMS curl_easy_init curl_easy_setopt
    _CHILDREN FOLDER_WEBCLI
}

