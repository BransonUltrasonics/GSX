/*-----------------------------------------------------------------------------
 * EcEEPRom.cpp             file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2008/7/25::12:25
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcCommon.h"
#include "EcCommonPrivate.h"

#include "EthernetServices.h"
#include "EcTimer.h"
#include "EcEscReg.h"
#include "EcEEPRom.h"

#include "EcMasterCommon.h"
#include "EcLink.h"

#include "EcInterfaceCommon.h"
#include "EcInterface.h"
#include "EcEthSwitch.h"
#include "EcServices.h"
#include "EcIoImage.h"
#include "EcFiFo.h"
#include "EcDevice.h"

#include "EcMaster.h"

#include "EcInvokeId.h"
#include "EcConfig.h"
#include "AtEthercatPrivate.h"

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_poEthDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_poEthDevice->LinkDbgMsg

/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor
*/
CEcEEPRom::CEcEEPRom(
    CEcMaster*  pMaster, 
    CEcDevice*  pDevice 
                    )
:   m_pMaster(pMaster)
,   m_poEthDevice(pDevice)
,   m_bFixedAddrUsr(EC_FALSE)
,   m_wSlaveAddr(0x0000)
,   m_pwDataBase(EC_NULL)
,   m_pwCurData(EC_NULL)
,   m_dwDataBaseLength(0)
,   m_wEEPRomBaseAddr(0)
,   m_wEEPRomCurAddr(0)
,   m_pdwDataProcessed(EC_NULL)
,   m_dwDataProcessed(0)
,   m_bAssignEEPToPDI(EC_FALSE)
,   m_bForceAssign(EC_FALSE)
,   m_pbActiveBuffer(EC_NULL)
,   m_dwEcatCmdPending(0)
,   m_dwCurEEPAddr(0)
,   m_wChecksum(0)
,   m_pRequest(EC_NULL)
,   m_pCurRequest(EC_NULL)
,   m_eCurState(eepsm_unknown)
,   m_eReqState(eepsm_unknown)
,   m_pCurrentEEPOpReq(EC_NULL)
{
    OsDbgAssert(0 == m_dwEcatCmdPending);
    m_dwEcatCmdPending   = 0;
    m_dwEcatCmdSent      = 0;
    m_dwEcatCmdProcessed = 0;
    OsMemset(&m_oRequest, 0, sizeof(m_oRequest));
    OsMemset(&m_abyEEPROMHead, 0, sizeof(m_abyEEPROMHead));
    m_wEEPStatusReg = 0;
    m_wEEPConfigReg = 0;
}

/***************************************************************************************************/
/**
\brief  Destructor
*/
CEcEEPRom::~CEcEEPRom()
{
    m_pMaster = EC_NULL;
    m_poEthDevice = EC_NULL;
}

