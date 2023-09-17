/*
 * BarcodeReader.cpp
 *
 *  Created on: Apr 16, 2018
 *      Author: ee209315
 */


#include <vxWorks.h>
#include <evdevLib.h>
#include <stdlib.h>
#include <usrFsLib.h>
#include <errno.h>
#include <stdio.h>
#include <ioLib.h>
#include <fioLib.h>
#include <iostream>
#include "CommonProperty.h"
#include "BarcodeReader.h"
using namespace std;


/**************************************************************************//**
* \brief   - Initialize and assign the Barcode reader class data if any
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
BarcodeReader :: BarcodeReader()
{	
}


/**************************************************************************//**
* \brief   - Establishes a connection to Barcode scanner device if it 
* 			is physically connected else fails to connect 
*
* \param   - None.
*
* \return  - STATUS(OK - connected, ERROR - not connected)
*
******************************************************************************/
STATUS BarcodeReader :: BarcodeScannerConnect()
{	
	STATUS scannerStatus	= ERROR;
	barcodeScannerFd		= ERROR;
	
	/* Open the barcode device for communication
	 * OPTIMIZED APIs of event device framework are being used here;
	 * All the messages of the event device are exchanged over a /input/event queue
	 * Messages are distinguished based on the input type - keyboard/mouse/pointer etc
	 * NOTE: All the input devices are READ ONLY devices
	 */
	barcodeScannerFd = open (EV_DEV_NAME, O_RDONLY, 0);
	if(barcodeScannerFd == ERROR)
	{
		LOGERR("BR_T : Error: opening barcode scanner!", 0 ,0 ,0);
		m_bBarcodeScannerStatus = false;
		scannerStatus = ERROR;			
	}
	else
	{
		LOGCRIT("BR_T : Success: opening barcode scanner!", 0 ,0 ,0);
		m_bBarcodeScannerStatus = true;
		scannerStatus = OK;
	}
	
	/* Initialize the device name buffer */	
	bzero (devName, sizeof (devName));
	
	/* Get the device name;
	 * The return value of this API may not be valid
	 * We will get the device name only if we are able to read the device descriptor!
	 */	
	if (ERROR == ioctl (barcodeScannerFd, EV_DEV_IO_GET_NAME, (char *)devName))
		{
		LOGERR ("EV_DEV_IO_GET_NAME failed!\n", 0 ,0 ,0);
		}
		LOGDBG ("Device name = %s\n", *devName, 0, 0);
	
	/* Get the bar-code device capabilities;
	 * The return value of this API may not be valid
	 * We will get the device capabilities only if we are able to read the configuration descriptor!
	 */	
	if (ERROR == ioctl (barcodeScannerFd, EV_DEV_IO_GET_CAP, (char *)&devCap))
		{
		LOGERR("EV_DEV_IO_GET_CAP failed!\n", 0, 0, 0);
		}
		LOGCRIT("Device capabilities = 0x%x\n", devCap, 0, 0);
		
	/* Get the Keyboard mode;
	 * The return value of this API may not be valid
	 * We will get the keyboard mode only if we are able to read the configuration descriptor!
	 */	
	if ((devCap & (EV_DEV_ABS | EV_DEV_REL)) == 0)
		{
		if (ERROR == ioctl (barcodeScannerFd, EV_DEV_IO_GET_KBD_MODE, (char *)&kbdMode))
			{
			LOGERR ("EV_DEV_IO_GET_KBD_MODE failed!\n",0, 0, 0);
			}
		else
			{
			if (EV_DEV_KBD_UNICODE_MODE == kbdMode)
				LOGDBG ("Keyboard Mode is Unicode.\n",0 ,0, 0);
			else
				LOGCRIT ("Keyboard Mode is Keycode.\n",0 ,0, 0);
			}
		
		/* Force the device to use the UNICODE mode. This helps to decode most of the chars
		 * The return value of this API may not be valid
		 * Bar-codes by default shall use either ASCII or UNI code chars
		 */
		kbdMode = EV_DEV_KBD_UNICODE_MODE;
		if (ERROR == ioctl (barcodeScannerFd, EV_DEV_IO_SET_KBD_MODE, (char *)&kbdMode))
			{
			LOGERR ("EV_DEV_IO_SET_KBD_MODE failed!\n",0 ,0, 0);
			}
			LOGCRIT("Keyboard Mode uses Unicode.\n",0 ,0, 0);
		}
	
	return scannerStatus;
}


