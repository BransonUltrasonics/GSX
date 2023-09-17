/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_gci_packetdefinitions_commands.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines all specific packet command codes of the EtherNet/IP stack.
These definitions contribute to the LFW API of the EtherNet/IP stack and are applicable
to the DPM packet interface.

All specific packet commands are contained in a unified fashion for master and slave
devices.

**************************************************************************************/


#ifndef EIP_GCI_PACKETDEFINITIONS_COMMANDS_H_
#define EIP_GCI_PACKETDEFINITIONS_COMMANDS_H_

/* Packet command codes */

#define EIP_APS_PACKET_START                        0x00003600

#if 0
/* This command is no longer supported with
 * EtherNet/IP Adapter v3.4.1.0 and newer versions
 */
#define EIP_APS_SET_CONFIGURATION_REQ               0x00003608
#define EIP_APS_SET_CONFIGURATION_CNF               0x00003609
#endif

#define EIP_APS_SET_PARAMETER_REQ                   0x0000360A
#define EIP_APS_SET_PARAMETER_CNF                   0x0000360B

#define EIP_APS_MS_NS_CHANGE_IND                    0x0000360C
#define EIP_APS_MS_NS_CHANGE_RES                    0x0000360D

#define EIP_APS_GET_MS_NS_REQ                       0x0000360E
#define EIP_APS_GET_MS_NS_CNF                       0x0000360F

#define EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ    0x00003612
#define EIP_APS_SET_CONFIGURATION_PARAMETERS_CNF    0x00003613

#define EIP_APS_CONFIG_DONE_REQ                     0x00003614
#define EIP_APS_CONFIG_DONE_CNF                     0x00003615

#define EIP_APS_PACKET_END                          0x00003615

/***************************************************************************************/

#define EIP_OBJECT_PACKET_START                     0x00001A00

#define EIP_OBJECT_MR_REGISTER_REQ                  0x00001A02
#define EIP_OBJECT_MR_REGISTER_CNF                  0x00001A03

#define EIP_OBJECT_AS_REGISTER_REQ                  0x00001A0C
#define EIP_OBJECT_AS_REGISTER_CNF                  0x00001A0D

/*
  Note:
  Support for EIP_OBJECT_ID_SETDEVICEINFO_REQ is removed since version V3.5.0.0 of the Hilscher EtherNet/IP stack.
  Please use EIP_OBJECT_CIP_SERVICE_REQ to set identity information towards your netX device.
*/
#if 0
#define EIP_OBJECT_ID_SETDEVICEINFO_REQ             0x00001A16
#define EIP_OBJECT_ID_SETDEVICEINFO_CNF             0x00001A17
#endif

#define EIP_OBJECT_RESET_IND                        0x00001A24
#define EIP_OBJECT_RESET_RES                        0x00001A25

#define EIP_OBJECT_CONNECTION_IND                   0x00001A2E
#define EIP_OBJECT_CONNECTION_RES                   0x00001A2F

#define EIP_OBJECT_CL3_SERVICE_IND                  0x00001A3E
#define EIP_OBJECT_CL3_SERVICE_RES                  0x00001A3F

#define EIP_OBJECT_REGISTER_SERVICE_REQ             0x00001A44
#define EIP_OBJECT_REGISTER_SERVICE_CNF             0x00001A45

#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND      0x00001A4C
#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_RES      0x00001A4D

#define EIP_OBJECT_FWD_CLOSE_FWD_IND                0x00001A4E
#define EIP_OBJECT_FWD_CLOSE_FWD_RES                0x00001A4F

#define EIP_OBJECT_LFWD_OPEN_FWD_IND                0x00001A60
#define EIP_OBJECT_LFWD_OPEN_FWD_RES                0x00001A61

#define EIP_OBJECT_CYCLIC_EVENT_IND                 0x00001AE0
#define EIP_OBJECT_WATCHDOG_IND                     0x00001AE2

#define EIP_OBJECT_CIP_OBJECT_CHANGE2_IND           0x00001AEA
#define EIP_OBJECT_CIP_OBJECT_CHANGE2_RES           0x00001AEB

#define EIP_OBJECT_SET_PARAMETER_REQ                0x00001AF2
#define EIP_OBJECT_SET_PARAMETER_CNF                0x00001AF3

#define EIP_OBJECT_CIP_SERVICE_REQ                  0x00001AF8
#define EIP_OBJECT_CIP_SERVICE_CNF                  0x00001AF9

#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND            0x00001AFA
#define EIP_OBJECT_CIP_OBJECT_CHANGE_RES            0x00001AFB

#define EIP_ENCAP_LISTIDENTITY_REQ                 0x00001810   /*!< Request a List Identity                */
#define EIP_ENCAP_LISTIDENTITY_CNF                 0x00001811
#define EIP_ENCAP_LISTIDENTITY_IND                 0x00001812   /*!< Indicate a List Identity Answer        */
#define EIP_ENCAP_LISTIDENTITY_RES                 0x00001813

#define EIP_ENCAP_LISTSERVICES_REQ                 0x00001814   /*!< Request a List Services                */
#define EIP_ENCAP_LISTSERVICES_CNF                 0x00001815
#define EIP_ENCAP_LISTSERVICES_IND                 0x00001816   /*!< Indicate a List Services Answer        */
#define EIP_ENCAP_LISTSERVICES_RES                 0x00001817

#define EIP_ENCAP_LISTINTERFACES_REQ               0x00001818   /*!< Request a List Interface               */
#define EIP_ENCAP_LISTINTERFACES_CNF               0x00001819
#define EIP_ENCAP_LISTINTERFACES_IND               0x0000181A   /*!< Indicate a List Interface Answer       */
#define EIP_ENCAP_LISTINTERFACES_RES               0x0000181B

#define EIP_OBJECT_UNCONNECT_MESSAGE_REQ          0x00001A36  /*!< Send an unconnected message request     */
#define EIP_OBJECT_UNCONNECT_MESSAGE_CNF          0x00001A37

#define EIP_OBJECT_OPEN_CL3_REQ                   0x00001A38  /*!< Open Class 3 Connection                 */
#define EIP_OBJECT_OPEN_CL3_CNF                   0x00001A39

#define EIP_OBJECT_CONNECT_MESSAGE_REQ            0x00001A3A  /*!< Send Class 3 Message Request            */
#define EIP_OBJECT_CONNECT_MESSAGE_CNF            0x00001A3B

#define EIP_OBJECT_CLOSE_CL3_REQ                  0x00001A3C  /*!< Close Class 3 Connection                */
#define EIP_OBJECT_CLOSE_CL3_CNF                  0x00001A3D

#define EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ          0x00001A48  /*!< Activate and deactivate slave from scan list */
#define EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF          0x00001A49

#define EIP_OBJECT_PACKET_END                     0x00001AFF

#endif
