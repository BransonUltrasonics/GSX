/* httpRpmAccessControl.c - Access Control RPM */

/*
 * Copyright (c) 1996-2008, 2010-2015 Wind River Systems, Inc.
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
25apr12,r_w  eliminate build error when enable DEBUG_INFO
                 CQID: WIND00346705
12jul10,m_z  change location auth. to avoid sub location being shielded
                 (WIND00206095)
2feb10,h_y   support for LP64 
04mar09,m_z  change verification of GID from str to structure list
                 (WIND00157654)
22feb07,ps   apigen reformatting
13feb07,msa  fix comments to remove APIgen errors.
05aug04,adb  added httpCtrlConfRemove and httpCtrlUrlRemove
14jan04,gsr  cleanup routine for SPR 93009
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpAccessControlListShow routine
31aug99,km   fixed duplication of httpCtrlUrlIterator() for docs.
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
23mar99,km   merged documentation revisions for auto man page generation.
17apr98,mb   indexed iterators established for easier use with WWW GUIs.
14apr98,mb   API change for httpGenListIterator() integrated.
15may97,ck   adapted for osal.
07may97,mb   httpSysRpmConfAdd() added to init function.
10mar97,mb   generic list support added.
11oct96,ck   beautified.
28sep96,mb   configuration moved to httpconfig.c.
11sep96,mb   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES: private/httpReq.h
               httpLib.h
               memPoolLib.h
               httpList.h
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
#include "commonCfg.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_ACCESS_CTRL
/* locals */

LOCAL HTTP_GEN_LIST_ID httpAccCtrlConfigId HTTP_PREDEFINE ( NULL ) ;

LOCAL unsigned short   sMembersPerCtrlMax  HTTP_PREDEFINE (  0   );
LOCAL unsigned short   sGidNameSizeMax     HTTP_PREDEFINE (  20  );
LOCAL unsigned short   sLocNameSizeMax     HTTP_PREDEFINE (  20  );


/***************************************************************************
*
* httpCtrlConfLock - lock the access control configuration list
*
* This routine locks the access control configuration data.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpCtrlConfUnlock(), httpOsalSemTake(), httpGenListLock()
*/

HTTP_STATUS httpCtrlConfLock (void)
    {
    return ( httpGenListLock ( httpAccCtrlConfigId ) );
    }

/***************************************************************************
*
* httpCtrlConfUnlock - unlock the access control configuration list
*
* This routine unlocks the access control configuration data.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpCtrlConfLock()
*/

HTTP_STATUS httpCtrlConfUnlock (void)
    {
    return httpGenListUnlock ( httpAccCtrlConfigId );
    }

/***************************************************************************
*
* httpCtrlConfGet - get the ID to the dynamic access control configuration list
*
* This routine provides the ID handle to the access control RPM configuration
* parameters.
*
* RETURNS: ID of the access configuration list, or NULL if error or no more
* authorizations in this user group.
*/

HTTP_GEN_LIST_ID httpCtrlConfGet(void)
    {
    return httpAccCtrlConfigId;
    }

/***************************************************************************
*
* httpCtrlUrlIndexIterator - get the first or next access control list entry
*
* This routine is an iterator that steps through the list of protected URLs
* using an index. The list of protected URLs are used by the access control
* RPM.  Use httpCtrlConfLock() to prevent other tasks from modifying data
* currently being examined.
*
* NOTE: pusIndex should be initialized with a pointer to 0 to get the first
* element of the access control list.
*
* RETURNS: ID of the access control list.
*
* SEE ALSO: httpCtrlUrlIterator(), httpGenListIndexIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpCtrlUrlIndexIterator
    (
    unsigned short * pusIndex  /* pointer to current access control entry index */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID urlEntryId;

    (void) httpCtrlConfLock();

    urlEntryId = httpGenListIndexIterator ( httpAccCtrlConfigId , pusIndex );

    (void) httpCtrlConfUnlock();
    return urlEntryId;
    }

