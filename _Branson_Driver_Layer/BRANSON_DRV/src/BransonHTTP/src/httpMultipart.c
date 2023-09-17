/* httpMultipart.c -  do some basic http multipart support */

/* Copyright 1999,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
-------------------- 
01d,02feb10,h_y   support for LP64 
01c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,13may97,mb   osal introduced.
01a,25sep96,ck   written.
*/

/*
DESCRIPTION
This file provides some basic methods for processing HTTP multipart documents.
It is especially needed for the type multipart/form-data, a special
NETSCAPE browser format, (see rfc 1867) which is used to upload files to the HTTP server.
It has a very specific format, and is used proprietary for this sole purpose.
go generate such a request, paste this HTML statements into your text:

EXAMPLE:
<FORM ENCTYPE="multipart/form-data" action="/scratch/data.bin" method=POST>
Send this file: <INPUT NAME="your_file" TYPE=file>
<INPUT TYPE="submit" VALUE="Upload File">

This will put the selected file to the location /scratch/bin on your target.

INCLUDE FILES: http/private/httpReq.h
           http/httpLib.h

NOMANUAL
*/

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpReq.h"


/*******************************************************************************
*
* httpMultipartHeaderDiscard - discard the multipart header of a file upload
*
* The upload protocol sends the "payload" encapsulated in a large amount of
* overhead.
*
* This routine discards the header information of a file upload.
* All data following this section then can be copied to a file.
* The number of bytes has to be determined with httpMultipartFileSize().
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO:
* httpMultipartFileSize(), httpMultipartFooterDiscard()
*/

HTTP_STATUS httpMultipartHeaderDiscard
    (
    HTTP_REQ_ID reqId,            /* Handle of the active request,   */
    long *      lBytesRemaining,  /* Number of bytes in socket       */
    short *     sCharsPerCRLF     /* Number of chars needed for CRLF */
    )
    {
    const char * szBoundary;
    char *       szTmp;
    HTTP_BOOL    fFoundCRLF = HTTP_FALSE;
    HTTP_BOOL    fBoundaryFound = HTTP_FALSE;
    long         lRead;

    szTmp = httpBufferGet (reqId);
    szBoundary = httpBoundaryGet (reqId);
    /*
     * read the socket until the first boundary of the multipart document
     */
    do
        {
        lRead = httpLineRead (reqId, szTmp, httpMaxTempBufSize);
        if (lRead<0)
            return (HTTP_ERROR);
        fBoundaryFound = (strncmp (szTmp, "--", 2) &&
                          strncmp (szTmp, szBoundary, strlen (szBoundary)));
        *lBytesRemaining -= lRead;
        }
    while (*lBytesRemaining > 0 && !fBoundaryFound);

    /*
     * read (and discard) until the next CR/LF
     */
    while (*lBytesRemaining > 0 && !fFoundCRLF)
        {
        lRead = httpLineRead (reqId, szTmp, httpMaxTempBufSize);
        if (lRead<0)
            return (HTTP_ERROR);
        fFoundCRLF = (szTmp[0]==0);
        *lBytesRemaining -= lRead;
        }
    if (fFoundCRLF)
        {
        *sCharsPerCRLF = (short) lRead;
        return (HTTP_OK);
        }
    else
        return (HTTP_ERROR);
    }


/*******************************************************************************
*
* httpMultipartFooterDiscard - discard the multipart footer of a file upload
*
* This routine clears the socket and removes footer protocol
* information after receiving a file upload, which is generally a good
* practice.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO:
* httpMultipartFileSize(), httpMultipartHeaderDiscard()
*/

HTTP_STATUS httpMultipartFooterDiscard
    (
    HTTP_REQ_ID reqId,           /* Handle of the active request,   */
    long        lBytesRemaining  /* Number of bytes in socket       */
    )
    {
    char *  szTmp;
    long    lRead;
    long    lChunkSize;

    szTmp = httpBufferGet (reqId);

    while (lBytesRemaining > 0)
        {
        if ((long)httpMaxTempBufSize < lBytesRemaining)
            lChunkSize = (long) httpMaxTempBufSize;
        else
            lChunkSize = lBytesRemaining;
        lRead = httpBlockRead (reqId, szTmp, (unsigned long) lChunkSize);
        if (lRead<0)
            return (HTTP_ERROR);
        lBytesRemaining -= lRead;
        }
    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpMultipartFileSize - determine the size of a file upload
*
* This routine returns the size of an uploaded file.
* When a file is uploaded, the size must be determined (how many bytes
* are to read from the socket).
* Do not read until EOF, which would include footer information
* that would clobber the file.
*
* NOTE:
* You need to specify the number of bytes needed for a CRLF in the current
* transmission.  This number can be obtained from httpMultipartHeaderDiscard().
*
* RETURNS: The file length in bytes.
*
* SEE ALSO:
* httpMultipartHeaderDiscard(), httpMultipartFooterDiscard()
*/

long httpMultipartFileSize
    (
    HTTP_REQ_ID reqId,           /* Handle of the active request,   */
    long        lBytesRemaining, /* Number of bytes in socket       */
    short       sCharsPerCRLF    /* Number of chars needed for CRLF */
    )
    {
    long   lFileSize;
    const char * szBoundary;

    szBoundary = httpBoundaryGet (reqId);

    lFileSize = lBytesRemaining -
                (long) strlen (szBoundary) -
                4 -                       /* additional '-' signs */
                sCharsPerCRLF;            /* chars per CRLF!      */
    return (lFileSize);
    }

