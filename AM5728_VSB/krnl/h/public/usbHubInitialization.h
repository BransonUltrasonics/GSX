/* usbHubInitialization.h - Initialization and cleanup for hub class driver */

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

This module provides the initialization and the clean up
functions for the USB Hub Class Driver.
*/



/*
INTERNAL
 *******************************************************************************
 * Filename         : HUB_Initialization.h
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
 * Description      : This module provides the initialization and the clean up
 *                    functions for the USB Hub Class Driver.
 *
 *
 ******************************************************************************/


#ifndef __USBHUBINITIALIZATION_H___
#define __USBHUBINITIALIZATION_H___


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Function Name    : HUB_Init
 * Description      : This function initializes the global variables and
 *                    registers USB Hub Class Driver function pointers with the
 *                    USB Host Software Stack.
 *                    This also retrieves the USB Host Software Stack functions
 *                    for future access.
 * Parameters       : None.
 * Return Type      : This returns 0 if the operation was successful.
 *                    Else this returns -1.
 ******************************************************************************/

INT8 usbHubInit(void);


/*******************************************************************************
 * Function Name    : HUB_Exit
 * Description      : This function de-registers and cleans up the
 *                    USB Hub Class Driver from the USB Host Software Stack.
 * Parameters       : None.
 * Return Type      : This returns 0 if the operation was successful.
 *                    Else this returns -1.
 ******************************************************************************/

INT8 usbHubExit(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef __HUB_INITIALIZATION_H___ */

/**************************** End of file HUB_Initialization.h ****************/
