/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
04mar16,m_c  Fixed names of components in FOLDER_SSL
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
*/


Folder FOLDER_SSL
{
    NAME            Secure Socket Layer (SSL) library
    SYNOPSIS        This folder contains different components and parameters \
                    used to configure SSL for your project.
    _CHILDREN       FOLDER_OPENSSL
    DEFAULTS        INCLUDE_IPSSL
}

Component INCLUDE_IPSSL
{
    NAME            SSL module
    SYNOPSIS        This component includes Wind River SSL in the image and \
                    loads the ciphers.
    _CHILDREN       FOLDER_SSL
    CONFIGLETTES    ipssl_config.c
    MODULES         ipssl.o \
                    ssl_lib.o
    INIT_RTN        ipssl_init();
    REQUIRES        INCLUDE_IPCRYPTO
}

Component INCLUDE_IPSSL_USE_CMDS
{
    NAME            Commands
    SYNOPSIS        This component enables the command-interpreter commands \
                    ciphers: s_client, s_server, s_time, ssl_clt, and ssl_srv.
    _CHILDREN       FOLDER_SSL
    MODULES         ipssl_cmds.o
    REQUIRES        INCLUDE_IPSSL
}

Component INCLUDE_IPSSL_USE_TEST_CMDS
{
    NAME            Test commands
    SYNOPSIS        This component enables the command-interpreter command, \
                    ssltest.
    _CHILDREN       FOLDER_SSL
    LINK_SYMS       ssltest_main
    REQUIRES        INCLUDE_IPSSL
}
