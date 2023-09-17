/* httpRpmUpload.c - Handle forms and files submitted according RFC 1867 */

/* Copyright (c) 1999-2008, 2010, 2012-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
17feb16,r_w  fix compiler and static analysis warnings.
11jun15,r_w  fix static analysis issue V7MAN-154.
26sep14,r_w  fix static CHECKED_RETURN issue.
27aug14,r_w  close file decription for defect VXW6-83291
05may14,r_w  fix static analysis issues(US35919)
05mar14,r_w  disable form post feature in webserver standalone mode. defect VXW6-80639 and VXW6-1806
22jan14,r_w  add error checking for rename() call(VXW6-69956)
08apr13,r_w  fix static analysis issues for defect WIND00411995
19dec12,r_w  increase HTTP_SYMBOL_NAME_SIZE_MAX and add memory protection.defect WIND00392421
02nov12,r_w  check the upload path existence before open/write. defect WIND00386627
09sep12,r_w  modify lclHttpRpmUpload().defect WIND00373593
20jul12,r_w  eliminate memory leak issue. defect WIND00359749
17dec10,m_z  part of APIs changed
             CQID: Fix WIND00247096
11nov10,m_z  file read detection when uploading by standalone web
             CQID: Fix WIND00240879
25sep10,m_z  requiement WIND00203779
02feb10,h_y  support for LP64 
28mar08,q_z  Fix defect WIND00118541
22feb07,ps   apigen reformatting
09nov04,dlr  SPR #102705 : allow upload to be last RPM.
13oct04,gsr  changed httpMaxTempBufSize in httpRpmUpload to
             kMagicMarkupBufferSize for SPR 102228
03jun04,adb  corrected and eliminated truncated boundary handling
15mar04,adb  introduced httpSecureUploadConfAdd
13jan04,gsr  cleanup routine for SPR 93009
15dec03,adb  merged in SPR 90310 fix
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpUploadConfigurationShow routine
01oct03,jws  remove assert()'s
29oct01,jc   renamed and modified from httpPostMethodUtil.c
27jan00,csr  adding more error checking in httpReqParse().
20jan00,va   removing protocol violation error to enable livecontrol
13jan00,va   fixed truncation of each line during file upload
05jan00,va   fixed string termination for <szTagContent>
13dec99,va   written.
 */

/*
DESCRIPTION
provides funtionality to handle form submission that use POST method

INCLUDE FILES: httpLib.h
               httpRpms.h
               httpShow.h
 */

/* includes */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <usrFsLib.h>
#include "httpLib.h"
#include "httpRpms.h"
#include "private/httpInt.h"
#include "httpShow.h"
#include "commonCfg.h"
#include "httpCfg.h"
#ifdef __POSIX_OS__
#include <sys/vfs.h>
#endif

#ifndef WEBCLI_STANDALONE_WEBSERVER
#ifdef INCLUDE_HTTP_UPLOAD

/* defines */

#define HTTP_SYMBOL_NAME_SIZE_MAX 256

#define HTTP_POST_ERROR_FLAG_WRITE            0x01
#define HTTP_POST_ERROR_FLAG_OPEN             (HTTP_POST_ERROR_FLAG_WRITE << 1)
#define HTTP_POST_ERROR_FLAG_LINEREAD         (HTTP_POST_ERROR_FLAG_OPEN << 1)
#define HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY    \
		(HTTP_POST_ERROR_FLAG_LINEREAD << 1)
#define HTTP_POST_ERROR_FLAG_CLOSE              \
		(HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY << 1)
#define HTTP_POST_ERROR_FLAG_RENAME              \
		(HTTP_POST_ERROR_FLAG_CLOSE << 1)

#define HTTP_POST_ERROR_FLAG_MPART_HEADER_PARSE \
		(HTTP_POST_ERROR_FLAG_RENAME << 1)
#define HTTP_POST_ERROR_FLAG_CONTENT_DISCARD \
		(HTTP_POST_ERROR_FLAG_MPART_HEADER_PARSE << 1)
#define HTTP_POST_ERROR_FLAG_MEM_FREE \
		(HTTP_POST_ERROR_FLAG_CONTENT_DISCARD << 1)
#define HTTP_POST_ERROR_FLAG_NOT_ENOUGH_SPACE_ON_DEVICE \
		(HTTP_POST_ERROR_FLAG_MEM_FREE << 1)
/* error messages */

#define HTTP_POST_ERROR_MESG_OPEN \
		"<H2>Permission denied: Server cannot create/open file</H2> terminating..."
#define HTTP_POST_ERROR_MESG_LINEREAD \
		httpErrStringGet(HTTP_ERROR_TRANSFER_INTERRUPTED)
#define HTTP_POST_ERROR_MESG_WRITE \
		"<H2>Permission denied: Server cannot write to file</H2> terminating..."
#define HTTP_POST_ERROR_MESG_CLOSE \
		"<H2>Permission denied: Server cannot close file</H2> terminating..."
#define HTTP_POST_ERROR_MESG_RENAME \
		"<H2>Permission denied: Server cannot rename file</H2> terminating..."
#define HTTP_POST_ERROR_MESG_OUT_OF_MEMORY \
		httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE)
#define HTTP_POST_ERROR_MESG_MEM_FREE \
		httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE)
#define HTTP_POST_ERROR_MESG_CONTENT_DISCARD \
		"<H2>Error: Server cannot read from client</H2> terminating..."
#define HTTP_POST_ERROR_MESG_MPART_HEADER_PARSE \
		"<H2>Error: Server cannot parse multipart header</H2> terminating..."
#define HTTP_POST_ERROR_MESG_NOT_ENOUGH_SPACE_ON_DEVICE \
		" Server has no enough disk space "
/* do not define this flag if httpWmbPost RPM is enabled */

#undef ENABLE_UPLOAD_STATISTICS

/* forward static functions */

LOCAL char *  httpPostBoundaryFind (char * pBuf, char * sz, unsigned long size);
#if 0
LOCAL int  httpPostTruncBoundaryFind (char * pBuf, char * sz, unsigned size);
#endif
LOCAL HTTP_STATUS httpPostContentDispositionParse
(char * buf, int * flag, char * tagValue);
LOCAL HTTP_STATUS httpPostRequestErrorHandler
(HTTP_REQ_ID reqId, int * iPostErrorMask);
LOCAL HTTP_STATUS httpPostUploadPathGet (HTTP_REQ_ID r, sbyte * p, unsigned long s);

#ifdef DEBUG_INFO
LOCAL void    httpPostPrintFiniteString (char * msg, char * buf, int len);
#endif

LOCAL HTTP_GEN_LIST_ID httpUpldConfigId HTTP_PREDEFINE ( NULL );
LOCAL HTTP_GEN_LIST_ID httpSecureUpldConfigId HTTP_PREDEFINE ( NULL );
void httpUploadReplyNoWmb(HTTP_REQ_ID reqId);
short lclHttpRpmUpload(HTTP_REQ_ID reqId,BOOL lclUploadRpmNotLast );

