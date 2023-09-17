/* httpRpmDispatch.c - RPM dispatcher  */

/* Copyright 1999-2004,2010 Wind River Systems, Inc. */

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
26sep14,r_w  fix static CHECKED_RETURN issue.
26jan10,h_y  support for LP64
04mar09,m_z  delete GID string initiating
                 (WIND00157654)
13feb07,msa  Fix comments to remove APIgen errors.
09mar04,vmk  suppressed multiple http error responses
24feb04,adb  annihilated httpRpmDispatchConfigId after clean-up
14jan04,gsr  cleanup routine for SPR 93009
10nov03,adb  WRS coding conventions modifications
31oct03,adb  added show routines
01oct03,jws  remove assert()'s
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01sep97,mb   double error messages error removed.
13may97,mb   osal introduced.
11oct96,mb   documentation added.
03sep96,mb   initial creation.
*/

/*
DESCRIPTION
The RPM dispatcher calls the RPMs in the configured order with the HTTP_REQ_ID
handle as argument. Each RPM must return a processing status to the dispatcher
indicating how further processing of this request should be done. There are four
possible processing status values:
\is
\i RPM_OK
   Indicates that the current RPM is finished processing the request, and the
   request can be passed on to the next RPM for further processing.

\i RPM_DONE
   Indicates that processing is complete for this request. No other RPMs will
   be called for this request.

\i RPM_RESTART
   Restarts processing of the request.

\i RPM_ERROR
   Indicates an ERROR during request processing.

\ie
INCLUDE FILES:
	httpLib.h
	httpRpms.h
	private/httpReq.h
	private/httpInt.h
    httpShow.h
*/

/* includes */

#include <string.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpShow.h"
#include "commonCfg.h"

/* locals */

LOCAL unsigned short   httpDispatchTabLimit;
LOCAL HTTP_GEN_LIST_ID httpRpmDispatchConfigId;


/***************************************************************************
*
* httpRpmConfLock - lock the Rpm dispatch configuration
* This routine locks the rpm dispath configuration data.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpRpmConfUnlock(), httpOsalSemTake(), httpGenListLock()
*/

HTTP_STATUS httpRpmConfLock (void)
    {
    return ( httpGenListLock ( httpRpmDispatchConfigId ) );
    }

/***************************************************************************
*
* httpRpmConfUnlock - unlock the Rpm dispatch configuration
*
* This routine unlocks the rpm dispatch configuration data.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*
* SEE ALSO: httpRpmConfLock()
*/

HTTP_STATUS httpRpmConfUnlock (void)
    {
    return httpGenListUnlock ( httpRpmDispatchConfigId);
    }

/***************************************************************************
*
* httpRpmConfInit - Initialize the sizes of RPM configuration tables.
*
* This routine initializes the internal boundaries required by
* other routines to access the  RPM configuration tables.
*
* This routine specifies how many HTTP methods (such as GET,POST,and HEAD) will
* be used, and how many RPMs will be used for each method.
*
* RETURNS: The list ID of the RPM dispatcher, or NULL if initialization fails.
*
* SEE ALSO: httpRpmConfAdd()
*/

HTTP_GEN_LIST_ID httpRpmConfInit
    (
    unsigned short usMethods,        /* max # of supported HTTP methods to dispatch      */
    unsigned short usDispatchTabSize /* max # of RPMs registered for one distinct method */
    )
    {
    httpRpmDispatchConfigId = NULL;

    if ( usMethods < HTTP_M_MAX )
    httpRpmDispatchConfigId = httpGenListCreate ( httpGlobalMemPartIdGet() , usMethods , HTTP_TRUE );

    if ( httpRpmDispatchConfigId != NULL )
    {
    httpDispatchTabLimit = usDispatchTabSize;
    }
    return ( httpRpmDispatchConfigId );
    }

/***************************************************************************
*
* httpRpmConfCleanup - Cleanup routine for httpRpmConfInit()
*
* RETURNS: HTTP_OK
*
* SEE ALSO: httpRpmConfInit()
*/

HTTP_STATUS httpRpmConfCleanup
    (
    void
    )
    {
    if  (httpRpmDispatchConfigId != NULL)
        {

         HTTP_GEN_LIST_ENTRY_ID entryId = NULL;
 
         /* walking the configuration list */
         while (NULL != httpGenListIterator (httpRpmDispatchConfigId, &entryId))
             {
             httpRpmMethodDelete(entryId);
             entryId = NULL;
             }
        httpGenListDestroy (httpRpmDispatchConfigId);

        httpRpmDispatchConfigId = NULL;
        } 

    return HTTP_OK;
    }

