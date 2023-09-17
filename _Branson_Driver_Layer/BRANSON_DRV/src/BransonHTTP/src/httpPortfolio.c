/* httpPortfolio.c - read portfolio files that can change server configuration */

/*
 * Copyright (c) 2002-2007, 2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
17dec10,m_z  part of APIs changed
             CQID: Fix WIND00247096
26jan10,h_y  support for LP64
23feb07,msa  Correct the apigen comments so they match the actual
             routines.
27may04,adb  cleaned up 
19may04,adb  added run time input file system discrimination
30apr04,vmk  added customTimeGet argument to match httpFileRegister (spr#92408)
13apr04,adb  added httpRequestPortfolioRead and httpTrustedPortfolioRead
17jan01,jc   created
*/

/*

DESCRIPTION

This module contains the functions that interpret a portfolio.  A portfolio
is an XML file with that contains server configuration information.  The
Wind River Portfolio Manager will notify the server in runtime when new
portfolio files are available.

*/

#include <ctype.h>
#include "httpLib.h"
#include "httpReq.h"
#include "httpRpms.h"
#include <string.h>

/* typedefs */
typedef struct SPECIAL_FILE_tag
    {
    sbyte *  filename;
    sbyte *  content_type;
    BOOL  uncompress;
    } SPECIAL_FILE;

typedef enum ELEM_tag
    {
    HTTP_CONFIG_K = 0,
    CONTENT_DIRECTORY_K,
    URL_SUBSTRING_K,
    MOUNTPOINT_K,
    SPECIAL_FILE_K,
    UNKNOWN_ELEM_K
    } ELEM;

typedef enum ATTR_tag
    {
    UNCOMPRESS_K = 0,
    CONTENT_TYPE_K,
    UNKNOWN_ATTR_K
    } ATTR;

typedef struct ELEM_DESCRIPTOR_tag
    {
    ELEM elem;
    sbyte * elemString;
    } ELEM_DESCRIPTOR;

typedef struct ATTR_DESCRIPTOR_tag
    {
    ATTR      attr;
    sbyte *   attrString;
    } ATTR_DESCRIPTOR;

typedef struct CONTENT_DIRECTORY_tag
    {
    ELEM            currentElem;
    sbyte *         url_substring;
    sbyte *         mountpoint;
    SPECIAL_FILE    special_file;
    } CONTENT_DIRECTORY;

/* locals */
LOCAL CONTENT_DIRECTORY * pCntntDir = NULL;
LOCAL const ELEM_DESCRIPTOR supportedElems[] =
    {
    { HTTP_CONFIG_K,        "http_config"       },
    { CONTENT_DIRECTORY_K,  "content_directory" },
    { URL_SUBSTRING_K,      "url_substring"     },
    { MOUNTPOINT_K,         "mountpoint"        },
    { SPECIAL_FILE_K,       "special_file"      },
    { UNKNOWN_ELEM_K,       ""                  }
    };

LOCAL const ATTR_DESCRIPTOR supportedAttrs[] =
    {
    { UNCOMPRESS_K,         "uncompress"        },
    { CONTENT_TYPE_K,       "content_type"      },
    { UNKNOWN_ATTR_K,       ""                  }
    };
MEM_POOL_ID g_memPool = NULL;


/* prototypes */

LOCAL void portflStartDocument (XML_DOC_ENV *pXmlEnv);
LOCAL void portflEndDocument (XML_DOC_ENV *pXmlEnv);
LOCAL STATUS portflStartElement (XML_DOC_ENV *pXmlEnv, sbyte *pTag, ubyte2 numAttributes);
LOCAL STATUS portflEndElement (XML_DOC_ENV *pXmlEnv, sbyte *pTag);
LOCAL STATUS portflCharacters (XML_DOC_ENV *pXmlEnv, sbyte *pChar, size_t length);
LOCAL STATUS portflIgnorableWhitespace (XML_DOC_ENV *pXmlEnv, sbyte *pSpace, size_t length);
LOCAL STATUS portflProcessingInstruction (XML_DOC_ENV *pXmlEnv, sbyte *pTarget, sbyte *pData);
LOCAL STATUS portflHandler (XML_DOC_ENV *pXmlEnv, SAX_FUNCTION_ID functionID,  sbyte *pData);
LOCAL sbyte* killTrail (sbyte*);

