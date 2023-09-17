/* httpRpmLiveControl.c - live remote control and observation of embedded systems */

/*
 * Copyright (c) 1999-2004, 2010-2015 Wind River Systems, Inc.
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
01mar12,r_w  eliminate compile warning for defect WIND00335205
11aug10,m_z  keep the LC tranaferred length field with 4 bytes
             CQID: Fix WIND00227522
02feb10,h_y   support for LP64 
26sep07,ps   use explicit host-to-network expression instead of NTOH4.
             Defect WIND00105787
13feb07,msa  Fix comments to remove APIgen errors.
19jul06,ps   fix network-host byte swap issues on Linux
16feb06,ps   changes for webcli on linux
27may04,dlr  Swap read error and memory alloc errors to actual errors
04jan04,dlr  Added support for GET and GETNEXT cmnd, variable instances
22dec03,dlr  wrap LC code inside #ifdef INCLUDE_HTTP_LIVE_CONTROL
10nov03,dlr  port to WM Web 4.3
09nov99,csr  checking for number of cookies on initialization
17jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
26nov96,mb   initial creation.
*/

/*
DESCRIPTION
RPM that provides persistent HTTP connections for the ProWeb live system.
The complete ProWeb protocol is handled and interpreted by these functions.

INCLUDE FILES: httpLib.h
               httpRpms.h
               httpTask.h
               httpUtil.h
               httpLCLib.h

NOMANUAL
*/


#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "httpTask.h"
#include "httpUtil.h"
#include "commonCfg.h"
#include "httpCfg.h"

/* #define DEBUG_INFO */
/* #define HTTP_LC_DEBUG */

#include "httpLCLib.h"
#include "mudLib.h"

#ifndef WEBCLI_ON_LINUX
#include "sysLib.h"
#endif

/* Only compile if feature is enabled (and existing) in project */
#ifdef  INCLUDE_HTTP_LIVE_CONTROL /* include Live Control Java processing */

/* local constants */
#define HTTP_LC_COOKIE_NAME   "LCMC"
#define HTTP_LC_URL           "/lc/"
#define HTTP_LC_COOKIE_MAXAGE 9999

/* local functions */
LOCAL short httpRpmLiveControlReceive (HTTP_REQ_ID const reqId);
LOCAL short httpRpmLiveControlSend (HTTP_REQ_ID const reqId);
HTTP_STATUS httpRpmLiveControlInit(char *strHTTP_LC_URL);

/* global variables */

/* local variables */
LOCAL short lcCookieCounter = 0;

/* cookies are required for proper operation of LiveControl */

IMPORT  unsigned short httpMaxSendCookies;
IMPORT  unsigned short httpMaxRecCookies;



#ifdef HTTP_LC_DEBUG
int httpLCReceiveEnable = 1;

/*******************************************************************************
*
* httpLCstop - stop Live Control communications
*
* This routine is available only if compiled with HTTP_LC_DEBUG defined and is
* used to suspend/stop any Live Control communications to client applets.
*
* NOMANUAL
*
* RETURNS: N/A
*
* SEE ALSO: httpLCstart()
*/

void httpLCstop()
    {
    httpLCReceiveEnable=0;
    httpLCSendEnable=0;
    logInfo ("LiveControl is disabled.");
    }

#endif

/*******************************************************************************
*
* httpLCstart - start Live Control communications
*
* This routine is available only if compiled with HTTP_LC_DEBUG defined and is
* used to resume any Live Control communications stopped by httpLCstop().
*
* NOMANUAL
*
* RETURNS: N/A
*
* SEE ALSO: httpLCstop()
*/

#ifdef HTTP_LC_DEBUG

void httpLCstart()
    {
    httpLCReceiveEnable=1;
    httpLCSendEnable=1;
    logInfo ("LiveControl is enabled.");
    }

#endif


