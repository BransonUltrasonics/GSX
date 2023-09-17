/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     httpProcessPostData gets the post data from http client and send it to RTPSC. It 
		also receives response from RTPSC and send it back to http client

 **********************************************************************************************************/

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "commonCfg.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#ifdef WEBCLI_ON_LINUX
# include <errno.h>
#endif

/* WM header files */

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpWmbExpandInstance.h"
#include "httpAuth.h"
#include "httpCommon.h"


/* WM project header files */

#if 0
#include "httpCfg.h"
#endif

#ifdef __SNMP_API_ENABLED__
#include "wmmErrorLog.h"
#endif

#define WEBSERVICE_MSG_Q_NAME "/WebService_Task"
#define WEBSERVICEREC_MSG_Q_NAME "/WebServiceRec_Task"

#define TO_SDO_WEB_SERVICE_LOGIN_RESP 		650
#define	MAX_SIZE_OF_MSG_LENGTH				768
#define	MAX_SIZE_OF_MSG_BUFF 				(MAX_SIZE_OF_MSG_LENGTH-4)

#define RUNTIME_FEATURES_RESPONSE_SIZE		3

typedef struct Message
{
	INT32	msgID;
	char	Buffer[MAX_SIZE_OF_MSG_LENGTH-4];	
}Message;

MSG_Q_ID WEBSERVICE_MSG_Q_ID  = MSG_Q_ID_NULL; 
MSG_Q_ID WEBSERVICE_MSG_Q_RECEIVE_ID  = MSG_Q_ID_NULL; 
#define WEBSERVICES_MESSAGE_RESPONSE_TIMEOUT	5*60 // 5Seconds

void   (*pwmStoreUserData)(char *chBuffer) = NULL;

int gdwErrCode = 0;

BOOL 	isFrstReq = TRUE;
BOOL 	isWebServicesEnabled = FALSE;
BOOL 	isFirmwareUpgradeEnabled = FALSE;
Message	respMsg;
/**************************************************************************//**
 * \brief  - This function is used to receive POST DATA from web client
 * 			 and sent it to RTP_SC
 *
 * \param  - HTTP_REQ_ID     reqId
 *
 * \return  - RPM_OK/RPM_DONE short
 *
 ******************************************************************************/
