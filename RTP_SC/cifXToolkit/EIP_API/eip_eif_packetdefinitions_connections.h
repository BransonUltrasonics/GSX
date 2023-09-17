/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_connections.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header provides definitions for connection states and connection types.
This information is presented to the host application with the indication
EIP_OBJECT_CONNECTION_IND whenever the state of an EtherNet/IP connection towards the
device changes.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/

#ifndef eip_eif_PACKETDEFINITIONS_CONNECTIONS_H_
#define eip_eif_PACKETDEFINITIONS_CONNECTIONS_H_

/* Connection states and types */

typedef enum EIP_CONNECTION_STATE_Ttag
{
  EIP_UNCONNECT = 0,   /*!< 0: Connection was closed                  */
  EIP_CONNECTED,       /*!< 1: Connection was established             */
  EIP_DIAGNOSTIC       /*!< 2: Diagnostics (EtherNet/IP Scanner only) */
} EIP_CONNECTION_STATE_T;

typedef enum EIP_EXT_CONNECTION_STATE_Ttag
{
  EIP_CONN_STATE_UNDEFINED = 0, /*!< 0: Undefined, not used  */
  EIP_CONN_STATE_TIMEOUT        /*!< 1: Connection timed out */
} EIP_EXT_CONNECTION_STATE_T;


/* Connection type */
typedef enum EIP_CONNECTION_TYPE_Ttag{
  EIP_CONN_TYPE_UNDEFINED = 0,              /*!< 0: Undefined, not used                                 */
  EIP_CONN_TYPE_CLASS_0_1_EXCLUSIVE_OWNER,  /*!< 1: Class0/Class1 (implicit) exclusive owner connection */
  EIP_CONN_TYPE_CLASS_0_1_REDUNDANT_OWNER,  /*!< 2: Class0/Class1 (implicit) redundant owner connection */
  EIP_CONN_TYPE_CLASS_0_1_LISTEN_ONLY,      /*!< 3: Class0/Class1 (implicit) listen only connection     */
  EIP_CONN_TYPE_CLASS_0_1_INPUT_ONLY,       /*!< 4: Class0/Class1 (implicit) input only connection      */
  EIP_CONN_TYPE_CLASS_3                     /*!< 5: Class3        (explicit) connection                 */
} EIP_CONNECTION_TYPE_T;


#endif