/***************************************************************************
*
* httpCtrlUrlIterator - get the first or next access control list entry
*
* This routine is an iterator to step through the list of protected URLs that
* are currently used by the access control RPM.  Use httpCtrlConfLock() to
* prevent other tasks from modifying data currently being examined.
*
* NOTE: currentId must be initialized with a pointer to NULL or a pointer
* to some valid entry ID of the access control list.
*
* RETURNS: next entry ID of the access control list, or NULL if error or no more entries.
*
* SEE ALSO: httpGenListIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpCtrlUrlIterator
    (
    HTTP_GEN_LIST_ENTRY_ID * currentId  /* pointer to current access control entry */
    )
    {
    (void) httpCtrlConfLock();
    httpGenListIterator ( httpAccCtrlConfigId , currentId );
    (void) httpCtrlConfUnlock();
    return (*currentId);
    }

/***************************************************************************
*
* httpCtrlAllowIterator - get the first or next access configuration list entry
*
* This routine is an iterator to step through the list of combinations of
* location groups and user groups that require access to some protected URL.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by a httpCtrlConfLock(), the configuration area could be modified in the time
* between calls made to httpCtrlAllowIterator(). To avoid this, enclose all
* the httpCtrlAllowIterator() calls by to httpCtrlConfLock() and
* httpCtrlConfUnlock().
*
* RETURNS: next ID of the access configuration list, or NULL if error or no more
* authorizations in this user group.
*
* SEE ALSO: httpGenListIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpCtrlAllowIterator
    (
    HTTP_GEN_LIST_ENTRY_ID   ctrlEntryId,  /* list entry id of access control URL entry */
    HTTP_GEN_LIST_ENTRY_ID * allowEntryId  /* pointer current configuration entry id    */
    )
    {
    HTTP_GEN_LIST_ID       allowListId;

    if  ( ctrlEntryId == NULL )
        {
        return NULL;
        }

    (void) httpCtrlConfLock();
    if  ( ( allowListId = httpGenListDataGet ( ctrlEntryId ) ) != NULL )
        {
        httpGenListIterator ( allowListId , allowEntryId );
        }
    (void) httpCtrlConfUnlock();
    return (*allowEntryId);
    }

/***************************************************************************
*
* httpCtrlAllowIndexIterator - get the first or next access configuration list entry
*
* This is an iterator to step through the list of combinations of location
* groups and user groups that may access some protected URL.
*
* The result is a list entry id that contains a location group and
* user group that must match both to grant access to the protected URL.
*
* pusIndex should be initialized with a pointer to 0 to get the first element.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by a httpCtrlConfLock(), it could happen that the configuration area
* will be modified between several calls to httpCtrlAllowIterator.
* To avoid this enclose all the httpCtrlAllowIterator calls by a
* httpCtrlConfLock() - httpCtrlConfUnlock() call.
*
* RETURNS: next ID of the access configuration list, or NULL if error or no more
* authorizations in this user group.
*
* SEE ALSO: httpCtrlAllowIterator(), httpGenListIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpCtrlAllowIndexIterator
    (
    HTTP_GEN_LIST_ENTRY_ID ctrlEntryId,  /* list entry id of access control URL entry */
    unsigned short *       pusIndex      /* pointer to current configuration entry id    */
    )
    {
    HTTP_GEN_LIST_ID       allowListId  = NULL;
    HTTP_GEN_LIST_ENTRY_ID allowEntryId = NULL;

    if ( ctrlEntryId == NULL )
        {
        return NULL;
        }

    (void) httpCtrlConfLock();
    if ( ( allowListId = httpGenListDataGet ( ctrlEntryId ) ) != NULL )
        {
        allowEntryId = httpGenListIndexIterator ( allowListId , pusIndex );
        }
    (void) httpCtrlConfUnlock();
    return allowEntryId;
    }

/***************************************************************************
*
* httpCtrlConfInit - initialize dynamic access control configuration
*
* This routine initializes the access control configuration tables
* used by the access-control RPM. It should be invoked once if the
* access-control RPM will be configured with API routines.
* It should not be called if only static configuration
* tables will be used.
*
* RETURNS: HTTP_OK, or HTTP_ERROR if there is not enough memory for the
* requested table size.
*/

