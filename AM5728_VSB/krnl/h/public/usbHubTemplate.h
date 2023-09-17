/* usbHubTemplate.h - Sample description */

/* Copyright 2004 Wind River Systems, Inc. 

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according  
   to the terms of their Wind River license agreement(s) applicable to 
   this software.
*/

/*
Modification history
--------------------
01a,27jun03,nrv Changing the code to WRS standards
*/

/*
DESCRIPTION

This is the module that is used to used to perform the host side 
data transfer validations.
*/ 


  
/*
INTERNAL
 *******************************************************************************
 * Filename         : Sample.h
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
 * Description      : This is a sample description
 *                    Some other sample description.
 *
 *
 ******************************************************************************/

#ifndef __USBHUBTEMPLATE_H__
#define __USBHUBTEMPLATE_H__

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Function Name : HostDataTransferTest
 * Description   : This function is used to perform the host side data transfer
 *                 validations.
 * Parameters    : uTransferType        IN  Specifies the type of Data Transfer.
 *                 uNumberOfIterations  IN  Specifies the number of iterations
 *                                          to be performed.
 * Return Type   : None
 ******************************************************************************/

void UsbHostDataTransferTest
                (
                unsigned int uTransferType,
                unsigned int uNumberOfIterations
                );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef __SAMPLE_H__ */

/**************************** End of file Sample.h ****************************/
