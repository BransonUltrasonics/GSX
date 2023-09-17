/* httpRpmAlias.c -  do some url -> path aliasing */

/*
 * Copyright (c) 1996-1997,1999, 2003-2004,2007, 2010-2011, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
29sep11,r_w  add way to delete WR Web server aliases.
                 defect:WIND00197521  
02feb10,h_y  support for LP64 
26feb07,msa  Correct apigen changes that should have been made, but were
                 removed on the last checkin.
22feb07,ps   apigen reformatting
13feb07,msa  Fix comments to remove APIgen errors.
14jan04,gsr  cleanup routine for SPR 93009
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpAliasesShow routine
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
15may97,mb   osal introduced.
05may97,mb   httpSysRpmConfAdd() added to init function.
05mar97,mb   generic list support added.
20sep96,ck   written.
*/

/*
DESCRIPTION

INCLUDE FILES: httpLib.h
               private/httpReq.h
               httpRpms.h
               httpShow.h
*/

/* includes */

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpRpms.h"
#include "httpShow.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_ALIASING
/* locals */

LOCAL HTTP_GEN_LIST_ID httpAliasConfigId HTTP_PREDEFINE (NULL);

/***************************************************************************
*
* httpAliasConfInit - initialize the sizes of the tables
*
* This routine initializes the internal boundaries needed for
* subsequent calls of all the other API routines to access the
* aliasing configuration tables.
*
* RETURNS: a pointer to the created alias list structure, or NULL if 
*          unsuccessful.
*
* SEE ALSO: httpAliasConfAdd()
*
* NOMANUAL
*/

HTTP_GEN_LIST_ID httpAliasConfInit
    (
    unsigned short sTabMax /* Maximum number of entries in predefined table */
    )
    {
      if ( httpSysRpmConfAdd ( httpRpmAlias ) == HTTP_OK )
    return ( httpAliasConfigId = httpGenListCreate (httpGlobalMemPartIdGet(), sTabMax , HTTP_TRUE ) );
      return NULL;
    }

/***************************************************************************
*
* httpAliasConfCleanup - cleanup routine for httpAliasConfInit()
*
* RETURNS: HTTP_OK
*
* SEE ALSO: httpAliasConfInit()
*
* NOMANUAL
*/

HTTP_STATUS httpAliasConfCleanup
    (
    void
    )
    {
    if  (NULL != httpAliasConfigId)
        {
        httpGenListDestroy (httpAliasConfigId);
        }

    httpAliasConfigId = NULL;

    return (HTTP_OK);

    } /* httpAliasConfCleanup */

/***************************************************************************
*
* httpAliasConfDel - delete an entry from the aliasing table
*
* This routine deletes the specified entry from the aliasing table if this entry exist in table.
*
* RETURNS: HTTP_OK.
*
* SEE ALSO: httpAliasConfAdd()
*/

HTTP_STATUS httpAliasConfDel
    (
    char * szSearch       /* Incoming URL request       */
     )
{
    HTTP_GEN_LIST_ENTRY_ID entryId;
 
    entryId = httpGenListFind(httpAliasConfigId,szSearch);

    if (entryId!=NULL)
    {
        httpGenListRemove(httpAliasConfigId,entryId);
    }
    return (HTTP_OK);
}

/***************************************************************************
*
* httpAliasConfAdd - add an entry to the aliasing table
*
* This routine adds the specified entry to the aliasing table.
*
* NOTE: The string is not copied.  Only a pointer to its location is
* stored.
*
* RETURNS: a pointer to the new alias list entry, or NULL if unsuccessful.
*
* SEE ALSO: httpAliasConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpAliasConfAdd
    (
    char * szSearch,       /* Incoming URL request       */
    char * szReplace       /* Corresponding aliased path */
    )
    {
    (void) httpAliasConfDel(szSearch);
    return ( httpGenListDataAppend ( httpAliasConfigId , szSearch , szReplace ) );
    }

/***************************************************************************
*
* pathSubstitute - 
*
* RETURNS:
*
* NOMANUAL
*/