HTTP_GEN_LIST_ID httpCtrlConfInit
    (
    unsigned short sCtrls,          /* max number of access control statements     */
    unsigned short sMembersPerCtrl, /* max number of members per control statement */
    unsigned short sCtrlUrlSize,    /* max number of chars for a control statement */
    unsigned short sGidNameSize,    /* max # chars for a user group string         */
    unsigned short sLocNameSize     /* max # chars for a location group string     */
    )
    {
    if  ( httpSysRpmConfAdd (httpRpmCtrl) == HTTP_OK )
        {
        sMembersPerCtrlMax = sMembersPerCtrl;
        sGidNameSizeMax    = sGidNameSize;
        sLocNameSizeMax    = sLocNameSize;
        return ( httpAccCtrlConfigId = httpGenListCreate ( httpGlobalMemPartIdGet(), sCtrls , HTTP_TRUE ) );
        }
    return NULL;
    }

/***************************************************************************
*
* httpCtrlConfCleanup - Cleanup routine for httpCtrlConfInit()
*
* RETURNS: HTTP_OK always
*/

HTTP_STATUS httpCtrlConfCleanup
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId = NULL;

    if  (NULL != httpAccCtrlConfigId)
        {
        /* walking the configuration list */
        while (NULL != httpGenListIterator (httpAccCtrlConfigId, &urlidEntryId))
            {
            httpCtrlUrlDelete(urlidEntryId);
            }

        httpGenListDestroy (httpAccCtrlConfigId);
        httpAccCtrlConfigId = NULL;
        }

    return HTTP_OK;

    } /* httpCtrlConfCleanup */

/***************************************************************************
*
* httpCtrlConfAdd - add an access control statement to a location group
*
* This routine adds a user and location group that may access a protection
* space to the list of access control statements.  This means that a new
* protection space is created if the URL pattern was not yet
* defined.  This function uses only pointers, so all arguments passed to
* it are not copied and must not be changed afterwards.
*
* NOTE:
* If you add a user and location group pair that is already in the table,
* a second  (duplicate) entry will be created.
*
* RETURNS: HTTP_OK, or HTTP_ERROR if there are no more free entries in
* the configuration table.
*/

HTTP_GEN_LIST_ENTRY_ID httpCtrlConfAdd
    (
    char * szUrl,      /* URL pattern to protect                          */
    char * szGid,      /* user group that may access this URL pattern     */
    char * szLoc       /* location group that may access this URL pattern */
    )
    {
    HTTP_GEN_LIST_ID       ctrlListId   = NULL;
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId = NULL;
    HTTP_GEN_LIST_ENTRY_ID ctrlEntryId  = NULL;


    if  (szUrl == NULL)
        {
        logError ("httpCtrlConfAdd: szUrl is NULL!");
        return NULL;
        }

    if  ((strlen (szLoc) >= sLocNameSizeMax) || (strlen (szGid) >= sGidNameSizeMax))
        {
        logError ("httpCtrlConfAdd: szLoc or szGid are too long!");
        return NULL;
        }

    if  ( ( urlidEntryId = httpGenListFind ( httpAccCtrlConfigId , szUrl ) ) != NULL)
        {
        if  ( ( ctrlListId = httpGenListDataGet ( urlidEntryId ) ) == NULL)
            {
            logError ( "httpCtrlConfAdd: access ctrl list id is NULL!" );
            return NULL;
            }
        }
    else
        {
        if  ( ( ctrlListId = httpGenListCreate ( httpGlobalMemPartIdGet(), sMembersPerCtrlMax , HTTP_TRUE ) ) == NULL )
            {
            logError ( "httpCtrlConfAdd: unable to create a new access control list!" );
            return NULL;
            }
        if  ( httpGenListDataAppend ( httpAccCtrlConfigId , httpStrdup (httpGlobalMemPartIdGet(),szUrl), ctrlListId ) == NULL )
            {
            logError ( "httpCtrlConfAdd: unable to add new URL pattern!" );
            return NULL;
            }
        }

    if  ( ( ctrlEntryId = httpGenListDataAppend ( ctrlListId , httpStrdup (httpGlobalMemPartIdGet(),szLoc) , httpStrdup (httpGlobalMemPartIdGet(),szGid))) == NULL )
        {
        logError ( "httpCtrlConfAdd: unable to add new Gid/Loc pair!" );
        }
    return ctrlEntryId;
    }

