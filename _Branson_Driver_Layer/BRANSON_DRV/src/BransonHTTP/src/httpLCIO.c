/* httpLCIO.c - protocol encapsulation for LiveControl connections */

/* Copyright (c) 1999-2004,2010,2013,2015 Wind River Systems, Inc.
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
21feb13,r_w  fix static analysis issue(defect WIND00404092)
17dec10,m_z part of APIs changed
               CQID: Fix WIND00247096
11aug10,m_z  keep transferred LC length field as 4 bytes
                 CQID: Fix WIND00227522
19jul10,h_y  modified for WIND00223629
02feb10,h_y   support for LP64 
13feb07,msa  Fix comments to remove APIgen errors.
19jul06,ps  remove debug printf
19jul06,ps  fix NTOH4 endian issues in httpLCPacketHeaderWrite on Linux
22jan03,dlr  wrap LC code inside #ifdef INCLUDE_HTTP_LIVE_CONTROL
20oct03,dlr  Port to WM Web 4.3
07apr98,mb   product status.
26nov96,mb   written.
*/

/*
DESCRIPTION
This package provides the LiveControl protocol functions on the target side.

INCLUDE FILES: httpLib.h
               httpLCLib.h

NOMANUAL
*/

#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include "httpLib.h"
#include "httpCfg.h"
#include "httpLCLib.h"

char *	httpWmDescriptionGet
	(
	char * szName	/* WindMark name */
	);

/* Only compile if feature is enabled (and existing) in project */
#ifdef INCLUDE_HTTP_LIVE_CONTROL       /* include Live Control Java processing */


#ifdef HTTP_LC_DEBUG
int httpLCSendEnable = 1;
#endif

/*******************************************************************************
*
* httpWmDescriptionGet - return a description string of the WindMark
*
* This routine will return a description of a WindMark LiveControl httpLCDescriptionWrite()
* packet. This is a place holder for the WWS routine httpSymDescriptionGet.
*
* In Wind Web Server symbols could have description strings attached, but this
* is not supported for WindMarks. The user can if desired replace this routine and
* perhaps have it return relavent information like the native data type, access rights,
* maximum size, and/or registered parent component.
*
* NOMANUAL
*
* RETURNS: Empty string
*/

char *	httpWmDescriptionGet
	(
	char * szName	/* WindMark name */
	)
	{
	/* User changable routine */

	return ("");
	}


/*******************************************************************************
*
* httpLCPacketHeaderWrite - begin transmission of a LiveControl packet
*
* This routine starts the transmission of a LiveControl packet
* with the specified size.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if transmission was successful, or
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCPacketHeaderWrite
    (
    MUD_CONN_ID	     ioHandle,	    /* Handle of the active connection    */
    ubyte4           ulSize  	   /* packet size    */
    )
    {

#ifdef WEBCLI_ON_LINUX

    char netbuf [4];

    netbuf [0] = (char) ((ulSize >> 24) & 0xFF);
    netbuf [1] = (char) ((ulSize >> 16) & 0xFF);
    netbuf [2] = (char) ((ulSize >> 8) & 0xFF);
    netbuf [3] = (char) (ulSize & 0xFF);
    return httpLCBlockPut ( ioHandle , netbuf , 4 );

#else
    ulSize = HTON4 ( ulSize );
    return httpLCBlockPut ( ioHandle , (char*) &ulSize , sizeof (ubyte4) );
#endif


    }


