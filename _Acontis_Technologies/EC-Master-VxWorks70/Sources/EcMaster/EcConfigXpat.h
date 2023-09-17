/*-----------------------------------------------------------------------------
 * EcConfigXpat.h           
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              expat implementation of the CEcConfigXpat class.
 *---------------------------------------------------------------------------*/
#if (defined INCLUDE_XPAT)

#ifndef ECCONFIGXPAT_INC
#define ECCONFIGXPAT_INC

/*-INCLUDES------------------------------------------------------------------*/
#include "EcConfig.h"
#include "expat.h"
#include "EcString.h"
#include "EcDeviceFactory.h"

/* global parser setting */

/*-DEFINES/MACROS------------------------------------------------------------*/
#ifndef CFGFILE_BUFSIZE
#define CFGFILE_BUFSIZE    (0x2000-0x500)       /* number of config bytes read from file into buffer */
#endif
#define CFGFILE_FWDSIZE     0x300       /* number of actual read config bytes when new file data shall be read */
#define MAX_XML_DATALEN     0x100       /* maximum internal XML data buffer */

const EC_T_DWORD C_dwCfgSlaveAllocGranularity = 10;

/* config parser tags/entries/attributes */
#define C_szTgEtherCATConfig "EtherCATConfig"
#if (defined INCLUDE_OEM)
#define C_szTgEncryptedEtherCATConfig "EncryptedEtherCATConfig"
#define C_szAttrEncryptionVersion "EncryptionVersion"
#endif
#define C_szTgConfig "Config"
#define C_szTgMaster "Master"
#define C_szTgSlave "Slave"
#define C_szTgCyclic "Cyclic"
#define C_szTgProcessImage "ProcessImage"
#define C_szTgProductKey "ProductKey"
#if (defined INCLUDE_OEM)
#define C_szTgManufacturer "Manufacturer"
#define C_szAttrSignature "Signature"
#endif

/* master */                                            
#define C_szTgInfo "Info"
#define C_szTgName "Name"
#define C_szTgDestination "Destination"
#define C_szTgSource "Source"
#define C_szTgEtherType "EtherType"
#define C_szTgMailboxStates "MailboxStates"
#define C_szTgStartAddr "StartAddr"
#define C_szTgCount "Count"
#define C_szTgEoE "EoE"
#define C_szTgMaxPorts "MaxPorts"
#define C_szTgMaxFrames "MaxFrames"
#define C_szTgMaxMACs "MaxMACs"
#define C_szTgInitCmds "InitCmds"
#define C_szTgInitCmd "InitCmd"
#define C_szTgProperties "Properties"
#define C_szTgTransition "Transition"
#define C_szTgBeforeSlave "BeforeSlave"
#define C_szTgComment "Comment"
#define C_szTgRequires "Requires"
#define C_szTgCmd "Cmd"
#define C_szTgAdp "Adp"
#define C_szTgAdo "Ado"
#define C_szTgAddr "Addr"
#define C_szTgData "Data"
#define C_szTgDataLength "DataLength"
#define C_szTgCnt "Cnt"
#define C_szTgRetries "Retries"
#define C_szTgValidate "Validate"
#define C_szTgDataMask "DataMask"
#define C_szTgTimeout "Timeout"


