/* httpRpmIpCheck.c -  do socket IP validation */

/*
 * Copyright (c) 1996-2008,2010-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
11jun15,r_w  fix static analysis issue V7MAN-154.
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
08apr13,r_w  fix static analysis issues for defect WIND00411995
27jul10,m_z  eliminate compile warning
12jul10,m_z  change location auth. to avoid sub location being shielded
             (WIND00206095)
02feb10,h_y   support for LP64 
25jun08,q_z  implementation of config file save and load(WIND00042066)
22feb07,ps   apigen reformatting
14jan04,gsr  cleanup routine for SPR 93009
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpIpCheckListShow routine
24sep03,jws  fix compiler warnings, SPR 90843
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
27nov98,ur   fixed configuration corruption error in httpIpConfSave (),
             reworked httpIpConfLoad ().
17apr98,mb   indexed iterators established for easier use with WWW GUIs.
14apr98,mb   API change for httpGenListIterator() integrated.
15may97,mb   osal introduced.
05may97,mb   httpSysRpmConfAdd() added to init function.
05mar97,mb   generic list support added.
28sep96,mb   configuration moved to httpconfig.c.
12sep96,ck   beta testing release.
15aug96,pk   beautified.
25jul96,ck   minor enhancements.
17jul96,ck   written.
*/

/*
DESCRIPTION

INCLUDE FILES: httpLib.h
               httpRpms.h
               httpShow.h
*/

/* includes */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpShow.h"
#include "commonCfg.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_IP_CHECKING
/* locals */

extern void httpLocCandCountReset(HTTP_REQ_ID reqId); 
extern void httpLocCandCountInc(HTTP_REQ_ID reqId);
extern void httpLocCandCountDec(HTTP_REQ_ID reqId);

LOCAL HTTP_GEN_LIST_ID httpIpCheckConfigId HTTP_PREDEFINE ( NULL );

LOCAL short   sIpPerLocMax HTTP_PREDEFINE ( 0 );
LOCAL short   sIpPatSizeMax    HTTP_PREDEFINE ( 16 );
LOCAL short   sLocatNameSizeMax HTTP_PREDEFINE ( 16 );

/***************************************************************************
*
* httpIpConfLock - lock the dynamic IP configuration tables
*
* This routine uses a mutual exclusion semaphore to prevent access to IP
* configuration data.
*
* RETURNS: HTTP_OK if locking was successful, HTTP_ERROR otherwise.
*
* SEE ALSO: wmosSemTake()
*/

HTTP_STATUS httpIpConfLock (void)
    {
    return httpGenListLock ( httpIpCheckConfigId );
    }

/***************************************************************************
*
* httpIpConfUnlock - unlock the dynamic IP configuration tables
*
* This routine unlocks accesses to IP configuration data that was
* locked with httpIpConfLock().
*
* RETURNS: N/A
*/

HTTP_STATUS httpIpConfUnlock (void)
    {
    return httpGenListUnlock ( httpIpCheckConfigId );
    }

/***************************************************************************
*
* httpIpLocationIterator - get the first or next location group
*
* This routine is an iterator to step through the list of location groups
* inside the IP configuration list.
*
* NOTE:
* Although all critical accesses to the IP configuration area are protected
* by a httpIpConfLock(), the IP configuration area might
* be modified between calls to httpIpLocationIterator().
* To prevent this from happening, enclose all the httpIpLocationIterator()
* calls with calls to httpIpConfLock() and httpIpConfUnlock().
*
* RETURNS: A list entry ID of the location group.
*
* SEE ALSO: httpGenListIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpIpLocationIterator
    (
    HTTP_GEN_LIST_ENTRY_ID * locationId /* pointer to valid entry of the location group list or NULL */
    )
    {
    if  (NULL == httpIpCheckConfigId)
        {
        return NULL;
        }
    (void) httpIpConfLock();
    httpGenListIterator ( httpIpCheckConfigId , locationId );
    (void) httpIpConfUnlock();
    return (*locationId);
    }

