/* httpLib.c - Very central main loop of the HTTP server   */

/*
 * Copyright (c) 2003-2008,2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02f,26sep14,r_w  fix static CHECKED_RETURN issue.
02e,12sep13,f_f  ipv6 support improvement
02d,08apr13,r_w  fix static analysis issues for defect WIND00411995
02c,13feb12,r_w  fix defect WIND00332752 to correct log message
02b,30mar11,r_w  fix defect WIND00257291 make API reference readable
02a,27jan10,h_y  support for LP64
01p,01apr10,m_z  use macro for SSL port instead of constant (WIND00203570)
                 CQID: Fix WIND00203639
01o,12jun08,q_z  clear the error messages on the console(WIND00040494)
01n,22feb07,ps   apigen reformatting
01m,10may06,ps   fix for CQ:WIND00044340
01l,14may04,adb  removed mudOverOpenSSL inclusion
01k,26feb04,adb  introduced httpMudTasksDelete and httpMudInfrastructureDelete
01j,17feb04,adb  httpServerStart wraps httpServerCreate
01i,17feb04,adb  httpInit and httpShutdown will also create and delete MUD
01h,12feb04,adb  httpMudCreate, httpServerCreate and deletion modifications
01g,31jan04,adb  multiport functionality necessitated changes
01f,30jan04,adb  added httpPortAdd
01e,21jan04,adb  modified httpServerRequestHandler connection verification test
01d,13jan04,gsr  upgrade httpServerKill to get rid of memory leaks
01c,09jan04,jws  eliminate HTTP_PREDEFINE use
01b,16dec03,adb  corrected MUD over WMNET layering violation
01a,18jun03,jws  Mod history added; assert.h include removed.
*/

/*
DESCRIPTION

This is the main module of the HTTP server.
Its task is (depending on several compiler switches) to listen on
the sockets stored in an array (done in the OSAL layer). When a new
connection is established, the connection socket is added to the list.
When ther is incoming data from one of the other sockets, it is treated
as an incoming request.
Normally, the Sockets are put back in the array after successfully
handling the request. Only if there is a timeout, the connection has
been closed by the client, or the closing is requested by the client
(See RFC2068: 14.4) the corresponding socket is taken from the list.

The processing of incoming requests is done by a thread taken from the
(configurable) thread pool.

INCLUDE FILES: memPoolLib.h
               http/private/httpTcp.h
               http/private/httpReq.h
               http/httpLib.h
               http/private/httpInt.h
               http/httpTask.h
               http/httpRpms.h


*/

#include <stdio.h>
#include <string.h>
#include "errno.h"

/* include C standard library */
#include <stdlib.h>

#include "httpLib.h"
#include "httpTcp.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpTask.h"
#include "httpRpms.h"

#include "commonCfg.h"


#define MUD_MEM_SIZE        5000
#define IP_ADDRESS          0x5A000001

/*
 * With version 02a, support for persistent connections and chunked
 * transmissions is included. This resulted in a slight OSAL API change!
 */


/* globals */

const char httpNullByte = 0;    /* whoever needs *NULL, here it is! */
MUD_ID     httpMud = NULL;

#if 0         /* never referenced */

HTTP_REQ_ID * httpReqIdResource = NULL;

#endif

/* locals */

/* forward declarations */

LOCAL MUD_CONN_ACTION httpServerRequestHandler (MUD_CONN_ID pConn);
LOCAL MUD_CONN_ACTION httpServerErrorHandler (MUD_CONN_ID pConn);
LOCAL MUD_CONN_ACTION httpServerInitHandler (MUD_CONN_ID pConn);