/* slave */
#define C_szTgPhysAddr "PhysAddr"
#define C_szTgAutoIncAddr "AutoIncAddr"
#define C_szTgIdentification "Identification"
#define C_szTgPhysics "Physics"
#define C_szTgVendorId "VendorId"
#define C_szTgProductCode "ProductCode"
#define C_szTgRevisionNo "RevisionNo"
#define C_szTgSerialNo "SerialNo"
#define C_szTgProductRevision "ProductRevision"
#define C_szTgSend "Send"
#define C_szTgRecv "Recv"
#define C_szTgSm0 "Sm0"
#define C_szTgSm1 "Sm1"
#define C_szTgSm2 "Sm2"
#define C_szTgSm3 "Sm3"
#define C_szTgSm4 "Sm4"
#define C_szTgSm5 "Sm5"
#define C_szTgSm6 "Sm6"
#define C_szTgSm7 "Sm7"
#define C_szTgRxPdo "RxPdo"
#define C_szTgTxPdo "TxPdo"
#define C_szTgBitStart "BitStart"
#define C_szTgBitLength "BitLength"
#define C_szTgProtocol "Protocol"
#define C_szTgCoE "CoE"
#define C_szTgStart "Start"
#define C_szTgLength "Length"
#define C_szTgPollTime "PollTime"
#define C_szTgStatusBitAddr "StatusBitAddr"
#define C_szTgFoE "FoE"
#define C_szTgEoE "EoE"
#define C_szTgVoE "VoE"
#define C_szTgAoE "AoE"
#define C_szTgFixed "Fixed"
#define C_szTgCompleteAccess "CompleteAccess"
#define C_szTgCcs "Ccs"
#define C_szTgIndex "Index"
#define C_szTgSubIndex "SubIndex"
#define C_szTgDisabled "Disabled"
#define C_szTgProcessData "ProcessData"
#define C_szTgMailbox "Mailbox"
#if (defined INCLUDE_FOE_SUPPORT)
#define C_szTgBootStrap "BootStrap"
#define C_szTgBootstrap "Bootstrap"
#define C_szTgBootMailbox "BootMailbox"
#endif
#define C_szTgPreviousPort "PreviousPort"
#define C_szTgDC "DC"
#define C_szTgReferenceClock "ReferenceClock"
#define C_szTgReferenceClockP "PotentialReferenceClock"
#define C_szTgCycleTime0 "CycleTime0"
#define C_szTgCycleTime1 "CycleTime1"
#define C_szTgShiftTime "ShiftTime"

#if (defined INCLUDE_SOE_SUPPORT)
#define C_szTgSoE "SoE"
#define C_szTgOpCode "OpCode"
#define C_szTgDriveNo "DriveNo"
#define C_szTgIDN "IDN"
#define C_szTgElements "Elements"
#define C_szTgAttribute "Attribute"
#endif
#if (defined INCLUDE_AOE_SUPPORT)
#define C_szTgNetId "NetId"
#endif

#define C_szTgRetReq    "ReturningRequest"
#define C_szTgResponse  "Response"
#define C_szTgNetId     "NetId"

#define C_szPPPort      "Port"
#define C_szPPPhysAddr  "PhysAddr"

#define C_szTgHotConnect  "HotConnect"

#if (defined INCLUDE_HOTCONNECT)
#define C_szHCGroupName         "GroupName"
#define C_szHCGroupMemberCnt    "GroupMemberCnt"
#define C_szHCIdentCommand      "IdentifyCmd"
#endif

/* cyclic */
#define C_szCycleTime "CycleTime"
#define C_szPriority "Priority"
#define C_szTaskId "TaskId"
#define C_szFrame "Frame"
#define C_szTgState "State"
#define C_szTgInputOffs "InputOffs"
#define C_szTgOutputOffs "OutputOffs"

/* ProcessImage */
#define C_szTgInputs "Inputs"
#define C_szTgOutputs "Outputs"
#define C_szTgByteSize "ByteSize"
#define C_szTgVariable "Variable"
#if (defined INCLUDE_VARREAD)
#define C_szTgDataType "DataType"
#define C_szTgBitSize "BitSize"
#define C_szTgBitOffs "BitOffs"
#endif

#define C_szTgGroup "Group"

