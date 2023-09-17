/*-----------------------------------------------------------------------------
 * EcDeviceICSS.cpp
 * Copyright               acontis technologies GmbH, Weingarten, Germany
 * Response                Mikhail Ledyaev
 * Description             ICSS EtherCAT linklayer driver.
 *---------------------------------------------------------------------------*/

/*-INCLUDES-------------------------------------------------------------------*/
#include "LinkOsLayer.h"
#include <EcError.h>
#include <EcType.h>
#include <EcLink.h>
#include <EcCommon.h>
#include <EthernetServices.h>
#include "ICSS.h"


/*
 * Function: EcLinkOpen
 * -------------------------------
 *   Open Link Layer connection.
 *
 *   pvLinkParms[in]:            link parameters.
 *   pfReceiveFrameCallback[in]: pointer to rx callback function
 *   pfLinkNotifyCallback[in]:   pointer to notification callback function
 *   pvContext[in]:              caller context, to be used in callback functions
 *   ppvInstance[out]:           instance handle
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkOpen( EC_T_VOID* pvLinkParms,
                       EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback,
                       EC_T_LINK_NOTIFY pfLinkNotifyCallback,
                       EC_T_VOID*  pvContext,
                       EC_T_VOID** ppvInstance )
{
    EC_T_LINK_PARMS_ICSS*  pLinkParmsAdapter = (EC_T_LINK_PARMS_ICSS*) pvLinkParms;
    ICSS_ADAPTER_INTERNAL* pAdapter = EC_NULL;
    EC_T_DWORD             dwRetVal = EC_E_ERROR;

    EC_UNREFPARM(pfLinkNotifyCallback);

    //
    // Check parameters.
    //

    if (EC_NULL == ppvInstance)
    {
       SYS_ERRORMSGC(("EcLinkOpen(): No memory for Driver Instance handle provided (ppvInstance may not be EC_NULL)!\n"));

       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    else
    {
        *ppvInstance = EC_NULL;
    }
    if (EC_NULL == pLinkParmsAdapter)
    {
       SYS_ERRORMSGC(("EcLinkOpen(): Missing Configuration for ICSS Link Layer!\n"));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    if (EC_LINK_PARMS_SIGNATURE_ICSS != pLinkParmsAdapter->linkParms.dwSignature)
    {
       SYS_ERRORMSGC(("EcLinkOpen(): Invalid Configuration for ICSS Link Layer!\n"));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    if (EcLinkMode_INTERRUPT == pLinkParmsAdapter->linkParms.eLinkMode)
    {
       SYS_ERRORMSGC(("EcLinkOpen(): interrupt mode not supported!\n"));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    if ((1 > pLinkParmsAdapter->linkParms.dwInstance) || (4 < pLinkParmsAdapter->linkParms.dwInstance))
    {
       SYS_ERRORMSGC(("EcLinkOpen(): PRU core must be between 1 and 4!\n" ));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    if ((EcLinkIcssBoard_am572x_emerson == pLinkParmsAdapter->eBoardType) && 
         (3 > pLinkParmsAdapter->linkParms.dwInstance))
    {
       SYS_ERRORMSGC(("EcLinkOpen(): only second PRU, for am572x-emerson, instance must be 3 or 4!\n" ));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }
    if (EcLinkIcssBoard_am572x != pLinkParmsAdapter->eBoardType &&
        EcLinkIcssBoard_am571x != pLinkParmsAdapter->eBoardType &&
        EcLinkIcssBoard_am572x_emerson != pLinkParmsAdapter->eBoardType)
    {
       SYS_ERRORMSGC(("EcLinkOpen(): unknown board type specified!\n"));
       dwRetVal = EC_E_INVALIDPARM;
       goto Error;
    }

    //
    // Memory Allocation.
    //

    pAdapter = (ICSS_ADAPTER_INTERNAL*) LinkOsMalloc(sizeof(ICSS_ADAPTER_INTERNAL));
    if( pAdapter == EC_NULL )
    {
        goto Error;
    }
    LinkOsMemset(pAdapter, 0, sizeof(ICSS_ADAPTER_INTERNAL));
    LinkOsMemcpy(&(pAdapter->oInitParms), pLinkParmsAdapter, sizeof(EC_T_LINK_PARMS_ICSS));

    // Allocate receive buffers.
    dwRetVal = pAdapter->memoryPool.Initialize(ICSS_MEMORY_POOL_BUFFER_COUNT, ICSS_MEMORY_POOL_BUFFER_SIZE);
    if (EC_E_NOERROR != dwRetVal)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Error;
    }

    //
    // Do a memory mapping.
    //

    // Set board type.
    pAdapter->eBoardType = pLinkParmsAdapter->eBoardType;

    // Create context.
    pAdapter->pvLinkOsContext = LinkOsCreateContext((EC_T_VOID*)&pLinkParmsAdapter->linkParms);

    // Initialize platform and cache before any memory mapping.
    pAdapter->dwLosalHandle = LinkOsOpen();
    LinkOsPlatformInit();
    
    // Map IO memory.
    if( ICSS_MapIoMemory(pAdapter) == EC_FALSE )
    {
        SYS_ERRORMSGC(("EcLinkOpen(): Could not map IO memory using ATEMSYS driver!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Error;
    }

    // Set MAC address.
    LinkOsMemcpy(&pAdapter->abyMac, &pAdapter->abyMac, 6);

    // Configure PRUSS/PRU structures.
    pAdapter->dwPrussNum = (pLinkParmsAdapter->linkParms.dwInstance - 1)>>1;
    pAdapter->dwPruNum   = (pLinkParmsAdapter->linkParms.dwInstance - 1) - (pAdapter->dwPrussNum<<1);
    pAdapter->pPruss     = &pAdapter->platform.pruss[pAdapter->dwPrussNum];
    pAdapter->pPru       = &pAdapter->pPruss->pru[pAdapter->dwPruNum];
    pAdapter->pPru->bMaster = pLinkParmsAdapter->bMaster;
    
    //
    // Initialize board.
    //

    if( EC_FALSE == ICSS_InitializePruss(pAdapter) )
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Error;
    }

    if( EC_FALSE == ICSS_InitializePru(pAdapter) )
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Error;
    }
    
    *ppvInstance = pAdapter;

    return EC_E_NOERROR;

Error:
    if( pAdapter != EC_NULL )
    {
        pAdapter->memoryPool.Deinitialize();

        LinkOsFree(pAdapter);
    }
    return dwRetVal;
}

/*
 * Function: EcLinkClose
 * -------------------------------
 *   Close Link Layer connection.
 *
 *   pvInstance[in]:             port instance.
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkClose(EC_T_VOID* pvInstance)
{
   EC_T_DWORD             dwRetVal = EC_E_ERROR;
   ICSS_ADAPTER_INTERNAL *pAdapter = (ICSS_ADAPTER_INTERNAL*) pvInstance;

#if defined DEBUG
   if (EC_NULL == pAdapter )
   {
      return EC_E_INVALIDPARM;
   }
#endif

   ICSS_HaltPru(pAdapter);

   pAdapter->memoryPool.Deinitialize();
   ICSS_UnmapIoMemory(pAdapter);
   LinkOsFree(pAdapter);

   dwRetVal    = EC_E_NOERROR;

   return dwRetVal;
}

/*
 * Function: EcLinkSendFrame
 * -------------------------------
 *   Sends data frame.
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkSendFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    EC_T_DWORD             dwRetVal = EC_E_ERROR;
    ICSS_ADAPTER_INTERNAL *pAdapter = (ICSS_ADAPTER_INTERNAL*) pvInstance;

#if defined DEBUG
   if (EC_NULL == pAdapter)
   {
      return EC_E_INVALIDPARM;
   }
#endif

   if( EC_TRUE != ICSS_SendPacket(pAdapter, pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize) )
   {
       SYS_ERRORMSGC(("EcLinkSendFrame(): cannot send frame!\n"));
       dwRetVal = EC_E_SENDFAILED;
   }
   else
   {
       dwRetVal = EC_E_NOERROR;
   }

   return dwRetVal;
}

/*
 * Function: EcLinkSendAndFreeFrame
 * --------------------------------
 *   Send data frame and free the frame buffer. This function must be
 *   supported if EcLinkAllocSendFrame() is supported.
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkSendAndFreeFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    return EcLinkSendFrame(pvInstance, pLinkFrameDesc);
}

/*
 * Function: EcLinkRecvFrame
 * --------------------------------
 *   Poll for received frame. This function is called by the ethercat Master
 *   if the function EcLinkGetMode() returns EcLinkMode_POLLING.
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkRecvFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    ICSS_ADAPTER_INTERNAL *pAdapter    = (ICSS_ADAPTER_INTERNAL*) pvInstance;
    EC_T_DWORD dwRetVal             = EC_E_NOERROR;
    EC_T_DWORD dwPendingPacketSize  = 0;
    EC_T_BOOL  bMore                = EC_FALSE;

#if defined DEBUG
   if (EC_NULL == pAdapter)
   {
      dwRetVal = EC_E_INVALIDPARM;
      goto Exit;
   }
#endif

    dwPendingPacketSize = ICSS_ReceiveGetPendingPacketSize(pAdapter);
    if( dwPendingPacketSize != 0 )
    {
        if( EC_E_NOERROR != pAdapter->memoryPool.Alloc(&pLinkFrameDesc->pbyFrame) )
        {
            SYS_ERRORMSGC(("EcLinkRecvFrame(): warning, no more available memory.\n"));
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        pLinkFrameDesc->dwSize = ICSS_ReceivePacket(pAdapter, pLinkFrameDesc->pbyFrame, dwPendingPacketSize, &bMore);
        if( pLinkFrameDesc->dwSize == 0 )
        {
            dwRetVal = EC_E_INVALIDSTATE;
            SYS_ERRORMSGC(("EcLinkRecvFrame(): Cannot receive frame!.\n"));
            pAdapter->memoryPool.FreeBuffer(pLinkFrameDesc->pbyFrame);
            pLinkFrameDesc->dwSize = 0;
            pLinkFrameDesc->pbyFrame = EC_NULL;
            goto Exit;
        }
    }
    else
    {
        pLinkFrameDesc->dwSize = 0;
        pLinkFrameDesc->pbyFrame = EC_NULL;
    }

Exit:
    return dwRetVal;

}

/*
 * Function: EcLinkAllocSendFrame
 * --------------------------------
 *   If the link layer doesn't support frame allocation, this function must return
 *   EC_E_NOTSUPPORTED
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *   dwSize[in]:         size of the frame to allocate
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkAllocSendFrame(EC_T_VOID* pvInstance,
                                EC_T_LINK_FRAMEDESC* pLinkFrameDesc,
                                EC_T_DWORD dwSize )
{
    // We do not support frame allocation because it slows down sending functions and
    // does not give any advantages.

    return EC_E_NOTSUPPORTED;
}

/*
 * Function: EcLinkFreeSendFrame
 * --------------------------------
 *   Release a frame buffer previously allocated with EcLinkAllocFrame()
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *
 *   returns: EC_T_VOID.
 */
