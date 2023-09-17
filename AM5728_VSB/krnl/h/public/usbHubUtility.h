/* usbHubUtility.h - Manages topology and other information about bus */

/*
 * Copyright (c) 2003-2004, 2006, 2009-2012, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003-2004, 2006, 2009-2012, 2014 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
23jan14,j_x  warnings cleanup
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
13dec11,m_y  Modify according to code check result (WIND00319317)
29jun10,w_x  Add more port reset delay and clean hub logging (WIND00216628)
13jan10,ghs  vxWorks 6.9 LP64 adapting
17jul09,w_x  Fix crash in reset callback (WIND00172353)
06jan06,ami  New macros defined (SPR #115386)
24nov04,hch  Add macros for USB BusM task name.
27jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This is the module that manages the topology and other  information
about a bus. This is an autonomous thread that is initiated every time
a root hub is detected and this ends only when the USB Hub Class Driver
is unloaded or when the root hub is disabled. This is used in conjunction
with the Port Event Handling Module and Hub Event Handling Module.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_Utility.h
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
 * Description      : This is the module that manages the topology and other
 *                    information about a bus. This is an autonomous thread that
 *                    is initiated every time a root hub is detected and this
 *                    ends only when the USB Hub Class Driver is unloaded or
 *                    when the root hub is disabled. This is used in conjunction
 *                    with the Port Event Handling Module and
 *                    Hub Event Handling Module.
 *
 *
 ******************************************************************************/

#ifndef __USBHUB_UTILITY_H___
#define __USBHUB_UTILITY_H___

