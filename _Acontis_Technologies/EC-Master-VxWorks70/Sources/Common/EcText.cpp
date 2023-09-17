/*-----------------------------------------------------------------------------
 * Copyright    acontis technologies GmbH, Weingarten, Germany
 * Response     Paul Bussmann
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"
#include "EcInterfaceCommon.h"
#include "EcText.h"
#include "EcError.h"
#include "EthernetServices.h"

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
/***************************************************************************************************/
/**
\brief  Get EtherCAT Master Text by ID (for internal use).
\return Text identified through ID.
*/
const EC_T_CHAR* EcErrorText(EC_T_DWORD dwTextId)
{
#if (defined INCLUDE_TEXT)
    const EC_T_CHAR* pszText = EC_NULL;

    switch (dwTextId)
    {
    case EC_E_NOERROR:                                      pszText = EC_SZTXT_E_NOERROR; break;
    case EC_E_ERROR:                                        pszText = EC_SZTXT_E_ERROR; break;
    case EC_E_NOTSUPPORTED:                                 pszText = EC_SZTXT_E_NOTSUPPORTED; break;
    case EC_E_INVALIDINDEX:                                 pszText = EC_SZTXT_E_INVALIDINDEX; break;
    case EC_E_INVALIDOFFSET:                                pszText = EC_SZTXT_E_INVALIDOFFSET; break;
    case EC_E_CANCEL:                                       pszText = EC_SZTXT_E_CANCEL;break;
    case EC_E_INVALIDSIZE:                                  pszText = EC_SZTXT_E_INVALIDSIZE; break;
    case EC_E_INVALIDDATA:                                  pszText = EC_SZTXT_E_INVALIDDATA; break;
    case EC_E_NOTREADY:                                     pszText = EC_SZTXT_E_NOTREADY; break;
    case EC_E_BUSY:                                         pszText = EC_SZTXT_E_BUSY; break;
    case EC_E_ACYC_FRM_FREEQ_EMPTY:                         pszText = EC_SZTXT_E_ACYC_FRM_FREEQ_EMPTY; break;
    case EC_E_NOMEMORY:                                     pszText = EC_SZTXT_E_NOMEMORY; break;
    case EC_E_INVALIDPARM:                                  pszText = EC_SZTXT_E_INVALIDPARM; break;
    case EC_E_NOTFOUND:                                     pszText = EC_SZTXT_E_NOTFOUND; break;
    case EC_E_DUPLICATE:                                    pszText = EC_SZTXT_E_DUPLICATE; break;
    case EC_E_INVALIDSTATE:                                 pszText = EC_SZTXT_E_INVALIDSTATE; break;
    case EC_E_TIMER_LIST_FULL:                              pszText = EC_SZTXT_E_TIMER_LIST_FULL; break;
    case EC_E_TIMEOUT:                                      pszText = EC_SZTXT_E_TIMEOUT; break;
    case EC_E_OPENFAILED:                                   pszText = EC_SZTXT_E_OPENFAILED; break;
    case EC_E_SENDFAILED:                                   pszText = EC_SZTXT_E_SENDFAILED; break;
    case EC_E_INSERTMAILBOX:                                pszText = EC_SZTXT_E_INSERTMAILBOX; break;
    case EC_E_INVALIDCMD:                                   pszText = EC_SZTXT_E_INVALIDCMD; break;
    case EC_E_UNKNOWN_MBX_PROTOCOL:                         pszText = EC_SZTXT_E_UNKNOWN_MBX_PROTOCOL; break;
    case EC_E_ACCESSDENIED:                                 pszText = EC_SZTXT_E_ACCESSDENIED; break;
    case EC_E_IDENTIFICATIONFAILED:                         pszText = EC_SZTXT_E_IDENTIFICATIONFAILED; break;
    case EC_E_PRODKEY_INVALID:                              pszText = EC_SZTXT_E_PRODKEY_INVALID; break;
    case EC_E_WRONG_FORMAT:                                 pszText = EC_SZTXT_E_WRONG_FORMAT; break;
    case EC_E_FEATURE_DISABLED:                             pszText = EC_SZTXT_E_FEATURE_DISABLED; break;
    case EC_E_SHADOW_MEMORY:                                pszText = EC_SZTXT_E_SHADOW_MEMORY; break;
    case EC_E_BUSCONFIG_MISMATCH:                           pszText = EC_SZTXT_E_BUSCONFIG_MISMATCH; break;
    case EC_E_CONFIGDATAREAD:                               pszText = EC_SZTXT_E_CONFIGDATAREAD; break;
    case EC_E_ENI_NO_SAFEOP_OP_SUPPORT:                     pszText = EC_SZTXT_E_ENI_NO_SAFEOP_OP_SUPPORT; break;
    case EC_E_XML_CYCCMDS_MISSING:                          pszText = EC_SZTXT_E_XML_CYCCMDS_MISSING; break;
    case EC_E_XML_ALSTATUS_READ_MISSING:                    pszText = EC_SZTXT_E_XML_ALSTATUS_READ_MISSING; break;
    case EC_E_XML_CYCCMDS_SIZEMISMATCH:                     pszText = EC_SZTXT_E_XML_CYCCMDS_SIZEMISMATCH; break;
    case EC_E_XML_INVALID_INP_OFF:                          pszText = EC_SZTXT_E_XML_INVALID_INP_OFF; break;
    case EC_E_XML_INVALID_OUT_OFF:                          pszText = EC_SZTXT_E_XML_INVALID_OUT_OFF; break;
    case EC_E_MCSM_FATAL_ERROR:                             pszText = EC_SZTXT_E_MCSM_FATAL_ERROR; break;
    case EC_E_SLAVE_ERROR:                                  pszText = EC_SZTXT_E_SLAVE_ERROR; break;
    case EC_E_FRAME_LOST:                                   pszText = EC_SZTXT_E_FRAME_LOST; break;
    case EC_E_CMD_MISSING:                                  pszText = EC_SZTXT_E_CMD_MISSING; break;
    case EC_E_CYCCMD_WKC_ERROR:                             pszText = EC_SZTXT_E_CYCCMD_WKC_ERROR; break;
    case EC_E_INVALID_DCL_MODE:                             pszText = EC_SZTXT_E_INVALID_DCL_MODE; break;
    case EC_E_AI_ADDRESS:                                   pszText = EC_SZTXT_E_AI_ADDRESS; break;
    case EC_E_INVALID_SLAVE_STATE:                          pszText = EC_SZTXT_E_INVALID_SLAVE_STATE; break;
    case EC_E_SLAVE_NOT_ADDRESSABLE:                        pszText = EC_SZTXT_E_SLAVE_NOT_ADDRESSABLE; break;
    case EC_E_CYC_CMDS_OVERFLOW:                            pszText = EC_SZTXT_E_CYC_CMDS_OVERFLOW; break;
    case EC_E_LINK_DISCONNECTED:                            pszText = EC_SZTXT_E_LINK_DISCONNECTED; break;
    case EC_E_MASTERCORE_INACCESSIBLE:                      pszText = EC_SZTXT_E_MASTERCORE_INACCESSIBLE; break;
    case EC_E_COE_MBXSND_WKC_ERROR:                         pszText = EC_SZTXT_E_COE_MBXSND_WKC_ERROR; break;
    case EC_E_COE_MBXRCV_WKC_ERROR:                         pszText = EC_SZTXT_E_COE_MBXRCV_WKC_ERROR; break;
    case EC_E_NO_MBX_SUPPORT:                               pszText = EC_SZTXT_E_NO_MBX_SUPPORT; break;
    case EC_E_NO_COE_SUPPORT:                               pszText = EC_SZTXT_E_NO_COE_SUPPORT; break;
    case EC_E_NO_EOE_SUPPORT:                               pszText = EC_SZTXT_E_NO_EOE_SUPPORT; break;
    case EC_E_NO_FOE_SUPPORT:                               pszText = EC_SZTXT_E_NO_FOE_SUPPORT; break;
    case EC_E_NO_SOE_SUPPORT:                               pszText = EC_SZTXT_E_NO_SOE_SUPPORT; break;
    case EC_E_NO_VOE_SUPPORT:                               pszText = EC_SZTXT_E_NO_VOE_SUPPORT; break;
    case EC_E_NO_AOE_SUPPORT:                               pszText = EC_SZTXT_E_NO_AOE_SUPPORT; break;
    case EC_E_AOE_INV_RESPONSE_SIZE:                        pszText = EC_SZTXT_EC_E_AOE_INV_RESPONSE_SIZE; break;
    case EC_E_EVAL_VIOLATION:                               pszText = EC_SZTXT_E_EVAL_VIOLATION; break;
    case EC_E_EVAL_EXPIRED:                                 pszText = EC_SZTXT_E_EVAL_EXPIRED; break;
    case EC_E_LICENSE_MISSING:                              pszText = EC_SZTXT_E_LICENSE_MISSING; break;
    case EC_E_SDO_ABORTCODE_TOGGLE:                         pszText = EC_SZTXT_E_SDO_ABORTCODE_TOGGLE; break;
    case EC_E_SDO_ABORTCODE_TIMEOUT:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_TIMEOUT;    break;
    case EC_E_SDO_ABORTCODE_CCS_SCS:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_CCS_SCS; break;
    case EC_E_SDO_ABORTCODE_BLK_SIZE:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_BLK_SIZE; break;
    case EC_E_SDO_ABORTCODE_SEQNO:                          pszText = EC_SZTXT_E_SDO_ABORTCODE_SEQNO;  break;
    case EC_E_SDO_ABORTCODE_CRC:                            pszText = EC_SZTXT_E_SDO_ABORTCODE_CRC; break;
    case EC_E_SDO_ABORTCODE_MEMORY:                         pszText = EC_SZTXT_E_SDO_ABORTCODE_MEMORY; break;
    case EC_E_SDO_ABORTCODE_ACCESS:                         pszText = EC_SZTXT_E_SDO_ABORTCODE_ACCESS; break;
    case EC_E_SDO_ABORTCODE_WRITEONLY:                      pszText = EC_SZTXT_E_SDO_ABORTCODE_WRITEONLY; break;
    case EC_E_SDO_ABORTCODE_READONLY:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_READONLY; break;
    case EC_E_SDO_ABORTCODE_SI_NOT_WRITTEN:                 pszText = EC_SZTXT_E_SDO_ABORTCODE_SI_NOT_WRITTEN; break;
    case EC_E_SDO_ABORTCODE_CA_TYPE_MISM:                   pszText = EC_SZTXT_E_SDO_ABORTCODE_CA_TYPE_MISM; break;
    case EC_E_SDO_ABORTCODE_OBJ_TOO_BIG:                    pszText = EC_SZTXT_E_SDO_ABORTCODE_OBJ_TOO_BIG; break;
    case EC_E_SDO_ABORTCODE_PDO_MAPPED:                     pszText = EC_SZTXT_E_SDO_ABORTCODE_PDO_MAPPED; break;
    case EC_E_SDO_ABORTCODE_INDEX:                          pszText = EC_SZTXT_E_SDO_ABORTCODE_INDEX; break;
    case EC_E_SDO_ABORTCODE_PDO_MAP:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_PDO_MAP; break;
    case EC_E_SDO_ABORTCODE_PDO_LEN:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_PDO_LEN; break;
    case EC_E_SDO_ABORTCODE_P_INCOMP:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_P_INCOMP; break;
    case EC_E_SDO_ABORTCODE_I_INCOMP:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_I_INCOMP; break;
    case EC_E_SDO_ABORTCODE_HARDWARE:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_HARDWARE; break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE:                      pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_SIZE; break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE1:                     pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_SIZE1; break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE2:                     pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_SIZE2; break;
    case EC_E_SDO_ABORTCODE_OFFSET:                         pszText = EC_SZTXT_E_SDO_ABORTCODE_OFFSET; break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE:                     pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_RANGE; break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE1:                    pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_RANGE1; break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE2:                    pszText = EC_SZTXT_E_SDO_ABORTCODE_DATA_RANGE2; break;
    case EC_E_SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH:       pszText = EC_SZTXT_E_SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH; break;
    case EC_E_SDO_ABORTCODE_MINMAX:                         pszText = EC_SZTXT_E_SDO_ABORTCODE_MINMAX; break;
    case EC_E_SDO_ABORTCODE_GENERAL:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_GENERAL; break;
    case EC_E_SDO_ABORTCODE_TRANSFER:                       pszText = EC_SZTXT_E_SDO_ABORTCODE_TRANSFER; break;
    case EC_E_SDO_ABORTCODE_TRANSFER1:                      pszText = EC_SZTXT_E_SDO_ABORTCODE_TRANSFER1; break;
    case EC_E_SDO_ABORTCODE_TRANSFER2:                      pszText = EC_SZTXT_E_SDO_ABORTCODE_TRANSFER2; break;
    case EC_E_SDO_ABORTCODE_DICTIONARY:                     pszText = EC_SZTXT_E_SDO_ABORTCODE_DICTIONARY; break;
    case EC_E_SDO_ABORTCODE_UNKNOWN:                        pszText = EC_SZTXT_E_SDO_ABORTCODE_UNKNOWN; break;

    case EC_E_FOE_ERRCODE_NOTDEFINED:                       pszText = EC_SZTXT_E_FOE_ERRCODE_NOTDEFINED; break;
    case EC_E_FOE_ERRCODE_NOTFOUND:                         pszText = EC_SZTXT_E_FOE_ERRCODE_NOTFOUND; break;
    case EC_E_FOE_ERRCODE_ACCESS:                           pszText = EC_SZTXT_E_FOE_ERRCODE_ACCESS; break;
    case EC_E_FOE_ERRCODE_DISKFULL:                         pszText = EC_SZTXT_E_FOE_ERRCODE_DISKFULL; break;
    case EC_E_FOE_ERRCODE_ILLEGAL:                          pszText = EC_SZTXT_E_FOE_ERRCODE_ILLEGAL; break;
    case EC_E_FOE_ERRCODE_PACKENO:                          pszText = EC_SZTXT_E_FOE_ERRCODE_PACKENO; break;
    case EC_E_FOE_ERRCODE_EXISTS:                           pszText = EC_SZTXT_E_FOE_ERRCODE_EXISTS; break;
    case EC_E_FOE_ERRCODE_NOUSER:                           pszText = EC_SZTXT_E_FOE_ERRCODE_NOUSER; break;
    case EC_E_FOE_ERRCODE_BOOTSTRAPONLY:                    pszText = EC_SZTXT_E_FOE_ERRCODE_BOOTSTRAPONLY; break;
    case EC_E_FOE_ERRCODE_NOTINBOOTSTRAP:                   pszText = EC_SZTXT_E_FOE_ERRCODE_NOTINBOOTSTRAP; break;
    case EC_E_FOE_ERRCODE_INVALIDPASSWORD:                  pszText = EC_SZTXT_E_FOE_ERRCODE_INVALIDPASSWORD; break;
    case EC_E_FOE_ERRCODE_PROGERROR:                        pszText = EC_SZTXT_E_FOE_ERRCODE_PROGERROR; break;
    case EC_E_FOE_ERRCODE_INVALID_CHECKSUM:                 pszText = EC_SZTXT_E_FOE_ERRCODE_INVALID_CHECKSUM; break;
    case EC_E_FOE_ERRCODE_INVALID_FIRMWARE:                 pszText = EC_SZTXT_E_FOE_ERRCODE_INVALID_FIRMWARE; break;
    case EC_E_FOE_ERRCODE_NO_FILE:                          pszText = EC_SZTXT_E_FOE_ERRCODE_NO_FILE; break;
    case EC_E_FOE_ERRCODE_FILE_HEAD_MISSING:                pszText = EC_SZTXT_E_FOE_ERRCODE_FILE_HEAD_MISSING; break;
    case EC_E_FOE_ERRCODE_FLASH_PROBLEM:                    pszText = EC_SZTXT_E_FOE_ERRCODE_FLASH_PROBLEM; break;
    case EC_E_FOE_ERRCODE_FILE_INCOMPATIBLE:                pszText = EC_SZTXT_E_FOE_ERRCODE_FILE_INCOMPATIBLE; break;

    case EC_E_CFGFILENOTFOUND:                              pszText = EC_SZTXT_E_CFGFILENOTFOUND; break;
    case EC_E_EEPROMREADERROR:                              pszText = EC_SZTXT_E_EEPROMREADERROR; break;
    case EC_E_EEPROMWRITEERROR:                             pszText = EC_SZTXT_E_EEPROMWRITEERROR; break;
    case EC_E_PORTCLOSE:                                    pszText = EC_SZTXT_E_PORTCLOSE; break;
    case EC_E_PORTOPEN:                                     pszText = EC_SZTXT_E_PORTOPEN; break;
    case EC_E_SOE_ERRORCODE_INVALID_ACCESS:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_INVALID_ACCESS; break;
    case EC_E_SOE_ERRORCODE_NOT_EXIST:                      pszText = EC_SZTXT_E_SOE_ERRORCODE_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_INVL_ACC_ELEM1:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_INVL_ACC_ELEM1; break;
    case EC_E_SOE_ERRORCODE_NAME_NOT_EXIST:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_NAME_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_NAME_UNDERSIZE:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_NAME_UNDERSIZE; break;
    case EC_E_SOE_ERRORCODE_NAME_OVERSIZE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_NAME_OVERSIZE; break;
    case EC_E_SOE_ERRORCODE_NAME_UNCHANGE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_NAME_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_NAME_WR_PROT:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_NAME_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_UNDERS_TRANS:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_UNDERS_TRANS; break;
    case EC_E_SOE_ERRORCODE_OVERS_TRANS:                    pszText = EC_SZTXT_E_SOE_ERRORCODE_OVERS_TRANS; break;
    case EC_E_SOE_ERRORCODE_ATTR_UNCHANGE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_ATTR_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_ATTR_WR_PROT:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_ATTR_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_UNIT_NOT_EXIST:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_UNIT_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_UNIT_UNDERSIZE:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_UNIT_UNDERSIZE; break;
    case EC_E_SOE_ERRORCODE_UNIT_OVERSIZE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_UNIT_OVERSIZE; break;
    case EC_E_SOE_ERRORCODE_UNIT_UNCHANGE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_UNIT_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_UNIT_WR_PROT:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_UNIT_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_MIN_NOT_EXIST:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_MIN_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_MIN_UNDERSIZE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_MIN_UNDERSIZE; break;
    case EC_E_SOE_ERRORCODE_MIN_OVERSIZE:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_MIN_OVERSIZE; break;
    case EC_E_SOE_ERRORCODE_MIN_UNCHANGE:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_MIN_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_MIN_WR_PROT:                    pszText = EC_SZTXT_E_SOE_ERRORCODE_MIN_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_MAX_NOT_EXIST:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_MAX_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_MAX_UNDERSIZE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_MAX_UNDERSIZE; break;
    case EC_E_SOE_ERRORCODE_MAX_OVERSIZE:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_MAX_OVERSIZE; break;
    case EC_E_SOE_ERRORCODE_MAX_UNCHANGE:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_MAX_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_MAX_WR_PROT:                    pszText = EC_SZTXT_E_SOE_ERRORCODE_MAX_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_DATA_NOT_EXIST:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_NOT_EXIST; break;
    case EC_E_SOE_ERRORCODE_DATA_UNDERSIZE:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_UNDERSIZE; break;
    case EC_E_SOE_ERRORCODE_DATA_OVERSIZE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_OVERSIZE; break;
    case EC_E_SOE_ERRORCODE_DATA_UNCHANGE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_DATA_WR_PROT:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_WR_PROT; break;
    case EC_E_SOE_ERRORCODE_DATA_MIN_LIMIT:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_MIN_LIMIT; break;
    case EC_E_SOE_ERRORCODE_DATA_MAX_LIMIT:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_MAX_LIMIT; break;
    case EC_E_SOE_ERRORCODE_DATA_INCOR:                     pszText = EC_SZTXT_E_SOE_ERRORCODE_DATA_INCOR; break;
    case EC_E_SOE_ERRORCODE_PASWD_PROT:                     pszText = EC_SZTXT_E_SOE_ERRORCODE_PASWD_PROT; break;
    case EC_E_SOE_ERRORCODE_TEMP_UNCHANGE:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_TEMP_UNCHANGE; break;
    case EC_E_SOE_ERRORCODE_INVL_INDIRECT:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_INVL_INDIRECT; break;
    case EC_E_SOE_ERRORCODE_TEMP_UNCHANGE1:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_TEMP_UNCHANGE1; break;
    case EC_E_SOE_ERRORCODE_ALREADY_ACTIVE:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_ALREADY_ACTIVE; break;
    case EC_E_SOE_ERRORCODE_NOT_INTERRUPT:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_NOT_INTERRUPT; break;
    case EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_CMD_NOT_AVAIL; break;
    case EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL1:                 pszText = EC_SZTXT_E_SOE_ERRORCODE_CMD_NOT_AVAIL1; break;
    case EC_E_SOE_ERRORCODE_DRIVE_NO:                       pszText = EC_SZTXT_E_SOE_ERRORCODE_DRIVE_NO; break;
    case EC_E_SOE_ERRORCODE_IDN:                            pszText = EC_SZTXT_E_SOE_ERRORCODE_IDN; break;
    case EC_E_SOE_ERRORCODE_FRAGMENT_LOST:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_FRAGMENT_LOST; break;
    case EC_E_SOE_ERRORCODE_BUFFER_FULL:                    pszText = EC_SZTXT_E_SOE_ERRORCODE_BUFFER_FULL; break; 
    case EC_E_SOE_ERRORCODE_NO_DATA:                        pszText = EC_SZTXT_E_SOE_ERRORCODE_NO_DATA; break;
    case EC_E_SOE_ERRORCODE_NO_DEFAULT_VALUE:               pszText = EC_SZTXT_E_SOE_ERRORCODE_NO_DEFAULT_VALUE; break;
    case EC_E_SOE_ERRORCODE_DEFAULT_LONG:                   pszText = EC_SZTXT_E_SOE_ERRORCODE_DEFAULT_LONG; break;
    case EC_E_SOE_ERRORCODE_DEFAULT_WP:                     pszText = EC_SZTXT_E_SOE_ERRORCODE_DEFAULT_WP; break;
    case EC_E_SOE_ERRORCODE_INVL_DRIVE_NO:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_INVL_DRIVE_NO; break;
    case EC_E_SOE_ERRORCODE_GENERAL_ERROR:                  pszText = EC_SZTXT_E_SOE_ERRORCODE_GENERAL_ERROR; break;
    case EC_E_SOE_ERRCODE_NO_ELEM_ADR:                      pszText = EC_SZTXT_E_SOE_ERRCODE_NO_ELEM_ADR; break;
    case EC_E_SLAVE_NOT_PRESENT:                            pszText = EC_SZTXT_E_SLAVE_NOT_PRESENT; break;
    case EC_E_NO_FOE_SUPPORT_BS:                            pszText = EC_SZTXT_E_NO_FOE_SUPPORT_BS; break;
    case EC_E_EEPROMRELOADERROR:                            pszText = EC_SZTXT_E_EEPROMRELOADERROR; break;
    case EC_E_SLAVECTRLRESETERROR:                          pszText = EC_SZTXT_E_SLAVECTRLRESETERROR; break;
    case EC_E_SYSDRIVERMISSING:                             pszText = EC_SZTXT_E_SYSDRIVERMISSING; break;
    case EC_E_MBX_ERROR_TYPE:                               pszText = EC_SZTXT_E_MBX_ERROR_TYPE; break;
    case EC_E_REDLINEBREAK:                                 pszText = EC_SZTXT_E_REDLINEBREAK; break;
    case EC_E_XML_INVALID_CMD_WITH_RED:                     pszText = EC_SZTXT_E_XML_INVALID_CMD_WITH_RED; break;
    case EC_E_XML_PREV_PORT_MISSING:                        pszText = EC_SZTXT_E_XML_PREV_PORT_MISSING; break;
    case EC_E_XML_DC_CYCCMDS_MISSING:                       pszText = EC_SZTXT_E_XML_DC_CYCCMDS_MISSING; break;
    case EC_E_LINE_CROSSED:                                 pszText = EC_SZTXT_E_LINE_CROSSED; break;
    case EC_E_LINE_CROSSED_SLAVE_INFO:                      pszText = EC_SZTXT_E_LINE_CROSSED_SLAVE_INFO; break;
    case EC_E_SOCKET_DISCONNECTED:                          pszText = EC_SZTXT_E_SOCKET_DISCONNECTED; break;
    case EC_E_ADO_NOT_SUPPORTED:                            pszText = EC_SZTXT_E_ADO_NOT_SUPPORTED; break;
    
    case EC_E_PTS_IS_NOT_RUNNING:                           pszText = EC_SZTXT_E_PTS_IS_NOT_RUNNING; break;
    case EC_E_PTS_IS_RUNNING:                               pszText = EC_SZTXT_E_PTS_IS_RUNNING; break;
    case EC_E_PTS_THREAD_CREATE_FAILED:                     pszText = EC_SZTXT_E_PTS_THREAD_CREATE_FAILED; break;
    case EC_E_PTS_SOCK_BIND_FAILED:                         pszText = EC_SZTXT_E_PTS_SOCK_BIND_FAILED; break;
    case EC_E_PTS_NOT_ENABLED:                              pszText = EC_SZTXT_EC_E_PTS_NOT_ENABLED; break;
    case EC_E_PTS_LL_MODE_NOT_SUPPORTED:                    pszText = EC_SZTXT_EC_E_PTS_LL_MODE_NOT_SUPPORTED; break;
    case EC_E_VOE_NO_MBX_RECEIVED:                          pszText = EC_SZTXT_EC_E_VOE_NO_MBX_RECEIVED; break;
    
    case EC_E_BUSCONFIG_TOPOCHANGE:                         pszText = EC_SZTXT_E_BUSCONFIG_TOPOCHANGE; break;
    case EC_E_DLSTATUS_IRQ_TOPOCHANGED:                     pszText = EC_SZTXT_E_DLSTATUS_IRQ_TOPOCHANGED; break;
    
    case EC_E_AOE_ERROR:                                    pszText = EC_SZTXT_E_AOE_ERROR; break;
    case EC_E_AOE_SRVNOTSUPP:                               pszText = EC_SZTXT_E_AOE_SRVNOTSUPP; break;
    case EC_E_AOE_INVALIDGRP:                               pszText = EC_SZTXT_E_AOE_INVALIDGRP; break;
    case EC_E_AOE_INVALIDOFFSET:                            pszText = EC_SZTXT_E_AOE_INVALIDOFFSET; break;
    case EC_E_AOE_INVALIDACCESS:                            pszText = EC_SZTXT_E_AOE_INVALIDACCESS; break;
    case EC_E_AOE_INVALIDSIZE:                              pszText = EC_SZTXT_E_AOE_INVALIDSIZE; break;
    case EC_E_AOE_INVALIDDATA:                              pszText = EC_SZTXT_E_AOE_INVALIDDATA; break;
    case EC_E_AOE_NOTREADY:                                 pszText = EC_SZTXT_E_AOE_NOTREADY; break;
    case EC_E_AOE_BUSY:                                     pszText = EC_SZTXT_E_AOE_BUSY; break;
    case EC_E_AOE_INVALIDCONTEXT:                           pszText = EC_SZTXT_E_AOE_INVALIDCONTEXT; break;
    case EC_E_AOE_NOMEMORY:                                 pszText = EC_SZTXT_E_AOE_NOMEMORY; break;
    case EC_E_AOE_INVALIDPARM:                              pszText = EC_SZTXT_E_AOE_INVALIDPARM; break;
    case EC_E_AOE_NOTFOUND:                                 pszText = EC_SZTXT_E_AOE_NOTFOUND; break;
    case EC_E_AOE_SYNTAX:                                   pszText = EC_SZTXT_E_AOE_SYNTAX; break;
    case EC_E_AOE_INCOMPATIBLE:                             pszText = EC_SZTXT_E_AOE_INCOMPATIBLE; break;
    case EC_E_AOE_EXISTS:                                   pszText = EC_SZTXT_E_AOE_EXISTS; break;
    case EC_E_AOE_SYMBOLNOTFOUND:                           pszText = EC_SZTXT_E_AOE_SYMBOLNOTFOUND; break;
    case EC_E_AOE_SYMBOLVERSIONINVALID:                     pszText = EC_SZTXT_E_AOE_SYMBOLVERSIONINVALID; break;
    case EC_E_AOE_INVALIDSTATE:                             pszText = EC_SZTXT_E_AOE_INVALIDSTATE; break;
    case EC_E_AOE_TRANSMODENOTSUPP:                         pszText = EC_SZTXT_E_AOE_TRANSMODENOTSUPP; break;
    case EC_E_AOE_NOTIFYHNDINVALID:                         pszText = EC_SZTXT_E_AOE_NOTIFYHNDINVALID; break;
    case EC_E_AOE_CLIENTUNKNOWN:                            pszText = EC_SZTXT_E_AOE_CLIENTUNKNOWN; break;
    case EC_E_AOE_NOMOREHDLS:                               pszText = EC_SZTXT_E_AOE_NOMOREHDLS; break;
    case EC_E_AOE_NOTINIT:                                  pszText = EC_SZTXT_E_AOE_NOTINIT; break;
    case EC_E_AOE_INVALIDWATCHSIZE:                         pszText = EC_SZTXT_E_AOE_INVALIDWATCHSIZE; break;
    case EC_E_AOE_TIMEOUT:                                  pszText = EC_SZTXT_E_AOE_TIMEOUT; break;
    case EC_E_AOE_NOINTERFACE:                              pszText = EC_SZTXT_E_AOE_NOINTERFACE; break;
    case EC_E_AOE_INVALIDINTERFACE:                         pszText = EC_SZTXT_E_AOE_INVALIDINTERFACE; break;
    case EC_E_AOE_INVALIDCLSID:                             pszText = EC_SZTXT_E_AOE_INVALIDCLSID; break;
    case EC_E_AOE_INVALIDOBJID:                             pszText = EC_SZTXT_E_AOE_INVALIDOBJID; break;
    case EC_E_AOE_PENDING:                                  pszText = EC_SZTXT_E_AOE_PENDING; break;
    case EC_E_AOE_ABORTED:                                  pszText = EC_SZTXT_E_AOE_ABORTED; break;
    case EC_E_AOE_WARNING:                                  pszText = EC_SZTXT_E_AOE_WARNING; break;
    case EC_E_AOE_INVALIDARRAYIDX:                          pszText = EC_SZTXT_E_DLSTATUS_IRQ_TOPOCHANGED; break;
    case EC_E_AOE_SYMBOLNOTACTIVE:                          pszText = EC_SZTXT_E_AOE_SYMBOLNOTACTIVE; break;
    case EC_E_AOE_ACCESSDENIED:                             pszText = EC_SZTXT_E_AOE_ACCESSDENIED; break;
    case EC_E_AOE_INTERNAL:                                 pszText = EC_SZTXT_E_AOE_INTERNAL; break;
    case EC_E_AOE_TARGET_PORT_NOT_FOUND:                    pszText = EC_SZTXT_E_AOE_TARGET_PORT_NOT_FOUND; break;
    case EC_E_AOE_TARGET_MACHINE_NOT_FOUND:                 pszText = EC_SZTXT_E_AOE_TARGET_MACHINE_NOT_FOUND; break;
    case EC_E_AOE_UNKNOWN_CMD_ID:                           pszText = EC_SZTXT_E_AOE_UNKNOWN_CMD_ID; break;
    case EC_E_AOE_PORT_NOT_CONNECTED:                       pszText = EC_SZTXT_E_AOE_PORT_NOT_CONNECTED; break;
    case EC_E_AOE_INVALID_AMS_LENGTH:                       pszText = EC_SZTXT_E_AOE_INVALID_AMS_LENGTH; break;
    case EC_E_AOE_INVALID_AMS_ID:                           pszText = EC_SZTXT_E_AOE_INVALID_AMS_ID; break;
    case EC_E_AOE_PORT_DISABLED:                            pszText = EC_SZTXT_E_AOE_PORT_DISABLED; break;
    case EC_E_AOE_PORT_CONNECTED:                           pszText = EC_SZTXT_E_AOE_PORT_CONNECTED; break;
    case EC_E_AOE_INVALID_AMS_PORT:                         pszText = EC_SZTXT_E_AOE_INVALID_AMS_PORT; break;
    case EC_E_AOE_NO_MEMORY:                                pszText = EC_SZTXT_E_AOE_NO_MEMORY; break;
    case EC_E_AOE_VENDOR_SPECIFIC:                          pszText = EC_SZTXT_E_AOE_VENDOR_SPECIFIC; break;
    case EC_E_XML_AOE_NETID_INVALID:                        pszText = EC_SZTXT_E_XML_AOE_NETID_INVALID; break;
    case EC_E_MAX_BUS_SLAVES_EXCEEDED:                      pszText = EC_SZTXT_E_MAX_BUS_SLAVES_EXCEEDED; break;
    case EC_E_MBXERR_SYNTAX:                                pszText = EC_SZTXT_E_MBXERR_SYNTAX; break;
    case EC_E_MBXERR_UNSUPPORTEDPROTOCOL:                   pszText = EC_SZTXT_E_MBXERR_UNSUPPORTEDPROTOCOL; break;
    case EC_E_MBXERR_INVALIDCHANNEL:                        pszText = EC_SZTXT_E_MBXERR_INVALIDCHANNEL; break;
    case EC_E_MBXERR_SERVICENOTSUPPORTED:                   pszText = EC_SZTXT_E_MBXERR_SERVICENOTSUPPORTED; break;
    case EC_E_MBXERR_INVALIDHEADER:                         pszText = EC_SZTXT_E_MBXERR_INVALIDHEADER; break;
    case EC_E_MBXERR_SIZETOOSHORT:                          pszText = EC_SZTXT_E_MBXERR_SIZETOOSHORT; break;
    case EC_E_MBXERR_NOMOREMEMORY:                          pszText = EC_SZTXT_E_MBXERR_NOMOREMEMORY; break;
    case EC_E_MBXERR_INVALIDSIZE:                           pszText = EC_SZTXT_E_MBXERR_INVALIDSIZE; break;
    
    case EC_E_DATA_TYPE_CONVERSION_FAILED:                  pszText = EC_SZTXT_E_DATA_TYPE_CONVERSION_FAILED; break;
    case EC_E_FOE_ERRCODE_MAX_FILE_SIZE:                    pszText = EC_SZTXT_E_FOE_ERRCODE_MAX_FILE_SIZE; break;
        
    case EC_E_EOE_MBX_WKC_ERROR:                            pszText = EC_SZTXT_E_EOE_MBX_WKC_ERROR; break;
    case EC_E_FOE_MBX_WKC_ERROR:                            pszText = EC_SZTXT_E_FOE_MBX_WKC_ERROR; break;
    case EC_E_SOE_MBX_WKC_ERROR:                            pszText = EC_SZTXT_E_SOE_MBX_WKC_ERROR; break;
    case EC_E_AOE_MBX_WKC_ERROR:                            pszText = EC_SZTXT_E_AOE_MBX_WKC_ERROR; break;
    case EC_E_VOE_MBX_WKC_ERROR:                            pszText = EC_SZTXT_E_VOE_MBX_WKC_ERROR; break;

    case EC_E_DC_REF_CLOCK_SYNC_OUT_UNIT_DISABLED:          pszText = EC_SZTXT_EC_E_DC_REF_CLOCK_SYNC_OUT_UNIT_DISABLED; break;
    case EC_E_DC_REF_CLOCK_NOT_FOUND:                       pszText = EC_SZTXT_EC_E_DC_REF_CLOCK_NOT_FOUND; break;
    case EC_E_DC_SLAVES_BEFORE_REF_CLOCK:                   pszText = EC_SZTXT_E_DC_SLAVES_BEFORE_REF_CLOCK; break;

    case EC_E_MBX_CMD_WKC_ERROR:                            pszText = EC_SZTXT_E_MBX_CMD_WKC_ERROR; break;

    case EC_E_OEM_SIGNATURE_MISMATCH:                       pszText = EC_SZTXT_E_OEM_SIGNATURE_MISMATCH; break;
    case EC_E_OEM_KEY_MISMATCH:                             pszText = EC_SZTXT_E_OEM_KEY_MISMATCH; break;
    case EC_E_OEM_KEY_MISSING:                              pszText = EC_SZTXT_E_OEM_KEY_MISSING; break;
    case EC_E_ENI_ENCRYPTION_WRONG_VERSION:                 pszText = EC_SZTXT_E_ENI_ENCRYPTION_WRONG_VERSION; break;
    case EC_E_ENI_ENCRYPTED:                                pszText = EC_SZTXT_E_ENI_ENCRYPTED; break;
    case EC_E_FRAMELOSS_AFTER_SLAVE:                        pszText = EC_SZTXT_E_FRAMELOSS_AFTER_SLAVE; break;

    case EMRAS_E_ERROR:                                     pszText = EMRAS_SZTXT_EMRAS_E_ERROR; break;
    case EMRAS_E_INVALIDCOOKIE:                             pszText = EMRAS_SZTXT_E_INVALIDCOOKIE; break;
    case EMRAS_E_MULSRVDISMULCON:                           pszText = EMRAS_SZTXT_E_MULSRVDISMULCON; break;
    case EMRAS_E_LOGONCANCELLED:                            pszText = EMRAS_SZTXT_E_LOGONCANCELLED; break;
    case EMRAS_E_INVALIDVERSION:                            pszText = EMRAS_SZTXT_E_INVALIDVERSION; break;
    case EMRAS_E_INVALIDACCESSCONFIG:                       pszText = EMRAS_SZTXT_E_INVALIDACCESSCONFIG; break;
    case EMRAS_E_ACCESSLESS:                                pszText = EMRAS_SZTXT_E_ACCESSLESS; break;
    case EMRAS_E_INVALIDDATARECEIVED:                       pszText = EMRAS_SZTXT_E_INVALIDDATARECEIVED; break;
    
    case EMRAS_EVT_SERVERSTOPPED:                           pszText = EMRAS_SZTXT_EVT_SERVERSTOPPED; break;
    case EMRAS_EVT_WDEXPIRED:                               pszText = EMRAS_SZTXT_EVT_WDEXPIRED; break;
    case EMRAS_EVT_RECONEXPIRED:                            pszText = EMRAS_SZTXT_EVT_RECONEXPIRED; break;
    case EMRAS_EVT_CLIENTLOGON:                             pszText = EMRAS_SZTXT_EVT_CLIENTLOGON; break;
    case EMRAS_EVT_RECONNECT:                               pszText = EMRAS_SZTXT_EVT_CLIENTLOGON; break;
    case EMRAS_EVT_SOCKCHANGE:                              pszText = EMRAS_SZTXT_EVT_SOCKCHANGE; break;
    case EMRAS_EVT_CLNTDISC:                                pszText = EMRAS_SZTXT_EVT_CLNTDISC; break;

    case DCM_E_ERROR:                                       pszText = DCM_SZTXT_E_ERROR; break;
    case DCM_E_NOTINITIALIZED:                              pszText = DCM_SZTXT_E_NOTINITIALIZED; break;
    case DCM_E_MAX_CTL_ERROR_EXCEED:                        pszText = DCM_SZTXT_E_MAX_CTL_ERROR_EXCEED; break;
    case DCM_E_NOMEMORY:                                    pszText = DCM_SZTXT_E_NOMEMORY; break;
    case DCM_E_INVALID_HWLAYER:                             pszText = DCM_SZTXT_E_INVALID_HWLAYER; break;
    case DCM_E_TIMER_MODIFY_ERROR:                          pszText = DCM_SZTXT_E_TIMER_MODIFY_ERROR;  break;
    case DCM_E_TIMER_NOT_RUNNING:                           pszText = DCM_SZTXT_E_TIMER_NOT_RUNNING; break;
    case DCM_E_WRONG_CPU:                                   pszText = DCM_SZTXT_E_WRONG_CPU; break;
    case DCM_E_INVALID_SYNC_PERIOD:                         pszText = DCM_SZTXT_E_INVALID_SYNC_PERIOD; break;
    case DCM_E_INVALID_SETVAL:                              pszText = DCM_SZTXT_E_INVALID_SETVAL; break;
    case DCM_E_DRIFT_TO_HIGH:                               pszText = DCM_SZTXT_E_DRIFT_TO_HIGH; break;
    case DCM_E_BUS_CYCLE_WRONG:                             pszText = DCM_SZTXT_E_BUS_CYCLE_WRONG; break;

    case DCX_E_NO_EXT_CLOCK:                                pszText = DCX_SZTXT_E_NO_EXT_CLOCK; break;
    case DCM_E_INVALID_DATA:                                pszText = DCM_SZTXT_E_INVALID_DATA; break;

    case EC_TEXTBASE:                                       pszText = EC_SZTXT_TEXTBASE; break;                     // 0x0200
    case EC_TXT_MASTER_STATE_CHANGE:                        pszText = EC_SZTXT_TXT_MASTER_STATE_CHANGE; break;
    case EC_TXT_SB_RESULT_OK:                               pszText = EC_SZTXT_TXT_SB_RESULT_OK; break;
    case EC_TXT_SB_RESULT_ERROR:                            pszText = EC_SZTXT_TXT_SB_RESULT_ERROR; break;
    case EC_TXT_DC_STATUS:                                  pszText = EC_SZTXT_TXT_DC_STATUS; break;
    case EC_TXT_DCL_STATUS:                                 pszText = EC_SZTXT_TXT_DCL_STATUS; break;
    case EC_TXT_COE_SDO_DNLD_ERROR:                         pszText = EC_SZTXT_TXT_COE_SDO_DNLD_ERROR; break;
    case EC_TXT_COE_SDO_UPLD_ERROR:                         pszText = EC_SZTXT_TXT_COE_SDO_UPLD_ERROR; break;
    case EC_TXT_COE_GETODL_ERROR:                           pszText = EC_SZTXT_TXT_COE_GETODL_ERROR; break;
    case EC_TXT_COE_GETOBDESC_ERROR:                        pszText = EC_SZTXT_TXT_COE_GETOBDESC_ERROR; break;
    case EC_TXT_COE_GETENTRYDESC_ERROR:                     pszText = EC_SZTXT_TXT_COE_GETENTRYDESC_ERROR; break;
    case EC_TXT_COE_EMRG_TFER_ERROR:                        pszText = EC_SZTXT_TXT_COE_EMRG_TFER_ERROR; break;
    case EC_TXT_COE_EMRG:                                   pszText = EC_SZTXT_TXT_COE_EMRG; break;
    case EC_TXT_CYCCMD_WKC_ERROR:                           pszText = EC_SZTXT_TXT_CYCCMD_WKC_ERROR; break;
    case EC_TXT_MASTINITCMD_WKC_ERROR:                      pszText = EC_SZTXT_TXT_MASTINITCMD_WKC_ERROR; break;
    case EC_TXT_SLVINITCMD_WKC_ERROR:                       pszText = EC_SZTXT_TXT_SLVINITCMD_WKC_ERROR; break;
    case EC_TXT_EOEMBXRCV_WKC_ERROR:                        pszText = EC_SZTXT_TXT_EOEMBXRCV_WKC_ERROR; break;
    case EC_TXT_COEMBXRCV_WKC_ERROR:                        pszText = EC_SZTXT_TXT_COEMBXRCV_WKC_ERROR; break;
    case EC_TXT_FOEMBXRCV_WKC_ERROR:                        pszText = EC_SZTXT_TXT_FOEMBXRCV_WKC_ERROR; break;
    case EC_TXT_SOEMBXRCV_WKC_ERROR:                        pszText = EC_SZTXT_TXT_SOEMBXRCV_WKC_ERROR; break;
    case EC_TXT_EOEMBXSND_WKC_ERROR:                        pszText = EC_SZTXT_TXT_EOEMBXSND_WKC_ERROR; break;
    case EC_TXT_COEMBXSND_WKC_ERROR:                        pszText = EC_SZTXT_TXT_COEMBXSND_WKC_ERROR; break;
    case EC_TXT_FOEMBXSND_WKC_ERROR:                        pszText = EC_SZTXT_TXT_FOEMBXSND_WKC_ERROR; break;
    case EC_TXT_SOEMBXSND_WKC_ERROR:                        pszText = EC_SZTXT_TXT_SOEMBXSND_WKC_ERROR; break;
    case EC_TXT_SOEMBX_WRITE_ERROR:                         pszText = EC_SZTXT_TXT_SOEMBX_WRITE_ERROR; break;
    case EC_TXT_FRMRESP_RETRY:                              pszText = EC_SZTXT_TXT_FRMRESP_RETRY; break;
    case EC_TXT_FRMRESP_NORETRY:                            pszText = EC_SZTXT_TXT_FRMRESP_NORETRY; break;
    case EC_TXT_ADDERRINFO:                                 pszText = EC_SZTXT_TXT_ADDERRINFO; break;
    case EC_TXT_CMDIDXACTVAL:                               pszText = EC_SZTXT_TXT_CMDIDXACTVAL; break;
    case EC_TXT_CMDIDXSETVAL:                               pszText = EC_SZTXT_TXT_CMDIDXSETVAL; break;
    case EC_TXT_SLVINITCMDRSPERR_NR:                        pszText = EC_SZTXT_TXT_SLVINITCMDRSPERR_NR; break;
    case EC_TXT_SLVINITCMDRSPERR_VE:                        pszText = EC_SZTXT_TXT_SLVINITCMDRSPERR_VE; break;
    case EC_TXT_SLVINITCMDRSPERR_FLD:                       pszText = EC_SZTXT_TXT_SLVINITCMDRSPERR_FLD; break;
    case EC_TXT_MASTINITCMDRSPERR_NR:                       pszText = EC_SZTXT_TXT_MASTINITCMDRSPERR_NR; break;
    case EC_TXT_MASTINITCMDRSPERR_VE:                       pszText = EC_SZTXT_TXT_MASTINITCMDRSPERR_VE; break;
    case EC_TXT_CMD_MISSING:                                pszText = EC_SZTXT_TXT_CMD_MISSING; break;
    case EC_TXT_MBSLV_INITCMDTO:                            pszText = EC_SZTXT_TXT_MBSLV_INITCMDTO; break;
    case EC_TXT_NOT_ALL_DEVS_OP:                            pszText = EC_SZTXT_TXT_NOT_ALL_DEVS_OP; break;
    case EC_TXT_CABLE_CONNECTED:                            pszText = EC_SZTXT_TXT_CABLE_CONNECTED; break;
    case EC_TXT_CABLE_NOT_CONNECTED:                        pszText = EC_SZTXT_TXT_CABLE_NOT_CONNECTED; break;
    case EC_TXT_CYCCMD_TIMEOUT:                             pszText = EC_SZTXT_TXT_CYCCMD_TIMEOUT; break;
    case EC_TXT_REDLINEBREAK:                               pszText = EC_SZTXT_TXT_REDLINEBREAK; break;
    case EC_TXT_SLVERR_DETECTED:                            pszText = EC_SZTXT_TXT_SLVERR_DETECTED; break;
    case EC_TXT_SLVERR_INFO:                                pszText = EC_SZTXT_TXT_SLVERR_INFO; break;
    case EC_TXT_SLV_NOT_ADDRABLE:                           pszText = EC_SZTXT_TXT_SLV_NOT_ADDRABLE; break;
    case EC_TXT_MBSLV_SDO_ABORT:                            pszText = EC_SZTXT_TXT_MBSLV_SDO_ABORT; break;
    case EC_TXT_DCSLVSYNC_INSYNC:                           pszText = EC_SZTXT_TXT_DCSLVSYNC_INSYNC; break;
    case EC_TXT_DCSLVSYNC_OUTOFSYNC:                        pszText = EC_SZTXT_TXT_DCSLVSYNC_OUTOFSYNC; break;
    case EC_TXT_DCL_SINGLE_LATCH:                           pszText = EC_SZTXT_TXT_DCL_SINGLE_LATCH; break;
    case EC_TXT_FOE_UPLD_ERROR:                             pszText = EC_SZTXT_TXT_FOE_UPLD_ERROR; break;           // 0x0230
    case EC_TXT_FOE_DNLD_ERROR:                             pszText = EC_SZTXT_TXT_FOE_DNLD_ERROR; break;
    case EC_TXT_CLNTREGDROP:                                pszText = EC_SZTXT_TXT_CLNTREGDROP; break;
    case EC_TXT_REDLINEFIXED:                               pszText = EC_SZTXT_TXT_REDLINEFIXED; break;
    case EC_TXT_DCM_INSYNC:                                 pszText = EC_SZTXT_TXT_DCM_INSYNC; break;
    case EC_TXT_DCM_OUTOFSYNC:                              pszText = EC_SZTXT_TXT_DCM_OUTOFSYNC; break;
    case EC_TXT_SLAVE_STATECHANGED:                         pszText = EC_SZTXT_TXT_SLAVE_STATECHANGED; break;
    case EC_TXT_DCX_INSYNC:                                 pszText = EC_SZTXT_TXT_DCX_INSYNC; break;
    case EC_TXT_DCX_OUTOFSYNC:                              pszText = EC_SZTXT_TXT_DCX_OUTOFSYNC; break;

    case EC_TXT_FRAME_TYPE_CYCLIC:                          pszText = EC_SZTXT_TXT_FRAME_TYPE_CYCLIC; break;
    case EC_TXT_FRAME_TYPE_ACYCLIC:                         pszText = EC_SZTXT_TXT_FRAME_TYPE_ACYCLIC; break;

    case EC_TXT_FRAME_RESPONSE_ERRTYPE_NO:                  pszText = EC_SZTXT_TXT_FRAME_RESPONSE_ERRTYPE_NO; break;
    case EC_TXT_FRAME_RESPONSE_ERRTYPE_WRONG:               pszText = EC_SZTXT_TXT_FRAME_RESPONSE_ERRTYPE_WRONG; break;
    case EC_TXT_FRAME_RESPONSE_ERRTYPE_UNEXPECTED:          pszText = EC_SZTXT_TXT_FRAME_RESPONSE_ERRTYPE_UNEXPECTED; break;
    case EC_TXT_FRAME_RESPONSE_ERRTYPE_RETRY_FAIL:          pszText = EC_SZTXT_TXT_FRAME_RESPONSE_ERRTYPE_RETRY_FAIL; break;

    case EC_TXT_DEVICE_STATE_UNKNOWN:                       pszText = EC_SZTXT_TXT_DEVICE_STATE_UNKNOWN; break;     // 0x0240
    case EC_TXT_DEVICE_STATE_INIT:                          pszText = EC_SZTXT_TXT_DEVICE_STATE_INIT; break;
    case EC_TXT_DEVICE_STATE_PREOP:                         pszText = EC_SZTXT_TXT_DEVICE_STATE_PREOP; break;
    case EC_TXT_DEVICE_STATE_BOOTSTRAP:                     pszText = EC_SZTXT_TXT_DEVICE_STATE_BOOTSTRAP; break;
    case EC_TXT_DEVICE_STATE_SAFEOP:                        pszText = EC_SZTXT_TXT_DEVICE_STATE_SAFEOP; break;
    case EC_TXT_DEVICE_STATE_OP:                            pszText = EC_SZTXT_TXT_DEVICE_STATE_OP; break;
              
    case EC_TXT_HC_DETAGRESULT_OK:                          pszText = EC_SZTXT_TXT_HC_DETAGRESULT_OK; break;        // 0x0250
    case EC_TXT_HC_DETAGRESULT_ERROR:                       pszText = EC_SZTXT_TXT_HC_DETAGRESULT_ERROR; break;
    case EC_TXT_SLAVE_ABSENT:                               pszText = EC_SZTXT_TXT_SLAVE_ABSENT; break;
    case EC_TXT_SLAVE_PRESENT:                              pszText = EC_SZTXT_TXT_SLAVE_PRESENT; break;
    case EC_TXT_HC_TOPOCHGDONE:                             pszText = EC_SZTXT_TXT_HC_TOPOCHGDONE; break;
    case EC_TXT_HC_BORDERCLOSE:                             pszText = EC_SZTXT_TXT_HC_BORDERCLOSE; break;
    case EC_TXT_SOE_WRITE_ERROR:                            pszText = EC_SZTXT_TXT_SOE_WRITE_ERROR; break;
    case EC_TXT_SOE_READ_ERROR:                             pszText = EC_SZTXT_TXT_SOE_READ_ERROR; break;
    case EC_TXT_SOE_EMRG_TFER_ERROR:                        pszText = EC_SZTXT_TXT_SOE_EMRG_TFER_ERROR; break;
    case EC_TXT_SOE_EMRG:                                   pszText = EC_SZTXT_TXT_SOE_EMRG; break;
    case EC_TXT_SOE_NOTIFIC_TFER_ERROR:                     pszText = EC_SZTXT_TXT_SOE_NOTIFIC_TFER_ERROR; break;
    case EC_TXT_SOE_NOTIFICATION:                           pszText = EC_SZTXT_TXT_SOE_NOTIFICATION; break;
    case EC_TXT_MBSLV_FOE_ABORT:                            pszText = EC_SZTXT_TXT_MBSLV_FOE_ABORT; break;
    case EC_TXT_MBXRCV_INVALID_DATA:                        pszText = EC_SZTXT_TXT_MBXRCV_INVALID_DATA; break;
    case EC_TXT_PDIWATCHDOG:                                pszText = EC_SZTXT_TXT_PDIWATCHDOG; break;
    case EC_TXT_SLAVE_NOTSUPPORTED:                         pszText = EC_SZTXT_TXT_SLAVE_NOTSUPPORTED; break;
    case EC_TXT_SLAVE_UNEXPECTED_STATE:                     pszText = EC_SZTXT_TXT_SLAVE_UNEXPECTED_STATE; break;
    case EC_TXT_ALL_DEVS_OP:                                pszText = EC_SZTXT_TXT_ALL_DEVS_OP; break;
    case EC_TXT_VOE_DNLD_ERROR:                             pszText = EC_SZTXT_TXT_VOE_DNLD_ERROR; break;
    case EC_TXT_VOE_UPLD_ERROR:                             pszText = EC_SZTXT_TXT_VOE_UPLD_ERROR; break;
    case EC_TXT_VOEMBXSND_WKC_ERROR:                        pszText = EC_SZTXT_TXT_VOEMBXSND_WKC_ERROR; break;
    case EC_TXT_AOE_CMD_ERROR:                              pszText = EC_SZTXT_TXT_AOE_CMD_ERROR; break;
    case EC_TXT_EEPROM_CHECKSUM_ERROR:                      pszText = EC_SZTXT_TXT_EEPROM_CHECKSUM_ERROR; break;
    case EC_TXT_JUNCTION_RED_CHANGE:                        pszText = EC_SZTXT_TXT_JUNCTION_RED_CHANGE; break;
    case EC_TXT_LINE_CROSSED:                               pszText = EC_SZTXT_TXT_LINE_CROSSED; break;
    case EC_TXT_SLAVES_UNEXPECTED_STATE:                    pszText = EC_SZTXT_TXT_SLAVES_UNEXPECTED_STATE; break;
    case EC_TXT_SLAVE_STATE_ERROR_UNKNOWN:                  pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_UNKNOWN; break;
    case EC_TXT_SLAVE_STATE_ERROR_INIT:                     pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_INIT; break;
    case EC_TXT_SLAVE_STATE_ERROR_PREOP:                    pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_PREOP; break;
    case EC_TXT_SLAVE_STATE_ERROR_BOOTSTRAP:                pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_BOOTSTRAP; break;
    case EC_TXT_SLAVE_STATE_ERROR_SAFEOP:                   pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_SAFEOP; break;
    case EC_TXT_SLAVE_STATE_ERROR_OP:                       pszText = EC_SZTXT_TXT_SLAVE_STATE_ERROR_OP; break;
    case EC_TXT_FRAMELOSS_AFTER_SLAVE:                      pszText = EC_SZTXT_TXT_FRAMELOSS_AFTER_SLAVE; break;

    case EC_TXT_STATUSCODE_NOERROR:                         pszText = EC_SZTXT_TXT_STATUSCODE_NOERROR; break;          // 0x0300
    case EC_TXT_STATUSCODE_ERROR:                           pszText = EC_SZTXT_TXT_STATUSCODE_ERROR; break;
    case EC_TXT_STATUSCODE_NO_MEMORY:                       pszText = EC_SZTXT_TXT_STATUSCODE_NO_MEMORY; break;
    case EC_TXT_STATUSCODE_INVDEVSETUP:                     pszText = EC_SZTXT_TXT_STATUSCODE_INVDEVSETUP; break;
    case EC_TXT_STATUSCODE_FW_MISMATCH:                     pszText = EC_SZTXT_TXT_STATUSCODE_FW_MISMATCH; break;
    case EC_TXT_STATUSCODE_FW_UPDATE_ERR:                   pszText = EC_SZTXT_TXT_STATUSCODE_FW_UPDATE_ERR; break;
    case EC_TXT_STATUSCODE_LICENSE_ERROR:                   pszText = EC_SZTXT_TXT_STATUSCODE_LICENSE_ERROR; break;
    case EC_TXT_STATUSCODE_INVREQSTATECNG:                  pszText = EC_SZTXT_TXT_STATUSCODE_INVREQSTATECNG; break;
    case EC_TXT_STATUSCODE_UNKREQSTATE:                     pszText = EC_SZTXT_TXT_STATUSCODE_UNKREQSTATE; break;
    case EC_TXT_STATUSCODE_BOOTSTRAPNSUPP:                  pszText = EC_SZTXT_TXT_STATUSCODE_BOOTSTRAPNSUPP; break;
    case EC_TXT_STATUSCODE_NOVALIDFW:                       pszText = EC_SZTXT_TXT_STATUSCODE_NOVALIDFW; break;
    case EC_TXT_STATUSCODE_INVALIDMBXCNF1:                  pszText = EC_SZTXT_TXT_STATUSCODE_INVALIDMBXCNF1; break;
    case EC_TXT_STATUSCODE_INVALIDMBXCNF2:                  pszText = EC_SZTXT_TXT_STATUSCODE_INVALIDMBXCNF2; break;
    case EC_TXT_STATUSCODE_INVALIDSMCNF:                    pszText = EC_SZTXT_TXT_STATUSCODE_INVALIDSMCNF; break;
    case EC_TXT_STATUSCODE_NOVALIDIN:                       pszText = EC_SZTXT_TXT_STATUSCODE_NOVALIDIN; break;
    case EC_TXT_STATUSCODE_NOVALIDOUT:                      pszText = EC_SZTXT_TXT_STATUSCODE_NOVALIDOUT; break;
    case EC_TXT_STATUSCODE_SYNCERROR:                       pszText = EC_SZTXT_TXT_STATUSCODE_SYNCERROR; break;
    case EC_TXT_STATUSCODE_SMWATCHDOG:                      pszText = EC_SZTXT_TXT_STATUSCODE_SMWATCHDOG; break;
    case EC_TXT_STATUSCODE_INVSMTYPES:                      pszText = EC_SZTXT_TXT_STATUSCODE_INVSMTYPES; break;
    case EC_TXT_STATUSCODE_INVOUTCONFIG:                    pszText = EC_SZTXT_TXT_STATUSCODE_INVOUTCONFIG; break;
    case EC_TXT_STATUSCODE_INVINCONFIG:                     pszText = EC_SZTXT_TXT_STATUSCODE_INVINCONFIG; break;
    case EC_TXT_STATUSCODE_INVWDCONFIG:                     pszText = EC_SZTXT_TXT_STATUSCODE_INVWDCONFIG; break;
    case EC_TXT_STATUSCODE_SLVNEEDCOLDRS:                   pszText = EC_SZTXT_TXT_STATUSCODE_SLVNEEDCOLDRS; break;
    case EC_TXT_STATUSCODE_SLVNEEDINIT:                     pszText = EC_SZTXT_TXT_STATUSCODE_SLVNEEDINIT; break;
    case EC_TXT_STATUSCODE_SLVNEEDPREOP:                    pszText = EC_SZTXT_TXT_STATUSCODE_SLVNEEDPREOP; break;
    case EC_TXT_STATUSCODE_SLVNEEDSAFEOP:                   pszText = EC_SZTXT_TXT_STATUSCODE_SLVNEEDSAFEOP; break;
    case EC_TXT_STATUSCODE_INVALID_INPUT_MAPPING:           pszText = EC_SZTXT_TXT_STATUSCODE_INVALID_INPUT_MAPPING; break;
    case EC_TXT_STATUSCODE_INVALID_OUTPUT_MAPPING:          pszText = EC_SZTXT_TXT_STATUSCODE_INVALID_OUTPUT_MAPPING; break;
    case EC_TXT_STATUSCODE_INCONSISTENT_SETTINGS:           pszText = EC_SZTXT_TXT_STATUSCODE_INCONSISTENT_SETTINGS; break;
    case EC_TXT_STATUSCODE_FREERUN_NOT_SUPPORTED:           pszText = EC_SZTXT_TXT_STATUSCODE_FREERUN_NOT_SUPPORTED; break;
    case EC_TXT_STATUSCODE_SYNCMODE_NOT_SUPPORTED:          pszText = EC_SZTXT_TXT_STATUSCODE_SYNCMODE_NOT_SUPPORTED; break;
    case EC_TXT_STATUSCODE_FREERUN_NEEDS_THREEBUFFER_MODE:  pszText = EC_SZTXT_TXT_STATUSCODE_FREERUN_NEEDS_THREEBUFFER_MODE; break;
    case EC_TXT_STATUSCODE_BACKGROUND_WATCHDOG:             pszText = EC_SZTXT_TXT_STATUSCODE_BACKGROUND_WATCHDOG; break;
    case EC_TXT_STATUSCODE_NO_VALID_INPUTS_AND_OUTPUTS:     pszText = EC_SZTXT_TXT_STATUSCODE_NO_VALID_INPUTS_AND_OUTPUTS; break;
    case EC_TXT_STATUSCODE_FATAL_SYNC_ERROR:                pszText = EC_SZTXT_TXT_STATUSCODE_FATAL_SYNC_ERROR; break;
    case EC_TXT_STATUSCODE_NO_SYNC_ERROR:                   pszText = EC_SZTXT_TXT_STATUSCODE_NO_SYNC_ERROR; break;
    case EC_TXT_STATUSCODE_CYCLE_TOO_SHORT:                 pszText = EC_SZTXT_TXT_STATUSCODE_CYCLE_TOO_SHORT; break;
    case EC_TXT_STATUSCODE_INVDCSYNCCNFG:                   pszText = EC_SZTXT_TXT_STATUSCODE_INVDCSYNCCNFG; break;
    case EC_TXT_STATUSCODE_INVDCLATCHCNFG:                  pszText = EC_SZTXT_TXT_STATUSCODE_INVDCLATCHCNFG; break;
    case EC_TXT_STATUSCODE_PLLERROR:                        pszText = EC_SZTXT_TXT_STATUSCODE_PLLERROR; break;
    case EC_TXT_STATUSCODE_INVDCIOERROR:                    pszText = EC_SZTXT_TXT_STATUSCODE_INVDCIOERROR; break;
    case EC_TXT_STATUSCODE_INVDCTOERROR:                    pszText = EC_SZTXT_TXT_STATUSCODE_INVDCTOERROR; break;
    case EC_TXT_STATUSCODE_DC_INVALID_SYNC_CYCLE_TIME:      pszText = EC_SZTXT_TXT_STATUSCODE_DC_INVALID_SYNC_CYCLE_TIME; break;
    case EC_TXT_STATUSCODE_DC_SYNC0_CYCLE_TIME:             pszText = EC_SZTXT_TXT_STATUSCODE_DC_SYNC0_CYCLE_TIME; break;
    case EC_TXT_STATUSCODE_DC_SYNC1_CYCLE_TIME:             pszText = EC_SZTXT_TXT_STATUSCODE_DC_SYNC1_CYCLE_TIME; break;
    case EC_TXT_STATUSCODE_MBX_AOE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_AOE; break;
    case EC_TXT_STATUSCODE_MBX_EOE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_EOE; break;
    case EC_TXT_STATUSCODE_MBX_COE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_COE; break;
    case EC_TXT_STATUSCODE_MBX_FOE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_FOE; break;
    case EC_TXT_STATUSCODE_MBX_SOE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_SOE; break;
    case EC_TXT_STATUSCODE_MBX_VOE:                         pszText = EC_SZTXT_TXT_STATUSCODE_MBX_VOE; break;
    case EC_TXT_STATUSCODE_EEPROM_NO_ACCESS:                pszText = EC_SZTXT_TXT_STATUSCODE_EEPROM_NO_ACCESS; break;
    case EC_TXT_STATUSCODE_EEPROM_ERROR:                    pszText = EC_SZTXT_TXT_STATUSCODE_EEPROM_ERROR; break;
    case EC_TXT_STATUSCODE_EXT_HARDWARE_NOT_READY:          pszText = EC_SZTXT_TXT_STATUSCODE_EXT_HARDWARE_NOT_READY; break;
    case EC_TXT_STATUSCODE_SLAVE_RESTARTED_LOCALLY:         pszText = EC_SZTXT_TXT_STATUSCODE_SLAVE_RESTARTED_LOCALLY; break;
    case EC_TXT_STATUSCODE_DEVICE_IDENTIFICATION_UPDATED:   pszText = EC_SZTXT_TXT_STATUSCODE_DEVICE_IDENTIFICATION_UPDATED; break;
    case EC_TXT_STATUSCODE_MODULE_ID_LIST_NOT_MATCH:        pszText = EC_SZTXT_TXT_STATUSCODE_MODULE_ID_LIST_NOT_MATCH; break;
    case EC_TXT_STATUSCODE_APPLICATION_CONTROLLER_AVAILABLE:pszText = EC_SZTXT_TXT_STATUSCODE_APPLICATION_CONTROLLER_AVAILABLE; break;

    default:                                
        pszText = (EC_T_CHAR*)"Unknown Text"; 
        EC_DBGMSG((EC_T_CHAR*)"EcErrorText(): unknown text ID 0x%x\n", dwTextId);
        break;
    }

    return pszText;
#else
    EC_UNREFPARM(dwTextId);
    return (EC_T_CHAR*)"Unknown Text";
#endif /* INCLUDE_TEXT */
}

