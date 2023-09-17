/* httpBase64Util.c - utility routines for handling base64-encoded data */

/* Copyright (c) 1996/97,1999-2003,2010,2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
28may14,r_w  fix compile warnings(V7MAN-78)
21jan14,r_w  add "signed" keyword to cDecoded(JIRA,VXW6-27044)
20feb13,r_w  move strlen outof loop and change sighed to unsigned
                 defect: WIND00403091 and WIND00403090
02feb10,h_y  support for LP64
08aug03,jws  HTTP_BOOL -> BOOL
06may03,jws  remove assert()'s
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
14may97,mb   osal introduced.
15oct96,ck   written.
*/

/*
DESCRIPTION
This module contains routines which allow the user to encode data into
base64 format, or decode data held in that format. This allows data to
be converted into "pure ASCII", for example, to send it over a comm-
link which is not 8-bit clean.

INCLUDE FILES: httpLib.h

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "httpLib.h"


/**************************************************************************
*
* httpBase64decode - Decode a base64-encrypted string.
*
* This routine decodes a string that is base64 encoded (according to
* RFC 1521).  Care should be taken that the generated string (about 3/4 of
* the size of the source string) will fit into the buffer <szDestination>.
*
* RETURNS: The length of the decoded string, or -1 if the input data
* is not in base64 format.
*
* SEE ALSO: httpBase64encode()
*/

short httpBase64decode
    (
    unsigned char * szSource,              /* base64 encoded string                  */
    unsigned char * szDestination          /* pointer to store the decoded string at */
    )
    {
    unsigned short  sIndexSrc;
    unsigned short  sIndexDst = 0;
    unsigned short  sIndexByte;
    unsigned short  sCounter = 0;
    unsigned short  sBitsInTriple = 0;
    unsigned short  sBytesInTriple = 0;
    unsigned short  srcLength = 0; 
    unsigned long   ulTriple = 0;     /* needs to hold at least 24 bit */
    unsigned char   cCurrent;
    signed char     cDecoded;         /* CHANGED FROM UNSIGNED -CK- */
    BOOL            fDone = FALSE;

    srcLength = (unsigned short)strlen ((char *)szSource);
    for (sIndexSrc = 0; sIndexSrc < srcLength && !fDone; sIndexSrc ++)
        {
        cCurrent = szSource [sIndexSrc];
        if (cCurrent >= 'A' && cCurrent <='Z')
            cDecoded = (signed char)(cCurrent - 'A');
        else if (cCurrent >= 'a' && cCurrent <='z')
            cDecoded = (signed char)(26 + cCurrent - 'a');
        else if (cCurrent >= '0' && cCurrent <='9')
            cDecoded = (signed char)(52 + cCurrent - '0');
        else if (cCurrent == '+')
            cDecoded = 62;
        else if (cCurrent == '/')
            cDecoded = 63;
        else if (cCurrent == '=')
            cDecoded = -1;
        else
            {
            szDestination[sIndexDst] = 0;
            return -1;
            }
        ulTriple = ulTriple << 6;
        if (cDecoded >= 0)
            {
            ulTriple |= (unsigned char)cDecoded & 63;
            sBitsInTriple = (unsigned short)(sBitsInTriple + 6);
            }
        sCounter ++;

        if (sCounter == 4)
        {
            for (sIndexByte = 1; sIndexByte <= 3; sIndexByte ++)
                {
                szDestination[sIndexDst + 3 - sIndexByte] = (unsigned char)(ulTriple & 0xff);
                ulTriple = ulTriple >> 8;
                }
            sBytesInTriple = ((sBitsInTriple) / 8);
            sIndexDst  = (unsigned short)(sIndexDst + sBytesInTriple);

            if (sBytesInTriple != 3)
			  fDone = TRUE;

            sCounter  = 0;
            sBitsInTriple = 0;
            ulTriple  = 0;
            }
        }
    szDestination [sIndexDst] = 0;
    return sIndexDst;
    }


/**************************************************************************
*
* httpBase64encode - Encode a string in base64 format.
*
* This routine encodes a byte array (usually a string) according to the
* base64 conventions (see RFC 1521).  Because the string can hold
* any characters (including zeroes), you must also specify the length of
* <cpSource>.
* The buffer <szDestination> specifies where the encoded string will be
* stored. It must be able to hold 4/3 of the original byte array's size.
*
* RETURNS: HTTP_ERROR if the destination buffer is too small, otherwise
* HTTP_OK.
*
* SEE ALSO: base64decode()
*/

HTTP_STATUS httpBase64encode
    (
    unsigned char *           cpSource,        /* pointer to source data       */
    unsigned short   sSourceLength,   /* length of source             */
    unsigned char *           szDestination,   /* buffer for encoded String    */
    unsigned short   usDestLength     /* length of destination Buffer */
    )
    {
    unsigned short  sIndexSrc;
    unsigned short  sIndexDst = 0;
    unsigned short  sIndexByte;
    unsigned short  sBitsInTriple = 0;
    unsigned short  sBytesInTriple = 0;
    unsigned long   ulTriple = 0;     /* needs to hold at least 24 bit */
    unsigned char   cCurrent;
    unsigned char   cEncoded;
    unsigned short  sTmp;
    BOOL            fDone = FALSE;

    for (sIndexSrc = 0; !fDone; sIndexSrc ++)
    {
        ulTriple = ulTriple << 8;
        if (sIndexSrc < sSourceLength)
        {
            ulTriple |= cpSource[sIndexSrc];
            sBitsInTriple = (unsigned short)(sBitsInTriple + 8);
        }

        if (sIndexSrc % 3 == 2)
            {
            sBytesInTriple = (unsigned short)((sBitsInTriple + 5)/6);
            if (sBitsInTriple == 0)
                {
                fDone = TRUE;
                break;
                }
            for (sIndexByte = 4; sIndexByte >0; sIndexByte --)
                {
                if (sIndexByte > sBytesInTriple)
                    {
                    cEncoded = '=';
                    fDone = TRUE;
                    }
                else
                    {
                    cCurrent = (unsigned char) (ulTriple & 0x3f);
                    if (cCurrent < 26)
                        cEncoded = (unsigned char)(cCurrent + 'A');
                    else if (cCurrent < 52)
                        cEncoded = (unsigned char)((cCurrent - 26) + 'a');
                    else if (cCurrent < 62)
                        cEncoded = (unsigned char)((cCurrent - 52) + '0');
                    else if (cCurrent == 62)
                        cEncoded = '+';
                    else if (cCurrent == 63)
                        cEncoded = '/';
                    else
                        return HTTP_ERROR;      /* this never can happen!? */
                    }
                sTmp = (unsigned short)(sIndexDst + sIndexByte - 1);

                /* 08/30/02 sld - Replace >= with > */
                if (sTmp > (usDestLength - 1))
                    {
                    return HTTP_ERROR;
                    }

                szDestination [sTmp] = cEncoded;
                ulTriple = ulTriple >> 6;
                }

            sIndexDst    = (unsigned short)(sIndexDst + 4);
            ulTriple      = 0;
            sBitsInTriple = 0;
            }
    }

    if ((unsigned short)sIndexDst < usDestLength)
        {
        szDestination [sIndexDst] = 0;
        return HTTP_OK;
        }
    return HTTP_ERROR;
    }


	
	