short httpProcessPostData(HTTP_REQ_ID     reqId)
{
	char msgBuffer[64] = {0};

	unsigned long          	entitySize;
	sbyte *                	tmpBuf;
	MEM_POOL_ID            	mpPool;
	sbyte *                	pEntity = NULL;
	long                   	bytesRead;
	unsigned long          	bytesRemaining;
	unsigned long          	chunkSize;
	Message 				firstMsg,recievedMsg;
	char 					recvBuffer[MAX_SIZE_OF_MSG_LENGTH + 4];

	// Getting the Post data Length form the HTTP POST request
	entitySize = strtoul(httpMimeHdrGet(reqId, HDR_IN, "Content-Length"),NULL,0);

	if (0 == entitySize)
	{
		(void)(*phttpPostReply)(reqId, HTTP_OK);
		return RPM_DONE;    /* empty POST, done */
	}

	//Getting the ID of the memory pool of the request
	mpPool = httpReqMemPartIdGet(reqId);
	if (entitySize > MAX_SIZE_OF_MSG_BUFF || strlen(reqId->szRpmData) > 64)
	{   
		httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
		(void) httpError (reqId, "httpProcessPostData: Request Buffer/ URl is BIG");
		reqId->fPersistent = HTTP_FALSE; /* close connection */
		return RPM_DONE;
	}

	/* allocate memory for the post entity (the CGI stream) */
	if (NULL == (pEntity = (sbyte *) memPoolAlloc(mpPool, entitySize+2)))
	{
		httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
		(void) httpError (reqId, "httpRpmPost: unable to allocate mem for entity. Increase HTTP_MAX_REQ_PART_SIZE");
		reqId->fPersistent = HTTP_FALSE;
		return RPM_DONE; 
	}

	//SSL *pssl = reqId->ioHandle->pMudConnSSLState->pSSL;
	
	/* Transfer from socket to buffer */
	tmpBuf = pEntity;
	bytesRemaining = entitySize;
	while (bytesRemaining > 0)
	{
		if (httpMaxTempBufSize < bytesRemaining)
			chunkSize = httpMaxTempBufSize;
		else
			chunkSize = bytesRemaining;
		bytesRead = httpBlockRead (reqId, tmpBuf, chunkSize);
		if (bytesRead < 0)
		{
			return (HTTP_ERROR);
		}
		bytesRemaining -= bytesRead;
		tmpBuf += bytesRead;
	}
	pEntity[entitySize]=0;
	
	/* Copy the RPM data into a message buffer*/
	strcpy(msgBuffer,reqId->szRpmData);

	/* Message queue open */
	WEBSERVICE_MSG_Q_ID = msgQOpen(WEBSERVICE_MSG_Q_NAME, 0, 0, 0, 0, 0);

	/* compare the message buffer with our macro to copy the entity to particular ID */
	memset(respMsg.Buffer,0x00,sizeof(respMsg.Buffer));
	if(strcmp(msgBuffer,SYTEM_LOGIN)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_LOGIN;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,SYSTEM_LOGOUT)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_LOGOUT;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,SW_VERSION)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_SW_VERSION;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,LAST_WELD)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_LAST_WELD;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_NUM_ALARMS)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_NUM_ALARMS;
		strcpy(respMsg.Buffer,pEntity);
	}	
	else if(strcmp(msgBuffer,SET_RECIPE_VALUE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_SET_RECIPE_VALUE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_RECIPE_VALUE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_RECIPE_VALUE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_SYSTEM_VALUE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_SYSTEM_VALUE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,SAVE_RECIPE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_SAVE_RECIPE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,SET_ACTIVE_RECIPE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_SET_ACTIVE_RECIPE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,DELETE_RECIPE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_DELETE_RECIPE;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_NUM_WELD_HISTORY)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_NUM_WELD_HISTROY;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_WELD_HISTORY)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_WELD_HISTROY;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_ALARM_LOG)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_ALARM_LOG;
		strcpy(respMsg.Buffer,pEntity);
	}
	else if(strcmp(msgBuffer,GET_GRAPH_RESULT)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_GRAPH_RESULT;
		strcpy(respMsg.Buffer,pEntity);
	}	
	else
	{
		gdwErrCode = 22;
		return RPM_OK;
	}

	/*   message receive from RTPSC*/
	if(MSG_Q_ID_NULL == WEBSERVICE_MSG_Q_RECEIVE_ID)
	{
		WEBSERVICE_MSG_Q_RECEIVE_ID = msgQOpen(WEBSERVICEREC_MSG_Q_NAME, 0, 0, 0, 0, 0);
		if(MSG_Q_ID_NULL == WEBSERVICE_MSG_Q_RECEIVE_ID)
		{
			logError("\n### httpRpmupload : WEBSERVICE_MSG_Q_RECEIVE_ID RETURNED NULL ###\n");
		}
		else
		{
			logError("\n### httpRpmupload : WEBSERVICE_MSG_Q_RECEIVE_ID IS CREATED ###\n");
		}
	}
	
	/* Before sending the new request clearing the all the old responses*/
	if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, 10)) != ERROR)
	{
		/* Just clearing the old request resposnes or pending responses or canceled request responses */ 
	}

	if(isFrstReq == TRUE)
	{
		/* Message send to RTPSC */
		firstMsg.msgID = TO_WEB_SERVICES_TASK_GET_RUNTIME_DETAILS;
		strcpy(firstMsg.Buffer,"  ");
		if(msgQSend(WEBSERVICE_MSG_Q_ID,(char*)(&firstMsg),sizeof(firstMsg),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			/* Indicating System Busy if fails */
			gdwErrCode = 6;
			return RPM_OK;
		}
		else
		{
			memset(recvBuffer,0x00,sizeof(recvBuffer));
			if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, WEBSERVICES_MESSAGE_RESPONSE_TIMEOUT)) != ERROR)
			{
				memcpy(&recievedMsg, recvBuffer, sizeof(recievedMsg));
				if(strlen(recievedMsg.Buffer) != RUNTIME_FEATURES_RESPONSE_SIZE || recievedMsg.Buffer[1] != ',')
				{
					isFrstReq = FALSE;
					return RPM_DONE;
				}
				
				if(recievedMsg.Buffer[0] == '1')
				{
					isWebServicesEnabled = TRUE;
				}
				
				if(recievedMsg.Buffer[2] == '1')
				{
					isFirmwareUpgradeEnabled = TRUE;
				}
				isFrstReq = FALSE;
			}
			else
			{
				/* Indicating System Busy if fails */
				gdwErrCode = 6;
				return RPM_OK;
			}
		}
	}
	if(isWebServicesEnabled == FALSE)
	{
		/* Indicating the client as Service is not avaialble with error code 503 if Runtime configuration is disabled */
		httpStatusSet (reqId, HTTP_SERV_UNAVAILABLE);
		(void) httpError (reqId, "httpProcessPostData: Web Service not available");
		reqId->fPersistent = HTTP_FALSE; /* close connection */
		return RPM_DONE;
	}
	/* Message send to RTPSC */
	if(msgQSend(WEBSERVICE_MSG_Q_ID,(char*)(&respMsg),sizeof(respMsg),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		logError("\n#### command to message queue send failed  #### \n");
	}
	else
	{
		logError("\n#### command to message queue send successfully  #### \n");
	}
	gdwErrCode = 0;
	return RPM_OK;
}