/*******************************************************************************
*
* httpLCPacketWrite - transmit Live Control packet
*
* This routine transmits the specified LiveControl packet
* with the specified packet identifier and an optional content inside.
*
* szPacket specifies the optional packet content subsequent to the packet
* identifier. If szPacket is NULL and/or ulPacketSize is 0, no packet
* content will be sent and only an empty packet with the packet id is
* transmitted.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if transmission was successful, or
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCPacketWrite
    (
    MUD_CONN_ID	                ioHandle,	     /* Handle of the active connection    */
    char            	        cCommand,     /* packet id           */
    char *          	        szPacket,     /* packet content      */
    unsigned long    	    	ulPacketSize  /* packet content size */
    )
    {
    HTTP_STATUS status;

#ifdef HTTP_LC_DEBUG
	printf ( "httpLiveControlSend<<<: Command %x = " , (unsigned long) cCommand );
	switch (cCommand)
		{
		case HTTP_LC_PACKET_PING         : printf("PING"); break;
		case HTTP_LC_PACKET_QUIT         : printf("QUIT"); break;
		case HTTP_LC_PACKET_VERSION      : printf("VERSION"); break;
		case HTTP_LC_PACKET_SUBSCRIBE    : printf("SUBSCRIBE"); break;
		case HTTP_LC_PACKET_UNSUBSCRIBE  : printf("UNSUBSCRIBE"); break;
		case HTTP_LC_PACKET_GET	         : printf("GET"); break;
		case HTTP_LC_PACKET_GETNEXT      : printf("GETNEXT"); break;
		case HTTP_LC_PACKET_VALUE        : printf("VALUE"); break;
		case HTTP_LC_PACKET_INTERVAL     : printf("INTERVAL"); break;
		case HTTP_LC_PACKET_ERROR        : printf("ERROR"); break;
		case HTTP_LC_PACKET_DESCRIPTION  : printf("DESCRIPTION"); break;
		case HTTP_LC_PACKET_SYMBOLTABLE  : printf("SYMBOLTABLE"); break;
		case HTTP_LC_PACKET_FINISHED     : printf("FINISHED"); break;
		case HTTP_LC_PACKET_PING_ACK     : printf("PING_ACK     "); break;
		case HTTP_LC_PACKET_VERSION_ACK  : printf("VERSION_ACK  "); break;
		case HTTP_LC_PACKET_SUBSCRIBE_ACK: printf("SUBSCRIBE_ACK"); break;
		case HTTP_LC_PACKET_GET_ACK      : printf("GET_ACK"); break;
		case HTTP_LC_PACKET_GETNEXT_ACK  : printf("GETNEXT_ACK"); break;
		case HTTP_LC_PACKET_INTERVAL_ACK : printf("INTERVAL_ACK "); break;
		default							 : printf("UNKNOWN"); break;
		}
	printf("\n");
#endif

    (void)httpLCPacketHeaderWrite ( ioHandle , (ubyte4)(ulPacketSize+1) );
    status = httpLCBlockPut ( ioHandle , &cCommand , 1 );
    if ( ( szPacket != NULL ) && ( ulPacketSize > 0 ) )
		status = httpLCBlockPut ( ioHandle , szPacket , ulPacketSize );

    return status;
    }


/*******************************************************************************
*
* httpWMtoLCType - Convert WMB type to Live Control type
*
* NOMANUAL
*
* RETURNS: HTTP_OK if transmission was successful, or
*          HTTP_ERROR otherwise.
*/

int httpWMtoLCType
    (
    DATATYPE_T    wmType		/* WindMark WMB type */
    )
    {
    ubyte4 nType;

	/* Convert WM type to LiveControl type */
	switch (wmType)
	{
    	case DATATYPE_BOOL_K 		: nType = HTTP_SYM_TYPE_BOOL_ID; break;

    	case DATATYPE_LONG_K 		: nType = HTTP_SYM_TYPE_LONG_ID; break;

    	case DATATYPE_INTEGER_K 	: nType = HTTP_SYM_TYPE_INT_ID; break;
    	case DATATYPE_UINTEGER_K 	: nType = HTTP_SYM_TYPE_UINT_ID; break;

    	case DATATYPE_COUNTER_K 	:
    	case DATATYPE_GAUGE_K 		:
    	case DATATYPE_ULONG_K 		: nType = HTTP_SYM_TYPE_ULONG_ID; break;

    	case DATATYPE_CHAR_K 		: nType = HTTP_SYM_TYPE_CHAR_ID; break;
    	case DATATYPE_UCHAR_K 		: nType = HTTP_SYM_TYPE_UCHAR_ID; break;

    	case DATATYPE_SHORT_K 		: nType = HTTP_SYM_TYPE_SHORT_ID; break;
    	case DATATYPE_USHORT_K 		: nType = HTTP_SYM_TYPE_USHORT_ID; break;

    /* every thing else maps to string for type safety */
    	case DATATYPE_STRING_K 		:
    	case DATATYPE_INT64_K 		:
    	case DATATYPE_UINT64_K 		:
    	case DATATYPE_COUNTER64_K 	:

    	case DATATYPE_OCTETS_K 		:
    	case DATATYPE_OPAQUE_K 		:
    	case DATATYPE_OID_K 		:
    	case DATATYPE_IPV4_K 		:
    	case DATATYPE_IPV6_K 		:
    	case DATATYPE_VOID_K 		:
    	case DATATYPE_NULL_K 		:
    	case DATATYPE_MACADDR_K 	:
    	case DATATYPE_ACCESS_K 		:

    	case DATATYPE_FUNCPTR_K 	:
    	case DATATYPE_VOIDFUNCPTR_K :

    	case DATATYPE_TIME_K 		:
    	case DATATYPE_TICKS_K 		:
		case DATATYPE_ROW_STATUS_K 	:
    	case DATATYPE_PORT_LIST_K 	:
    	case DATATYPE_BRIDGE_ID_K 	:
	    default						: nType = HTTP_SYM_TYPE_STRING_ID; break;
		}

	return (int)nType;
	}