/*-FORWARD DECLARATIONS------------------------------------------------------*/
class CEcConfigXpat;
class CEcConfigData;
class CCfgHotConnect;

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
/* config parser states */
typedef enum _EC_T_CFG_PARSER_STATE
{
    eCfgState_UNKNOWN=0,                /*< unknown */
    eCfgState_ERROR,                    /*< generic error state */
    eCfgState_ERROR_NoSubNode,          /*< error state: no sub node below this node*/
    eCfgState_INIT,                     /*< init state */

    eCfgState_Root,                                                         /*< / */
    eCfgState_EcCfg,                                                        /*< /EtherCATConfig */
    eCfgState_EcCfg_Cfg,                                                    /*< /EtherCATConfig/Config */
    eCfgState_EcCfg_Cfg_Mas,                                                /*< /EtherCATConfig/Config/Master */
    eCfgState_EcCfg_Cfg_Slv,
    eCfgState_EcCfg_Cfg_Cyclic,
    eCfgState_EcCfg_Cfg_ProcessImage,

    eCfgState_EcCfg_ProductKey,                                             /*< /EtherCATConfig/ProductKey */
    eCfgState_EcCfg_Cfg_ProductKey,                                         /*< /EtherCATConfig/Config/ProductKey */

    eCfgState_EcCfg_Cfg_Mas_Info,                                           /*< /EtherCATConfig/Config/Master/Info */
    eCfgState_EcCfg_Cfg_Mas_Info_Name,                                      /*<     :           :           :      */ 
    eCfgState_EcCfg_Cfg_Mas_Info_Destination,                               /*<     :           :           :      */ 
    eCfgState_EcCfg_Cfg_Mas_Info_Source,                                    /*<     :           :           :      */ 
    eCfgState_EcCfg_Cfg_Mas_Info_EtherType,
    eCfgState_EcCfg_Cfg_Mas_MailboxStates,                                 
    eCfgState_EcCfg_Cfg_Mas_MailboxStates_StartAddr,
    eCfgState_EcCfg_Cfg_Mas_MailboxStates_Count,
    eCfgState_EcCfg_Cfg_Mas_EoE,                                           
    eCfgState_EcCfg_Cfg_Mas_EoE_MaxPorts,                                           
    eCfgState_EcCfg_Cfg_Mas_EoE_MaxFrames,
    eCfgState_EcCfg_Cfg_Mas_EoE_MaxMACs,
    eCfgState_EcCfg_Cfg_Mas_ICmds,                                      
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                              
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_BeforeSlave,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Requires,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cmd,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Adp,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Ado,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Addr,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Data,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_DataLength,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cnt,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Retries,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Data,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_DataMask,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Timeout,
    eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Mas_Props,
    eCfgState_EcCfg_Cfg_Mas_Props_Prop,
    eCfgState_EcCfg_Cfg_Mas_Props_Prop_Name,
    eCfgState_EcCfg_Cfg_Mas_Props_Prop_Value,

    eCfgState_EcCfg_Cfg_Slv_Info,
    eCfgState_EcCfg_Cfg_Slv_Info_Name,
    eCfgState_EcCfg_Cfg_Slv_Info_PhysAddr,
    eCfgState_EcCfg_Cfg_Slv_Info_AutoIncAddr,
    eCfgState_EcCfg_Cfg_Slv_Info_Identification,
    eCfgState_EcCfg_Cfg_Slv_Info_Identification_Ado,
    eCfgState_EcCfg_Cfg_Slv_Info_Physics,
    eCfgState_EcCfg_Cfg_Slv_Info_VendorId,
    eCfgState_EcCfg_Cfg_Slv_Info_ProductCode,
    eCfgState_EcCfg_Cfg_Slv_Info_RevisionNo,
    eCfgState_EcCfg_Cfg_Slv_Info_SerialNo,
    eCfgState_EcCfg_Cfg_Slv_Info_ProductRevision,
    eCfgState_EcCfg_Cfg_Slv_ProcessData,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Send,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitStart,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitLength,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitStart,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitLength,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,

#if (defined INCLUDE_VARREAD)
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Type,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MinSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MaxSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_DefaultSize,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_StartAddress,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_ControlByte,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Virtual,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Enable,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Watchdog,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Pdo,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Index,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Name,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Exclude,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Index,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Subindex,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Bitlen,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Name,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Comment,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype,

    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Index,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Name,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Exclude,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Index,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Subindex,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Bitlen,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Name,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Comment,
    eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype,
#endif
    eCfgState_EcCfg_Cfg_Slv_Mbx,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Send,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Start,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Length,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Start,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Length,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_PollTime,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_StatusBitAddr,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Protocol,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE,
    eCfgState_EcCfg_Cfg_Slv_Mbx_FoE,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Fixed,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_CompleteAccess,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Ccs,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Index,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_SubIndex,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Data,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Disabled,
    eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_Prof,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Fixed,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Data,
#if (defined INCLUDE_SOE_SUPPORT)
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Fixed,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_OpCode,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_DriveNo,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_IDN,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Elements,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Attribute,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Data,
    eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Disabled,
#endif
#if (defined INCLUDE_FOE_SUPPORT)
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Start,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Length,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Start,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Length,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_PollTime,
    eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_StatusBitAddr,
    eCfgState_EcCfg_Cfg_Slv_BootMbx,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Send,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Start,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Length,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Start,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Length,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_PollTime,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_StatusBitAddr,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_Protocol,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_CoE,
    eCfgState_EcCfg_Cfg_Slv_BootMbx_FoE,
#endif

    eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_RetReq,
    eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_Response,

    eCfgState_EcCfg_Cfg_Slv_Mbx_VoE,

#if (defined INCLUDE_AOE_SUPPORT)
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE,
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds,                         
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,  
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Fixed,  
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Data,
    eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_NetId,
#endif

    
    eCfgState_EcCfg_Cfg_Slv_ICmds,                                      
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                              
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Transition,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_BeforeSlave,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Requires,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cmd,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Adp,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Ado,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Addr,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Data,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_DataLength,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cnt,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Retries,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Data,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_DataMask,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Timeout,
    eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Timeout,
    eCfgState_EcCfg_Cfg_Slv_PreviousPort,
    eCfgState_EcCfg_Cfg_Slv_PreviousPort_Port,
    eCfgState_EcCfg_Cfg_Slv_PreviousPort_PhysAddr,
    eCfgState_EcCfg_Cfg_Slv_PreviousPort_Selected,
    eCfgState_EcCfg_Cfg_Slv_DC,
#if (defined INCLUDE_DC_SUPPORT)
    eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClock,
    eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClockP,
    eCfgState_EcCfg_Cfg_Slv_DC_CycleTime0,
    eCfgState_EcCfg_Cfg_Slv_DC_CycleTime1,
    eCfgState_EcCfg_Cfg_Slv_DC_ShiftTime,
#endif
    eCfgState_EcCfg_Cfg_Slv_HotConnect,
#if (defined INCLUDE_HOTCONNECT)
    eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupName,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupMemberCnt,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Comment,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Requires,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cmd,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Adp,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Ado,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Data,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_DataLength,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cnt,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Retries,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Data,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_DataMask,
    eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Timeout,
#endif

    eCfgState_EcCfg_Cfg_Slv_Group,

    eCfgState_EcCfg_Cfg_Cyclic_Comment,
    eCfgState_EcCfg_Cfg_Cyclic_CycleTime,
    eCfgState_EcCfg_Cfg_Cyclic_Priority,
    eCfgState_EcCfg_Cfg_Cyclic_TaskId,
    eCfgState_EcCfg_Cfg_Cyclic_Frame,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Comment,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_State,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Comment,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cmd,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Adp,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Ado,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Addr,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Data,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_DataLength,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cnt,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_InputOffs,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_OutputOffs,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Src,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Dst,
    eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Size,

    eCfgState_EcCfg_Cfg_ProcessImage_Inputs,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_ByteSize,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,
#if (defined INCLUDE_VARREAD)
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Name,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Comment,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_DataType,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitSize,
    eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitOffs,
#endif
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_ByteSize,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,
#if (defined INCLUDE_VARREAD)
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Name,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Comment,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_DataType,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitSize,
    eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitOffs,
#endif
#if (defined INCLUDE_OEM)
    eCfgState_EcCfg_Manufacturer,
#endif

    /* Borland C++ datatype alignment correction */
    eCfgState_BCppDummy   = 0xFFFFFFFF
} EC_T_CFG_PARSER_STATE; 

