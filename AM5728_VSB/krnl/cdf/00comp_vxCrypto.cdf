/* 00comp_vxCrypto.cdf - vxCrypto components configuration file */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
04mar16,m_c  Fixed name of FOLDER_SEC_CIPHER and related components
07dec15,rbo  Removed components related to test
10dec14,rbo  Created
*/

Folder  FOLDER_SEC_CIPHER {
        NAME            Cipher algorithms
        SYNOPSIS        VxWorks has a Cipher API. This folder holds the \
                        components that configure what algorithms to include.
        _CHILDREN       FOLDER_SECURITY
        CHILDREN        INCLUDE_SEC_CIPHER_AES_128_CBC \
                        INCLUDE_SEC_CIPHER_AES_128_ECB \
                        INCLUDE_SEC_CIPHER_AES_128_CTR \
                        INCLUDE_SEC_CIPHER_AES_192_CBC \
                        INCLUDE_SEC_CIPHER_AES_192_CTR \
                        INCLUDE_SEC_CIPHER_AES_256_CBC \
                        INCLUDE_SEC_CIPHER_AES_256_CTR \
                        INCLUDE_SEC_CIPHER_AES_256_ECB \
                        INCLUDE_SEC_CIPHER_BLOWFISH_CBC \
                        INCLUDE_SEC_CIPHER_CAST_CBC \
                        INCLUDE_SEC_CIPHER_DES_CBC \
                        INCLUDE_SEC_CIPHER_3DES_CBC \
                        INCLUDE_SEC_CIPHER_RC4 \
                        FOLDER_SEC_CIPHER_PROVIDERS
}

Folder  FOLDER_SEC_CIPHER_PROVIDERS {
        NAME            Providers
        SYNOPSIS        VxWorks has a Cipher API. This folder holds the components \
                        that configure what providers of included algorithms to \
                        include.
        CHILDREN        SELECT_SEC_CIPHER_AES_128_CBC_IMPL \
                        SELECT_SEC_CIPHER_AES_128_ECB_IMPL \
                        SELECT_SEC_CIPHER_AES_128_CTR_IMPL \
                        SELECT_SEC_CIPHER_AES_192_CBC_IMPL \
                        SELECT_SEC_CIPHER_AES_192_CTR_IMPL \
                        SELECT_SEC_CIPHER_AES_256_CBC_IMPL \
                        SELECT_SEC_CIPHER_AES_256_CTR_IMPL \
                        SELECT_SEC_CIPHER_AES_256_ECB_IMPL \
                        SELECT_SEC_CIPHER_BLOWFISH_CBC_IMPL \
                        SELECT_SEC_CIPHER_CAST_CBC_IMPL \
                        SELECT_SEC_CIPHER_DES_CBC_IMPL \
                        SELECT_SEC_CIPHER_3DES_CBC_IMPL \
                        SELECT_SEC_CIPHER_RC4_IMPL
}


InitGroup usrSecCipherInit {
        SYNOPSIS        Initialize the secCipher providers.
        INIT_RTN        usrSecCipherInit ();
        INIT_ORDER      INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL \
                        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL \
                        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_RC4_OPENSSL \
                        INCLUDE_SEC_CIPHER_RC4_OPENSSL_EVP 
        _INIT_ORDER     usrRoot
        INIT_BEFORE     usrKernelExtraInit usrNetworkInit0
}


