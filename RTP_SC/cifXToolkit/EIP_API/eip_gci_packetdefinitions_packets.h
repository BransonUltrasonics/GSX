/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_gci_packetdefinitions_packets.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header provides all packet definitions specific to the EtherNet/IP stack.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/


#ifndef EIP_GCI_PACKETDEFINITIONS_PACKETS_H_
#define EIP_GCI_PACKETDEFINITIONS_PACKETS_H_

#include <Hil_Packet.h>

#define EIP_OBJECT_MAX_PACKET_LEN               1520  /*!< Maximum packet length */

/* pragma pack */
#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(EIP_DPMINTF_PUBLIC)
#endif

typedef enum EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_Etag
{
  EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_USE_OBJECT_PROVIDED_BY_STACK = 1, /* Activate a stack internal CIP object.
                                                                           This option can currently be used for the following CIP objects

                                                                             - Time Sync object (class code 0x43)
                                                                           */
} EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_E;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_DPMINTF_TI_ACD_LAST_CONFLICT_Ttag
{
  uint8_t    bAcdActivity;      /*!< State of ACD activity when last
                                       conflict detected */

  uint8_t    abRemoteMac[6];    /*!< MAC address of remote node from
                                       the ARP PDU in which a conflict was
                                       detected */

  uint8_t    abArpPdu[28];      /*!< Copy of the raw ARP PDU in which
                                       a conflict was detected. */
} EIP_DPMINTF_TI_ACD_LAST_CONFLICT_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_DPMINTF_TI_MCAST_CONFIG_Ttag
{
  uint8_t    bAllocControl;     /* Multicast address allocation control
                                    word. Determines how addresses are
                                    allocated. */
  uint8_t    bReserved;
  uint16_t   usNumMCast;        /* Number of IP multicast addresses
                                   to allocate for EtherNet/IP */
  uint32_t   ulMcastStartAddr;  /* Starting multicast address from which */
} EIP_DPMINTF_TI_MCAST_CONFIG_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST  EIP_DPMINTF_QOS_CONFIG_Ttag
{
  uint32_t    ulQoSFlags;
  uint8_t     bTag802Enable;        /* QoS Attribute 1 */
  uint8_t     bDSCP_PTP_Event;      /* QoS Attribute 2 */
  uint8_t     bDSCP_PTP_General;    /* QoS Attribute 3 */
  uint8_t     bDSCP_Urgent;         /* QoS Attribute 4 */
  uint8_t     bDSCP_Scheduled;      /* QoS Attribute 5 */
  uint8_t     bDSCP_High;           /* QoS Attribute 6 */
  uint8_t     bDSCP_Low;            /* QoS Attribute 7 */
  uint8_t     bDSCP_Explicit;       /* QoS Attribute 8 */
} EIP_DPMINTF_QOS_CONFIG_T;


/*#####################################################################################*/


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_SET_PARAMETER_REQ_Ttag
{
  uint32_t  ulParameterFlags;    /*!< Parameter flags \n

                                       \description
                                       \ref EIP_APS_PRM_SIGNAL_MS_NS_CHANGE\n
                                       If set, this flag enables the notification of
                                       the network and module status. Every time the status
                                       of the module or network changes packet
                                       EIP_APS_MS_NS_CHANGE_IND will be sent to the
                                       registered EtherNet/IP Application task. */

} EIP_APS_SET_PARAMETER_REQ_T;

#define EIP_APS_SET_PARAMETER_REQ_SIZE (sizeof(EIP_APS_SET_PARAMETER_REQ_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_PARAMETER_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_APS_SET_PARAMETER_REQ_T     tData;
} EIP_APS_PACKET_SET_PARAMETER_REQ_T;

 #define EIP_APS_SET_PARAMETER_CNF_SIZE 0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_PARAMETER_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead;
} EIP_APS_PACKET_SET_PARAMETER_CNF_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_MS_NS_CHANGE_IND_Ttag
{
  uint32_t  ulModuleStatus;     /*!< Module Status \n

                                       \description
                                       The module status describes the current state of the
                                       corresponding MS-LED (provided that it is connected).
                                       See \ref EIP_HW_MODUL_STATUS_E for more details. */

  uint32_t  ulNetworkStatus;     /*!< Network Status \n

                                       \description
                                       The network status describes the current state of the
                                       corresponding NS-LED (provided that it is connected).
                                       See \ref EIP_HW_NET_STATUS_E for more details. */

} EIP_APS_MS_NS_CHANGE_IND_T;

#define EIP_APS_MS_NS_CHANGE_IND_SIZE (sizeof(EIP_APS_MS_NS_CHANGE_IND_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_MS_NS_CHANGE_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_APS_MS_NS_CHANGE_IND_T      tData;
} EIP_APS_PACKET_MS_NS_CHANGE_IND_T;


#define EIP_APS_MS_NS_CHANGE_RES_SIZE 0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_MS_NS_CHANGE_RES_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
} EIP_APS_PACKET_MS_NS_CHANGE_RES_T;

/*#####################################################################################*/


#define EIP_APS_GET_MS_NS_REQ_SIZE       0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_GET_MS_NS_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
} EIP_APS_PACKET_GET_MS_NS_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_GET_MS_NS_CNF_Ttag
{
  uint32_t  ulModuleStatus;     /*!< Module Status \n

                                       \description
                                       The module status describes the current state of the
                                       corresponding MS-LED (provided that it is connected).
                                       See \ref EIP_HW_MODUL_STATUS_E for more details. */

  uint32_t  ulNetworkStatus;     /*!< Network Status \n

                                       \description
                                       The network status describes the current state of the
                                       corresponding NS-LED (provided that it is connected).
                                       See \ref EIP_HW_NET_STATUS_E for more details. */

} EIP_APS_GET_MS_NS_CNF_T;

#define EIP_APS_GET_MS_NS_CNF_SIZE  (sizeof(EIP_APS_GET_MS_NS_CNF_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_GET_MS_NS_CNF_Ttag
{
  HIL_PACKET_HEADER_T      tHead;
  EIP_APS_GET_MS_NS_CNF_T  tData;

} EIP_APS_PACKET_GET_MS_NS_CNF_T;

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_REGISTER_REQ_Ttag
{
  uint32_t ulReserved1;              /*!< Reserved, set to 0x00 */

  uint32_t ulClass;                 /*!< Object class identifier

                                           \valueRange \n\n
                                           Instances of the Assembly Object are divided into the following address ranges to \n
                                           provide for extensions to device profiles \n\n
                                           0x0001 - 0x0063 : Open                            \n
                                           0x0064 - 0x00C7 : Vendor Specific                 \n
                                           0x00C8 - 0x00EF : Reserved by ODVA for future use \n
                                           0x00F0 - 0x02FF : Open                            \n
                                           0x0300 - 0x04FF : Vendor Specific                 \n
                                           0x0500 - 0xFFFF : Reserved by ODVA for future use \n */

  uint32_t ulOptionFlags;           /*!< Option flags

                                           see EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_E
                                           */

} EIP_OBJECT_MR_REGISTER_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_PACKET_REGISTER_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_MR_REGISTER_REQ_T  tData;
} EIP_OBJECT_MR_PACKET_REGISTER_REQ_T;

