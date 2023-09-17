/* usbHubPortEventHandler.h - Functions for handling port events */

/*
 * Copyright (c) 2003, 2010 Wind River Systems, Inc.
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
01b,13jan10,ghs  vxWorks 6.9 LP64 adapting
01a,27jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This provides the functions for handling the port events.This module
is used in conjunction with the Hub Event Handling Module and Bus
Manager Module.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_PortEventHandler.h
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
 * Description      : This provides the functions for handling the port events.
 *                    This module is used in conjunction with the Hub Event
 *                    Handling Module and Bus Manager Module.
 *
 ******************************************************************************/

#ifndef __USBHUBPORTEVENTHANDLER_H___
#define __USBHUBPORTEVENTHANDLER_H___


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Function Name    : HUB_PortEventHandler
 * Description      : This function is the handler for any port event that has
 *                    happened.
 * Parameters       : pHub        IN  The hub on which the event occurred.
 *                    uPortNumber IN  The port number where the event occurred.
 * Return Type      : Returns USBHST_INVALID_PARAMETER if parameters are wrong.
 *                    Returns USBHST_FAILURE If any event handler failed.
 *                    Returns USBHST_SUCCESS If all the events were handled
 *                            successfully.
 ******************************************************************************/

LOCAL USBHST_STATUS usbHubPortEventHandler
                   (
                   pUSB_HUB_INFO pHub,
                   UINT8         uPortNumber
                   );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __HUB_PORTEVENTHANDLER_H___ */

/**************************** End of file HUB_PortEventHandler.h **************/