EC_T_VOID  EcLinkFreeSendFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
}

/*
 * Function: EcLinkFreeRecvFrame
 * --------------------------------
 *   Release a frame buffer given to the ethercat master through the receive
 *   callback function.
 *
 *   pvInstance[in]:     port instance.
 *   pLinkFrameDesc[in]: link frame descriptor.
 *
 *   returns: EC_T_VOID.
 */
EC_T_VOID EcLinkFreeRecvFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
{
    ICSS_ADAPTER_INTERNAL *pPort = (ICSS_ADAPTER_INTERNAL*) pvInstance;

#if defined DEBUG
   if (EC_NULL == pPort )
   {
      return;
   }
#endif

   if( pLinkFrameDesc->pbyFrame != EC_NULL )
   {
       pPort->memoryPool.FreeBuffer(pLinkFrameDesc->pbyFrame);

       pLinkFrameDesc->pbyFrame = EC_NULL;
   }
}

/*
 * Function: EcLinkGetEthernetAddress
 * ----------------------------------
 *   Determine link layer MAC address
 *
 *   pvInstance[in]:          port instance.
 *   pbyEthernetAddress[out]: receiving buffer for MAC address.
 *
 *   returns: LinkLayer Error code.
 */
EC_T_DWORD EcLinkGetEthernetAddress(EC_T_VOID* pvInstance, EC_T_BYTE* pbyEthernetAddress)
{
    ICSS_ADAPTER_INTERNAL *pPort = (ICSS_ADAPTER_INTERNAL*) pvInstance;

    LinkOsMemcpy(pbyEthernetAddress, pPort->abyMac, 6);

    return EC_E_NOERROR;
}