/***************************************************************************
*
* httpMudCreate - HTTP MUD initialization routine
*
* RETURNS: HTTP_OK if HTTP mud was successfully initialized or HTTP_ERROR
*/
HTTP_STATUS httpMudCreate
    (
    void
    )
    {
    if  (NULL != httpMud)
        {
        if (globle_HTTP_DEBUG)
          printf ("httpMudCreate: initialized HTTP MUD was encountered\n");

        return (HTTP_ERROR);
        }

    if  (OK != mudInfrastructureCreate
               (NULL,                    /* automatically create MUD name     */
                NULL,                    /* global access restrictions        */
                httpMaxPorts,            /* maximum number of ports           */
                httpMaxConnections,      /* maximum number of connections     */
                httpSockTimeout,         /* socket timeout in seconds         */
                httpMaxProcesses,        /* number of threads                 */
                httpPriority,            /* priority (use 0 to use default)   */
                httpdTaskStackSize,      /* stack size (use 0 to use default) */
                httpGlobalMemPartSize,   /* use 0 to use own memory           */
                httpReqMemPartSize,      /* use 0 to use own memory           */
                &httpMud))
        {
        return (HTTP_ERROR);
        }
    else
        {
        if (globle_HTTP_DEBUG)
          printf ("httpMudCreate: MUD %p was created\n", httpMud);

        }

    return (HTTP_OK);

    } /* httpMudCreate */