/*******************************************************************************
*
* httpLCDescriptionWrite - transmit Live Control packet with WindMark description
*
* This routine creates and transmits a Live Control packet containing any
* information a client needs to know about a WindMark.
* The transmited information contains the WindMark name, WindMark type, textual
* description and number of items stored within WindMark value.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if transmission was successful, or
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCDescriptionWrite
    (
	MUD_CONN_ID	ioHandle,	/* Handle of the active connection    */
	char *      szName, 	/* WindMark name         */
    DATATYPE_T  wmType		/* WM Data type if known */
    )
    {
    unsigned long		  ulSize;
    unsigned long		  ulNumItemsGet;
    char *                szType = "";
    char *                szDescription = NULL;
    HTTP_STATUS           status = HTTP_ERROR;
    char 		          tempBuff[256];	/* temp buffer for httpPrintf !!! */
    ubyte4       		  nType;

	/* test for valid name */
    if (( szName == NULL ) || ('\0' == *szName))
    	return HTTP_ERROR;

	/* Prepare LC description fields */
	szDescription = httpWmDescriptionGet(szName);
	szType        = "string";
	ulNumItemsGet = 1;

	/* Get the entry's data type */
	if (DATATYPE_INVALID_K == wmType)
		if (HTTP_OK != httpLCWindMarkType(szName, &wmType))
			wmType = DATATYPE_STRING_K;
#ifdef HTTP_LC_REG_STRINGS_ONLY
	wmType = DATATYPE_STRING_K;
#endif

	/* Convert WM type to LiveControl type */
	nType = (ubyte4)httpWMtoLCType(wmType);

	/* Convert Live Control type to LC type description */
	switch (nType)
		{
		case HTTP_SYM_TYPE_CHAR_ID        : szType = "char";	break;
		case HTTP_SYM_TYPE_SHORT_ID       : szType = "short";	break;
		case HTTP_SYM_TYPE_LONG_ID        : szType = "long";	break;
		case HTTP_SYM_TYPE_INT_ID         : szType = "long";	break;

		case HTTP_SYM_TYPE_UCHAR_ID       : szType = "uchar";	break;
		case HTTP_SYM_TYPE_USHORT_ID      : szType = "ushort";	break;
		case HTTP_SYM_TYPE_ULONG_ID       : szType = "ulong";	break;
		case HTTP_SYM_TYPE_UINT_ID        : szType = "ulong";	break;

		case HTTP_SYM_TYPE_BOOL_ID        : szType = "bool";	break;
		case HTTP_SYM_TYPE_FLOAT_ID       : szType = "float";	break;
		case HTTP_SYM_TYPE_DOUBLE_ID      : szType = "double";	break;

		case HTTP_SYM_TYPE_GENLIST_ID     : szType = "genlist";	break;

		case HTTP_SYM_TYPE_STRING_ID      :
		default  						  : szType = "string";	break;
		}


#ifdef HTTP_LC_DEBUG
	printf ( "httpLiveControlSend<<<: Command %x = DESCRIPTION of '%s'\n",HTTP_LC_PACKET_DESCRIPTION,szName);
#endif

	/* compose and send message */
	ulSize = 1 + strlen (szName) + 1 + strlen (szType) + 1 +strlen (szDescription) + 1 + 8;
	status = httpLCPacketHeaderWrite ( ioHandle , (ubyte4)ulSize );
	if (HTTP_OK != status)
	    return HTTP_ERROR;

	(void)snprintf ( tempBuff , 255,"%c%s\1%s\1%s\1%08lx",
						  HTTP_LC_PACKET_DESCRIPTION,szName,szType,szDescription,
						  ulNumItemsGet);
	status = httpLCBlockPut ( ioHandle , tempBuff , ulSize );

    return status;
    }


/*********************************************************************
* httpLCClientNotify - send a notification for a WindMark
*
* This routine sends a notification to the client telling him the
* current value of WindMark specified by name.
* Normally this function is called only if a WindMark was even
* subscribed or its value has beed changed.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if notification was done correctly or
*          HTTP_ERROR otherwise
*/

