/* httpRpmPassword.c - Password Control RPM */

/*
 * Copyright (c) 1996-2008,2010,2013,2015 Wind River Systems, Inc.
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
20feb13,r_w  change char * to unsigned char *
                 defect: WIND00403091 and WIND00403090
02feb10,h_y  support for LP64 
04mar09,m_z  allow password-verified user belonging multi group
                 (WIND00157654)
25jun08,q_z  implementation of config file save and load(WIND00042066)
13feb07,msa  Fix comments to remove APIgen errors.
03aug04,adb  added httpPwdGroupRemove and httpPwdConfUserRemove
21jan04,jws  simplify HTTP_AUTHORIZATION_ID_SIZE macro
16jan04,jws  remove per project dependencies
14jan04,gsr  cleanup routine for SPR 93009
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added httpPasswordListShow routine
24sep03,jws  fix compiler warnings, SPR 90843
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
27nov98,ur   fixed configuration corruption error in httpPwdConfSave ();
                 reworked httpPwdConfLoad ();
                 fixed memory leak in httpRpmPass ();
                 fixed some potential buffer overruns.
17apr98,mb   indexed iterators established for easier use with WWW GUIs.
14apr98,mb   API change for httpGenListIterator() integrated.
15may97,mb   osal introduced.
07may97,mb   httpSysRpmConfAdd() added to init function.
25mar97,mb   load/save functions rewritten, using generic lists now.
10mar97,mb   generic list support added.
16sep96,mb   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES: httpLib.h
               httpRpms.h
               private/httpReq.h
               private/httpInt.h
               httpShow.h
               httpCfg.h
*/

/* includes */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpShow.h"
#include "commonCfg.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_PWD_CHECKING
/* locals */

#ifdef UNDEFINED
LOCAL const char * const szAuthentication = "Basic realm=\"%s\"";
#else
#define szAuthentication "Basic realm=\"%s\""
#endif

LOCAL char       *       szRealm;

LOCAL HTTP_GEN_LIST_ID httpPwdConfigId HTTP_PREDEFINE (NULL);

LOCAL unsigned short   sUsersPerGidMax HTTP_PREDEFINE (0);
LOCAL unsigned short   sCredentialSizeMax HTTP_PREDEFINE (0);   /* max # of bytes for a credential string (including the 0 termination character) */
LOCAL unsigned short   sGidNameSizeMax HTTP_PREDEFINE (0);


LOCAL char *  szCredentialBuffer HTTP_PREDEFINE (NULL);
LOCAL char *  szUserPwd HTTP_PREDEFINE (NULL);
HTTP_STATUS httpPwdGroupRemove(char * szGroup );

/* defines */

#ifdef HTTP_SHOW
LOCAL void authorizationListShow
    (
    HTTP_GEN_LIST_ID listId,
    char *           pListName,
    char *           pLeftMargin
    );
#endif /* HTTP_SHOW */



/***************************************************************************
*
* httpPwdConfLock - lock the dynamic user configuration tables
*
* This routine uses a mutual exclusion semaphore to prevent access to
* user configuration data.
*
* RETURNS: HTTP_OK if locking was successful, HTTP_ERROR otherwise.
*
* SEE ALSO: httpPwdConfUnlock(), httpOsalSemTake()
*/

HTTP_STATUS httpPwdConfLock (void)
    {
    return (httpGenListLock (httpPwdConfigId));
    }

/***************************************************************************
*
* httpPwdConfUnlock - unlock the dynamic user configuration tables
*
* This routine unlocks accesses to user configuration data locked
* by httpPwdConfLock().
*
* RETURNS: HTTP_OK or HTTP_ERROR if unlocking failed.
*
* SEE ALSO: httpPwdConfLock()
*/

HTTP_STATUS httpPwdConfUnlock (void)
    {
    return httpGenListUnlock (httpPwdConfigId);
    }

/***************************************************************************
*
* httpPwdGroupIterator - get the first or next user group
*
* This routine is an iterator that steps through the list of user groups in the
* user configuration area.  An entry ID is assigned inside
* the configuration area, so be careful to perform a httpPwdConfLock()
* for as long as the result is in use.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by httpPwdConfLock(), the user configuration area could be
* modified between calls to httpPwdGroupIterator().
* To prevent this from happening, enclose all the httpPwdGroupIterator()
* calls with calls to httpPwdConfLock() and httpPwdConfUnlock().
*
* RETURNS: Generic list entry ID to next user group entry
* of the user configuration.
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdGroupIterator
    (
    HTTP_GEN_LIST_ENTRY_ID * gidEntryId  /* pointer to valid the user group entry id */
    )
    {
    if  (NULL == httpPwdConfigId)
        {
        return NULL;
        }
    (void) httpPwdConfLock();
    httpGenListIterator (httpPwdConfigId , gidEntryId);
    (void) httpPwdConfUnlock();
    return (*gidEntryId);
    }