/***************************************************************************
*
* httpServerCreate - HTTP server initialization routine
*
* This routine is called once at start-up.
*
* RETURNS: HTTP_OK if the server  started successfully or HTTP_ERROR
*/
HTTP_STATUS httpServerCreate
    (
    void
    )
    {
    HTTP_STATUS          rs =                HTTP_ERROR;

#ifdef INCLUDE_OPENSSL_SUPPORT    
    SSL_CONFIGURATION_ID pSSLConfiguration = NULL; /* crucial initialization */
#endif

    if  (NULL == httpMud)
        {
        if (globle_HTTP_DEBUG)
          printf ("httpServerCreate: HTTP MUD has not been created\n");

        return (HTTP_ERROR);
        }

    if  (httpSSLPort == httpPort)
        {
        if (globle_HTTP_DEBUG)
          printf ("httpServerCreate: parameter validation failed\n");

        return (HTTP_ERROR);
        }

#ifdef INCLUDE_OPENSSL_SUPPORT
    if  (80 == httpSSLPort)
        {
        if (globle_HTTP_DEBUG)
          printf ("httpServerCreate: invalid SSL configuration parameters\n");

        return (HTTP_ERROR);
        }
#endif /* INCLUDE_OPENSSL_SUPPORT */

    if  (0 !=  httpSSLPort)
#ifndef INCLUDE_OPENSSL_SUPPORT
        {
#ifdef _WRS_KERNEL
        (void)logMsg
        ("httpServerCreate: port = %d, INCLUDE_OPENSSL_SUPPORT undefined\n",
         httpSSLPort, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }
#else  /* INCLUDE_OPENSSL_SUPPORT */
        {
        if  (OK != mudSSLSupportInitialize (httpMud))
            {
            wmError
            (ERROR_GENERAL,
             "httpServerCreate: port = 443, mudSSLSupportInitialize failed");

            goto EXIT_LABEL;
            }

        if  (NULL == (pSSLConfiguration =
                      (SSL_CONFIGURATION *)
                      wmosMalloc (sizeof (SSL_CONFIGURATION))))
            {
            wmError (ERROR_GENERAL,
                     "httpServerCreate: auxiliary memory allocation failed");

            goto EXIT_LABEL;
            }

        pSSLConfiguration->pCertificateFile = httpSSLCertificateFile;

        pSSLConfiguration->pPrivateKeyFile =  httpSSLPrivateKeyFile;

/*
        if  (NULL != httpSSLCipherList)
            {
            pSSLConfiguration->pCipherList = httpSSLCipherList;
            }
        else
            {
            pSSLConfiguration->pCipherList = MUD_SSL_CIPHER_LIST;
            }

        if  (NULL != httpSSLCAFile)
            {
            pSSLConfiguration->pCAFile = httpSSLCAFile;
            }
        else
            {
            pSSLConfiguration->pCAFile = MUD_SSL_CA_FILE;
            }

        if  (NULL != httpSSLCAPath)
            {
            pSSLConfiguration->pCAPath = httpSSLCAPath;
            }
        else
            {
            pSSLConfiguration->pCAPath = MUD_SSL_CA_PATH;
            }
*/
        pSSLConfiguration->pCipherList = httpSSLCipherList;
        pSSLConfiguration->pCAFile = httpSSLCAFile;
        pSSLConfiguration->pCAPath = httpSSLCAPath;
#ifdef __WM_INET6__
					  if	(OK !=
							 mudPortAdd6 (httpMud,
										  httpSSLPort,			   /* SSL port			 */
										  NULL, 				   /* global access data */
										  pSSLConfiguration,	   /* SSL configuration  */
										  httpServerInitHandler,	/* connAcceptHandler  */
										  httpServerRequestHandler, /* connReceiveHandler */
										  httpServerErrorHandler,	/* connErrorHandler  */
										  NULL, 				   /* connCloseCallback  */
										  NULL					   /* connCloseClbkArg	 */
										  ))
							{
							/* no, if SSL ipv6 doesnt init,
							 * we still want to serve non-SSL requests
							 */
							/*   goto EXIT_LABEL;*/
							}
#endif
        if  (OK !=
             mudPortAdd (httpMud,
                         httpSSLPort,              /* SSL port           */
                         NULL,                     /* global access data */
                         pSSLConfiguration,        /* SSL configuration  */
                         httpServerInitHandler,    /* connAcceptHandler  */
                         httpServerRequestHandler, /* connReceiveHandler */
                         httpServerErrorHandler,   /* connErrorHandler   */
                         NULL,                     /* connCloseCallback  */
                         NULL                      /* connCloseClbkArg   */
                         ))
            {
            /* no, if SSL doesnt init,
             * we still want to serve non-SSL requests
             */
				goto EXIT_LABEL;
            }
			/* Stopping the HTTP port creation so that HTTP requests will not be processed*/
			httpPort = 0;
        }
#endif /* INCLUDE_OPENSSL_SUPPORT */

    if  (0 != httpPort)
        {
#ifdef __WM_INET6__
				if	(OK !=
					 mudPortAdd6 (httpMud,
								  httpPort, 			   /* port				 */
								  NULL, 				   /* global access data */
								  NULL			  , 	   /* SSL configuration  */
								  httpServerInitHandler,	/* connAcceptHandler  */
								  httpServerRequestHandler, /* connReceiveHandler */
								  httpServerErrorHandler,	/* connErrorHandler  */
								  NULL, 				   /* connCloseCallback  */
								  NULL					   /* connCloseClbkArg	 */
								 ))
					{
					/* no, if ipv6 doesnt init,
					 * we still want to serve ipv4 requests
					 */
					/* goto EXIT_LABEL; */
					}
#endif

        if  (OK !=
             mudPortAdd (httpMud,
                         httpPort,                 /* port               */
                         NULL,                     /* global access data */
                         NULL             ,        /* SSL configuration  */
                         httpServerInitHandler,    /* connAcceptHandler  */
                         httpServerRequestHandler, /* connReceiveHandler */
                         httpServerErrorHandler,   /* connErrorHandler   */
                         NULL,                     /* connCloseCallback  */
                         NULL                      /* connCloseClbkArg   */
                        ))
            {
            goto EXIT_LABEL;
            }
        }

    (void) mudStart(httpMud);
    rs = HTTP_OK;

EXIT_LABEL:

#ifdef INCLUDE_OPENSSL_SUPPORT
    if  (NULL != pSSLConfiguration)
        {
        wmosFree (pSSLConfiguration);
        }
#endif

    if  (HTTP_OK != rs)
        {
        /*
         * This is a policy decision!
         * Alternatively, we could simply return error or partially cleanup.
         */

        (void) httpShutdown ();

        /* we do not check the return code */
        }

    return (rs);

    } /* httpServerCreate */

/***************************************************************************
*
* httpPortAdd - add a port
*
* RETURNS: HTTP_OK or HTTP_ERROR
*/
HTTP_STATUS httpPortAdd
    (
    unsigned short port,
    SSL_CONFIGURATION_ID pSSLConfiguration
    )
    {
    if  (NULL == httpMud)
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpPortAdd: httpMud is NULL\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (OK != mudPortAdd (httpMud,
                           port,
                           NULL,
                           pSSLConfiguration,
                           httpServerInitHandler,
                           httpServerRequestHandler,
                           httpServerErrorHandler,
                           NULL,
                           NULL))
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpPortAdd: mudPortAdd failed\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    return (HTTP_OK);

    } /* httpPortAdd */

/***************************************************************************
*
* httpServerStart - deprecated
*
* RETURNS: HTTP_OK or HTTP_ERROR
*/
HTTP_STATUS httpServerStart
    (
    void
    )
    {
    if (NULL == httpMud)
        {
        if (globle_HTTP_DEBUG)
          printf("call httpServerCreate first\n");

        return HTTP_ERROR;
        }

    return (httpServerCreate ());
    }

/***************************************************************************
*
* httpServerInitHandler - called every time a new connection is established
*
* NOMANUAL
*
* RETURNS: CONN_CLOSE or CONN_KEEP
*/

LOCAL MUD_CONN_ACTION httpServerInitHandler
    (
    MUD_CONN_ID pConn
    )
    {
#if 0 /* legacy code violating VxWorks multi-threading conventions */
    /* clear errno */
    errno = 0;
#endif

    return CONN_KEEP;
    }

/***************************************************************************
*
* httpServerRequestHandler - the main request processing handler of the server
*
* This routine gets called by MUD everytime new data arrives on the port
* that MUD is listening to.
*
* NOMANUAL
*
* RETURNS: CONN_CLOSE or CONN_KEEP
*/

LOCAL MUD_CONN_ACTION httpServerRequestHandler
    (
    MUD_CONN_ID pConn
    )
    {
    HTTP_REQ_ID     reqId;
    MEM_POOL_ID     memPoolId = NULL; /* mem pool for this connection */
#if 0
#ifdef MUD_CONNECTION_VERIFICATION_IS_REQUIRED
    MUD_CONN_STATE  mudConnState = CONN_STATE_INVALID;
    MUD_CONN_STATUS mudConnStatus = MUD_CONN_INVALID;
#endif /* MUD_CONNECTION_VERIFICATION_IS_REQUIRED */
#endif

#if 0 /* legacy code violating VxWorks multi-threading conventions */
    /* clear errno */
    errno = 0;
#endif

#if 0
#ifdef MUD_CONNECTION_VERIFICATION_IS_REQUIRED
/* mudConnVerify and mudConnAccess precede within the body of guarded handler*/
    if  ((OK != mudConnStateGet (pConn, &mudConnState)) ||
         ((CONN_STATE_BUSY != mudConnState) &&
          (CONN_STATE_NEW != mudConnState)) ||
         (OK != mudConnStatusGet (pConn, &mudConnStatus)) ||
         (MUD_CONN_OK != mudConnStatus))
        {
        return CONN_CLOSE;
        }
#endif /* MUD_CONNECTION_VERIFICATION_IS_REQUIRED */
#endif

    if  ((OK != mudConnTempMemPoolIdGet (pConn, &memPoolId)) ||
         (NULL == memPoolId))
        {
        return CONN_CLOSE;
        };

    if  ((reqId = httpReqStructInit (memPoolId)) == NULL)
        {
        logError ("httpServerRequestHandler: initialization failed!");
        return CONN_CLOSE;
        }

    /* store in the request structure the MUD connection I/O handle */

    reqId->ioHandle = pConn;

    logInfoMsg ("Request structure for pConn %d initialized", reqId->ioHandle);

    if  (HTTP_ERROR == httpReqTrans (reqId))
        {
        return CONN_CLOSE;
        }

#ifdef DEBUG_INFO
    if  (strcmp (reqId->szAbsPath,"/quit") == 0)
        {
        httpStatusSet (reqId,HTTP_PAYMENT_REQ);
        httpError (reqId,"<H2>Client requested server termination</H2>");
        httpServerKill();
        }
#endif
        logInfoMsg("HTTP/1.%d", reqId->sProtocol);
        logInfoMsg("Dispatching URL! (%s)", reqId->szAbsPath);

        if  (RPM_ERROR == httpDispatcher (reqId))
            {
            logInfo ("dispatcher got an error from some RPM");
            }

    if  (reqId->fPersistent == HTTP_TRUE)
        {
        return CONN_KEEP;
        }
    else
        {
        return CONN_CLOSE;
        }

    } /* httpServerRequestHandler */


/***************************************************************************
*
* httpServerErrorHandler - the main error handler of the server
*
* This routine gets called by MUD everytime a connection error occurs
*
* NOMANUAL
*
* RETURNS: CONN_CLOSE or CONN_KEEP
*/

LOCAL MUD_CONN_ACTION httpServerErrorHandler
    (
    MUD_CONN_ID pConn
    )
    {
    MUD_CONN_STATUS mudConnStatus = MUD_CONN_INVALID;

    if  (OK != mudConnStatusGet (pConn, &mudConnStatus))
        {
        return (CONN_CLOSE);
        }

    switch (mudConnStatus)
        {
        case MUD_CONN_SYSTEM_LIMITS_EXCEEDED:
            /* !! Here send server busy message on connection !! */
            logInfo ("Maximum number of connections reached");
            return CONN_CLOSE;
           /* break;*/
        case MUD_CONN_CLOSED_BY_CLIENT:
            logInfo ("Connection closed by browser.  Closing server side too.");
            return CONN_CLOSE;
           /* break;*/
        case MUD_CONN_REQUESTS_PERMISSION_TO_CLOSE:
            logInfoMsg ("Dennied REQUEST_TO_CLOSE MUD_CONN %p\n", (int)pConn);
            return CONN_KEEP;
           /* break;*/
        case MUD_CONN_SOCKET_TIMEOUT:
            logInfoMsg ("Connection timed out. MUD_CONN %p\n", (int)pConn);
            return CONN_CLOSE;
           /* break;*/
        default:
            logInfoMsg ("Connection status number %d", mudConnStatus);
            return CONN_CLOSE;
           /* break;*/
        }
    }

/***************************************************************************
*
* httpMudTasksDelete - this will gracefully terminate HTTP MUD tasks
*
* RETURNS: HTTP_OK or HTTP_ERROR
*/
HTTP_STATUS httpMudTasksDelete
    (
    void
    )
    {
    if  (NULL == httpMud)
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpMudTasksDelete: NULL httpMud\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (OK != mudTasksDelete (httpMud))
        {
        return (HTTP_ERROR);
        }

    return (HTTP_OK);

    } /* httpMudTasksDelete */

/***************************************************************************
*
* httpMudInfrastructureDelete - this will delete the HTTP MUD infrastructure
*
* RETURNS: HTTP_OK or HTTP_ERROR
*/
HTTP_STATUS httpMudInfrastructureDelete
    (
    void
    )
    {
    if  (NULL == httpMud)
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpMudInfrastructureDelete: NULL httpMud\n",
                0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (OK != mudInfrastructureDelete (httpMud))
        {
        return (HTTP_ERROR);
        }

    httpMud = NULL;

    return (HTTP_OK);

    } /* httpMudInfrastructureDelete */

/***************************************************************************
*
* httpServerKill - currently a wrapper of httpShutdown
*
* This is an administrative function and should never be called within
* an HTTP task -RPM- context.
*
* RETURNS: HTTP_OK or HTTP_ERROR
*/
HTTP_STATUS httpServerKill
    (
    void
    )
    {
    return (httpShutdown ());

    } /* httpServerKill */