typedef enum _EC_T_CFG_PARSER_SUPPORTED_TAG
{
    eTgSupp=1,
    eTgNotSupp,
    /* Borland C++ datatype alignment correction */
    eTg_BCppDummy   = 0xFFFFFFFF
} EC_T_CFG_PARSER_SUPPORTED_TAG; 

typedef enum _EC_T_CFG_PARSER_TAG_LOCATION
{
    eFrstTg=1,
    eMdlTg,
    eLstTg,
    eSnglTg,
    eValTg,
    /* Borland C++ datatype alignment correction */
    ePTL_BCppDummy   = 0xFFFFFFFF
} EC_T_CFG_PARSER_TAG_LOCATION; 

typedef struct _EC_T_CFG_STATE_CHANGE_DESC
{
    EC_T_WORD                       wCfgParserOldState;
    EC_T_WORD                       wCfgParserNewState;
    EC_T_WORD                       wTgLocation;
    EC_T_WORD                       wIsTgSupported;
    const EC_T_CHAR*                szTg;
} EC_T_CFG_STATE_CHANGE_DESC;

typedef struct _EC_T_CFG_STATE_DESCRIPTION
{
    EC_T_CFG_PARSER_STATE       CfgParserState;
    const EC_T_CHAR*            szDescription;
} EC_T_CFG_STATE_DESCRIPTION;