/**************************************************************************//**
 * \brief  - This function is used to send POST DATA to web client
 * 			 which is received from RTPSC
 *
 * \param  - HTTP_REQ_ID     reqId
 *
 * \return  - RPM_DONE
 *
 ******************************************************************************/
short httpSendResponse(HTTP_REQ_ID     reqId)
{
	const sbyte *     	contentEnc;
	WMB_SESSION_T *   	pSession = NULL;
	WMB_SESSION_T *   	pGetSession = NULL;
	WMB_TID_T         	tid = NULL;
	WMB_OBJ_T         	pObj;
	ubyte4            	trans_flags;
	BOOL              	doSet;
	char 				recvBuffer[MAX_SIZE_OF_MSG_LENGTH + 4];
	Message 			recievedMsg;

	/****************************************************************************//*

		This part of code is to set the transaction for sending the 
		message to the http web page

	 ******************************************************************************/	
	contentEnc = httpMimeHdrGet(reqId, HDR_IN, "Content-Type");
	wmbSessionCreate (httpGetWmbComp(), 0, WMB_SESSION_NO_DEF_TRANSACTION, &pSession);
	wmbSessionStateSet ( pSession, (void *) reqId );
	wmbSessionTypeSet ( pSession, WMB_TYPE_WEB_K );
	wmbSessionCreate(httpGetWmbComp(),0,0,&pGetSession);
	doSet = ((wmbTransactionCreate(pSession, 0, trans_flags, &tid) == OK) &&
			(SendResponse(reqId, pSession, tid, pGetSession) == OK))
	            								?  TRUE : FALSE;
	if (doSet)  /* If nothing is wrong, we now do a SET transaction. */
	{
		if (wmbTransactionProcess(tid, CMD_SET_K) != OK)
		{
			pObj = wmbTransactionErrQGet( tid );
			if (NULL != pObj )
			{
				(*phttpWindMarkSetFailed)(reqId, wmbObjectNamePtrGet(pObj), (unsigned short)wmbTransactionStatusGet(tid));
			}
			else
			{
				logError("httpRpmWmbPost: failed to process transaction\n");
			}
		}
		/****************************************************************************/
		reqId->pSession = pSession;
		httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		/*Set the status based for the response to the client*/
		httpStatusSet (reqId, HTTP_OK);
		/*This is used to generate header based on the status to a connection socket*/
		httpHeaderGenerate (reqId);
		if(gdwErrCode)
		{
			char chBuffer[64]={0};
			sprintf(chBuffer,"{\"StatusCode\":%d}",gdwErrCode);
			httpPrintf(reqId, chBuffer);
		}
		else
		{
			BOOL bIsDataFinished;
			while(1)
			{
				if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, WEBSERVICES_MESSAGE_RESPONSE_TIMEOUT)) != ERROR)
				{
					memcpy(&recievedMsg, recvBuffer, sizeof(recievedMsg));
					switch(recievedMsg.msgID)
					{
					case TO_WEBCLI_KERNEL_LOGIN:
					case TO_WEBCLI_KERNEL_LOGOUT:
					case TO_WEBCLI_KERNEL_SW_VERSION:
					case TO_WEBCLI_KERNEL_LAST_WELD:
					case TO_WEBCLI_KERNEL_SET_RECIPE_VALUE:
					case TO_WEBCLI_KERNEL_GET_RECIPE_VALUE:
					case TO_WEBCLI_KERNEL_GET_SYSTEM_VALUE:
					case TO_WEBCLI_KERNEL_SAVE_RECIPE:
					case TO_WEBCLI_KERNEL_SET_ACTIVE_RECIPE:
					case TO_WEBCLI_KERNEL_DELETE_RECIPE:
					case TO_WEBCLI_KERNEL_GET_NUM_WELD_HISTROY:
					case TO_WEBCLI_KERNEL_GET_NUM_ALARMS:
					case TO_WEBCLI_KERNEL_GET_WELD_HISTROY_FINISH:
					case TO_WEBCLI_KERNEL_GET_GRAPH_RESULT_FINISH:
					case TO_WEBCLI_KERNEL_GET_ALARM_LOG_FINISH:
						bIsDataFinished = TRUE;
						break;
					case TO_WEBCLI_KERNEL_GET_WELD_HISTROY_NEXT:
					case TO_WEBCLI_KERNEL_GET_GRAPH_RESULT_NEXT:
					case TO_WEBCLI_KERNEL_GET_ALARM_LOG_NEXT:
						bIsDataFinished = FALSE;
						break;
					default:
						bIsDataFinished = TRUE;
						break;
					}

					if(bIsDataFinished)
					{
						/*This is used to print the message in http connection socket */
						httpPrintf(reqId, recievedMsg.Buffer);
						break;
					}
					else
					{
						/*This is used to print the message in http connection socket */
						httpPrintf(reqId, recievedMsg.Buffer);
					}
				}
				else
				{
					// Need to send INTERNAL COMMUNICATION
					break;
				}
			}
		}
	}
	if (NULL != pGetSession)
		(void) wmbSessionDestroy(pGetSession);

	(void) wmbTransactionEnd(tid);
	(void) wmbSessionDestroy(pSession);
	
	return RPM_DONE;
}

