/*
 * Copyright (c) 2007, 2014, 2015, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
04mar16,m_c Fixed names of components in FOLDER_CRYPO
05feb15,chm add dependence INCLUDE_RANDOM_NUM_GEN.(V7NET-510) 
18sep07,spw Remove space
10sep07,mze re-added modules back into INCLUDE_IPCRYPTO
25aug14,ljl Complete openssl layer and cleanup crypto/ssl, US44321
16aug07,mze removed MODULES in INCLUDE_IPCRYPTO
*/


Folder FOLDER_CRYPTO
{
    NAME            Cryptography library
    SYNOPSIS        This folder contains different components and parameters \
                    that can be used to configure the cryptography library for \
                    your project.
    _CHILDREN       FOLDER_OPENSSL
    DEFAULTS        INCLUDE_IPCRYPTO
}

Component INCLUDE_IPCRYPTO 
{
        NAME            Crypto module
        SYNOPSIS        Includes the Wind River Cryptography Libraries in the image. \
                        This is required for any application using cryptography.
        _CHILDREN       FOLDER_CRYPTO
        REQUIRES        INCLUDE_RANDOM_NUM_GEN
        CONFIGLETTES    ipcrypto_config.c
        INIT_RTN        ipcrypto_init();
        CFG_PARAMS      OPENSSL_HOME \
                        OPENSSL_CONF
        MODULES         ipcrypto.o \
                        bn_lib.o \
                        dh_lib.o \
                        dsa_lib.o \
                        evp_lib.o \
                        ocsp_lib.o \
                        pem_lib.o \
                        pk7_lib.o \
                        rsa_lib.o
}

Component INCLUDE_IPCRYPTO_USE_CMDS
        {
        NAME            Commands
        SYNOPSIS        This component enables the OpenSSL command-interpreter \
                        commands and the fips_ctrl shell commands. For a list \
                        of OpenSSL command-interpreter commands, see \
                        ipcrypto_config.c in the \
                        vxworks-7\pkgs\net\ipnet\crypto\ipcrypto\cfg directory. \
                        For descriptions of the commands, see the OpenSSL \
                        documentation. For more information about fips_ctrl shell \
                        commands, see fips_ctrl shell commands.
        _CHILDREN       FOLDER_CRYPTO
        REQUIRES        INCLUDE_IPCRYPTO
        }

Component INCLUDE_IPCRYPTO_USE_TEST_CMDS
        {
        NAME            Test commands
        SYNOPSIS        This component enables command-interpreter commands for \
                        testing the operation of the cryptography libraries. \
                        For a list of the commands see ipcrypto_config.c in \
                        vxworks-7/pkgs/security/openSSL/cfg.
        _CHILDREN       FOLDER_CRYPTO
        REQUIRES        INCLUDE_IPCRYPTO
        }

Parameter OPENSSL_HOME
        {
        NAME            Home directory
        SYNOPSIS        This parameter specifies the home directory for OpenSSL. \
                        By default, it is "/ram/openssl".    
        TYPE            string
        DEFAULT         "/ram/openssl"
        }

Parameter OPENSSL_CONF
        {
        NAME            Configuration file
        SYNOPSIS        This parameter sets the name of the OpenSSL configuration \
                        file. By default, it is OPENSSL_HOME "/openssl.cnf".
        TYPE            string
        DEFAULT         OPENSSL_HOME "/openssl.cnf"
        }