const EC_T_CHAR* EcNotifyText(EC_T_DWORD  dwNotifyCode)
{
#ifdef INCLUDE_TEXT
    const EC_T_CHAR* pszText     = EC_NULL;
    
    switch (dwNotifyCode)
    {
    case EC_NOTIFY_STATECHANGED:                    pszText = EC_SZTXT_NOTIFY_STATECHANGED; break;
    case EC_NOTIFY_ETH_LINK_CONNECTED:              pszText = EC_SZTXT_NOTIFY_ETH_LINK_CONNECTED; break;
    case EC_NOTIFY_SB_STATUS:                       pszText = EC_SZTXT_NOTIFY_SB_STATUS; break;
    case EC_NOTIFY_DC_STATUS:                       pszText = EC_SZTXT_NOTIFY_DC_STATUS; break;
    case EC_NOTIFY_DC_SLV_SYNC:                     pszText = EC_SZTXT_NOTIFY_DC_SLV_SYNC; break;
    case EC_NOTIFY_DCL_STATUS:                      pszText = EC_SZTXT_NOTIFY_DCL_STATUS; break;
    case EC_NOTIFY_DCM_SYNC:                        pszText = EC_SZTXT_NOTIFY_DCM_SYNC; break;
    case EC_NOTIFY_DCX_SYNC:                        pszText = EC_SZTXT_NOTIFY_DCX_SYNC; break;
    case EC_NOTIFY_SLAVE_STATECHANGED:              pszText = EC_SZTXT_NOTIFY_SLAVE_STATECHANGED; break;
    case EC_NOTIFY_SLAVES_STATECHANGED:             pszText = EC_SZTXT_NOTIFY_SLAVES_STATECHANGED; break;
    case EC_NOTIFY_RAWCMD_DONE:                     pszText = EC_SZTXT_NOTIFY_RAWCMD_DONE; break;
    case EC_NOTIFY_MBOXRCV:                         pszText = EC_SZTXT_NOTIFY_MBOXRCV; break;
    case EC_NOTIFY_RAWMBX_DONE:                     pszText = EC_SZTXT_NOTIFY_RAWMBX_DONE; break;
    case EC_NOTIFY_COE_INIT_CMD:                    pszText = EC_SZTXT_EC_NOTIFY_COE_INIT_CMD; break;
#ifdef INCLUDE_COE_PDO_SUPPORT
    case EC_NOTIFY_COE_TX_PDO:                      pszText = EC_SZTXT_NOTIFY_COE_TX_PDO; break;
#endif
    case EC_NOTIFY_CYCCMD_WKC_ERROR:                pszText = EC_SZTXT_NOTIFY_CYCCMD_WKC_ERROR; break;
    case EC_NOTIFY_MASTER_INITCMD_WKC_ERROR:        pszText = EC_SZTXT_NOTIFY_MASTER_INITCMD_WKC_ERROR; break;
    case EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR:         pszText = EC_SZTXT_NOTIFY_SLAVE_INITCMD_WKC_ERROR; break;
    case EC_NOTIFY_EOE_MBXSND_WKC_ERROR:            pszText = EC_SZTXT_NOTIFY_EOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_COE_MBXSND_WKC_ERROR:            pszText = EC_SZTXT_NOTIFY_COE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_FOE_MBXSND_WKC_ERROR:            pszText = EC_SZTXT_NOTIFY_FOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_FRAME_RESPONSE_ERROR:            pszText = EC_SZTXT_NOTIFY_FRAME_RESPONSE_ERROR; break;
    case EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR:    pszText = EC_SZTXT_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR; break;
    case EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR:   pszText = EC_SZTXT_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR; break;
    case EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT:         pszText = EC_SZTXT_NOTIFY_MBSLAVE_INITCMD_TIMEOUT; break;
    case EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL:     pszText = EC_SZTXT_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL; break;
    case EC_NOTIFY_ETH_LINK_NOT_CONNECTED:          pszText = EC_SZTXT_NOTIFY_ETH_LINK_NOT_CONNECTED; break;
    case EC_NOTIFY_RED_LINEBRK:                     pszText = EC_SZTXT_NOTIFY_RED_LINEBRK; break;
    case EC_NOTIFY_STATUS_SLAVE_ERROR:              pszText = EC_SZTXT_NOTIFY_STATUS_SLAVE_ERROR; break;
    case EC_NOTIFY_SLAVE_ERROR_STATUS_INFO:         pszText = EC_SZTXT_NOTIFY_SLAVE_ERROR_STATUS_INFO; break;
    case EC_NOTIFY_SLAVES_ERROR_STATUS:             pszText = EC_SZTXT_NOTIFY_SLAVES_ERROR_STATUS; break;
    case EC_NOTIFY_FRAMELOSS_AFTER_SLAVE:           pszText = EC_SZTXT_NOTIFY_FRAMELOSS_AFTER_SLAVE; break;
    case EC_NOTIFY_SLAVE_NOT_ADDRESSABLE:           pszText = EC_SZTXT_NOTIFY_SLAVE_NOT_ADDRESSABLE; break;
    case EC_NOTIFY_SOE_MBXSND_WKC_ERROR:            pszText = EC_SZTXT_NOTIFY_SOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_SOE_WRITE_ERROR:                 pszText = EC_SZTXT_NOTIFY_SOE_WRITE_ERROR; break;
    case EC_NOTIFY_MBSLAVE_COE_SDO_ABORT:           pszText = EC_SZTXT_NOTIFY_MBSLAVE_COE_SDO_ABORT; break;
    case EC_NOTIFY_CLIENTREGISTRATION_DROPPED:      pszText = EC_SZTXT_NOTIFY_CLIENTREGISTRATION_DROPPED; break;
    case EC_NOTIFY_RED_LINEFIXED:                   pszText = EC_SZTXT_NOTIFY_RED_LINEFIXED; break;
    case EC_NOTIFY_FOE_MBSLAVE_ERROR:               pszText = EC_SZTXT_NOTIFY_FOE_MBSLAVE_ERROR; break;
    case EC_NOTIFY_MBXRCV_INVALID_DATA:             pszText = EC_SZTXT_NOTIFY_MBXRCV_INVALID_DATA; break;
    case EC_NOTIFY_PDIWATCHDOG:                     pszText = EC_SZTXT_NOTIFY_PDIWATCHDOG; break;
    case EC_NOTIFY_SLAVE_NOTSUPPORTED:              pszText = EC_SZTXT_NOTIFY_SLAVE_NOTSUPPORTED; break;
    case EC_NOTIFY_SLAVE_UNEXPECTED_STATE:          pszText = EC_SZTXT_NOTIFY_SLAVE_UNEXPECTED_STATE; break;
    case EC_NOTIFY_ALL_DEVICES_OPERATIONAL:         pszText = EC_SZTXT_NOTIFY_ALL_DEVICES_OPERATIONAL; break;
    case EC_NOTIFY_VOE_MBXSND_WKC_ERROR:            pszText = EC_SZTXT_NOTIFY_VOE_MBXSND_WKC_ERROR; break;
    case EC_NOTIFY_EEPROM_CHECKSUM_ERROR:           pszText = EC_SZTXT_NOTIFY_EEPROM_CHECKSUM_ERROR; break;
    case EC_NOTIFY_LINE_CROSSED:                    pszText = EC_SZTXT_NOTIFY_LINE_CROSSED; break;
    case EC_NOTIFY_JUNCTION_RED_CHANGE:             pszText = EC_SZTXT_NOTIFY_JUNCTION_RED_CHANGE; break;    
    case EC_NOTIFY_SLAVES_UNEXPECTED_STATE:         pszText = EC_SZTXT_NOTIFY_SLAVES_UNEXPECTED_STATE; break;
    case EC_NOTIFY_SB_MISMATCH:                     pszText = EC_SZTXT_NOTIFY_SB_MISMATCH; break;
    case EC_NOTIFY_SB_DUPLICATE_HC_NODE:            pszText = EC_SZTXT_NOTIFY_SB_DUPLICATE_HC_NODE; break;
    case EC_NOTIFY_HC_DETECTADDGROUPS:              pszText = EC_SZTXT_NOTIFY_HC_DETECTADDGROUPS; break;
    case EC_NOTIFY_HC_PROBEALLGROUPS:               pszText = EC_SZTXT_NOTIFY_HC_PROBEALLGROUPS; break;
    case EC_NOTIFY_HC_TOPOCHGDONE:                  pszText = EC_SZTXT_NOTIFY_HC_TOPOCHGDONE; break;
    case EC_NOTIFY_SLAVE_DISAPPEARS:                pszText = EC_SZTXT_NOTIFY_SLAVE_DISAPPEARS; break;
    case EC_NOTIFY_SLAVE_APPEARS:                   pszText = EC_SZTXT_NOTIFY_SLAVE_APPEARS; break;
    case EC_NOTIFY_SLAVE_PRESENCE:                  pszText = EC_SZTXT_NOTIFY_SLAVE_PRESENCE; break;
    case EC_NOTIFY_SLAVES_PRESENCE:                 pszText = EC_SZTXT_NOTIFY_SLAVES_PRESENCE; break;
    case EC_NOTIFY_REFCLOCK_PRESENCE:               pszText = EC_SZTXT_NOTIFY_REFCLOCK_PRESENCE; break;
    case ATEMRAS_NOTIFY_CONNECTION:                 pszText = EC_SZTXT_ATEMRAS_NOTIFY_CONNECTION; break;
    case ATEMRAS_NOTIFY_REGISTER:                   pszText = EC_SZTXT_ATEMRAS_NOTIFY_REGISTER; break;
    case ATEMRAS_NOTIFY_UNREGISTER:                 pszText = EC_SZTXT_ATEMRAS_NOTIFY_UNREGISTER; break;
    case ATEMRAS_NOTIFY_MARSHALERROR:               pszText = EC_SZTXT_ATEMRAS_NOTIFY_MARSHALERROR; break;
    case ATEMRAS_NOTIFY_ACKERROR:                   pszText = EC_SZTXT_ATEMRAS_NOTIFY_ACKERROR; break;
    case ATEMRAS_NOTIFY_NONOTIFYMEMORY:             pszText = EC_SZTXT_ATEMRAS_NOTIFY_NONOTIFYMEMORY; break;

    case ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL:       pszText = EC_SZTXT_ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL; break;
    case ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL:       pszText = EC_SZTXT_ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL; break;

    default:                                
        pszText = (EC_T_CHAR*)"Unknown Text"; 
        EC_DBGMSG((EC_T_CHAR*)"EcNotifyText(): unknown notification code 0x%x\n", dwNotifyCode);
        break;
    }
    return pszText;
#else
    EC_UNREFPARM(dwNotifyCode);
    return (EC_T_CHAR*)"Unknown Text";
#endif /* INCLUDE_TEXT */
}