/**************************************************************************//**
 * \brief  - This function is used to receive POST DATA from web client for FOE
 * 			 and sent it to RTP_SC
 *
 * \param  - HTTP_REQ_ID     reqId
 *
 * \return  - RPM_OK/RPM_DONE short
 *
 ******************************************************************************/
short httpProcessFOEPostData(HTTP_REQ_ID     reqId)
{
	char msgBuffer[64] = {0};

	unsigned long          	entitySize;
	sbyte *                	tmpBuf;
	MEM_POOL_ID            	mpPool;
	sbyte *                	pEntity = NULL;
	long                   	bytesRead;
	unsigned long          	bytesRemaining;
	unsigned long          	chunkSize;
	Message 				firstMsg,recievedMsg;
	char 					recvBuffer[MAX_SIZE_OF_MSG_LENGTH + 4];

	/* Copy the RPM data into a message buffer*/
	strcpy(msgBuffer,reqId->szRpmData);

	/* Message queue open */
	WEBSERVICE_MSG_Q_ID = msgQOpen(WEBSERVICE_MSG_Q_NAME, 0, 0, 0, 0, 0);

	/* compare the message buffer with our macro to copy the entity to particular ID */
	memset(respMsg.Buffer,0x00,sizeof(respMsg.Buffer));
	if(strcmp(msgBuffer,FIRMWARE_UPGRADE)==0)
	{
		respMsg.msgID = TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS_FU;
		strcpy(respMsg.Buffer,pEntity);
	}	
	else
	{
		gdwErrCode = 22;
		return RPM_OK;
	}

	/*   message receive from RTPSC*/
	if(MSG_Q_ID_NULL == WEBSERVICE_MSG_Q_RECEIVE_ID)
	{
		WEBSERVICE_MSG_Q_RECEIVE_ID = msgQOpen(WEBSERVICEREC_MSG_Q_NAME, 0, 0, 0, 0, 0);
		if(MSG_Q_ID_NULL == WEBSERVICE_MSG_Q_RECEIVE_ID)
		{
			logError("\n### httpRpmupload : WEBSERVICE_MSG_Q_RECEIVE_ID RETURNED NULL ###\n");
		}
		else
		{
			logError("\n### httpRpmupload : WEBSERVICE_MSG_Q_RECEIVE_ID IS CREATED ###\n");
		}
	}
	
	/* Before sending the new request clearing the all the old responses*/
	if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, 10)) != ERROR)
	{
		/* Just clearing the old request resposnes or pending responses or canceled request responses */ 
	}

	if(isFrstReq == TRUE)
	{
		/* Message send to RTPSC */
		firstMsg.msgID = TO_WEB_SERVICES_TASK_GET_RUNTIME_DETAILS;
		strcpy(firstMsg.Buffer,"  ");
		if(msgQSend(WEBSERVICE_MSG_Q_ID,(char*)(&firstMsg),sizeof(firstMsg),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			/* Indicating System Busy if fails */
			gdwErrCode = 6;
			return RPM_OK;
		}
		else
		{
			memset(recvBuffer,0x00,sizeof(recvBuffer));
			if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, WEBSERVICES_MESSAGE_RESPONSE_TIMEOUT)) != ERROR)
			{
				memcpy(&recievedMsg, recvBuffer, sizeof(recievedMsg));
				if(strlen(recievedMsg.Buffer) != 3)
				{
					isFrstReq = FALSE;
					return RPM_DONE;
				}
				
				if(recievedMsg.Buffer[0] == 49)
				{
					isWebServicesEnabled = TRUE;
				}
				
				if(recievedMsg.Buffer[2] == 49)
				{
					isFirmwareUpgradeEnabled = TRUE;
				}
				isFrstReq = FALSE;
			}
			else
			{
				/* Indicating System Busy if fails */
				gdwErrCode = 6;
				return RPM_OK;
			}
		}
	}
	if((respMsg.msgID == TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS_FU && isFirmwareUpgradeEnabled == FALSE))
	{
		/* Indicating the client as Service is not avaialble with error code 503 if Runtime configuration is disabled */
		httpStatusSet (reqId, HTTP_SERV_UNAVAILABLE);
		(void) httpError (reqId, "httpProcessPostData: Firmware Upgrade Service not available");
		reqId->fPersistent = HTTP_FALSE; /* close connection */
		return RPM_DONE;
	}
	/* Message send to RTPSC */
	if(msgQSend(WEBSERVICE_MSG_Q_ID,(char*)(&respMsg),sizeof(respMsg),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{		
		logError("\n#### command to message queue send failed  #### \n");
	}
	else
	{
		logError("\n#### command to message queue send successfully  #### \n");
	}

	gdwErrCode = 0;
	return RPM_OK;
}

/**************************************************************************//**
 * \brief  - This function is used to send POST DATA to web client FOR FOE
 * 			 which is received from RTPSC
 *
 * \param  - HTTP_REQ_ID     reqId
 *
 * \return  - RPM_DONE
 *
 ******************************************************************************/
short httpSendFOEResponse(HTTP_REQ_ID     reqId)
{
	const sbyte *     	contentEnc;
	WMB_SESSION_T *   	pSession = NULL;
	WMB_SESSION_T *   	pGetSession = NULL;
	WMB_TID_T         	tid = NULL;
	WMB_OBJ_T         	pObj;
	ubyte4            	trans_flags;
	BOOL              	doSet;
	char 				recvBuffer[MAX_SIZE_OF_MSG_LENGTH + 4];
	Message 			recievedMsg;

	if(gdwErrCode)
	{
		char chBuffer[64]={0};
		sprintf(chBuffer,"{\"StatusCode\":%d}",gdwErrCode);
		httpPrintf(reqId, chBuffer);
	}
	else
	{
		if ((msgQReceive(WEBSERVICE_MSG_Q_RECEIVE_ID, (char *) recvBuffer, MAX_SIZE_OF_MSG_LENGTH, WEBSERVICES_MESSAGE_RESPONSE_TIMEOUT)) != ERROR)
		{
			memcpy(&recievedMsg, recvBuffer, sizeof(recievedMsg));
			switch(recievedMsg.msgID)
			{
			case TO_WEBCLI_KERNEL_GET_ALL_USER_DETAILS_FU:
				pwmStoreUserData(recievedMsg.Buffer);
				break;
			default:
				break;
			}
		}
	}

	return RPM_OK;
}

#endif