/*
 * Function: EcLinkGetStatus
 * ----------------------------------
 *   Determine current link status.
 *   This routine is called in the EtherCAT main cycle.
 *   eLinkStatus_HALFDUPLEX is not supported.
 *
 *   pvInstance[in]:     port instance.
 *
 *   returns: EC_T_LINKSTATUS
 */
EC_T_LINKSTATUS EcLinkGetStatus(EC_T_VOID* pvInstance)
{
    ICSS_ADAPTER_INTERNAL *pPort = (ICSS_ADAPTER_INTERNAL*) pvInstance;

 #if defined DEBUG
    if (EC_NULL == pPort)
    {
       return eLinkStatus_UNDEFINED;
    }
 #endif

    if( ICSS_UpdatePortLinkStatus(pPort) )
    {
        return eLinkStatus_OK;
    }
    else
    {
        return eLinkStatus_DISCONNECTED;
    }

    return eLinkStatus_UNDEFINED;
}

/*
 * Function: EcLinkGetSpeed
 * ----------------------------------
 *   Determine link speed.
 *
 *   pvInstance[in]:     port instance.
 *
 *   returns: ink speed in MBit/second
 */
EC_T_DWORD EcLinkGetSpeed(EC_T_VOID* pvInstance)
{

    return 100;
}