EC_T_DWORD EcConvertAdsErrorToEcError(EC_T_DWORD dwAoeError)
{
    EC_T_DWORD dwErrorCode;
#if (defined INCLUDE_AOE_SUPPORT)
    switch (dwAoeError)
    {
    case  ADSERR_NOERROR:                       dwErrorCode = EC_E_NOERROR;                         break;
    case  ADSERR_DEVICE_NO_RTIME:               dwErrorCode = EC_E_AOE_NO_RTIME;                    break;
    case  ADSERR_DEVICE_LOCKED_MEMORY:          dwErrorCode = EC_E_AOE_LOCKED_MEMORY;               break;
    case  ADSERR_DEVICE_MAILBOX:                dwErrorCode = EC_E_AOE_MAILBOX;                     break;
    case  ADSERR_DEVICE_WRONG_HMSG:             dwErrorCode = EC_E_AOE_WRONG_HMSG;                  break;
    case  ADSERR_DEVICE_BAD_TASK_ID:            dwErrorCode = EC_E_AOE_BAD_TASK_ID;                 break;
    case  ADSERR_DEVICE_NO_IO:                  dwErrorCode = EC_E_AOE_NO_IO;                       break;
    case  ADSERR_DEVICE_UNKNOWN_AMS_COMMAND:    dwErrorCode = EC_E_AOE_UNKNOWN_AMS_COMMAND;         break;
    case  ADSERR_DEVICE_WIN32:                  dwErrorCode = EC_E_AOE_WIN32;                       break;
    case  ADSERR_DEVICE_LOW_INSTALL_LEVEL:      dwErrorCode = EC_E_AOE_LOW_INSTALL_LEVEL;           break;
    case  ADSERR_DEVICE_NO_DEBUG:               dwErrorCode = EC_E_AOE_NO_DEBUG;                    break;
    case  ADSERR_DEVICE_AMS_SYNC_WIN32:         dwErrorCode = EC_E_AOE_AMS_SYNC_WIN32;              break;
    case  ADSERR_DEVICE_AMS_SYNC_TIMEOUT:       dwErrorCode = EC_E_AOE_AMS_SYNC_TIMEOUT;            break;
    case  ADSERR_DEVICE_AMS_SYNC_AMS:           dwErrorCode = EC_E_AOE_AMS_SYNC_AMS;                break;
    case  ADSERR_DEVICE_AMS_SYNC_NO_INDEX_MAP:  dwErrorCode = EC_E_AOE_AMS_SYNC_NO_INDEX_MAP;       break;
    case  ADSERR_DEVICE_TCP_SEND:               dwErrorCode = EC_E_AOE_TCP_SEND;                    break;
    case  ADSERR_DEVICE_HOST_UNREACHABLE:       dwErrorCode = EC_E_AOE_HOST_UNREACHABLE;            break;
    case  ADSERR_DEVICE_INVALIDAMSFRAGMENT:     dwErrorCode = EC_E_AOE_INVALIDAMSFRAGMENT;          break;
    case  ADSERR_DEVICE_NO_LOCKED_MEMORY:       dwErrorCode = EC_E_AOE_NO_LOCKED_MEMORY;            break;
    case  ADSERR_DEVICE_MAILBOX_FULL:           dwErrorCode = EC_E_AOE_MAILBOX_FULL;                break;
    case  ADSERR_DEVICE_ERROR:                  dwErrorCode = EC_E_AOE_ERROR;                       break;
    case  ADSERR_DEVICE_SRVNOTSUPP:		        dwErrorCode = EC_E_AOE_SRVNOTSUPP;                  break;
    case  ADSERR_DEVICE_INVALIDGRP: 	        dwErrorCode = EC_E_AOE_INVALIDGRP;                  break;
    case  ADSERR_DEVICE_INVALIDOFFSET:		    dwErrorCode = EC_E_AOE_INVALIDOFFSET;               break;
    case  ADSERR_DEVICE_INVALIDACCESS:		    dwErrorCode = EC_E_AOE_INVALIDACCESS;               break;
    case  ADSERR_DEVICE_INVALIDSIZE:	        dwErrorCode = EC_E_AOE_INVALIDSIZE;                 break;
    case  ADSERR_DEVICE_INVALIDDATA:	        dwErrorCode = EC_E_AOE_INVALIDDATA;                 break;
    case  ADSERR_DEVICE_NOTREADY:			    dwErrorCode = EC_E_AOE_NOTREADY;                    break;
    case  ADSERR_DEVICE_BUSY:				    dwErrorCode = EC_E_AOE_BUSY;                        break;
    case  ADSERR_DEVICE_INVALIDCONTEXT:	        dwErrorCode = EC_E_AOE_INVALIDCONTEXT;              break;
    case  ADSERR_DEVICE_NOMEMORY:			    dwErrorCode = EC_E_AOE_NOMEMORY;                    break;
    case  ADSERR_DEVICE_INVALIDPARM:	        dwErrorCode = EC_E_AOE_INVALIDPARM;                 break;
    case  ADSERR_DEVICE_NOTFOUND:			    dwErrorCode = EC_E_AOE_NOTFOUND;                    break;
    case  ADSERR_DEVICE_SYNTAX:			        dwErrorCode = EC_E_AOE_SYNTAX;                      break;
    case  ADSERR_DEVICE_INCOMPATIBLE:		    dwErrorCode = EC_E_AOE_INCOMPATIBLE;                break;
    case  ADSERR_DEVICE_EXISTS:			        dwErrorCode = EC_E_AOE_EXISTS;                      break;
    case  ADSERR_DEVICE_SYMBOLNOTFOUND:	        dwErrorCode = EC_E_AOE_SYMBOLNOTFOUND;              break;
    case  ADSERR_DEVICE_SYMBOLVERSIONINVALID:	dwErrorCode = EC_E_AOE_SYMBOLVERSIONINVALID;        break;
    case  ADSERR_DEVICE_INVALIDSTATE:		    dwErrorCode = EC_E_AOE_INVALIDSTATE;                break;
    case  ADSERR_DEVICE_TRANSMODENOTSUPP:	    dwErrorCode = EC_E_AOE_TRANSMODENOTSUPP;            break;
    case  ADSERR_DEVICE_NOTIFYHNDINVALID:	    dwErrorCode = EC_E_AOE_NOTIFYHNDINVALID;            break;
    case  ADSERR_DEVICE_CLIENTUNKNOWN:	        dwErrorCode = EC_E_AOE_CLIENTUNKNOWN;               break;
    case  ADSERR_DEVICE_NOMOREHDLS:	            dwErrorCode = EC_E_AOE_NOMOREHDLS;                  break;
    case  ADSERR_DEVICE_INVALIDWATCHSIZE:	    dwErrorCode = EC_E_AOE_INVALIDWATCHSIZE;            break;
    case  ADSERR_DEVICE_NOTINIT:	            dwErrorCode = EC_E_AOE_NOTINIT;                     break;
    case  ADSERR_DEVICE_TIMEOUT:	            dwErrorCode = EC_E_AOE_TIMEOUT;                     break;
    case  ADSERR_DEVICE_NOINTERFACE:	        dwErrorCode = EC_E_AOE_NOINTERFACE;                 break;
    case  ADSERR_DEVICE_INVALIDINTERFACE:	    dwErrorCode = EC_E_AOE_INVALIDINTERFACE;            break;
    case  ADSERR_DEVICE_INVALIDCLSID:	        dwErrorCode = EC_E_AOE_INVALIDCLSID;                break;
    case  ADSERR_DEVICE_INVALIDOBJID:	        dwErrorCode = EC_E_AOE_INVALIDOBJID;                break;
    case  ADSERR_DEVICE_PENDING:	            dwErrorCode = EC_E_AOE_PENDING;                     break;
    case  ADSERR_DEVICE_ABORTED:	            dwErrorCode = EC_E_AOE_ABORTED;                     break;
    case  ADSERR_DEVICE_WARNING:	            dwErrorCode = EC_E_AOE_WARNING;                     break;
    case  ADSERR_DEVICE_INVALIDARRAYIDX:	    dwErrorCode = EC_E_AOE_INVALIDARRAYIDX;             break;
    case  ADSERR_DEVICE_SYMBOLNOTACTIVE:	    dwErrorCode = EC_E_AOE_SYMBOLNOTACTIVE;             break;
    case  ADSERR_DEVICE_ACCESSDENIED:	        dwErrorCode = EC_E_AOE_ACCESSDENIED;                break;
    case  ADSERR_DEVICE_INTERNAL:               dwErrorCode = EC_E_AOE_INTERNAL;                    break;
    case  ADSERR_DEVICE_TARGET_PORT_NOT_FOUND:  dwErrorCode = EC_E_AOE_TARGET_PORT_NOT_FOUND;       break;
    case  ADSERR_DEVICE_TARGET_MACHINE_NOT_FOUND: dwErrorCode = EC_E_AOE_TARGET_MACHINE_NOT_FOUND;  break;
    case  ADSERR_DEVICE_UNKNOWN_CMD_ID:         dwErrorCode = EC_E_AOE_UNKNOWN_CMD_ID;              break;
    case  ADSERR_DEVICE_PORT_NOT_CONNECTED:     dwErrorCode = EC_E_AOE_PORT_NOT_CONNECTED;          break;
    case  ADSERR_DEVICE_INVALID_AMS_LENGTH:     dwErrorCode = EC_E_AOE_INVALID_AMS_LENGTH;          break;
    case  ADSERR_DEVICE_INVALID_AMS_ID:         dwErrorCode = EC_E_AOE_INVALID_AMS_ID;              break;
    case  ADSERR_DEVICE_PORT_DISABLED:          dwErrorCode = EC_E_AOE_PORT_DISABLED;               break;
    case  ADSERR_DEVICE_PORT_CONNECTED:         dwErrorCode = EC_E_AOE_PORT_CONNECTED;              break;
    case  ADSERR_DEVICE_INVALID_AMS_PORT:       dwErrorCode = EC_E_AOE_INVALID_AMS_PORT;            break;
    case  ADSERR_DEVICE_NO_MEMORY:              dwErrorCode = EC_E_AOE_NO_MEMORY;                   break;
    default:                                    dwErrorCode = EC_E_AOE_ERROR;                       break;
    }
#else
	EC_UNREFPARM(dwAoeError);
    dwErrorCode = EC_E_AOE_ERROR;
#endif
	return dwErrorCode;
}


