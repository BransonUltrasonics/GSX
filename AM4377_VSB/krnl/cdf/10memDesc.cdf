/* 10memDesc.cdf - memory descriptor stub file */

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
27mar14,cww  added LOCAL_MEM_LOCAL_ADRS and SYS_MEM_DESC_NUM from core and arch
19aug13,x_z  created
*/

#ifdef _WRS_CONFIG_USE_MEMDESC
Component INCLUDE_MEM_DESC {
    NAME            memory descriptor stub
    SYNOPSIS        memory descriptor stub for sysMemDescInit()/sysMemDescGet()
    CONFIGLETTES    usrMemDesc.c
    CFG_PARAMS	    SYS_MEM_DESC_NUM
    _CHILDREN       FOLDER_MEMORY
}

Parameter SYS_MEM_DESC_NUM {
    NAME            Number of System Memory Descriptors
    SYNOPSIS        Maximum number of System Memory Descriptors.
    DEFAULT         8
}
#endif /* _WRS_CONFIG_USE_MEMDESC */

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        local memory address
    SYNOPSIS    The start of the on-board memory area
    DEFAULT     0x0
}