/*
 * Function: EcLinkGetMode
 * ----------------------------------
 *   Determine link mode
 *
 *   pvInstance[in]:     port instance.
 *
 *   returns: link mode
 */
EC_T_LINKMODE EcLinkGetMode(EC_T_VOID* pvInstance)
{
    return EcLinkMode_POLLING;
}

/*
 * Function: emllRegisterICSS
 * -------------------------------
 *   Registers link layer functions.
 *
 *   pLinkDrvDesc[out]:     structure with link layer function pointers.
 *   dwLinkDrvDescSize[in]: size in bytes of link layer driver descriptor.
 *
 *   returns: LinkLayer Error code.
 */
extern "C" EC_T_DWORD emllRegisterICSS(EC_T_LINK_DRV_DESC*  pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize)
{
   EC_T_DWORD dwResult = EC_E_NOERROR;

   if (pLinkDrvDesc == EC_NULL)
   {
      dwResult = EC_E_INVALIDPARM;
      goto Exit;
   }

   if (pLinkDrvDesc->dwValidationPattern != LINK_LAYER_DRV_DESC_PATTERN)
   {
      dwResult = EC_E_INVALIDPARM;
      goto Exit;
   }

   if (dwLinkDrvDescSize > sizeof(EC_T_LINK_DRV_DESC))
   {
      dwResult = EC_E_INVALIDPARM;
      goto Exit;
   }

   // Check if the version of the interface is supported .
   if ( !(sizeof(EC_T_LINK_DRV_DESC) == dwLinkDrvDescSize) ||
        !(pLinkDrvDesc->dwInterfaceVersion == LINK_LAYER_DRV_DESC_VERSION) )
   {
       dwResult = EC_E_INVALIDPARM;
       goto Exit;
   }

   pLinkDrvDesc->pfEcLinkOpen = EcLinkOpen;
   pLinkDrvDesc->pfEcLinkClose = EcLinkClose;
   pLinkDrvDesc->pfEcLinkSendFrame = EcLinkSendFrame;
   pLinkDrvDesc->pfEcLinkSendAndFreeFrame = EcLinkSendAndFreeFrame;
   pLinkDrvDesc->pfEcLinkRecvFrame = EcLinkRecvFrame;
   pLinkDrvDesc->pfEcLinkAllocSendFrame = EcLinkAllocSendFrame;
   pLinkDrvDesc->pfEcLinkFreeSendFrame = EcLinkFreeSendFrame;
   pLinkDrvDesc->pfEcLinkFreeRecvFrame = EcLinkFreeRecvFrame;
   pLinkDrvDesc->pfEcLinkGetEthernetAddress = EcLinkGetEthernetAddress;
   pLinkDrvDesc->pfEcLinkGetStatus = EcLinkGetStatus;
   pLinkDrvDesc->pfEcLinkGetSpeed = EcLinkGetSpeed;
   pLinkDrvDesc->pfEcLinkGetMode = EcLinkGetMode;
   pLinkDrvDesc->pfEcLinkIoctl = EC_NULL;
   
   /* store DBG Message hook */
   LinkOsAddDbgMsgHook(pLinkDrvDesc->pfOsDbgMsgHook);

Exit:
   return dwResult;
}