/***************************************************************************
*
* httpIpLocationIndexIterator - gets the first or next location group
*
* This routine is an iterator to step through the list of location groups
* inside the IP configuration list using an index.
*
* NOTE:
* Although all critical accesses to the IP configuration area are protected
* by a httpIpConfLock(), the IP configuration area might
* be modified between calls to httpIpLocationIterator().
* To prevent this from happening, enclose all the httpIpLocationIterator()
* calls with calls to httpIpConfLock() and httpIpConfUnlock().
* Initialize the pointer 'pusIndex' to 0 to get the first entry ID of the
* location group list.
*
* RETURNS: A list entry ID of the location group.
*
* SEE ALSO: httpIpLocationIterator(), httpGenListIndexIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpIpLocationIndexIterator
    (
    unsigned short * pusIndex /* pointer to index */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID locationEntryId;

    if  (NULL == httpIpCheckConfigId)
        {
        return NULL;
        }
    (void) httpIpConfLock();
    locationEntryId = httpGenListIndexIterator ( httpIpCheckConfigId , pusIndex );
    (void) httpIpConfUnlock();
    return locationEntryId;
    }

/***************************************************************************
*
* httpIpPatternIterator - get the first or next IP pattern
*
* This routine is an iterator to step through the list of IP patterns of a
* location group.
*
* NOTE:
* Although all critical accesses to the IP configuration area are protected
* by a httpIpConfLock() call, the IP configuration area might
* be modified between calls to httpIpPatternIterator().
* To prevent this from happening, enclose all calls to httpIpPatternIterator()
* with calls to httpIpConfLock() and httpIpConfUnlock().
*
* RETURNS: List entry ID to an IP pattern entry, or
* NULL for error or no more IP patterns in this location group.
*/

HTTP_GEN_LIST_ENTRY_ID httpIpPatternIterator
    (
    HTTP_GEN_LIST_ENTRY_ID   locEntryId, /* list entry id of location group  */
    HTTP_GEN_LIST_ENTRY_ID * ipEntryId   /* pointer to a IP pattern entry id */
    )
    {
    HTTP_GEN_LIST_ID       ipListId;

    if (locEntryId == NULL)
    return NULL;

    (void) httpIpConfLock();
    ipListId = httpGenListDataGet ( locEntryId );
    httpGenListIterator ( ipListId , ipEntryId );
    (void) httpIpConfUnlock();
    return (*ipEntryId);
    }

/***************************************************************************
*
* httpIpPatternIndexIterator - gets the first or next IP pattern
*
* This routine is an iterator to step through the list of IP patterns of a
* location group using an index.
*
* NOTE:
* Although all critical accesses to the IP configuration area are protected
* by a httpIpConfLock() call, the IP configuration area might
* be modified between calls to httpIpPatternIterator().
* To prevent this from happening, enclose all calls to httpIpPatternIterator()
* with calls to httpIpConfLock() and httpIpConfUnlock().
* Initialize the pointer 'pusIndex' to 0 to get the first entry ID of the
* location group list.
*
* RETURNS: List entry ID to an IP pattern entry, or
* NULL for error or no more IP patterns in this location group.
*
* SEE ALSO: httpIpPatternIterator(), httpGenListIndexIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpIpPatternIndexIterator
    (
    HTTP_GEN_LIST_ENTRY_ID locEntryId, /* list entry id of location group */
    unsigned short *       pusIndex    /* pointer to index                */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID ipEntryId = NULL;
    HTTP_GEN_LIST_ID       ipListId;

    if (locEntryId != NULL)
    {
    (void) httpIpConfLock();
    ipListId = httpGenListDataGet ( locEntryId );
    ipEntryId = httpGenListIndexIterator ( ipListId , pusIndex );
    (void) httpIpConfUnlock();
    }
    return (ipEntryId);
    }