/***************************************************************************
*
* httpPwdGroupIndexIterator - get the first or next user group
*
* This routine is an iterator that steps through the list of user groups in the
* user configuration area using an index.  An entry ID is assigned inside
* the configuration area, so be careful to perform a httpPwdConfLock()
* for as long as the result is in use.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by httpPwdConfLock(), the user configuration area could be
* modified between calls to httpPwdGroupIterator().
* To prevent this from happening, enclose all the httpPwdGroupIterator()
* calls with calls to httpPwdConfLock() and httpPwdConfUnlock().
*
* RETURNS: Generic list entry ID to next user group entry
* of the user configuration, or NULL if last item had been returned already.
*
* SEE ALSO: httpGenListIndexIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdGroupIndexIterator
    (
    unsigned short * pusIndex  /* pointer to current index */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID gidEntryId;

    if  (NULL == httpPwdConfigId)
        {
        return NULL;
        }
    (void) httpPwdConfLock();
    gidEntryId = httpGenListIndexIterator (httpPwdConfigId , pusIndex);
    (void) httpPwdConfUnlock();

    return (gidEntryId);
    }

/***************************************************************************
*
* httpPwdAuthIterator - get the first or next authorization entry
*
* This routine is an iterator that steps through the list of
* authorizations for a  user group.  An authorization is the combination
* of <user:password>, base64 encoded.
* For more details see the HTML 1.1 specification for the
* subject "Basic Authentication Scheme".
* The result is a list entry ID of the user configuration area, so be
* careful to lock the configuration data as long as you use this ID.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by httpPwdConfLock(), the user configuration area could
* be modified between calls to httpPwdAuthIterator().
* To prevent this from happening, enclose all the httpPwdAuthIterator()
* calls with calls to  httpPwdConfLock() and httpPwdConfUnlock().
*
* RETURNS: List entry ID to an user authorization string or
* NULL for error or no more authorizations in this user group.
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdAuthIterator
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId,    /* list entry id of user group             */
    HTTP_GEN_LIST_ENTRY_ID * authEntryId  /* pointer to valid authorization entry id */
    )
    {
    HTTP_GEN_LIST_ID       authListId;

    if (gidEntryId == NULL)
    return (NULL);

    (void) httpPwdConfLock();
    if ((authListId = httpGenListDataGet (gidEntryId)) != NULL)
        httpGenListIterator (authListId , authEntryId);
    (void) httpPwdConfUnlock();
    return (*authEntryId);
    }

/***************************************************************************
*
* httpPwdAuthIndexIterator - get the first or next authorization entry
*
* This routine is an iterator that steps through the list of
* authorizations for a  user group using an index.  An authorization is the
* combination of <user:password>, base64 encoded.
* For more details see the HTML 1.1 specification for the
* subject "Basic Authentication Scheme".
* The result is a list entry ID of the user configuration area, so be
* careful to lock the configuration data as long as you use this ID.
*
* NOTE:
* Although all critical accesses to the user configuration area are protected
* by httpPwdConfLock(), the user configuration area could
* be modified between calls to httpPwdAuthIterator().
* To prevent this from happening, enclose all the httpPwdAuthIterator()
* calls with calls to  httpPwdConfLock() and httpPwdConfUnlock().
* Initialize pointer 'pusIndex' to 0 to get the first element of the
* authorization list.
*
* RETURNS: List entry ID to an user authorization string or
* NULL for error or no more authorizations in this user group.
*
* SEE ALSO: httpPwdAuthIterator()
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdAuthIndexIterator
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId,  /* list entry id of user group */
    unsigned short       * pusIndex     /* pointer to index            */
    )
    {
    HTTP_GEN_LIST_ID       authListId;
    HTTP_GEN_LIST_ENTRY_ID authEntryId = NULL;

    if (gidEntryId != NULL)
    {
    (void) httpPwdConfLock();
    if ((authListId = httpGenListDataGet (gidEntryId)) != NULL)
        authEntryId = httpGenListIndexIterator (authListId , pusIndex);
    (void) httpPwdConfUnlock();
    }
    return (authEntryId);
    }

/***************************************************************************
*
* httpPwdConfGet - get the handle ID of the dynamic password configuration list
*
* This routine returns the handle ID of the password RPM configuration
* parameter list.
*
* RETURNS: List id of user group list or
*          NULL if the user group list could not be created
*/

