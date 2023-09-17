/* 00cplus.cdf - cplus components */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history            
--------------------
19nov14,gvm    INCLUDE_CPLUS now pulls in INCLUDE_CPLUS_LANG (TCGNU-3747)
01oct13,mcm    created based on content from the common 00vxWorks.cdf 
               in version 6.9
*/

/*
DESCRIPTION
Components that are part of kernel cplus 
*/

Folder  FOLDER_CPLUS {
        NAME            C++ components
        SYNOPSIS        C++ class libraries
        CHILDREN        INCLUDE_CTORS_DTORS     \
                        INCLUDE_CPLUS           \
                        INCLUDE_CPLUS_LANG      \
                        FOLDER_CPLUS_STDLIB
        DEFAULTS        INCLUDE_CPLUS           \
                        INCLUDE_CPLUS_LANG
}

Component INCLUDE_CPLUS {
        NAME            C++ core runtime
        SYNOPSIS        Basic support for C++ applications
        INIT_RTN        usrCplusLibInit ();
        HDR_FILES       private/cplusLibP.h
        MODULES         cplusCore.o \
                        cplusInit.o \
                        cplusLib.o \
                        cxa_atexit.o
        CONFIGLETTES    usrCplus.c
        REQUIRES        INCLUDE_CTORS_DTORS INCLUDE_CPLUS_LANG
}

Component INCLUDE_CPLUS_LANG {
        NAME            C++ compiler support routines
        SYNOPSIS        Support for exception handling, new, delete etc.
}

Component INCLUDE_CTORS_DTORS {
        NAME            run static initializers
        SYNOPSIS        run compiler generated initialization functions at system startup
        INIT_RTN        cplusCtorsLink ();
        HDR_FILES       cplusLib.h
        MODULES         cplusXtors.o
}

Folder FOLDER_CPLUS_STDLIB {
        NAME            standard library
        SYNOPSIS        C++ Standard Library
        CHILDREN        INCLUDE_CPLUS_IOSTREAMS
        DEFAULTS        INCLUDE_CPLUS_IOSTREAMS
}

Component INCLUDE_CPLUS_DEMANGLER {
        NAME            C++ symbol demangler
        SYNOPSIS        Support library for kernel shell and loader: provides human readable forms of C++ identifiers
        INIT_RTN        cplusDemanglerInit ();
        MODULES         cplusDem.o
        INCLUDE_WHEN    INCLUDE_CPLUS INCLUDE_SYM_TBL
}