/*-----------------------------------------------------------------------------
 *
 *
 * TEST APPLICATION.
 *
 *
 *---------------------------------------------------------------------------*/

#if defined USE_ICSS_TEST

#define PRINT(msg, ...) SYS_ERRORMSGC(((msg), ##__VA_ARGS__))

static EC_T_BOOL DbgMsgHook(const EC_T_CHAR* szFormat, EC_T_LINKVALIST vaArgs)
   {
       EC_UNREFPARM(szFormat);
       EC_UNREFPARM(vaArgs);
#if !(defined INSTRUMENT_LL)
       vprintf(szFormat, vaArgs);
       fflush(stdout);
#endif
       return EC_FALSE; /* no need to output the message */
   }

/*
 * Forward declarations for internal test functions.
 */
EC_T_DWORD emllICSSTest(EC_T_LINK_PARMS_ICSS* pLinkParams);
EC_T_DWORD TestDbgMsgHook(struct _EC_T_LOG_CONTEXT* pContext, EC_T_DWORD dwLogMsgSeverity, const EC_T_CHAR* szFormat, ...);
EC_T_DWORD TestGetLinkLayerParams(int argc, char **argv, EC_T_LINK_PARMS_ICSS* pllParams);
EC_T_VOID  TestDumpUsage(int argc, char **argv);

/*
 * entry point.
 */
int main(int argc, char **argv)
{
    EC_T_LINK_PARMS_ICSS llParams = {0};
    EC_T_DWORD dwStatus = 0;

    dwStatus = TestGetLinkLayerParams(argc, argv, &llParams);
    if( dwStatus != EC_E_NOERROR)
    {
        return EC_E_ERROR;
    }
    if( emllICSSTest(&llParams) != EC_E_NOERROR )
    {
        return -1;
    }

    return 0;
}

#if defined(VXWORKS)
/// in VxWorks command line parameters are hard coded.
extern "C" int test(char* lpCmdLine)
{
    EC_T_LINK_PARMS_ICSS llParams = {0};
    
    LinkOsMemset(&llParams, 0, sizeof(EC_T_LINK_PARMS_ICSS));
    llParams.linkParms.dwInstance = 3;
    llParams.linkParms.eLinkMode  = EcLinkMode_POLLING;
    llParams.eBoardType = EcLinkIcssBoard_am571x;
    llParams.linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_ICSS;

    // Set MAC address.
    llParams.abyMac[0] = 0x01;
    llParams.abyMac[1] = 0xB4;
    llParams.abyMac[2] = 0xC3;
    llParams.abyMac[3] = 0xDD;
    llParams.abyMac[4] = 0xEE;
    llParams.abyMac[5] = 0xFF;
    
    emllICSSTest(&llParams);
    return 0;
}
#endif

/*
 * Function: emllICSSTest
 * ----------------------------------
 *   Main testing function.
 *
 *   How to test: 1. Connect desired PRUICSS 100 Mbit port with set of Beckhoff
 *                   EtherCAT slaves (EK1100,ES1104,ES2004 and etc).
 *                   For example port 4.
 *                2. Run test as "emllICSSTest 4 1".
 *
 *   What to expect: 25 EtherCAT frames are sent to the slaves and received back.
 *                   the modified answer is verified.
 *
 *
 *   returns: link layer error code.
 */