Component INCLUDE_SEC_CIPHER_AES_128_CBC {
        NAME            AES-128 CBC
        SYNOPSIS        Includes the secCipher AES 128 CBC API and installs \
                        a configurable provider for this algorithm.
        MODULES         secCipherAes128Cbc.o
        REQUIRES        SELECT_SEC_CIPHER_AES_128_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 128 CBC provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAes.o
        PROTOTYPE       void secCipherOpensslAes128CbcOpensslInit(void);
        INIT_RTN        secCipherOpensslAes128CbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 128 CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 128 CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes128CbcEvpInit(void);
        INIT_RTN        secCipherOpensslAes128CbcEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_128_CBC_IMPL {
        NAME            AES-128 CBC
        SYNOPSIS        Selects whether the AES-128 CBC provider for the secCipher \
                        API is implemented based on the low-level OpenSSL APIs or \
                        the OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_AES_128_ECB {
        NAME            AES-128 ECB
        SYNOPSIS        This component includes the secCipher AES 128 ECB API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes128Ecb.o
        REQUIRES        SELECT_SEC_CIPHER_AES_128_ECB_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 128 ECB provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAesEcb.o
        PROTOTYPE       void secCipherOpensslAes128EcbInit(void);
        INIT_RTN        secCipherOpensslAes128EcbInit();
}

Component INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 128 ECB provider for the \
                        secCipher API. The implementation is based on the OpenSSL \
                        EVP API. This AES 128 ECB provider is required when \
                        OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes128EcbEvpInit(void);
        INIT_RTN        secCipherOpensslAes128EcbEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_128_ECB_IMPL {
        NAME            AES-128 ECB
        SYNOPSIS        Selects whether the AES-128 ECB provider for the secCipher API \
                        is implemented based on the low-level OpenSSL APIs or the \
                        OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_ECB_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_AES_128_CTR {
        NAME            AES-128 CTR
        SYNOPSIS        This component includes the secCipher AES 128 CTR API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes128Ctr.o
        REQUIRES        SELECT_SEC_CIPHER_AES_128_CTR_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 128 CTR provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAesCtr.o
        PROTOTYPE       void secCipherOpensslAes128CtrInit(void);
        INIT_RTN        secCipherOpensslAes128CtrInit();
}

Component INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 128 CTR provider for the \
                        secCipher API. The implementation is based on the OpenSSL \
                        EVP API. This AES 128 CTR provider is required when \
                        OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes128CtrEvpInit(void);
        INIT_RTN        secCipherOpensslAes128CtrEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_128_CTR_IMPL {
        NAME            AES-128 CTR
        SYNOPSIS        Selects whether the AES-128 CTR provider for the secCipher \
                        API is implemented based on the low-level OpenSSL APIs \
                        or the OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL_EVP
/*
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_128_CTR_OPENSSL
#endif
*/
}

Component INCLUDE_SEC_CIPHER_AES_192_CBC {
        NAME            AES-192 CBC
        SYNOPSIS        This component includes the secCipher AES 192 CBC API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes192Cbc.o
        REQUIRES        SELECT_SEC_CIPHER_AES_192_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 192 CBC provider for \
                        the secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAes.o
        PROTOTYPE       void secCipherOpensslAes192CbcOpensslInit(void);
        INIT_RTN        secCipherOpensslAes192CbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 192 CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 192 CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes192CbcEvpInit(void);
        INIT_RTN        secCipherOpensslAes192CbcEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_192_CBC_IMPL {
        NAME            AES-192 CBC
        SYNOPSIS        Selects whether the AES-192 CBC provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_192_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_AES_192_CTR {
        NAME            AES-192 CTR
        SYNOPSIS        This component includes the secCipher AES 192 CTR API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes192Ctr.o
        REQUIRES        SELECT_SEC_CIPHER_AES_192_CTR_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 192 CTR provider for \
                        the secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAesCtr.o
        PROTOTYPE       void secCipherOpensslAes192CtrInit(void);
        INIT_RTN        secCipherOpensslAes192CtrInit();
}

Component INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 192 CTR provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 192 CTR provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes192CtrEvpInit(void);
        INIT_RTN        secCipherOpensslAes192CtrEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_192_CTR_IMPL {
        NAME            AES-192 CTR
        SYNOPSIS        Selects whether the AES-192 CTR provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL_EVP
/*
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_192_CTR_OPENSSL
#endif
*/
}

Component INCLUDE_SEC_CIPHER_AES_256_CBC {
        NAME            AES-256 CBC
        SYNOPSIS        Includes the secCipher AES 256 CBC API and installs \
                        a configurable provider for this algorithm
        MODULES         secCipherAes256Cbc.o
        REQUIRES        SELECT_SEC_CIPHER_AES_256_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 256 CBC provider for \
                        the secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAes.o
        PROTOTYPE       void secCipherOpensslAes256CbcOpensslInit(void);
        INIT_RTN        secCipherOpensslAes256CbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 256 CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 256 CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes256CbcEvpInit(void);
        INIT_RTN        secCipherOpensslAes256CbcEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_256_CBC_IMPL {
        NAME            AES-256 CBC
        SYNOPSIS        Selects whether the AES-256 CBC provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_AES_256_CTR {
        NAME            AES-256 CTR
        SYNOPSIS        This component includes the secCipher AES 256 CTR API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes256Ctr.o
        REQUIRES        SELECT_SEC_CIPHER_AES_256_CTR_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 256 CTR provider for \
                        the secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAesCtr.o
        PROTOTYPE       void secCipherOpensslAes256CtrInit(void);
        INIT_RTN        secCipherOpensslAes256CtrInit();
}

Component INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 256 CTR provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 256 CTR provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes256CtrEvpInit(void);
        INIT_RTN        secCipherOpensslAes256CtrEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_256_CTR_IMPL {
        NAME            AES-256 CTR
        SYNOPSIS        Selects whether the AES-256 CTR provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL_EVP
/*
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_CTR_OPENSSL
#endif
*/
}

Component INCLUDE_SEC_CIPHER_AES_256_ECB {
        NAME            AES-256 ECB
        SYNOPSIS        This component includes the secCipher AES 256 ECB API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherAes256Ecb.o
        REQUIRES        SELECT_SEC_CIPHER_AES_256_ECB_IMPL
}

Component INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an AES 256 ECB provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslAes.o
        PROTOTYPE       void secCipherOpensslAes256EcbInit(void);
        INIT_RTN        secCipherOpensslAes256EcbInit();
}

Component INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an AES 256 ECB provider for the \
                        secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 256 ECB provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslAes256EcbEvpInit(void);
        INIT_RTN        secCipherOpensslAes256EcbEvpInit();
}

Selection SELECT_SEC_CIPHER_AES_256_ECB_IMPL {
        NAME            AES-256 ECB
        SYNOPSIS        Selects whether the AES-256 EBC provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_AES_256_ECB_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_BLOWFISH_CBC {
        NAME            Blowfish CBC
        SYNOPSIS        This component includes the secCipher Blowfish CBC API \
                        and installs a configurable provider for this algorithm.
        MODULES         secCipherBlowfishCbc.o
        REQUIRES        SELECT_SEC_CIPHER_BLOWFISH_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers a Blowfish CBC provider for \
                        the secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslBlowfish.o
        PROTOTYPE       void secCipherOpensslBlowfishCbcOpensslInit(void);
        INIT_RTN        secCipherOpensslBlowfishCbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers a Blowfish CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This Blowfish CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslBlowfishCbcEvpInit(void);
        INIT_RTN        secCipherOpensslBlowfishCbcEvpInit();
}

Selection SELECT_SEC_CIPHER_BLOWFISH_CBC_IMPL {
        NAME            Blowfish CBC
        SYNOPSIS        Selects whether the Blowfish CBC provider for the \
                        secCipher API is implemented based on the low-level \
                        OpenSSL APIs or the OpenSSL EVP API. The latter is \
                        selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_BLOWFISH_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_CAST_CBC {
        NAME            CAST CBC
        SYNOPSIS        This component includes the secCipher CAST CBC API and \
                        installs a configurable provider for this algorithm
        MODULES         secCipherCastCbc.o
        REQUIRES        SELECT_SEC_CIPHER_CAST_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers a CAST CBC provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslCast.o
        PROTOTYPE       void secCipherOpensslCastCbcOpensslInit(void);
        INIT_RTN        secCipherOpensslCastCbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers a CAST CBC provider for the \
                        secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This Cast CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslCastCbcEvpInit(void);
        INIT_RTN        secCipherOpensslCastCbcEvpInit();
}

Selection SELECT_SEC_CIPHER_CAST_CBC_IMPL {
        NAME            CAST CBC
        SYNOPSIS        Selects whether the CAST CBC provider for the secCipher \
                        API is implemented based on the low-level OpenSSL APIs \
                        or the OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_CAST_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_DES_CBC {
        NAME            DES CBC
        SYNOPSIS        This component includes the secCipher DES CBC API and \
                        installs a configurable provider for this algorithm.
        MODULES         secCipherDesCbc.o
        REQUIRES        SELECT_SEC_CIPHER_DES_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers a DES CBC provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslDes.o
        PROTOTYPE       void secCipherOpensslDesCbcOpensslInit(void);
        INIT_RTN        secCipherOpensslDesCbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers a Blowfish CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 256 CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslDesCbcEvpInit(void);
        INIT_RTN        secCipherOpensslDesCbcEvpInit();
}

Selection SELECT_SEC_CIPHER_DES_CBC_IMPL {
        NAME            DES CBC
        SYNOPSIS        Selects whether the DES CBC provider for the secCipher \
                        API is implemented based on the low-level OpenSSL APIs \
                        or the OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_DES_CBC_OPENSSL
#endif
}

Component INCLUDE_SEC_CIPHER_3DES_CBC {
        NAME            3DES CBC
        SYNOPSIS        This component includes the secCipher 3DES CBC API and \
                        installs a configurable provider for this algorithm.
        MODULES         secCipher3DesCbc.o
        REQUIRES        SELECT_SEC_CIPHER_3DES_CBC_IMPL
}

Component INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        Registers a 3DES CBC provider for the secCipher API. \
                The implementation is based on the low-level OpenSSL APIs. \
                This provider does not require the full OpenSSL library to be linked in.
    MODULES        secCipherOpenssl3Des.o
    PROTOTYPE    void secCipherOpenssl3DesCbcOpensslInit(void);
        INIT_RTN        secCipherOpenssl3DesCbcOpensslInit();
}

Component INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers a Blowfish CBC provider for \
                        the secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This AES 256 CBC provider is required \
                        when OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpenssl3DesCbcEvpInit(void);
        INIT_RTN        secCipherOpenssl3DesCbcEvpInit();
}

Selection SELECT_SEC_CIPHER_3DES_CBC_IMPL {
        NAME            3DES CBC
        SYNOPSIS        Selects whether the 3DES CBC provider for the secCipher \
                        API is implemented based on the low-level OpenSSL \
                        APIs or the OpenSSL EVP API. The latter is selected \
                        by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL
        DEFAULTS        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL_EVP    
/*
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_3DES_CBC_OPENSSL
#endif
*/
}

Component INCLUDE_SEC_CIPHER_RC4 {
        NAME            RC4
        SYNOPSIS        This component includes the secCipher RC4 API and \
                        installs a configurable provider for this algorithm.
        MODULES         secCipherRc4.o
        REQUIRES        SELECT_SEC_CIPHER_RC4_IMPL
}

Component INCLUDE_SEC_CIPHER_RC4_OPENSSL {
        NAME            OpenSSL
        SYNOPSIS        This component registers an RC4 provider for the \
                        secCipher API. The implementation is based on the \
                        low-level OpenSSL APIs. This provider does not require \
                        the full OpenSSL library to be linked in.
        MODULES         secCipherOpensslRc4.o
        PROTOTYPE       void secCipherOpensslRc4Init(void);
        INIT_RTN        secCipherOpensslRc4Init();
}

Component INCLUDE_SEC_CIPHER_RC4_OPENSSL_EVP {
        NAME            OpenSSL EVP
        SYNOPSIS        This component registers an RC4 provider for the \
                        secCipher API. The implementation is based on the \
                        OpenSSL EVP API. This RC4 provider is required when \
                        OpenSSL is built in FIPS 140-2 mode.
        MODULES         secCipherEvp.o
        PROTOTYPE       void secCipherOpensslRc4EvpInit(void);
        INIT_RTN        secCipherOpensslRc4EvpInit();
}

Selection SELECT_SEC_CIPHER_RC4_IMPL {
        NAME            RC4
        SYNOPSIS        Selects whether the RC4 CBC provider for the secCipher \
                        API is implemented based on the low-level OpenSSL APIs \
                        or the OpenSSL EVP API. The latter is selected by default.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_CIPHER_RC4_OPENSSL_EVP \
                        INCLUDE_SEC_CIPHER_RC4_OPENSSL
#if defined(_WRS_CONFIG_OPENSSL_FIPS) || defined(_WRS_CONFIG_IPNET_IPSEC) || defined(_WRS_CONFIG_IPNET_SSH)
        DEFAULTS        INCLUDE_SEC_CIPHER_RC4_OPENSSL_EVP
#else
        DEFAULTS        INCLUDE_SEC_CIPHER_RC4_OPENSSL
#endif
}
