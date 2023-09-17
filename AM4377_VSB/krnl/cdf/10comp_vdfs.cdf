/* 10comp_vdfs.cdf - Virtual Disk File System component */

/*
 * Copyright 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17feb15,dcc  re-write
*/

/*
DESCRIPTION
This file contains the description for virtual disk file system component.
*/

Folder FOLDER_DEVFS
    {
    NAME            Virtual Disk file system
    SYNOPSIS        Virtual Disk file system interface
    CHILDREN        INCLUDE_VDFS
    }

Component INCLUDE_VDFS
    {
    NAME            Virtual Disk filesystem
    SYNOPSIS        Virtual Disk file system interface
    MODULES         vdFsLib.o
    INIT_RTN        vdFsInit (NUM_VDFS_FILES);
    CFG_PARAMS      NUM_VDFS_FILES
    HDR_FILES       vdFsLib.h
    REQUIRES        INCLUDE_FS_MONITOR INCLUDE_FS_EVENT_UTIL
    }

Parameter NUM_VDFS_FILES
    {
    NAME            max # open VDFS files
    TYPE            uint
    DEFAULT         20
    }

