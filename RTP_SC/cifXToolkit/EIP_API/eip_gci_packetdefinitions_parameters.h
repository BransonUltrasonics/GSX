/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_gci_packetdefinitions_parameters.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines parameter flags applicable to the packets
EIP_OBJECT_SET_PARAMETER_REQ and EIP_APS_SET_PARAMETER_REQ.
They dynamically enable or disable certain optional features.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/


#ifndef EIP_GCI_PACKETDEFINITIONS_PARAMETERS_H_
#define EIP_GCI_PACKETDEFINITIONS_PARAMETERS_H_

/* Option-Flags for EIP_OBJECT_SET_PARAMETER_REQ */
#define EIP_OBJECT_PRM_FWRD_OPEN_CLOSE_FORWARDING    0x00000001 /*!< Setting this flag the host application will
                                                                     receive the complete forward_open/close message
                                                                     from the stack. */
#define EIP_OBJECT_PRM_DISABLE_FLASH_LEDS_SERVICE    0x00000002 /*!< Setting this flag the host application will
                                                                     disable the Flash_LEDs service of the Identity object */


/* Option-Flags for EIP_APS_SET_PARAMETER_REQ */
#define EIP_APS_PRM_SIGNAL_MS_NS_CHANGE              0x00000001 /*!< This flag enables or disables the notification of
                                                                     the network and module status. Every time the status
                                                                     of the module or network changes packet
                                                                     EIP_APS_MS_NS_CHANGE_IND will be sent to the
                                                                     registered EtherNet/IP Application task.    */
#endif