STATUS httpPortfolioRead
    (
    sbyte * fileName    /* pointer to file name */
    )
    {
    return (httpTrustedPortfolioRead (fileName, FALSE));
    } /* httpPortfolioRead */

STATUS httpRequestPortfolioRead
    (
    HTTP_REQ_ID reqId,
    sbyte *     fileName    /* pointer to file name */
    )
    {
    STATUS  rs = ERROR;
    char *  pChar;

    if  (NULL == (pChar = wmMalloc ((ubyte4)httpMaxTempBufSize)))
        {
        return (ERROR);
        }

    if  (HTTP_OK != httpFsRootGet (reqId, 
                                   pChar,
                                   httpMaxTempBufSize))

        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpRequestPortfolioRead: unknown GET and custom shared FS \n",
                0, 0, 0, 0, 0, 0);
#endif
        wmFree (pChar);
        return (ERROR);
        }

    strncat (pChar,
             fileName,
             httpMaxTempBufSize - strlen (pChar) - 1);

    rs = mudConnSSLVerify (reqId->ioHandle) ?
         httpTrustedPortfolioRead (pChar, TRUE) :
         httpTrustedPortfolioRead (pChar, FALSE);

    wmFree (pChar);

    return (rs);

    } /* httpRequestPortfolioRead */

STATUS httpTrustedPortfolioRead
    (
    sbyte * fileName,   /* pointer to file name */
    BOOL    useSecureFS /* if TRUE then use secure file system */
    )
    {
    XML_DOC_ENV xmlEnv;
    generic_fd_t f;
    sbyte * pPortfolio;
    unsigned long bufSize, fileSize;
    ubyte4 tempbufSize;
    STATUS rs = ERROR;

    xmlEnv.useSecureFS = useSecureFS;

    g_memPool = httpGlobalMemPartIdGet();

    if  ((NULL == fileName) || '\0' == (*fileName)) 
        {
        return (wmError (ERROR_GENERAL, "invalid portfolio name input"));
        }

    /* open file */
    if  (OK != httpFileOpen (fileName, "rb", &f))
        {
        return (wmError (ERROR_GENERAL, "unable to open portfolio file"));
        }

    fileSize = (unsigned long)httpFileSize(&f);
    bufSize = fileSize +1;

    /* allocate buffer */

    if  (NULL == (pPortfolio = httpConservativeAlloc (g_memPool, (size_t*)&bufSize)))
        {
        goto EXIT_LABEL;
        }

    if  (bufSize < fileSize)
        {
        wmError (ERROR_MEMMGR_NO_MEMORY, "portfolio file too large");

        goto EXIT_LABEL;
        }
     
    tempbufSize = (ubyte4)fileSize;
    if  (OK != httpFileRead (&f, pPortfolio, (ubyte4)bufSize, &tempbufSize))
        {
        wmError (ERROR_GENERAL, "portfolio httpFileRead failed");

        goto EXIT_LABEL;
        }
    pPortfolio[tempbufSize] = '\0';

    /* call parser with callback function */
    rs = sax_startParser (&xmlEnv, portflHandler, pPortfolio, fileSize);

EXIT_LABEL:

    if  (NULL != pPortfolio)
        {
        (void)memPoolFree (g_memPool, pPortfolio);
        }

    /* close file */
    (void)httpFileClose(&f);
    
    return (rs);
    } /* httpRequestPortfolioRead */