/*******************************************************************************
* httpRpmLiveControlInit - initialized Live Control subsystem
*
* This routine is called from 'httpglob.c' if INCLUDE_LIVE_CONTROL is defined.
*
* RETURNS: HTTP_OK if initialization completed successfully,
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpRpmLiveControlInit
    (
    char *strHTTP_LC_URL
    )
    {
    if ( (httpMaxSendCookies <= 0) || (httpMaxRecCookies <= 0) )
		{
		logError ( "Live Control package cannot operate without cookies." );
		return HTTP_ERROR;
		}

	/* Set up polling values (mSec to ticks) */
	httpRpmLCMediumPoll = (unsigned int)(HTTP_LC_POLL_MED * CLOCKS_PER_SEC) / 1000;

    if ( httpRpmConfAdd ( HTTP_M_POST , strHTTP_LC_URL , httpRpmLiveControl ) == NULL )
		{
		logError ( "Live Control package not initialized!" );
		return HTTP_ERROR;
		}


#ifdef HTTP_LC_DEBUG
    httpLCstart();
#endif

    return HTTP_OK;
    }


/*******************************************************************************
*
* httpRpmLCDenyService - return a Live Control deny service message
*
* This routine returns a message if the Live Control RPM denies service.
*
* NOMANUAL
*
* RETURNS: N/A
*/

LOCAL void httpRpmLCDenyService
    (
    HTTP_REQ_ID reqId
    )
    {
#ifdef HTTP_LC_DEBUG
	printf("httpRpmLCDenyService: ERROR\n");
#endif

    (void) httpLCLock();
    httpStatusSet ( reqId , HTTP_SERV_UNAVAILABLE );       /* Set the HTTP status code */
    (void) httpMimeContentTypeSet ( reqId , HDR_OUT , "text/html" );
    (void) httpHeaderGenerate ( reqId );                          /* Send HTTP header         */
    (void)httpPrintf ( reqId , "<H1>Sorry,</H1>there are too many clients using Live Control currently!" );
    (void) httpLCUnlock();
    }


/*******************************************************************************
*
* httpRpmLiveControl - Live Control RPM
*
* This is the Live Control RPM. This routine is accessed using the HTTP
* POST method and provides access to the symbol table of the HTTP server
* using Java Applets. This routine is only called by the RPM dispatcher and
* never called directly. To use the Live Control service, it must be
* initialized by calling httpRpmLiveControlInit().
*
* RETURNS: RPM_DONE if some request has been successfully completed,
*          RPM_ERROR if the curent request was invalid or has been denied.
*/

short httpRpmLiveControl
    (
    HTTP_REQ_ID const reqId
    )
    {
    /* some commands arrived from an already registered client */
#ifdef HTTP_LC_DEBUG
    if ( httpLCReceiveEnable == 0 )
		{
		httpRpmLCDenyService(reqId);
		return RPM_DONE;
		}
#endif

    if ( httpMimeInCookieGet ( reqId , HTTP_LC_COOKIE_NAME ) != NULL )
		return httpRpmLiveControlReceive(reqId);
    else
		return httpRpmLiveControlSend(reqId);
    }