/***************************************************************************
*
* httpRpmConfAdd - Associate an RPM with a URL substring.
*
* This routine associates an RPM with specified URL substring and an 
* HTTP method.
*
* NOTE: To support the HEAD method in the HTTP protocol, it is neccessary to
* perform an additional httpRpmConfAdd() (HTTP_M_HEAD, and so on) for most
* RPMs.
*
* ATTENTION: The <szUrl> string is not copied.  Only a pointer to its
* location is stored.
*
* RETURNS: An entry ID of new added RPM.
*
* SEE ALSO: httpRpmConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpRpmConfAdd
    (
    short     sMethod,    /* Method for which to add this RPM       */
    char *    szUrl,      /* URL for which this RPM will be invoked */
    HTTP_RPM  pRpm        /* Pointer to the URL-function            */
    )
    {
    HTTP_GEN_LIST_ID       methodListId;
    HTTP_GEN_LIST_ENTRY_ID methodEntryId,rpmEntryId;

    if ( pRpm != NULL )
    {
    if ((methodEntryId = httpGenListFind ( httpRpmDispatchConfigId , (char*)httpMethods[sMethod].szName )) == NULL)
        {
        if (( methodListId = httpGenListCreate ( httpGlobalMemPartIdGet() , httpDispatchTabLimit , HTTP_TRUE )) == NULL )
        {
        logErrorMsg ( "httpRpmConfAdd: cannot create method %s!",httpMethods[sMethod].szName );
        return NULL;
        }
        if (httpGenListDataAppend ( httpRpmDispatchConfigId , (char*)httpMethods[sMethod].szName , methodListId ) == NULL )
        {
        logErrorMsg ( "httpRpmConfAdd: cannot add method %s to dispatcher list!",httpMethods[sMethod].szName );
        return NULL;
        }
        }
    else
        {
        methodListId = httpGenListDataGet ( methodEntryId );
        }

    if (( rpmEntryId = httpGenListFuncAppend ( methodListId , szUrl , (HTTP_FUNCPTR)pRpm )) != NULL)
        {
        logInfoMsg ( "httpRpmConfAdd: RPM mounted at %s" , szUrl );
        return rpmEntryId;
        }
    }
    logErrorMsg ( "httpRpmConfAdd: URL %s not mounted!" , szUrl );
    return NULL;
    }

/***************************************************************************
*
* httpRpmMethodStringGet - retrieve a method name
*
* This routine retrieves a method name.
*
* RETURNS: A pointer to a string.
*/

char * httpRpmMethodStringGet
    (
    HTTP_GEN_LIST_ENTRY_ID methodEntryId
    )
    {
    return httpGenListIdGet (methodEntryId);
    }

/***************************************************************************
*
* httpRpmConfDelete - delete an rpm  and URL from a Method
*
* This routine removes the requested rpm and its associated URL from the method
*
* RETURNS: N/A
*/

void httpRpmConfDelete
    (
    HTTP_GEN_LIST_ENTRY_ID methodEntryId,
    HTTP_GEN_LIST_ENTRY_ID rpmEntryId
    )
    {
    (void) httpRpmConfLock();

#ifdef DEBUG_INFO
    printf ("httpCtrlConfDelete: removing access %s from method %s.\n",httpGenListIdGet (rpmEntryId),httpGenListIdGet (methodEntryId));
#endif

    httpGenListRemove (httpGenListDataGet (methodEntryId) , rpmEntryId);
    (void) httpRpmConfUnlock();
    }

/***************************************************************************
*
* httpRpmMethodDelete - delete a Method
*
* This routine removes all entries from a Method, and deletes
* the Method itself
*
* RETURNS: N/A
*/

void httpRpmMethodDelete
    (
    HTTP_GEN_LIST_ENTRY_ID methodEntryId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID rpmEntryId = NULL;
    HTTP_GEN_LIST_ID       rpmListId;

#ifdef DEBUG_INFO
    printf ("httpRpmMethodDelete: removing method %s.\n",httpRpmMethodStringGet (methodEntryId));
#endif
    (void) httpRpmConfLock ();
    if ((rpmListId = httpGenListDataGet (methodEntryId)) != NULL)
    while (httpGenListIterator (rpmListId , &rpmEntryId) != NULL)
        {
        httpRpmConfDelete (methodEntryId , rpmEntryId);
        rpmEntryId = NULL;
        }
    httpGenListDestroy (httpGenListDataGet (methodEntryId));
    httpGenListRemove (httpRpmDispatchConfigId , methodEntryId);
    (void) httpRpmConfUnlock ();
    }


/***************************************************************************
*
* httpDispatcher - Calls all RPMs in the specified order.
*
* The RPM Dispatcher calls all system and user RPMs.
* Argument to all RPMs is HTTP_REQ *
*
* NOMANUAL
*
* RETURNS: HTTP_OK if request processing succeded, HTTP_ERROR otherwise
*/