/********************************************************************************/
/** \brief Convert master error code to AoE error code
*
* \return AOE error code
*/
EC_T_DWORD EcConvertEcErrorToAdsError(EC_T_DWORD dwErrorCode)
{
    EC_T_DWORD dwAoeError;
#if (defined INCLUDE_AOE_SUPPORT)
    switch (dwErrorCode)
    {
    case EC_E_NOERROR:                       dwAoeError = ADSERR_NOERROR;                          break;
    case EC_E_AOE_NO_RTIME:                  dwAoeError = ADSERR_DEVICE_NO_RTIME;                  break;
    case EC_E_AOE_LOCKED_MEMORY:             dwAoeError = ADSERR_DEVICE_LOCKED_MEMORY;             break;
    case EC_E_AOE_MAILBOX:                   dwAoeError = ADSERR_DEVICE_MAILBOX;                   break;
    case EC_E_AOE_WRONG_HMSG:                dwAoeError = ADSERR_DEVICE_WRONG_HMSG;                break;
    case EC_E_AOE_BAD_TASK_ID:               dwAoeError = ADSERR_DEVICE_BAD_TASK_ID;               break;
    case EC_E_AOE_NO_IO:                     dwAoeError = ADSERR_DEVICE_NO_IO;                     break;
    case EC_E_AOE_UNKNOWN_AMS_COMMAND:       dwAoeError = ADSERR_DEVICE_UNKNOWN_AMS_COMMAND;       break;
    case EC_E_AOE_WIN32:                     dwAoeError = ADSERR_DEVICE_WIN32;                     break;
    case EC_E_AOE_LOW_INSTALL_LEVEL:         dwAoeError = ADSERR_DEVICE_LOW_INSTALL_LEVEL;         break;
    case EC_E_AOE_NO_DEBUG:                  dwAoeError = ADSERR_DEVICE_NO_DEBUG;                  break;
    case EC_E_AOE_AMS_SYNC_WIN32:            dwAoeError = ADSERR_DEVICE_AMS_SYNC_WIN32;            break;
    case EC_E_AOE_AMS_SYNC_TIMEOUT:          dwAoeError = ADSERR_DEVICE_AMS_SYNC_TIMEOUT;          break;
    case EC_E_AOE_AMS_SYNC_AMS:              dwAoeError = ADSERR_DEVICE_AMS_SYNC_AMS;              break;
    case EC_E_AOE_AMS_SYNC_NO_INDEX_MAP:     dwAoeError = ADSERR_DEVICE_AMS_SYNC_NO_INDEX_MAP;     break;
    case EC_E_AOE_TCP_SEND:                  dwAoeError = ADSERR_DEVICE_TCP_SEND;                  break;
    case EC_E_AOE_HOST_UNREACHABLE:          dwAoeError = ADSERR_DEVICE_HOST_UNREACHABLE;          break;
    case EC_E_AOE_INVALIDAMSFRAGMENT:        dwAoeError = ADSERR_DEVICE_INVALIDAMSFRAGMENT;        break;
    case EC_E_AOE_NO_LOCKED_MEMORY:          dwAoeError = ADSERR_DEVICE_NO_LOCKED_MEMORY;          break;
    case EC_E_AOE_MAILBOX_FULL:              dwAoeError = ADSERR_DEVICE_MAILBOX_FULL;              break;
    case EC_E_AOE_ERROR:                     dwAoeError = ADSERR_DEVICE_ERROR;                     break;
    case EC_E_AOE_SRVNOTSUPP:                dwAoeError = ADSERR_DEVICE_SRVNOTSUPP;		        break;
    case EC_E_AOE_INVALIDGRP:                dwAoeError = ADSERR_DEVICE_INVALIDGRP; 	            break;
    case EC_E_AOE_INVALIDOFFSET:             dwAoeError = ADSERR_DEVICE_INVALIDOFFSET;	            break;
    case EC_E_AOE_INVALIDACCESS:             dwAoeError = ADSERR_DEVICE_INVALIDACCESS;	            break;
    case EC_E_AOE_INVALIDSIZE:               dwAoeError = ADSERR_DEVICE_INVALIDSIZE;	            break;
    case EC_E_AOE_INVALIDDATA:               dwAoeError = ADSERR_DEVICE_INVALIDDATA;	            break;
    case EC_E_AOE_NOTREADY:                  dwAoeError = ADSERR_DEVICE_NOTREADY;		            break;
    case EC_E_AOE_BUSY:                      dwAoeError = ADSERR_DEVICE_BUSY;		                break;
    case EC_E_AOE_INVALIDCONTEXT:            dwAoeError = ADSERR_DEVICE_INVALIDCONTEXT;	        break;
    case EC_E_AOE_NOMEMORY:                  dwAoeError = ADSERR_DEVICE_NOMEMORY;		            break;
    case EC_E_AOE_INVALIDPARM:               dwAoeError = ADSERR_DEVICE_INVALIDPARM;	            break;
    case EC_E_AOE_NOTFOUND:                  dwAoeError = ADSERR_DEVICE_NOTFOUND;		            break;
    case EC_E_AOE_SYNTAX:                    dwAoeError = ADSERR_DEVICE_SYNTAX;		            break;
    case EC_E_AOE_INCOMPATIBLE:              dwAoeError = ADSERR_DEVICE_INCOMPATIBLE;	            break;
    case EC_E_AOE_EXISTS:                    dwAoeError = ADSERR_DEVICE_EXISTS;		            break;
    case EC_E_AOE_SYMBOLNOTFOUND:            dwAoeError = ADSERR_DEVICE_SYMBOLNOTFOUND;	        break;
    case EC_E_AOE_SYMBOLVERSIONINVALID:      dwAoeError = ADSERR_DEVICE_SYMBOLVERSIONINVALID;      break;
    case EC_E_AOE_INVALIDSTATE:              dwAoeError = ADSERR_DEVICE_INVALIDSTATE;              break;
    case EC_E_AOE_TRANSMODENOTSUPP:          dwAoeError = ADSERR_DEVICE_TRANSMODENOTSUPP;	        break;
    case EC_E_AOE_NOTIFYHNDINVALID:          dwAoeError = ADSERR_DEVICE_NOTIFYHNDINVALID;	        break;
    case EC_E_AOE_CLIENTUNKNOWN:             dwAoeError = ADSERR_DEVICE_CLIENTUNKNOWN;	            break;
    case EC_E_AOE_NOMOREHDLS:                dwAoeError = ADSERR_DEVICE_NOMOREHDLS;	            break;
    case EC_E_AOE_INVALIDWATCHSIZE:          dwAoeError = ADSERR_DEVICE_INVALIDWATCHSIZE;	        break;
    case EC_E_AOE_NOTINIT:                   dwAoeError = ADSERR_DEVICE_NOTINIT;	                break;
    case EC_E_AOE_TIMEOUT:                   dwAoeError = ADSERR_DEVICE_TIMEOUT;	                break;
    case EC_E_AOE_NOINTERFACE:               dwAoeError = ADSERR_DEVICE_NOINTERFACE;	            break;
    case EC_E_AOE_INVALIDINTERFACE:          dwAoeError = ADSERR_DEVICE_INVALIDINTERFACE;	        break;
    case EC_E_AOE_INVALIDCLSID:              dwAoeError = ADSERR_DEVICE_INVALIDCLSID;	            break;
    case EC_E_AOE_INVALIDOBJID:              dwAoeError = ADSERR_DEVICE_INVALIDOBJID;	            break;
    case EC_E_AOE_PENDING:                   dwAoeError = ADSERR_DEVICE_PENDING;	                break;
    case EC_E_AOE_ABORTED:                   dwAoeError = ADSERR_DEVICE_ABORTED;	                break;
    case EC_E_AOE_WARNING:                   dwAoeError = ADSERR_DEVICE_WARNING;	                break;
    case EC_E_AOE_INVALIDARRAYIDX:           dwAoeError = ADSERR_DEVICE_INVALIDARRAYIDX;	        break;
    case EC_E_AOE_SYMBOLNOTACTIVE:           dwAoeError = ADSERR_DEVICE_SYMBOLNOTACTIVE;	        break;
    case EC_E_AOE_ACCESSDENIED:              dwAoeError = ADSERR_DEVICE_ACCESSDENIED;	            break;
    case EC_E_AOE_INTERNAL:                  dwAoeError = ADSERR_DEVICE_INTERNAL;                  break;
    case EC_E_AOE_TARGET_PORT_NOT_FOUND:     dwAoeError = ADSERR_DEVICE_TARGET_PORT_NOT_FOUND;     break;
    case EC_E_AOE_TARGET_MACHINE_NOT_FOUND:  dwAoeError = ADSERR_DEVICE_TARGET_MACHINE_NOT_FOUND;  break;
    case EC_E_AOE_UNKNOWN_CMD_ID:            dwAoeError = ADSERR_DEVICE_UNKNOWN_CMD_ID;            break;
    case EC_E_AOE_PORT_NOT_CONNECTED:        dwAoeError = ADSERR_DEVICE_PORT_NOT_CONNECTED;        break;
    case EC_E_AOE_INVALID_AMS_LENGTH:        dwAoeError = ADSERR_DEVICE_INVALID_AMS_LENGTH;        break;
    case EC_E_AOE_INVALID_AMS_ID:            dwAoeError = ADSERR_DEVICE_INVALID_AMS_ID;            break;
    case EC_E_AOE_PORT_DISABLED:             dwAoeError = ADSERR_DEVICE_PORT_DISABLED;             break;
    case EC_E_AOE_PORT_CONNECTED:            dwAoeError = ADSERR_DEVICE_PORT_CONNECTED;            break;
    case EC_E_AOE_INVALID_AMS_PORT:          dwAoeError = ADSERR_DEVICE_INVALID_AMS_PORT;          break;
    case EC_E_AOE_NO_MEMORY:                 dwAoeError = ADSERR_DEVICE_NO_MEMORY;                 break;
    case EC_E_AOE_VENDOR_SPECIFIC:           dwAoeError = ADSERR_DEVICE_INTERNAL;                  break;
    default:                                 dwAoeError = ADSERR_DEVICE_INTERNAL;                  break;
    }
#else
    EC_UNREFPARM(dwErrorCode);
	dwAoeError = ADSERR_DEVICE_INTERNAL;
#endif
     return dwAoeError;
}

