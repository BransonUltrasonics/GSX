/*-----------------------------------------------------------------------------
 * EcEEPRom.h               file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2008/7/25::12:25
 *---------------------------------------------------------------------------*/

#if !(defined __ECEEPROM_H__)
#define __ECEEPROM_H__  1

/*-INCLUDES------------------------------------------------------------------*/

/*-DEFINES-------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/
typedef enum _eepr_t_eepr_state
{
    eepr_Init               ,
    eepr_ReadStart          ,
    eepr_ReadCheckPreBusy   ,
    eepr_WriteReadCmd       ,
    eepr_PollReadCmpl       ,
    eepr_ReadDataValue      ,
    eepr_ReadDone           ,
    
    eepr_WriteStart         ,
    eepr_WriteCheckPreBusy  ,
    eepr_SendWriteCmd       ,
    eepr_PollWriteCmpl      ,
    eepr_WriteDone          ,

    eepr_AssignStart        ,
    eepr_AssignWrite        ,
    eepr_AssignDone         ,

    eepr_ActiveStart        ,
    eepr_ActiveDone         ,

    eepr_ReloadStart        ,
    eepr_ReloadCheckPreBusy ,
    eepr_WriteReloadCmd     ,
    eepr_PollReloadCmpl     ,
    eepr_ReloadDone         ,

    eepr_BCppDummy          = 0xffffffff
} EEPR_T_EEPR_STATE;

/*-CLASS---------------------------------------------------------------------*/

class CEcMaster;
class CEcDevice;

class CEcEEPRom
{
public:
    CEcEEPRom(                  CEcMaster*              pMaster,
                                CEcDevice*              pDevice                 );
    ~CEcEEPRom();
    