/***************************************************************************
*
* httpIpConfInit - initialize the dynamic IP configuration list
*
* This routine initializes the IP configuration list with the specified
* parameters.  This routine should be invoked exactly once before you
* configure the IP-checking RPM.
*
* RETURNS: List ID of location group list, or
* NULL if the location group list could not be created.
*/

HTTP_GEN_LIST_ID httpIpConfInit
    (
    unsigned short sLocs,         /* max # location groups                */
    unsigned short sIpPerLoc,     /* max # IP patterns per location group */
    unsigned short sIpPatSize,    /* max length of an IP pattern name     */
    unsigned short sLocatNameSize /* max length of a location name        */
    )
    {
      if ( httpSysRpmConfAdd (httpRpmIPchk) == HTTP_OK )
      {
      sIpPerLocMax      = (short) sIpPerLoc;
      sIpPatSizeMax     = (short) sIpPatSize;
      sLocatNameSizeMax = (short) sLocatNameSize;
      return ( httpIpCheckConfigId = httpGenListCreate ( httpGlobalMemPartIdGet(), sLocs , HTTP_TRUE ) );
      }
      return NULL;
    }

/***************************************************************************
*
* httpIpConfCleanup - cleanup routine for httpIpConfInit()
*
* RETURNS: HTTP_OK always 
*/

HTTP_STATUS httpIpConfCleanup 
    (
    void
    )
    {
    if  (NULL != httpIpCheckConfigId)
        {

        HTTP_GEN_LIST_ENTRY_ID entryId = NULL;

        /* walking the configuration list */
        while (NULL != httpGenListIterator (httpIpCheckConfigId, &entryId))
            {
            httpIpGroupDelete(entryId);

            entryId = NULL;
            }

        httpGenListDestroy (httpIpCheckConfigId);

        httpIpCheckConfigId = NULL;
        } 

    return (HTTP_OK);
    
    } /* httpIpConfCleanup */

/***************************************************************************
*
* httpIpConfGet - get the handle of the dynamic IP configuration list
*
* This routine gets the handle ID of the IP RPM configuration parameter list.
*
* RETURNS: List ID of location group list, or
* NULL if the location group list could not be created.
*/

HTTP_GEN_LIST_ID httpIpConfGet(void)
    {
    return httpIpCheckConfigId;
    }

/***************************************************************************
*
* httpIpConfAddInternal - add an IP pattern to a location group
*
* This routine adds an IP pattern to the location group with name <szLoc>.
* It assumes that the IP configuration table has been locked.
*
* If the location group does not yet exist, it is created.
* If <szLoc> is valid and <szIp> is NULL, this routine ensures that the
* location group will be created if it does not exist, but this
* new group will be empty.
*
* NOTE:
* This routine uses httpStrdup() to copy all strings provided to it.
*
* If you add an IP pattern that is already in the table, there will be a
* second entry.
*
* NOMANUAL
*
* RETURNS: Entry ID of the added IP pattern, or
* NULL if addition failed,
* if <szIp> was NULL, the return value is NULL, but it is enforced that
* there is a location group <szLoc>.
*/