HTTP_STATUS httpLCClientNotify
    (
    MUD_CONN_ID	ioHandle,	/* Handle of the active connection    */
    char*       		szName,		/* WindMark's  name */
    void*       		szValue,	/* WindMark's value */
    unsigned long       nValueSize	/* Size of value */
    )
    {
    LOCAL char cmd = HTTP_LC_PACKET_VALUE;
    LOCAL char separator = 1;
    HTTP_STATUS			 status = HTTP_OK;

    if ( ( ioHandle != (MUD_CONN_ID) NULL ) && ( szName != NULL )  && ( szValue != NULL ) )
		{
#ifdef HTTP_LC_DEBUG
		if (httpLCSendEnable == 0)
			{
			httpDoLCSocketClosed(ioHandle); /* use the internal non-locked call */
			return HTTP_ERROR;
			}

		printf ( "httpLiveControlSend<<<: Command %x = VALUE of '%s', Size=%d\n",
			HTTP_LC_PACKET_VALUE,szName,nValueSize);
#endif

		if (HTTP_OK == status)
			status = httpLCPacketHeaderWrite ( ioHandle , (ubyte4)(1 + strlen(szName) + 1 + nValueSize) );
		/* Send command */
		if (HTTP_OK == status)
			status = httpLCBlockPut ( ioHandle , &cmd , sizeof (char ) );
		/* Send size, then name of WindMark */
		if (HTTP_OK == status)
			status = httpLCBlockPut ( ioHandle , szName , strlen(szName ));
		if (HTTP_OK == status)
			status = httpLCBlockPut ( ioHandle , &separator , sizeof (char ) );
		/* Send size, then value of new data */
		if (HTTP_OK == status)
			status = httpLCBlockPut ( ioHandle , szValue, nValueSize);

		/* if the socket is not working, un-register the respective client */
		if (HTTP_OK != status)
			(void) httpDoLCSocketClosed(ioHandle); /* use the internal non-locked call */
		}

    return status;
    }


/*******************************************************************************
*
* httpLCBlockPut - Output a block of specified length to the HTTP connection socket.
*
* This routine is called with the request handle of an active HTTP connection
* and sends a block of length <ulSize> to the output socket.
*
* NOMANUAL
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpLCBlockPut
    (
    MUD_CONN_ID	        ioHandle,	/* Handle of the active connection    */
    char * 				pchBuffer,	/* Buffer with data to send to socket */
    unsigned long 		ulSize		/* Number of bytes to write to socket */
    )
    {

    if (0 == ulSize) return (HTTP_OK);

#ifdef HTTP_LC_DEBUG
		{
	    unsigned long i;
		printf("httpLCBlockWrite(%ld)=",ulSize);
		for (i=0;i<ulSize;i++)
			printf( (isalnum(pchBuffer[i])) ? "%c":"<%x>",pchBuffer[i] & 0x0ff);
		printf("\n");
		}
#endif

	if (OK != mudWrite(ioHandle, (char *) pchBuffer, ulSize))
        {
        wmError(0, "unable to write to socket");
        return HTTP_ERROR;
        }

    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpLCBlockRead - Read a block of data from the HTTP connection socket.
*
* This routine reads a block of data from the I/O socket (mainly used for
* POST method).
*
* NOMANUAL
*
* RETURNS: The number of bytes read.
*/

long httpLCBlockRead
    (
    MUD_CONN_ID	ioHandle,	/* Handle of the active connection    */
    char * 				pchBuffer,  /* Buffer to store the read block     */
    unsigned long 		ulMaxSize	/* Maximum size of block              */
    )
    {
    unsigned long lRead;
#ifndef _WRS_CONFIG_LP64
    (void)mudRead(ioHandle, pchBuffer, (ubyte4)ulMaxSize, (ubyte4 *)&lRead);
#else
    (void)mudRead(ioHandle, pchBuffer, (unsigned long)ulMaxSize, (unsigned long *)&lRead);
#endif

#ifdef HTTP_LC_DEBUG
		{
		unsigned long i;
		printf("httpLCBlockRead(%ld)=",lRead);
		for (i=0;i<lRead;i++)
			printf( (isalnum(pchBuffer[i])) ? "%c":"<%x>",pchBuffer[i] & 0x0ff);
		printf("\n");
		}
#endif

	return ((long)lRead);
    }




#endif /* INCLUDE_HTTP_LIVE_CONTROL  : include Live Control Java processing */