EC_T_DWORD emllICSSTest(EC_T_LINK_PARMS_ICSS* pLinkParams)
{
    EC_T_DWORD dwStatus     = EC_E_NOERROR;
    EC_T_PVOID pvLLInstance = EC_NULL;
    EC_T_LINK_FRAMEDESC  abyFrameDesc;

#if 1
    EC_T_BYTE abyTestPacket[] =
    {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   /* Dest Mac */                            /*0-5*/
       0xD0, 0xAD, 0xBE, 0xEF, 0xBA, 0xBE,   /* Src Mac */                             /*6-11*/
       0x88, 0xa4,                           /* Type ECAT */                           /*12-13*/
       0x0e, 0x10,                           /* E88A4 Length 0x0e, Type ECAT (0x1) */  /*14-15*/
       0x01,                                 /* APRD */                                /*16*/
       0x80,                                 /* Index */                               /*17*/
       0x00, 0x00,                           /* Slave Address */                       /*18-19*/
       0x30, 0x01,                           /* Offset Address */                      /*20-21*/
       0x04, 0x00,                           /* Length - Last sub command */           /*22-23*/
       0x00, 0x00,                           /* Interrupt */                           /*24-25*/
       0x00, 0x00,                           /* Data */                                /*26-27*/
       0x00, 0x00,                           /* Data 2 */                              /*28-29*/
       0x00, 0x00                            /* Working Counter */                     /*30-31*/
    };
#else
    EC_T_BYTE abyTestPacket[1428] =
    {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   /* Dest Mac */                            /*0-5*/
       0xD0, 0xAD, 0xBE, 0xEF, 0xBA, 0xBE,   /* Src Mac */                             /*6-11*/
       0x88, 0xa4,                           /* Type ECAT */                           /*12-13*/
       0x0e, 0x10,                           /* E88A4 Length 0x0e, Type ECAT (0x1) */  /*14-15*/
       0x01,                                 /* APRD */                                /*16*/
       0x80,                                 /* Index */                               /*17*/
       0x00, 0x00,                           /* Slave Address */                       /*18-19*/
       0x30, 0x01,                           /* Offset Address */                      /*20-21*/
       0x78, 0x05,                           /* Length - Last sub command */           /*22-23*/
       0x00, 0x00,                           /* Interrupt */                           /*24-25*/
       0x00, 0x00,                           /* Data */                                /*26-27*/
       0x00, 0x00,                           /* Data 2 */                              /*28-29*/
       0x00, 0x00                            /* Working Counter */                     /*30-31*/
    };
#endif

    LinkOsAddDbgMsgHook(DbgMsgHook);    

    printf("emllICSS test program\n");

    //
    // Open Link Layer.
    //

    dwStatus = EcLinkOpen( pLinkParams, EC_NULL, EC_NULL, EC_NULL, &pvLLInstance );
    if (dwStatus != EC_E_NOERROR)
    {
        printf( "emllICSSTest: error 0x%x in EcLinkOpen!\n", dwStatus );
       return dwStatus;
    }

    //
    // Wait for establishing link.
    //

    while( EcLinkGetStatus(pvLLInstance) != eLinkStatus_OK  )
    {
        printf( "emllICSSTest: Waiting for link on the PHY port...\n" );
        LinkOsSleep(1000);
    }

    //
    // Sending frame.
    //

    EC_T_DWORD  dwPacketSent    = 0, dwPacketReceived = 0;
    EC_T_DWORD  dwMaxSendCount  = 20;
    EC_T_BOOL   bGeneralFailure = EC_FALSE;

    while ((dwPacketSent < dwMaxSendCount || dwPacketReceived < dwMaxSendCount) && !bGeneralFailure)
    {
        if( dwPacketSent < dwMaxSendCount )
        {
            printf( "emllICSSTest: Sending frame %d...", dwPacketSent + 1 );
            abyFrameDesc.pbyFrame = abyTestPacket;
            abyFrameDesc.dwSize   = sizeof(abyTestPacket);
            if( EC_E_NOERROR == EcLinkSendFrame(pvLLInstance, &abyFrameDesc) )
            {
                dwPacketSent++;
                printf( "OK\n" );
            }
            else
            {
                printf( "FAILED\n" );
                break;
            }
        }

        LinkOsSleep(50);

        for(;;)
        {
            if( EcLinkRecvFrame(pvLLInstance, &abyFrameDesc) != EC_E_NOERROR )
            {
                printf( "Error during receive. Stop!\n" );
                bGeneralFailure = true;
                break;
            }
            if( abyFrameDesc.dwSize == 0 )
            {
                break;
            }
            else
            {
                // working counter changed?
                EC_T_WORD wCounter = *((EC_T_WORD*)(abyFrameDesc.pbyFrame + 30));
                if( wCounter == 0 )
                {
                    printf( "emllICSSTest: Receiving frame %d... wrong working counter\n", dwPacketReceived+1 );
                }
                // source mac is the same (except first byte)?
                else if( abyFrameDesc.pbyFrame[6] != (abyTestPacket[6] | 0x2))
                {
                    printf( "emllICSSTest: Receiving frame %d... wrong source MAC in packet\n", dwPacketReceived+1 );
                }
                else
                {
                    printf( "emllICSSTest: Receiving frame %d...OK\n", dwPacketReceived+1 );
                    dwPacketReceived ++;
                }

                EcLinkFreeRecvFrame(pvLLInstance, &abyFrameDesc);
            }
        }
    }

    if( dwPacketSent == dwPacketReceived )
    {
        printf("TESTS PASSED! Sent=%d Received=%d\n", dwPacketSent, dwPacketReceived);
    }
    else
    {
        printf("TESTS FAILED! Sent=%d Received=%d\n", dwPacketSent, dwPacketReceived);
    }

    //
    // Close LinkLayer
    //
    dwStatus = EcLinkClose( pvLLInstance );
    if( dwStatus != EC_E_NOERROR )
    {
        printf( "EcLinkClose returned error 0x%x\n", dwStatus );
       return dwStatus;
    }

   return EC_E_NOERROR;
}