HTTP_GEN_LIST_ID httpPwdConfGet()
    {
    return httpPwdConfigId;
    }

/***************************************************************************
*
* httpPwdConfInit - initializes the dynamic password configuration
*
* This routine initializes the password configuration list with the
* given parameters.
* Additionally, the realm for the basic authentication scheme is set.
* For more information on this subject see the HTTP/1.1 specification
* from chapter 11.1 on.
* This routine should be invoked exactly once before configuring the
* password checking RPM with API functions.
*
* NOTE:
* Credentials are base64 encoded strings, which always have a length
* divisible by 4 (rounded up), plus a null termination character.
*
* RETURNS: List id of user group list or
*          NULL if the user group list could not be created
*/

HTTP_GEN_LIST_ID httpPwdConfInit
    (
    unsigned short sGids,  /* max number of user groups          */
    unsigned short sUsersPerGid,    /* max number of users per user group */
    unsigned short sGidNameSize,    /* max # characters for gid name      */
    unsigned short sCredentialSize, /* max # characters for a credential  */
    char *         szBasicRealm          /* realm for basic authentication     */
    )
    {
    if (httpSysRpmConfAdd (httpRpmPass) == HTTP_OK)
    {
    sUsersPerGidMax    = sUsersPerGid;
    sCredentialSizeMax = sCredentialSize;
    sGidNameSizeMax    = sGidNameSize;
    szRealm            = szBasicRealm;

    if ((szCredentialBuffer = memPoolAlloc(httpGlobalMemPartIdGet(), (unsigned long)(2*sCredentialSize+1)*sizeof (char))) != NULL)
        if ((szUserPwd = memPoolAlloc(httpGlobalMemPartIdGet(), (unsigned long)(sCredentialSize+1)*sizeof (char)))!=NULL)
            httpPwdConfigId = httpGenListCreate (httpGlobalMemPartIdGet(), sGids , HTTP_TRUE);
    return ( httpPwdConfigId);
    }
    return NULL;
    }

/***************************************************************************
*
* httpPwdConfCleanup - Cleanup routine for httpPwdConfInit()
*
* RETURNS: HTTP_OK always
*/

HTTP_STATUS httpPwdConfCleanup
    (
    void
    )
    {
    if  (NULL != httpPwdConfigId)
        {

        HTTP_GEN_LIST_ENTRY_ID entryId = NULL;

        /* walking the configuration list */
        while (NULL != httpGenListIterator (httpPwdConfigId, &entryId))
            {
            httpPwdGroupDelete(entryId);

            entryId = NULL;
            }

        httpGenListDestroy (httpPwdConfigId);

        httpPwdConfigId = NULL;
        }

    return (HTTP_OK);
    
    } /* httpPwdConfCleanup */

/***************************************************************************
*
* httpPwdDecode - get the user name from an authorization string
*
* This routine extracts the user name from an authorization
* string.
*
* RETURNS: HTTP_OK, or
* HTTP_ERROR if the authorization could not be decoded by base64decode
* or if <sSize> was smaller than the string "name:password".
*/

HTTP_STATUS httpPwdDecode
    (
    char * szAuth, /* authorization string */
    char * szName, /* buffer where to return the extracted user name */
    unsigned short  sSize   /* size of buffer szName, must be long enough for a credential! */
    )
    {
    char * szTmp;
    short  sLength;

    if (sSize >= sCredentialSizeMax)
    {
    sLength = httpBase64decode ((unsigned char *)szAuth,(unsigned char *)szName);

    if ((sLength > 0) && (sLength <= sSize))
        {
        if ((szTmp = strchr (szName,':')) != NULL)
        {
        szTmp[0] = 0;
        return HTTP_OK;
        } else szName[0] = 0;
        }
    }
    return HTTP_ERROR;
    }

/***************************************************************************
*
* httpPwdConfAdd - add a user and password pair to a user group
*
* This routine adds a user and password pair to the user group
* with name <szGid>.
* If the user group does not yet exist, it will be created.
*
* NOTE:
* This function uses httpStrdup() to copy all strings provided to it.
* If you add a user and password pair that is already in the table,
* a second (duplicate) entry is created.
*
* RETURNS: HTTP_GEN_LIST_ENTRY_ID of added user configuration entry, or
* NULL if there are no more free entries in the configuration list.
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdConfAdd
    (
    char * szGid,      /* the user group where a user should be added */
    char * szName,     /* user name as ASCII string */
    char * szPwd       /* password for the user as ASCII string */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID newId = NULL;

    if (szGid != NULL)
        {
        (void)snprintf (szUserPwd,((unsigned short)((global_HTTP_USERNAME_SIZE+global_HTTP_PASSWORD_SIZE+1)*4/3+4)),"%s:%s",szName,szPwd);
        if (httpBase64encode ((unsigned char *)szUserPwd, (unsigned short) strlen (szUserPwd),
                              (unsigned char *)szCredentialBuffer, (unsigned short)sCredentialSizeMax)==HTTP_OK)
            newId = httpPwdAuthAdd (szGid,szCredentialBuffer);
        }
    return (newId);
    }

