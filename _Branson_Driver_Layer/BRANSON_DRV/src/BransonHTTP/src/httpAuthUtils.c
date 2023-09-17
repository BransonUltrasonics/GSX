/* httpAuthUtils.c - access functions for authentication */

/* Copyright (c) 1996-2003,2009-2014 Wind River Systems, Inc.
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
21feb13,r_w  fix static analysis issue(defect WIND00404092)
28jul10,m_z  eliminate compile warning
12jul10,m_z  change location auth. to avoid sub location being shielded
             (WIND00206095)
02feb10,h_y  support for LP64
04mar09,m_z  change GID information content
             (WIND00157654)
07aug03,jws  minor fix-ups
19jan00,csr  updating documentation API.
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
06apr98,ck   new Mime header access fns applied, understood, bugfixed.
14may97,mb   osal introduced.
12may97,ck   merged against wrs changes.
11oct96,ck   documentation.
28sep96,mb   written.
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for authentication.

INCLUDE FILES:
	httpLib.h
	private/httpTcp.h
	private/httpReq.h
	private/httpInt.h


*/

#include <stdio.h>
#include <string.h>

#include "httpLib.h"
#include "httpTcp.h"
#include "httpReq.h"
#include "private/httpInt.h"

static const char * const szMimeAuthorizationEntityKey  = "Authorization";
static const char * const szMimeAuthenticationEntityKey = "WWW-Authenticate";

HTTP_STATUS httpMimeAuthorization
    (
    HTTP_REQ_ID  reqId,    /* the client request to process           */
    char *       szLine    /* the line containing authorization field */
    );
extern void httpLocCandCountReset(HTTP_REQ_ID reqId); 
extern void httpLocCandCountInc(HTTP_REQ_ID reqId);
extern void httpLocCandCountDec(HTTP_REQ_ID reqId);


/**************************************************************************
*
* httpMimeAuthorization - Parse MIME Authorization field for user name/password.
*
* This routine parses the MIME Authorization header field for the
* authentication scheme, user name, and password.  The entire header
* is saved in the request's `szAuth' header field. The authentication
* scheme is stored in the AUTH_TYPE server environment variable.
*
* NOTE: For base64 authorization, the authorization string will contain
* 'basic <encrypted string>'.
*
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeAuthorization
    (
    HTTP_REQ_ID  reqId,    /* the client request to process           */
    char *       szLine    /* the line containing authorization field */
    )
    {
    char * szTmp;
    char * szAuthType;

    MEM_POOL_ID reqMemId = httpReqMemPartIdGet ( reqId );

    /* strip off leading spaces */

    for (szTmp = szLine ; ((szTmp[0] != 0) && (szTmp[0]==' ')) ; szTmp++);

    if (strlen (szTmp) > 80)         /* password was too long, ignored */
        return (HTTP_OK);

    (void) httpMimeHdrSet (reqId, HDR_IN, szMimeAuthorizationEntityKey, szTmp);

    if ((szAuthType = httpStrdup (reqMemId, szTmp)) != NULL)
        {

        /*
         * Tried to understand, what this is good for.... (ck)
         * from the browser, you get authorization: basic 23408lkjfsl93
         * the "authorization:" was already stripped, now we only
         * need "basic" .... that's what we do here...
         */

        if ( ( szTmp = strchr ( szAuthType , ' ' ) ) != NULL )
            szTmp[0] = 0;

        httpSetEnv ( reqId , "AUTH_TYPE" , szAuthType );
        (void)memPoolFree ( reqMemId , szAuthType );
        }

    return (HTTP_OK);
    }


/**************************************************************************
*
* httpAuthorizationGet - Get the MIME Authorization header field.
*
* This routine gets the MIME Authorization header field.  This call is used to
* get the client authorization string, which is part of the MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the response
* header.  The header is of the form `Authorization: basic {encoded string}'.
*
* RETURNS: Pointer to authorization or an empty string.
*/

char * httpAuthorizationGet
    (
    HTTP_REQ_ID     reqId,     /* Handle of the active request */
    unsigned short  usSelect   /* HDR_IN or HDR_OUT            */
    )
    {
    return (char *) httpMimeHdrGet (reqId, usSelect,
                                    szMimeAuthorizationEntityKey);
    }


/**************************************************************************
*
* httpAuthenticationGet - Get the MIME WWW-Authenticate header field.
*
* This routine gets the client authentication string,
* which is part of the MIME header.
* Use HDR_IN for the incoming MIME header, or HDR_OUT for the
* response header.
*
* RETURNS: Pointer to authentication string or an empty string.
*/