EC_T_DWORD EcConvertFoeErrorToEcError(EC_T_DWORD dwFoeError)
{
    EC_T_DWORD dwErrorCode;
#if (defined INCLUDE_FOE_SUPPORT)
    switch (dwFoeError)
    {
    case ECAT_FOE_ERRCODE_NOTDEFINED:        dwErrorCode = EC_E_FOE_ERRCODE_NOTDEFINED;              break;
    case ECAT_FOE_ERRCODE_NOTFOUND:          dwErrorCode = EC_E_FOE_ERRCODE_NOTFOUND;                break;
    case ECAT_FOE_ERRCODE_ACCESS:            dwErrorCode = EC_E_FOE_ERRCODE_ACCESS;                  break;
    case ECAT_FOE_ERRCODE_DISKFULL:          dwErrorCode = EC_E_FOE_ERRCODE_DISKFULL;                break;
    case ECAT_FOE_ERRCODE_ILLEGAL:           dwErrorCode = EC_E_FOE_ERRCODE_ILLEGAL;                 break;
    case ECAT_FOE_ERRCODE_PACKENO:           dwErrorCode = EC_E_FOE_ERRCODE_PACKENO;                 break;
    case ECAT_FOE_ERRCODE_EXISTS:            dwErrorCode = EC_E_FOE_ERRCODE_EXISTS;                  break;
    case ECAT_FOE_ERRCODE_NOUSER:            dwErrorCode = EC_E_FOE_ERRCODE_NOUSER;                  break;
    case ECAT_FOE_ERRCODE_BOOTSTRAPONLY:     dwErrorCode = EC_E_FOE_ERRCODE_BOOTSTRAPONLY;           break;
    case ECAT_FOE_ERRCODE_NOTINBOOTSTRAP:    dwErrorCode = EC_E_FOE_ERRCODE_NOTINBOOTSTRAP;          break;
    case ECAT_FOE_ERRCODE_NORIGHTS:          dwErrorCode = EC_E_FOE_ERRCODE_INVALIDPASSWORD;         break;
    case ECAT_FOE_ERRCODE_PROGERROR:         dwErrorCode = EC_E_FOE_ERRCODE_PROGERROR;               break;
    case ECAT_FOE_ERRCODE_INVALID_CHECKSUM:  dwErrorCode = EC_E_FOE_ERRCODE_INVALID_CHECKSUM;        break;
    case ECAT_FOE_ERRCODE_INVALID_FIRMWARE:  dwErrorCode = EC_E_FOE_ERRCODE_INVALID_FIRMWARE;        break;
    case ECAT_FOE_ERRCODE_NO_FILE:           dwErrorCode = EC_E_FOE_ERRCODE_NO_FILE;                 break;
    case ECAT_FOE_ERRCODE_FILE_HEAD_MISSING: dwErrorCode = EC_E_FOE_ERRCODE_FILE_HEAD_MISSING;       break;
    case ECAT_FOE_ERRCODE_FLASH_PROBLEM:     dwErrorCode = EC_E_FOE_ERRCODE_FLASH_PROBLEM;           break;
    case ECAT_FOE_ERRCODE_FILE_INCOMPATIBLE: dwErrorCode = EC_E_FOE_ERRCODE_FILE_INCOMPATIBLE;       break;
    default:                                 dwErrorCode = EC_E_FOE_ERRCODE_NOTDEFINED;              break;
    }
#else
    EC_UNREFPARM(dwFoeError);
    dwErrorCode = EC_E_FOE_ERRCODE_NOTDEFINED;
#endif
    return dwErrorCode;
}