LOCAL HTTP_GEN_LIST_ENTRY_ID httpIpConfAddInternal
    (
    char * szLoc,      /* the location group to which an IP should be added */
    char * szIp        /* IP pattern */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID locEntryId;
    HTTP_GEN_LIST_ENTRY_ID newEntryId = NULL;
    HTTP_GEN_LIST_ID       ipListId;

    if ( ( locEntryId = httpGenListFind ( httpIpCheckConfigId , szLoc ) ) != NULL)
    {
    if ( ( ipListId = httpGenListDataGet ( locEntryId ) ) == NULL)
        {
        logError ( "httpIpConfAdd: IP pattern list id is NULL!" );
        return NULL;
        }
    }
    else
    {
    if ( ( ipListId = httpGenListCreate ( httpGlobalMemPartIdGet(), (unsigned short)sIpPerLocMax , HTTP_TRUE ) ) == NULL )
        {
        logError ( "httpIpConfAdd: unable to create a new IP pattern list!" );
        return NULL;
        }
    if ( httpGenListDataAppend ( httpIpCheckConfigId , httpStrdup ( httpGlobalMemPartIdGet() , szLoc ) , ipListId ) == NULL )
        {
        logError ( "httpIpConfAdd: unable to add a new location group!" );
        return NULL;
        }
    }

    if ( szIp != NULL )
    if ( ( newEntryId = httpGenListDataAppend ( ipListId , httpStrdup ( httpGlobalMemPartIdGet() , szIp ) , NULL ) ) == NULL )
        {
        logError ( "httpIpConfAdd: unable to add new location/IP pair!" );
        return NULL;
        }

    return newEntryId;
    }

/***************************************************************************
*
* httpIpConfAdd - add an IP pattern to a location group
*
* This routine adds an IP pattern to the location group with name <szLoc>.
* If the location group does not yet exist, it is created.
* If <szLoc> is valid and <szIp> is NULL, this routine ensures that the
* provided location group will be created if it does not exist, but this
* new group will be empty.
*
* NOTE:
* This routine uses httpStrdup() to copy all strings provided to it.
* If you add an IP pattern that is already in the table, there will be a
* second entry.
*
* RETURNS: Entry ID of the added IP pattern, or
* NULL if addition failed.
* If <szIp> was NULL, the return value is NULL, but it is enforced that
* there is a location group <szLoc>.
*/

HTTP_GEN_LIST_ENTRY_ID httpIpConfAdd
    (
    char * szLoc,      /* the location group to which an IP should be added */
    char * szIp        /* IP pattern */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID newEntryId = NULL;

#ifdef DEBUG_INFO
    printf ("httpIpConfAdd: add %s to location group %s\n",szIp,szLoc);
#endif

    if ( szLoc == NULL)
    {
        logError ("httpIpConfAdd: location group is NULL!");
        return NULL;
    }

    /* Lock the IP configuration, and add the new entry. */
    /* httpIpConfLock();  */
/* jc - locking not needed since httpIpConfAddInternal uses safe API to list */
    newEntryId = httpIpConfAddInternal (szLoc, szIp);
    /* httpIpConfUnlock();*/

    return newEntryId;
    }

/***************************************************************************
*
* httpIpGroupDelete - remove a location group
*
* This routine removes all entries of a location group and deletes the
* location group.
*
* RETURNS: N/A
*/

void httpIpGroupDelete
    (
    HTTP_GEN_LIST_ENTRY_ID locEntryId  /* location group entry to delete */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID ipEntryId = NULL;

#ifdef DEBUG_INFO
    printf ("httpIpGroupDelete: delete location group %s\n",httpGenListIdGet ( locEntryId ));
#endif

    (void) httpIpConfLock ();
    while ( httpGenListIterator ( (HTTP_GEN_LIST_ID)httpGenListDataGet ( locEntryId ) , &ipEntryId ) != NULL )
      {
    httpIpConfDelete ( locEntryId , ipEntryId );
    ipEntryId = NULL;
      }
    httpGenListDestroy ( httpGenListDataGet ( locEntryId ) );
    (void)memPoolFree ( httpGlobalMemPartIdGet() , httpGenListIdGet ( locEntryId ) );
    httpGenListRemove ( httpIpCheckConfigId , locEntryId );
    (void) httpIpConfUnlock ();
    }

/***************************************************************************
*
* httpIpConfDelete - delete an IP pattern from a location group
*
* This routine deletes an IP pattern entry from the location group specified
* by <locEntryId>.  Empty location groups stay in the list.
*
* RETURNS: N/A
*/

