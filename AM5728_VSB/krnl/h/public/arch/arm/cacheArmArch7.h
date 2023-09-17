/* cacheArmArch7.h - ARM ARCH7 cache library header file */

/*
 * Copyright (c) 2008, 2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
28nov13,cfm  moved the contents to cacheArmArch.h
09nov12,j_b  remove unnecessary L1 data cache operation macro
04oct12,j_b  add macro to operate only on entire L1 D-cache
21sep12,j_b  Add DSB to end of _CORTEX_AR_ENTIRE_DATA_CACHE_OP
10sep12,m_h  L2 cachePipeFlush (WIND00240234)
02jun10,m_h  Thumb-2 Support
19may10,z_l  Declare new functions which support L2 cache.
01aug08,j_b  fix _CORTEX_AR_ENTIRE_DATA_CACHE_OP labels
11jun08,j_b  Created from cacheArmArch6.h, rev 01b
*/

#include "cacheArmArch.h"

