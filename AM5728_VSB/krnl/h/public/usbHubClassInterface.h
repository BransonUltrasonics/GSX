/* usbHubClassInterface.h - Functions used by the host software stcak */

/*
 * Copyright (c) 2004, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004, 2010 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
01c,27jan10,y_l Change hub device reset (WIND00151882)
01b,13jan10,ghs  vxWorks 6.9 LP64 adapting
01a,27jun03,nrv Changing the code to WRS standards
*/

/*
DESCRIPTION

This module implements the functions to be used by the USB Host
Software Stack. The following functions are described in this module:
1. Function for plug and play.
2. Function for power management.
3. Function for root hub management.
4. Function for selective suspend and resume.
5. Function for checking hub power capability.
6. Function for resetting a device.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_ClassInterface.h
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
 * Description      : This module implements the functions to be used by the
 *                    USB Host Software Stack. The following functions are
 *                    described in this module:
 *                    1.    Function for plug and play.
 *                    2.    Function for power management.
 *                    3.    Function for root hub management.
 *                    4.    Function for selective suspend and resume.
 *                    5.    Function for checking hub power capability.
 *                    6.    Function for resetting a device.
 *
 *
 ******************************************************************************/

#ifndef __USBHUBCLASSINTERFACE_H___
#define __USBHUBCLASSINTERFACE_H___

#ifdef __cplusplus
LOCAL "C" {
#endif

/*******************************************************************************
 * Function Name    : HUB_AddHub
 * Description      : This function is called by USB Host Software for
 *                    adding a new hub and to configure it.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    uInterfaceNumber  IN   Interface number corresponding to
 *                                           the hub class functionality.
 *                    pContext          OUT  Pointer to the context variable.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if the parameters are
 *                    wrong/ when it is unable to find a device in topology.
 *                    USBHST_FAILURE if the operation failed.
 *                    USBHST_SUCCESS if the operation passed.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubAdd
                 (
                 UINT32 uDeviceHandle,
                 UINT8 uInterfaceNumber,
                 UINT8 uSpeed,
                 void ** pContext
                 );


/*******************************************************************************
 * Function Name    : HUB_RemoveHub
 * Description      : This function is called by the USB Host Software Stack
 *                    for deletion of a hub.
 *                    In this process it removes its children and does the
 *                    required clean up.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    pContext          IN   Pointer to the context variable.
 * Return Type      : None.
 ******************************************************************************/

LOCAL void usbHubRemove
          (
          UINT32 uDeviceHandle,
          void * pContext
          );

/*******************************************************************************
 * Function Name    : HUB_SuspendHub
 * Description      : This function is called by the USB Host Software Stack
 *                    for suspending a hub.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    pContext          IN   Pointer to the context variable.
 * Return Type      : None.
 ******************************************************************************/

LOCAL void usbHubSuspend
          (
          UINT32 uDeviceHandle,
          void * pContext
          );

/*******************************************************************************
 * Function Name    : HUB_ResumeHub
 * Description      : This function is called by the USB Host Software Stack
 *                    for resuming a hub.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    pContext          IN   Pointer to the context variable.
 * Return Type      : None.
 ******************************************************************************/

LOCAL void usbHubResume
          (
          UINT32 uDeviceHandle,
          void * pContext
          );

/*******************************************************************************
 * Function Name    : HUB_SelectiveSuspendDevice
 * Description      : This function is called by the USB Host Software Stack for
 *                    selectively suspending a device. In this process, it
 *                    selectively suspends the port of the parent hub where the
 *                    device is connected.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 * Return Type      : None.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubSelectiveSuspendDevice (UINT32 uDeviceHandle);


/*******************************************************************************
 * Function Name    : HUB_SelectiveResumeDevice
 * Description      : This function is called by the USB Host Software Stack for
 *                    selectively resuming a device. In this process it
 *                    selectively resumes the port of the parent hub where the
 *                    device is connected.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 * Return Type      : None.
 ******************************************************************************/

LOCAL  USBHST_STATUS usbHubSelectiveResumeDevice (UINT32 uDeviceHandle);


/*******************************************************************************
 * Function Name    : HUB_AddRootHub
 * Description      : This function is called by the USB Host Software Stack
 *                    for adding a new root hub. In this process, it configures
 *                    the root hub and also initializes the structures to handle
 *                    the bus under the root hub.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    uBusHandle        IN   Unique identifier for the bus
 * Return Type      : None.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubAddRoot
                  (
                  UINT32 uDeviceHandle,
                  UINT8 uBusHandle,
                  UINT8 uSpeed
                  );

/*******************************************************************************
 * Function Name    : HUB_RemoveRootHub
 * Description      : This function is called by the USB Host Software Stack for
 *                    removing a root hub.
 * Parameters       : uBusHandle        IN   Unique identifier for the bus
 * Return Type      : None.
 ******************************************************************************/

LOCAL void usbHubRemoveRoot (UINT8 uBusHandle);

/*******************************************************************************
 * Function Name    : HUB_CheckPower
 * Description      : This function is called by the USB Host Software Stack for
 *                    checking if the parent hub can support a power for a
 *                    device.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    uPowerRequirement OUT  Power Requirement for the  device
 *                                           in units of 2 mA.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if the parameters are
 *                    wrong.
 *                    Returns USBHST_FAILURE if the power cannot be supported.
 *                    Returns USBHST_SUCCESS if the power can be supported.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubCheckPower
	           (
                   UINT32 uDeviceHandle,
                   UINT8 uPowerRequirement
                   );

/*******************************************************************************
 * Function Name    : HUB_ResetDevice
 * Description      : This function is called by the USB Host Software Stack
 *                    for resetting a device.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 * Return Type      : None.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubResetDevice (UINT32 uDeviceHandle, INT32  timeoutMS);


/*******************************************************************************
 * Function Name    : HUB_ClearTT
 * Description      : This function submits a request for clear TT
 * Parameters       : uHubHandle        IN    Hub handle.
 * 					  uPortNumber       IN    Port To which the TT is attached
 *                    uValue            IN    wValue for ClearTT request
 *
 *                    pContext          IN    Context for USBD
 * Return Type      : a) Status
 *
 ******************************************************************************/
LOCAL USBHST_STATUS usbHubClearTT( UINT32 uHubHandle ,UINT8 uPortNumber ,
                             UINT16 uValue, void * pContext );
/*******************************************************************************
 * Function Name    : HUB_ResetTT
 * Description      : This function submits a request for clear TT
 * Parameters       : uHubHandle        IN    Hub handle.
 *                    uPortNumber       IN    Port To which the TT is attached
 *                    pContext          IN    Context for USBD
 * Return Type      : a) Status
 ******************************************************************************/
LOCAL USBHST_STATUS usbHubResetTT(UINT32 uHubHandle , UINT8 uPortNumber ,
                            void * pContext);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef __USBHUBCLASSINTERFACE_H___ */

/**************************** End of file HUB_ClassInterface.h ****************/