#ifdef __cplusplus
LOCAL "C" {
#endif


/* includes */

#include <usbOsal.h>
#include <usbHst.h>
#include <usbHubCommon.h>

/* defines */

#define USB_BUSM_TASK_NAME_BASE         "BusM  "
#define USB_BUSM_TASK_NAME_BASE_LEN     7

/*
 * The max reset check count multiplies with the 10ms internal delay, total
 * about 10000ms (10s) delay, which should be in a user acceptable range.
 */

#define USB_HUB_MAX_RESET_CHECK_COUNT          1000

#define USB_HUB_PPORT_POWER_SELF_POWERED       250 /* per-port power for a */
                                                   /* self powered hub */
#define USB_HUB_PPORT_POWER_BUS_POWERED        50  /* per-port power for a */
                                                   /* bus-poered hub */
#define USB_HUB_POWER_CHECK                 0x0001 /* hub bus/self power */


/************************ GLOBAL FUNCTIONS DECLARATION ************************/

/*******************************************************************************
 * Function Name    : HUB_FindPortNumber
 * Description      : This function finds the port number of a device given the
 *                    device handle and hub pointer.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the device.
 *                    pHub              IN   search at this Hub.
 * Return Type      : PORT_NUMBER_NOT_FOUND if the match was not found/invalid
 *                    params.
 *                    If found, it returns the port number.
 ******************************************************************************/

UINT8 usbHubFindPortNumber
       (
       pUSB_HUB_INFO  pHub,
       UINT32 uDeviceHandle
       );


/*******************************************************************************
 * Function Name    : HUB_FindParentHubInBuses
 * Description      : This searches for a parent hub.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the device.
 * Return Type      : NULL if the match was not found/invalid params.
 *                    If found, it returns pointer to the PHUB_INFO structure of
 *                    the parent hub.
 ******************************************************************************/

pUSB_HUB_INFO usbHubFindParentHubInBuses (UINT32 uDeviceHandle);


/*******************************************************************************
 * Function Name    : HUB_FindParentHub
 * Description      : This function recursively searches for the parent hub of
 *                    the specified device.
 * Parameters       : uDeviceHandle     IN   Unique identifier for the device.
 *                    pHub              IN   Start search at this Hub.
 * Return Type      : NULL if the match was not found/invalid params.
 *                    If found, it returns pointer to the PHUB_INFO structure of
 *                    the parent hub.
 ******************************************************************************/

pUSB_HUB_INFO usbHubFindParentHub
               (
               UINT32 uDeviceHandle ,
               pUSB_HUB_INFO pHub
               );


/*******************************************************************************
 * Function Name    : HUB_CreateBusStructure
 * Description      : This function creates a bus structure and connects the
 *                    same to the global Bus list. This also launches a
 *                    thread to handle the events occurring on the Bus.
 * Parameters       : uBusHandle        IN   Unique identifier for the bus
 * Return Type      : This returns NULL if If the bus structure was unable to be
 *                    created, thread was unable to be created,
 *                    memory allocation failure.
 *                    This returns HUB_BUS_INFO if the The pointer to the bus
 *                    structure which was created.
 ******************************************************************************/

pUSB_HUB_BUS_INFO usbHubCreateBusStructure
                   (
                   UINT8 uBusHandle
                   );


/*******************************************************************************
 * Function Name    : HUB_DeleteBusStructure
 * Description      : This function deletes the Bus Structure and kills any
 *                    thread associated with it.
 * Parameters       : uBusHandle        IN   Unique identifier for the bus
 * Return Type      : None.
 ******************************************************************************/

void usbHubDeleteBusStructure ( UINT8 uBusHandle);


/*******************************************************************************
 * Function Name    : HUB_PortDebounceHandler
 * Description      : This function is used for handling de-bounce condition.
 * Parameters       : pHub         IN  The hub on which the event occurred.
 *                    uPortNumber  IN  The port number where the event occurred.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If errors were encountered while
 *                            handling de-bounce.
 *                    Returns USBHST_SUCCESS If the handling was successful/ the
 *                            time is not reached yet.
 ******************************************************************************/

USBHST_STATUS usbHubPortDebounceHandler
               (
               pUSB_HUB_INFO pHub,
               UINT8 uPortNumber
               );


/*******************************************************************************
 * Function Name    : HUB_ConfigureHub
 * Description      : This function configures a given hub.
 * Parameters       : pHub              OUT  Returns the hub Structure
 *                    uDeviceHandle     IN   Unique identifier for the hub
 *                                           device.
 *                    pBus              IN   Pointer to the Bus Structure of
 *                                           which this hub would be a child.
 *                    pParentHub        IN   Pointer to the Parent of the hub
 *                                           to be added.
 * Return Type      : Returns USBHST_INSUFFICIENT_MEMORY if the
 *                    memory allocation failed.
 *                    returns USBHST_FAILURE If any requests
 *                    failed due to errors.
 *                    Returns USBHST_SUCCESS If the configuration
 *                    was successful.
 ******************************************************************************/

USBHST_STATUS usbHubConfigure
               (
               pUSB_HUB_INFO      * pHub,
               UINT32              uDeviceHandle,
               pUSB_HUB_BUS_INFO   pBus,
               pUSB_HUB_INFO       pParentHub
               );


/*******************************************************************************
 * Function Name    : HUB_HandleDeviceConnection
 * Description      : This function handles the device connection.
 * Parameters       : pHub        IN   The hub on which the device is connected.
 *                    uPortNumber IN   The port on which the device is
 *                                     connected.
 * Return Type      : Returns USBHST_INVALID_PARAMETERS If there are errors in
 *                            parameters passed.
 *                    Returns USBHST_SUCCESS other wise.
 ******************************************************************************/

 USBHST_STATUS usbHubHandleDeviceConnection
               (
               pUSB_HUB_INFO pHub,
               UINT8         uPortNumber
               );


/*******************************************************************************
 * Function Name    : HUB_RetryDeviceConfiguration
 * Description      : This function is called to retry a device configuration.
 *                    If the configuration retries have expired, then
 *                    the port is disabled.
 * Parameters       : pHub        IN   The hub on which the device configuration
 *                                     is retried.
 *                    uPortNumber IN   The port number on which the device
 *                                     configuration is retried.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If errors were encountered while
 *                            handling re-configuration.
 *                    Returns USBHST_SUCCESS If the re-configuration was success
 ******************************************************************************/

USBHST_STATUS usbHubRetryDeviceConfiguration
               (
               pUSB_HUB_INFO pHub,
               UINT8         uPortNumber
               );


/*******************************************************************************
 * Function Name    : HUB_PowerOnPort
 * Description      : This function is to re-enable a port that has caused a
 *                    serious error.
 * Parameters       : pHub         IN   The hub on which the device is to be
 *                                      re-enabled.
 *                    uPortNumber  IN   The port on the hub on which the device
 *                                      is to be re-enabled.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If Error in trying to power
 *                            on the port.
 *                    Returns USBHST_SUCCESS If the power on of the port was
 *                            successfully initiated.
 ******************************************************************************/

USBHST_STATUS usbHubPowerOnPort
               (
               pUSB_HUB_INFO pHub,
               UINT8         uPortNumber
               );


/*******************************************************************************
 * Function Name    : HUB_ResetPort
 * Description      : This function is used to reset a Port. This function
 *                    ensures that the reset is issued to only one device per
 *                    bus.
 * Parameters       : pHub         IN  The hub on which the reset has to be
 *                                     initiated.
 *                    uPortNumber  IN  The port number on which the reset has
 *                                     to be initiated.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If Error in trying to reset
 *                            the port.
 *                    Returns USBHST_SUCCESS If the reset was successfully
 *                            initiated.
 ******************************************************************************/

USBHST_STATUS usbHubResetPort
                   (
                   pUSB_HUB_INFO pHub,
                   UINT8 uPortNumber
                   );


/*******************************************************************************
 * Function Name    : HUB_SubmitControlRequest
 * Description      : This function is to submit a blocking feature request to
 *                    the hub.
 * Parameters       : pHub       IN   The pointer to the hub where the control
 *                                    request has to be issued.
 *                    bmRequest  IN   The bmRequest field.
 *                    bRequest   IN   The bRequest field.
 *                    wValue     IN   The wValue field.
 *                    wIndex     IN   The wIndex field.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If Error in trying to submit a
 *                            control request to the hub.
 *                    Returns USBHST_SUCCESS If the submission of control
 *                            request to the hub is successful.
 ******************************************************************************/

USBHST_STATUS usbHubSubmitControlRequest
               (
               pUSB_HUB_INFO   pHub,
               UINT8           bmRequest,
               UINT8           bRequest,
               UINT16          wValue,
               UINT16          wIndex
               );


/*******************************************************************************
 * Function Name    : HUB_GetHubDescriptor
 * Description      : This function is to retrieve the Hub descriptor.
 * Parameters       : uDeviceHandle  IN     The Device handle of the hub device.
 *                    pBuffer        IN OUT The buffer to copy the data to.
 *                    pBufferLength  IN OUT The length of the buffer.
 * Return Type      : a) Returns USBHST_INVALID_PARAMETER if parameters
 *                       are wrong.
 *                    b) Returns USBHST_FAILURE If Error in trying to submit a
 *                       GetHubDescriptor request to the hub.
 *                    c) Returns USBHST_SUCCESS If the submission of
 *                       get status request to the hub is successful.
 ******************************************************************************/

USBHST_STATUS usbHubGetDescriptor
               (
               UINT32    uDeviceHandle,
               UINT8  *  pBuffer,
               UINT32 *  pBufferLength
               );

/*******************************************************************************
*
* usbHubSetHubDepth - issue the Set Hub Depth hub class specific request 
*
* This routine is to issue the Set Hub Depth hub class specific request.
*
* RETURNS: USBHST_SUCCESS, USBHST_FAILURE, USBHST_INVALID_PARAMETERS if
* parameters are wrong.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

USBHST_STATUS usbHubSetHubDepth
                    (
                    UINT32   uDeviceHandle,
                    UINT16   uHubDepth
                    );

/*******************************************************************************
 * Function Name    : HUB_GetStatus
 * Description      : This function is to submit a blocking get status
 *                    request to the hub.
 * Parameters       : pHub           IN     The Hub in which the status is to be
 *                                          recieved.
 *                    uPortNumber    IN     If this is a GetPortStatus request
 *                                          then this contains the port number
 *                                          Else this is 0.
 *                    bmRequest      IN     Specifies if this is a hub status or
 *                                          a request for port status request.
 *                    pBuffer        IN OUT The buffer to copy the data to.
 *                    pBufferLength  IN OUT The length of the buffer.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If Error in trying to submit a
 *                            getStatus request to the hub.
 *                    Returns USBHST_SUCCESS If the submission of
 *                            get status request to the hub is successful.
 ******************************************************************************/

USBHST_STATUS usbHubGetStatus
               (
               pUSB_HUB_INFO  pHub,
               UINT8          uPortNumber,
               UINT8          bmRequest,
               UINT8 *        pBuffer,
               UINT8 *        pBufferLength
               );

/*******************************************************************************
*
* usbHubGetPortStatus - submit a get port status request to the hub
*
* This routine submits a get port status request to the hub.
*
* RETURNS: USBHST_SUCCESS, USBHST_FAILURE, USBHST_INVALID_PARAMETERS if
* parameters are wrong.
*
* ERRNO: None
*
* \NOMANUAL
*/

USBHST_STATUS usbHubGetPortStatus
              (
              pUSB_HUB_INFO  pHub,
              UINT8      wIndex,
              UINT8      bmRequest,
              UINT8  *   pBuffer,
              UINT8  *   pBufferLength
              );


/*******************************************************************************
* Function Name    : HUB_SubmitInterruptRequest
 * Description      : This function is used for submitting a status change
 *                    Interrupt IN request.
 * Parameters       : pHub    IN  The hub on which the status change interrupt
 *                                IN request has to be initiated.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If Error in trying to submit a
 *                            status change interrupt IN request to the hub.
 *                    Returns USBHST_SUCCESS If the submission of status change
 *                            interrupt IN request to the hub was successful.
 ******************************************************************************/

USBHST_STATUS usbHubSubmitInterruptRequest (pUSB_HUB_INFO pHUB);



/*******************************************************************************
* Function Name     : HUB_FindNearestParentHub
 * Description      : This function is used to find the nearest high speed parent
 *                    hub
 * Parameters       : uDeviceHandle    IN  Handle to the device
 *                    pHub             IN  Pointer to HubInfo structure
 *                    pPortNumber      OUT Pointer to port number
 * Return Type      : Returns PHUB_INFO pointer to the nearest high speed
 *                    parent Hub
 *
 ******************************************************************************/

pUSB_HUB_INFO usbHubFindNearestParent
               (
               UINT32        uDeviceHandle ,
               pUSB_HUB_INFO pHub ,
               UINT8 *       pPortNumber
               );



/*******************************************************************************
 * Function Name    : HUB_RemoveDevice
 * Description      : This calls the remove device of the Host stack.This
 *                    also frees the memory as required.
 * Parameters       : pHub        IN   pointer to the Parent Hub.
 *                    uPortIndex  IN   port Index
 * Return Type      : a) USBHST_INVALID_PARAMETER if there are invalid params
 *                    b) USBHST_SUCCESS other wise.
 *
 ******************************************************************************/

USBHST_STATUS usbHubRemoveDevice(pUSB_HUB_INFO pHub, UINT8 uPortIndex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __HUB_UTILITY_H___ */

/**************************** End of file HUB_Utility.h ***********************/