/*
 *  portflHandler
 *
 *  DESCRIPTION:
 *      Entry point by the event driven parser. Main distribution point.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *      functionID  - Function to dispatch.
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflHandler
    (
    XML_DOC_ENV     *pXmlEnv,    /* [in] Pointer to the XML_DOC_ENV structure */
    SAX_FUNCTION_ID functionID,  /* [in] Function ID to dispatch. */
    sbyte           *pData       /* [in] Received data if any     */
    )
    {
    STATUS      s = OK;
    sbyte       *pTarget = NULL;
    sbyte       *pBuffer = NULL;
    ubyte2      numAttributes;

    /* Save the XML_DOC_ENV structure for use by all functions  */
    switch (functionID)
        {
        case kSAX_START_DOCUMENT:
            portflStartDocument(pXmlEnv);
            break;

        case kSAX_END_DOCUMENT:
            portflEndDocument(pXmlEnv);
            break;

        case kSAX_START_ELEMENT:
            (void) sax_getAttributeCount(pXmlEnv, pData, &numAttributes);
            s = portflStartElement(pXmlEnv, pData, numAttributes);
            break;

        case kSAX_END_ELEMENT:
            s = portflEndElement(pXmlEnv, pData);
            break;

        case kSAX_CHARACTERS:
            if (NULL != pData)
                s = portflCharacters(pXmlEnv, pData, STRLEN(pData));
            break;

        case kSAX_IGNORABLE_WHITESPACE:
            if (NULL != pData)
                s = portflIgnorableWhitespace(pXmlEnv, pData, STRLEN(pData));
            break;

        case kSAX_PROCESSING_INSTRUCTION:
            s = portflProcessingInstruction(pXmlEnv, pTarget, pBuffer);
            break;

        default:
            break;
        }

    return s;
    }


/*
 **- portflStartDocument
 *
 *  DESCRIPTION:
 *      The first notification from the parser. Initialize internal data.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *
 *  RETURNS:
 *
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL void portflStartDocument
    (
    XML_DOC_ENV     *pXmlEnv    /* [in] Pointer to the XML_DOC_ENV structure */
    )
    {
    logInfo ("Portfolio opened");
    }/* portflStartDocument */

/*
 **- portflEndDocument
 *
 *  DESCRIPTION:
 *      The last function called by the parser.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *
 *  RETURNS:
 *
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL void portflEndDocument
    (
    XML_DOC_ENV     *pXmlEnv    /* [in] Pointer to the XML_DOC_ENV structure */
    )
    {
    logInfo ("Portfolio closed");
    }/* portflEndDocument */