    EC_T_DWORD PrepareUpload(   EC_T_BOOL               bFixedAddressing
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_WORD               wEEPRomAddress
                               ,EC_T_WORD*              pwData
                               ,EC_T_DWORD              dwLen
                               ,EC_T_DWORD*             pdwUseData              
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_DWORD PrepareDownload( EC_T_BOOL               bFixedAddressing
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_WORD               wEEPRomAddress
                               ,EC_T_WORD*              pwData
                               ,EC_T_DWORD              dwLen
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_DWORD PrepareAssign(   EC_T_BOOL               bFixedAddressing
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_BOOL               bSlavePDIAccessEnable
                               ,EC_T_BOOL               bForceAssign
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_DWORD PrepareActive(   EC_T_BOOL               bFixedAddressing
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_BOOL*              pbSlavePDIAccessActive
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_DWORD PrepareReload(   EC_T_BOOL               bFixedAddressing        
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_DWORD PrepareReset(    EC_T_BOOL               bFixedAddressing        
                               ,EC_T_WORD               wSlaveAddress
                               ,EC_T_DWORD              dwTimeout               );

    EC_T_VOID  ProcessResult(   EC_T_DWORD              result
                               ,EC_T_DWORD              invokeId
                               ,PETYPE_EC_CMD_HEADER    pEcCmdHeader            );
    
    

private:
    EC_T_BYTE  CalcCRC(         EC_T_BYTE*              pbyData
                               ,EC_T_DWORD              dwLength                );

    CEcMaster*          m_pMaster;
    CEcDevice*          m_poEthDevice;

    EC_T_BOOL           m_bFixedAddrUsr;
    EC_T_WORD           m_wSlaveAddr;

    /* Up / Download */
    EC_T_WORD*          m_pwDataBase;
    EC_T_WORD*          m_pwCurData;
    EC_T_DWORD          m_dwDataBaseLength;
    EC_T_WORD           m_wEEPRomBaseAddr;
    EC_T_WORD           m_wEEPRomCurAddr;
    EC_T_DWORD*         m_pdwDataProcessed;
    EC_T_DWORD          m_dwDataProcessed;

    /* Assignment */
    EC_T_BOOL           m_bAssignEEPToPDI;
    EC_T_BOOL           m_bForceAssign;

    EC_T_BOOL*          m_pbActiveBuffer;

    EC_T_DWORD          m_dwEcatCmdPending;             /* number of pending ecat commands */
    EC_T_DWORD          m_dwEcatCmdSent;                /* number of pending ecat commands in a handling step */
    EC_T_DWORD          m_dwEcatCmdProcessed;           /* number of pending ecat commands in a handling step */

    EC_T_WORD           m_wEEPStatusReg;                /* last read of the eeprom status register (0x502)*/
    EC_T_WORD           m_wEEPConfigReg;                /* last read of the eeprom configuration register (0x500) */

    /* storage of EEPROM content for Checksum'd data in first 6 WORDs + 1 WORD Checksum */
    EC_T_DWORD          m_dwCurEEPAddr;
    EC_T_BYTE           m_abyEEPROMHead[(ESC_SII_REG_CHECKSUM*sizeof(EC_T_WORD))];
    EC_T_WORD           m_wChecksum;

public:
    typedef enum _ECEEPSMC_T_CMDSTATES
    {
        eceepsmc_idle               ,
        eceepsmc_pending            ,
        eceepsmc_retry              ,
        eceepsmc_error              ,
            
        eceepsmc_BCppDummy          = 0xFFFFFFFF
    } ECEEPSMC_T_CMDSTATES;
    

    EC_T_VOID EEPStateMachine(EC_T_VOID);

    typedef enum _ECEEP_T_EEPROMOPSTATE
    {
        eepsm_unknown                  ,
        eepsm_idle                     ,
        eepsm_start                    ,

        eepsm_initial_eep_check        ,
        eepsm_initial_eep_check_wait   ,
        eepsm_initial_eep_check_done   ,

        eepsm_initial_eep_clear_err    ,
        eepsm_initial_eep_clear_err_wait ,
        eepsm_initial_eep_clear_err_done ,
                                        
        eepsm_assign_rdassignment       ,
        eepsm_assign_rdassignment_wait  ,
        eepsm_assign_rdassignment_done  ,

        eepsm_assign_wrassignment       ,
        eepsm_assign_wrassignment_wait  ,
        eepsm_assign_wrassignment_done  ,

        eepsm_active_rdactive           ,
        eepsm_active_rdactive_wait      ,
        eepsm_active_rdactive_done      ,

        eepsm_checkassignment           ,
        eepsm_checkassignment_wait      ,
        eepsm_checkassignment_done      ,

        eepsm_upload_start              ,
        
        eepsm_upload_cmd                ,
        eepsm_upload_cmd_wait           ,
        eepsm_upload_cmd_done           ,

        eepsm_upload_data               ,
        eepsm_upload_data_wait          ,
        eepsm_upload_data_done          ,

        eepsm_download_start            ,
        
        eepsm_download_cmd              ,
        eepsm_download_cmd_wait         ,
        eepsm_download_cmd_done         ,

        eepsm_read_eep_status_reg       ,
        eepsm_read_eep_status_reg_wait  ,
        eepsm_read_eep_status_reg_done  ,

        eepsm_correct_crc_checksum      ,

        eepsm_read_crc_area_cmd         ,
        eepsm_read_crc_area_cmd_wait    ,
        eepsm_read_crc_area_cmd_done    ,

        eepsm_read_crc_area_status      ,
        eepsm_read_crc_area_status_wait ,
        eepsm_read_crc_area_status_done ,
        
        eepsm_read_crc_area_data        ,
        eepsm_read_crc_area_data_wait   ,
        eepsm_read_crc_area_data_done   ,

        eepsm_crc_area_eval_checksum    ,

        eepsm_write_checksum_cmd        ,
        eepsm_write_checksum_cmd_wait   ,
        eepsm_write_checksum_cmd_done   ,
        
        epsm_write_checksum_status      ,
        epsm_write_checksum_status_wait ,
        epsm_write_checksum_status_done ,
        
        eepsm_reload_cmd                ,
        eepsm_reload_cmd_wait           ,
        eepsm_reload_cmd_done           ,        

        eepsm_reset_send                ,
        eepsm_reset_send_wait           ,
        eepsm_reset_send_done           ,

        eepsm_AssignEEPROM              ,
        eepsm_CheckEEPROMActive         ,
        eepsm_UploadEEPROM              ,
        eepsm_DownloadEEPROM            ,
        eepsm_ReloadEEPROM              ,
        eepsm_ResetSlaveController      ,
                                        
        eepsm_reqto                     ,
        ecbtsm_link_disconnected        ,
        eepsm_stuck                     ,
                                        
        eepsm_BCppDummy                 = 0xFFFFFFFF
    } ECEEP_T_EEPROMOPSTATE;
    EC_T_CHAR* EEPSMSTATESText(ECEEP_T_EEPROMOPSTATE eState);

    typedef struct _ECEEPSM_REQ
    {
        ECEEP_T_EEPROMOPSTATE   eState;
        CEcTimer          oTimeout;
        EC_T_DWORD              dwResult;
        EC_T_BOOL               bUsed;
    } ECEEPSM_REQ;

    EC_T_DWORD  RequestEEPROMOpReq(     ECEEP_T_EEPROMOPSTATE   eState          
                                       ,EC_T_DWORD              dwTimeout       
                                       ,ECEEPSM_REQ**           pHandle                     );
    EC_T_DWORD  StartEEPROMOP(          EC_T_VOID                                           );
    EC_T_DWORD  PollEEPROMOp(                                                               );
    EC_T_DWORD  PollEEPROMOpReq(        ECEEPSM_REQ*            pHandle                     );
    EC_T_DWORD  ReleaseEEPROMOpReq(     ECEEPSM_REQ*            pHandle                     );

private:
    /* Request buffers */
    ECEEPSM_REQ             m_oRequest[2];
    /* Queued Request */
    ECEEPSM_REQ*            m_pRequest;
    /* Current Running Request */
    ECEEPSM_REQ*            m_pCurRequest;
    
    /* Current State */
    ECEEP_T_EEPROMOPSTATE   m_eCurState;
    /* Requested State */
    ECEEP_T_EEPROMOPSTATE   m_eReqState;

    /* Handle to current running Port operation */
    ECEEPSM_REQ*            m_pCurrentEEPOpReq;

    /* State of current command(s) */
};

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/


#endif  /* __ECEEPROM_H__ */ 
/*-END OF SOURCE FILE--------------------------------------------------------*/
