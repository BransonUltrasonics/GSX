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



InitGroup usrCryptoMiscInit {
	SYNOPSIS        Initialize Encryption misc library
	INIT_RTN        usrCryptoMiscInit ();
	INIT_ORDER     	INCLUDE_IPHWCRYPTO       \
					INCLUDE_IPFREESCALE
	_INIT_ORDER     usrRoot
	INIT_AFTER		usrNetworkInit
	INIT_BEFORE     usrTipcInit
}