/***************************************************************************
*
* httpCtrlConfDelete - delete a access control statement from a user group
*
* This routine removes a access control statement from a user group,
*
* RETURNS: N/A
*/

void httpCtrlConfDelete
    (
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId,
    HTTP_GEN_LIST_ENTRY_ID ctrlEntryId
    )
    {
    (void) httpCtrlConfLock();

#ifdef DEBUG_INFO
    printf ("httpCtrlConfDelete: removing access %s from group %s.\n",httpGenListIdGet (ctrlEntryId),httpGenListIdGet (urlidEntryId));
#endif

    (void)memPoolFree (httpGlobalMemPartIdGet() , httpGenListIdGet (ctrlEntryId));
    (void)memPoolFree (httpGlobalMemPartIdGet() , httpGenListDataGet(ctrlEntryId));
    httpGenListRemove (httpGenListDataGet (urlidEntryId),ctrlEntryId);

    (void) httpCtrlConfUnlock();
    }


/***************************************************************************
*
* httpCtrlConfRemove - remove a URL control access entry
*
* This routine receives as input the URL, user group name, and location
* group name and removes the entry and multiple entries if they exist.
*
* RETURNS: HTTP_OK or HTTP_ERROR if the entry does not exist
*/

HTTP_STATUS httpCtrlConfRemove
    (
    char * szUrl,   /* URL */
    char * szGid,   /* user group name */
    char * szLoc    /* location group name */
    )
    {
    HTTP_STATUS             rs           = HTTP_ERROR;
    HTTP_GEN_LIST_ENTRY_ID  urlidEntryId = NULL;
    HTTP_GEN_LIST_ID        ctrlListId   = NULL;
    HTTP_GEN_LIST_ENTRY_ID  ctrlEntryId  = NULL;

    if  ((NULL == szUrl) || (NULL == szGid) || (NULL == szLoc))
        {
        logError ("httpCtrlConfRemove: invalid input");
        return rs;
        }

    (void) httpCtrlConfLock();

    if  (NULL == (urlidEntryId = httpGenListFind (httpAccCtrlConfigId , szUrl)))
        {
        logError ("httpCtrlConfRemove: unable to find URL");
        (void) httpCtrlConfUnlock();
        return rs;
        }

    ctrlListId = httpGenListDataGet (urlidEntryId);

    while (NULL != httpGenListIterator (ctrlListId, &ctrlEntryId))
        {
        if  ((!(strcmp (szGid, httpGenListDataGet (ctrlEntryId)))) &&
             (!(strcmp (szLoc, httpGenListIdGet (ctrlEntryId)))))
            {
            httpCtrlConfDelete (urlidEntryId, ctrlEntryId);
            rs = HTTP_OK;
            }
        }

    (void) httpCtrlConfUnlock();
    return rs;

    } /* httpCtrlConfRemove */

/***************************************************************************
*
* httpCtrlUrlDelete - delete a URL group
*
* This routine removes all entries from a URL group, and deletes
* the URL group.
*
* RETURNS: N/A
*/

void httpCtrlUrlDelete
    (
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID ctrlEntryId = NULL;
    HTTP_GEN_LIST_ID       ctrlListId  = NULL;

#ifdef DEBUG_INFO
    printf ("httpCtrlUrlDelete: removing group %s.\n",httpCtrlUrlStringGet (urlidEntryId));
#endif

    (void) httpCtrlConfLock ();
    if ((ctrlListId = httpGenListDataGet (urlidEntryId)) != NULL)
        {
        while (httpGenListIterator (ctrlListId , &ctrlEntryId) != NULL)
            {
            httpCtrlConfDelete (urlidEntryId , ctrlEntryId);
            }
        }
    (void)memPoolFree (httpGlobalMemPartIdGet(), httpGenListIdGet(urlidEntryId));
    httpGenListDestroy (ctrlListId);
    httpGenListRemove (httpAccCtrlConfigId,urlidEntryId);
    (void) httpCtrlConfUnlock();
    }

/***************************************************************************
*
* httpCtrlUrlRemove - remove a URL from the access control database
*
* This routine receives as input the name of a URL. If a matching group
* exists in the database it removes all its entries and deletes it.
*
* RETURNS: HTTP_OK or HTTP_ERROR if the group does not exist
*/

