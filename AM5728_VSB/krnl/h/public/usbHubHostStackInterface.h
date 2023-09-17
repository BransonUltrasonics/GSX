/* usbHubHostStackInterface.h - Allows the host stack to interact with hub
class driver */

/* Copyright (c) 2003, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2010 Wind River Systems, Inc.

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
01a,27jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This provides the structure that allow the USB Host Stack
Software to interact with the USB Hub Class Driver.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_HostStackInterface.h
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
 * Description      : This provides the structure that allow the USB Host Stack
 *                    Software to interact with the USB Hub Class Driver.
 *
 ******************************************************************************/


#ifndef __USBHUBHOSTSTACKINTERFACE_H__
#define __USBHUBHOSTSTACKINTERFACE_H__


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This structure is used to store the pointers to USB Hub Class Driver function
 * pointers that are visible to only USB Host Software Stack.
 */
 typedef struct usb_hub_function_list
{
    /* Function pointer to suspend a device */
    USBHST_STATUS (*usbHubSelectiveSuspendDevice)( UINT32 uDeviceHandle);

    /* Function pointer to resume a device */
    USBHST_STATUS (*usbHubSelectiveResumeDevice) ( UINT32 uDeviceHandle);

    /* Function pointer to add a root hub */
    USBHST_STATUS (*usbHubAddRoot)   ( UINT32 uDeviceHandle, UINT8 uBusHandle ,UINT8 uSpeed);

    /* Function pointer to remove a root hub */
    void          (*usbHubRemoveRoot)( UINT8 uBusHandle);

    /*
     * Function pointer to check whether the power for a device can be supported
     */
    USBHST_STATUS (*usbHubCheckForPower)( UINT32 uDeviceHandle,
                                     UINT8 uPowerRequirement);
    /* Function pointer to reset a device */
    USBHST_STATUS (*usbHubResetDevice)   ( UINT32 uDeviceHandle, INT32 timeoutMS);

} USB_HUB_FUNCTION_LIST, *pUSB_HUB_FUNCTION_LIST;



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef __HUB_HOSTSTACKINTERFACE_H__ */

/********************** End of file HUB_HostStackInterface.h ******************/