/*******************************************************************************
*
* httpRpmLiveControlReceive - process incoming Live Control requests
*
* This is routine processes incoming Live Control requests.
*
* NOMANUAL
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/


LOCAL short httpRpmLiveControlReceive
    (
    HTTP_REQ_ID const reqId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID clientId;
    long                   lContentLength = 0;
    ubyte4                 ulBytes;
    char *                 szLength;
    char *                 szError;
    char *                 szCookie;
    char                   szBuffer[32];
    char *                 szReadBuffer = NULL;
    char *                 szReadBufferTemp = NULL;
    char *				   szTempBuffer = NULL;
    char		           cCommand;
    STATUS                 status = HTTP_OK;
    MUD_CONN_ID   	       sendIo;
	DATATYPE_T    		   wmType;
	int				       nType;
	ubyte4				   nPriority;	/* priority indicates which polling list */
	ubyte4				   subscribeStatus;

    logInfo ( "httpRpmLiveControlReceive: running...>>>>>>>>>>>>>>>>>>>>>>>>>" );

    /* some commands arrived from an already registered client */
    szCookie = httpMimeInCookieGet ( reqId , HTTP_LC_COOKIE_NAME );
	(void) httpLCLock ();
    if (( clientId = httpLCClientFindByCookie ( szCookie ) ) == NULL )
		{
		(void) httpLCUnlock ();
		logErrorMsg ( "httpRpmLiveControlReceive: can not find Live Control client with cookie %s!",szCookie );
		httpRpmLCDenyService ( reqId );
			return RPM_ERROR;
		}
	(void) httpLCUnlock ();

    if ( ( szLength = httpGetEnv(reqId,"Content-Length") ) != NULL )
	lContentLength = atol ( szLength );

    sendIo = (MUD_CONN_ID) httpGenListDataGet ( clientId );
    if (NULL == sendIo)
    	{
		logErrorMsg("ERROR: httpRpmLiveControlReceive:Cannot find Client %lx\n",clientId);
		sendIo = reqId->ioHandle;
		}

	/* Lock the LC access */
	(void) httpLCLock ();
    while ( lContentLength > 0 )
	{
		szError = NULL;
		if ( ( httpLCBlockRead (reqId->ioHandle , szBuffer , sizeof (ubyte4) ) == sizeof (ubyte4) ) &&
			 ( httpLCBlockRead (reqId->ioHandle , &cCommand , 1 ) == 1 ) )
			{
			lContentLength = (long)(lContentLength - (long)(sizeof (unsigned long)+1));

                        ulBytes =   (((ubyte4) szBuffer [0]) << 24)
                                  + (((ubyte4) szBuffer [1]) << 16)
                                  + (((ubyte4) szBuffer [2]) << 8)
                                  + ((ubyte4) szBuffer [3]);

#ifdef DEBUG_INFO
		    logInfoMsg ( "httpRpmLiveControlReceive: got packet length %ld\n", ulBytes);
			logInfoMsg ( "httpRpmLiveControlReceive: Command %x = " , (unsigned short)cCommand );
#endif


#ifdef HTTP_LC_DEBUG
			printf ( "httpLiveControlReceive>>>: Command %x = " , (unsigned short)cCommand );
			switch (cCommand)
				{
				case HTTP_LC_PACKET_PING         : printf("PING"); break;
				case HTTP_LC_PACKET_QUIT         : printf("QUIT"); break;
				case HTTP_LC_PACKET_VERSION      : printf("VERSION"); break;
				case HTTP_LC_PACKET_SUBSCRIBE_NONE : printf("SUBSCRIBE-NON"); break;
				case HTTP_LC_PACKET_SUBSCRIBE_MED  : printf("SUBSCRIBE-MED"); break;
				case HTTP_LC_PACKET_SUBSCRIBE_HIGH : printf("SUBSCRIBE-HGH"); break;
				case HTTP_LC_PACKET_UNSUBSCRIBE  : printf("UNSUBSCRIBE"); break;
				case HTTP_LC_PACKET_VALUE        : printf("VALUE"); break;
				case HTTP_LC_PACKET_INTERVAL     : printf("INTERVAL"); break;
				case HTTP_LC_PACKET_ERROR        : printf("ERROR"); break;
				case HTTP_LC_PACKET_DESCRIPTION  : printf("DESCRIPTION"); break;
				case HTTP_LC_PACKET_SYMBOLTABLE  : printf("SYMBOLTABLE"); break;
				case HTTP_LC_PACKET_GET  		 : printf("HTTP_LC_PACKET_GET"); break;
				case HTTP_LC_PACKET_GETNEXT      : printf("HTTP_LC_PACKET_GETNEXT"); break;
				case HTTP_LC_PACKET_FINISHED     : printf("FINISHED"); break;
				default							 : printf("UNKNOWN"); break;
				}
			printf("\n");
#endif

			if ( (szReadBuffer = memPoolAlloc ( httpReqMemPartIdGet (reqId) , ulBytes-- ) ) != NULL )
				{
				if ( (ubyte4)httpLCBlockRead (reqId->ioHandle , szReadBuffer , ulBytes ) == ulBytes )
				  {
				  szReadBuffer[ulBytes] = 0; /* better 0-terminate szReadBuffer */
				  lContentLength = lContentLength - (long)ulBytes;
#ifdef HTTP_LC_DEBUG
					{
					long i;
					printf ( "httpRpmLiveControlReceive: got packet content [" );
					for(i=0;i<ulBytes;i++)
					printf ( "%x.",(short)szBuffer[i] & 0x0ff);
					printf ( "]\n" );
					}
#endif
					logInfoMsg ( "httpRpmLiveControlReceive: Got Packet with %ld Bytes data" , ulBytes );
					switch ( cCommand )
					{
					case HTTP_LC_PACKET_PING:
						logInfo ( "httpRpmLiveControlReceive: ping..." );
						(void) httpLCPacketWrite ( sendIo , (char)HTTP_LC_PACKET_PING_ACK , NULL , 0 );
						break;
					case HTTP_LC_PACKET_QUIT:
						logInfo ( "httpRpmLiveControlReceive: quit connection..." );
						httpLCClientRemove ( sendIo );

						reqId->fPersistent = HTTP_FALSE;
						lContentLength = -1;
						break;
					case HTTP_LC_PACKET_SUBSCRIBE:
					case HTTP_LC_PACKET_SUBSCRIBE_NONE:
					case HTTP_LC_PACKET_SUBSCRIBE_HIGH:
						logInfo ( "httpRpmLiveControlReceive: subscribe..." );

						/* which polling list */
						switch( cCommand )
							{
							case HTTP_LC_PACKET_SUBSCRIBE_NONE : nPriority = kPollPushNone; break;
							case HTTP_LC_PACKET_SUBSCRIBE_HIGH : nPriority = kPollPushHigh; break;
							case HTTP_LC_PACKET_SUBSCRIBE      :
							default:                             nPriority = kPollPushMedium; break;
							}

						if (TRUE != httpLCSubscribeAccess (reqId, szReadBuffer))
							{
							logInfoMsg ( "httpRpmLiveControlReceive: No access for \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							break;
							}
						if (TRUE == httpLCSubscribe (sendIo, szReadBuffer, nPriority, clientId, &subscribeStatus ))
							{
							/* Forward the new entry's description */
							(void) httpLCDescriptionWrite ( sendIo , szReadBuffer, DATATYPE_INVALID_K );

							/* Notify the Clients of the value of this new entry */
							(void) httpLCNotify(szReadBuffer,NULL,0,HTTP_LC_PACKET_GET);

							/* acknowledge for subscribe */
							(void) httpLCPacketWrite ( sendIo , (char)HTTP_LC_PACKET_SUBSCRIBE_ACK , szReadBuffer , strlen ( szReadBuffer ));
							}
						else
							{
							logInfoMsg ( "httpRpmLiveControlReceive: can not subscribe \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							}
						break;
					case HTTP_LC_PACKET_UNSUBSCRIBE:
						logInfo ( "httpRpmLiveControlReceive: unsubscribe..." );
						if ( httpLCUnsubscribe ( szReadBuffer , clientId ) == HTTP_FALSE)
							{
							logInfoMsg ( "httpRpmLiveControlReceive: can not unsubscribe \"%s\"",szReadBuffer );
							szError = "unsubscribe error!";
							}
						break;
					case HTTP_LC_PACKET_GET:
						logInfo ( "httpRpmLiveControlReceive: get..." );

						/* which polling list */
						if (TRUE != httpLCSubscribeAccess (reqId, szReadBuffer))
							{
							logInfoMsg ( "httpRpmLiveControlReceive: No access for \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							break;
							}
                        szReadBufferTemp = szReadBuffer;
						if (TRUE == httpLCSubscribe (sendIo, szReadBufferTemp, kPollPushNone, clientId, &subscribeStatus ))
							{
							if (LC_SUBSCRIBE_DUP != subscribeStatus)
								{
								/* Forward the new entry's description if first subscription */
								(void) httpLCDescriptionWrite ( sendIo , szReadBuffer, DATATYPE_INVALID_K );
								}

							/* Notify the Clients of the value of this new entry */
							(void) httpLCNotify(szReadBuffer,NULL,0,HTTP_LC_PACKET_GET);

							/* acknowledge for subscribe */
							(void) httpLCPacketWrite ( sendIo , (char)HTTP_LC_PACKET_GET_ACK , szReadBuffer , strlen ( szReadBuffer ));
							}
						else
							{
							logInfoMsg ( "httpRpmLiveControlReceive: can not subscribe \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							}
						break;
					case HTTP_LC_PACKET_GETNEXT:
						logInfo ( "httpRpmLiveControlReceive: getnext..." );

						/* which polling list */
						if (TRUE != httpLCSubscribeAccess (reqId, szReadBuffer))
							{
							logInfoMsg ( "httpRpmLiveControlReceive: No access for \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							break;
							}

						/* Create temp name buffer for changing instance */
						if (NULL == (szTempBuffer = memPoolAlloc ( httpGlobalMemPartIdGet() , kMagicMarkupBufferSize) ))
							{
							logInfoMsg ( "httpRpmLiveControlReceive: Out of memory for \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							break;
							}
						strncpy(szTempBuffer,szReadBuffer,(kMagicMarkupBufferSize-1));
                        szReadBufferTemp = szTempBuffer;
						if (TRUE == httpLCSubscribe (sendIo, szReadBufferTemp, kPollPushNone, clientId, &subscribeStatus ))
							{
							if (LC_SUBSCRIBE_DUP != subscribeStatus)
								{
								/* Forward the new entry's description if first subscription */
								(void) httpLCDescriptionWrite ( sendIo , szReadBuffer, DATATYPE_INVALID_K );
								}

							/* Notify the Clients of the value of this new entry */
							/* Presumably Name will change with this call */
							(void) httpLCNotify(szTempBuffer,NULL,0,HTTP_LC_PACKET_GETNEXT);

							/* acknowledge for subscribe */
							(void) httpLCPacketWrite ( sendIo , (char)HTTP_LC_PACKET_GETNEXT_ACK , szTempBuffer , strlen ( szTempBuffer ));
							}
						else
							{
							logInfoMsg ( "httpRpmLiveControlReceive: can not subscribe \"%s\"",szReadBuffer );
							szError = "subscribe error!";
							}

						/* Free the temp buffer */
						(void)memPoolFree ( httpGlobalMemPartIdGet() , szTempBuffer );

						break;
					case HTTP_LC_PACKET_SYMBOLTABLE:
						{
						ubyte4 ulIndex = 0;
						DB_ENTRY_T * pDBEntry;

						logInfo ( "httpRpmLiveControlReceive: describe symbol table..." );
					 	while (NULL != ( pDBEntry = wmbDBEntriesIterator( NULL, NULL, FALSE, &ulIndex) ))
					 		{
							(void) httpLCDescriptionWrite ( sendIo ,  wmbEntryNameGet(pDBEntry) , (DATATYPE_T) wmbEntryTypeGet(pDBEntry));
							}
						}
						break;
					case HTTP_LC_PACKET_VALUE:
						{
						char *        	szValue = NULL;
						long			nValueLength;
						char *		  	szInstance;
						WMB_SESSION_T *	pSession;
						ubyte2			nShort = 0;
						long			nLong  = 0;
						long			i;

						/* Fetch value */
						httpOsalStringToken ( szReadBuffer , "\1" , &szValue );

						/* extract any instance/argument from name */
						if (NULL != (szInstance = strchr (szReadBuffer, '.')))
							{
							*szInstance++ = '\0';
							}
						else if (NULL != (szInstance = strchr (szReadBuffer, '(')))
							{
							char *szTemp = NULL;

							*szInstance++ = '\0';

							/* remove closing paren, using szValue as temp pointer */
							if (NULL !=(szTemp = strrchr (szInstance, ')')))
								*szTemp = '\0';
							}
						else szInstance = "";

#ifdef DEBUG_INFO
						logInfoMsg ( "httpRpmLiveControlReceive: Symbol   is '%s'" , szReadBuffer );
						logInfoMsg ( "httpRpmLiveControlReceive: Instance is '%s'" , szInstance );
						logInfoMsg ( "httpRpmLiveControlReceive: Value    is '%s'" , szValue );
#endif
#ifdef HTTP_LC_DEBUG
						printf ( "httpRpmLiveControlReceive: Symbol   is '%s'\n" , szReadBuffer );
						printf ( "httpRpmLiveControlReceive: Instance is '%s'\n" , szInstance );
						printf ( "httpRpmLiveControlReceive: Value    is '%s'\n" , szValue );
						printf ( "httpRpmLiveControlReceive: ValueSize = '%d'\n" , (long)(&szReadBuffer[ulBytes]-szValue) );
#endif

						pSession = httpWmbSessionGet(reqId);
						if (NULL == pSession)
							pSession = pLCSession;

						/* Convert WM type to LiveControl type and perform Network to Native ordering as needed */
						(void)httpLCWindMarkType(szReadBuffer,&wmType);
						nType = httpWMtoLCType(wmType);

#ifdef HTTP_LC_REG_STRINGS_ONLY
						nType = HTTP_SYM_TYPE_STRING_ID;
						/* Delete decimal notation for string integers from LC */
						switch (nType)
							{
							case HTTP_SYM_TYPE_UCHAR_ID       :
							case HTTP_SYM_TYPE_CHAR_ID        :

							case HTTP_SYM_TYPE_SHORT_ID       :
							case HTTP_SYM_TYPE_USHORT_ID      :

							case HTTP_SYM_TYPE_BOOL_ID        :
							case HTTP_SYM_TYPE_LONG_ID        :
							case HTTP_SYM_TYPE_INT_ID         :
							case HTTP_SYM_TYPE_ULONG_ID       :
							case HTTP_SYM_TYPE_UINT_ID        :
								if (NULL != (szInstance = strchr (szValue, '.')))
									*szInstance = '\0';
								break;
							}
#endif

						switch (nType)
							{
							case HTTP_SYM_TYPE_BOOL_ID        :
									nValueLength = 4;
									nLong        = (ubyte4)((1 == *(char *)szValue) ? 1:0);
									szValue 	 = (void *) &nLong ;
									break;

							case HTTP_SYM_TYPE_UCHAR_ID       :
							case HTTP_SYM_TYPE_CHAR_ID        :
									nValueLength = 1; break;

							case HTTP_SYM_TYPE_SHORT_ID       :
							case HTTP_SYM_TYPE_USHORT_ID      :
									/* convert network order (HighEndian) short to host short */
									/* number from LC may be truncated for empty high-order byte */
									for (i=0,nShort=0L;
										(i<2) && (i < (short)(&szReadBuffer[ulBytes]-szValue));
										i++)
										{
										nShort = (ubyte2)((nShort << 8) + (((short) szValue[i]) & 0x0ff));
										}
									szValue      = (void *) &nShort ;
									nValueLength = 2;
									break;

							case HTTP_SYM_TYPE_LONG_ID        :
							case HTTP_SYM_TYPE_INT_ID         :
							case HTTP_SYM_TYPE_ULONG_ID       :
							case HTTP_SYM_TYPE_UINT_ID        :
									/* convert network order (HighEndian) long to host long */
									/* number from LC may be truncated for empty high-order bytes */
									for (i=0,nLong=0L;
										(i<4) && (i < (long)(&szReadBuffer[ulBytes]-szValue));
										i++)
										{
										nLong = (nLong << 8) + (((long) szValue[i]) & 0x0ff);
										}
									szValue      = (void *) &nLong ;
									nValueLength = 4;
									break;

							case HTTP_SYM_TYPE_FLOAT_ID       : /* these are forced to string type */
							case HTTP_SYM_TYPE_DOUBLE_ID      :

							case HTTP_SYM_TYPE_STRING_ID      :
							case HTTP_SYM_TYPE_GENLIST_ID     :
							default  						  :
									nValueLength = (long)(strlen(szValue) + 1); /* include zero terminator */
									break;
							}

						/* NOTE: we do not set status, because an out-of-range value causes the whole */
						/* client to re-register! They will know the value is bad on the next push  */
						switch (nType)
							{
							case HTTP_SYM_TYPE_FLOAT_ID       : /* these are forced to string type */
							case HTTP_SYM_TYPE_DOUBLE_ID      :
							case HTTP_SYM_TYPE_STRING_ID      :
							case HTTP_SYM_TYPE_GENLIST_ID     :
								(void)wmbDataStringSet (pSession, szReadBuffer, szInstance, (sbyte4)strlen(szInstance),
														  szValue, (long)strlen(szValue) );
								break;
							default:
								(void)wmbDataNativeSet (pSession, szReadBuffer, szInstance, (sbyte4)strlen(szInstance),
														  szValue, nValueLength );
								break;
							}


					/*	if ( HTTP_OK != status)
							{
							szError = "httpRpmLiveControlReceive: setValue ERROR.";
							logInfoMsg ( "httpRpmLiveControlReceive: setValue failed for symbol %s", szReadBuffer );
#ifdef HTTP_LC_DEBUG
							printf ( "httpRpmLiveControlReceive: setValue ERROR for symbol %s\n", szReadBuffer );
#endif
							}
						else
							{*/
							logInfoMsg ( "httpRpmLiveControlReceive: symbol %s set.", szReadBuffer );
						/*	}*/
						}
						break;
					/* Not yet implemented */
/*					case HTTP_LC_PACKET_FINISHED:
					status = !HTTP_OK;
					logInfo ( "httpRpmLiveControlReceive: got end-of-request packet." );
					break;
*/
					default:
						szError = "invalid command!";
						status = HTTP_NO_CONTENT;
						lContentLength = -1;
						break;
					}
					if ( szError != NULL )
						(void) httpLCPacketWrite ( sendIo , HTTP_LC_PACKET_ERROR , szError , strlen(szError) );
				  }
				else
				  {
				  logInfo ( "httpRpmLiveControlReceive: read error, closing!" );
				  status = HTTP_NO_CONTENT;
				  lContentLength = -1;
				  break;
				  }
				}
		    else
				{
				/* not enough memory to read parameters */
				logInfo ( "httpRpmLiveControlReceive: not enough memory available, aborting!" );
				status = HTTP_ENTITY_TOO_LARGE;
				lContentLength = -1;
				break;
				}

		    }
		else
			{
			logInfo ( "httpRpmLiveControlReceive: short read!" );
			status = HTTP_NO_CONTENT;
			break;
			}
	}
	(void) httpLCUnlock ();

    /* the initial request will be answered with full HTTP/MIME response */
    if ( status != HTTP_OK )
		{
		reqId->fPersistent = HTTP_FALSE;
		logInfo("httpRpmLiveControlReceive: connection will not persist.");
		}

    (void) httpLCLock();
    httpStatusSet ( reqId , (short)status );       /* Set the HTTP status code       */
    (void) httpMimeContentTypeSet ( reqId , HDR_OUT , "text/html" );
    (void) httpMimeHdrSet (reqId, HDR_OUT, "Content-Length", "0");
    (void) httpHeaderGenerate ( reqId );            /* Send HTTP header */
    (void) httpLCUnlock();
    logInfo ( "httpRpmLiveControlReceive: stopped.<<<<<<<<<<<<<<<<<<<<<<<<<<<<" );
    return RPM_DONE;
    }


/*******************************************************************************
*
* httpRpmLiveControlSend - process outgoing Live Control requests
*
* This is routine processes outgoing Live Control requests. It installs
* exactly one outgoing connection to any Java client.
*
* NOMANUAL
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/

LOCAL short httpRpmLiveControlSend
    (
    HTTP_REQ_ID const reqId
    )
    {
    char *                 szCookie;
    char                   szBuffer[32];
	MUD_CONN_STATE         MudConnState;

    logInfo ( "httpRpmLiveControlSend: running...>>>>>>>>>>>>>>>>>>>>>>>>>" );

	(void) mudConnStateGet(reqId->ioHandle,&MudConnState);
    if (MudConnState == CONN_STATE_NEW)
		{
		/* handle new connections: register new client, provide cookie */
		(void) httpLCLock();
		(void)snprintf ( szBuffer , 31, "LC%lx%02d" , (unsigned long)(reqId->ioHandle), lcCookieCounter++ % 100);
		szCookie = szBuffer;
		if ( httpLCClientAdd ( szBuffer , reqId ) == NULL )
			{
			(void) httpLCUnlock();
			logError ( "httpRpmLiveControlSend: can not register client!" );
			httpRpmLCDenyService ( reqId );
			return RPM_ERROR;
			}
		/* pass cookie to new client */
		(void) httpMimeOutCookieSet ( reqId,HTTP_LC_COOKIE_NAME,szCookie,HTTP_LC_URL,NULL,NULL,HTTP_LC_COOKIE_MAXAGE,HTTP_FALSE );
		httpStatusSet ( reqId , HTTP_OK );       /* Set the HTTP status code       */
		(void) httpMimeContentTypeSet ( reqId , HDR_OUT , "application/livecontrol" );
		(void) httpMimeHdrSet (reqId, HDR_OUT, "Content-Length", "999999");
		(void) httpHeaderGenerate ( reqId );            /* Send HTTP header               */
		(void) httpLCUnlock();
		logInfo ( "httpRpmLiveControlSend: stopped.<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" );
		return RPM_DONE;
		}
    else
		{
		/* request arrived on existing link - error, because pipelining not supported */
		logError ( "httpRpmLiveControlSend: pipelining not supported!" );
		httpRpmLCDenyService ( reqId );
		return RPM_DONE;
		}
    }


#endif /* INCLUDE_HTTP_LIVE_CONTROL  : include Live Control Java processing */

