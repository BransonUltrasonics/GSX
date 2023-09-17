/* webcli.cdf - webcli component description file */

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
This file contains descriptions for the webcli components.
*/

Folder FOLDER_WEBCLI {
    NAME       WebCLI
    SYNOPSIS   CLI,Web and MIBWay product group
    _CHILDREN  FOLDER_APPLICATION
    CHILDREN   INCLUDE_WEBCLI_BACKPLANE \
               INCLUDE_WEBCLI_COMMON \
               INCLUDE_WEBCLI_CLI \
               INCLUDE_WEBCLI_CLIDEMO \
               INCLUDE_WEBCLI_CURL \
               INCLUDE_WEBCLI_MIBWAY \
               INCLUDE_WEBCLI_HTTP \
               INCLUDE_WEBCLI_WEBDEMO \
               INCLUDE_WEBCLI_WEBCLIDEMO
}