EC_T_DWORD EcConvertEcErrorToFoeError(EC_T_DWORD dwErrorCode)
{
    EC_T_DWORD dwFoeError;
#if (defined INCLUDE_FOE_SUPPORT)
    switch (dwErrorCode)
    {
    case EC_E_FOE_ERRCODE_NOTDEFINED:          dwFoeError = ECAT_FOE_ERRCODE_NOTDEFINED;             break;
    case EC_E_FOE_ERRCODE_NOTFOUND:            dwFoeError = ECAT_FOE_ERRCODE_NOTFOUND;               break;
    case EC_E_FOE_ERRCODE_ACCESS:              dwFoeError = ECAT_FOE_ERRCODE_ACCESS;                 break;
    case EC_E_FOE_ERRCODE_DISKFULL:            dwFoeError = ECAT_FOE_ERRCODE_DISKFULL;               break;
    case EC_E_FOE_ERRCODE_ILLEGAL:             dwFoeError = ECAT_FOE_ERRCODE_ILLEGAL;                break;
    case EC_E_FOE_ERRCODE_PACKENO:             dwFoeError = ECAT_FOE_ERRCODE_PACKENO;                break;
    case EC_E_FOE_ERRCODE_EXISTS:              dwFoeError = ECAT_FOE_ERRCODE_EXISTS;                 break;
    case EC_E_FOE_ERRCODE_NOUSER:              dwFoeError = ECAT_FOE_ERRCODE_NOUSER;                 break;
    case EC_E_FOE_ERRCODE_BOOTSTRAPONLY:       dwFoeError = ECAT_FOE_ERRCODE_BOOTSTRAPONLY;          break;
    case EC_E_FOE_ERRCODE_NOTINBOOTSTRAP:      dwFoeError = ECAT_FOE_ERRCODE_NOTINBOOTSTRAP;         break;
    case EC_E_FOE_ERRCODE_INVALIDPASSWORD:     dwFoeError = ECAT_FOE_ERRCODE_NORIGHTS;               break;
    case EC_E_FOE_ERRCODE_PROGERROR:           dwFoeError = ECAT_FOE_ERRCODE_PROGERROR;              break;
    case EC_E_FOE_ERRCODE_INVALID_CHECKSUM:    dwFoeError = ECAT_FOE_ERRCODE_INVALID_CHECKSUM;       break;
    case EC_E_FOE_ERRCODE_INVALID_FIRMWARE:    dwFoeError = ECAT_FOE_ERRCODE_INVALID_FIRMWARE;       break;
    case EC_E_FOE_ERRCODE_NO_FILE:             dwFoeError = ECAT_FOE_ERRCODE_NO_FILE;                break;
    case EC_E_FOE_ERRCODE_FILE_HEAD_MISSING:   dwFoeError = ECAT_FOE_ERRCODE_FILE_HEAD_MISSING;      break;
    case EC_E_FOE_ERRCODE_FLASH_PROBLEM:       dwFoeError = ECAT_FOE_ERRCODE_FLASH_PROBLEM;          break;
    case EC_E_FOE_ERRCODE_FILE_INCOMPATIBLE:   dwFoeError = ECAT_FOE_ERRCODE_FILE_INCOMPATIBLE;      break;
    default:                                   dwFoeError = ECAT_FOE_ERRCODE_NOTDEFINED;             break;
    }
#else
    EC_UNREFPARM(dwErrorCode);
    dwFoeError = EC_E_FOE_ERRCODE_NOTDEFINED;
#endif
    return dwFoeError;
}

