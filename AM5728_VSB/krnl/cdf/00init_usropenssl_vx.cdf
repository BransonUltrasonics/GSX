/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
*/



InitGroup usrOpensslInit {
	SYNOPSIS        Initialize Openssl library
	INIT_RTN        usrOpensslInit ();
	INIT_ORDER      INCLUDE_IPCRYPTO  \
					INCLUDE_IPSSL
	_INIT_ORDER     usrRoot
	INIT_AFTER		INCLUDE_SOCKLIB
	INIT_BEFORE     usrNetworkInit
}
