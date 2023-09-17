/**
 * \addtogroup FoE FileTransfer over EtherCAT
 * @{
 */

/**
\file foeappl.h
\author EthercatSSC@beckhoff.com

\version 5.01

<br>Changes to version - :<br>
V5.01 : Start file change log
 */

#ifndef _FOEAPPL_H_
#define _FOEAPPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include <ecat_def.h>


/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

#endif //_FOEAPPL_H_

#ifdef _FOEAPPL_
    #define PROTO
#else
    #define PROTO extern
#endif


/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/
PROTO    ESS_UINT16     FOE_Read(ESS_UINT16 MBXMEM * pName, ESS_UINT16 nameSize, ESS_UINT16 MBXMEM * pData, ESS_UINT32 password);
PROTO    ESS_UINT16     FOE_Write(ESS_UINT16 MBXMEM * pName, ESS_UINT16 nameSize, ESS_UINT32 password);
PROTO    ESS_UINT16     FOE_Data(ESS_UINT16 MBXMEM * pData, ESS_UINT16 Size);
PROTO    ESS_UINT16     FOE_Ack(ESS_UINT32 fileOffset, ESS_UINT16 MBXMEM * pData);
PROTO    ESS_UINT16     FOE_Busy(ESS_UINT16 done, ESS_UINT32 fileOffset, ESS_UINT16 MBXMEM * pData);
PROTO    void           FOE_Error(ESS_UINT32 errorCode);
PROTO    ESS_UINT16 	CRC_Check(const void *,ESS_UINT32);

#undef PROTO
/** @}*/
