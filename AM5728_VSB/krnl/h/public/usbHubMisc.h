/* usbHubMisc.h - Miscellaneous for hub class driver */

/* Copyright 2004, 2014 Wind River Systems, Inc. 

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according  
   to the terms of their Wind River license agreement(s) applicable to 
   this software.
*/

/*
Modification history
--------------------
01b,28jan14,wyy Add code under USB_AUTO_TEST to support USB internal testing
01a,27jun03,cfc Changing the code to WRS standards
*/


/*
INTERNAL
 *******************************************************************************
 * Filename         : usbHubMisc.h
 *
 * Copyright        :
 *
 * THE COPYRIGHT IN THE CONTENTS OF THIS SOFTWARE VEST WITH WIPRO
 * LIMITED A COMPANY INCORPORATED UNDER THE LAWS OF INDIA AND HAVING
 * ITS REGISTERED OFFICE AT DODDAKANNELLI SARJAPUR ROAD  BANGALORE
 * 560 035. DISTRIBUTION OR COPYING OF THIS SOFTWARE BY
 * ANY INDIVIDUAL OR ENTITY OTHER THAN THE ADDRESSEE IS STRICTLY
 * PROHIBITED AND MAY INCUR LEGAL LIABILITY. IF YOU ARE NOT THE
 * ADDRESSEE PLEASE NOTIFY US IMMEDIATELY BY PHONE OR BY RETURN EMAIL.
 * THE ADDRESSEE IS ADVISED TO MAINTAIN THE PROPRIETARY INTERESTS OF
 * THIS COPYRIGHT AS PER APPLICABLE LAWS.
 *
 *
 * Description      : This module provides the debug functionality function
 *                    header file for the USB Hub Class Driver.
 *
 *
 ******************************************************************************/


#ifndef __usbHubMisch
#define __usbHubMisch

/*
 * The following structure is to support USB internal testing.
 */
#ifdef _WRS_CONFIG_USB_AUTO_TEST
typedef struct usb_hub_auto_test_func
    {
    BOOL (* autoPlugMatch)(void * pHub, UINT8  uPortIndex);    /* autoPlugMatch */
    BOOL (* autoUnPlugMatch)(void * pHub, UINT8  uPortIndex);  /* autoUnPlugMatch */
    } USB_HUB_AUTO_TEST_FUNC;

#endif


/************************ GLOBAL FUNCTIONS ************************************/


/*******************************************************************************
 * Function Name    : usbHubPortCntGet
 * Description      : This function returns the number of ports connected to a
 *                    hub.
 * Parameters       : uHubId         IN   Hub Device Handle
 *                    pPortCount     OUT  pointer to the Port count
 * Return Type      : a) USBHST_SUCCESS if port count was successfully returned
 *                    b) USBHST_FAILURE if error occured.
 ******************************************************************************/
USBHST_STATUS usbHubPortCntGet(UINT32 uHubId, PUINT16 pPortCount);

/*******************************************************************************
 * Function Name    : usbHubNodeIDGet
 * Description      : Gets the node id of the node connected to hub port
 * Parameters       : uHubId      IN  Handle to the hub.
 *                    uPortIndex  IN  port index
 *                    pNodeType   OUT pointer to recieve the node type
 *                    pNodeId     OUT pointer to recive the node Id
 * Return Type      : a) USBHST_SUCCESS if node id was successfully returned
 *                    b) USBHST_FAILURE if error occured.
 ******************************************************************************/
USBHST_STATUS usbHubNodeIDGet(UINT32  uHubId,
                            UINT16  uPortIndex,
                            PUINT16 pNodeType,
                            PUINT32 pNodeId);

/*******************************************************************************
 * Function Name    : usbHubGetRootHubHandle
 * Description      : This function is called by the USB Host Software Stack for
 *                    Finding the root hub handle of a Bus.
 * Parameters       : uBusHandle        IN   Unique identifier for the bus
 *                    pRootHubHandle    OUT  Pointer to the root hub handle
 * Return Type      : a) USBHST_SUCCESS if handle was successfully returned
 *                    b) USBHST_FAILURE if error occured.
 ******************************************************************************/
USBHST_STATUS usbHubGetRootHubHandle (UINT8 uBusHandle, PUINT32 pRootHubHandle);


#endif /* End of #ifndef __usbHubMisch */


/**************************** End of File usbHubMisc.h *************************/