#define EIP_OBJECT_MR_REGISTER_REQ_SIZE   (sizeof(EIP_OBJECT_MR_REGISTER_REQ_T) )

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_PACKET_REGISTER_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
} EIP_OBJECT_MR_PACKET_REGISTER_CNF_T;

#define EIP_OBJECT_MR_REGISTER_CNF_SIZE   0

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_REGISTER_REQ_Ttag
{
  uint32_t      ulInstance;       /*!< Assembly instance number \n

                                         \valueRange \n\n
                                         Instances of the Assembly Object are divided into the following address ranges to  \n
                                         provide for extensions to device profiles \n\n
                                         0x00000001 - 0x00000063 : Open (static assemblies defined in device profile)       \n
                                         0x00000064 - 0x000000C7 : Vendor Specific static assemblies and dynamic assemblies \n
                                         0x000000C8 - 0x000002FF : Open (static assemblies defined in device profile)       \n
                                         0x00000300 - 0x000004FF : Vendor Specific static assemblies and dynamic assemblies \n
                                         0x00000500 - 0x000FFFFF : Open (static assemblies defined in device profile)       \n
                                         0x00100000 - 0xFFFFFFFF : Reserved by CIP for future use.

                                         \note \n\n
                                         The instance numbers 192 (hex C0) and 193 (hex C1) are usually used as the dummy connection
                                         points for listen only and input only connections. These values <b>must not </b> be used for new
                                         connection points.\n\n
                                         If loadable firmware is used these values are not adaptable. If not, theses values can be changed
                                         within the startup parameters of the object task
                                         (see structure EIP_OBJECT_PARAMETER_T in EipObject_Functionlist.h).
                                         */

  uint32_t      ulDPMOffset;      /*!< Relative offset of the assembly instance data within the DPM input/output area. */
  uint32_t      ulSize;           /*!< Size of the assembly instance data in bytes */
  uint32_t      ulFlags;          /*!< Flags to configure the assembly instance properties \n

                                         \valueRange \n

                                         - Bit  0: <b> \ref EIP_AS_TYPE_INPUT           </b> \n\n
                                         - Bit  3: <b> \ref EIP_AS_OPTION_NO_RUNIDLE    </b> \n\n
                                         - Bit  4: <b> \ref EIP_AS_OPTION_RESERVED      </b> \n\n
                                         - Bit  5: <b> \ref EIP_AS_TYPE_CONFIG          </b> \n\n
                                         - Bit  6: <b> \ref EIP_AS_OPTION_HOLDLASTSTATE </b> \n\n
                                         - Bit  7: <b> \ref EIP_AS_OPTION_FIXEDSIZE     </b> \n\n
                                         - Bit  8: <b> \ref EIP_AS_OPTION_MAP_RUNIDLE   </b> \n\n
                                         - Bit  9: <b> \ref EIP_AS_OPTION_INVISIBLE     </b> \n\n
                                         - Bit 10: <b> \ref EIP_AS_OPTION_MAP_SEQCOUNT  </b> \n\n
                                         - Bit 11: <b> \ref EIP_AS_OPTION_RXTRIGGER     </b> \n\n
                                         - Bit 30: <b> \ref EIP_AS_TYPE_INPUTONLY       </b> \n\n
                                         - Bit 31: <b> \ref EIP_AS_TYPE_LISTENONLY      </b> \n\n */

} EIP_OBJECT_AS_REGISTER_REQ_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_PACKET_REGISTER_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_AS_REGISTER_REQ_T  tData;
} EIP_OBJECT_AS_PACKET_REGISTER_REQ_T;

#define EIP_OBJECT_AS_REGISTER_REQ_SIZE   (sizeof(EIP_OBJECT_AS_REGISTER_REQ_T) )

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_REGISTER_CNF_Ttag
{
  uint32_t  ulInstance;               /*!< Assembly instance number from the request packet              */
  uint32_t  ulDPMOffset;              /*!< DPM offset for the instance data area from the request packet */
  uint32_t  ulSize;                   /*!< Size of the data area from the request packet                 */
  uint32_t  ulFlags;                  /*!< Assembly flags from the request packet                        */
  void*     hDataBuf;                 /*!< Handle of the triple data buffer                              */
} EIP_OBJECT_AS_REGISTER_CNF_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_PACKET_REGISTER_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_AS_REGISTER_CNF_T  tData;
} EIP_OBJECT_AS_PACKET_REGISTER_CNF_T;

#define EIP_OBJECT_AS_REGISTER_CNF_SIZE   (sizeof(EIP_OBJECT_AS_REGISTER_CNF_T) )

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_RESET_IND_Ttag
{
  uint32_t ulDataIdx;                 /*!< Index of the service */
  uint32_t ulResetTyp;                /*!< Type of the reset \n

                                             \valueRange
                                             0, 1

                                             \description
                                             0: Reset is done emulating power cycling of
                                                the device(default)\n
                                             1: Return as closely as possible to the factory
                                                default configuration. Reset is then done
                                                emulating power cycling of the device.<br>
                                                <b>Note</b>:\n Reset type 1 is not supported
                                                by default. It needs to be enabled separately
                                                using the command EIP_OBJECT_SET_PARAMETER_REQ.*/

}EIP_OBJECT_RESET_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_RESET_IND_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_RESET_IND_T  tData;
}EIP_OBJECT_PACKET_RESET_IND_T;

#define EIP_OBJECT_RESET_IND_SIZE          (sizeof(EIP_OBJECT_RESET_IND_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_RESET_RES_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
}EIP_OBJECT_PACKET_RESET_RES_T;