char * httpAuthenticationGet
    (
    HTTP_REQ_ID    reqId,    /* Handle of the active request */
    unsigned short usSelect  /* HDR_IN or HDR_OUT            */
    )
    {
    return (char *) httpMimeHdrGet (reqId, usSelect,
                                    szMimeAuthenticationEntityKey);
    }


/**************************************************************************
*
* httpAuthenticationSet - Set the MIME WWW-Authenticate header field.
*
* This routine sets the header field that tells the client which <realm> it
* should authenticate for. Select HDR_IN for the incoming MIME header,
* or HDR_OUT for the response header.
*
* RETURNS: HTTP_OK if the set operation was successful, or HTTP_ERROR if not.
*/

HTTP_STATUS httpAuthenticationSet
    (
    HTTP_REQ_ID      reqId,     /* Handle of the active request */
    unsigned short   usSelect,  /* HDR_IN or HDR_OUT            */
    char *           szAuth     /* Authentication String        */
    )
    {
    return (httpMimeHdrSet (reqId, usSelect,
                            szMimeAuthenticationEntityKey,
                            szAuth));
    }


/**************************************************************************
*
* httpLocationSet - Set the location (IP pattern) in the request structure.
*
* This routine sets the (IP pattern) location string in the request structure.
* This string is evaluated by the access control RPM.
* The string should match the one in the table aRpmIPConfigTab[ ].
* Typically, this function is used for implementing an IP pattern checker.
*
* RETURNS: HTTP_OK if the set operation was successful, or HTTP_ERROR if not.
*/

HTTP_STATUS httpLocationSet
    (
    HTTP_REQ_ID  reqId,
    char *       szLocation
    )
    {
    reqId->szLocation = szLocation;
    return (HTTP_OK);
    }


/**************************************************************************
*
* httpLocationGet - Get the location (IP pattern) from the request structure.
*
* This routine gets the (IP pattern) location string from the request structure.
* This string is typically evaluated by the access control RPM or any
* user-specific logging module.
* The string matches one in the table 'aRpmIPConfigTab[ ]'.
*
* RETURNS: A pointer to the location string.
*/

char * httpLocationGet
    (
    HTTP_REQ_ID   reqId
    )
    {
    return (reqId->szLocation);
    }

/**************************************************************************
*
* httpLocCandidateAdd - Set the possible location in the request structure.
*
* This routine sets the possible location string in the request structure.
* This string is evaluated by the access control RPM.
* The string should match the one in the table aRpmIPConfigTab[ ].
* Typically, this function is used for implementing an IP pattern checker.
*
* RETURNS: HTTP_OK if the set operation was successful, or HTTP_ERROR if not.
*/

HTTP_STATUS httpLocCandEntryAdd
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID locEntryId
    )
    {
    locEntryId->nextId = reqId->locCandEntryList;
    reqId->locCandEntryList = locEntryId;
    return (HTTP_OK);
    }

/**************************************************************************
*
* httpLocCandidateGet - Get the possible location in the request structure.
*
* This routine gets the possible location string in the request structure.
* This string is evaluated by the access control RPM.
* The string should match the one in the table aRpmIPConfigTab[ ].
* Typically, this function is used for implementing an IP pattern checker.
*
* RETURNS: HTTP_OK if the set operation was successful, or HTTP_ERROR if not.
*/

HTTP_GEN_LIST_ENTRY_ID httpLocCandEntryGet
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID * locEntryId
    )
    {
    if (NULL == locEntryId)
        return reqId->locCandEntryList;

    if (NULL == *locEntryId)
        *locEntryId = reqId->locCandEntryList;
    else
        *locEntryId = (*locEntryId)->nextId;

    return (*locEntryId);
     }

/**************************************************************************
*
* httpLocCandEntryDataGet - get  possible location's data from the request structure.
*
* This routine get possible location data from the request structure.	This is 
* typically for evaluating by the access control RPM or any location-specific logging module.
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: GID value - string
*/

char * httpLocCandEntryDataGet
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID locEntryId)
    {
    return ((HTTP_GEN_LIST_ENTRY_ID)(locEntryId->listItem.pData))->szId;
    }


/**************************************************************************
*
* httpLocCandEntryRemove - Remove possible location's entry from the list of the request structure.
*
* This routine remove possible location entry into the request structure.  
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: GID entry
*/