/***************************************************************************
*
* httpPwdAuthAdd - add an authorization to a user group
*
* This routine adds an authorization to the user group with name <szGid>.
* An authorization is a string with the components <user:password> that has
* been base64 encoded.
* If the user group does not yet exist, it is created.
* <szAuth> may be a NULL pointer to install an empty user group.
* This function uses its own memory management, so all arguments passed to
* it are copied and can be changed afterwards.
*
* NOTE:
* If you add a user and password pair that is already in the table, a
* second (duplicate) entry will be created.
*
* RETURNS: HTTP_OK, or HTTP_ERROR if there are no more free entries
* in the configuration table.
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdAuthAdd
    (
    char * szGid, /* the user group where an authorization should be added */
    char * szAuth /* base64 encoded authorization string */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID gidEntryId;
    HTTP_GEN_LIST_ENTRY_ID newEntryId = NULL;
    HTTP_GEN_LIST_ID       authListId;


    if (szGid == NULL)
        return NULL;


    if ((gidEntryId = httpGenListFind (httpPwdConfigId , szGid)) != NULL)
    {
    if ((authListId = httpGenListDataGet (gidEntryId)) == NULL)
        {
        logError ("httpPwdConfAdd: gid list id is NULL!");
        goto httpPwdConfAddERROR;
        }
    } else {
    if ((authListId = httpGenListCreate (httpGlobalMemPartIdGet(), sUsersPerGidMax , HTTP_TRUE)) == NULL)
        {
        logError ("httpPwdConfAdd: unable to create a new gid list!");
        goto httpPwdConfAddERROR;
        }
    if (httpGenListDataAppend (httpPwdConfigId , httpStrdup (httpGlobalMemPartIdGet() , szGid) , authListId) == NULL)
        {
        logError ("httpPwdConfAdd: unable to add a new group id!");
        goto httpPwdConfAddERROR;
        }
    }
    if (szAuth != NULL)
    if ((newEntryId = httpGenListDataAppend (authListId , httpStrdup (httpGlobalMemPartIdGet() , szAuth) , NULL)) == NULL)
        {
        logError ("httpPwdConfAdd: unable to add new user gid/password pair!");
        goto httpPwdConfAddERROR;
        }
    goto httpPwdConfAddOK;

      httpPwdConfAddERROR:
    logError ("httpPwdConfAdd failed!");
httpPwdConfAddOK:

    return (newEntryId);
    }

/***************************************************************************
*
* httpPwdConfDelete - delete a user from a user group
*
* This routine removes a user and password pair from a user group,
* but leaves user groups that don't have  any user and password entries.
*
* RETURNS: N/A
*/

void httpPwdConfDelete
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId,
    HTTP_GEN_LIST_ENTRY_ID authEntryId
    )
    {
    (void) httpPwdConfLock();

#ifdef DEBUG_INFO
    printf ("httpPwdConfDelete: removing user %s from group %s.\n",httpGenListIdGet (authEntryId),httpGenListIdGet (gidEntryId));
#endif

    (void)memPoolFree (httpGlobalMemPartIdGet() , httpGenListIdGet (authEntryId));
    httpGenListRemove (httpGenListDataGet (gidEntryId) , authEntryId);
    (void) httpPwdConfUnlock();
    }

/***************************************************************************
*
* httpPwdConfUserRemove - remove a user from a user group
*
* This routine receives as input the group name and the user name and 
* removes all user and password pairs from a user group if the user exists.
*
* RETURNS: HTTP_OK or HTTP_ERROR if the user does not exist
*/