#if (defined INCLUDE_VARREAD)
typedef struct _EC_T_SLAVE_PDO_ENTRY
{
    EC_T_WORD          wIndex;                              /* object index */
    EC_T_WORD          wSubIndex;                           /* object subindex */
    EC_T_WORD          wPdoIndex;                           /* PDO Index */
    EC_T_INT           nBitLen;
    EC_T_INT    	   nBitOffs;
    EC_T_BOOL    	   bSwapData;
} EC_T_SLAVE_PDO_ENTRY;

typedef struct _EC_T_SLAVE_PDO_DESC
{
    EC_T_WORD               wSm;                            /* sync manager number */
    EC_T_DWORD              dwPdoArraySize;                 /* size of the m_apoPdo[] array */
    EC_T_DWORD              dwPdoCount;                     /* total number of PDO Entries */
    EC_T_SLAVE_PDO_ENTRY    oCurPdo;
    EC_T_SLAVE_PDO_ENTRY**  apoPdo;
} EC_T_SLAVE_PDO_DESC;

typedef struct _EC_T_SLAVE_SM_DESC
{
    EC_T_BOOL   bInput;                                     /* sm type (input/output) */
    EC_T_BOOL   bEnabled;                                   /* sm enabled */
    EC_T_DWORD  dwPDOffset;                                 /* process data offset (BitStart) */
    EC_T_DWORD  dwPDSize;                                   /* process data size (BitLength) */
    EC_T_DWORD  dwPDOffsetCur;                              /* current process data offset used for pdo entries */
} EC_T_SLAVE_SM_DESC;
#endif

typedef struct
{
    CEcConfigXpat*      poEcConfigXpat;
    EC_T_ENI_SLAVE*     pEniSlave;
    EC_T_WORD           wPDOutCurEntry;                     /* current process data out entry */
    EC_T_WORD           wPDInCurEntry;                      /* current process data in entry */

#if (defined INCLUDE_VARREAD)
    EC_T_SLAVE_PDO_DESC oCurRxPdoDesc;                      /* RxPDO */
    EC_T_SLAVE_PDO_DESC oCurTxPdoDesc;                      /* TxPDO */
    EC_T_SLAVE_SM_DESC  aSmSettings[8];                     /* sm settings <sm0> .. <sm7> */
#endif
} EC_T_XPAT_SLAVE;


/********************************************************************************/
/** \brief  Class for EtherCAT configuration.
*
* This class is used by the application to configure EtherCAT.
*
* \return 
*/
class CEcConfigXpat
{
private:
    XML_Parser  m_oXmlParser;
    EC_T_BOOL   m_bXmlParserCreated;

    EC_T_INT    m_nDepth;
    EC_T_VOID*  m_hCfgFile;

    EC_T_BOOL   m_bErrorHit;
    EC_T_DWORD  m_dwLastError;

