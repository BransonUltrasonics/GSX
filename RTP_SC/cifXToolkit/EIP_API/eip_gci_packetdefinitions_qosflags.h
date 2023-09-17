/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_gci_packetdefinitions_qosflags.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines flags applicable to the QoS configuration provided with packet
EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/


#ifndef EIP_GCI_PACKETDEFINITIONS_QOSFLAGS_H_
#define EIP_GCI_PACKETDEFINITIONS_QOSFLAGS_H_

#define EIP_OBJECT_QOS_FLAGS_ENABLE          0x00000001
#define EIP_OBJECT_QOS_FLAGS_DEFAULT         0x00000002
#define EIP_OBJECT_QOS_FLAGS_DISABLE_802_1Q  0x00000004

#endif