/**************************************************************************//**
* \brief   - Get Barcode scanner connect status
*
* \param   - none
*
* \return  - bool(true - connected, false - not connected)
*
******************************************************************************/
bool BarcodeReader :: GetScannerConnectStatus()
{
	return m_bBarcodeScannerStatus;
}


/**************************************************************************//**
* \brief   - Get Barcode scanner file descriptor
*
* \param   - BOOL &scannerFd - Scanner file descriptor
*
* \return  - None
*
******************************************************************************/
void BarcodeReader :: GetBarcodeScannerFd(BOOL &scannerFd)
{
	scannerFd = barcodeScannerFd;
}


/**************************************************************************//**
* \brief   - Send the message via msgQSend based on the ID
*
* \param   - Message& message(Message to send), MSG_Q_ID msgQID(Message ID to send)
*
* \return  - None
*
******************************************************************************/

void BarcodeReader :: SendToMsgQ(BarcodeMessage& message, MSG_Q_ID msgQID)
{
	if(msgQSend(msgQID, reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("BR_T : Message send failed to MSG Q",0,0,0);
	}	
}


/**************************************************************************//**
* \brief   - De-Initialize allocated memory if any
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
BarcodeReader :: ~BarcodeReader()
{	
}



/**************************************************************************//**
* \brief   - Barcode Reader task to process the Barcode data. This task is 
* 			 running at least priority(226)  
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void BarcodeReader_Task()
{
	LOGCRIT("BR_T : ----------- Barcode Reader Task spawned ----------",0,0,0);	
	int					barcodeScannerFd;
	UINT16				numOfBytesRead; 		
	CommonProperty		*ptrCP;
	BarcodeMessage		barcodeMessage;	
	EV_DEV_MSG			evdevMsg;
	
	STATUS	scannerConnectStatus	= ERROR;
	bool	scannerSendStatus		= false;
	
	static int maxMsg				= MAX_NUM_OF_BYTES_TO_READ;
	static int bufIdx				= 0;
	ptrCP				= CommonProperty :: getInstance();			
	MSG_Q_ID UImsgQId	= ptrCP->GetMsgQId(cTaskName[UI_T]);
	int devCap				= 0;
	int kbdMode				= 0;
	int	msgCount;
						
	while(ptrCP->bIsTaskRun == true)	
	{		
		/* Checks until the barcode scanner is connected to SC  */						
		memset(&barcodeMessage, 0x00, sizeof(barcodeMessage));
		barcodeMessage.msgID = TO_UI_TASK_BARCODE_SCANNER_STATUS_IND;
		
		/* Establish connection to barcode scanner */
		if(scannerConnectStatus == ERROR)
		{
			scannerConnectStatus = ptrCP->barcodeReader.BarcodeScannerConnect();
		}
		
		/* Indicate UIC that the Barcode scanner is connected to SC, only if task is still running */	
		if(scannerConnectStatus != OK)
		{	
			LOGDBG("BR_T : ---------------- Barcode Scanner NOT connected ----------------",0,0,0);
			sprintf(barcodeMessage.Buffer,"%d",(!scannerConnectStatus));
			ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);							
			while( (scannerConnectStatus != OK) && (ptrCP->bIsTaskRun == true))
			{
				taskDelay(SCANNER_CONNECT_DELAY);
				scannerConnectStatus = ptrCP->barcodeReader.BarcodeScannerConnect();	
			}
			if(ptrCP->bIsTaskRun == true)
			{
				LOGDBG("BR_T : ---------------- Barcode Scanner connected ----------------",0,0,0);
				sprintf(barcodeMessage.Buffer,"%d",(!scannerConnectStatus));
				ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);
				scannerSendStatus = true;
			}
		}
		else if( (scannerConnectStatus == OK) && (scannerSendStatus == false) )
		{
			LOGDBG("BR_T : ---------------- Barcode Scanner connected ----------------",0,0,0);
			sprintf(barcodeMessage.Buffer,"%d",(!scannerConnectStatus));
			ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);
			scannerSendStatus = true;
		}
		
		/* Clearing the buffer before the scan data is received to it */
		memset(&barcodeMessage, 0x00, sizeof(barcodeMessage));
		ptrCP->barcodeReader.GetBarcodeScannerFd(barcodeScannerFd);
		
		/* Check to ensure the device will not exceed the EVDEV message size
		 * clear msg
		 */
	    if (ERROR == ioctl (barcodeScannerFd, FIONREAD, (char *)&msgCount))
	        {
	    	LOGDBG ("FIONREAD failed!\n", 0, 0, 0);
	        }
		while (msgCount >= sizeof (EV_DEV_MSG))
			{
			if (sizeof (EV_DEV_MSG) != read (barcodeScannerFd, (char *)&evdevMsg, sizeof (EV_DEV_MSG)))
				{
				break;
				}

			if (ERROR == ioctl (barcodeScannerFd, FIONREAD, (char *)&msgCount))
				{
				break;
				}
			}
		
		LOGCRIT("---Begin receiving %u messages from event device.---\n", maxMsg, 0, 0);
	    while (maxMsg > 0)
	        {
	    	
	        msgCount = (int)read (barcodeScannerFd, (char *)&evdevMsg, sizeof (EV_DEV_MSG));	        
	        if ((int)sizeof (EV_DEV_MSG) == msgCount)
	            {
	        	LOGDBG ("Received a message!\n", 0, 0, 0);

	            switch (evdevMsg.msgType)
	                {
	            	/* Keyboard message */
	                case EV_DEV_MSG_KBD:
	                    {
	                    if( 0 == evdevMsg.msgData.kbdData.state)
	                    	{
	                    	/* Convert UINT16 to CHAR */
							sprintf(&barcodeMessage.Buffer[bufIdx],"%c",evdevMsg.msgData.kbdData.value);
							LOGDBG("* Raw char:%c"
									" ASCII/UNI CODE:%d "
									" state = %d *\n"
									, evdevMsg.msgData.kbdData.value
									, evdevMsg.msgData.kbdData.value
									, evdevMsg.msgData.kbdData.state);
							
							
							/* If the terminating char of the string is:
							 * carriage return(ASCII/UNI CODE: 13) or
							 * NULL(ASCII/UNI CODE: 13), consider this is the last of the chars
							 * in the stream and append a NULL at the end of the string and 
							 * send the string over the message queue
							 */
							if(barcodeMessage.Buffer[bufIdx] == 13 || barcodeMessage.Buffer[bufIdx] == 0)
								{
									LOGDBG("\t\t BarcodeMessage.Buffer[%d]:%c\n"
											, bufIdx
											, barcodeMessage.Buffer[bufIdx]
											, 0);
									
									/* Append a terminating NULL char at the end of the string */									
									barcodeMessage.Buffer[bufIdx] = '\0';
									
									/* Reset the buffer index */
									bufIdx = 0;
									
									/* Set up the bar code message with right parameters:
									 * Message queue ID + Buffer(bar-code) and send it over the queue
									 */									
									barcodeMessage.msgID = TO_UI_TASK_BARCODE_DATA;
									ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);
									memset(&barcodeMessage, 0x00, sizeof(barcodeMessage));
								}
							else
								{
									LOGDBG(" Raw buffer: %c", evdevMsg.msgData.kbdData.value, 0, 0);
									LOGDBG("\t\t BarcodeMessage.Buffer[%d]:%c\n"
											, bufIdx
											, barcodeMessage.Buffer[bufIdx]
											, 0);
									/* Increment the buffer index */
									bufIdx++;
									if(bufIdx >= BARCODE_DATA_SIZE) 
									{ //Barcode too long! Reset the buffer and send an empty string for invalid barcode read message
										bufIdx = 0;
										barcodeMessage.Buffer[bufIdx] = '\0';
										barcodeMessage.msgID = TO_UI_TASK_BARCODE_DATA;
										ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);
										memset(&barcodeMessage, 0x00, sizeof(barcodeMessage));
									}
								}
							}	                    
	                    }
	                    break;
	                    
	                default:
	                	LOGERR ("Unknown message = 0x%x\n", evdevMsg.msgType, 0, 0);
	                	break;
	                }
	            }
	        else
	            {
	        	scannerConnectStatus = ERROR;
	        	LOGERR ("Barcode message error!\n", 0, 0, 0);
	        	barcodeMessage.msgID = TO_UI_TASK_BARCODE_ERROR_IND;
				sprintf(barcodeMessage.Buffer,"%d",BARCODE_DATA_OVERFLOW);
				ptrCP->barcodeReader.SendToMsgQ(barcodeMessage, UImsgQId);
	            }
	        }
		
		if(scannerConnectStatus == ERROR)
			{
			LOGERR (" Stop receiving message from HID event device!n", 0, 0, 0);
			if (ERROR == close (barcodeScannerFd))
				{
				barcodeScannerFd = ERROR;
				LOGERR ("Close HID event device failed!\n", 0, 0, 0);
				}
			}
	}
	LOGCRIT("BR_T : ----------------Barcode Reader Task Terminated----------------",0,0,0);
}