#define EIP_OBJECT_RESET_RES_SIZE          0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_EXT_CONNECTION_INFO_Ttag
{
  uint32_t  ulProConnId;           /*!< Producer Connection ID (T->O)                 */
  uint32_t  ulConConnId;           /*!< Consumer Connection ID (O->T)                 */

  uint32_t  ulConnSerialNum;       /*!< Connection serial number                      */
  uint16_t  usOrigVendorId;        /*!< Originator device vendor ID                   */
  uint32_t  ulOrigDeviceSn;        /*!< Originator device serial number               */

  /* Producer parameters */
  uint32_t  ulProApi;              /*!< Actual packet interval (usecs) (T->O)         */
  uint16_t  usProConnParams;       /*!< Connection parameters (T->O) from ForwardOpen */

  /* Consumer parameters */
  uint32_t  ulConApi;              /*!< Actual packet interval (usecs) (O->T)          */
  uint16_t  usConConnParams;       /*!< Connection parameters (O->T)  from ForwardOpen */

  uint8_t   bTimeoutMultiplier;    /*!< Connection timeout multiplier                  */

}  EIP_OBJECT_EXT_CONNECTION_INFO_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECTION_IND_Ttag
{

  uint32_t ulConnectionState;

  uint16_t usNumExclusiveOwner;
  uint16_t usNumInputOnly;
  uint16_t usNumListenOnly;
  uint16_t usNumExplicitMessaging;

  uint8_t   bConnType;
  uint8_t   abReserved[3];

  uint32_t  ulClass;
  uint32_t  ulInstance;
  uint32_t  ulOTConnectionPoints;
  uint32_t  ulTOConnectionPoints;

  uint16_t  usConnSerialNum;
  uint16_t  usVendorId;
  uint32_t  ulOSerialNum;

  uint8_t   bPriority;
  uint8_t   bTimeOutTicks;
  uint8_t   bTimeoutMultiple;
  uint8_t   bTriggerType;

  uint32_t  ulOTConnID;
  uint32_t  ulTOConnID;

  uint32_t  ulOTRpi;
  uint16_t  usOTConnParam;
  uint16_t  usOTConnSize;
  uint32_t  ulTORpi;
  uint16_t  usTOConnParam;
  uint16_t  usTOConnSize;

  uint32_t  ulProInhib;


  uint32_t ulExtendedState;          /*!< Extended status (see \ref EIP_EXT_CONNECTION_STATE_Ttag)          */

} EIP_OBJECT_CONNECTION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECTION_IND_Ttag
{
  HIL_PACKET_HEADER_T          tHead;
  EIP_OBJECT_CONNECTION_IND_T  tData;
}EIP_OBJECT_PACKET_CONNECTION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECTION_RES_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
}EIP_OBJECT_PACKET_CONNECTION_RES_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CL3_SERVICE_IND_Ttag
{
  uint32_t    ulConnectionId;          /*!< Connection Handle \n

                                           \description \n
                                           The connection handle uniquely identifies
                                           the connection. The same connection handle is
                                           used for all transmissions of a particular
                                           connection. \n\n
                                           <b>Important: \n</b>
                                           This handle must be used for the response to
                                           this indication */

  uint32_t    ulService;               /*!< Service \n

                                           \description \n
                                           This parameter holds the requested CIP service
                                           that related to the next parameter (ulObject).\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulObject;                /*!< Object class \n

                                           \description \n
                                           This parameter holds the CIP object class ID.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulInstance;              /*!< Instance \n

                                           \description \n
                                           This parameter holds the instance number
                                           of the object class specified in ulObject.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulAttribute;             /*!< Attribute \n

                                           \description \n
                                           This parameter holds the attribute number
                                           of the object class instance specified in
                                           ulInstance.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint8_t     abData[1];
} EIP_OBJECT_CL3_SERVICE_IND_T ;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CL3_SERVICE_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_CL3_SERVICE_IND_T    tData;
} EIP_OBJECT_PACKET_CL3_SERVICE_IND_T;



#define EIP_OBJECT_CL3_SERVICE_IND_SIZE (sizeof(EIP_OBJECT_CL3_SERVICE_IND_T)-1)


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST IP_OBJECT_CL3_SERVICE_RES_Ttag
{
  uint32_t    ulConnectionId;          /*!< Connection Handle \n

                                           \description \n
                                           The connection handle from the indication packet. */

  uint32_t    ulService;               /*!< Service \n

                                           \description \n
                                           The service code from the indication packet. */

  uint32_t    ulObject;                /*!< Object class \n

                                           \description \n
                                           The object class ID from the indication packet */

  uint32_t    ulInstance;              /*!< Instance \n

                                           \description \n
                                           The instance number from the indication packet */

  uint32_t    ulAttribute;             /*!< Attribute \n

                                           \description \n
                                           The attribute number from the indication packet */

  uint32_t    ulGRC;                 /*!< Generic Error Code \n

                                           \description \n
                                            This generic error code can be used to
                                            indicate whether the service request was successful
                                            or unsuccessful. */

  uint32_t    ulERC;                 /*!< Extended Error Code \n

                                           \description \n
                                           This extended error code can be used to
                                           describe the occurred error (GRC) in more detail. */

  uint8_t     abData[1];             /*!< Response service  data */

} EIP_OBJECT_CL3_SERVICE_RES_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CL3_SERVICE_RES_Ttag
{
  HIL_PACKET_HEADER_T            tHead;
  EIP_OBJECT_CL3_SERVICE_RES_T   tData;
} EIP_OBJECT_PACKET_CL3_SERVICE_RES_T;



#define EIP_OBJECT_CL3_SERVICE_RES_SIZE (sizeof(EIP_OBJECT_CL3_SERVICE_RES_T)-1)

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_REGISTER_SERVICE_REQ_Ttag
{
  uint32_t ulService;                /*!< Service code \n

                                      \valueRange \n\n
                                       Instances of the Assembly Object are divided into the following address ranges to \n
                                       provide for extensions to device profiles \n\n

                                       0x00 - 0x31 : Open. These are referred to as Common Services.
                                                     These are defined in the EtherNet/IP spec. Vol.1 Appendix A.\n
                                       0x32 - 0x4A : Vendor Specific                        \n
                                       0x4B - 0x63 : Object Class Specific                  \n
                                       0x64 - 0x7F : Reserved by ODVA for future use        \n
                                       0x80 - 0xFF : Reserved for use as Reply Service Code \n

                                       \description \n\n
                                       Pre-defined service codes (hex): \n\n
                                       00:    Reserved \n
                                       01:    Get_Attributes_All \n
                                       02:    Set_Attributes_All \n
                                       03:    Get_Attribute_List \n
                                       04:    Set_Attribute_List \n
                                       05:    Reset \n
                                       06:    Start \n
                                       07:    Stop \n
                                       08:    Create \n
                                       09:    Delete \n
                                       0A:    Multiple_Service_Packet \n
                                       0B:    Reserved for future use \n
                                       0D:    Apply_Attributes \n
                                       0E:    Get_Attribute_Single \n
                                       0F:    Reserved for future use \n
                                       10:   Set_Attribute_Single \n
                                       11:    Find_Next_Object_Instance \n
                                       12-13: Reserved for future use \n
                                       14:    Error Response (used by DevNet only) \n
                                       15:    Restore \n
                                       16:    Save \n
                                       17:    No Operation (NOP) \n
                                       18:    Get_Member \n
                                       19:    Set_Member \n
                                       1A:    Insert_Member \n
                                       1B:    Remove_Member \n
                                       1C:    GroupSync \n
                                       1D-31: Reserved for additional Common Services \n */


} EIP_OBJECT_REGISTER_SERVICE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_REGISTER_SERVICE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
  EIP_OBJECT_REGISTER_SERVICE_REQ_T  tData;
} EIP_OBJECT_PACKET_REGISTER_SERVICE_REQ_T;

#define EIP_OBJECT_REGISTER_SERVICE_REQ_SIZE (sizeof(EIP_OBJECT_REGISTER_SERVICE_REQ_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_REGISTER_SERVICE_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
} EIP_OBJECT_PACKET_REGISTER_SERVICE_CNF_T;

#define EIP_OBJECT_REGISTER_SERVICE_CNF_SIZE 0

/*#####################################################################################*/

/* Set Parameter Request */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_SET_PARAMETER_REQ_Ttag
{
  uint32_t ulParameterFlags;    /*!< Parameter flags \n

                                       \description
                                       \ref EIP_OBJECT_PRM_FWRD_OPEN_CLOSE_FORWARDING \n
                                       Enable forwarding of Forward_Open/Forward_Close frames
                                       to the EtherNet/IP Application.
                                       \ref EIP_OBJECT_PRM_DISABLE_FLASH_LEDS_SERVICE
                                       Disable/Unsupport FLash_LEDs service of Identity object.
                                   */

} EIP_OBJECT_SET_PARAMETER_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_PARAMETER_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_SET_PARAMETER_REQ_T  tData;
}EIP_OBJECT_PACKET_SET_PARAMETER_REQ_T;