HTTP_GEN_LIST_ENTRY_ID httpLocCandEntryRemove 
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID * locEntryId)
    {
    HTTP_GEN_LIST_ENTRY_ID curEntryId = NULL;
    HTTP_GEN_LIST_ENTRY_ID preEntryId = NULL;

    if (NULL == locEntryId)
        {
        preEntryId = reqId->locCandEntryList;
        if (NULL != reqId->locCandEntryList)
            reqId->locCandEntryList = reqId->locCandEntryList->nextId;
        return preEntryId;
        }

    if (NULL == *locEntryId || reqId->locCandEntryList == *locEntryId)
        {
        preEntryId = reqId->locCandEntryList;
        if (NULL != reqId->locCandEntryList)
            reqId->locCandEntryList = reqId->locCandEntryList->nextId;
        *locEntryId = preEntryId;
        return preEntryId;
        }
/*
    if (NULL != preEntryId)
        curEntryId = preEntryId->nextId;

    while (NULL != curEntryId)
        {
        if (curEntryId == *locEntryId)
            break;
        preEntryId = curEntryId;
        curEntryId = preEntryId->nextId;
        }

    if (NULL != curEntryId)
        {
        preEntryId->nextId = curEntryId->nextId;
        curEntryId->nextId = 0;
        }
*/

    return curEntryId;
    }

void httpLocCandCountReset(HTTP_REQ_ID reqId)
{
    reqId->usLocCandEntryCount=0;
}

void httpLocCandCountDec(HTTP_REQ_ID reqId)
{
    reqId->usLocCandEntryCount--;
}
void httpLocCandCountInc(HTTP_REQ_ID reqId)
{
    reqId->usLocCandEntryCount++;
}
/**************************************************************************
*
* httpGidEntryInsert - Insert  (user) group ID's entry to the request structure.
*
* This routine inserts password-passed  (user) group ID entry into the request structure.  This
* list replaces the old Gid string, which is typically for evaluating by the access control RPM 
* or any user-specific logging module.
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: HTTP_OK if the set operation was successful, or HTTP_ERROR if not.
*/

HTTP_STATUS httpGidEntryInsert
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID gidEntryId)
    {
    gidEntryId->nextId = reqId->gidEntryList;
    reqId->gidEntryList = gidEntryId;
    return (HTTP_OK);
    }

	
/**************************************************************************
*
* httpGidEntryGet - locate  (user) group ID's entry from the request structure.
*
* This routine locate password-passed  (user) group ID entry from the request structure. This
* list replaces the old Gid string, which is typically for evaluating by the access control RPM
* or any user-specific logging module.
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: GID entry
*/

HTTP_GEN_LIST_ENTRY_ID httpGidEntryGet
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID * gidEntryId)
    {
    if (NULL == gidEntryId)
        return reqId->gidEntryList;

    if (NULL == *gidEntryId)
        *gidEntryId = reqId->gidEntryList;
    else
        *gidEntryId = (*gidEntryId)->nextId;

    return (*gidEntryId);
    }

/**************************************************************************
*
* httpGidEntryDataGet - get  (user) group ID's data from the request structure.
*
* This routine get password-passed  (user) group ID data from the request structure.	This is 
* typically for evaluating by the access control RPM or any user-specific logging module.
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: GID value - string
*/

char * httpGidEntryDataGet
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID gidEntryId)
    {
    return ((HTTP_GEN_LIST_ENTRY_ID)(gidEntryId->listItem.pData))->szId;
    }

/**************************************************************************
*
* httpGidEntryRemove - Remove (user) group ID's entry from the list of the request structure.
*
* This routine remove password-passed  (user) group ID entry into the request structure.  This
* list replaces the old Gid string, which is typically for evaluating by the access control RPM
* or any user-specific logging module.
* 
* This routine is used for implementing an authentication RPM.
*
* RETURNS: GID entry
*/

HTTP_GEN_LIST_ENTRY_ID httpGidEntryRemove 
    (
    HTTP_REQ_ID reqId, 
    HTTP_GEN_LIST_ENTRY_ID * gidEntryId)
    {
    HTTP_GEN_LIST_ENTRY_ID curEntryId = NULL;
    HTTP_GEN_LIST_ENTRY_ID preEntryId = NULL;

    if (NULL == gidEntryId)
        {
        preEntryId = reqId->gidEntryList;
        if (NULL != reqId->gidEntryList)
            reqId->gidEntryList = reqId->gidEntryList->nextId;
        return preEntryId;
        }

    if (NULL == *gidEntryId || reqId->gidEntryList == *gidEntryId)
        {
        preEntryId = reqId->gidEntryList;
        if (NULL != reqId->gidEntryList)
            reqId->gidEntryList = reqId->gidEntryList->nextId;
        *gidEntryId = preEntryId;
        return preEntryId;
        }
/*    if (NULL != preEntryId)
        curEntryId = preEntryId->nextId;
    while (NULL != curEntryId)
        {
        if (curEntryId == *gidEntryId)
            break;
        preEntryId = curEntryId;
        curEntryId = preEntryId->nextId;
        }

    if (NULL != curEntryId)
        {
        preEntryId->nextId = curEntryId->nextId;
        curEntryId->nextId = 0;
        }
*/
    return curEntryId;
    }

