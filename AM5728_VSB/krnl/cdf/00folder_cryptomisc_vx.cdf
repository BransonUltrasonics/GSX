/*
 * Copyright (c) 2014,2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23dec16,ply  Remove needless character. (V7SEC-342)
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
*/


Folder FOLDER_CRYPTOMISC {
    NAME        CRYPTO MISC Components
    CHILDREN    INCLUDE_IPHWCRYPTO \
                INCLUDE_IPFREESCALE
    _CHILDREN   FOLDER_SECURITY
}