/***************************************************************************************************/
/**
\brief  Read slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::PrepareUpload(
    EC_T_BOOL       bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD       wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_WORD       wEEPRomAddress          /**< [in]   Address to start EEPRom Read from.*/                                           
   ,EC_T_WORD*      pwData                  /**< [in]   Pointer to WORD array to carry the read data */                                
   ,EC_T_DWORD      dwLen                   /**< [in]   Sizeof Read Data WORD array (in WORDS) */                                      
   ,EC_T_DWORD*     pdwUseData              /**< [out]  Pointer to DWORD carrying actually read data after completion */               
   ,EC_T_DWORD      dwTimeout               /**< [in]   Operation Timeout */
                                   ) 
{   
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    if( (EC_NULL == pwData) || (0 == dwLen) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    m_bFixedAddrUsr      = bFixedAddressing;
    m_wSlaveAddr         = wSlaveAddress;
    m_wEEPRomBaseAddr    = wEEPRomAddress;
    m_pwDataBase         = pwData;
    m_dwDataBaseLength   = dwLen;
    m_pdwDataProcessed   = pdwUseData;

    dwRetVal = RequestEEPROMOpReq( eepsm_UploadEEPROM, dwTimeout, &m_pCurrentEEPOpReq);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Write Slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::PrepareDownload(
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_WORD   wEEPRomAddress          /**< [in]   Address to start EEPRom Write from */                                          
   ,EC_T_WORD*  pwData                  /**< [in]   Pointer to WORD array carrying the write data */                              
   ,EC_T_DWORD  dwLen                   /**< [in]   Sizeof Write Data WORD array (in WORDS) */                                   
   ,EC_T_DWORD  dwTimeout               /**< [in]   Operation Timeout */
                                     )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    
    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    
    if( (EC_NULL == pwData) || (0 == dwLen) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    m_bFixedAddrUsr     = bFixedAddressing;
    m_wSlaveAddr        = wSlaveAddress;
    m_wEEPRomBaseAddr   = wEEPRomAddress;
    m_pwDataBase        = pwData;
    m_dwDataBaseLength  = dwLen;
    m_pdwDataProcessed  = EC_NULL;
    
    dwRetVal = RequestEEPROMOpReq( eepsm_DownloadEEPROM, dwTimeout, &m_pCurrentEEPOpReq);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Write Slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::PrepareAssign(
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL   bSlavePDIAccessEnable   /**< [in]   EC_TRUE: assign EEPRom to slave PDI application, EC_FALSE: assign EEPRom to ECat Master */
   ,EC_T_BOOL   bForceAssign            /**< [in]   EC_TRUE: force Assignment, only available for ECat Master Assignment, EC_FALSE: don't force */
   ,EC_T_DWORD  dwTimeout               /**< [in]   Operation Timeout */
                                      )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    
    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    
    m_bFixedAddrUsr     = bFixedAddressing;
    m_wSlaveAddr        = wSlaveAddress;
    m_bAssignEEPToPDI   = bSlavePDIAccessEnable;
    m_bForceAssign      = bForceAssign;
    
    dwRetVal = RequestEEPROMOpReq( eepsm_AssignEEPROM, dwTimeout, &m_pCurrentEEPOpReq);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Write Slave EEPRom Data.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::PrepareActive(
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */
   ,EC_T_BOOL*  pbSlavePDIAccessActive  /**< [out]  Pointer holding Boolean buffer, EC_TRUE: Slave PDI application EEPRom access active, EC_FALSE: not active */
   ,EC_T_DWORD  dwTimeout               /**< [in]   Operation Timeout */
                                      )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    
    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    if( (EC_NULL == pbSlavePDIAccessActive) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    m_bFixedAddrUsr     = bFixedAddressing;
    m_wSlaveAddr        = wSlaveAddress;
    m_pbActiveBuffer    = pbSlavePDIAccessActive;
    m_pdwDataProcessed  = EC_NULL;
    
    dwRetVal = RequestEEPROMOpReq( eepsm_CheckEEPROMActive, dwTimeout, &m_pCurrentEEPOpReq);
        
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  EEPROM Reload
*/
EC_T_DWORD CEcEEPRom::PrepareReload( 
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */ 
   ,EC_T_DWORD  dwTimeout               /**< [in]   Operation Timeout */
                                   )
{    
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    
    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    
    m_bFixedAddrUsr     = bFixedAddressing;
    m_wSlaveAddr        = wSlaveAddress;
    m_pdwDataProcessed  = EC_NULL;
    
    dwRetVal = RequestEEPROMOpReq( eepsm_ReloadEEPROM, dwTimeout, &m_pCurrentEEPOpReq);
    
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Slave Controller Reset
*/
EC_T_DWORD CEcEEPRom::PrepareReset( 
    EC_T_BOOL   bFixedAddressing        /**< [in]   EC_TRUE: use fixed addressing, EC_FALSE: use auto increment addressing */      
   ,EC_T_WORD   wSlaveAddress           /**< [in]   Slave Address, fixed or auto increment address depending on bFixedAddressing */ 
   ,EC_T_DWORD  dwTimeout               /**< [in]   Operation Timeout */
                                  )
{    
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    
    if( m_eCurState != m_eReqState )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    
    m_bFixedAddrUsr     = bFixedAddressing;
    m_wSlaveAddr        = wSlaveAddress;
    m_pdwDataProcessed  = EC_NULL;
    
    dwRetVal = RequestEEPROMOpReq( eepsm_ResetSlaveController, dwTimeout, &m_pCurrentEEPOpReq);
    
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Datagram return point.
*/
EC_T_VOID CEcEEPRom::ProcessResult(EC_T_DWORD result ,EC_T_DWORD invokeId ,PETYPE_EC_CMD_HEADER pEcCmdHeader)
{
    EC_T_WORD   wWorkingCnt = 0;
    EC_T_PBYTE  pbyData     = EC_NULL;
    
    /* handle error case */
    if (result != EC_E_NOERROR)
    {
        goto Exit;
    }
    /* get command information */
    if (EC_NULL != pEcCmdHeader)
    {
        wWorkingCnt  = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
        pbyData      = (EC_T_PBYTE)EC_ENDOF(pEcCmdHeader);
    }
    else
    {
        goto Exit;
    }
    
    switch (invokeId)
    {
    case INVOKE_ID_EEPA_CLEAR_ERR:
            if (m_eCurState == eepsm_initial_eep_clear_err_wait)
            {
                if (wWorkingCnt == 1)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;        
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        break;
    case INVOKE_ID_EEPA_WRELOADCMD:
        {
            if( m_eCurState == eepsm_reload_cmd_wait )
            {
                if( wWorkingCnt == 1 )
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;        
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        }break; 

    case INVOKE_ID_EEPA_READ_EEP_CONFIG_REG:
        {
            if(   m_eCurState == eepsm_checkassignment_wait
               || m_eCurState == eepsm_active_rdactive_wait)
            {
                if(wWorkingCnt == 1)
                {
                    m_wEEPConfigReg = EC_GET_FRM_WORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        }break;


    case INVOKE_ID_EEPA_READ_EEP_STATUS_REG:
        {
            if(   m_eCurState == eepsm_initial_eep_check_wait
               || m_eCurState == eepsm_read_eep_status_reg_wait
               || m_eCurState == eepsm_read_crc_area_status_wait
               || m_eCurState == epsm_write_checksum_status_wait)
            {
                if(wWorkingCnt == 1)
                {
                    m_wEEPStatusReg = EC_GET_FRM_WORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        }break;
    case INVOKE_ID_EEPA_CMD:
        {
            if(    m_eCurState == eepsm_upload_cmd_wait 
                || m_eCurState == eepsm_download_cmd_wait
                || m_eCurState == eepsm_read_crc_area_cmd_wait
                || m_eCurState == eepsm_write_checksum_cmd_wait)
            {
                if(wWorkingCnt == 1)
                {
                    /* mark command as processed */
	                m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        } break;
    case INVOKE_ID_EEPA_READVALUE:
        {
            if( m_eCurState == eepsm_upload_data_wait || m_eCurState == eepsm_read_crc_area_data_wait )
            {
                if( wWorkingCnt == 1 )
                {
                    if( m_eCurState == eepsm_upload_data_wait )
                    {
                        OsMemcpy((EC_T_BYTE*)m_pwCurData, pbyData, 2);
                    }
                    else
                    {
                        OsMemcpy((EC_T_BYTE*)&(m_abyEEPROMHead[(m_dwCurEEPAddr*sizeof(EC_T_WORD))]), pbyData, 2);
                    }
                    /* mark command as processed */
	                m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        } break;
    case INVOKE_ID_EEPA_ASRD:
        {
            if( m_eCurState == eepsm_assign_rdassignment_wait )
            {
                if( wWorkingCnt == 1 )
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        } break;
    case INVOKE_ID_EEPA_ASWR:
        {
            if( m_eCurState == eepsm_assign_wrassignment_wait )
            {
                if( wWorkingCnt == 1 )
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;   
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        } break;
    case INVOKE_ID_EEPA_RESET_R:
        {
            if(m_eCurState == eepsm_reset_send_wait)
            {
                if( wWorkingCnt == 1 )
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;   
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }

        } break;
    case INVOKE_ID_EEPA_RESET_E:
        {
            if( m_eCurState == eepsm_reset_send_wait )
            {
                if( wWorkingCnt == 3 )
                {
                    if( 0x01 == pbyData[0] )
                    {
                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;                           
                    }
                    else
                    {
                        EC_DBGMSG("Invalid Response from slave when Writing Slave Reset 'E' Expected: 0x01 Received: 0x%02X", pbyData[0]);
                    }
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }

        } break;
    case INVOKE_ID_EEPA_RESET_S:
        {
            if( m_eCurState == eepsm_reset_send_wait )
            {
                if(wWorkingCnt == 3)
                {
                    if( 0x02 != pbyData[0] )
                    {
                        EC_DBGMSG("Invalid Response from slave when Writing Slave Reset 'S' Expected: 0x02 Received: 0x%02X", pbyData[0]);
                    }

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( eepsm_reqto == m_eCurState );
            }
        } break;
    default:
        {
            OsDbgAssert( EC_FALSE );
        } break;
    }
    
Exit:
    if( 0 != m_dwEcatCmdPending )
    {
        m_dwEcatCmdPending --;
    }

    return;
}

/***************************************************************************************************/
/**
\brief  Evaluate EEPROM Checksum.

\return Checksum Value according to CRC-8 (ATM) Polynomial 8210.
*/
EC_T_BYTE CEcEEPRom::CalcCRC(
    EC_T_BYTE*  pbyData     /**< [in]   Data to calculate CRC */
   ,EC_T_DWORD  dwLength    /**< [in]   Length of Data */
                            )
{
#define POLY 0x07
    EC_T_BYTE   byCrc   = (EC_T_BYTE)0xFF;
    EC_T_DWORD  i       = 0;
    EC_T_DWORD  j       = 0;
    
    for( i = 0; i < dwLength; i++ )
    {
        byCrc ^= pbyData[i];
        
        for( j = 0; j < 8; j++ )
        {
            if( byCrc & 0x80 )
            {
                byCrc = (EC_T_BYTE)((byCrc<<1) ^ POLY);
            }
            else
            {
                byCrc <<= 1;
            }
        }
    }
    return byCrc;
}

/***************************************************************************************************/
/**
\brief  EEPROM Operation state machine.
*/
EC_T_VOID CEcEEPRom::EEPStateMachine(EC_T_VOID)
{
    EC_T_BOOL               bContinueSM = EC_FALSE;

#ifdef INCLUDE_LOG_MESSAGES
    EC_T_BYTE               byDbgLvl    = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
    ECEEP_T_EEPROMOPSTATE   eOldState   = eepsm_unknown;
    EC_T_DWORD              dwRes       = EC_E_ERROR;
            
    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_EEPSM);

    /* check whether state machine is idle */
    if( (m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest) )
    {
        /* stable, check for queued request */
        if( EC_NULL != m_pRequest )
        {
            /* Fetch new Request */
            m_pCurRequest   = m_pRequest;
            /* free Request queue */
            m_pRequest      = EC_NULL;

            /* store current request as target for sm */
            m_eReqState             = m_pCurRequest->eState;

            /* start state Machine */
            m_eCurState             = eepsm_start;
            bContinueSM             = EC_TRUE;
        }
        else
        {
            /* no new request leave SM */
            bContinueSM             = EC_FALSE;
        }
    }
    else
    {
        /* request still pending, go into SM */
        bContinueSM                 = EC_TRUE;
    }

    /* state is stable, request not yet thrown out */
    if(    (m_eCurState == m_eReqState) && (EC_NULL != m_pCurRequest) )
    {
        m_pCurRequest->oTimeout.Stop();
        
        m_pCurRequest = EC_NULL;
        bContinueSM = EC_FALSE;
    }
    
    while( bContinueSM )
    {
        /* only do a single run of SM per default */
        bContinueSM = EC_FALSE;

        eOldState = m_eCurState;

        /* if Current Request is missing, and SM is not stable -> stuck */
        if( EC_NULL == m_pCurRequest && m_eCurState != m_eReqState && m_eCurState != eepsm_idle )
        {
            OsDbgAssert(EC_FALSE);
            m_eCurState = m_eReqState = eepsm_stuck;
            OsDbgAssert(EC_FALSE);
            continue;
        }

        /* link was disconnected */
        if((EC_NULL != m_pCurRequest) && !m_poEthDevice->IsLinkConnected())
        {
            m_pCurRequest->oTimeout.Stop();
            m_eReqState = ecbtsm_link_disconnected;             
        }

        /* current request did time out */
        if( (EC_NULL != m_pCurRequest) && m_pCurRequest->oTimeout.IsElapsed() )
        {
            m_pCurRequest->oTimeout.Stop();
            m_eReqState = eepsm_reqto; 
        }

        switch( m_eCurState )
        {
        case eepsm_reqto:
            {
                m_pCurRequest->dwResult = EC_E_TIMEOUT;
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = eepsm_idle;

                OsDbgAssert(m_dwEcatCmdPending == 0);

            } break;
        case ecbtsm_link_disconnected:
            {
                m_pCurRequest->dwResult = EC_E_LINK_DISCONNECTED;
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = eepsm_idle;

                OsDbgAssert(m_dwEcatCmdPending == 0);
            }break;
        case eepsm_stuck:
            {
                /* wait for outstanding commands returned (ProcResult) which is always the case (don't care if frameloss e.g.) 
                 * and return to idle */
                if( m_dwEcatCmdPending == 0) /* if no DGram calls are pending */
                {
                    m_eCurState = eepsm_idle;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_idle:
            {
                /* from idle, decide which way to go */
                switch( m_eReqState )
                {
                case eepsm_AssignEEPROM:
                case eepsm_CheckEEPROMActive:
                case eepsm_UploadEEPROM:
                case eepsm_DownloadEEPROM:
                case eepsm_ReloadEEPROM:
                case eepsm_ResetSlaveController:
                    m_eCurState = eepsm_start;
                    bContinueSM = EC_TRUE;
                    break;
                default: OsDbgAssert(EC_FALSE); break;
                }
            } break;
        case eepsm_start:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                switch( m_eReqState )
                {
                case eepsm_AssignEEPROM:
                    {
                        m_eCurState = eepsm_assign_rdassignment;
                        bContinueSM = EC_TRUE;
                    } break;
                case eepsm_CheckEEPROMActive:
                    {
                        m_eCurState = eepsm_active_rdactive;
                        bContinueSM = EC_TRUE;
                    } break;
                case eepsm_UploadEEPROM:
                    {
                        m_eCurState = eepsm_upload_start;
                        bContinueSM = EC_TRUE;
                    } break;
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = eepsm_download_start;
                        bContinueSM = EC_TRUE;
                    } break;
                case eepsm_ReloadEEPROM:
                    {
                        m_eCurState = eepsm_initial_eep_check;
                        bContinueSM = EC_TRUE;
                    } break;
                case eepsm_ResetSlaveController:
                    {
                        m_eCurState = eepsm_reset_send;
                        bContinueSM = EC_TRUE;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                        bContinueSM = EC_TRUE;
                    } break;
                }
            } break;
        case eepsm_initial_eep_check:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* read the eeprom control/status register 0x502*/
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_STATUS_REG, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;
                
                /* determine next state */
                m_eCurState = eepsm_initial_eep_check_wait;
            } break;
        case eepsm_initial_eep_check_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_initial_eep_check_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_initial_eep_check_done:
            {     
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_initial_eep_check;
                    break;
                }

                /* check if the busy bit of the EEPROM Status register is cleared */
                if(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_BUSY)
                {
                    m_eCurState = eepsm_read_eep_status_reg;
                    break;
                }

                /* check if the error bits of the EEPROM Status register are cleared  */
                if(   (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_WRITE_ENA))
                {
                    m_eCurState = eepsm_initial_eep_clear_err;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_UploadEEPROM:
                    {
                        m_eCurState = eepsm_upload_cmd;
                    }break;
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = eepsm_download_cmd;     
                    } break;
                case eepsm_ReloadEEPROM:
                    {
                        m_eCurState = eepsm_reload_cmd;     
                    } break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = eepsm_stuck; 
                }
            }break;

        case eepsm_initial_eep_clear_err:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                EC_T_WORD wData = 0; 
                dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, 
                        INVOKE_ID_EEPA_CLEAR_ERR,
                        ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                        m_wSlaveAddr,
                        ECM_SB_EEP_SLV_CTRLSTATUS,
                        0x2, &wData
                        );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_initial_eep_clear_err_wait;
            } break;
        case eepsm_initial_eep_clear_err_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_initial_eep_clear_err_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_initial_eep_clear_err_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_initial_eep_clear_err;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_UploadEEPROM:
                    {
                        m_eCurState = eepsm_upload_cmd;
                    }break;
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = eepsm_download_cmd;     
                    } break;
                case eepsm_ReloadEEPROM:
                    {
                        m_eCurState = eepsm_reload_cmd;     
                    } break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = eepsm_stuck; 
                }
                bContinueSM = EC_TRUE;
            } break;

        case eepsm_assign_rdassignment:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_ASRD,
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_PDIACCSTATE, 
                    0x2, EC_NULL                
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_assign_rdassignment_wait;
            } break;
        case eepsm_assign_rdassignment_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_assign_rdassignment_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_assign_rdassignment_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_assign_rdassignment;
                    break;
                }

                /* eceepsmc_error indicates that eeprom is already assigned */
                switch( m_eReqState )
                {
                case eepsm_AssignEEPROM:
                    {
                        m_eCurState = eepsm_assign_wrassignment;
                    } break;
                case eepsm_CheckEEPROMActive:
                    {
                        m_eCurState = eepsm_active_rdactive;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_assign_wrassignment:
            {
                EC_T_BYTE   byWriteAssign = 0;

                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                
                if( m_bAssignEEPToPDI )
                {
                    byWriteAssign |= ECM_SB_EEP_PDIACCSTATE_PDI;
                }
                else
                {
                    byWriteAssign = 0;
                
                    if( m_bForceAssign )
                    {
                        byWriteAssign |= ECM_SB_EEP_PDIACCSTATE_OVERRIDE;
                    }
                }
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_ASWR,
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_PDIACCSTATE,
                    0x1, &byWriteAssign
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_assign_wrassignment_wait;
            } break;
        case eepsm_assign_wrassignment_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_assign_wrassignment_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_assign_wrassignment_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_assign_wrassignment;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_AssignEEPROM:
                    {
                        m_eCurState = eepsm_checkassignment;
                        bContinueSM = EC_TRUE;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_checkassignment:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_CONFIG_REG,
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    (ECM_SB_EEP_SLV_PDIACCSTATE ), 
                    0x2, EC_NULL                
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_checkassignment_wait;
            }break;
        case eepsm_checkassignment_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_checkassignment_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_checkassignment_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_checkassignment;
                    break;
                }

                if( m_bAssignEEPToPDI )
                {
                    if( !(m_wEEPConfigReg & ECM_SB_EEP_PDIACCSTATE_PDI))
                    {
                        /* Assignment of the EEPROM to the slave went wrong. Return an error */
                        m_pCurRequest->dwResult = EC_E_EEPROMASSIGNERROR;
                        m_pCurRequest->oTimeout.Stop();
                        m_pCurRequest           = EC_NULL;
                        m_eCurState = m_eReqState = eepsm_idle;
                        break;
                    }
                }
                else
                {
                    if(m_wEEPConfigReg & ECM_SB_EEP_PDIACCSTATE_PDI)
                    {
                        /* Assignment ofthe EEPROM to EtherCAT went wrong. Return an error */
                        m_pCurRequest->dwResult = EC_E_EEPROMASSIGNERROR;
                        m_pCurRequest->oTimeout.Stop();
                        m_pCurRequest           = EC_NULL;
                        m_eCurState = m_eReqState = eepsm_idle;
                        break;
                    }
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_AssignEEPROM:
                    {
                        m_eCurState = eepsm_AssignEEPROM;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_active_rdactive:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_CONFIG_REG,
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_PDIACCSTATE, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_active_rdactive_wait;
            } break;
        case eepsm_active_rdactive_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_active_rdactive_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_active_rdactive_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_active_rdactive;
                    break;
                }

                if( ECM_SB_EEP_PDIACCSTATE_PDIACTIVE == (m_wEEPConfigReg&ECM_SB_EEP_PDIACCSTATE_PDIACTIVE) )
                {
                    /* PDI active */
                    if( EC_NULL != m_pbActiveBuffer )
                    {
                        EC_SETDWORD(m_pbActiveBuffer, EC_TRUE);
                    }
                }
                else
                {
                    /* PDI in-active */
                    if( EC_NULL != m_pbActiveBuffer )
                    {
                        EC_SETDWORD(m_pbActiveBuffer, EC_FALSE);
                    }
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_CheckEEPROMActive:
                    {
                        m_eCurState = eepsm_CheckEEPROMActive;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;

        case eepsm_upload_start:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_wEEPRomCurAddr     = m_wEEPRomBaseAddr;
                m_pwCurData          = m_pwDataBase;
                m_dwDataProcessed    = 0;
                if( EC_NULL != m_pdwDataProcessed )
                {
                    EC_SETDWORD(m_pdwDataProcessed, 0);
                }
                m_eCurState = eepsm_initial_eep_check;
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_upload_cmd:
            {
                EC_T_SB_EEP_REGS oRegFrm = {0};

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                
                EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, ECM_SB_EEP_CTRLSTATUS_WRITE_READACCESS);
                EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, EC_MAKEDWORD(0,m_wEEPRomCurAddr));

                /* send EtherCAT command to read DL Information */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_CMD, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    6, (EC_T_PVOID)&oRegFrm
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_upload_cmd_wait;
            } break;
        case eepsm_upload_cmd_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_upload_cmd_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_upload_cmd_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_upload_cmd;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_UploadEEPROM:
                    {
                        m_eCurState = eepsm_read_eep_status_reg;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_upload_data:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* check for busy flag */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READVALUE, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_EEPDATA, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_upload_data_wait;
            } break;
        case eepsm_upload_data_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_upload_data_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_upload_data_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_upload_data;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_UploadEEPROM:
                    {
                        /* increment processed counter */
                        m_dwDataProcessed++;
                            
                        if( m_dwDataProcessed < m_dwDataBaseLength )
                        {
                            m_pwCurData++;
                            m_wEEPRomCurAddr++;
                            m_eCurState = eepsm_upload_cmd;
                        }
                        else
                        {
                            m_eCurState = eepsm_UploadEEPROM;
                        }
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;

        case eepsm_download_start:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_wEEPRomCurAddr     = m_wEEPRomBaseAddr;
                m_pwCurData          = m_pwDataBase;
                m_dwDataProcessed    = 0;

                m_eCurState          = eepsm_initial_eep_check;
                bContinueSM          = EC_TRUE;
            } break;
        case eepsm_download_cmd:
            {
                EC_T_SB_EEP_REGS oRegFrm = {0};

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, (EC_T_WORD)(ECM_SB_EEP_CTRLSTATUS_WRITE_WRITEACCESS | ECM_SB_EEP_CTRLSTATUS_WRITE_ENABLE));
                EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, EC_MAKEDWORD(0,m_wEEPRomCurAddr));
                OsMemcpy(&oRegFrm.abyEEPData[0], (EC_T_BYTE*)m_pwCurData, 2);

                /* send EtherCAT command to read DL Information */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_CMD, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    8, (EC_T_PVOID)&oRegFrm
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_download_cmd_wait;
            } break;
        case eepsm_download_cmd_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_download_cmd_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_download_cmd_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_download_cmd;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = eepsm_read_eep_status_reg;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_correct_crc_checksum:
            {
                /* reset old data */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                OsMemset(m_abyEEPROMHead, 0, sizeof(m_abyEEPROMHead));
                m_dwCurEEPAddr  = ESC_SII_REG_PDICONTROL;
                m_wChecksum     = 0;
                m_eCurState     = eepsm_read_crc_area_cmd;
                bContinueSM     = EC_TRUE;
            } break;
        case eepsm_read_crc_area_cmd:
            {
                EC_T_SB_EEP_REGS oRegFrm = {0};

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, ECM_SB_EEP_CTRLSTATUS_WRITE_READACCESS);
                EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, m_dwCurEEPAddr);

                /* send EtherCAT command to read DL Information */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_CMD, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    6, (EC_T_PVOID)&oRegFrm
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_read_crc_area_cmd_wait;
            } break;
        case eepsm_read_crc_area_cmd_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_read_crc_area_cmd_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_read_crc_area_cmd_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_correct_crc_checksum;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = eepsm_read_crc_area_status;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_read_crc_area_status:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* read the eeprom control/status register 0x502*/
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_STATUS_REG, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_read_crc_area_status_wait;
            } break;
        case eepsm_read_crc_area_status_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_read_crc_area_status_done;
                    bContinueSM = EC_TRUE;   
                }
            } break;
        case eepsm_read_crc_area_status_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_read_crc_area_status;
                    break;
                }

                /* check if the eeprom interface is still busy */
                if(   (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_BUSY)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_WRITEINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_READINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_RELOADINPROGRESS))
                {
                    m_eCurState = eepsm_read_crc_area_status;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                       if(   !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                          && !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_WRITE_ENA) )
                       { 
                            m_eCurState = eepsm_read_crc_area_data;
                       }
                       else
                       {
                            m_eCurState = eepsm_read_crc_area_cmd;
                       }
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_read_crc_area_data:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* retrieve data  */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READVALUE, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_EEPDATA, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_read_crc_area_data_wait;
            } break;
        case eepsm_read_crc_area_data_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {                        
                    m_eCurState = eepsm_read_crc_area_data_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_read_crc_area_data_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_read_crc_area_data;
                    break;
                }

                 /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                        if( m_dwCurEEPAddr < (ESC_SII_REG_CHECKSUM-1) )
                        {
                            m_dwCurEEPAddr++;
                            m_eCurState = eepsm_read_crc_area_cmd;
                        }
                        else
                        {
                            m_eCurState = eepsm_crc_area_eval_checksum;
                        }
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_crc_area_eval_checksum:
            {
                EC_T_BYTE byCheckSum = 0;

                byCheckSum = CalcCRC(m_abyEEPROMHead, sizeof(m_abyEEPROMHead) );

                m_wChecksum = EC_MAKEWORD(0, byCheckSum);
                m_eCurState = eepsm_write_checksum_cmd;
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_write_checksum_cmd:
            {
                EC_T_SB_EEP_REGS oRegFrm = {0};

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                
                EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, (EC_T_WORD)(ECM_SB_EEP_CTRLSTATUS_WRITE_WRITEACCESS | ECM_SB_EEP_CTRLSTATUS_WRITE_ENABLE));
                EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, (EC_T_DWORD)ESC_SII_REG_CHECKSUM);
                EC_SET_FRM_DWORD((EC_T_DWORD*)&oRegFrm.abyEEPData[0], EC_MAKEDWORD(0,m_wChecksum));

                /* send EtherCAT command to read DL Information */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_CMD, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    10, (EC_T_PVOID)&oRegFrm
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_write_checksum_cmd_wait;
            } break;
        case eepsm_write_checksum_cmd_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_write_checksum_cmd_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_write_checksum_cmd_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_crc_area_eval_checksum;
                    break;
                }

                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                        m_eCurState = epsm_write_checksum_status;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case epsm_write_checksum_status:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* read the eeprom control/status register 0x502*/
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_STATUS_REG, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = epsm_write_checksum_status_wait;
            } break;
        case epsm_write_checksum_status_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = epsm_write_checksum_status_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case epsm_write_checksum_status_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = epsm_write_checksum_status;
                    break;
                }

                /* check if the eeprom interface is still busy */
                if(   (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_BUSY)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_WRITEINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_READINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_RELOADINPROGRESS))
                {
                    m_eCurState = epsm_write_checksum_status;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_DownloadEEPROM:
                    {
                        if(   !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                           && !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_WRITE_ENA) )
                        { 
                            m_eCurState = eepsm_DownloadEEPROM;
                        }
                        else
                        {
                            m_eCurState = eepsm_write_checksum_cmd;
                        }                        
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_reload_cmd:
            {
                EC_T_WORD wRegFrm = 0;

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                
                EC_SET_FRM_WORD(&wRegFrm, (ECM_SB_EEP_CTRLSTATUS_WRITE_RELOADACCESS /*| ECM_SB_EEP_CTRLSTATUS_WRITE_ENABLE*/));
                
                /* send EtherCAT command to read DL Information */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_WRELOADCMD, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    sizeof(EC_T_WORD), &wRegFrm
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_reload_cmd_wait;
            } break;
        case eepsm_reload_cmd_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_reload_cmd_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_reload_cmd_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_reload_cmd;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_ReloadEEPROM:
                    {
                        m_eCurState = eepsm_read_eep_status_reg;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_reset_send:
            {
                EC_T_BYTE bySendData    = 0;

                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* Send R */
                bySendData  = 0x52; /* 'R' */
                dwRes = m_pMaster->QueueRegisterCmdReq( 
                    EC_NULL, 
                    INVOKE_ID_EEPA_RESET_R, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPWR:EC_CMD_TYPE_APWR)),
                    m_wSlaveAddr,
                    ECREG_RESET_ECAT,
                    sizeof(EC_T_BYTE), &bySendData);
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;
                m_pMaster->EcatFlushCurrPendingSlaveFrame();

                /* Send E */
                bySendData  = 0x45; /* 'E' */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_RESET_E, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRW:EC_CMD_TYPE_APRW)),
                    m_wSlaveAddr,
                    ECREG_RESET_ECAT,
                    sizeof(EC_T_BYTE), &bySendData
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;
                m_pMaster->EcatFlushCurrPendingSlaveFrame();

                /* Send S */
                bySendData  = 0x53; /* 'S' */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_RESET_S, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRW:EC_CMD_TYPE_APRW)),
                    m_wSlaveAddr,
                    ECREG_RESET_ECAT,
                    sizeof(EC_T_BYTE), &bySendData
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;
                m_pMaster->EcatFlushCurrPendingSlaveFrame();
                
                /* determine next state */
                m_eCurState = eepsm_reset_send_wait;
            } break;
        case eepsm_reset_send_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_reset_send_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_reset_send_done:
            {
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* after the reset commands are sent, the link is possibly down and the 
                   last frame does not return. So we dont check the processed commands */
                // if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_ResetSlaveController:
                    {
                        m_eCurState = eepsm_ResetSlaveController;
                    } break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_read_eep_status_reg:
            {
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* read the eeprom control/status register 0x502*/
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, 
                    INVOKE_ID_EEPA_READ_EEP_STATUS_REG, 
                    ((EC_T_CHAR)((m_bFixedAddrUsr)?EC_CMD_TYPE_FPRD:EC_CMD_TYPE_APRD)),
                    m_wSlaveAddr,
                    ECM_SB_EEP_SLV_CTRLSTATUS, 
                    0x2, EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurState = eepsm_read_eep_status_reg_wait;
            } break;
        case eepsm_read_eep_status_reg_wait:
            {
                if(m_dwEcatCmdPending == 0)
                {
                    m_eCurState = eepsm_read_eep_status_reg_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case eepsm_read_eep_status_reg_done:
            {     
                /* handle timeout request */
                if (m_eReqState == eepsm_reqto)
                {
                    m_eCurState = eepsm_reqto;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* handle link disconnected request */
                if (m_eReqState == ecbtsm_link_disconnected)
                {
                    m_eCurState = ecbtsm_link_disconnected;
                    bContinueSM = EC_TRUE;
                    break;
                }

                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    m_eCurState = eepsm_read_eep_status_reg;
                    break;
                }

                /* check if the eeprom interface is still busy */
                if(   (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_BUSY)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_WRITEINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_READINPROGRESS)
                   || (m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_READ_RELOADINPROGRESS))
                {
                    m_eCurState = eepsm_read_eep_status_reg;
                    break;
                }

                /* determine the next step */
                switch( m_eReqState )
                {
                case eepsm_UploadEEPROM:
                    {
                        if(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                        {
                            m_eCurState = eepsm_upload_cmd; 
                        }
                        else
                        {
                            m_eCurState = eepsm_upload_data;
                        }
                    }break;
                case eepsm_DownloadEEPROM:
                    {          
                        /* check if error bits are set */
                        if(   !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                            && !(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_WRITE_ENA) )
                        {                               
                            if( (m_dwDataProcessed +1) < m_dwDataBaseLength )
                            {
                                /* prepare the next byte/word for download */
                                m_dwDataProcessed++;
                                m_pwCurData++;
                                m_wEEPRomCurAddr++;
                                m_eCurState = eepsm_download_cmd;
                            }
                            else
                            {
                                /* download finished, now check whether EEPROM checksum correction is necassary */
                                if( m_wEEPRomBaseAddr <= ESC_SII_REG_CHECKSUM )
                                {
                                    if( (m_wEEPRomBaseAddr+m_dwDataBaseLength) >= ESC_SII_REG_CHECKSUM )
                                    {
                                        /* don't recalculate CRC if CRC was part of the write data */
                                        m_eCurState = eepsm_DownloadEEPROM;
                                    }
                                    else
                                    {
                                        m_eCurState = eepsm_correct_crc_checksum;
                                    }
                                }
                                else
                                {
                                    m_eCurState = eepsm_DownloadEEPROM;
                                }
                            }
                        }
                        else
                        {
                            m_eCurState = eepsm_download_cmd;
                        }     
                    } break;
                case eepsm_ReloadEEPROM:
                    {
                        if(m_wEEPStatusReg & ECM_SB_EEP_CTRLSTATUS_ERR_ACK)
                        {
                            m_eCurState = eepsm_reload_cmd;
                        }
                        else
                        {
                            m_eCurState = eepsm_ReloadEEPROM;
                        }
                    }
                    break;
                default: 
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = eepsm_stuck; 
                    } break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case eepsm_AssignEEPROM:
        case eepsm_CheckEEPROMActive:
        case eepsm_ReloadEEPROM:
        case eepsm_ResetSlaveController:
            {
                /* store result and throw out request */
                m_pCurRequest->dwResult = EC_E_NOERROR;
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest           = EC_NULL;
                m_eCurState = m_eReqState = eepsm_idle;
            } break;

        case eepsm_UploadEEPROM:
            {
                if( EC_NULL != m_pdwDataProcessed )
                {
                    EC_SETDWORD(m_pdwDataProcessed, m_dwDataProcessed);
                }
                
                m_bFixedAddrUsr         = EC_FALSE;
                m_wSlaveAddr            = 0x0000;
                m_pwDataBase            = EC_NULL;
                m_pwCurData             = EC_NULL;
                m_dwDataBaseLength      = 0;
                m_wEEPRomBaseAddr       = 0;
                m_wEEPRomCurAddr        = 0;
                m_pdwDataProcessed      = EC_NULL;
                m_dwDataProcessed       = 0;
                m_dwEcatCmdPending      = 0;
                
                m_pCurRequest->dwResult = EC_E_NOERROR;
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest           = EC_NULL;
                m_eCurState = m_eReqState = eepsm_idle;
            } break;

        case eepsm_DownloadEEPROM:
            {
                m_bFixedAddrUsr         = EC_FALSE;
                m_wSlaveAddr            = 0x0000;
                m_pwDataBase            = EC_NULL;
                m_pwCurData             = EC_NULL;
                m_dwDataBaseLength      = 0;
                m_wEEPRomBaseAddr       = 0;
                m_wEEPRomCurAddr        = 0;
                m_pdwDataProcessed      = EC_NULL;
                m_dwDataProcessed       = 0;
                m_dwEcatCmdPending      = 0;
                
                m_pCurRequest->dwResult = EC_E_NOERROR;
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest           = EC_NULL;
                m_eCurState = m_eReqState = eepsm_idle;
            } break;

        default:
            {
                OsDbgAssert(EC_FALSE);
                m_eCurState = m_eReqState = eepsm_stuck;
                bContinueSM = EC_FALSE;
            } /* default */
        } /* switch m_eCurState */
#ifdef INCLUDE_LOG_MESSAGES
        if( (m_eCurState != eOldState) && byDbgLvl )
        {
            EC_DBGMSGL(( 
                byDbgLvl, ETHTYP0_EEPSM, ETHTYP1_EEPSM_SC,
                "EEPStateMachine %s->%s (%s)\n", EEPSMSTATESText(eOldState), EEPSMSTATESText(m_eCurState), EEPSMSTATESText(m_eReqState)
                ));
        }
#endif
    } /* while( bContinueSM)  */

Exit:
    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_EEPSM);
    return;
}