/* File upload notify=ication to user-space variables*/
#define SDO_MSG_Q_NAME "/SDO_Task"
#define TO_SDO_FW_ETHRNT_UPLOAD_RESP 109
#define	MAX_SIZE_OF_MSG_LENGTH		512
#define   SUCCESS  					 0
#define   FAILURE					-1
#define   FILE_LIST_ERROR      	 	0x0003
#define   CACHE_FILE          		"/ram0/FirmwareFiles.txt"
/* Firmware upgrade file name format */
#define   AC_FILE_NAME_FORMAT        "AC_*.bin"
#define   PC_FILE_NAME_FORMAT        "PC_*.bin"
#define   SC_FILE_NAME_FORMAT        "SC_*.bin"

#define   RDWR_CREAT         		"w+"
#define   ZERO_BYTES                 0
#define	  READ_BYTE           		 1
#define   FW_UPGRADE_FILE_COUNT_MAX  10
#define   FILE_NAME_MAX_SIZE     	 255
/* File headers for SC, PC & AC */
#define   SC_FILE_HEADER			 "SC_"
#define   PC_FILE_HEADER		     "PC_"
#define   AC_FILE_HEADER		     "AC_"


typedef struct Message
{
	INT32	msgID;
	char	Buffer[MAX_SIZE_OF_MSG_LENGTH-4];	
}Message;

MSG_Q_ID SDO_MSG_Q_ID  = MSG_Q_ID_NULL; 

/* locals */

LOCAL BOOL uploadRpmNotLast = FALSE; /* another RPM will follow this one */

LOCAL OS_SEM_T			semLockUpload     = NULL;	/* File Upload lock */

/***************************************************************************
 *
 * httpUploadConfInit - initialize the sizes of the tables
 *
 * This routine initializes the internal boundaries needed for
 * subsequent calls of all the other API functions to access the
 * upload configuration tables.
 *
 * RETURNS: List ID of the configuration list, or
 * NULL if the configuration list cannot be created.
 *
 * SEE ALSO: httpUploadConfAdd
 *           httpSecureUploadConfAdd
 */

HTTP_STATUS httpUploadConfInit
(
		unsigned short ulBufMax  /* Maximum number of entries in predefined table */
)
{
	MEM_POOL_ID memPartId=httpGlobalMemPartIdGet();

	/* Init semaphore for Upload */
	if ( OK != wmosSemMCreate ( &semLockUpload, HTTP_SEM_Q_PRIORITY ))
	{
		semLockUpload = NULL;
		return (HTTP_ERROR);
	}

	if  (NULL == (httpUpldConfigId = httpGenListCreate (memPartId,
			ulBufMax,
			HTTP_TRUE)))
	{
		return (HTTP_ERROR);
	}

	if  (NULL == (httpSecureUpldConfigId = httpGenListCreate (memPartId,
			ulBufMax,
			HTTP_TRUE)))
	{
		httpUploadConfCleanup ();

		return (HTTP_ERROR);
	}

	return (HTTP_OK);
}

/***************************************************************************
 *
 * httpUploadConfCleanup - cleanup the sizes of the tables
 *
 * This routine cleanup the resources allocated by httpUploadConfInit
 *
 * RETURNS: HTTP_OK always
 *
 * SEE ALSO: httpUploadConfInit()
 */

HTTP_STATUS httpUploadConfCleanup
(
		void
)
{
	if  (NULL != httpUpldConfigId)
	{
		httpGenListDestroy (httpUpldConfigId);
	}

	httpUpldConfigId = NULL;

	if  (NULL != httpSecureUpldConfigId)
	{
		httpGenListDestroy (httpSecureUpldConfigId);
	}

	httpSecureUpldConfigId = NULL;

	if (semLockUpload != NULL)
	{
		(void)wmosSemDestroy(semLockUpload);
		semLockUpload = NULL;
	}

	return (HTTP_OK);

} /* httpUploadConfCleanup */

/***************************************************************************
 *
 * httpUploadRpmNotLast - signal that another RPM
 *
 * This routine is called by another RPM (such as httpRpmWmbPost) to request
 * from httpRpmUpload the opportunity to post process POST requests.
 * This will force httpRpmUpload to return RPM_OK instead of RPM_DONE once it's
 * done its job.
 *
 * RETURNS: HTTP_OK or HTTP_ERROR
 *
 */

HTTP_STATUS httpUploadRpmNotLast (void)
{
	uploadRpmNotLast = TRUE;
	return HTTP_OK;
}

/***************************************************************************
 *
 * httpUploadConfAdd - add an entry to the uploading table
 *
 * This routine adds the specified entry to the uploading table.
 *
 * NOTE: The string is not copied.  Only a pointer to its location is
 * stored.
 *
 * RETURNS: HTTP_GEN_LIST_ENTRY_ID of new configuration entry, or
 * NULL if the new entry could not be added.
 *
 * SEE ALSO: httpUploadConfInit()
 */

HTTP_GEN_LIST_ENTRY_ID httpUploadConfAdd
(
		char * szUrl,         /* Incoming URL request          */
		char * szFsRoot       /* Corresponding filesystem path */
)
{
	return ( httpGenListDataAppend ( httpUpldConfigId , szUrl , szFsRoot ) );
}

/***************************************************************************
 *
 * httpSecureUploadConfAdd - add an entry to the secure URL uploading table
 *
 * This routine adds the specified entry to the secure URL uploading table.
 *
 * NOTE: The string is not copied.  Only a pointer to its location is
 * stored.
 *
 * RETURNS: HTTP_GEN_LIST_ENTRY_ID of new configuration entry, or
 * NULL if the new entry could not be added.
 *
 * SEE ALSO: httpUploadConfInit()
 */

HTTP_GEN_LIST_ENTRY_ID httpSecureUploadConfAdd
(
		char * szUrl,         /* Incoming URL request          */
		char * szFsRoot       /* Corresponding filesystem path */
)
{
	return (httpGenListDataAppend (httpSecureUpldConfigId, szUrl, szFsRoot));
}

#if 0
/***************************************************************************
 *
 * httpPostTruncBoundaryFind - finds a truncated MIME boundary
 *
 * This routine searches for a truncated MIME boundary in the end of a given
 * buffer <pBuffer>
 *
 * NOMANUAL
 *
 * RETURNS: the size of the truncated boundary or 0 if not found
 */
LOCAL int httpPostTruncBoundaryFind
(
		char * pBuffer,              /* Buffer to be searched      */
		char * szBoundary,           /* Boundary                   */
		unsigned int lChunkSize      /* Buffer Length of <pBuffer> */
)
{
	size_t  offset;
	char    *fBoundary = NULL;
	int     lBoundaryLen = wmMin (strlen(szBoundary), lChunkSize);

	/*
	 * an entire boundary was not found, attempt to find a truncated
	 * boundary
	 */

	for (offset = (lChunkSize - lBoundaryLen + 1);offset < lChunkSize;offset++)
	{
		lBoundaryLen--;
		if ((memcmp(pBuffer+offset, szBoundary,
				(size_t) lBoundaryLen)) == 0)
		{
			fBoundary = pBuffer + offset;
			break;
		}
	}
	return (lChunkSize - offset);
}
#endif

/***************************************************************************
 *
 * httpPostBoundaryFind - finds a MIME boundary
 *
 * This routine searches for a MIME boundary in a given buffer <pBuffer>
 *
 * NOMANUAL
 *
 * RETURNS: a pointer to the start of the boundary or NULL
 */