    EC_T_CHAR   m_achBuff[CFGFILE_BUFSIZE];
    EC_T_BOOL   m_bNewDataElement;  /* EC_TRUE if the data handler for the current
                                     * node is called for the first time.
                                     * In this case leading white-space characters are skipped */

    EC_T_CFG_PARSER_STATE   m_ECfgParserState;                      /* current config parser state (XML node) */
    EC_T_CFG_PARSER_STATE   m_ELastCfgParserState;                  /* last config parser state (parent XML node) */
    EC_T_BOOL               m_bGoingDown;                           /* EC_TRUE if the parser is going down in the tree, EC_FALSE if not */
    EC_T_BOOL               m_bExecuteDataHandler;                  /* EC_TRUE if the data handler for the current
                                                                    * node shall be executed. This is needed to
                                                                    * paste all character data until data is complete.
                                                                    */
    EC_T_BOOL               m_bDataHandlerExecuted;                 /* EC_TRUE if the data handler for the current
                                                                    * node is already executed. This is needed to
                                                                    * avoid calling the data handler twice.
                                                                    */
    XML_Char*               m_pXmlCharDataBuf;                      /* temporary buffer to store XML character data */
    EC_T_INT                m_nXmlCharDataBufSize;                  /* current size of buffer above */
    EC_T_INT                m_nXmlCharLen;                          /* size of temporary XML character data buffer */
    EC_T_BOOL               m_bSmartParseSkipNodes;                 /* EC_TRUE if sub nodes shall be skipped */
    EC_T_INT                m_nSmartParseParentSkipNodeDepth;       /* node depth of the parent of all skipped node */
    EC_T_DWORD              m_dwCurFirstStateChangeEntryIndex;      /* current first index in state change descriptor table */

    //EC_T_MASTER_PROP_DESC*  m_poCurMasterProp;                      /* current master property description */

    EC_T_XPAT_SLAVE*        m_poCurCfgSlave;                        /* current slave configuration object */
    EC_T_XPAT_SLAVE**       m_apoCfgSlave;                          /* array of slave configuration objects */
    EC_T_DWORD              m_dwCfgSlaveArraySize;                  /* size of the m_apoCfgSlave[] array */
    EC_T_DWORD              m_dwNumCfgSlaves;                       /* number of list entries */

    EC_T_ENI_SLAVE*                 m_poCurEniSlave;                /* current ENI slave configuration object */
    EC_T_SLAVE_MBX_INIT_CMD_DESC    m_oMbxInitCmdDesc;              /* generic init command description for mailboxes */

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_HC_GROUP_CFG_DESC*         m_pCurHotConnectGroup;
#endif

#if (defined INCLUDE_VARREAD)
    EC_T_PD_VAR_INFO_INTERN         m_oCurProcessVarInfo;         /* current process image var. object */
    EC_T_CHAR                       m_szPDVarInfoSpecificDataType[MAX_PROCESS_VAR_NAME_LEN];
#endif

    EC_T_CYCLIC_ENTRY_CFG_DESC*     m_poCurCfgCyclic;             /* current cyclic configuration object */
    EcCycFrameDesc*                 m_pCurCyclicFrameDesc;        /* current cyclic frame configuration object */
    EC_T_CYCLIC_CMD_CFG_DESC        m_oCurCycCmdCfgDesc;          /* current cyclic command configuration description */

    /* Copy information for Slave-to-Slave communication */
    EC_T_CYC_COPY_INFO*     m_poCurCopyInfoEntry;                 /* copy info stored in CEcConfigData */

    /* init command parsing */
    EC_T_ECAT_INIT_CMDS_DESC        m_oCurEcatInitCmdDesc;
    EC_T_BYTE*                      m_pbyBufferData;
    EC_T_WORD                       m_wBufferDataLen;
    EC_T_WORD                       m_wBufferValidateDataLen;
    EC_T_WORD                       m_wBufferValidateDataMaskLen;

#if (defined INCLUDE_OEM)
    EC_T_BOOL               m_bEncrypted;
#endif