/*
 **- portflStartElement
 *
 *  DESCRIPTION:
 *      A new element is read.
 *  PARAMETERS:
 *      pXmlEnv         - Pointer to the XML_DOC_ENV structure
 *      szTag           - Name of the element
 *      numAttributes   - Number of attributes.
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflStartElement
    (
    XML_DOC_ENV *pXmlEnv,       /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte       *pTag,          /* [in] Name of the element */
    ubyte2      numAttributes   /* [in] Number of attributes */
    )
    {
    ubyte2 i = 0;
    STATUS s = OK;
    sbyte * szName;
    sbyte * szValue;

    logInfoMsg ("Element read %s", pTag);
    while (strcmp(pTag, supportedElems[i].elemString))
        if (UNKNOWN_ELEM_K == supportedElems[++i].elem) break;

    switch (supportedElems[i].elem)
        {
        case HTTP_CONFIG_K:
            break;
        case CONTENT_DIRECTORY_K:
            pCntntDir = memPoolAlloc (g_memPool,
                                              sizeof(CONTENT_DIRECTORY));
            if (NULL == pCntntDir)
                return wmError (ERROR_MEMMGR_NO_MEMORY, "out of memory");
            break;
        case URL_SUBSTRING_K:
            if (NULL != pCntntDir)
                pCntntDir->currentElem = URL_SUBSTRING_K;
            break;
        case MOUNTPOINT_K:
            if (NULL != pCntntDir)
                pCntntDir->currentElem = MOUNTPOINT_K;
            break;
        case SPECIAL_FILE_K:
            if (NULL != pCntntDir)
                pCntntDir->currentElem = SPECIAL_FILE_K;
            break;
        case UNKNOWN_ELEM_K:
            return wmError(ERROR_GENERAL, "unknow config command");
            break;
        };
    /* at this time, only special_file elements has attributes */
    if (SPECIAL_FILE_K != supportedElems[i].elem) return s;
    /* Let's get the attribute values associated with this element  */
    for (i = 0; i < numAttributes; i++)
    {
        s = sax_getAttribute(pXmlEnv, i, &szName, &szValue);
        logInfoMsg ("Attributes read %s", szName);
        logInfoMsg (" = %s ", szValue);
        switch (supportedAttrs[i].attr)
            {
            case UNCOMPRESS_K:
                if (NULL != pCntntDir)
                {
                  if ( !(strcmp("true", szValue) && strcmp("TRUE", szValue)) )
                    pCntntDir->special_file.uncompress = TRUE;
                  else 
                    pCntntDir->special_file.uncompress = FALSE;
                }
                (void)memPoolFree(g_memPool, szValue);
                szValue = NULL;
                break;
            case CONTENT_TYPE_K:
                if ((NULL != pCntntDir)&& (NULL != szValue))
                  pCntntDir->special_file.content_type = szValue;
                /* content-type is not freed for the lifetime of the server !     */
                /* a better way to do it is to maintain a dynamic list of content
                   types, allocate only those that are not on the list            */
                break;
            case UNKNOWN_ATTR_K:
                (void)memPoolFree(g_memPool, szValue);
                szValue = NULL;                
                return wmError(ERROR_GENERAL, "unknown config element");
                break;
            };
        (void)memPoolFree(g_memPool, szName);
        if (OK != s)
            break;
    }
    return s;

    }/* portflStartElement */