EC_T_DWORD EcConvertSoeErrorToEcError(EC_T_DWORD dwSoeError)
{
	EC_T_DWORD dwErrorCode;
#if (defined INCLUDE_SOE_SUPPORT)
    switch (dwSoeError)
    {
    case ECAT_SOE_ERRCODE_INVALID_ACCESS:   dwErrorCode = EC_E_SOE_ERRORCODE_INVALID_ACCESS;      break;
    case ECAT_SOE_ERRCODE_NOT_EXIST:        dwErrorCode = EC_E_SOE_ERRORCODE_NOT_EXIST;           break;
    case ECAT_SOE_ERRCODE_INVL_ACC_ELEM1:   dwErrorCode = EC_E_SOE_ERRORCODE_INVL_ACC_ELEM1;      break;
    case ECAT_SOE_ERRCODE_NAME_NOT_EXIST:   dwErrorCode = EC_E_SOE_ERRORCODE_NAME_NOT_EXIST;      break;
    case ECAT_SOE_ERRCODE_NAME_UNDERSIZE:   dwErrorCode = EC_E_SOE_ERRORCODE_NAME_UNDERSIZE;      break;
    case ECAT_SOE_ERRCODE_NAME_OVERSIZE:    dwErrorCode = EC_E_SOE_ERRORCODE_NAME_OVERSIZE;       break;
    case ECAT_SOE_ERRCODE_NAME_UNCHANGE:    dwErrorCode = EC_E_SOE_ERRORCODE_NAME_UNCHANGE;       break;
    case ECAT_SOE_ERRCODE_NAME_WR_PROT:     dwErrorCode = EC_E_SOE_ERRORCODE_NAME_WR_PROT;        break;
    case ECAT_SOE_ERRCODE_UNDERS_TRANS:     dwErrorCode = EC_E_SOE_ERRORCODE_UNDERS_TRANS;        break;
    case ECAT_SOE_ERRCODE_OVERS_TRANS:      dwErrorCode = EC_E_SOE_ERRORCODE_OVERS_TRANS;         break;
    case ECAT_SOE_ERRCODE_ATTR_UNCHANGE:    dwErrorCode = EC_E_SOE_ERRORCODE_ATTR_UNCHANGE;       break;
    case ECAT_SOE_ERRCODE_ATTR_WR_PROT:     dwErrorCode = EC_E_SOE_ERRORCODE_ATTR_WR_PROT;        break;
    case ECAT_SOE_ERRCODE_UNIT_NOT_EXIST:   dwErrorCode = EC_E_SOE_ERRORCODE_UNIT_NOT_EXIST;      break;
    case ECAT_SOE_ERRCODE_UNIT_UNDERSIZE:   dwErrorCode = EC_E_SOE_ERRORCODE_UNIT_UNDERSIZE;      break;
    case ECAT_SOE_ERRCODE_UNIT_OVERSIZE:    dwErrorCode = EC_E_SOE_ERRORCODE_UNIT_OVERSIZE;       break;
    case ECAT_SOE_ERRCODE_UNIT_UNCHANGE:    dwErrorCode = EC_E_SOE_ERRORCODE_UNIT_UNCHANGE;       break;
    case ECAT_SOE_ERRCODE_UNIT_WR_PROT:     dwErrorCode = EC_E_SOE_ERRORCODE_UNIT_WR_PROT;        break;
    case ECAT_SOE_ERRCODE_MIN_NOT_EXIST:    dwErrorCode = EC_E_SOE_ERRORCODE_MIN_NOT_EXIST;       break;
    case ECAT_SOE_ERRCODE_MIN_UNDERSIZE:    dwErrorCode = EC_E_SOE_ERRORCODE_MIN_UNDERSIZE;       break;
    case ECAT_SOE_ERRCODE_MIN_OVERSIZE:     dwErrorCode = EC_E_SOE_ERRORCODE_MIN_OVERSIZE;        break;
    case ECAT_SOE_ERRCODE_MIN_UNCHANGE:     dwErrorCode = EC_E_SOE_ERRORCODE_MIN_UNCHANGE;        break;
    case ECAT_SOE_ERRCODE_MIN_WR_PROT:      dwErrorCode = EC_E_SOE_ERRORCODE_MIN_WR_PROT;         break;
    case ECAT_SOE_ERRCODE_MAX_NOT_EXIST:    dwErrorCode = EC_E_SOE_ERRORCODE_MAX_NOT_EXIST;       break;
    case ECAT_SOE_ERRCODE_MAX_UNDERSIZE:    dwErrorCode = EC_E_SOE_ERRORCODE_MAX_UNDERSIZE;       break;
    case ECAT_SOE_ERRCODE_MAX_OVERSIZE:     dwErrorCode = EC_E_SOE_ERRORCODE_MAX_OVERSIZE;        break;
    case ECAT_SOE_ERRCODE_MAX_UNCHANGE:     dwErrorCode = EC_E_SOE_ERRORCODE_MAX_UNCHANGE;        break;
    case ECAT_SOE_ERRCODE_MAX_WR_PROT:      dwErrorCode = EC_E_SOE_ERRORCODE_MAX_WR_PROT;         break;
    case ECAT_SOE_ERRCODE_DATA_NOT_EXIST:   dwErrorCode = EC_E_SOE_ERRORCODE_DATA_NOT_EXIST;      break;
    case ECAT_SOE_ERRCODE_DATA_UNDERSIZE:   dwErrorCode = EC_E_SOE_ERRORCODE_DATA_UNDERSIZE;      break;
    case ECAT_SOE_ERRCODE_DATA_OVERSIZE:    dwErrorCode = EC_E_SOE_ERRORCODE_DATA_OVERSIZE;       break;
    case ECAT_SOE_ERRCODE_DATA_UNCHANGE:    dwErrorCode = EC_E_SOE_ERRORCODE_DATA_UNCHANGE;       break;
    case ECAT_SOE_ERRCODE_DATA_WR_PROT:     dwErrorCode = EC_E_SOE_ERRORCODE_DATA_WR_PROT;        break;
    case ECAT_SOE_ERRCODE_DATA_MIN_LIMIT:   dwErrorCode = EC_E_SOE_ERRORCODE_DATA_MIN_LIMIT;      break;
    case ECAT_SOE_ERRCODE_DATA_MAX_LIMIT:   dwErrorCode = EC_E_SOE_ERRORCODE_DATA_MAX_LIMIT;      break;
    case ECAT_SOE_ERRCODE_DATA_INCOR:       dwErrorCode = EC_E_SOE_ERRORCODE_DATA_INCOR;          break;
    case ECAT_SOE_ERRCODE_PASWD_PROT:       dwErrorCode = EC_E_SOE_ERRORCODE_PASWD_PROT;          break;
    case ECAT_SOE_ERRCODE_TEMP_UNCHANGE:    dwErrorCode = EC_E_SOE_ERRORCODE_TEMP_UNCHANGE;       break;
    case ECAT_SOE_ERRCODE_INVL_INDIRECT:    dwErrorCode = EC_E_SOE_ERRORCODE_INVL_INDIRECT;       break;
    case ECAT_SOE_ERRCODE_TEMP_UNCHANGE1:   dwErrorCode = EC_E_SOE_ERRORCODE_TEMP_UNCHANGE1;      break;
    case ECAT_SOE_ERRCODE_ALREADY_ACTIVE:   dwErrorCode = EC_E_SOE_ERRORCODE_ALREADY_ACTIVE;      break;
    case ECAT_SOE_ERRCODE_NOT_INTERRUPT:    dwErrorCode = EC_E_SOE_ERRORCODE_NOT_INTERRUPT;       break;
    case ECAT_SOE_ERRCODE_CMD_NOT_AVAIL:    dwErrorCode = EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL;       break;
    case ECAT_SOE_ERRCODE_CMD_NOT_AVAIL1:   dwErrorCode = EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL1;      break;
    case ECAT_SOE_ERRCODE_NO_DATA:          dwErrorCode = EC_E_SOE_ERRORCODE_NO_DATA;             break;
    case ECAT_SOE_ERRCODE_NO_DEFAULT_VALUE: dwErrorCode = EC_E_SOE_ERRORCODE_NO_DEFAULT_VALUE;    break;
    case ECAT_SOE_ERRCODE_DEFAULT_LONG:     dwErrorCode = EC_E_SOE_ERRORCODE_DEFAULT_LONG;        break;
    case ECAT_SOE_ERRCODE_DEFAULT_WP:       dwErrorCode = EC_E_SOE_ERRORCODE_DEFAULT_WP;          break;
    case ECAT_SOE_ERRCODE_INVL_DRIVE_NO:    dwErrorCode = EC_E_SOE_ERRORCODE_INVL_DRIVE_NO;       break;
    case ECAT_SOE_ERRCODE_GENERAL_ERROR:    dwErrorCode = EC_E_SOE_ERRORCODE_GENERAL_ERROR;       break;
    case ECAT_SOE_ERRCODE_NO_ELEM_ADR:      dwErrorCode = EC_E_SOE_ERRCODE_NO_ELEM_ADR;           break;
    default:                                dwErrorCode = EC_E_SOE_ERRORCODE_NOT_EXIST;           break;
    }
#else
    EC_UNREFPARM(dwSoeError);
    dwErrorCode = EC_E_SOE_ERRORCODE_NOT_EXIST;
#endif
    return dwErrorCode;
}