#define EIP_OBJECT_SET_PARAMETER_REQ_SIZE  (sizeof(EIP_OBJECT_SET_PARAMETER_REQ_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_PARAMETER_CNF_Ttag
{
  HIL_PACKET_HEADER_T          tHead;
}EIP_OBJECT_PACKET_SET_PARAMETER_CNF_T;

#define EIP_OBJECT_SET_PARAMETER_CNF_SIZE  0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_SERVICE_REQ_Ttag
{
  uint32_t    ulService;                          /*!< CIP service code          */
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */
  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< CIP Service Data. <br><br>
                                                         Number of bytes provided in this field
                                                         must be add to the packet header length field
                                                         ulLen.
                                                         Do the following to set the proper packet length:<br>
                                                         ptReq->tHead.ulLen = EIP_OBJECT_CIP_SERVICE_REQ_SIZE + number of bytes provided in abData */
} EIP_OBJECT_CIP_SERVICE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_SERVICE_REQ_Ttag
{
  HIL_PACKET_HEADER_T            tHead;
  EIP_OBJECT_CIP_SERVICE_REQ_T   tData;
}EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T;

#define EIP_OBJECT_CIP_SERVICE_REQ_SIZE     (sizeof(EIP_OBJECT_CIP_SERVICE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_SERVICE_CNF_Ttag
{
  uint32_t    ulService;                          /*!< CIP service code          */
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */

  uint32_t    ulGRC;                              /*!< Generic Error Code        */
  uint32_t    ulERC;                              /*!< Extended Error Code       */

  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< CIP service data. <br><br>
                                                         Number of bytes provided in this field
                                                         is encoded in the packet header length field
                                                         ulLen. <br><br>
                                                         Do the following to get the data size:<br>
                                                         ulSize = ptCnf->tHead.ulLen - EIP_OBJECT_CIP_SERVICE_CNF_SIZE */

} EIP_OBJECT_CIP_SERVICE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_SERVICE_CNF_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_CIP_SERVICE_CNF_T    tData;
} EIP_OBJECT_PACKET_CIP_SERVICE_CNF_T;

#define EIP_OBJECT_CIP_SERVICE_CNF_SIZE     (sizeof(EIP_OBJECT_CIP_SERVICE_CNF_T)) - EIP_OBJECT_MAX_PACKET_LEN

/*#####################################################################################*/

/**
   Packets defined for forwarding of forward close and forward open
   from Stack to host application.
   Packets are defined on the assumption/rule that the host application uses the received
   packet for generating the response.
   That's why the indication reserves some space that are used for response parameters
*/

#define EIP_DEFAULT_PATH_LEN              1000

/* Large Forward Open Service - Request Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_LARGEFWOPEN_REQ_Ttag
{
  uint8_t   bPriority;          /* used to calculate request timeout information    */
  uint8_t   bTimeOutTicks;      /* used to calculate request timeout information    */
  uint32_t  ulOTConnID;         /* Network connection ID originator to target       */
  uint32_t  ulTOConnID;         /* Network connection ID target to originator       */
  uint16_t  usConnSerialNum;    /* Connection serial number                         */
  uint16_t  usVendorId;         /* Originator Vendor ID                             */
  uint32_t  ulOSerialNum;       /* Originator serial number                         */
  uint8_t   bTimeoutMultiple;   /* Connection timeout multiple                      */
  uint8_t   abReserved1[3];     /* reserved                                         */
  uint32_t  ulOTRpi;            /* Originator to target requested packet rate in us */
  uint32_t  ulOTConnParam;      /* Originator to target connection parameter        */
  uint32_t  ulTORpi;            /* target to originator requested packet rate in us */
  uint32_t  ulTOConnParam;      /* target to originator connection parameter        */
  uint8_t   bTriggerType;       /* Transport type/trigger                           */
  uint8_t   bConnPathSize;      /* Connection path size                             */
  uint8_t   bConnPath[EIP_DEFAULT_PATH_LEN];        /* connection path              */
} EIP_CM_LARGEFWOPEN_REQ_T;

/* Deliver Forward Open to host application */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_LFWD_OPEN_FWD_IND_Ttag
{
  void*                    pRouteMsg;              /**< Link to to remember underlying Encapsulation request (must not be modified by app)   */
  uint32_t                 aulReserved[4];         /**< Place holder to be filled by response parameters, see  EIP_OBJECT_LFWD_OPEN_FWD_RES_T */
  EIP_CM_LARGEFWOPEN_REQ_T tFwdOpenData;           /**< Forward Open request data to be delivered to host                                    */
} EIP_OBJECT_LFWD_OPEN_FWD_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_LFWD_OPEN_FWD_IND_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_LFWD_OPEN_FWD_IND_T       tData;
} EIP_OBJECT_PACKET_LFWD_OPEN_FWD_IND_T;

#define EIP_OBJECT_LFWD_OPEN_FWD_IND_SIZE  (sizeof(EIP_OBJECT_LFWD_OPEN_FWD_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)


/**
   Deliver Forward Open to host application - Response
   Contains the potentially modified forward open data (since host application may need to modify connection points e.g. for safety)
   Additional parameters are: Status (host application result) and a reference (size/offset) to the application reply data
   that needs to be appended to the Forward open response generated by the stack
*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_LFWD_OPEN_FWD_RES_Ttag
{
  void*                    pRouteMsg;            /**< Link to underlying Encapsulation request           */
  uint32_t                 ulGRC;                /**< General Error Code                                 */
  uint32_t                 ulERC;                /**< Extended Error Code                                */
  uint32_t                 ulAppReplyOffset;     /**< Offset of Application Reply data                   */
  uint32_t                 ulAppReplySize;       /**< Byte-size of Application Reply data                */
  EIP_CM_LARGEFWOPEN_REQ_T tFwdOpenData;         /**< modified forward open (Note that the application
                                                      reply data is appended, which is not visible here) */
} EIP_OBJECT_LFWD_OPEN_FWD_RES_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_LFWD_OPEN_FWD_RES_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  EIP_OBJECT_LFWD_OPEN_FWD_RES_T            tData;
} EIP_OBJECT_PACKET_LFWD_OPEN_FWD_RES_T;

#define EIP_OBJECT_LFWD_OPEN_FWD_RES_SIZE (sizeof(EIP_OBJECT_LFWD_OPEN_FWD_RES_T) - EIP_DEFAULT_PATH_LEN)

/*#####################################################################################*/

/** Status indication of Forward Open, that was previously processed by the
 * host (see EIP_OBJECT_LFWD_OPEN_FWD_IND)*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_Ttag
{
  uint16_t  usCmInstance;       /**< Connection manager instance\n

                                       \description
                                       The connection is administrated in
                                       the connection manager instance
                                       usCmInstance.

                                       \valueRange
                                       0: Only 0 if connection could not be established\n
                                       1-64: Valid instances              */

  uint16_t  usConnSerialNum;    /**< Connection serial number           */
  uint16_t  usVendorId;         /**< Originator Vendor ID               */
  uint32_t  ulOSerialNum;       /**< Originator serial number           */
  uint32_t  ulGRC;              /**< General Error Code                 */
  uint32_t  ulERC;              /**< Extended Error Code                */
} EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T  tData;
} EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_IND_T;