/*
 **- portflEndElement
 *
 *  DESCRIPTION:
 *      Process the end of the element notice. Tell DOM handler to add node to the
 *      tree.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *      szTag       - Name of the element
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflEndElement
    (
    XML_DOC_ENV *pXmlEnv,   /* [in] Pointer to an XML_DOC_ENV structure */
    sbyte       *pTag       /* [in] Name of the element */
    )
    {
    sbyte4    fs;
    ubyte2    i = 0;
    STATUS    s = OK;
    size_t    fnameLen = 0;
    sbyte *   pFullFileName;
    sbyte *   cntntType;
    sbyte *   pTemp;
    while (strcmp(pTag, supportedElems[i].elemString))
        if (UNKNOWN_ELEM_K == supportedElems[++i].elem) break;

    switch (supportedElems[i].elem)
        {
        case HTTP_CONFIG_K:
            break;
        case CONTENT_DIRECTORY_K:
            logInfoMsg("registered URL %s", pCntntDir->url_substring);
            httpRpmConfAdd(HTTP_M_GET, pCntntDir->url_substring, httpRpmFs);

            if  (pXmlEnv->useSecureFS)
                {
                httpSecureFsConfAdd (pCntntDir->url_substring, 
                                     pCntntDir->mountpoint);
                }
            else
                {
                httpFsConfAdd (pCntntDir->url_substring, 
                               pCntntDir->mountpoint);
                }

            break;
        case URL_SUBSTRING_K:
            break;
        case MOUNTPOINT_K:
            break;
        case SPECIAL_FILE_K:
            pTemp = killTrail(pCntntDir->mountpoint);
            if(NULL != pTemp)
              fnameLen = strlen(pTemp);
            pTemp = killTrail(pCntntDir->special_file.filename);
            if(NULL != pTemp)
              fnameLen += strlen(pTemp);
            pFullFileName = memPoolAlloc(g_memPool, fnameLen+1);
            if (NULL == pFullFileName)
                return wmError(ERROR_MEMMGR_NO_MEMORY, "no memory");
            strcpy(pFullFileName, pCntntDir->mountpoint);
            strcat(pFullFileName, pCntntDir->special_file.filename);
            logInfoMsg("registered file %s", pFullFileName);
            if (pCntntDir->special_file.uncompress) fs = CMP_ANSI_FS;
            else fs = ANSI_FS;
            cntntType = pCntntDir->special_file.content_type;
            httpFileRegister (pFullFileName, fs, cntntType,
                              NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            /* free the filename because it has been copied to pFullFileName */
           (void) memPoolFree(g_memPool, pCntntDir->special_file.filename);
            /* do not free pFullFileName nor pCntnDir->url_substring for the life of the server */
            break;
        case UNKNOWN_ELEM_K:
            break;
        };
    return s;

    }/* portflEndElement */

/*
 **- portflCharacters
 *
 *  DESCRIPTION:
 *      Process the character string read. This is the contents of an element.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *      pChar   - Pointer to the character string
 *      length  - Number of characters read. This is UNICODE encoded.
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflCharacters
    (
    XML_DOC_ENV *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte       *pChar,     /* [in] Pointer to the character string */
    size_t      length      /* [in] Number of characters            */
    )
    {
    STATUS        s = OK;
    logInfoMsg ("Chars read %s", pChar);
    /* at this time, we only process elements under content_directory */
    if (NULL == pCntntDir) return OK;
    switch (pCntntDir->currentElem)
        {
        case URL_SUBSTRING_K:
            pCntntDir->url_substring = killTrail(pChar);
            /* pChar not freed for the lifetime of the server */
            break;
        case MOUNTPOINT_K:
            pCntntDir->mountpoint = killTrail(pChar);
            /* pChar not freed for the lifetime of the server */
            break;
        case SPECIAL_FILE_K:
            pCntntDir->special_file.filename = killTrail(pChar);
            /* pChar not freed for the lifetime of the server */
            break;
        case UNKNOWN_ELEM_K:
        default:
            return wmError(ERROR_GENERAL, "unknown content element");
            break;
        };
    return s;
    }/* portflCharacters */

/*
 **- portflIgnorableWhitespace
 *
 *  DESCRIPTION:
 *      A character string is read, which is simply White space.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *      pSpace - Pointer to the character string
 *      length - Number of characters read. This is UNICODE encoded.
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflIgnorableWhitespace
    (
    XML_DOC_ENV *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte       *pSpace,    /* [in] Pointer to the character string */
    size_t      length      /* [in] Number of characters            */
    )
    {
    /* Terminate strings */
    /* at this time, we only process elements under content_directory */
    if (NULL == pCntntDir) return OK;
    (*pSpace) = 0;
    return OK;
    }/* portflIgnorableWhitespace */

/*
 **- portflProcessingInstruction
 *
 *  DESCRIPTION:
 *      A processing instruction is read.
 *  PARAMETERS:
 *      pXmlEnv     - Pointer to the XML_DOC_ENV structure
 *      pTarget - Pointer to the target tag
 *      pData - Pointer to data tag.
 *
 *  RETURNS:
 *      STATUS
 *  NOTES:
 *
 *
* NOMANUAL
*/
LOCAL STATUS portflProcessingInstruction
    (
    XML_DOC_ENV *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte       *pTarget,   /* [in] Pointer to the target tag */
    sbyte       *pData      /* [in] Pointer to the data string */
    )
    {
    /* We don't process the PI at the moment */
    return OK;

    }/* portflProcessingInstruction */

/* delete the trailing spaces of a string */
LOCAL sbyte* killTrail
    (
    sbyte* inString
    )
    {
    sbyte* str = inString;
    if (NULL == str) return NULL;

    while (0 != (*str))
        {
        if (isspace ((int) (*str)))
            {
            (*str) = 0;
            break;
            }
        ++str;
        }
    return inString;
    }
