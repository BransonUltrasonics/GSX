/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_gci_packetdefinitions_sysflags.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines system behavior flags provided with packet
EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ.

These definitions contribute to the LFW API of the EtherNet/IP stack and
are applicable to the DPM packet interface.

**************************************************************************************/


#ifndef EIP_GCI_PACKETDEFINITIONS_SYSFLAGS_H_
#define EIP_GCI_PACKETDEFINITIONS_SYSFLAGS_H_

/* Configuration system flags  */

#define WSTART_SYSFLAG_START_MSK            0x00000001
#define WSTART_SYSFLAG_START_APPLICATION    0x00000001
#define WSTART_SYSFLAG_START_AUTO           0x00000000

#define WSTART_SYSFLAG_IOSTAT_ENABLE_MSK    0x00000002
#define WSTART_SYSFLAG_IOSTAT_ENABLED_MSK   0x00000002
#define WSTART_SYSFLAG_IOSTAT_DISABLED_MSK  0x00000000

#define WSTART_SYSFLAG_IOSTAT_LEN_MSK       0x00000004
#define WSTART_SYSFLAG_IOSTAT_LEN_8BIT      0x00000000
#define WSTART_SYSFLAG_IOSTAT_LEN_32BIT     0x00000004


#endif