LOCAL HTTP_BOOL pathSubstitute
    (
    HTTP_REQ_ID   reqId,
    char *        szPath,
    char *        szPattern,
    char *        szReplacement
    )
    {
    unsigned long ulRepIndex = 0;
    unsigned long ulBufferIndex = 0;
    unsigned long ulMaxBuffer;
    char *        szTmp;
    char *        szPatternAfterAsterisk;

    if (((strlen(szPattern) - (unsigned long) httpSignificantChars (szPattern)) != 1 ||
        ((strlen(szReplacement) - (unsigned long) httpSignificantChars (szReplacement)) != 1)))
        {      /* not exactly same number of asterisks in pattern & replacement */
        if ((strlen(szPattern) - (unsigned long) httpSignificantChars (szPattern)) == 0)
        {
        strncpy (szPath,szReplacement,httpMaxUrlSize);   /* no asterisks -> only replaceing */
        }
        return (HTTP_TRUE);
        }

    ulMaxBuffer = httpMaxUrlSize<httpMaxTempBufSize?httpMaxUrlSize:httpMaxTempBufSize;

    ulMaxBuffer--;
    /*
     * copy first part of the replacement string to the buffer (until a
     * asterisk in the szReplacement string occures).
     */

    while (szReplacement[ulRepIndex] != '*' &&
           szReplacement[ulRepIndex] != 0 &&
           ulRepIndex < ulMaxBuffer)
        {
        reqId->szBuffer[ulRepIndex] = szReplacement[ulRepIndex];
        ulRepIndex++;
        }
    ulBufferIndex = ulRepIndex;

    /*
     * find the matching part after the asterisk in the szPath
     * and append path up to this index to the szBuffer
     */

    szPatternAfterAsterisk = strchr (szPattern, '*') + 1;
    szTmp = szPath + (szPatternAfterAsterisk - szPattern) - 1;

    while ((szTmp[0] != 0) &&
           (strcmp (szTmp, szPatternAfterAsterisk) != 0) &&
           (ulBufferIndex < ulMaxBuffer))
    reqId->szBuffer[ulBufferIndex++] = *(szTmp++);

    /*
     * append the missing rest from the replacement string to the buffer
     */

    ulRepIndex ++;  /* skip asterisk */

    while (szReplacement[ulRepIndex] != 0 &&
           ulBufferIndex < ulMaxBuffer)
        {
        reqId->szBuffer[ulBufferIndex] = szReplacement[ulRepIndex];
        ulRepIndex++;
        ulBufferIndex++;
        }

    reqId->szBuffer [ulBufferIndex] = 0;

    strcpy (szPath, reqId->szBuffer);
    return (HTTP_TRUE);
    }

/***************************************************************************
*
* httpRpmAlias - perform URL translations
*
* This routine creates soft links in a URL (path) tree structure, so that
* one page can be accessed with multiple URLs.
* It can be used to ensure that every request
* for a URL specifying a given directory (for example, `/pub/')
* is mapped to one file in this directory, such as `index.html'.
*
* CONFIGURATION:
* Modify the table aRpmAliasingTableConf[] in the file httpconfig.c.
* 
* RETURNS: RPM_OK or RPM_ERROR.
*/

short httpRpmAlias
    (
    HTTP_REQ_ID reqId      /* Handle of the active request, needed for all */
                           /* http - API function calls                    */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID configId = NULL;


    logInfoMsg ("URL before aliasing: %s", reqId->szAbsPath);

    /* do */
    {
    while ( httpGenListIterator ( httpAliasConfigId , &configId ) != NULL )
    {
    if (httpIsMatching (httpReqMemPartIdGet (reqId),  httpGenListIdGet (configId) , reqId->szAbsPath ))
        {
        pathSubstitute (reqId,reqId->szAbsPath,httpGenListIdGet (configId),httpGenListDataGet (configId));
        logInfoMsg ("URL after aliasing: %s\n", reqId->szAbsPath);
        }
    }
    }
    /*   while (HTTP_FALSE);  // if we want recursive aliasing... */


    return (RPM_OK);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpAliasesShow - displays HTTP Aliases Configuration list
*
* The aliases configuration list is indexed by regular expression encoding
* URL prefixes and each entry's data is the corresponding actual URL prefix.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpAliasesShow
    (
    void
    )
    {
    httpListShow (httpAliasConfigId, 
                  "httpAliases", 
                  FALSE,    /* entry id is a string */
                  FALSE,    /* entry data is a string */
                  NULL      /* left margin is not required */
                 );

    return;
    } /* httpAliasesShow */
#endif /* HTTP_SHOW */
#endif