HTTP_STATUS httpDispatcher
    (
    HTTP_REQ_ID reqId
    )
    {
    short                   sResult;
    unsigned short          i = 0;
    HTTP_RPM                pRpm;
    HTTP_GEN_LIST_ID        methodListId;
    HTTP_GEN_LIST_ENTRY_ID  methodEntryId;
    HTTP_GEN_LIST_ENTRY_ID  dispatchEntryId;

    /* process all System RPMs as given in httpSystemRpms[] in Config.c */
    (void) httpLocationSet (reqId,(char *)httpDefLocationString);

    /* dispatch system RPMs */
    while ((pRpm = httpSystemRpms[i++]) != NULL)
        {
        sResult = pRpm (reqId);
        switch (sResult)
               {
            case RPM_OK:
                break;
            case RPM_DONE:
                return (HTTP_OK);
            case RPM_RESTART:
                pRpm = httpSystemRpms[ i=0 ];
                continue;
            case RPM_ERROR:
            default:
            	if (reqId->fHdrWritten == HTTP_FALSE)
             	   {
               	   reqId->sStatus =  HTTP_INTERNAL_ERROR;
                   httpError (reqId , httpErrStringGet(HTTP_ERROR_INTERNAL));
                   }
                return (HTTP_ERROR);
            }
        }

    /* find dispatcher list for requested HTTP method */
    if (( dispatchEntryId = httpGenListFind ( httpRpmDispatchConfigId , (char*)httpMethods[reqId->sMethod].szName )) == NULL)
        {
        reqId->sStatus =  HTTP_NOT_IMPLEMENTED;
        (void) httpError (reqId , httpErrStringGet(HTTP_ERROR_NO_RPM));
        return (HTTP_ERROR);
        }

    methodListId = httpGenListDataGet ( dispatchEntryId );

    /* process all URL specific RPMs dependend on method and URL */
    methodEntryId = NULL;
    while (httpGenListIterator ( methodListId , &methodEntryId ) != NULL)
        {
    pRpm = (HTTP_RPM)httpGenListFuncGet ( methodEntryId );
    while (pRpm != NULL)
            {
            /* RPM idetifiers MUST BE sorted by longest idetifier first in "config.c" */
            if ( strstr (reqId->szAbsPath,httpGenListIdGet ( methodEntryId )) != reqId->szAbsPath )
                break;

            reqId->szRpmIdent = httpGenListIdGet ( methodEntryId );
            reqId->szRpmData = &reqId->szAbsPath[ strlen (reqId->szRpmIdent) ];
            sResult = pRpm (reqId);
            switch (sResult)
                {
                case RPM_OK:
                    break;
                case RPM_DONE:
                    return (HTTP_OK);
                case RPM_RESTART:
            logInfo ( "httpDispatcher: restarting URL specific dispatching" );
            methodEntryId = NULL;
            if (httpGenListIterator ( methodListId , &methodEntryId ) != NULL)
            pRpm = (HTTP_RPM)httpGenListFuncGet ( methodEntryId );
                    continue;
                case RPM_ERROR:
            return (HTTP_ERROR);
                default:
            if ( reqId->fHdrWritten == HTTP_FALSE )
            {
            reqId->sStatus =  HTTP_INTERNAL_ERROR;
            httpError (reqId , httpErrStringGet(HTTP_ERROR_INTERNAL));
            }


            return (HTTP_ERROR);
                }
            break;
            }
        }

    /* no RPM for this URL and method found */
    if ( reqId->fHdrWritten == HTTP_FALSE )
        {
        reqId->sStatus = HTTP_NOT_IMPLEMENTED;
        httpError (reqId , httpErrStringGet (HTTP_ERROR_NO_RPM));
        }
    return (HTTP_ERROR);
    
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpSystemRpmsShow - displays system RPMs
*
* HTTP System RPMs are kept in an array terminated by a NULL entry.
* This function simply displays the non-NULL entries of this array.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpSystemRpmsShow 
    (
    void
    )
    {
    HTTP_RPM    pRpm;  
    int         i = 0;

    printf ("httpSystemRpmsShow\n");
    printf ("------------------\n");

    while ((pRpm = httpSystemRpms[i]) != NULL)
        {
        printf ("i = %2d, system RPM = %p\n", i, pRpm);
    
        i++;
        }

    printf ("------------------\n");

    return;
    } /* httpSystemRpmsShow */

/***************************************************************************
*
* httpRpmDispatchListShow - displays all non-system RPMs grouped by method
*
* The RPMs are kept in a list indexed by HTTP method name with elements lists
* with entries indexed by URL prefixes and data the corresponding internally
* resolvable executable function pointers.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpRpmDispatchListShow
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID  entryId = NULL;
    char auxHeader[32];
    char auxLeftMargin[32];

    /* we always print a header even if the list is empty */

    printf ("httpRpmDispatchListShow: p = %p\n", httpRpmDispatchConfigId);

    printf ("%4s------\n", "");

    if  (NULL == httpRpmDispatchConfigId)
        {
        printf ("httpRpmDispatchList is empty\n");
        
        printf ("%4s------\n", "");

        return;
        }

    while (NULL != httpGenListIterator (httpRpmDispatchConfigId, &entryId))
        {
        printf ("%4spEntry = %p\n", "", entryId);       

        printf ("%8sid = %s\n", "", httpGenListIdGet (entryId));

        printf ("%6sdata = %p\n", "", httpGenListDataGet (entryId));

        /* prepare httpListShow header and left margin auxiliary strings */

        if  (strlen (httpGenListIdGet (entryId)) < 
             sizeof (auxHeader) - strlen (" RPMs") - 1)
            {
            sprintf (auxHeader, "%s RPMs", httpGenListIdGet (entryId));
            }
        else
            {
            sprintf (auxHeader, "%s RPMs", "name_too_long_to_be_displayed");
            }

        sprintf (auxLeftMargin, "%13s", ""); 
        /* 
         * 13 == strlen ("    pEntry = ")
         *    == strlen ("       sid = ")
         *    == strlen ("      data = ") 
         */

        httpListShow (httpGenListDataGet (entryId),
                      auxHeader, 
                      FALSE,    /* entry id is a string */
                      TRUE,     /* entry data is a pointer */
                      auxLeftMargin);

        printf ("%4s------\n", "");
        }

    return;
    
    } /* httpRpmDispatchListShow */