HTTP_STATUS httpPwdConfUserRemove
    (
    char * szGroup,   /* group name */
    char * szName     /* user name */
    )
    {
    HTTP_STATUS             rs =            ERROR; 
    HTTP_GEN_LIST_ENTRY_ID  gidEntryId =    NULL;
    HTTP_GEN_LIST_ENTRY_ID  authEntryId =   NULL;
   
    if  ((NULL == szGroup) || (NULL == szName))
        {
        logError ("httpPwdConfUserRemove: invalid input");

        return (HTTP_ERROR);
        }

    (void) httpPwdConfLock();

    if  (NULL == (gidEntryId = httpGenListFind (httpPwdConfigId , szGroup)))
        {
        logError ("httpPwdConfUserRemove: unable to find group");

        goto EXIT_LABEL;
        }
    
    if  (NULL == (authEntryId = httpPwdNameFind (gidEntryId,
                                                 szName,
                                                 szCredentialBuffer)))
        {
        logError ("httpPwdConfUserRemove: unable to find user");

        goto EXIT_LABEL;
        }

    httpPwdConfDelete (gidEntryId, authEntryId);

    /* remove multiple user entries */

    while (NULL != (authEntryId = httpPwdNameFind (gidEntryId,
                                                   szName,
                                                   szCredentialBuffer)))
        {
        httpPwdConfDelete (gidEntryId, authEntryId);
        }

    rs = OK;

EXIT_LABEL:

    (void) httpPwdConfUnlock();

    return (rs);

    } /* httpPwdConfUserRemove */

/***************************************************************************
*
* httpPwdGroupDelete - delete a user group
*
* This routine removes all entries from a user group, and deletes
* the user group.
*
* RETURNS: N/A
*/

void httpPwdGroupDelete
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID authEntryId = NULL;
    HTTP_GEN_LIST_ID       authListId;

#ifdef DEBUG_INFO
    printf ("httpPwdGroupDelete: removing group %s.\n",httpPwdGroupStringGet (gidEntryId));
#endif

    (void) httpPwdConfLock ();
    if ((authListId = httpGenListDataGet (gidEntryId)) != NULL)
    while (httpGenListIterator (authListId , &authEntryId) != NULL)
        {
        httpPwdConfDelete (gidEntryId , authEntryId);
        authEntryId = NULL;
        }
    httpGenListDestroy (httpGenListDataGet (gidEntryId));
    (void) memPoolFree (httpGlobalMemPartIdGet() , httpPwdGroupStringGet (gidEntryId));
    httpGenListRemove (httpPwdConfigId , gidEntryId);
    (void) httpPwdConfUnlock ();
    }

/***************************************************************************
*
* httpPwdGroupRemove - remove a user group
*
* This routine receives as input the name of user group. If the group exists
* it removes all its entries and deletes it.
*
* RETURNS: HTTP_OK or HTTP_ERROR if the group does not exist
*/

HTTP_STATUS httpPwdGroupRemove
    (
    char * szGroup    /* group name */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID gidEntryId = NULL;

    if  (NULL == szGroup)
        {
        logError ("httpPwdGroupRemove: invalid input");

        return (HTTP_ERROR);
        }

    if  (NULL == (gidEntryId = httpGenListFind (httpPwdConfigId , szGroup)))
        {
        logError ("httpPwdGroupRemove: unable to find group");

        return (HTTP_ERROR);
        }

    /* locking and unlocking occurs within httpPwdGroupDelete */

    httpPwdGroupDelete (gidEntryId);

    return (HTTP_OK);

    } /* httpPwdGroupRemove */

/***************************************************************************
*
* httpPwdNameFind - find a user name inside an authorization list
*
* This routine attempts to find an entry in an authorization list that
* matches a given user name and returns that entry.
*
* RETURNS: HTTP_GEN_LIST_ENTRY_ID of entry with matching username or
* NULL if there is no user matching <szUser>.
*/

HTTP_GEN_LIST_ENTRY_ID httpPwdNameFind
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId,  /* user group entry to search in    */
    char *                 szUser,      /* user name to search in the group */
    char *                 szBuffer     /* temporary buffer to invoke httpPwdDecode() with */
                                        /* this buffer must keep a credential, so it should be  */
                                        /* as long as specified with sCredentialSize in httpPwdConfInit() */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID authEntryId = NULL;

    (void) httpPwdConfLock();
    while (httpPwdAuthIterator (gidEntryId , &authEntryId) != NULL)
    {
    if (httpPwdDecode (httpGenListIdGet (authEntryId),szBuffer,sCredentialSizeMax) == HTTP_ERROR)
        {
        logError ("httpPwdNameFind: httpPwdDecode failed!");
        szBuffer[0] = 0;
        }
#ifdef DEBUG_INFO
    printf ("httpPwdNameFind: Group: %s, auth: %s\n",httpGenListIdGet (gidEntryId) ,szBuffer);
#endif
    if (strcmp (szBuffer , szUser) == 0)
        {
        httpPwdConfUnlock();
        return (authEntryId);
        }
    }
    (void) httpPwdConfUnlock();
    return NULL;
    }

/***************************************************************************
*
* httpPwdGroupStringGet - retrieve a group name
*
* This routine retrieves a group name.
*
* RETURNS: A pointer to a string.
*/