#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_SIZE   (sizeof(EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_RES_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
} EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_RES_T;

#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_RES_SIZE   0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_APP_FWCLOSE_IND_Ttag
{
  uint8_t   bPriority;                             /* Used to calculate request timeout information  */
  uint8_t   bTimeOutTicks;                         /* Used to calculate request timeout information  */
  uint16_t  usConnSerialNum;                       /* Connection serial number                       */
  uint16_t  usVendorId;                            /* Originator Vendor ID                           */
  uint32_t  ulOSerialNum;                          /* Originator serial number                       */
  uint8_t   bConnPathSize;                         /* Connection path size in 16bit words            */
  uint8_t   bReserved1;
  uint8_t   bConnPath[EIP_OBJECT_MAX_PACKET_LEN];  /* connection path                                */
} EIP_CM_APP_FWCLOSE_IND_T;


/* Deliver Forward Close to host application - indication */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_CLOSE_FWD_IND_Ttag
{
  void*                     pRouteMsg;              /*!< Link to underlying Encapsulation request           */
  uint32_t                aulReserved[2];         /*!< Placeholder to be filled by response parameters,
                                                         see  EIP_OBJECT_FWD_CLOSE_FWD_RES_T                */
  EIP_CM_APP_FWCLOSE_IND_T  tFwdCloseData;          /*!< Forward close request data to be delivered to host */
} EIP_OBJECT_FWD_CLOSE_FWD_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_CLOSE_FWD_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_FWD_CLOSE_FWD_IND_T  tData;
} EIP_OBJECT_PACKET_FWD_CLOSE_FWD_IND_T;

#define EIP_OBJECT_FWD_CLOSE_FWD_IND_SIZE   (sizeof(EIP_OBJECT_FWD_CLOSE_FWD_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)


/*
  Deliver Forward Close to host application - response
  Contains the modified forward close (since application may need to modify connection points e.g. for safety)
  Additional parameters are: Status (host application result)
*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_CLOSE_FWD_RES_Ttag
{
  void*                     pRouteMsg;             /*!< Link to underlying Encapsulation request */
  uint32_t                ulGRC;                 /*!< General Error Code                       */
  uint32_t                ulERC;                 /*!< Extended Error Code                      */
  EIP_CM_APP_FWCLOSE_IND_T  tFwdCloseData;         /*!< Modified forward close                   */
} EIP_OBJECT_FWD_CLOSE_FWD_RES_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_CLOSE_FWD_RES_Ttag
{
  HIL_PACKET_HEADER_T              tHead;
  EIP_OBJECT_FWD_CLOSE_FWD_RES_T   tData;
} EIP_OBJECT_PACKET_FWD_CLOSE_FWD_RES_T;

#define EIP_OBJECT_FWD_CLOSE_FWD_RES_SIZE   (sizeof(EIP_OBJECT_FWD_CLOSE_FWD_RES_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_PROPOSE  (0x10)    /*!< The attribute change is pending
                                                                  and the host is given the chance to decide */
#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_INFORM    (0x20)    /*!< The attribute change already took place
                                                                  and the indication is simply informed about it */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_OBJECT_CHANGE_IND_Ttag
{
  uint32_t    ulInfoFlags;                        /*!< Information flags     */
  uint32_t    ulService;                          /*!< CIP service code      */
  uint32_t    ulClass;                            /*!< CIP class ID          */
  uint32_t    ulInstance;                         /*!< CIP instance number   */
  uint32_t    ulAttribute;                        /*!< CIP attribute number  */
  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< Service Data          */

} EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T   tData;
} EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T;

#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_SIZE     (sizeof(EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T   tData;
} EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_T;

#define EIP_OBJECT_CIP_OBJECT_CHANGE_RES_SIZE    (sizeof(EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_CONFIG_DONE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
} EIP_APS_PACKET_CONFIG_DONE_REQ_T;

#define EIP_APS_CONFIG_DONE_REQ_SIZE 0

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_CONFIG_DONE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
} EIP_APS_PACKET_CONFIG_DONE_CNF_T;

#define EIP_APS_CONFIG_DONE_CNF_SIZE 0
/*! \}*/
/*#####################################################################################*/

