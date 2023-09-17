/* wmMD5.h */

/* Copyright(c) 2005, 2009-2010, 2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01e,18apr12,r_w  remove dependence of retrofit.h
                 Requirement: WIND00299859
01d,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01c,30mar09,y_z  Remove wmw_httpconf.h from the include list.For use MD5 without
                 web server.
01b,25apr05,???  Get #define for JavaScript Authentication from wm_options.h,
                 since these files are in the common folder.
01a,17sep04,dkg  ported from RC3.3 to WM4.4 for JavaScript Authentication
*/

#ifndef  __MD5_HEADER__
#define  __MD5_HEADER__

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#include "wm.h"
#include "wmErrors.h"

#define     kDigestMesgBufSize  16
#define     kMD5_Match          0
#define     kMD5_NoMatch        -1

/* Data structure for MD5 (Message Digest) computation */
typedef struct
{
  ubyte4    i[2];                           /* number of _bits_ handled mod 2^64 */
  ubyte4    buf[4];                         /* scratch buffer */
  ubyte     in[64];                         /* input buffer */
  ubyte     digest[kDigestMesgBufSize];     /* actual digest after MD5_Final call */

} MD5_CTX;

/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS wmMD5INIT(void);
extern STATUS wmMD5MessageDigest(sbyte *pInputMessage, Length MesgLen, sbyte *pRetDigestMesg);
extern STATUS wmMD5MessageCompare(sbyte *pFirstMesg, sbyte *pSecondMesg);

#ifdef __cplusplus
}
#endif

#endif /* __MD5_HEADER__ */