char * httpPwdGroupStringGet
    (
    HTTP_GEN_LIST_ENTRY_ID gidEntryId
    )
    {
    return httpGenListIdGet (gidEntryId);
    }

/***************************************************************************
*
* httpPwdAuthStringGet -  retrieve a base64 encoded username and password
*
* This routine retrieves a base64 encoded username and password string.
*
* RETURNS: A pointer to a string.
*/

char * httpPwdAuthStringGet
    (
    HTTP_GEN_LIST_ENTRY_ID authEntryId
    )
    {
    return httpGenListIdGet (authEntryId);
    }

/***************************************************************************
*
* httpPwdConfSave - save dynamic user configuration data to a file
*
* This routine saves the dynamic user configuration data (location groups
* and the corresponding user patterns) to a file.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpPwdConfSave
    (
    generic_fd_t * outFile
    )
    {
    HTTP_GEN_LIST_ENTRY_ID authEntryId;
    HTTP_GEN_LIST_ENTRY_ID gidEntryId = NULL;
    char *                 szTmpAuth;
    char *                 szTmpGid;
    char *                 szBuffer;
    HTTP_STATUS            fStatus    = HTTP_OK;
    ubyte4                 ubytesWritten;

    szBuffer = memPoolAlloc (httpGlobalMemPartIdGet(), (unsigned long)sCredentialSizeMax + sGidNameSizeMax + 2 + 5);
    if (szBuffer == NULL)
    {
    logError ("httpPwdConfSave: Out of request pool memory!");
    return (HTTP_ERROR);
    }
    (void) httpPwdConfLock();

    while (httpPwdGroupIterator (&gidEntryId) != NULL)
    {
    authEntryId = NULL;
    while (httpPwdAuthIterator (gidEntryId , &authEntryId) != NULL)
        {
        if  ((szTmpGid = httpGenListIdGet (gidEntryId)) == NULL)
            {
            fStatus = HTTP_ERROR;
            }
        else
            {
            strncpy (szBuffer, szTmpGid, sGidNameSizeMax);
            strcat (szBuffer, ",");
            if ((szTmpAuth = httpGenListIdGet (authEntryId)) != NULL)
                strncat (szBuffer, szTmpAuth, sCredentialSizeMax);
            strncat (szBuffer, "\n",2);
    
            fStatus = (HTTP_STATUS) httpFileWrite (outFile, szBuffer, (sbyte4)strlen (szBuffer), &ubytesWritten);
            }

        if  (fStatus == HTTP_ERROR)
            {
            logError ("httpPwdConfSave: failed writing Pwd config!");
            httpPwdConfUnlock();
            memPoolFree (httpGlobalMemPartIdGet(), szBuffer);
            return (HTTP_ERROR);
            }
        }
    }
    (void) httpPwdConfUnlock();
    (void) memPoolFree (httpGlobalMemPartIdGet(), szBuffer);
    return (HTTP_OK);
    }

/***************************************************************************
*
* httpPwdConfLoad - load dynamic user configuration data from a file
*
* This routine loads the dynamic user configuration data (user groups and the
* corresponding credentials) from a file.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpPwdConfLoad
    (
    generic_fd_t * inFile
    )
    {
    ubyte4        lBufSz;
    char *        szBuffer;
    HTTP_STATUS   fStatus   = HTTP_OK;
    ubyte4        bytesRead = 0;
    ubyte4        filePos   = 0;

    lBufSz = (ubyte4)(sCredentialSizeMax + sGidNameSizeMax + 2 + 5);
    if ((szBuffer = memPoolAlloc (httpGlobalMemPartIdGet(), lBufSz)) == NULL)
    {
    logError ("httpPwdConfLoad: request memory pool too small!");
    return HTTP_ERROR;
    }

    (void) httpPwdConfLock();
    memset(szBuffer,'\0',lBufSz);
    if  (OK != httpFileRead(inFile,szBuffer,lBufSz,&bytesRead))
        {
        logError ("httpPwdConfLoad: file read failed!");
        fStatus = HTTP_ERROR;
        }
    else
        {
        while (bytesRead)
            {
            char *      szTmpAuth;
            char *      szTmpAuthEnd;
            char *      szTmpGid;
            
            filePos += bytesRead;
            *(szBuffer + lBufSz) = '\0';    /* terminate the string */
            szTmpGid = szBuffer;
            if  ((szTmpAuth = strchr (szBuffer, ',')) == NULL)
                {
                logError ("httpPwdConfLoad: data corruption!");
                fStatus = HTTP_ERROR;
                break;
                }
            else
                {
                *szTmpAuth = '\0';
                szTmpAuth++;
                
                if  ((szTmpAuthEnd = strstr (szTmpAuth, "\n")) != NULL)
                    {
                    *szTmpAuthEnd = '\0';
                    }
                
                if  (*szTmpAuth == '\0') /* empty group */
                    {
                    szTmpAuth = NULL;
                    }
                
                if  (httpPwdAuthAdd (szTmpGid , szTmpAuth) == NULL)
                    {
                    logError ("httpPwdConfLoad: httpPwdAuthAdd failed!");
                    fStatus = HTTP_ERROR;
                    break;
                    }
                }

            filePos = (ubyte4)(filePos - (bytesRead - (szTmpAuthEnd - szTmpGid + 1)));
            (void) httpFileSeek(inFile, filePos);
            memset(szBuffer,'\0',lBufSz);
            if  (OK != httpFileRead(inFile,szBuffer,lBufSz,&bytesRead))
                {
                logError ("httpPwdConfLoad: file read failed!");
                fStatus = HTTP_ERROR;
                break;
                }
            }
        }

    (void) httpPwdConfUnlock();
    (void)memPoolFree (httpGlobalMemPartIdGet() , szBuffer);

    if  (fStatus == HTTP_ERROR)
        {
        logError ("httpPwdConfLoad: load failed!");
        }

    return (fStatus);
    }