/***************************************************************************************************/
/**
\brief  Enqueue new Request for Port Operation Instance.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcEEPRom::RequestEEPROMOpReq(
    ECEEP_T_EEPROMOPSTATE   eState          /**< [in]   Desired state */
   ,EC_T_DWORD              dwTimeout       /**< [in]   Request Timeout */
   ,ECEEPSM_REQ**           pHandle         /**< [out]  Handle to Request if EC_E_NOERROR is returned */
                                        )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    if( EC_NULL == pHandle )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    switch(eState)
    {
    case eepsm_AssignEEPROM: 
    case eepsm_CheckEEPROMActive: 
    case eepsm_UploadEEPROM: 
    case eepsm_DownloadEEPROM: 
    case eepsm_ReloadEEPROM: 
    case eepsm_ResetSlaveController: 
        break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( (m_oRequest[0].bUsed) && (m_oRequest[1].bUsed) )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    OsDbgAssert( EC_NULL == m_pRequest );
    if( EC_NULL == m_pRequest )
    {
        if( m_oRequest[0].bUsed )
        {
            (*pHandle) = &(m_oRequest[1]);
        }
        else
        {
            (*pHandle) = &(m_oRequest[0]);
        }
        (*pHandle)->bUsed       = EC_TRUE;
        (*pHandle)->eState      = eState;
        (*pHandle)->dwResult    = EC_E_BUSY;
        (*pHandle)->oTimeout.Start(dwTimeout, m_pMaster->GetMsecCounterPtr());
        
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_BUSY;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start performing queued EEPROM operation.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::StartEEPROMOP(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( EC_NULL == m_pCurrentEEPOpReq )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    m_pRequest = m_pCurrentEEPOpReq;
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll current EEPROM operation.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcEEPRom::PollEEPROMOp()
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
        
    EEPStateMachine();

    if( EC_NULL == m_pCurrentEEPOpReq )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    
    dwRetVal = PollEEPROMOpReq(m_pCurrentEEPOpReq);

    if( EC_E_BUSY != dwRetVal && EC_E_NOTREADY != dwRetVal)
    {
        /* release request */
        ReleaseEEPROMOpReq(m_pCurrentEEPOpReq);
        m_pCurrentEEPOpReq = EC_NULL;
    }
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll success of requested state.

\return error code.
*/
EC_T_DWORD CEcEEPRom::PollEEPROMOpReq(
    ECEEPSM_REQ*        pHandle     /**< [in]   desired request to wait for */
                                     )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if( m_pRequest == pHandle )
    {
        /* request not fetched for execution by state machine */
        dwRetVal = EC_E_NOTREADY;
        goto Exit;
    }
    
    if( m_pCurrentEEPOpReq == pHandle )
    {
        /* request fetched, result is updated by state machine */
        dwRetVal = pHandle->dwResult;
    }
    else
    {
        /* already out of SM, waits for result read */
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Release handle after reading result code.

\return error code.
*/
EC_T_DWORD CEcEEPRom::ReleaseEEPROMOpReq(
    ECEEPSM_REQ*   pHandle     /**< [in]   desired request to release */
                                          )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( (pHandle != &(m_oRequest[0])) && (pHandle != &(m_oRequest[1])) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    if( pHandle == m_pRequest )
    {
        m_pRequest = EC_NULL;
    }

    if( pHandle == m_pCurRequest )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    pHandle->bUsed = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Return Enum Text for EEPROM StateMachine.

\return State Text.
*/
EC_T_CHAR* CEcEEPRom::EEPSMSTATESText(
    ECEEP_T_EEPROMOPSTATE       eState  /**< [in]   State to retrieve text to */
                                      )
{
    EC_T_CHAR* pszRet = (EC_T_CHAR*)"Unknown state";

    switch( eState )
    {
    case eepsm_unknown:                     pszRet = (EC_T_CHAR*)"eepsm_unknown"; break;
    case eepsm_initial_eep_check:           pszRet = (EC_T_CHAR*)"eepsm_initial_eep_check"; break;
    case eepsm_initial_eep_check_wait:      pszRet = (EC_T_CHAR*)"eepsm_initial_eep_check_wait"; break;
    case eepsm_initial_eep_check_done:      pszRet = (EC_T_CHAR*)"eepsm_initial_eep_check_done"; break;
    case eepsm_initial_eep_clear_err:       pszRet = (EC_T_CHAR*)"eepsm_initial_eep_clear_err"; break;
    case eepsm_initial_eep_clear_err_wait:  pszRet = (EC_T_CHAR*)"eepsm_initial_eep_clear_err_wait"; break;
    case eepsm_initial_eep_clear_err_done:  pszRet = (EC_T_CHAR*)"eepsm_initial_eep_clear_err_done"; break;
    case eepsm_read_eep_status_reg:         pszRet = (EC_T_CHAR*)"eepsm_read_eep_status_reg"; break;
    case eepsm_read_eep_status_reg_wait:    pszRet = (EC_T_CHAR*)"eepsm_read_eep_status_reg_wait"; break;
    case eepsm_read_eep_status_reg_done:    pszRet = (EC_T_CHAR*)"eepsm_read_eep_status_reg_done"; break;
    case eepsm_idle:                        pszRet = (EC_T_CHAR*)"eepsm_idle"; break;
    case eepsm_start:                       pszRet = (EC_T_CHAR*)"eepsm_start"; break;
    case eepsm_assign_rdassignment:         pszRet = (EC_T_CHAR*)"eepsm_assign_rdassignment"; break;
    case eepsm_assign_rdassignment_wait:    pszRet = (EC_T_CHAR*)"eepsm_assign_rdassignment_wait"; break;
    case eepsm_assign_rdassignment_done:    pszRet = (EC_T_CHAR*)"eepsm_assign_rdassignment_done"; break;
    case eepsm_assign_wrassignment:         pszRet = (EC_T_CHAR*)"eepsm_assign_wrassignment"; break;
    case eepsm_assign_wrassignment_wait:    pszRet = (EC_T_CHAR*)"eepsm_assign_wrassignment_wait"; break;
    case eepsm_assign_wrassignment_done:    pszRet = (EC_T_CHAR*)"eepsm_assign_wrassignment_done"; break;
    case eepsm_checkassignment:             pszRet = (EC_T_CHAR*)"eepsm_checkassignment"; break;
    case eepsm_checkassignment_wait:        pszRet = (EC_T_CHAR*)"eepsm_checkassignment_wait"; break;     
    case eepsm_checkassignment_done:        pszRet = (EC_T_CHAR*)"eepsm_checkassignment_done"; break;
    case eepsm_active_rdactive:             pszRet = (EC_T_CHAR*)"eepsm_active_rdactive"; break;
    case eepsm_active_rdactive_wait:        pszRet = (EC_T_CHAR*)"eepsm_active_rdactive_wait"; break;
    case eepsm_active_rdactive_done:        pszRet = (EC_T_CHAR*)"eepsm_active_rdactive_done"; break;
    case eepsm_upload_start:                pszRet = (EC_T_CHAR*)"eepsm_upload_start"; break;
    case eepsm_upload_cmd:                  pszRet = (EC_T_CHAR*)"eepsm_upload_cmd"; break;
    case eepsm_upload_cmd_wait:             pszRet = (EC_T_CHAR*)"eepsm_upload_cmd_wait"; break;
    case eepsm_upload_cmd_done:             pszRet = (EC_T_CHAR*)"eepsm_upload_cmd_done"; break;
    case eepsm_upload_data:                 pszRet = (EC_T_CHAR*)"eepsm_upload_data"; break;
    case eepsm_upload_data_wait:            pszRet = (EC_T_CHAR*)"eepsm_upload_data_wait"; break;
    case eepsm_upload_data_done:            pszRet = (EC_T_CHAR*)"eepsm_upload_data_done"; break;
    case eepsm_download_start:              pszRet = (EC_T_CHAR*)"eepsm_download_start"; break;
    case eepsm_download_cmd:                pszRet = (EC_T_CHAR*)"eepsm_download_cmd"; break;
    case eepsm_download_cmd_wait:           pszRet = (EC_T_CHAR*)"eepsm_download_cmd_wait"; break;
    case eepsm_download_cmd_done:           pszRet = (EC_T_CHAR*)"eepsm_download_cmd_done"; break;
    case eepsm_correct_crc_checksum:        pszRet = (EC_T_CHAR*)"eepsm_correct_crc_checksum"; break;
    case eepsm_read_crc_area_cmd:           pszRet = (EC_T_CHAR*)"eepsm_download_rdeepromcmd"; break;
    case eepsm_read_crc_area_cmd_wait:      pszRet = (EC_T_CHAR*)"eepsm_download_rdeepromcmd_wait"; break;
    case eepsm_read_crc_area_cmd_done:      pszRet = (EC_T_CHAR*)"eepsm_download_rdeepromcmd_done"; break;
    case eepsm_read_crc_area_status:        pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_status"; break;
    case eepsm_read_crc_area_status_wait:   pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_status_wait"; break;
    case eepsm_read_crc_area_status_done:   pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_status_done"; break;
    case eepsm_read_crc_area_data:          pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_data"; break;
    case eepsm_read_crc_area_data_wait:     pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_data_wait"; break;
    case eepsm_read_crc_area_data_done:     pszRet = (EC_T_CHAR*)"eepsm_read_crc_area_data_done"; break;
    case eepsm_crc_area_eval_checksum:      pszRet = (EC_T_CHAR*)"eepsm_crc_area_eval_checksum"; break;
    case eepsm_write_checksum_cmd:          pszRet = (EC_T_CHAR*)"eepsm_write_checksum_cmd"; break;
    case eepsm_write_checksum_cmd_wait:     pszRet = (EC_T_CHAR*)"eepsm_write_checksum_cmd_wait"; break;
    case eepsm_write_checksum_cmd_done:     pszRet = (EC_T_CHAR*)"eepsm_write_checksum_cmd_done"; break;
    case epsm_write_checksum_status:        pszRet = (EC_T_CHAR*)"epsm_write_checksum_status"; break;
    case epsm_write_checksum_status_wait:   pszRet = (EC_T_CHAR*)"epsm_write_checksum_status_wait"; break;
    case epsm_write_checksum_status_done:   pszRet = (EC_T_CHAR*)"epsm_write_checksum_status_done"; break;
    case eepsm_reload_cmd:                  pszRet = (EC_T_CHAR*)"eepsm_reload_cmd"; break;
    case eepsm_reload_cmd_wait:             pszRet = (EC_T_CHAR*)"eepsm_reload_cmd_wait"; break;
    case eepsm_reload_cmd_done:             pszRet = (EC_T_CHAR*)"eepsm_reload_cmd_done"; break;
    case eepsm_reset_send:                  pszRet = (EC_T_CHAR*)"eepsm_reset_send"; break;
    case eepsm_reset_send_wait:             pszRet = (EC_T_CHAR*)"eepsm_reset_send_wait"; break;
    case eepsm_reset_send_done:             pszRet = (EC_T_CHAR*)"eepsm_reset_send_done"; break;
    case eepsm_AssignEEPROM:                pszRet = (EC_T_CHAR*)"eepsm_AssignEEPROM"; break;
    case eepsm_CheckEEPROMActive:           pszRet = (EC_T_CHAR*)"eepsm_CheckEEPROMActive"; break;
    case eepsm_UploadEEPROM:                pszRet = (EC_T_CHAR*)"eepsm_UploadEEPROM"; break;
    case eepsm_DownloadEEPROM:              pszRet = (EC_T_CHAR*)"eepsm_DownloadEEPROM"; break;
    case eepsm_ReloadEEPROM:                pszRet = (EC_T_CHAR*)"eepsm_ReloadEEPROM"; break;
    case eepsm_ResetSlaveController:        pszRet = (EC_T_CHAR*)"eepsm_ResetSlaveController"; break;
    case eepsm_reqto:                       pszRet = (EC_T_CHAR*)"eepsm_reqto"; break;
    case ecbtsm_link_disconnected:          pszRet = (EC_T_CHAR*)"ecbtsm_link_disconnected"; break;
    case eepsm_stuck:                       pszRet = (EC_T_CHAR*)"eepsm_stuck"; break;

    default:                                pszRet = (EC_T_CHAR*)"Unknown state"; break;
    }

    return pszRet;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