/*
 * Function: TestGetLinkLayerParams
 * ----------------------------------
 *   Parses command line parameters and initializes link layer parameters.
 *
 *   argc, argv[in]: the same command line data passed from main() function.
 *   pllParams[out]: configured link layer parameters..
 *
 *
 *   returns: link layer error code.
 */
EC_T_DWORD TestGetLinkLayerParams(int argc, char **argv, EC_T_LINK_PARMS_ICSS* pllParams)
{
    if( argc != 4 )
    {
        TestDumpUsage(argc, argv);

        return EC_E_ERROR;
    }

    LinkOsMemset(pllParams, 0, sizeof(EC_T_LINK_PARMS_ICSS));
    pllParams->linkParms.dwInstance = (EC_T_DWORD)atoi(argv[1]);
    pllParams->linkParms.eLinkMode  = (EC_T_DWORD)atoi(argv[2]) == 0 ? EcLinkMode_INTERRUPT : EcLinkMode_POLLING;

    if( LinkOsStrcmp(argv[3], "am572x-idk") == 0 )
    {
        pllParams->eBoardType = EcLinkIcssBoard_am572x;
    }
    else if( LinkOsStrcmp(argv[3], "am571x-idk") == 0 )
    {
        pllParams->eBoardType = EcLinkIcssBoard_am571x;
    }
    else if( LinkOsStrcmp(argv[3], "am572x-emerson") == 0 )
    {
        pllParams->eBoardType = EcLinkIcssBoard_am572x_emerson;
    }
    else
    {
        printf("Unkown board specified: %s\n", argv[3]);

        return EC_E_ERROR;
    }

    if( pllParams->linkParms.dwInstance < 1 || pllParams->linkParms.dwInstance > 4 )
    {
        TestDumpUsage(argc, argv);

        return EC_E_ERROR;
    }

    if( pllParams->linkParms.eLinkMode !=  EcLinkMode_POLLING)
    {
        TestDumpUsage(argc, argv);

        printf("Only Poling mode is supported\n");

        return EC_E_ERROR;
    }

    pllParams->linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_ICSS;

    // Set MAC address.
    pllParams->abyMac[0] = 0x01;
    pllParams->abyMac[1] = 0xB4;
    pllParams->abyMac[2] = 0xC3;
    pllParams->abyMac[3] = 0xDD;
    pllParams->abyMac[4] = 0xEE;
    pllParams->abyMac[5] = 0xFF;


    return EC_E_NOERROR;
}

/*
 * Function: TestDumpUsage
 * ----------------------------------
 *   Prints information about test program arguments.
 *
 *   returns: link layer error code.
 */
EC_T_VOID TestDumpUsage(int argc, char **argv)
{
    printf(
       "Usage: emllICSSTest <Port> <IntFlag> <RefBoard>\n"
          "  Port:     1..4\n"
          "  IntFlag:  [0 | 1] 0 Polling, 1 Interrupt\n"
          "  RefBoard: am572x-idk | am571x-idk | am572x-emerson\n"
          "\n"
          );
}



#endif
