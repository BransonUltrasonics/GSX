/**
\addtogroup CoE CAN Application Profile over EtherCAT
@{
*/

/**
\file ecatcoe.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains the CoE mailbox interface

\version 5.11

<br>Changes to version V5.0:<br>
V5.11 COE4: "change prototype of ""COE_ContinueInd()"" return <> 0 if a failure occurred"<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 TEST6: add test object to trigger Slave-to-Slave communication<br>
V5.11 TEST7: add test behaviour to send an emergency on every SDO request (in SafeOP)<br>
<br>Changes to version V4.40:<br>
V5.0 SDO7: "SDOS_SdoInfoInd()" never return pending SDO Info service. Delete "NOERROR_INWORK" handling.<br>
<br>Changes to version V4.08:<br>
V4.40 SDO1: add initial value for "nSdoInfoFragmentsLeft"<br>
V4.40 MBX6: change return value if no mailbox buffer is available<br>
V4.08 MBX 1: If the switch MAILBOX_QUEUE was set, we have to put all SDO Info Responses in the Send Queue
*/

/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include <ecat_def.h>

#if COE_SUPPORTED

#include <sdoserv.h>
#define    _ECATCOE_    1
#include <ecatcoe.h>
#undef      _ECATCOE_
/* ECATCHANGE_START(V5.11) ECAT10*/
/*remove definition of _ECATCOE_ (#ifdef is used in ecatcoe.h)*/
/* ECATCHANGE_END(V5.11) ECAT10*/

#if TEST_APPLICATION
#include <testappl.h>

#if EMERGENCY_SUPPORTED
/*ECATCHANGE_START(V5.11) TEST7*/

#include <emcy.h>
#include <ecatslv.h>
/*ECATCHANGE_END(V5.11) TEST7*/
#endif
#endif


/*---------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
---------------------------------------------------------------------------------------*/

#define    ECATCOE        0x4300
#define    ECATCOEMAX    0x02

/*---------------------------------------------------------------------------------------
------
------    static variables
------
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    static functions
------
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    functions
------
---------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function intialize the CoE Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void COE_Init(void)
{
    pCoeSendStored = 0;
#if SEGMENTED_SDO_SUPPORTED
    nSdoInfoFragmentsLeft = 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pCoeMbx      Pointer to the received mailbox data from the master.

 \return    result of the operation (0 (success) or mailbox error code (MBXERR_.... defined in
            mailbox.h))

 \brief    This function is called when a CoE (CAN application layer over EtherCAT) service is received from
             the master.
*////////////////////////////////////////////////////////////////////////////////////////

