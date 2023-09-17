/* usbdMisc.h - USBD Miscellaneous Definitions */

/* Copyright 2004 Wind River Systems, Inc. 

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according  
   to the terms of their Wind River license agreement(s) applicable to 
   this software.
*/

/*
Modification history
--------------------
01a,17mar03,ssh  written
*/

/*
DESCRIPTION

This file contains the function prototypes, macros and structures 
required for interfaces that provide user specific information 
from USBD
*/


/*
INTERNAL
 *******************************************************************************
 * Filename         : usbdMisc.h
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
 * Description      : This file contains the function prototypes, macros and
 *                    structures required for interfaces that provide user
 *                    specific information from USBD
 *
 *
 ******************************************************************************/

 #ifndef __INCusbdMisch
 #define __INCusbdMisch


#ifdef __cplusplus
extern "C" {
#endif

/******************************** GLOBAL FUNCTIONS ***************************/

/*******************************************************************************
 * Function Name    : usbdBusCntGet
 * Description      : This function is called by the Integration testing tool to
 *                    obtain the number of USB's attached to the host.
 * Parameters       : Total number of buses(host controllers)on the system
 * Return Type      : UINT16 (bus count)
 * Global Variables : gHCDriverList
 * Calls            : None
 * Called by        : Translation Unit
 * To Do            : None
 ******************************************************************************/
UINT16 usbdBusCntGet(void);

 /*******************************************************************************
 * Function Name    : usbdRootNodeIDGet
 * Description      : This function is called by the Integration testing tool to
 *                    obtain the node id of the root hub on a Host Controller.
 * Parameters       : uBusIndex  - Bus Index
 *                    phRootHub  - Pointer to the root hub handle
 * Return Type      : USBHST_SUCCESS           - If successful
 *                    USBHST_FAILURE           - If unsuccessful
 *                    USBHST_INVALID_PARAMETER - If invalid parameter was passed
 * Global Variables : None
 * Calls            : None
 * Called by        : Translation Unit
 * To Do            : None
 ******************************************************************************/
 USBHST_STATUS usbdRootNodeIDGet(UINT16   uBusIndex,
								 PUINT32 phRootHub);

#if 0
/***************************************************************************
*
* usbdHubPortCntGet - Returns number of ports connected to a hub
*
* usbdHubPortCountGet() provides clients with a convenient mechanism to
* retrieve the number of downstream ports provided by the specified hub.
* Clients can also retrieve this information by retrieving configuration
* descriptors from the hub using the Configuration Functions describe in
* a following section.
*
* <hubId> must be the Node Id for the desired USB hub.	An error will be
* returned if <hubId> does not refer to a hub.	<pPortCount> must point to
* a UINT16 variable in which the total number of ports on the specified
* hub will be stored.
* Return Type      : USBHST_SUCCESS           - If successful
*                    USBHST_FAILURE           - If unsuccessful
*                    USBHST_INVALID_PARAMETER - If invalid parameter was passed
*
*/

USBHST_STATUS usbdHubPortCntGet
    (
    UINT32 hubId, 		/* Node Id for desired hub */
    PUINT16 pPortCount			/* bfr to receive port count */
    );
/***************************************************************************
*
* usbdNodeIDGet - Gets the id of the node connected to a hub port
*
* Clients use this function to retrieve the Node Id for devices attached to
* each of a hub’s ports.  <hubId> and <portIndex> identify the hub and port
* to which a device may be attached.  <pNodeType> must point to a UINT16
* variable to receive a type code as follows:
*
* .IP "USB_NODETYPE_NONE"
* No device is attached to the specified port.
* .IP "USB_NODETYPE_HUB"
* A hub is attached to the specified port.
* .IP "USB_NODETYPE_DEVICE"
* A device (non-hub) is attached to the specified port.
*
* If the node type is returned as USBD_NODE_TYPE_NONE, then a Node Id is
* not returned and the value returned in <pNodeId> is undefined.  If the
* node type indicates a hub or device is attached to the port, then
* <pNodeId> will contain that hub or device’s nodeId upon return.
*
* RETURNS: OK, or ERROR if unable to get node ID.
*/

USBHST_STATUS usbdNodeIDGet
    (
    UINT32 hubId, 		/* Node Id for desired hub */
    UINT16 portIndex,			/* Port index */
    PUINT16 pNodeType,			/* bfr to receive node type */
    PUINT32 pNodeId		/* bfr to receive Node Id */
    );


 /*******************************************************************************
 * Function Name    : usbdFrameWindowGet
 * Description      : This function is called by the Integration testing tool to
 *                    obtain the Frame Window and current frame number from HCD.
 * Parameters       : nodeId - Node ID of the device
 *                    pFrameWindow - Pointer to frame information i.e,Max frame
 *                                   scheduling window and current frame number
 * Return Type      : USBHST_SUCCESS           - If successful
 *                    USBHST_FAILURE           - If unsuccessful
 *                    USBHST_INVALID_PARAMETER - If invalid parameter was passed
 * Global Variables : g_pHCDriverList
 * Calls            : None
 * Called by        : None
 * To Do            : None
 ******************************************************************************/

#endif

USBHST_STATUS usbdFrameWindowGet(UINT32  nodeId,
                                 PUINT16 pFrameWindow);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* End of #ifndef __INCusbdMisch */
 /**************************** End of usbdMisc.h ****************************/
