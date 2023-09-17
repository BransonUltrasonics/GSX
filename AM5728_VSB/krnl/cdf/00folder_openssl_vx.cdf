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
04mar16,m_c  Fixed name of FOLDER_OPENSSL
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
*/

Folder FOLDER_OPENSSL {
    NAME        OpenSSL
	SYNOPSIS    Toolkit for the Transport Layer Security (TLS) and Secure Sockets Layer (SSL) protocols.
    CHILDREN    FOLDER_CRYPTO \
                FOLDER_SSL
    _CHILDREN   FOLDER_SECURITY
}