void httpIpConfDelete
    (
    HTTP_GEN_LIST_ENTRY_ID locEntryId, /* the location group to deleted from */
    HTTP_GEN_LIST_ENTRY_ID ipEntryId   /* IP pattern to delete */
    )
    {
    if ( ( locEntryId == NULL) || ( ipEntryId == NULL ))
      {
        logError ("httpIpConfDelete: invalid argument!");
      }

    (void) httpIpConfLock();

#ifdef DEBUG_INFO
    printf ("httpIpConfDelete: delete %s from location group %s\n",httpGenListIdGet ( ipEntryId ),httpGenListIdGet ( locEntryId ));
#endif

    (void)memPoolFree ( httpGlobalMemPartIdGet() , httpGenListIdGet ( ipEntryId ) );
    httpGenListRemove ( httpGenListDataGet ( locEntryId ) , ipEntryId );
    (void) httpIpConfUnlock();
    }

/***************************************************************************
*
* httpIpConfSave - save dynamic IP configuration data to a file
*
* Save the dynamic IP configuration data (location groups and the
* corresponding IP patterns) to a file.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpIpConfSave
    (
    generic_fd_t * outFile
    )
    {
    HTTP_GEN_LIST_ENTRY_ID patEntryId;
    HTTP_GEN_LIST_ENTRY_ID locEntryId = NULL;
    char *                 szTmpPat;
    char *                 szTmpLoc;
    char *                 szBuffer;
    HTTP_STATUS            fStatus    = HTTP_OK;
    ubyte4                 ubytesWritten;

    szBuffer = memPoolAlloc (httpGlobalMemPartIdGet(), (unsigned long) (sIpPatSizeMax + sLocatNameSizeMax + 2 + 5 ));
    if (szBuffer == NULL)
    {
    logError ("httpIpConfSave: Out of request pool memory!");
    return (HTTP_ERROR);
    }
    (void) httpIpConfLock();

    while ( httpIpLocationIterator ( &locEntryId ) != NULL )
    {
    patEntryId = NULL;
    while ( httpIpPatternIterator ( locEntryId , &patEntryId ) != NULL )
        {
        if  ((szTmpLoc = httpGenListIdGet ( locEntryId )) == NULL)
            {
        fStatus = HTTP_ERROR;
            }
        else
            {
            strncpy (szBuffer, szTmpLoc,(size_t)sLocatNameSizeMax);
            strcat (szBuffer, ",");
            if ((szTmpPat = httpGenListIdGet ( patEntryId )) != NULL)
                strncat (szBuffer, szTmpPat, (size_t)sIpPatSizeMax);
            strncat (szBuffer, "\n",2);
    
            fStatus = (HTTP_STATUS)httpFileWrite (outFile, szBuffer, (sbyte4)strlen (szBuffer), &ubytesWritten);
            }

        if  (fStatus == HTTP_ERROR)
            {
            logError ( "httpIpConfSave: failed writing IP config!" );
            httpIpConfUnlock();
            memPoolFree (httpGlobalMemPartIdGet(), szBuffer);
            return (HTTP_ERROR);
            }
        }
    }
    (void) httpIpConfUnlock();
    (void) memPoolFree (httpGlobalMemPartIdGet(), szBuffer);
    return (HTTP_OK);
    }

/***************************************************************************
*
* httpIpConfLoad - load the dynamic IP configuration data from a file
*
* Load the dynamic IP configuration data (location groups and the
* corresponding IP patterns) from a file.
*
* NOTE: Uses httpBufferGet() for internal buffer needs.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpIpConfLoad
    (
    generic_fd_t * inFile
    )
    {
    unsigned long      lBufSz;
    char *             szBuffer;
    HTTP_STATUS        fStatus   = HTTP_OK;
    ubyte4             bytesRead = 0;
    unsigned long      filePos   = 0;


    lBufSz = (unsigned long)(sIpPatSizeMax + sLocatNameSizeMax + 2 + 5);
    if  ( (szBuffer = memPoolAlloc (httpGlobalMemPartIdGet(), lBufSz)) == NULL )
        {
        logError ( "httpIpConfLoad: request memory pool too small!" );
        return HTTP_ERROR;
        }

    (void) httpIpConfLock();
    memset(szBuffer,'\0',lBufSz);
    if  (OK != httpFileRead(inFile,szBuffer,(ubyte4)lBufSz,&bytesRead))
        {
        logError ("httpIpConfLoad: file read failed!");
        fStatus = HTTP_ERROR;
        }
    else
        {
        while (bytesRead)
            {
            char *      szTmpPat;
            char *      szTmpPatEnd;
            char *      szTmpLoc;
            
            filePos = filePos + (long)bytesRead;
            *(szBuffer + lBufSz) = '\0';    /* terminate the string */
            szTmpLoc = szBuffer;
            if  ((szTmpPat = strchr (szBuffer, ',')) == NULL)
                {
                logError ( "httpIpConfLoad: data corruption!" );
                fStatus = HTTP_ERROR;
                break;
                }
            else
                {
                *szTmpPat = '\0';
                szTmpPat++;
    
                if  ((szTmpPatEnd = strstr (szTmpPat, "\n")) != NULL)
                    {
                    *szTmpPatEnd = '\0';
                    }
    
                if  (*szTmpPat == '\0')
                    {
                    szTmpPat = NULL;
                    }
    
                if  ( httpIpConfAdd ( szTmpLoc , szTmpPat ) == NULL )
                    {
                    logError ( "httpIpConfLoad: httpIpConfAdd failed!" );
                    fStatus = HTTP_ERROR;
                    break;
                    }
                }

            filePos = (unsigned long)(filePos - (unsigned long)(bytesRead - (unsigned long)(szTmpPatEnd - szTmpLoc + 1)));
            (void) httpFileSeek(inFile, (ubyte4)filePos);
            memset(szBuffer,'\0',lBufSz);
            if  (OK != httpFileRead(inFile,szBuffer,(ubyte4)lBufSz,&bytesRead))
                {
                logError ("httpIpConfLoad: file read failed!");
                fStatus = HTTP_ERROR;
                break;
                }
            }
        }

    (void) httpIpConfUnlock();
    (void) memPoolFree ( httpGlobalMemPartIdGet() , szBuffer );

    if  (fStatus == HTTP_ERROR)
        {
        logError ( "httpIpConfLoad: load failed!" );
        }

    return (fStatus);
    }

