/* usbHubEventHandler.h - Functions for handling HUB events */

/*
 * Copyright (c) 2003, 2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2010, 2014 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
23jan14,j_x  warnings cleanup
13jan10,ghs  vxWorks 6.9 LP64 adapting
27jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This provides the functions for handling the hub events.This is
used in conjunction with the Port Event Handling Module and
Bus Manager Module.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_HubEventHandler.h
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
 * Description      : This provides the functions for handling the hub events.
 *                    This is used in conjunction with the Port Event Handling
 *                    Module and Bus Manager Module.
 *
 ******************************************************************************/


#ifndef __USBHUBHUBEVENTHANDLER_H___
#define __USBHUBHUBEVENTHANDLER_H___


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Function Name    : HUB_HubEventHandler
 * Description      : This is the function to handle the hub event. This has
 *                    the maximum priority over any other event. Each Bus
 *                    structure maintains a variable whose count is incremented
 *                    every time a hub event is detected and decremented every
 *                    time a hub event is handled. Only when this counter is 0
 *                    are other events handled.
 * Parameters       : pHub              IN   Pointer to the hub structure which
 *                                           has to be checked.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If errors were encountered while
 *                            handling hub event.
 *                    Returns USBHST_SUCCESS If the hub event handling
 *                            was success.
 ******************************************************************************/

USBHST_STATUS usbHubEventHandler (pUSB_HUB_INFO   pHub);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef __HUB_HUBEVENTHANDLER_H___ */

/**************************** End of file HUB_HubEventHandler.h ***************/