HTTP_STATUS httpCtrlUrlRemove
    (
    char * szUrl    /* URL */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId = NULL;

    if  (NULL == szUrl)
        {
        logError ("httpCtrlUrlRemove: invalid input");
        return HTTP_ERROR;
        }

    if  (NULL == (urlidEntryId = httpGenListFind (httpAccCtrlConfigId , szUrl)))
        {
        logError ("httpCtrlUrlRemove: unable to find group");
        return HTTP_ERROR;
        }

    /* locking and unlocking occurs within httpCtrlUrlDelete */

    httpCtrlUrlDelete (urlidEntryId);
    return HTTP_OK;

    } /* httpCtrlUrlRemove */

/***************************************************************************
*
* httpCtrlUrlStringGet - retrieve a URL name
*
* This routine retrieves a URL name.
*
* RETURNS: A pointer to a string.
*/

char * httpCtrlUrlStringGet
    (
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId
    )
    {
    return httpGenListIdGet (urlidEntryId);
    }


/***************************************************************************
*
* httpRpmCtrl - control access to parts of the URL tree
*
* The access-control RPM depends on the information gathered by the
* IP-checking RPM or the password-checking RPM (or both),  and it does not
* try to get further information about the request. It uses the user group
* and location group determined by the IP-checking RPM and the password
* -checking RPM to decide if the request should be processed, rejected, or
* if the user must provide a password to continue.
*
* RETURNS: RPM_OK for access granted, RPM_DONE for no access, or RPM_ERROR
* on error.
*/