#define EIP_APS_CONFIGURATION_PARAMETER_SET_V3 3

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_CONFIGURATION_PARAMETER_SET_V3_Ttag
{

  uint32_t  ulSystemFlags;    /*!< Reserved for IO status */
  uint32_t  ulWdgTime;        /*!< Watch dog time (in milliseconds) <br>

                                     \valueRange
                                     0, 20...65535\n
                                     Default: 1000

                                     \description
                                     Value 0 switches off the watch dog */

  uint32_t  ulInputLen;       /*!< Length of input data (Input Assembly)\n

                                     \valueRange
                                     0...504 \n
                                     Default: 16

                                     \description
                                     Defines the input data size and corresponds directly
                                     to the input assembly instance provided by ulInputAssInstance */
  uint32_t  ulOutputLen;      /*!< Length of Output data (Output Assembly)\n

                                     \valueRange
                                     0...504 \n
                                     Default: 16

                                     \description
                                     Defines the output data size and corresponds directly
                                     to the input assembly instance provided by ulOutputAssInstance */

  uint32_t  ulTcpFlag;        /*!< TCP configuration flags\n

                                     \valueRange
                                     Default: 0x00000410 (DHCP and Auto-Negotiation)

                                     \description
                                     For further information see TCPIP-Manual */
  uint32_t  ulIpAddr;         /*!< IP address \n
                                     \valueRange
                                     All valid IP addresses\n
                                     Default: 0.0.0.0 */
  uint32_t  ulNetMask;        /*!< Network mask \n

                                     \valueRange
                                     All valid masks\n
                                     Default: 0.0.0.0 */
  uint32_t  ulGateway;        /*!< Gateway address \n

                                     \valueRange
                                     All valid IP addresses\n
                                     Default: 0.0.0.0 */

  uint16_t  usVendId;         /*!< CIP vendor identification \n

                                     \valueRange
                                     1...65535 \n
                                     Default: 283 (Hilscher GmbH)

                                     \description
                                     This is an identification number for
                                     the manufacturer of an EtherNet/IP device.
                                     Vendor IDs are managed by ODVA.
                                     The value zero is not valid */
  uint16_t  usProductType;    /*!< CIP device type \n

                                     \valueRange
                                     Publicly defined: 0x00 - 0x64    \n
                                     Vendor specific: 0x64 - 0xC7     \n
                                     Reserved by CIP: 0xC8 - 0xFF     \n
                                     Publicly defined: 0x100 - 0x2FF  \n
                                     Vendor specific: 0x300 - 0x4FF   \n
                                     Reserved by CIP: 0x500 - 0xFFFF  \n\n
                                     Default: 0x0C (Communication Device)

                                     \description
                                     The list of device types is managed by ODVA.
                                     It is used to identify the device profile that a
                                     particular product is using. Device profiles
                                     define minimum requirements a device must
                                     implement as well as common options. */
  uint16_t  usProductCode;    /*!< Product code \n

                                     \valueRange
                                     1...65535

                                     \description
                                     The vendor assigned Product Code identifies a particular product within a device type. Each
                                     vendor assigns this code to each of its products. The Product Code typically maps to one or
                                     more catalog/model numbers. Products shall have different codes if their configuration and/or
                                     runtime options are different. Such devices present a different logical view to the network. On
                                     the other hand for example, two products that are the same except for their color or mounting
                                     feet are the same logically and may share the same product code.
                                     The value zero is not valid. */

  uint32_t  ulSerialNumber;   /*!< Serial number \n

                                     \valueRange
                                     0x00000000... 0xFFFFFFFF

                                     \description
                                     This attribute is a number used in conjunction with the Vendor ID to form a unique identifier
                                     for each device on any CIP network. Each vendor is responsible for guaranteeing the
                                     uniqueness of the serial number across all of its devices. \n
                                     Usually, this number will be set automatically by the firmware,
                                     if a security memory is available. */

  uint8_t   bMinorRev;        /*!< Minor revision \n

                                     \valueRange
                                     1...255 */

  uint8_t   bMajorRev;        /*!< Major revision \n

                                     \valueRange
                                     1...127 */

  uint8_t   abDeviceName[32]; /*!< Device name\n

                                     \description
                                     This text string should represent a short description of the product/product family represented
                                     by the product code. The same product code may have a variety of product name
                                     strings.\n\n
                                     The first byte indicates the name length, byte
                                     2-31 contain the actual characters of the device name.*/

  uint32_t  ulInputAssInstance;   /*!< Instance number of input assembly\n

                                     \valueRange \n\n
                                     Instances of the Assembly Object are divided into the following address ranges to \n
                                     provide for extensions to device profiles \n\n
                                     0x00000001 - 0x00000063 : Open (static assemblies defined in device profile)       \n
                                     0x00000064 - 0x000000C7 : Vendor Specific static assemblies and dynamic assemblies \n
                                     0x000000C8 - 0x000002FF : Open (static assemblies defined in device profile)       \n
                                     0x00000300 - 0x000004FF : Vendor Specific static assemblies and dynamic assemblies \n
                                     0x00000500 - 0x000FFFFF : Open (static assemblies defined in device profile)      \n
                                     0x00100000 - 0xFFFFFFFF : Reserved by CIP for future use.

                                     \description
                                     This parameter defines the instance number of the input assembly.

                                     */

  uint32_t  ulInputAssFlags;      /*!< Input assembly flags\n

                                      \valueRange \n

                                       - <b> Bit 0:</b> \n
                                         This flag is used internally and must be set to 0 \n\n

                                       - <b> Bit 1:</b> \n
                                         This flag is used internally and must be set to 0 \n\n

                                       - <b> Bit 2:</b> \n
                                         This flag is used internally and must be set to 0 \n\n

                                       - <b> Bit 3:</b> \n
                                         If set, the assembly data is modeless
                                         (i.e. it does <b>not</b> contain run/idle information) \n\n

                                       - <b> Bit 4:</b> \n
                                         This flag is used internally and must be set to 0 \n\n

                                       - <b> Bit 5:</b> \n
                                         This flag is used internally and must be set to 0 \n\n

                                       - <b> Bit 6:</b> \n
                                         This flag decides whether the data that is mapped into the
                                         DPM memory area is cleared upon closing of
                                         the connection or whether the last sent/received data is left
                                         unchanged in the memory.
                                         If the bit is set the data will be left unchanged.\n\n
                                     */


  uint32_t  ulOutputAssInstance;  /*!< Instance number of output assembly\n

                                     \valueRange \n\n
                                     Instances of the Assembly Object are divided into the following address ranges to \n
                                     provide for extensions to device profiles \n\n
                                     0x00000001 - 0x00000063 : Open (static assemblies defined in device profile)       \n
                                     0x00000064 - 0x000000C7 : Vendor Specific static assemblies and dynamic assemblies \n
                                     0x000000C8 - 0x000002FF : Open (static assemblies defined in device profile)       \n
                                     0x00000300 - 0x000004FF : Vendor Specific static assemblies and dynamic assemblies \n
                                     0x00000500 - 0x000FFFFF : Open (static assemblies defined in device profile)      \n
                                     0x00100000 - 0xFFFFFFFF : Reserved by CIP for future use.

                                     \description
                                     This parameter defines the instance number of the output assembly.

                                     */

  uint32_t   ulOutputAssFlags;     /*!< Output assembly flags\n

                                      \valueRange \n
                                      See description of ulInputAssFlags
                                     */

  EIP_DPMINTF_QOS_CONFIG_T tQoS_Config; /*!< Quality of Service \n

                                          \description

                                           This parameter set configures the Quality of Service Object
                                           (CIP Id 0x48).
                                           For more information about specific parameters see command
                                           EIP_OBJECT_CFG_QOS_REQ.

                                         */

  uint32_t   ulNameServer;         /*!< Name Server 1\n

                                       \description
                                        This parameter corresponds to an entry of attribute 5 of the
                                        TCP Interface Object (CIP Id 0xF5).

                                        \valueRange \n
                                        Default value: 0.0.0.0
                                     */

  uint32_t   ulNameServer_2;       /*!< Name Server 2\n

                                       \description
                                        This parameter corresponds to an entry of attribute 5 of the
                                        TCP Interface Object (CIP Id 0xF5).

                                        \valueRange \n
                                        Default value: 0.0.0.0
                                      */

  uint8_t    abDomainName[48 + 2]; /*!< Domain Name\n

                                       \description
                                        This parameter corresponds to an entry of attribute 5 of the
                                        TCP Interface Object (CIP Id 0xF5).
                                        The first two bytes of the name provide the length of the name.

                                        \valueRange \n
                                        Default value: No Name ("") \n
                                        Max. name length is 48 bytes

                                      */

  uint8_t    abHostName[64+2];     /*!< Host Name\n

                                       \description
                                        This parameter corresponds to attribute 6 of the
                                        TCP Interface Object (CIP Id 0xF5).
                                        The first two bytes of the name provide the length of the name.

                                        \valueRange \n
                                        Default value: No Name ("") \n
                                        Max. name length is 64 bytes
                                      */

  uint8_t    bSelectAcd;           /*!< Select ACD

                                         \description
                                        This parameter corresponds to attribute 10 of the
                                        TCP Interface Object (CIP Id 0xF5).

                                        \valueRange \n
                                        1: ACD on (default) \n
                                        0: ACD off

                                      */

  EIP_DPMINTF_TI_ACD_LAST_CONFLICT_T  tLastConflictDetected; /*!< Last detected conflict\n

                                                               \description
                                                               This parameter corresponds to attribute 11 of the
                                                               TCP Interface Object (CIP Id 0xF5).

                                                               \valueRange \n
                                                               Default: All zero
                                                             */

  uint8_t                           bQuickConnectFlags; /*!< Quick Connect flags \n

                                                               \description
                                                               This parameter enables/disables the Quick Connect
                                                               functionality within the stack.
                                                               This affects the TCP Interface Object (0xF5)
                                                               attribute 12. \n\n
                                                               \Note: This functionality needs special hardware settings (e.g. fast
                                                               flash)

                                                               \valueRange \n
                                                               See: \ref EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE
                                                               and  \ref EIP_OBJECT_QC_FLAGS_ENABLE_QC

                                                               <b>Examples:</b>\n\n
                                                               Do not support Quick Connect:\n
                                                               Set to zero \n\n

                                                               Support Quick Connect - Quick Connect disabled:\n
                                                               Set only flag \ref EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE
                                                               in order to activate the Quick Connect attribute within the object.\n\n

                                                               Support Quick Connect - Quick Connect enabled:\n
                                                               Set flag \ref EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE
                                                               and flag \ref EIP_OBJECT_QC_FLAGS_ENABLE_QC in order to
                                                               activate the attribute and simultaneously enable the
                                                               Quick Connection functionality.\n\n
                                                             */


  /* New parameters for packet V2 start here **************/

  uint8_t                           abAdminState[2];       /*!< Admin State \n

                                                               \description
                                                               This parameter corresponds to attribute 9 of the
                                                               Ethernet Link Object (CIP Id 0xFF).
                                                               The Admin State attribute shall allow administrative
                                                               setting of the interface state. The interface (PHYs) can be
                                                               enabled or disabled.
                                                               Each array entry stands for one ethernet port.
                                                               This attribute shall be stored in non-volatile memory.

                                                               \valueRange \n
                                                               0x01: Interface enabled
                                                               0x02: Interface disabled
                                                               Default: Both entries 0x01 (enabled)
                                                             */

  /* New parameters for packet V3 start here **************/

  uint8_t                           bTTLValue;             /*!< TTL Value \n

                                                               \description
                                                               This parameter corresponds to attribute 8 of the
                                                               TCP/IP Interface Object (CIP Id 0xF5).
                                                               The TTL value attribute shall use for the IP header Time-to-Live
                                                               when sending EtherNet/IP packets via multicast.
                                                               This attribute shall be stored in non-volatile memory.

                                                               \valueRange \n
                                                               1 - 255
                                                               Default: 1
                                                             */
  EIP_DPMINTF_TI_MCAST_CONFIG_T       tMCastConfig;          /*!< Multicast Configuration \n

                                                               \description
                                                               This parameter corresponds to attribute 9 of the
                                                               TCP/IP Interface Object (CIP Id 0xF5).
                                                               The MCast Config set the used multicast range for multicast connections.
                                                               This attribute shall be stored in non-volatile memory.

                                                               \valueRange \n
                                                               0 - disabled
                                                               1 - 3600 seconds
                                                               Default: 120 seconds
                                                             */

  uint16_t                          usEncapInactivityTimer;/*!< Encapsulation Inactivity Timeout \n

                                                               \description
                                                               This parameter corresponds to attribute 13 of the
                                                               TCP/IP Interface Object (CIP Id 0xF5).
                                                               The Encapsulation Inavtivity Timeout is used to close socketes when the
                                                               defined time (seconds) eclapsed without Encapsulation activity.
                                                               This attribute shall be stored in non-volatile memory.

                                                               \valueRange \n
                                                               0 - disabled
                                                               1 - 3600 seconds
                                                               Default: 120 seconds
                                                             */

} EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T;