/***************************************************************************
*
* httpRpmIPchk - determine location of user by IP address
*
* This routine is part of the authentication mechanisms provided for the
* server. It checks the IP address of the client initiating this request
* and tries to assign it to one of the defined groups in the table
* aRpmIPConfigTab[]. The result is stored for further processing.
* No access control is done by this module.
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*
* SEE ALSO: httpRpmCtrl(), httpRpmPass()
*/

short httpRpmIPchk
    (
    HTTP_REQ_ID reqId  /* Handle of the active request */
    )
    {
    long                   lBestLength = 0;
    char *                 szGroup;
    HTTP_GEN_LIST_ENTRY_ID locEntryId=NULL;
    HTTP_GEN_LIST_ENTRY_ID ipEntryId;
    HTTP_GEN_LIST_ENTRY_ID bestMatchId = NULL;
    HTTP_GEN_LIST_ENTRY_ID tmpEntryId = NULL;
    HTTP_GEN_LIST_ID       ipListId;
    char *                 szTmpBuffer = httpBufferGet (reqId);  /* get address of scratch buffer */
    MEM_POOL_ID            reqMemId = httpReqMemPartIdGet (reqId);

    if  (NULL == httpIpCheckConfigId)
    {
    return RPM_ERROR;
    }

    if (NULL != httpGetEnv(reqId, "REMOTE_ADDR"))
        strncpy (szTmpBuffer, httpGetEnv (reqId, "REMOTE_ADDR"),httpMaxTempBufSize-1);

    (void) httpIpConfLock();
/*    reqMemId = httpReqMemPartIdGet (reqId);*/
/*     reqId->usLocCandEntryCount = 0; */
    httpLocCandCountReset(reqId);
    while ( httpGenListIterator ( httpIpCheckConfigId , &locEntryId ) != NULL )
      {
        ipEntryId = NULL;
        ipListId = httpGenListDataGet ( locEntryId );
        while ( httpGenListIterator ( ipListId , &ipEntryId ) != NULL)
            {
            if (httpIsMatching (httpReqMemPartIdGet (reqId), httpGenListIdGet ( ipEntryId ) , szTmpBuffer)) 
                {
                tmpEntryId = memPoolAlloc (reqMemId , sizeof(HTTP_GEN_LIST_ENTRY));
                tmpEntryId->nextId = NULL;
                tmpEntryId->listItem.pData = locEntryId;
                httpLocCandEntryAdd(reqId, tmpEntryId);

/*                 reqId->usLocCandEntryCount++; */
                httpLocCandCountInc(reqId);
                
                if (httpSignificantChars (httpGenListIdGet ( ipEntryId )) >= lBestLength)
                    {
                    lBestLength = httpSignificantChars (httpGenListIdGet ( ipEntryId ));
                    bestMatchId = locEntryId;
                    }
                }
            }
      }
    szGroup =  ( bestMatchId != NULL ) ? httpGenListIdGet ( bestMatchId ) : NULL;

#ifdef DEBUG_INFO
    printf("Group determined by ipckecking: %s -> %s\n", szTmpBuffer, szGroup);
#endif
    httpLocationSet (reqId, szGroup);
    (void) httpIpConfUnlock();
    return (RPM_OK);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpIpCheckListShow - show HTTP IP access check control list
*
* The IP access check control list is indexed by location names. For each 
* location entry there exists a list of acceptable IP patterns.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpIpCheckListShow
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID entryId = NULL;
    char auxHeader[32];
    char auxLeftMargin[32];

    /* we always print a header even if the list is empty */

    printf ("httpIpCheckListShow: p = %p\n", httpIpCheckConfigId);

    printf ("%4s------\n", "");

    if  (NULL == httpIpCheckConfigId)
        {
        printf ("%4shttpIpCheckList is empty\n", "");
        
        printf ("%4s------\n", "");

        return;
        }

    (void) httpIpConfLock();

    /* walking the configuration list */

    while (NULL != httpGenListIterator (httpIpCheckConfigId, &entryId))
        {
        printf ("%4spEntry = %p\n", "", entryId);       

        printf ("%8sid = %s\n", "", httpGenListIdGet (entryId));

        printf ("%6sdata = %p\n", "", httpGenListDataGet (entryId));

        if  (strlen (httpGenListIdGet (entryId)) < 
             sizeof (auxHeader) - strlen (" location IP patterns"))
            {
            (void)snprintf (auxHeader, 31,
                     "%s location IP patterns", 
                     httpGenListIdGet (entryId));
            }
        else
            {
            (void)snprintf (auxHeader, 31, 
                     "%s location IP patterns", 
                     "name_too_long_to_be_displayed");
            }

        (void)snprintf (auxLeftMargin, 31, "%13s", ""); 
        /* 
         * 13 == strlen ("    pEntry = ")
         *    == strlen ("       sid = ")
         *    == strlen ("      data = ") 
         */

        /* printing the sublist */

        httpListShow (httpGenListDataGet (entryId),
                      auxHeader, 
                      FALSE,    /* entry id is a string */
                      FALSE,    /* entry data is a string */
                      auxLeftMargin);


        printf ("%4s------\n", "");
        }

    (void)httpIpConfUnlock();

    return;
    
    } /* httpIpCheckListShow */
#endif /* HTTP_SHOW */
#endif