EC_T_DWORD EcConvertEcErrorToSoeError(EC_T_DWORD dwErrorCode)
{
	EC_T_DWORD dwSoeError;
#if (defined INCLUDE_SOE_SUPPORT)
    switch (dwErrorCode)
    {
    case EC_E_SOE_ERRORCODE_INVALID_ACCESS:      dwSoeError = ECAT_SOE_ERRCODE_INVALID_ACCESS;    break;
    case EC_E_SOE_ERRORCODE_NOT_EXIST:           dwSoeError = ECAT_SOE_ERRCODE_NOT_EXIST;         break;
    case EC_E_SOE_ERRORCODE_INVL_ACC_ELEM1:      dwSoeError = ECAT_SOE_ERRCODE_INVL_ACC_ELEM1;    break;
    case EC_E_SOE_ERRORCODE_NAME_NOT_EXIST:      dwSoeError = ECAT_SOE_ERRCODE_NAME_NOT_EXIST;    break;
    case EC_E_SOE_ERRORCODE_NAME_UNDERSIZE:      dwSoeError = ECAT_SOE_ERRCODE_NAME_UNDERSIZE;    break;
    case EC_E_SOE_ERRORCODE_NAME_OVERSIZE:       dwSoeError = ECAT_SOE_ERRCODE_NAME_OVERSIZE;     break;
    case EC_E_SOE_ERRORCODE_NAME_UNCHANGE:       dwSoeError = ECAT_SOE_ERRCODE_NAME_UNCHANGE;     break;
    case EC_E_SOE_ERRORCODE_NAME_WR_PROT:        dwSoeError = ECAT_SOE_ERRCODE_NAME_WR_PROT;      break;
    case EC_E_SOE_ERRORCODE_UNDERS_TRANS:        dwSoeError = ECAT_SOE_ERRCODE_UNDERS_TRANS;      break;
    case EC_E_SOE_ERRORCODE_OVERS_TRANS:         dwSoeError = ECAT_SOE_ERRCODE_OVERS_TRANS;       break;
    case EC_E_SOE_ERRORCODE_ATTR_UNCHANGE:       dwSoeError = ECAT_SOE_ERRCODE_ATTR_UNCHANGE;     break;
    case EC_E_SOE_ERRORCODE_ATTR_WR_PROT:        dwSoeError = ECAT_SOE_ERRCODE_ATTR_WR_PROT;      break;
    case EC_E_SOE_ERRORCODE_UNIT_NOT_EXIST:      dwSoeError = ECAT_SOE_ERRCODE_UNIT_NOT_EXIST;    break;
    case EC_E_SOE_ERRORCODE_UNIT_UNDERSIZE:      dwSoeError = ECAT_SOE_ERRCODE_UNIT_UNDERSIZE;    break;
    case EC_E_SOE_ERRORCODE_UNIT_OVERSIZE:       dwSoeError = ECAT_SOE_ERRCODE_UNIT_OVERSIZE;     break;
    case EC_E_SOE_ERRORCODE_UNIT_UNCHANGE:       dwSoeError = ECAT_SOE_ERRCODE_UNIT_UNCHANGE;     break;
    case EC_E_SOE_ERRORCODE_UNIT_WR_PROT:        dwSoeError = ECAT_SOE_ERRCODE_UNIT_WR_PROT;      break;
    case EC_E_SOE_ERRORCODE_MIN_NOT_EXIST:       dwSoeError = ECAT_SOE_ERRCODE_MIN_NOT_EXIST;     break;
    case EC_E_SOE_ERRORCODE_MIN_UNDERSIZE:       dwSoeError = ECAT_SOE_ERRCODE_MIN_UNDERSIZE;     break;
    case EC_E_SOE_ERRORCODE_MIN_OVERSIZE:        dwSoeError = ECAT_SOE_ERRCODE_MIN_OVERSIZE;      break;
    case EC_E_SOE_ERRORCODE_MIN_UNCHANGE:        dwSoeError = ECAT_SOE_ERRCODE_MIN_UNCHANGE;      break;
    case EC_E_SOE_ERRORCODE_MIN_WR_PROT:         dwSoeError = ECAT_SOE_ERRCODE_MIN_WR_PROT;       break;
    case EC_E_SOE_ERRORCODE_MAX_NOT_EXIST:       dwSoeError = ECAT_SOE_ERRCODE_MAX_NOT_EXIST;     break;
    case EC_E_SOE_ERRORCODE_MAX_UNDERSIZE:       dwSoeError = ECAT_SOE_ERRCODE_MAX_UNDERSIZE;     break;
    case EC_E_SOE_ERRORCODE_MAX_OVERSIZE:        dwSoeError = ECAT_SOE_ERRCODE_MAX_OVERSIZE;      break;
    case EC_E_SOE_ERRORCODE_MAX_UNCHANGE:        dwSoeError = ECAT_SOE_ERRCODE_MAX_UNCHANGE;      break;
    case EC_E_SOE_ERRORCODE_MAX_WR_PROT:         dwSoeError = ECAT_SOE_ERRCODE_MAX_WR_PROT;       break;
    case EC_E_SOE_ERRORCODE_DATA_NOT_EXIST:      dwSoeError = ECAT_SOE_ERRCODE_DATA_NOT_EXIST;    break;
    case EC_E_SOE_ERRORCODE_DATA_UNDERSIZE:      dwSoeError = ECAT_SOE_ERRCODE_DATA_UNDERSIZE;    break;
    case EC_E_SOE_ERRORCODE_DATA_OVERSIZE:       dwSoeError = ECAT_SOE_ERRCODE_DATA_OVERSIZE;     break;
    case EC_E_SOE_ERRORCODE_DATA_UNCHANGE:       dwSoeError = ECAT_SOE_ERRCODE_DATA_UNCHANGE;     break;
    case EC_E_SOE_ERRORCODE_DATA_WR_PROT:        dwSoeError = ECAT_SOE_ERRCODE_DATA_WR_PROT;      break;
    case EC_E_SOE_ERRORCODE_DATA_MIN_LIMIT:      dwSoeError = ECAT_SOE_ERRCODE_DATA_MIN_LIMIT;    break;
    case EC_E_SOE_ERRORCODE_DATA_MAX_LIMIT:      dwSoeError = ECAT_SOE_ERRCODE_DATA_MAX_LIMIT;    break;
    case EC_E_SOE_ERRORCODE_DATA_INCOR:          dwSoeError = ECAT_SOE_ERRCODE_DATA_INCOR;        break;
    case EC_E_SOE_ERRORCODE_PASWD_PROT:          dwSoeError = ECAT_SOE_ERRCODE_PASWD_PROT;        break;
    case EC_E_SOE_ERRORCODE_TEMP_UNCHANGE:       dwSoeError = ECAT_SOE_ERRCODE_TEMP_UNCHANGE;     break;
    case EC_E_SOE_ERRORCODE_INVL_INDIRECT:       dwSoeError = ECAT_SOE_ERRCODE_INVL_INDIRECT;     break;
    case EC_E_SOE_ERRORCODE_TEMP_UNCHANGE1:      dwSoeError = ECAT_SOE_ERRCODE_TEMP_UNCHANGE1;    break;
    case EC_E_SOE_ERRORCODE_ALREADY_ACTIVE:      dwSoeError = ECAT_SOE_ERRCODE_ALREADY_ACTIVE;    break;
    case EC_E_SOE_ERRORCODE_NOT_INTERRUPT:       dwSoeError = ECAT_SOE_ERRCODE_NOT_INTERRUPT;     break;
    case EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL:       dwSoeError = ECAT_SOE_ERRCODE_CMD_NOT_AVAIL;     break;
    case EC_E_SOE_ERRORCODE_CMD_NOT_AVAIL1:      dwSoeError = ECAT_SOE_ERRCODE_CMD_NOT_AVAIL1;    break;
    case EC_E_SOE_ERRORCODE_NO_DATA:             dwSoeError = ECAT_SOE_ERRCODE_NO_DATA;           break;
    case EC_E_SOE_ERRORCODE_NO_DEFAULT_VALUE:    dwSoeError = ECAT_SOE_ERRCODE_NO_DEFAULT_VALUE;  break;
    case EC_E_SOE_ERRORCODE_DEFAULT_LONG:        dwSoeError = ECAT_SOE_ERRCODE_DEFAULT_LONG;      break;
    case EC_E_SOE_ERRORCODE_DEFAULT_WP:          dwSoeError = ECAT_SOE_ERRCODE_DEFAULT_WP;        break;
    case EC_E_SOE_ERRORCODE_INVL_DRIVE_NO:       dwSoeError = ECAT_SOE_ERRCODE_INVL_DRIVE_NO;     break;
    case EC_E_SOE_ERRORCODE_GENERAL_ERROR:       dwSoeError = ECAT_SOE_ERRCODE_GENERAL_ERROR;     break;
    case EC_E_SOE_ERRCODE_NO_ELEM_ADR:           dwSoeError = ECAT_SOE_ERRCODE_NO_ELEM_ADR;       break;
    default:                                     dwSoeError = ECAT_SOE_ERRCODE_NAME_NOT_EXIST;    break;
    }
#else
    EC_UNREFPARM(dwErrorCode);
    dwSoeError = ECAT_SOE_ERRCODE_NAME_NOT_EXIST;
#endif
    return dwSoeError;
}

EC_T_DWORD EcConvertCoeErrorToEcError(EC_T_DWORD dwCoeError)
{
    EC_T_DWORD dwErrorCode;
    
    switch (dwCoeError)
    {
    case SDO_ABORTCODE_TOGGLE:          dwErrorCode = EC_E_SDO_ABORTCODE_TOGGLE;          break;
    case SDO_ABORTCODE_TIMEOUT:         dwErrorCode = EC_E_SDO_ABORTCODE_TIMEOUT;         break;
    case SDO_ABORTCODE_CCS_SCS:         dwErrorCode = EC_E_SDO_ABORTCODE_CCS_SCS;         break;
    case SDO_ABORTCODE_BLK_SIZE:        dwErrorCode = EC_E_SDO_ABORTCODE_BLK_SIZE;        break;
    case SDO_ABORTCODE_SEQNO:           dwErrorCode = EC_E_SDO_ABORTCODE_SEQNO;           break;
    case SDO_ABORTCODE_CRC:             dwErrorCode = EC_E_SDO_ABORTCODE_CRC;             break;
    case SDO_ABORTCODE_MEMORY:          dwErrorCode = EC_E_SDO_ABORTCODE_MEMORY;          break;
    case SDO_ABORTCODE_ACCESS:          dwErrorCode = EC_E_SDO_ABORTCODE_ACCESS;          break;
    case SDO_ABORTCODE_WRITEONLY:       dwErrorCode = EC_E_SDO_ABORTCODE_WRITEONLY;       break;
    case SDO_ABORTCODE_READONLY:        dwErrorCode = EC_E_SDO_ABORTCODE_READONLY;        break;
    case SDO_ABORTCODE_SI_NOT_WRITTEN:  dwErrorCode = EC_E_SDO_ABORTCODE_SI_NOT_WRITTEN;  break;
    case SDO_ABORTCODE_CA_TYPE_MISM:    dwErrorCode = EC_E_SDO_ABORTCODE_CA_TYPE_MISM;    break;
    case SDO_ABORTCODE_OBJ_TOO_BIG:     dwErrorCode = EC_E_SDO_ABORTCODE_OBJ_TOO_BIG;     break;
    case SDO_ABORTCODE_PDO_MAPPED:      dwErrorCode = EC_E_SDO_ABORTCODE_PDO_MAPPED;      break;
    case SDO_ABORTCODE_INDEX:           dwErrorCode = EC_E_SDO_ABORTCODE_INDEX;           break;
    case SDO_ABORTCODE_PDO_MAP:         dwErrorCode = EC_E_SDO_ABORTCODE_PDO_MAP;         break;
    case SDO_ABORTCODE_PDO_LEN:         dwErrorCode = EC_E_SDO_ABORTCODE_PDO_LEN;         break;
    case SDO_ABORTCODE_P_INCOMP:        dwErrorCode = EC_E_SDO_ABORTCODE_P_INCOMP;        break;
    case SDO_ABORTCODE_I_INCOMP:        dwErrorCode = EC_E_SDO_ABORTCODE_I_INCOMP;        break;
    case SDO_ABORTCODE_HARDWARE:        dwErrorCode = EC_E_SDO_ABORTCODE_HARDWARE;        break;
    case SDO_ABORTCODE_DATA_SIZE:       dwErrorCode = EC_E_SDO_ABORTCODE_DATA_SIZE;       break;
    case SDO_ABORTCODE_DATA_SIZE1:      dwErrorCode = EC_E_SDO_ABORTCODE_DATA_SIZE1;      break;
    case SDO_ABORTCODE_DATA_SIZE2:      dwErrorCode = EC_E_SDO_ABORTCODE_DATA_SIZE2;      break;
    case SDO_ABORTCODE_OFFSET:          dwErrorCode = EC_E_SDO_ABORTCODE_OFFSET;          break;
    case SDO_ABORTCODE_DATA_RANGE:      dwErrorCode = EC_E_SDO_ABORTCODE_DATA_RANGE;      break;
    case SDO_ABORTCODE_DATA_RANGE1:     dwErrorCode = EC_E_SDO_ABORTCODE_DATA_RANGE1;     break;
    case SDO_ABORTCODE_DATA_RANGE2:     dwErrorCode = EC_E_SDO_ABORTCODE_DATA_RANGE2;     break;
    case SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH:    dwErrorCode = EC_E_SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH;  break;
    case SDO_ABORTCODE_MINMAX:          dwErrorCode = EC_E_SDO_ABORTCODE_MINMAX;          break;
    case SDO_ABORTCODE_GENERAL:         dwErrorCode = EC_E_SDO_ABORTCODE_GENERAL;         break;
    case SDO_ABORTCODE_TRANSFER:        dwErrorCode = EC_E_SDO_ABORTCODE_TRANSFER;        break;
    case SDO_ABORTCODE_TRANSFER1:       dwErrorCode = EC_E_SDO_ABORTCODE_TRANSFER1;       break;
    case SDO_ABORTCODE_TRANSFER2:       dwErrorCode = EC_E_SDO_ABORTCODE_TRANSFER2;       break;
    case SDO_ABORTCODE_DICTIONARY:      dwErrorCode = EC_E_SDO_ABORTCODE_DICTIONARY;      break;
    default:                            dwErrorCode = EC_E_SDO_ABORTCODE_UNKNOWN;         break;
    }
    return dwErrorCode;
}

EC_T_DWORD EcConvertEcErrorToCoeError(EC_T_DWORD dwErrorCode)
{
    EC_T_DWORD dwCoeError;
    
    switch (dwErrorCode)
    {
    case EC_E_SDO_ABORTCODE_TOGGLE:         dwCoeError = SDO_ABORTCODE_TOGGLE;         break;
    case EC_E_SDO_ABORTCODE_TIMEOUT:        dwCoeError = SDO_ABORTCODE_TIMEOUT;        break;
    case EC_E_SDO_ABORTCODE_CCS_SCS:        dwCoeError = SDO_ABORTCODE_CCS_SCS;        break;
    case EC_E_SDO_ABORTCODE_BLK_SIZE:       dwCoeError = SDO_ABORTCODE_BLK_SIZE;       break;
    case EC_E_SDO_ABORTCODE_SEQNO:          dwCoeError = SDO_ABORTCODE_SEQNO;          break;
    case EC_E_SDO_ABORTCODE_CRC:            dwCoeError = SDO_ABORTCODE_CRC;            break;
    case EC_E_SDO_ABORTCODE_MEMORY:         dwCoeError = SDO_ABORTCODE_MEMORY;         break;
    case EC_E_SDO_ABORTCODE_ACCESS:         dwCoeError = SDO_ABORTCODE_ACCESS;         break;
    case EC_E_SDO_ABORTCODE_WRITEONLY:      dwCoeError = SDO_ABORTCODE_WRITEONLY;      break;
    case EC_E_SDO_ABORTCODE_READONLY:       dwCoeError = SDO_ABORTCODE_READONLY;       break;
    case EC_E_SDO_ABORTCODE_SI_NOT_WRITTEN: dwCoeError = SDO_ABORTCODE_SI_NOT_WRITTEN; break;
    case EC_E_SDO_ABORTCODE_CA_TYPE_MISM:   dwCoeError = SDO_ABORTCODE_CA_TYPE_MISM;   break;
    case EC_E_SDO_ABORTCODE_OBJ_TOO_BIG:    dwCoeError = SDO_ABORTCODE_OBJ_TOO_BIG;    break;
    case EC_E_SDO_ABORTCODE_PDO_MAPPED:     dwCoeError = SDO_ABORTCODE_PDO_MAPPED;     break;
    case EC_E_SDO_ABORTCODE_INDEX:          dwCoeError = SDO_ABORTCODE_INDEX;          break;
    case EC_E_SDO_ABORTCODE_PDO_MAP:        dwCoeError = SDO_ABORTCODE_PDO_MAP;        break;
    case EC_E_SDO_ABORTCODE_PDO_LEN:        dwCoeError = SDO_ABORTCODE_PDO_LEN;        break;
    case EC_E_SDO_ABORTCODE_P_INCOMP:       dwCoeError = SDO_ABORTCODE_P_INCOMP;       break;
    case EC_E_SDO_ABORTCODE_I_INCOMP:       dwCoeError = SDO_ABORTCODE_I_INCOMP;       break;
    case EC_E_SDO_ABORTCODE_HARDWARE:       dwCoeError = SDO_ABORTCODE_HARDWARE;       break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE:      dwCoeError = SDO_ABORTCODE_DATA_SIZE;      break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE1:     dwCoeError = SDO_ABORTCODE_DATA_SIZE1;     break;
    case EC_E_SDO_ABORTCODE_DATA_SIZE2:     dwCoeError = SDO_ABORTCODE_DATA_SIZE2;     break;
    case EC_E_SDO_ABORTCODE_OFFSET:         dwCoeError = SDO_ABORTCODE_OFFSET;         break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE:     dwCoeError = SDO_ABORTCODE_DATA_RANGE;     break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE1:    dwCoeError = SDO_ABORTCODE_DATA_RANGE1;    break;
    case EC_E_SDO_ABORTCODE_DATA_RANGE2:    dwCoeError = SDO_ABORTCODE_DATA_RANGE2;    break;
    case EC_E_SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH: dwCoeError = SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH; break;
    case EC_E_SDO_ABORTCODE_MINMAX:         dwCoeError = SDO_ABORTCODE_MINMAX;         break;
    case EC_E_SDO_ABORTCODE_GENERAL:        dwCoeError = SDO_ABORTCODE_GENERAL;        break;
    case EC_E_SDO_ABORTCODE_TRANSFER:       dwCoeError = SDO_ABORTCODE_TRANSFER;       break;
    case EC_E_SDO_ABORTCODE_TRANSFER1:      dwCoeError = SDO_ABORTCODE_TRANSFER1;      break;
    case EC_E_SDO_ABORTCODE_TRANSFER2:      dwCoeError = SDO_ABORTCODE_TRANSFER2;      break;
    case EC_E_SDO_ABORTCODE_DICTIONARY:     dwCoeError = SDO_ABORTCODE_DICTIONARY;     break;
    default:                                dwCoeError = SDO_ABORTCODE_GENERAL;        break;
    }
    return dwCoeError;
}