ESS_UINT8 COE_ServiceInd(TCOEMBX MBXMEM *pCoeMbx)
{
    ESS_UINT8 result = 0;

    switch ((pCoeMbx->CoeHeader & COEHEADER_COESERVICEMASK) >> COEHEADER_COESERVICESHIFT)
    {
    case COESERVICE_SDOREQUEST:
        /* SDO-Request received, call SDOS_SdoInd to process the SDO-Request
           if an existing SDO-Stack shall be used, the corresponding function
            should be called */
#if TEST_APPLICATION && EMERGENCY_SUPPORTED
/*ECATCHANGE_START(V5.11) TEST7*/
        if(IS_TEST_ACTIVE(EmergencyOnSdoRequest_Test) && bEcatInputUpdateRunning && !bEcatOutputUpdateRunning)
        {
            TEMCYMESSAGE EMCYMEM *    pEmcy    = EMCY_GetEmcyBuffer();
            if ( pEmcy )
            {
                pEmcy->Emcy.Code = SWAPWORD(0x1000);

                pEmcy->Emcy.RegData[EMCY_OFFS_ERRORREGISTER] &= ~EMCY_MASK_ERRORREGISTER;
                pEmcy->Emcy.RegData[EMCY_OFFS_ERRORREGISTER] |= 0x01 << EMCY_SHIFT_ERRORREGISTER;


                /* the faultyCode is stored in byte 3 of the Emergency */
                pEmcy->Emcy.RegData[EMCY_OFFS_DIAGCODE] &= ~EMCY_MASK_DIAGCODE;
                pEmcy->Emcy.RegData[EMCY_OFFS_DIAGCODE] |= 0x00 << EMCY_SHIFT_DIAGCODE;
                
                pEmcy->Emcy.RegData[EMCY_OFFS_DIAGDATA] = SWAPWORD(0x0001);
                pEmcy->Emcy.RegData[EMCY_OFFS_DIAGDATA+1] = SWAPWORD(0x0BD4);




                EMCY_SendEmergency(pEmcy);

                INC_TEST_CNT(EmergencyOnSdoRequest_Test);
            }
        }
/*ECATCHANGE_END(V5.11) TEST7*/
#endif
        result = SDOS_SdoInd( (TINITSDOMBX MBXMEM *) pCoeMbx );
        break;

    case COESERVICE_SDOINFO:
        /* SDO-Information Request received, call SDOS_SdoInfoInd to process the SDO-Request */
        result = SDOS_SdoInfoInd( (TSDOINFORMATION MBXMEM *) pCoeMbx );
        /*NOERROR_INWORK is never returned by SDOS_SdoInfoInd() => delete return code handling*/
        break;


    case COESERVICE_SDORESPONSE:
#if TEST_APPLICATION
/*ECATCHANGE_START(V5.11) TEST6*/
        if((sObj300D_S2S.Status == 1) && (pCoeMbx->MbxHeader.Address == sObj300D_S2S.DstSlv))
        {
            //S-2-S upload response of ox1018.2 was received
            sObj300D_S2S.IdentData = 0;
            sObj300D_S2S.IdentData = pCoeMbx->Data[2];
            sObj300D_S2S.IdentData |= ((ESS_UINT32)pCoeMbx->Data[3]) << 16;
            //HMEMCPY(&sObj300D_S2S.IdentData,&pCoeMbx->Data[2],SIZEOF(sObj300D_S2S.IdentData));

            sObj300D_S2S.Status = 2;
        }
/*ECATCHANGE_END(V5.11) TEST6*/
#endif
    case COESERVICE_EMERGENCY:
    case COESERVICE_TXPDO:
    case COESERVICE_RXPDO:
    case COESERVICE_TXPDOREMREQ:
    case COESERVICE_RXPDOREMREQ:
        /* these CoE services are not supported yet */
        result = MBXERR_SERVICENOTSUPPORTED;
        break;

    default:
        result = MBXERR_INVALIDHEADER;
        break;
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx      Pointer to the free mailbox to sent.

 \return    result of the operation (0 (success)

 \brief    This function is called when a CoE service to be sent is stored and can
 \brief  be put in the send mailbox.
*////////////////////////////////////////////////////////////////////////////////////////

/*ECATCHANGE_START(V5.11) COE4*/
ESS_UINT8 COE_ContinueInd(TMBX MBXMEM * pMbx)
/*ECATCHANGE_END(V5.11) COE4*/
{
    if (pCoeSendStored)
    {
        /* send the stored CoE service which could not be sent before */
        MBX_MailboxSendReq(pCoeSendStored, 0);
        pCoeSendStored = 0;
    }
    else
    {
        /* send the next fragment of the last CoE service (only for SDO-Information possible) */
#if MAILBOX_QUEUE
        /* in mailbox queue mode pMbx is always 0, so a mailbox buffer shall be get */
        pMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(SIZEOF(TMBX));
        /* it shall be checked if a valid pointer was returned */
/*ECATCHANGE_START(V5.11) COE4*/
        if (pMbx == NULL)
        {
            return MBXERR_NOMOREMEMORY;
        }
        else
/*ECATCHANGE_END(V5.11) COE4*/
#endif
        {
            /* copy the stored SDO-Info-Header in the request */
            MBXMEMCPY(pMbx, aSdoInfoHeader, SDO_INFO_HEADER_BYTE_SIZE);
            /* call SDOS_SdoInfoInd to generate and send the next fragment */
            SDOS_SdoInfoInd( (TSDOINFORMATION MBXMEM *) pMbx );
        }
    }

    return 0;
}

/** @} */

#endif /* COE_SUPPORTED */