#define EIP_APS_CONFIGURATION_PARAMETER_SET_V3_SIZE (sizeof(EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T) )

/*#####################################################################################*/

/* Request Packet */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_Ttag
{
  uint32_t  ulParameterVersion;  /*!< Version related to the following configuration union,
                                      only EIP_APS_CONFIGURATION_PARAMETER_SET_V3 is supported.
                                 */

  __HIL_PACKED_PRE union __HIL_PACKED_POST
  {
#if 0
    /* Parameter sets V1 and V2 are no longer supported with
     * EtherNet/IP Adapter v3.4.1.0 and newer versions
     */
    EIP_APS_CONFIGURATION_PARAMETER_SET_V1_T tV1;
    EIP_APS_CONFIGURATION_PARAMETER_SET_V2_T tV2;
#endif
    EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T tV3;
  } unConfig;

} EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;
  EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_T tData;

} EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_T;

#define EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_SIZE   4 /* Basic size of request packet.
                                                             The size of the following parameter union needs to be added. */

/* Confirmation Packet */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;
} EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_CNF_T;

#define EIP_APS_SET_CONFIGURATION_PARAMETERS_CNF_SIZE   0

/*#####################################################################################*/

/*#####################################################################################*/

#define PID_EIP_IP_CONFIGURATION  0x3000A001 /* EtherNet/IP Address Settings      */
#define PID_EIP_IP_CONFIGCONTROL  0x3000A002 /* EtherNet/IP Configuration Control */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PID_EIP_IP_CONFIGURATION_Ttag
{
  uint32_t ulIP;
  uint32_t ulNetmask;
  uint32_t ulGateway;
} PID_EIP_IP_CONFIGURATION_T;

#define PRM_CFGCTRL_STORED_CFG  0
#define PRM_CFGCTRL_DHCP        1
#define PRM_CFGCTRL_BOOTP       2
#define PRM_CFGCTRL_FIXIP       3

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PID_EIP_IP_CONFIGCONTROL_Ttag
{
  uint32_t ulConfigurationControl;
} PID_EIP_IP_CONFIGCONTROL_T;

/*#####################################################################################*/
/* Interface to Encapsulation Layer                                                    */
/*#####################################################################################*/
#define EIP_ENCAP_DATA_PCKT_LEN 1520

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTIDENTITY_REQ_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTIDENTITY_REQ_T tData;
} EIP_ENCAP_PACKET_LISTIDENTITY_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTIDENTITY_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_IND_Ttag
{
  uint16_t usItemCount;          /* CPF Format: Number of items in message */
  uint16_t usIdentityItemId;     /* CPF Format: The first item is always the identity
                                    item, code 0x0C */
  uint16_t usIdentityItemLength; /* CPF Format: Length in bytes (varies depending
                                    on product name string length */
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usEncapProtocolVersion;
    __HIL_PACKED_PRE struct __HIL_PACKED_POST
    {
      int16_t  sin_family;
      uint16_t sin_port;
      uint32_t sin_addr;
      uint8_t  sin_zero[8];
    } tSocketAddress;

    uint16_t usVendorId;
    uint16_t usDeviceType;
    uint16_t usProductCode;
    uint8_t  abRevision[2];
    uint16_t usStatus;
    uint32_t ulSerialNumber;
    uint8_t  abProductName[1];                 /* CIP short string: one byte
                                                  length specifier */
  } tIdentityItem;
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN];  /* Remaining (variable size) data
                                                 (actual product name, state field,
                                                  and possibly further CPF items) */
} EIP_ENCAP_LISTIDENTITY_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTIDENTITY_IND_T tData;
} EIP_ENCAP_PACKET_LISTIDENTITY_IND_T;

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTSERVICES_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTSERVICES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTSERVICES_REQ_T tData;
} EIP_ENCAP_PACKET_LISTSERVICES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTSERVICES_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTSERVICES_IND_Ttag
{
  uint16_t usItemCount;          /* CPF Format: Number of items in message */
  uint16_t usServicesItemId;     /* CPF Format: The first item is the service
                                    item, code 0x0100 */
  uint16_t usServicesItemLength; /* CPF Format: Length in bytes (varies depending
                                    on product name string length */
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usEncapProtocolVersion; /* shall be set to 1 */
    uint16_t usCapabilityFlags;      /* expect bit 5 (EIP encap), bit 8 (class 0/1) */
    uint8_t  abNameOfService[16];    /* expect NUL-term. ASCII: "Communications" */
  } tServicesItem;
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN];  /* Remaining (variable size) data
                                                 (for, possibly, further CPF items) */
} EIP_ENCAP_LISTSERVICES_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTSERVICES_IND_T tData;
} EIP_ENCAP_PACKET_LISTSERVICES_IND_T;

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTINTERFACES_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTINTERFACES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTINTERFACES_REQ_T tData;
} EIP_ENCAP_PACKET_LISTINTERFACES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTINTERFACES_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTINTERFACES_IND_Ttag
{
  uint16_t usItemCount;                     /* CPF Format: Number of items in message      */
  uint8_t  abData[EIP_ENCAP_DATA_PCKT_LEN]; /* As of Jan/2019, no public items are defined */
} EIP_ENCAP_LISTINTERFACES_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTINTERFACES_IND_T tData;
} EIP_ENCAP_PACKET_LISTINTERFACES_IND_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_UNCONNECT_MESSAGE_REQ_Ttag
{
  uint32_t ulIPAddr;                  /*!< Destination IP address */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  bReserved;                 /*!< Reserved padding       */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */

  uint8_t  abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< Service Data   */
} EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_REQ_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T  tData;
} EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_UNCONNECT_MESSAGE_CNF_Ttag
{
  uint32_t ulIPAddr;                  /*!< Destination IP address */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  bReserved;                 /*!< Reserved padding       */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */

  struct
  {
    uint8_t bGrc;                              /*!< CIP General status information */
    uint8_t bExtStatusSize;                    /*!< Number of 16 bit words of CIP Extended
                                                    status information */
    uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< CIP Extended Status information, if present,
                                                    followed by the actual response data */
  } tResponse;
} EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T  tData;
}EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_CNF_T;