    /************************* Member variables linked to CEcConfigData *************************/
    CEcConfigData*          m_poEcConfigData;                       /* pointer to config data storage object */

    EcMasterDesc*           m_poCfgMaster;                          /* pointer to master configuration object stored in CEcConfigData */

    /* Functions */
private:
    EC_T_XPAT_SLAVE* CreateXpatSlave(CEcConfigXpat*  poEcConfigXpat);
    EC_T_VOID  DeleteXpatSlave(EC_T_XPAT_SLAVE*  pXpatSlave);
    EC_T_BOOL  CfgReadFixedSizeBinData(EC_T_CHAR* achInputData, EC_T_INT nInputDataLen, EC_T_DWORD wResultSize, EC_T_BYTE* pbyOutputData);
    EC_T_BOOL  CfgReadVariableSizeBinData(EC_T_CHAR* achInputData, EC_T_INT nInputDataLen, EC_T_WORD* pwDataLen, EC_T_BYTE** ppbyData, EC_T_WORD* pwBufferDataLen);
    EC_T_BOOL  CfgReadWord(EC_T_CHAR* achInputData, EC_T_INT nInputDataLen, EC_T_WORD* pwOutputData);
    EC_T_BOOL  CfgReadDword(EC_T_CHAR* achInputData, EC_T_INT nInputDataLen, EC_T_DWORD* pdwOutputData);
    EC_T_BOOL  CfgReadBoolean(EC_T_CHAR* achInputData, EC_T_BOOL* pbOutputData);
    EC_T_VOID  CfgReadTransition(EC_T_CHAR* achInputData, PEcInitCmdDesc pEcInitCmdDesc);
    EC_T_DWORD CfgReadTransition(EC_T_CHAR* achInputData);
    EC_T_WORD  CfgReadState(EC_T_CHAR* achInputData);
    EC_T_BOOL  ExpandPointerArray( EC_T_VOID*** pppvArray, EC_T_DWORD* pdwCurSize, EC_T_DWORD dwExpandSize );
#if (defined INCLUDE_VARREAD)
    EC_T_BOOL  CfgCreateECatPdoCmd(EC_T_XPAT_SLAVE* pXpatSlave, EC_T_SLAVE_PDO_DESC* pEcatPdoDesc);
    EC_T_BOOL  CfgReadSmType(EC_T_CHAR* achInputData, EC_T_BOOL* pbIsInput);
    EC_T_BOOL  CfgAssignPDOffsetToSm(EC_T_XPAT_SLAVE* pXpatSlave, EC_T_WORD wSm);
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_BOOL ParseAoeNetId(EC_T_CHAR* pszStreamBytes, EC_T_INT nStreamLen, EC_T_BYTE* pbyAoeNetId);
#endif

    EC_T_BOOL CfgCreateEcSlaveCreateParams( EC_T_ENI_SLAVE* poEniSlave );

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_poEcConfigData->GetMemLog(); }

public:	
    CEcConfigXpat(CEcConfigData* poEcConfigData);
    virtual ~CEcConfigXpat();
    
    /* Interface */
    EC_T_DWORD          Init(EC_T_CONFIGPARM_DESC* pCfgParm);

    EC_T_BOOL           IsErrorHit(EC_T_VOID)
                            { return m_bErrorHit; }
    EC_T_DWORD          LastError(EC_T_VOID)
                            { return m_dwLastError; }

#if (defined INCLUDE_VARREAD)
    EC_T_WORD           DataTypeName2DataTypeIdentifier(EC_T_BYTE* szDataTypeName);
#endif /* INCLUDE_VARREAD */

    /* internal helper */
    XML_Error StartElementHandler(const XML_Char* pTgName, const XML_Char** apAttributes);
    XML_Error EndElementHandler(const EC_T_CHAR* szTgName);
    XML_Error CharacterDataHandler(const XML_Char* pCharData, EC_T_INT nLen);
};

#endif /* ECCONFIGXPAT_INC */

#endif /*(defined INCLUDE_XPAT)*/

/*-END OF SOURCE FILE--------------------------------------------------------*/