HTTP_STATUS httpRpmCtrl
    (
    HTTP_REQ_ID reqId
    )
    {
    HTTP_BOOL              fLocMatch    = HTTP_FALSE;
    HTTP_BOOL              fUrlMatch    = HTTP_FALSE;
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId = NULL;
    HTTP_GEN_LIST_ID       ctrlListId   = NULL;
    HTTP_GEN_LIST_ENTRY_ID ctrlEntryId  = NULL;
    HTTP_GEN_LIST_ENTRY_ID gidEntry = NULL; 
    HTTP_GEN_LIST_ENTRY_ID locEntry = NULL; 

    MEM_POOL_ID            reqMemory;
    char *                 szTmp;
	unsigned short    usGidCount = 0;
	unsigned short    usLocCandCount = 0;

#ifdef DEBUG_INFO
    printf ("Authorization: Loc: %s \n", httpLocationGet (reqId));
    while (NULL !=  httpGidEntryGet (reqId, &gidEntry))
    	{
    	printf("Authorization: GID: %s\n", httpGidEntryDataGet (reqId, gidEntry));
    	}
    gidEntry = NULL;
#endif

    reqMemory = httpReqMemPartIdGet (reqId);
    while ( httpGenListIterator ( httpAccCtrlConfigId , &urlidEntryId ) != NULL )
        {
        if (httpIsMatching (reqMemory, httpGenListIdGet (urlidEntryId) , reqId->szAbsPath ))
            {
            logInfo ("httpRpmCTRL: URL in access list found");
            fUrlMatch   = HTTP_TRUE;
            ctrlListId  = httpGenListDataGet ( urlidEntryId );
            ctrlEntryId = NULL;
            while ( httpGenListIterator (ctrlListId, &ctrlEntryId) != NULL)
                {
                char *szLocation = httpGenListIdGet ( ctrlEntryId );
                char *szGid = httpGenListDataGet ( ctrlEntryId );
                gidEntry = NULL;
                logInfo ("httpRpmCTRL: checking CTRL config list");
                locEntry = NULL;
                while(httpLocCandEntryGet(reqId, &locEntry) != NULL)
                {
                    szTmp = httpLocCandEntryDataGet(reqId, locEntry);
                    if (((szTmp == NULL) && (strcmp (szLocation,"*") == 0)) ||
                        httpIsMatching (reqMemory, httpGenListIdGet ( ctrlEntryId ),szTmp))
                        {
                        logInfo ("httpRpmCTRL: Location matched");
                        fLocMatch = HTTP_TRUE;
                        while (NULL != httpGidEntryGet(reqId, &gidEntry))
                        	{
                            szTmp = httpGidEntryDataGet(reqId, gidEntry);
                            if (((szTmp == NULL) && (strcmp (szGid,"*") == 0)) ||
                                httpIsMatching (reqMemory, httpGenListDataGet ( ctrlEntryId ),szTmp))
                                {
                                gidEntry = NULL;
                                while ((gidEntry = httpGidEntryRemove(reqId, NULL)))
                                    {
                                    memPoolFree(reqMemory, gidEntry);
                                    reqId->usGidEntryCount--;
                                    }
                                logInfo ("httpRpmCTRL: GID matched, access granted");

                                locEntry = NULL;
                                while ((locEntry = httpLocCandEntryRemove(reqId, NULL)))
                                    {
                                    memPoolFree(reqMemory, locEntry);
                                    reqId->usLocCandEntryCount--;
                                    }
                                logInfo ("httpRpmCTRL: Location matched, access granted");
                                return (RPM_OK);
                                }
                        	}
                        }                
                    }
                }
            }
        }

    usGidCount = reqId->usGidEntryCount;
    usLocCandCount = reqId->usLocCandEntryCount;
    gidEntry = NULL;
    while ((gidEntry = httpGidEntryRemove(reqId, NULL)))
        {
        (void)memPoolFree(reqMemory, gidEntry);
        reqId->usGidEntryCount--;
        }
    locEntry = NULL;
    while ((locEntry = httpLocCandEntryRemove(reqId, NULL)))
        {
        (void)memPoolFree(reqMemory, locEntry);
        reqId->usLocCandEntryCount--;
        }


    if ((0 < usGidCount) && (0 < usLocCandCount) && fLocMatch)
        {
        logInfo ("httpRpmCTRL: authorization required, IP ok");
        httpStatusSet (reqId,HTTP_UNAUTHORIZED);
        httpError (reqId,httpErrStringGet (HTTP_ACCESS_DENIED));
        return (RPM_DONE);
        }

    if (fUrlMatch == HTTP_TRUE)
        {
        logInfo ("httpRpmCTRL: access forbidden");
        httpStatusSet (reqId,HTTP_UNAUTHORIZED);
        httpError (reqId,httpErrStringGet (HTTP_ACCESS_DENIED));
        return (RPM_DONE);
        }

    logInfo ("httpRpmCTRL: access unrestricted");
    return (RPM_OK);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpAccessControlListShow - displays HTTP Access Control Configuration List
*
* The access control configuration is kept in a list indexed by URL prefixes.
* The list elements are lists with entries indexed by user/password group names
* and data IP address location groups.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpAccessControlListShow
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID urlidEntryId = NULL;
    char aux[32];
    char auxLeftMargin[32];

    /* we always print a header even if the list is empty */

    printf ("httpAccessControlListShow: p = %p\n", httpAccCtrlConfigId);

    printf ("%4s------\n", "");

    if  (NULL == httpAccCtrlConfigId)
        {
        printf ("%4shttpAccessControlList is empty\n", "");

        printf ("%4s------\n", "");

        return;
        }

    (void) httpCtrlConfLock ();

    /* walking the configuration list */

    while (NULL != httpGenListIterator (httpAccCtrlConfigId, &urlidEntryId))
        {
        printf ("%4spEntry = %p\n", "", urlidEntryId);

        printf ("%8sid = %s\n", "", httpGenListIdGet (urlidEntryId));

        (void)snprintf (aux, 31, "locations and groups");

        printf ("%6sdata = %p\n", "", httpGenListDataGet (urlidEntryId));

        (void)snprintf (auxLeftMargin, 31, "%13s", "");
        /*
         * 13 == strlen ("    pEntry = ")
         *    == strlen ("       sid = ")
         *    == strlen ("      data = ")
         */

        /* printing the sublist */

        httpListShow (httpGenListDataGet (urlidEntryId),
                      aux,
                      FALSE,    /* entry id is a string */
                      FALSE,    /* entry data is a string */
                      auxLeftMargin);

        printf ("%4s------\n", "");
        }

    (void)httpCtrlConfUnlock ();

    return;

    } /* httpAccessControlListShow */
#endif /* HTTP_SHOW */
#endif