/***************************************************************************
*
* httpFirstMethodsShow - displays the first <input> registered HTTP methods
*
* The registered HTTP methods are kept in a table which has programmatically
* unknown size. Calling this function may lead to an exception. This is not
* a problem since the intended usage is only tracing and debugging. Currently
* -nov03- there are seven HTTP registered methods.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpFirstMethodsShow 
    (
    unsigned int numberOfHttpMethods
    )
    {
    unsigned int i = 0;

    printf ("httpFirstMethodsShow\n");
    printf ("--------------------\n");

    printf ("do not request more that the number of registered HTTP methds\n");

    for (i = 0; i < numberOfHttpMethods+1; i++)
        {
        printf ("i = %2d: mNumber = %4d, szName = %s\n", 
                i, httpMethods[i].mNumber, httpMethods[i].szName);
        }

    printf ("--------------------\n");

    return;
    } /* httpFirstMethodsShow */

/***************************************************************************
*
* httpFirstMethodRpmsShow - displays the serving RPMs of the first HTTP methods
*
* The registered HTTP methods are kept in a table which has programmatically
* unknown size. Calling this function may lead to an exception. This is not
* a problem since the intended usage is only tracing and debugging. Currently
* -nov03- there are seven HTTP registered methods. For each registered HTTP
* method indexed by a number smaller than the <input> the HTTP RPM Dispatch 
* list is searched and the corresponding URL prefixes and their matching
* internally resolvable executable service function pointers are displayed.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpFirstMethodRpmsShow
    (
    unsigned int numberOfHttpMethods
    )
    {
    HTTP_GEN_LIST_ID        methodListId;
    HTTP_GEN_LIST_ENTRY_ID  methodEntryId;
    HTTP_RPM                pRpm;
    unsigned int            i = 0;

    printf ("httpFirstMethodRpmsShow\n");
    printf ("-----------------------\n");

    printf ("do not request more that the number of registered HTTP methds\n");

    for (i = 0; i < numberOfHttpMethods+1; i++)
        {
        printf ("i = %2d: mNumber = %4d, szName = %s\n", 
                i, httpMethods[i].mNumber, httpMethods[i].szName);

        methodListId = 
        (httpGenListDataGet 
         (httpGenListFind 
          (httpRpmDispatchConfigId, (char*)httpMethods[i].szName)));

        if  (NULL == methodListId)
            {
            printf ("%8sNULL methodListId\n", "");
            continue;
            }

        printf ("%8smethodListId = %p\n", "", methodListId);

        methodEntryId = NULL;

        while (NULL != httpGenListIterator (methodListId, &methodEntryId))
            {
            pRpm = (HTTP_RPM)httpGenListFuncGet (methodEntryId);

            printf ("%12smethodEntryId = %p, pRpm = %p, name = %s\n",
                    "", methodEntryId, pRpm, httpGenListIdGet (methodEntryId));
            }
        }

    printf ("-----------------------\n");

    return;

    } /* httpFirstMethodRpmsShow */
#endif /* HTTP_SHOW */