#define EIP_OBJECT_UNCONNECT_MESSAGE_REQ_SIZE (sizeof(EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)
#define EIP_OBJECT_UNCONNECT_MESSAGE_CNF_SIZE (sizeof(EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T) - EIP_OBJECT_MAX_PACKET_LEN)


/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_OPEN_CL3_REQ_Ttag
{
  uint32_t ulIPAddr;                  /*!< Destination IP address */
  uint32_t ulTime;                    /*!< Expected Message Time  */
  uint32_t ulTimeoutMult;             /*!< Timeout Multiplier     */
} EIP_OBJECT_OPEN_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_OPEN_CL3_REQ_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_OPEN_CL3_REQ_T   tData;
} EIP_OBJECT_PACKET_OPEN_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_OPEN_CL3_CNF_Ttag
{
  uint32_t    ulConnection;          /*!< Connection Handle    */
  uint32_t    ulGRC;                 /*!< Generic Error Code   */
  uint32_t    ulERC;                 /*!< Extended Error Code  */
} EIP_OBJECT_OPEN_CL3_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_OPEN_CL3_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_OPEN_CL3_CNF_T   tData;
} EIP_OBJECT_PACKET_OPEN_CL3_CNF_T;

#define EIP_OBJECT_OPEN_CL3_REQ_SIZE (sizeof(EIP_OBJECT_OPEN_CL3_REQ_T))
#define EIP_OBJECT_OPEN_CL3_CNF_SIZE (sizeof(EIP_OBJECT_OPEN_CL3_CNF_T))

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECT_MESSAGE_REQ_Ttag
{
  uint32_t ulConnection;              /*!< Connection Handle      */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  bReserved;                 /*!< Reserved padding       */
  uint16_t ulClass;                   /*!< CIP class ID           */
  uint16_t ulInstance;                /*!< CIP Instance           */
  uint16_t ulAttribute;               /*!< CIP Attribute          */

  uint8_t  abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< Service Data   */
} EIP_OBJECT_CONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECT_MESSAGE_REQ_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_CONNECT_MESSAGE_REQ_T  tData;
} EIP_OBJECT_PACKET_CONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECT_MESSAGE_CNF_Ttag
{
  uint32_t ulConnection;              /*!< Connection Handle      */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  bReserved;                 /*!< Reserved padding       */
  uint16_t ulClass;                   /*!< CIP class ID           */
  uint16_t ulInstance;                /*!< CIP Instance           */
  uint16_t ulAttribute;               /*!< CIP Attribute          */

  uint8_t  abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< Service Data   */
} EIP_OBJECT_CONNECT_MESSAGE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECT_MESSAGE_CNF_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_CONNECT_MESSAGE_CNF_T  tData;
} EIP_OBJECT_PACKET_CONNECT_MESSAGE_CNF_T;


#define EIP_OBJECT_CONNECT_MESSAGE_REQ_SIZE (sizeof(EIP_OBJECT_CONNECT_MESSAGE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)
#define EIP_OBJECT_CONNECT_MESSAGE_CNF_SIZE (sizeof(EIP_OBJECT_CONNECT_MESSAGE_CNF_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CLOSE_CL3_REQ_Ttag
{
  uint32_t    ulConnection;          /*!< Connection Handle    */
} EIP_OBJECT_CLOSE_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CLOSE_CL3_REQ_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_CLOSE_CL3_REQ_T   tData;
} EIP_OBJECT_PACKET_CLOSE_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CLOSE_CL3_CNF_Ttag
{
  uint32_t    ulGRC;                 /*!< Generic Error Code   */
  uint32_t    ulERC;                 /*!< Extended Error Code  */
} EIP_OBJECT_CLOSE_CL3_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CLOSE_CL3_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_CLOSE_CL3_CNF_T   tData;
} EIP_OBJECT_PACKET_CLOSE_CL3_CNF_T;

#define EIP_OBJECT_CLOSE_CL3_REQ_SIZE (sizeof(EIP_OBJECT_CLOSE_CL3_REQ_T))
#define EIP_OBJECT_CLOSE_CL3_CNF_SIZE (sizeof(EIP_OBJECT_CLOSE_CL3_CNF_T))

/*#####################################################################################*/


typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_Ttag
{
  uint32_t ulSlaveHandle;           /*!< Handle of the connection to the slave \n
                                      \valueRange \n\n
                                      \description \n\n
                                       none \n\n */

  uint32_t ulActivate;              /*!< Action to the slave \n
                                      \valueRange \n\n
                                       0x01 : activate slave at scanlist             \n
                                       0x02 : deactivate slave from scanlist         \n
                                      \description \n\n
                                       none \n\n */

  uint32_t ulDelayTime;             /*!< Delay to execute start the communication \n

                                      \valueRange \n\n
                                       0x0000 - 0x7FFF : time at ms \n
                                       0xFFFFFFFF : default time from configuration \n

                                      \description \n\n
                                       none \n\n */

} EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T  tData;
} EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_REQ_T;

#define EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_SIZE (sizeof(EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_Ttag
{
  uint32_t ulSlaveHandle;           /*!< Handle of the connection to the slave \n

                                      \valueRange \n\n

                                      \description \n\n
                                       none \n\n */

  uint32_t ulActivate;              /*!< Action to the slave \n

                                      \valueRange \n\n
                                       0x01 : activate slave at scanlist             \n
                                       0x02 : deactivate slave from scanlist         \n

                                      \description \n\n
                                       none \n\n */

} EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T  tData;
} EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_CNF_T;

#define EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_SIZE (sizeof(EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T))

/* pragma unpack */
#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(EIP_DPMINTF_PUBLIC)
#endif

#endif