LOCAL char * httpPostBoundaryFind
(
		char *pBuffer,               /* Buffer to be searched      */
		char *szBoundary,            /* Boundary                   */
		unsigned long lChunkSize      /* Buffer Length of <pBuffer> */
)
{
	const char *p1;
	const char *p2;
	size_t  offset, n;
	char    *fBoundary = NULL;
	unsigned long  lBoundaryLen = strlen(szBoundary);
	int     fEqual;

	if (lChunkSize < lBoundaryLen) return (NULL);

	for (offset=0 ; offset < (lChunkSize - lBoundaryLen + 1) ; offset++)
	{

		p1 = pBuffer+offset;
		p2 = szBoundary;
		n  = lBoundaryLen;
		fEqual = 0;
		while (*p1++ == *p2++)
		{
			if (--n == 0)
			{
				fEqual = 1;
				break;
			}
		}
		if (fEqual)
		{
			fBoundary = pBuffer + offset;
			break;
		}
	}
	return (fBoundary);
}

void httpUploadReplyNoWmb
(
		HTTP_REQ_ID     reqId           /* pointer to request structure */
)
{
	generic_fd_t           f;
	unsigned int           lCount;
	sbyte *                szBuffer;
	sbyte *                cpTransBuffer;
	unsigned long          ulMaxTransBufSize;
	MEM_POOL_ID            mpPool;
	ssize_t                lFileSize=0;
	unsigned long          lReadCount=0;
	ubyte4                 bytesRead = 0;
	ubyte4                 bytesToRead;


	szBuffer = httpBufferGet (reqId);

	if  (HTTP_OK != httpFsRootGet (reqId,szBuffer,httpMaxTempBufSize))
	{
		httpStatusSet( reqId, HTTP_NOT_FOUND );
		(void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
		return;
	}

	strncat (szBuffer,
			httpRpmDataGet (reqId),
			httpMaxTempBufSize - strlen (szBuffer) - 1);

	if (OK != httpFileOpen(szBuffer, "rb", &f))
	{
		httpStatusSet( reqId, HTTP_NOT_FOUND );
		(void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
		return;
	}

	if (0 >= (lFileSize = httpFileSize(&f)))
	{
		(void) httpFileClose(&f);
		return;
	}

	/*
	 * After next call httpBlockPut will send chunks, not plain blocks.
	 * If req is HTTP/1.0, this has no effect.
	 */

	httpChunkedEncodingOn (reqId);

	mpPool = httpReqMemPartIdGet (reqId);

	ulMaxTransBufSize = (unsigned long)lFileSize;
	cpTransBuffer = httpConservativeAlloc (mpPool, (size_t*)&ulMaxTransBufSize);

	lCount = (unsigned int)((ulMaxTransBufSize > lFileSize) ? lFileSize : ulMaxTransBufSize);

	if (cpTransBuffer == NULL)
	{
		logError ("httpRpmWmbParse: out of request-pool memory");
		httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
		httpError( reqId, httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE));
		(void) httpFileClose(&f);
		return;
	}

	logInfoMsg ("httpRpmFs: opening file %s...\n",szBuffer);

	/* if we are here, we have a non-empty open file waiting to be read */

	httpStatusSet (reqId, HTTP_OK);

	/*
	 * If a Content-Type was registered for the file, use it.
	 * Else, leave the one guessed by the Mime RPM.
	 */

	if (httpFileContentTypeGet(&f))
		(void) httpMimeHdrSet(reqId, HDR_OUT, "Content-Type",
				httpFileContentTypeGet(&f));

	(void) httpHeaderGenerate (reqId);

	do
	{
		bytesToRead = (ubyte4)((ulMaxTransBufSize > (lFileSize)) ?
				(lFileSize ) : (ulMaxTransBufSize));

		if (0 == bytesToRead) break;

		if (OK != httpFileRead(&f, cpTransBuffer,
				bytesToRead, &bytesRead))
		{
			wmError (ERROR_GENERAL, "failed reading file");
			break;
		}

		lReadCount += bytesRead;
		if (HTTP_ERROR == httpBlockPut (reqId, cpTransBuffer, bytesRead))
			break;

		lCount = (unsigned int)((ulMaxTransBufSize > (lFileSize - lReadCount)) ? (lFileSize-lReadCount ) :
				ulMaxTransBufSize);
		if (0 == lCount) break;

	} while (bytesRead);
	(void)httpBlockPut (reqId, NULL, 0); /* needed to terminate chunked encoding */
	(void)httpFileClose (&f);
	(void)memPoolFree (mpPool, cpTransBuffer);
	return;
}

short lclHttpRpmUpload
(
		HTTP_REQ_ID     reqId,      		/* pointer to request structure */
		BOOL    		lclUploadRpmNotLast	/* chain to next RPM (return RPM_OK) */
)
{
	char *        szTagTmp = NULL;     /* Symbol or Filename sent in the header */
	char *        szTagContent = NULL; /* Value of Symbol correspoding to <szTagTmp>*/
	char *        pStrTmp = NULL;      /* temporary character */
	unsigned long lRead = 0;     /* Bytes read from the socket */
	unsigned long lContentLength = 0; /* Content Length */
	unsigned long lFileSize = 0;      /* Length of Block to be read*/
	unsigned long lChunkSize = 0;
	short        sCharsPerCRLF = 0;  /* Number of chars representing CR and LF */
	MEM_POOL_ID partitionId;     /* Request Memory partition Id */
	generic_fd_t iFile;          /* File Descriptor for file operations */
	short fFileOpened = 0;       /* Flag - if any file is currently open */
	unsigned long  ulUploadedBytes=0;       /* Total bytes of current symbol or file */
	int iPostErrorMask =HTTP_OK; /* the error returned by a specific function */
	char *pPrevBuf       = NULL;
	unsigned long lBytesInPrevBuf = 0;
	char *pCurrBuf       = NULL;
	unsigned long lBytesInCurrBuf = 0;
	char *pSwapBuf       = NULL;
	char *pFoundBoundary     = NULL; /*pointers to boundary found in mergedbuf*/
	char *szBoundary;           /* Pointer to Boundary */
	/*unsigned long lBoundaryLen    = 0;*/
	unsigned long len = 0; /* Length of <bufToWrite> */
	int firsttime=1; /* flag */
	int flush=0;     /* flag */
	int tagFlag=1; /* type of form element flag */
	ubyte4 bw = 0;
	STATUS status = 0;
	unsigned long freeCount = 0;
	struct statfs f;
	size_t tmpLenth = 0;


#ifdef ENABLE_UPLOAD_STATISTICS
	time_t   elapsedTime;
	time_t   startTime;
#endif
	char czTmpFilename[64]={0};
	char czTmpSrcFilename[256];
	char czTmpDstFilename[256];
	time_t tFileTime;
	size_t tempLen=0;
	int i = 0;
	int nameCount = 1;

	(void) time(&tFileTime);
	(void)snprintf(czTmpFilename, sizeof(czTmpFilename), "%s", ctime(&tFileTime));
	tempLen=strlen(czTmpFilename);
	czTmpFilename[tempLen-1]=' ';
	(void)snprintf(czTmpFilename+tempLen,sizeof(czTmpFilename)-tempLen, "%ld", (long)reqId);
	strncat(czTmpFilename, ".tmp", 5);
	for(i=0;i<(int)strlen (czTmpFilename);i++)
	{
		if((czTmpFilename[i]==' ')||(czTmpFilename[i]==':'))
			czTmpFilename[i]='_';
	}

	if (strcmp (httpMimeContentTypeGet (reqId, HDR_IN), "multipart/form-data"))
	{
		logError ("httpRpmUpload: Media type unknown, not handled here!");
		return (RPM_OK); /* maybe another RPM will handle it... */
	}

	szBoundary = httpBoundaryGet(reqId); /* Get the Boundary for this request */
	/*lBoundaryLen = strlen( szBoundary );*/ /* calculated once for performance */
	partitionId = httpReqMemPartIdGet(reqId); /* Request Memory partition*/

	/* This should be done right before all the memory allocations and disk eval */

	lContentLength = (unsigned long)httpMimeContentLengthGet (reqId, HDR_IN);
	logInfoMsg ("httpRpmUpload: Content Length: %lu\n",lContentLength);

	/* Check if there is enough space for upload */

	if (HTTP_OK != httpPostUploadPathGet(reqId, czTmpSrcFilename, 256))
	{
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OPEN;
		return RPM_ERROR;
	}

	status = statfs(czTmpSrcFilename, &f);
	if(ERROR == status)
	{
		logError("httpRpmUpload: get disk space infor failed! ");
	}
	else
	{
		logErrorMsg("httpRpmUpload: lContentLength=%d ",lContentLength);
		logErrorMsg("httpRpmUpload: freeCount=%d ",freeCount);

		freeCount = (unsigned long)(f.f_bsize * f.f_bavail);
		/* Check for enough space */
		if(lContentLength > freeCount)
		{
			logError("httpRpmUpload: not enough space for upload! ");
			iPostErrorMask |= HTTP_POST_ERROR_FLAG_NOT_ENOUGH_SPACE_ON_DEVICE;
			goto HTTP_POST_REQUEST_PARSE_MEM_FREE;
		}
	}

	/* Allocate buffers for all operations */

	if (partitionId == NULL)
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY;
	if ((pPrevBuf  = memPoolAlloc( partitionId, httpMaxTempBufSize )) == NULL)
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY;
	if ((pCurrBuf = memPoolAlloc( partitionId, httpMaxTempBufSize)) == NULL)
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY;
	if ((szTagTmp = memPoolAlloc(partitionId, HTTP_SYMBOL_NAME_SIZE_MAX)) == NULL)
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY;
	if ((szTagContent = memPoolAlloc(partitionId, kMagicMarkupBufferSize )) == NULL)
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY;

	if ( iPostErrorMask & HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY )
	{
		logError("httpRpmUpload: buffer memory allocation failed");
		goto HTTP_POST_REQUEST_PARSE_MEM_FREE;
	}

	/* Discard the first header read - Read Boundary */

	lRead = (unsigned long)httpLineRead (reqId, pPrevBuf, strlen(szBoundary)+4);
	logInfoMsg ("httpRpmUpload: Boundary Length Read: %lu \n",lRead );

#ifdef DEBUG_INFO
	httpPostPrintFiniteString("httpRpmUpload: First Bound: ", pCurrBuf, lRead);
#endif

	lContentLength-=lRead; /* Decrease the length of Boundary read */

	/*
	 * Read Content till you read <lContentLength>, First parse header,
	 * then content corresponding to symbol or filename
	 */

	while (lContentLength > 0)
	{
		lFileSize = 0;
		logInfo("httpRpmUpload: Header parsing now ");

		if (httpPostMultipartHeaderParse (reqId, (long *) &lContentLength,
				&sCharsPerCRLF, &tagFlag, szTagTmp) == HTTP_ERROR)
		{
			logError ("httpRpmUpload: Protocol violation - Header format mismatch");
			lFileSize = 0;
			iPostErrorMask |= HTTP_POST_ERROR_FLAG_MPART_HEADER_PARSE;
			break;
		}

		/* current buffer size */

		lFileSize = (lContentLength > httpMaxTempBufSize ) ?
				httpMaxTempBufSize: lContentLength;


		firsttime = 1;
		len = 0;
		lRead = 0;
		ulUploadedBytes=0;
		fFileOpened = 0;

		/* File needs to be opened only if tagFlag is set, and only if filename exists */
		len = strlen(szTagTmp);
		if ( (tagFlag) && (len > 0) )
		{
			/*if ( (szTagTmp != NULL) ||
                  ( (len == 1) && (strcmp(szTagTmp, " ") != 0) ) )
                {*/
			pStrTmp = szTagTmp;
			while (isspace ((int) *pStrTmp)) pStrTmp++;
			if (HTTP_OK != httpPostUploadPathGet
					(reqId, pCurrBuf, httpMaxTempBufSize - strlen(pStrTmp)+1))
			{
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_OPEN;
				break;
			}
			/* check the upload path existence */
			if(0 != access(pCurrBuf,0))
			{
				logError ("httpRpmUpload: upload path error\n");
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_OPEN;
				break;
			}
			tmpLenth = STRLEN(pCurrBuf);
			if(tmpLenth > 255)
			{
				memcpy (czTmpSrcFilename, pCurrBuf,255);
				czTmpSrcFilename[255] = '\0';
				memcpy (czTmpDstFilename, pCurrBuf,255);
				czTmpDstFilename[255] = '\0';                    
			}
			else
			{
				memcpy (czTmpSrcFilename, pCurrBuf,tmpLenth);
				czTmpSrcFilename[tmpLenth] = '\0';                    
				memcpy (czTmpDstFilename, pCurrBuf,tmpLenth);
				czTmpDstFilename[tmpLenth] = '\0';                     
			}
			strncat(czTmpSrcFilename, czTmpFilename, strlen (czTmpFilename) + 1);
			strncat(czTmpDstFilename, pStrTmp, strlen(pStrTmp)+1);
			strncat(pCurrBuf, pStrTmp, strlen(pStrTmp)+1);
			if (OK != httpFileOpen(czTmpSrcFilename, "wb", &iFile))
			{
				logError ("httpRpmUpload: file open/create error\n");
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_OPEN;
				break;
			}
			fFileOpened = 1;
#ifdef ENABLE_UPLOAD_STATISTICS
			time( &startTime );
#endif /* ENABLE_UPLOAD_STATISTICS */
			/*}*/
		}
		/*
		 * This loop will read data till a boundary is found.
		 * The content read here corresponds to the symbol or
		 * filename determined by the header parse above.
		 */

		while ( (lFileSize > 0) && (lContentLength > 0) )
		{
			lChunkSize = lFileSize;

			if (!flush) lRead = (unsigned long)httpLineRead (reqId,
					pPrevBuf,
					lChunkSize) ;
			else lRead=0;   /* boundary found in previous pass. Stop reading */


			if ( lRead == (unsigned long)HTTP_ERROR)
			{
				logError(" Client hangup");
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_LINEREAD;

				/* don't try to discard the remaining buffer contents */

				lContentLength = 0;
				break;
			}

			/* remove the null char at the end the line added by httpLineRead */
			if ((lRead == lChunkSize) && (pPrevBuf[lRead-1]== (char)0))
				/* If no CRLF found, httpOsalReadLine reads until the
                   end of the buffer and terminates the string so...     */
				lRead--;  /* to suppress the string terminator character */
			else if ((lRead > 1) && (pPrevBuf[lRead-2]== (char)0))
			{
				/*
                If we are here, we know that we did not read the whole chunk,
                so ReadLine encountered a CRLF and replaced it by a NUL.
                The next two lines undo that change.
				 */
				pPrevBuf[lRead-2]= (char)13;
				pPrevBuf[lRead-1]= (char)10;
			}
			/* At this point lRead points to the next writable position */            /* At this point lRead points to the next writable position */

#ifdef DEBUG_INFO
			httpPostPrintFiniteString
			("httpRpmUpload: Here it is: ", pPrevBuf, lRead);
#endif
			lContentLength -= lRead ;

			/*
   swap buffers now.  pCurrBuf points now to the data that we just read,
   and we'll look for the boundary in pPrevBuf
			 */
			pSwapBuf = pPrevBuf;
			pPrevBuf = pCurrBuf;
			pCurrBuf = pSwapBuf;
			lBytesInPrevBuf = lBytesInCurrBuf;
			lBytesInCurrBuf = lRead;

			logInfo("Searching for boundary string\n");

			/* We need to handle 2 exit conditions from this block of data:

        1. Boundary entirely in current buffer
        2. Boundary divided between current and previous buffer
			 */
			if (!flush) pFoundBoundary = httpPostBoundaryFind ( pCurrBuf,
					szBoundary,
					lBytesInCurrBuf);
			if (flush)
			{
				len = (unsigned long)(pFoundBoundary - pPrevBuf - sCharsPerCRLF);
				lFileSize = 0;
				flush = 0;
			}
			else if ( pFoundBoundary != NULL ) /* full boundary in pCurrBuf   */
					{
				len = lBytesInPrevBuf - (unsigned long)sCharsPerCRLF; /* write all pPrevBuff */
				flush = 1;   /* Stop reading the socket and do one more loop  */
					}            /*   to flush both buffers to the file           */

#if 0 /*
 * httpPostTruncBoundaryFind will always return zero
 * because a MIME boundary is always preceded by CRLF and
 * httpLineRead never passes the CRLF line separator
 */

			else if (!firsttime &&
					(lTruncSize = httpPostTruncBoundaryFind ( pPrevBuf,
							szBoundary,
							lBytesInPrevBuf)))

			{   /* truncated boundary found in pPrevBuf */

				/* look the rest of the boundary at the start of pCurrBuf */
				pRestOfBoundary = httpPostBoundaryFind (pCurrBuf,
						szBoundary + lTruncSize,
						lBoundaryLen - lTruncSize);

				if ( pRestOfBoundary == pCurrBuf )
				{
					/* Bingo. Found rest of boundary. Write up to boundary */
					len = lBytesInPrevBuf - lTruncSize;
					lFileSize = 0;
				}
				else
				{
					/* false alarm.  The rest of the buffer was not there */
					len = lBytesInPrevBuf; /* Write the entire buffer     */
					pFoundBoundary = NULL; /* = 'full boundary not found' */
				}
			}
#endif /* we expect that httpPostTruncBoundaryFind will always return zero */

			else len = lBytesInPrevBuf; /* Boundary not found: keep looping   */
			if (!firsttime)
				/* not the first buffer read */
			{
				if (tagFlag && fFileOpened)
				{
					if ( OK != httpFileWrite (&iFile, pPrevBuf, (sbyte4)len, &bw))
					{
						logError ("httpRpmUpload: file write error\n");
						iPostErrorMask |= HTTP_POST_ERROR_FLAG_WRITE;
						(void) httpFileClose(&iFile);
						(void) httpFileDelete(czTmpSrcFilename, httpFileGetDefaultFs());
						break;
					}
				}
				else
				{
					memcpy(szTagContent + (size_t)ulUploadedBytes,pPrevBuf,len);
				}
				ulUploadedBytes += len;

				logInfoMsg("\nWrote %lu bytes to file \n", len);
			}

			firsttime = 0;

			logInfoMsg("httpRpmUpload:After 1 LineRead lRead%lu", lRead);
			logInfoMsg("httpRpmUpload: ContentLen%lu", lContentLength);
			logInfoMsg("httpRpmUpload: lFileSize%lu",lFileSize);
			logInfoMsg("httpRpmUpload: uploaded%lu", ulUploadedBytes);
		} /* Inner while loop ended: One Block of Content Read*/

		logInfoMsg ("httpRpmUpload: inner loop ended, iPostErrorMask is: %d\n",
				iPostErrorMask);

		if (tagFlag && fFileOpened)
		{
			logInfo("httpRpmUpload: Trying to close file");

			if( OK != httpFileClose (&iFile))
			{
				logError ("httpRpmUpload: file close error\n");
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_CLOSE;
				break;
			}
			if (OK != wmosSemTake (semLockUpload, WAIT_FOREVER_K))
				return ERROR;

			/* httpFileDelete(czTmpDstFilename, ALL_FS);*/				

			/* Check for existence */
			tempLen = strlen(czTmpDstFilename);
			while(0 == access(czTmpDstFilename,0))
			{
				(void)snprintf(czTmpDstFilename+tempLen,(256-tempLen),"(%d)", nameCount++);
			}
			if( OK != rename(czTmpSrcFilename,czTmpDstFilename))
			{
				logError ("httpRpmUpload: upload file name error\n");
				iPostErrorMask |= HTTP_POST_ERROR_FLAG_RENAME;
				(void) wmosSemGive (semLockUpload);
				httpSetEnv(reqId, "UPLOAD_FILE_NAME",czTmpSrcFilename);
				break;
			}
			else
			{
				logInfoMsg("Rename OK from %s ",czTmpSrcFilename);
				logInfoMsg("to %s. \n",czTmpDstFilename);
			}
			(void) wmosSemGive (semLockUpload);
			httpSetEnv(reqId, "UPLOAD_FILE_NAME",czTmpDstFilename);
#ifdef ENABLE_UPLOAD_STATISTICS
			elapsedTime = time(NULL) - startTime;
			(void)sprintf(pPrevBuf, "%d", (ubyte4)elapsedTime); /* safe reuse of pPrevBuf */
			if (NULL == httpSetEnv(reqId, "UPLOAD_TIME_SECS", pPrevBuf))
				logError("httpRpmUpload: increase HTTP_MAX_ARGUMENTS");
#endif /* ENABLE_UPLOAD_STATISTICS */
		}
		else
		{
			szTagContent[ulUploadedBytes] = (char)0;
#ifdef DEBUG_INFO
			logInfoMsg("httpRpmUpload: setting ENV of %s ",szTagTmp);
			logInfoMsg("httpRpmUpload: setting above ENV to %s ",szTagContent);
#endif
			httpSetEnv (reqId, szTagTmp, szTagContent);
		}
		logInfoMsg( "httpRpmUpload: New Header ContentLength %lu\n",
				lContentLength);

		/* stop processing on any error */

		if (iPostErrorMask != HTTP_OK)
		{
			logErrorMsg ("httpRpmUpload: break loop, iPostErrorMask is: %d\n",
					iPostErrorMask);
			break;
		}
	}  /* end of outer while loop */

	HTTP_POST_REQUEST_PARSE_MEM_FREE:
	if ( ( (pPrevBuf != NULL) && ((memPoolFree( partitionId, pPrevBuf )) == HTTP_ERROR) ) ||
			( (pCurrBuf != NULL) && ((memPoolFree( partitionId, pCurrBuf)) == HTTP_ERROR) ) ||
			( (szTagTmp != NULL) && ((memPoolFree( partitionId, szTagTmp )) == HTTP_ERROR) ) ||
			( (szTagContent != NULL) && ((memPoolFree( partitionId, szTagContent )) == HTTP_ERROR) ) )
	{
		logError("httpRpmUpload: buffer memory - deallocation failed");
		iPostErrorMask |= HTTP_POST_ERROR_FLAG_MEM_FREE;
	}

	if ( (lContentLength > 0) && (iPostErrorMask) )
	{
		if (httpMultipartFooterDiscard (reqId, (long)lContentLength) == HTTP_ERROR)
		{
			iPostErrorMask |= HTTP_POST_ERROR_FLAG_CONTENT_DISCARD;
		}
	}

	if (iPostErrorMask != HTTP_OK)
	{
		httpPostRequestErrorHandler (reqId, &iPostErrorMask);
		return RPM_ERROR;
	}
	/* if WMB backplane or another post handler RPM is enabled it's time to leave */
	if (lclUploadRpmNotLast) return RPM_OK;

#ifdef WEBCLI_STANDALONE_WEBSERVER
	reqId->sMethod = HTTP_M_GET;
	httpUploadReplyNoWmb(reqId);
	/*note: user should provide a clean httpPostReply without backplane and so. */
	/*     return RPM_DONE; */
#endif

	(void)(*phttpPostReply) (reqId, HTTP_OK);

	return RPM_DONE;
}



/***************************************************************************
 *
 * assignSdoMsgQID - Assign message queue for sending the notification
 * 				   and file name to user space SDO task.
 *
 * RETURNS: void
 */



void assignSdoMsgQID()
{
	SDO_MSG_Q_ID = msgQOpen(SDO_MSG_Q_NAME, 0, 0, 0, 0, 0);

	if(MSG_Q_ID_NULL == SDO_MSG_Q_ID)
	{
		printf("\n### httpRpmupload : SDO_MSG_Q_ID RETURNED NULL ###\n");
	}

}

/***************************************************************************
 *
 * getUploadedFileNames - get the HTTPS uploaded file names and validate
 * 						 the firmware are proper.

 *
 * RETURNS: (char *) proper upgrade firmware file names.
 */



char * getUploadedFileNames() 
{
	char fileBuffer[FILE_NAME_MAX_SIZE] = { 0x00 };
	INT32 fwFileNameRead = 1;
	UINT32 fileListError = 0;
	int fd = 0;
	FILE *fp;
	STATUS httpFileReadStatus;
	static Message fwFileNameMsg;

	/* change the default directory to /ram0 */

	httpFileReadStatus = cd("/ram0/");

	/* Check Whether USB is Opened */
	if (httpFileReadStatus == ERROR) 
	{
		return NULL;
	}

	/* Open Cache File to store Firmware Upgrade File List */
	fp = fopen( CACHE_FILE, RDWR_CREAT);
	if (fp == NULL) 
	{
		return NULL;
	}

	/* create File descriptor from File pointer */
	fd = fileno(fp);

	/* generate a long listing of directory contents */

	/* Read SC File names */

	httpFileReadStatus = dirList(fd, AC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (httpFileReadStatus == ERROR)
	{
		fileListError += 1;
	}

	/* Read PC File names */

	httpFileReadStatus = dirList(fd, PC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (httpFileReadStatus == ERROR)
	{
		fileListError += 1;
	}

	/* Read AC File names */
	httpFileReadStatus = dirList(fd, SC_FILE_NAME_FORMAT, FALSE, FALSE);
	if (httpFileReadStatus == ERROR) 
	{
		fileListError += 1;
	}

	/* Check Error Occurred While Listing File Names */
	if (fileListError == FILE_LIST_ERROR) 
	{
		/* close and remove cache file */
		fclose(fp);
		remove(CACHE_FILE);
		return NULL;
	}

	/* seek to start of the file */
	fseek(fp, ZERO_BYTES, SEEK_SET);
	memset(fwFileNameMsg.Buffer, '\0', sizeof(fwFileNameMsg.Buffer));

	/* Read the Firmware upgrade file names from cache file */
	while (fwFileNameRead > ZERO_BYTES) 
	{
		/* Clear the buffer before storing */
		memset(fileBuffer, '\0', sizeof(char) * FILE_NAME_MAX_SIZE);

		/* Read the file names from cache file */
		fwFileNameRead = fscanf(fp, "%s", fileBuffer);

		/* Check SC file name and file count */
		if (strncmp(fileBuffer, SC_FILE_HEADER, strlen(SC_FILE_HEADER)) == 0)
		{
			/* Append to SC File name Buffer */
			strcat(fwFileNameMsg.Buffer,fileBuffer);
			strcat(fwFileNameMsg.Buffer,",");

		}
		/* Check PC file name and file count */
		if (strncmp(fileBuffer, PC_FILE_HEADER, strlen(PC_FILE_HEADER)) == 0) 
		{
			/* Append to PC File name Buffer */
			strcat(fwFileNameMsg.Buffer,fileBuffer);
			strcat(fwFileNameMsg.Buffer,",");
		}
		/* Check AC file name and file count */
		if (strncmp(fileBuffer, AC_FILE_HEADER, strlen(AC_FILE_HEADER)) == 0) 
		{
			/* Append to AC File name Buffer */
			strcat(fwFileNameMsg.Buffer,fileBuffer);
			strcat(fwFileNameMsg.Buffer,",");
		}
	}

	/* close and remove cache file */
	fclose(fp);
	remove(CACHE_FILE);
	return (char*)fwFileNameMsg.Buffer;
}

/***************************************************************************
 *
 * httpRpmUpload - process form data sent by POST method
 *
 * This routine handles the content sent by a form that uses POST method
 * with media type (ENCTYPE) of multipart/form-data. This routine ignores other
 * media types and does not process them.
 * Using a tag, for eg: <WINDWEB_AUTO FUNC=TextField TYPE=text SYMBOL=szString>
 * produces an HTTP response by the client similar to
 *
 * -------0732123123
 * content-dispostion: form-data; name=szString
 *
 * <textfield data appears here>
 * -------0732123123
 *
 * The number  -------0732123123 is the boundary.
 * This routine parses the multipart form data and sets the environment variables
 * with name of SYMBOL if the TYPE is textarea, textfield, checkbox,
 * radiobutton or hidden field.
 * If TYPE is "file", it opens the file  (filename sent as part of
 * content-disposition) and writes the data to the file.
 *
 * NOTE: File upload is possible only if the file system  is a read-write
 * file system.
 * 
 *
 * RETURNS: RPM_DONE, RPM_OK, or RPM_ERROR
 */


short httpRpmUpload
(
		HTTP_REQ_ID     reqId    /* pointer to request structure */
)
{
	short ret;
	Message respMsg;
	char *srcBuffer = NULL;

	/* clear the ram0 folder*/
	xdelete("/ram0/*.bin");


	printf("\n######### HTTP UPLOAD ###########\n");

	/* backwards compatible */
	ret = lclHttpRpmUpload(reqId, uploadRpmNotLast);

	if ((ret == RPM_OK) || (ret == RPM_DONE))
	{
		printf("\n######### HTTP UPLOAD SUCCESS ###########\n");

		memset(respMsg.Buffer, 0x00, sizeof(respMsg.Buffer));

		/* get the firmware names */
		srcBuffer = (char *) getUploadedFileNames();

		if ((strlen(srcBuffer)!= 0) && (srcBuffer != NULL))
		{
			strcpy(respMsg.Buffer, srcBuffer);
			respMsg.msgID = TO_SDO_FW_ETHRNT_UPLOAD_RESP;

			if(MSG_Q_ID_NULL == SDO_MSG_Q_ID)
			{
				assignSdoMsgQID();
			}

			if(msgQSend(SDO_MSG_Q_ID,(char*)(&respMsg),sizeof(respMsg),NO_WAIT,MSG_PRI_NORMAL) != OK)
			{
				printf("\n#### httpRpmUpload : Upload Response message sent failed to SDO Task #### \n");
			}
			else
			{
				printf("\n#### HTTP UPLOAD NOTIFICATION SENT TO SDO Task ####\n");
				printf("NORIFY ID : %d \n", respMsg.msgID);
				printf("NORIFY MSG : %s \n", respMsg.Buffer);
			}
		}
		else
		{
			printf("\n#### UPLOAD_ERR : uploaded file is invalid ####\n");
		}

	}
	else
	{
		printf("\n######### HTTP UPLOAD FAILURE ###########\n");
	}

	return ret; /* This is not last RPM */
}

/***************************************************************************
 *
 * httpRpmUploadLast - 
 *
 * NOMANUAL
 *
 */

short httpRpmUploadLast
(
		HTTP_REQ_ID     reqId    /* pointer to request structure */
)
{
	return( lclHttpRpmUpload(reqId, FALSE) );
}

/***************************************************************************
 *
 * httpPostMultipartHeaderParse - parse the multipart header sent by POST method
 *
 * This routine parses the multipart header and determines the symbol name or
 * filename of the content being sent
 *
 * RETURNS: HTTP_OK or HTTP_ERROR, if header format is incorrect.
 */

HTTP_STATUS httpPostMultipartHeaderParse
(
		HTTP_REQ_ID reqId,            /* Handle of the active request,   */
		long *      lBytesRemaining,  /* Number of bytes in socket       */
		short *     sCharsPerCRLF,    /* Number of chars needed for CRLF */
		int * tagFlag,
		char * tagValue
)
{
	/*const char * szBoundary;*/
	char *       szTmp;
	HTTP_BOOL    fFoundCRLF = HTTP_FALSE;
	long         lRead = 0;
	int flag = 1;

	tagValue[0] = (char)0;

	if ((szTmp = memPoolAlloc(httpReqMemPartIdGet(reqId), httpMaxTempBufSize ))
			== NULL)
		return (HTTP_ERROR);

	/* szBoundary = httpBoundaryGet (reqId);*/

	/* read (and determine symbol or filename) until the next CR/LF */

	logInfo( "httpPostMultipartHeaderParse: entered.\n");
	while ((*lBytesRemaining > 0) && (!fFoundCRLF))
	{
		lRead = httpLineRead (reqId, szTmp, httpMaxTempBufSize);
#ifdef DEBUG_INFO
		logInfoMsg( "httpPostMultipartHeaderParse: Calling Header parse %ld\n", lRead);
#endif
		if (( flag == 1) && (lRead > 10))
		{

			/*
			 * Parsing the content disposition header only on the first buffer read
			 * The code does not support content disposition headers that span across
			 * multiple httpLineReads
			 */

			flag = 0;
			if(HTTP_OK !=
					( httpPostContentDispositionParse(szTmp, tagFlag, tagValue)))
			{
				*lBytesRemaining -= lRead;
				return HTTP_ERROR;
			}

		}
#ifdef DEBUG_INFO
		httpPostPrintFiniteString("httpPostMultipartHeaderParse: Header- ",
				szTmp, lRead);
#endif

		if (lRead<0)
			return (HTTP_ERROR);
		fFoundCRLF = (szTmp[0]==0);
		*lBytesRemaining -= lRead;
	}
	if (fFoundCRLF)
	{
		*sCharsPerCRLF = (short) lRead;
#ifdef DEBUG_INFO
		logInfo(" CRLF found \n");
		logInfoMsg("httpPostMultipartHeaderParse: TagValue %s\n", tagValue);
#endif
		if((memPoolFree( httpReqMemPartIdGet(reqId), szTmp )) == HTTP_ERROR)
			return (HTTP_ERROR);
		return (HTTP_OK);
	}
	else
		return (HTTP_ERROR);
}

/***************************************************************************
 *
 * httpPostPrintFiniteString - dump a sub-string of finite length to the debug output
 *
 * This routine prints the data in <msg> buffer upto the length <len> specified.
 * This is a debug routine.
 * NOMANUAL
 *
 * RETURNS: none
 */
#ifdef DEBUG_INFO
LOCAL void  httpPostPrintFiniteString
(
		char * msg, /*Text to be output*/
		char * buf, /* buffer to be dumped*/
		int len     /* Length of buffer <buf> that has to be dumped */
)
{
	int sIndex = 0;
	char prntChar; /* printable character */
	if (len < 0)
		return;
	printf("%s", msg);
	for (sIndex = 0; sIndex < len; sIndex ++)
	{
		if (10 == (prntChar = *buf)) prntChar = 'L';
		if (13 == prntChar) prntChar = 'C';
		if (33 > prntChar) prntChar = '.';
		printf("%c", prntChar);
		buf++;
	}
	printf("\n");
}

#endif

/***************************************************************************
 *
 * httpPostContentDispositionParse - parse the content-disposition line of header
 *
 * This routine parses the content disposition line of the header and determines
 * filename or symbol name.
 *
 * NOMANUAL
 *
 * RETURNS: HTTP_OK,
 * or HTTP_ERROR, if the content-disposition line is of incorrect format
 */

LOCAL HTTP_STATUS  httpPostContentDispositionParse
(
		char * buf,
		int * flag,
		char * tagValue
)
{

	char * bufStart = NULL;
	char * bufEnd = NULL;
	long tagLen = 0;
	char * fileMark = NULL;


	if((bufStart = strstr( buf, "filename=")) != NULL)
	{
		bufEnd = strrchr ( bufStart, '"');
		if (( bufStart = strchr (bufStart, '"')) != NULL)
		{
			bufStart++;
			tagLen = bufEnd - bufStart;
		}
		if( tagLen > 0)
		{
			if (( fileMark = strrchr (bufStart, '/')) != NULL)
				bufStart = ++fileMark;

			if  (( fileMark = strrchr (bufStart, '\\')) != NULL)
				bufStart = ++fileMark;
			tagLen = bufEnd - bufStart;
			*flag = 1;
			logInfo("httpPostContentDispositionParse: In header Parse: filename found \n");
		}
		else   /* truncated filename */
			return HTTP_ERROR;

	}
	else if ((bufStart = strstr( buf, " name="))!= NULL)
	{
		bufEnd = strrchr ( bufStart, '"');
		if (( bufStart = strchr (bufStart, '"')) != NULL)
		{
			bufStart++;
			tagLen = bufEnd - bufStart;
			*flag = 0;
		}
		if ( tagLen <= 0) return HTTP_ERROR;
	}
	else
		return HTTP_ERROR;

	if ( tagLen > 0 )
	{
		tagLen = wmMin(tagLen, (HTTP_SYMBOL_NAME_SIZE_MAX-1));
		strncpy(tagValue,bufStart,(size_t)tagLen);
	}
	*(tagValue + tagLen ) = (char)0;
	logInfoMsg("httpPostContentDispositionParse: TagValue in header Parse %s\n", tagValue);
	return HTTP_OK;
}

/***************************************************************************
 *
 * httpPostRequestErrorHandler - send an appropriate error message
 *
 * This routine sends an error page to the client based on the error mask
 * which is passed in.
 *
 * NOMANUAL
 *
 * RETURNS: HTTP_OK, or HTTP_ERROR, if httpError() or httpStatusSet() fail.
 */

LOCAL HTTP_STATUS httpPostRequestErrorHandler
(
		HTTP_REQ_ID reqId,         /* the current request connection          */
		int * errorMask            /* the integer containing the error status */
)
{
	if ((reqId != NULL) && (errorMask != HTTP_OK))
	{
		if (*errorMask & HTTP_POST_ERROR_FLAG_OPEN)
		{
			httpStatusSet (reqId, HTTP_FORBIDDEN);
			return httpError(reqId, HTTP_POST_ERROR_MESG_OPEN);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_CLOSE)
		{
			httpStatusSet (reqId, HTTP_FORBIDDEN);
			return httpError(reqId, HTTP_POST_ERROR_MESG_CLOSE);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_RENAME)
		{
			httpStatusSet (reqId, HTTP_FORBIDDEN);
			return httpError(reqId, HTTP_POST_ERROR_MESG_RENAME);
		}    
		if (*errorMask & HTTP_POST_ERROR_FLAG_WRITE)
		{
			httpStatusSet (reqId, HTTP_FORBIDDEN);
			return httpError(reqId, HTTP_POST_ERROR_MESG_WRITE);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_LINEREAD)
		{
			httpStatusSet (reqId, HTTP_REQ_TIME_OUT);
			return httpError(reqId, HTTP_POST_ERROR_MESG_LINEREAD);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_NOT_ENOUGH_SPACE_ON_DEVICE)
		{
			httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
			return httpError (reqId, HTTP_POST_ERROR_MESG_NOT_ENOUGH_SPACE_ON_DEVICE);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_OUT_OF_MEMORY)
		{
			httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
			return httpError (reqId, HTTP_POST_ERROR_MESG_OUT_OF_MEMORY);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_MEM_FREE)
		{
			httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
			return httpError(reqId, HTTP_POST_ERROR_MESG_MEM_FREE);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_CONTENT_DISCARD)
		{
			httpStatusSet (reqId, HTTP_INTERNAL_ERROR);
			return httpError(reqId, HTTP_POST_ERROR_MESG_CONTENT_DISCARD);
		}
		if (*errorMask & HTTP_POST_ERROR_FLAG_MPART_HEADER_PARSE)
		{
			httpStatusSet (reqId, HTTP_BAD_REQUEST);
			return httpError(reqId, HTTP_POST_ERROR_MESG_MPART_HEADER_PARSE);
		}
	}
	return HTTP_ERROR;
}

/***************************************************************************
 *
 * httpPostUploadPathGet - retrieve the upload path for this URL
 *
 * This will search the list of registered upload paths until it finds
 * the one that matches the current URL
 * which is passed in.
 *
 * NOMANUAL
 *
 * RETURNS: HTTP_OK if the path string has been copied to szUploadPath
 *          or HTTP_ERROR
 */

LOCAL HTTP_STATUS httpPostUploadPathGet
(
		HTTP_REQ_ID   reqId,
		sbyte *       szUploadPath,
		unsigned long sizeBuffSize
)
{
	HTTP_GEN_LIST_ID        auxListId = NULL;
	HTTP_GEN_LIST_ENTRY_ID  configId =  NULL;
	sbyte *                 szRoot =    NULL;
	HTTP_STATUS             status =    HTTP_ERROR;

	auxListId = (mudConnSSLVerify (reqId->ioHandle)) ?
			httpSecureUpldConfigId :
			httpUpldConfigId;

	if (NULL == (configId = httpGenListFind (auxListId,
			httpRpmIdentGet (reqId))))
	{
		logInfo( "httpPostUploadPathGet: FS Root configId NULL \n");
	}
	else
	{
		if ((szRoot=httpGenListDataGet (configId)) != NULL)
			if (sizeBuffSize > strlen(szRoot))
			{
				strncpy ( szUploadPath, szRoot,(sizeBuffSize-1));
				logInfoMsg("httpPostUploadPathGet: File to upload to: %s\n",
						szUploadPath);
				status = HTTP_OK;
			}
			else
				logError("httpPostUploadPathGet: Path and/or filename too long \n");
		else
		{
			logError("httpPostUploadPathGet: Path and/or filename not defined \n");
		}
	}
	return status;
}

#ifdef HTTP_SHOW
/***************************************************************************
 *
 * httpUploadConfigurationShow - displays HTTP Upload Configuration
 *
 * The Upload configuration list is indexed by URL prefixes and each
 * entry's data is the corresponding internally resolvable directory path.
 *
 * RETURNS: NA
 *
 * NOMANUAL
 */
void httpUploadConfigurationShow
(
		void
)
{
	httpListShow (httpUpldConfigId,
			"httpUploadConfiguration",
			FALSE,    /* entry id is a string */
			FALSE,    /* entry data is a string */
			NULL      /* left margin is not required */
	);

	return;
} /* httpUploadConfigurationShow */

/***************************************************************************
 *
 * httpSecureUploadConfigurationShow - displays HTTP secure Upload Configuration
 *
 * The secure Upload configuration list is indexed by URL prefixes and each
 * entry's data is the corresponding internally resolvable directory path.
 *
 * RETURNS: NA
 *
 * NOMANUAL
 */
void httpSecureUploadConfigurationShow
(
		void
)
{
	httpListShow (httpSecureUpldConfigId,
			"httpSecureUploadConfiguration",
			FALSE,    /* entry id is a string */
			FALSE,    /* entry data is a string */
			NULL      /* left margin is not required */
	);

	return;
} /* httpSecureUploadConfigurationShow */
#endif /* HTTP_SHOW */
#endif /* INCLUDE_HTTP_UPLOAD */
#endif

