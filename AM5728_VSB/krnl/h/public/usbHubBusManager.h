/* usbHubBusManager.h - Manages the topology and other info about a bus */

/*
 * Copyright (c) 2003, 2005, 2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2005, 2010, 2014 Wind River Systems, Inc.

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
01dec05,ami  Typo rectified (SPR #109940 Fix)
27jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This is the module that manages the topology and other information about
a bus. This is an autonomous thread that is initiated every time a root
hub is detected and this ends only when the USB Hub Class Driver is
unloaded or when the root hub is disabled. This is used in conjunction
with the Port Event Handling Module and Hub Event Handling Module.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_BusManager.h
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

#ifndef __USBHUBBUSMANAGER_H___
#define __USBHUBBUSMANAGER_H___

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Function Name    : HUB_MainThread
 * Description      : This Function is the thread that keeps checking every hub
 *                    on a bus for events that require handling.
 * Parameters       : pContext     IN   The pointer to the bus to work on.
 * Return Type      : None.
 ******************************************************************************/
void usbHubThread(void * pContext);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __USBHUBBUSMANAGER_H___ */

/**************************** End of file HUB_BusManager.h ********************/