/***************************************************************************
*
* httpRpmPass - determine user by authentication
*
* This routine assigns the user to a user group according to
* the authentication scheme sent by the client browser.
* It does this according to the list
* aRpmPassConfigTab[] in the file httpconfig.c.  The result is stored for
* further processing.  No access control is performed by this module.
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*
* SEE ALSO: httpRpmCTRL(), httpRpmIPchk()
*/

HTTP_STATUS httpRpmPass
    (
    HTTP_REQ_ID reqId
    )
    {
    char *                 szUser;
    unsigned short         usSize;
    char *                 szTmp;
    char *                 szAuth = httpAuthorizationGet (reqId,HDR_IN);
    HTTP_GEN_LIST_ENTRY_ID gidEntryId = NULL;
    HTTP_GEN_LIST_ENTRY_ID tmpEntryId = NULL;
    HTTP_GEN_LIST_ID       userListId;
    MEM_POOL_ID            reqMemId = httpReqMemPartIdGet (reqId);

    if  (NULL == httpPwdConfigId)
    {
    return RPM_ERROR;
    }

    if ((szTmp = memPoolAlloc (reqMemId , strlen (szAuthentication) + strlen (szRealm) + 1)) != NULL)
    {
    (void)snprintf (szTmp ,(strlen (szAuthentication) + strlen (szRealm)), szAuthentication , szRealm);
    (void) httpAuthenticationSet (reqId,HDR_OUT, (char *)szTmp);
    /*
     * httpAuthenticationSet () already duplicated the authentication string,
         * so we should free this piece of memory here.
     */
    (void)memPoolFree (reqMemId , szTmp);
    }
    else
    {
    szTmp = (char *)szAuthentication;
    logInfo ("httpRpmPass: not enough memory to set authentication!");
    (void) httpAuthenticationSet (reqId,HDR_OUT, (char *)szTmp);
    }

    if (strncmp (szAuth,"Basic ",6)==0)
    szAuth = &szAuth[6];

    usSize =(unsigned short) strlen (szAuth);
    if ((szUser = memPoolAlloc (reqMemId , (unsigned long)(usSize + 1))) != NULL)
    {
    if (httpPwdDecode (szAuth,szUser,usSize) == HTTP_OK)
        httpSetEnv (reqId , "REMOTE_USER" , szUser);
    (void)memPoolFree (reqMemId , szUser);
    }

    (void) httpPwdConfLock();
    reqId->usGidEntryCount = 0;
    while (httpGenListIterator (httpPwdConfigId , &gidEntryId) != NULL)
        {
        userListId = httpGenListDataGet (gidEntryId);
        if (httpGenListFind (userListId , szAuth) != NULL)
            {
            tmpEntryId = memPoolAlloc (reqMemId , sizeof(HTTP_GEN_LIST_ENTRY));
            tmpEntryId->nextId = NULL;
            tmpEntryId->listItem.pData = gidEntryId;
            httpGidEntryInsert(reqId, tmpEntryId);
            reqId->usGidEntryCount++;
            }
        }

    httpPwdConfUnlock();
    if (0 < reqId->usGidEntryCount)
        logInfo ("httpRpmPass: authorization correct");
    else
        logInfo ("httpRpmPass: no authorization");
    return (RPM_OK);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* authorizationListShow - shows authorization sublist contents
*
* This is a LOCAL function and the only expected caller is the function
* httpPasswordListShow. It is a trusted interface and the entry strings
* are expected to be valid. The authorization list is a list of entries
* with no data. All the information is in the entry index which is the
* base 64 encoding of user password pair separated by a column.
*
* RETURNS: NA
*
* NOMANUAL
*/

LOCAL void authorizationListShow
    (
    HTTP_GEN_LIST_ID listId,
    char *           pListName,
    char *           pLeftMargin
    )
    {
    HTTP_GEN_LIST_ENTRY_ID  entryId = NULL;
    char *                  authId = NULL;
    char *                  szName = NULL;
    short                   sSize = 0;

    sSize = (short) (2*sCredentialSizeMax+1);
            /* conservative interpretation of legacy code */

    /* we always print a header even if the list is empty */

    printf ("%s", pLeftMargin);
    printf ("authorizationListShow: p = %p, name = %s\n", listId, pListName);

    printf ("%s", pLeftMargin);
    printf ("%4s--------\n", "");

    if  (NULL == listId)
        {
        printf ("%s", pLeftMargin);
        printf ("%4slist is empty\n", "");

        printf ("%s", pLeftMargin);
        printf ("%4s--------\n", "");

        goto EXIT_LABEL;
        }

    /* walking the authorization list */

    if  (NULL == (szName = malloc ((size_t)(sSize+1))))
        {
        printf ("authorizationListShow: auxiliary malloc failed\n");
     
        goto EXIT_LABEL;
        }

    while (NULL != httpGenListIterator (listId, &entryId))
        {
        printf ("%s", pLeftMargin);
        printf ("%6spEntry = %p\n", "", entryId);

        authId = (char *)httpGenListIdGet (entryId);

        printf ("%s", pLeftMargin);
        printf ("%10sid = %s\n", "", authId);

        printf ("%s", pLeftMargin);
        printf ("%8sdata = %s\n", "", (char *)(httpGenListDataGet (entryId)));

        if  (HTTP_OK == httpPwdDecode (authId, szName, (unsigned short)sSize))
            {
            printf ("%s", pLeftMargin);
            printf ("%8suser = %s\n", "", szName);
            printf ("%s", pLeftMargin);
            printf ("%4spassword = %s\n", "", szName+strlen (szName)+1);

            /*
             * even though it is not clearly advertised upon successful
             * execution of httpPwdDecode we can access both user and password
             */

            }
        else
            {
            printf ("%s", pLeftMargin);
            printf ("%4shttpPwdDecode failed\n", "");
            }

        printf ("%s", pLeftMargin);
        printf ("%4s--------\n", "");
        }

EXIT_LABEL:

    if  (NULL != szName)
        {
        free (szName);
        }

    return;

    } /* authorizationListShow */

/***************************************************************************
*
* httpPasswordListShow - displays user/password pairs for all access groups
*
* The HTTP Password configuration list is a list of lists indexed by user
* group names. For each group httpPasswordListShow displays all member
* user password pair.
*
* RETURNS: NA
*
* NOMANUAL
*/

void httpPasswordListShow
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID entryId = NULL;
    char auxHeader[32];
    char auxLeftMargin[32];

    /* we always print a header even if the list is empty */

    printf ("httpPasswordListShow: p = %p\n", httpPwdConfigId);

    printf ("%4s------\n", "");

    if  (NULL == httpPwdConfigId)
        {
        printf ("%4shttpPasswordList is empty\n", "");

        printf ("%4s------\n", "");

        return;
        }

    (void) httpPwdConfLock();

    /* walking the configuration list */

    while (NULL != httpGenListIterator (httpPwdConfigId, &entryId))
        {
        printf ("%4spEntry = %p\n", "", entryId);

        printf ("%8sid = %s\n", "", httpGenListIdGet (entryId));

        printf ("%6sdata = %p\n", "", httpGenListDataGet (entryId));

        if  (strlen (httpGenListIdGet (entryId)) <
             sizeof (auxHeader) - strlen (" group authorization"))
            {
            (void)snprintf (auxHeader, 31,
                     "%s group authorization",
                     httpGenListIdGet (entryId));
            }
        else
            {
            (void)snprintf (auxHeader, 31,
                     "%s group authorization",
                     "name_too_long_to_be_displayed");
            }

        (void)snprintf (auxLeftMargin, 31, "%13s", "");
        /*
         * 13 == strlen ("    pEntry = ")
         *    == strlen ("       sid = ")
         *    == strlen ("      data = ")
         */

        authorizationListShow (httpGenListDataGet (entryId),
                               auxHeader,
                               auxLeftMargin);

        printf ("%4s------\n", "");
        }

    (void)httpPwdConfUnlock();

    return;

    } /* httpPasswordListShow */
#endif /* HTTP_SHOW */
#endif

