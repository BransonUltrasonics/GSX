/*-----------------------------------------------------------------------------
 * ICSS.cpp             source file
 * Copyright            acontis technologies GmbH, Weingarten, Germany
 * Response             Mikhail Ledyaev
 * Description          Hardware/Firmware low level functions.
 *---------------------------------------------------------------------------*/

#include "ICSS.h"
#include <EcError.h>
#include <EcType.h>
#include <EcLink.h>
#include <EcCommon.h>

/*-LOGGING-------------------------------------------------------------------*/
#if (defined INCLUDE_LOG_MESSAGES)
#define pLogMsgCallback       (pEcLogParmsLL)->pfLogMsg
#define pEcLogContext         (struct _EC_T_LOG_CONTEXT*)(pEcLogParmsLL)->pLogContext
#define dwEcLogLevel          (pEcLogParmsLL)->dwLogLevel
#endif

/*
 * Constants.
 */

#define DEFAULT_TIME_TO_WAIT_FOR_CONNECTION                   2000


/*
 * Forward declarations for internal functions.
 */
static EC_T_BOOL ICSS_PRUSS_MapIoMem(ICSS_ADAPTER_INTERNAL* pAdapter,
                                       ICSS_PRUSS_CONFIGURATION* pPruss,
                                       EC_T_DWORD dwPruss);
static EC_T_BOOL ICSS_MapMemoryBank(ICSS_ADAPTER_INTERNAL* pAdapter,
                                const EC_T_CHAR* pszBankName, ICSS_IO_ADDRESS* pIoAddress);

static EC_T_VOID ICSS_PRUSS_UnmapIoMem(ICSS_ADAPTER_INTERNAL* pAdapter,
                                       ICSS_PRUSS_CONFIGURATION* pPruss);

static EC_T_VOID ICSS_UnmapMemoryBank(ICSS_ADAPTER_INTERNAL* pAdapter,
                                      ICSS_IO_ADDRESS* pIoAddress);

static EC_T_VOID ICSS_BOARD_WakeUp(ICSS_BOARD_CONFIGURATION *pBoard);
static EC_T_VOID ICSS_PRUSS_WakeUp(ICSS_BOARD_CONFIGURATION *pBoard, EC_T_DWORD dwPrussNum);
static EC_T_VOID ICSS_PRUSS_ClearMemory(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_InitializeGpiMode(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_InitializeGpMux(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_InitializeSetIepClockSource(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_InitializeEnableMiiRtMapping(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_InitializeEnableSppXfrShift(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_INTC_MapInterrupts(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_EnableIepCounter(ICSS_ADAPTER_INTERNAL* pPort);

static EC_T_VOID ICSS_PRUSS_MDIO_InitializeStateMachine(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_MDIO_EnableLinkInterrupt(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_BOOL ICSS_PRUSS_MDIO_GetLinkStatus(uint32_t baseAddr, uint32_t phyAddr);

static EC_T_VOID ICSS_PRUSS_INTC_MapInterruptToChannel( EC_T_DWORD dwSystemInterrupt,
                                         EC_T_DWORD dwIntcChannel,
                                         EC_T_DWORD dwIntcBaseAddr);
static EC_T_VOID ICSS_PRUSS_INTC_MapChannelToInterrupt( EC_T_DWORD  dwIntcChannel,
                                         EC_T_DWORD  dwHostInterrupt,
                                         EC_T_DWORD  dwBaseAddr);

static EC_T_VOID ICSS_PRUSS_PRU_Halt(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_PRU_ClearMemory(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_PRU_InitializeMiiRt(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_PRU_InitializeConstantTable(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_PRU_LoadFirmware(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_PRUSS_PRU_Start(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_BOOL ICSS_PRUSS_PRU_WaitForStableConnection(ICSS_ADAPTER_INTERNAL* pPort, EC_T_DWORD dwMaxWaitTimeUsec);
static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo);
static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am572x(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo);
static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am571x(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo);

static EC_T_VOID ICSS_FIRMWARE_SetMacAddr(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_FIRMWARE_InitializeReceiveQueues(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_FIRMWARE_InitializeSendQueues(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_FIRMWARE_EnableRx(ICSS_ADAPTER_INTERNAL* pPort);
static EC_T_VOID ICSS_FIRMWARE_SetPortSpeed(ICSS_ADAPTER_INTERNAL* pPort, PHY_SPEED speed);

static EC_T_VOID ICSS_GPIO_InitializePinmux(ICSS_ADAPTER_INTERNAL* pPort);
static PHY_SPEED ICSS_PHY_GetSpeed(ICSS_ADAPTER_INTERNAL* pPort);


/*-----------------------------------------------------------------------------
 *
 *
 * Public Functions.
 *
 *
 *---------------------------------------------------------------------------*/

/*
 * Function: ICSS_MapIoMemory
 * -----------------------------------------------
 *   Maps all needed board memory into user space.
 *
 *   returns: EC_TRUE, if memory has been mapped.
 */
EC_T_BOOL ICSS_MapIoMemory(ICSS_ADAPTER_INTERNAL* pAdapter)
{
    ICSS_BOARD_CONFIGURATION *configuration = &pAdapter->platform;
    ICSS_BOARD_CONFIGURATION physMemory = AM57XX_HARDWARE_INITDATA;
    
    *configuration = physMemory;

    //
    // MPU.
    //

    if( !ICSS_MapMemoryBank(pAdapter, "gpio3", &configuration->mpuGpio3) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "gpio4", &configuration->mpuGpio4) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "gpio5", &configuration->mpuGpio5) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "SCM", &configuration->mpuScm) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "CM_CORE(PRCM)", &configuration->cmCore) )
    {
        return EC_FALSE;
    }

    //
    // PRUSS1 & PRUSS2.
    //

    if( EC_FALSE == ICSS_PRUSS_MapIoMem(pAdapter, &configuration->pruss[0], 0) )
    {
        return EC_FALSE;
    }

    if( EC_FALSE == ICSS_PRUSS_MapIoMem(pAdapter, &configuration->pruss[1], 1) )
    {
        return EC_FALSE;
    }

    return EC_TRUE;
}

/*
 * Function: ICSS_UnmapIoMemory
 * -----------------------------------------------
 *   Unmaps all board memory from user space.
 *
 *   returns: void.
 */
EC_T_VOID ICSS_UnmapIoMemory(ICSS_ADAPTER_INTERNAL* pAdapter)
{
    ICSS_BOARD_CONFIGURATION *configuration = &pAdapter->platform;

    //
    // MPU.
    //

    ICSS_UnmapMemoryBank(pAdapter, &configuration->mpuScm);
    ICSS_UnmapMemoryBank(pAdapter, &configuration->mpuGpio3);
    ICSS_UnmapMemoryBank(pAdapter, &configuration->mpuGpio5);

    //
    // PRUSS1 & PRUSS2.
    //

    ICSS_PRUSS_UnmapIoMem(pAdapter,  &configuration->pruss[0]);
    ICSS_PRUSS_UnmapIoMem(pAdapter,  &configuration->pruss[1]);
}

/*
 * Function: ICSS_InitializePruss
 * -------------------------------
 *   Initializes PRUSS if not yet initialized in another instance.
 *
 *   pPort[in]: port information structure.
 *
 *   returns: EC_TRUE, if PRUSS was initialized.
 */
EC_T_BOOL ICSS_InitializePruss(ICSS_ADAPTER_INTERNAL *pPort)
{
    if( pPort->pPru->bMaster )
    {
        ICSS_BOARD_WakeUp(&pPort->platform);
        ICSS_PRUSS_ClearMemory(pPort);
        ICSS_PRUSS_InitializeSetIepClockSource(pPort);
        ICSS_PRUSS_InitializeEnableMiiRtMapping(pPort);
        ICSS_PRUSS_InitializeEnableSppXfrShift(pPort);
        ICSS_PRUSS_INTC_MapInterrupts(pPort);
        ICSS_PRUSS_EnableIepCounter(pPort);
        ICSS_PRUSS_MDIO_InitializeStateMachine(pPort);
    }

    return EC_TRUE;
}

/*
 * Function: ICSS_InitializePru
 * -------------------------------
 *   Initializes PRU.
 *   Initializes queues, loads firmware, starts pru.
 *
 *   pPort[in]: port information structure.
 *
 *   returns: EC_TRUE, if PRUSS was initialized.
 */
EC_T_BOOL ICSS_InitializePru(ICSS_ADAPTER_INTERNAL *pPort)
{
    ICSS_HaltPru(pPort);
    ICSS_PRUSS_PRU_ClearMemory(pPort);
    ICSS_PRUSS_PRU_InitializeMiiRt(pPort);
    ICSS_PRUSS_PRU_InitializeConstantTable(pPort);
    ICSS_PRUSS_MDIO_EnableLinkInterrupt(pPort);
    ICSS_GPIO_InitializePinmux(pPort);
    ICSS_PRUSS_InitializeGpMux(pPort);
    ICSS_PRUSS_InitializeGpiMode(pPort);

    ICSS_FIRMWARE_SetMacAddr(pPort);
    ICSS_FIRMWARE_InitializeReceiveQueues(pPort);
    ICSS_FIRMWARE_InitializeSendQueues(pPort);
    ICSS_FIRMWARE_EnableRx(pPort);

    ICSS_PRUSS_PRU_LoadFirmware(pPort);
    ICSS_PRUSS_PRU_Start(pPort);
    ICSS_PRUSS_PRU_WaitForStableConnection(pPort, DEFAULT_TIME_TO_WAIT_FOR_CONNECTION);

    return EC_TRUE;
}

/*
 * Function: ICSS_HaltPru
 * ---------------------------------------------------
 * Halts PRU.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
EC_T_VOID ICSS_HaltPru(ICSS_ADAPTER_INTERNAL* pPort)
{
    ICSS_PRUSS_PRU_Halt(pPort);
}

/*
 * Function: ICSS_UpdatePortLinkStatus
 * ---------------------------------------------------
 * Checks for raised link system interrupts and resets them.
 * Updates internal bLink flag in the port structure.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
EC_T_BOOL ICSS_UpdatePortLinkStatus(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwIntcAddr = pPort->pPruss->interruptController.dwVirt;
    EC_T_DWORD dwMdioAddr = pPort->pPruss->mdio.dwVirt;
    EC_T_DWORD dwTempAddr, dwTempVal;

    EC_T_DWORD  dwSystemInterruptValueAddr = (EC_T_DWORD)(dwIntcAddr +
                                                          PRUSS_INTC_SECR1);
    EC_T_DWORD  dwInterruptMask  = (pPort->dwPruNum == 0 ?
                                    PRUSS_INTC_SECR1_LINK0_PRU_EVT_MASK :
                                    PRUSS_INTC_SECR1_LINK1_PRU_EVT_MASK);

    EC_T_BYTE*  pbyFwPortStatus  = (EC_T_BYTE*)(pPort->pPru->dram.dwVirt +
                                                FIRMWARE_DRAM_PORT_STATUS_OFFSET);
    EC_T_BYTE*  pbyFwPortControl = (EC_T_BYTE*)(pPort->pPru->dram.dwVirt +
                                                FIRMWARE_DRAM_PORT_CONTROL_OFFSET);

    // System interrupt raised?
    if( (HW_RD_REG32(dwSystemInterruptValueAddr) & dwInterruptMask) > 0 )
    {
        // MDIO_LINKINTRAW
        // Clear link interrupt in PRUSS MDIO for the specified MDIO port
        // (0x1 - port 0, 0x2 - port 1).
        dwTempAddr = dwMdioAddr + PRUSS_MDIO_LINKINTMASKED;
        HW_WR_REG32(dwTempAddr, pPort->dwPruNum+1);

        // Issue read to make sure value written has taken affect,
        // if not re-read once again.
        dwTempVal = HW_RD_REG32(dwTempAddr);
        if ((dwTempVal & (pPort->dwPruNum+1)) != 0U)
        {
            HW_RD_REG32(dwTempAddr);
        }

        // Set 1 to the event mask to clear status of system INTC interrupt.
        // Re-read again, to make sure value written has taken affect.
        HW_WR_REG32(dwSystemInterruptValueAddr, dwInterruptMask);
        dwTempVal = HW_RD_REG32(dwSystemInterruptValueAddr);

        if ((dwTempVal & dwInterruptMask) != 0U)
        {
            HW_RD_REG32(dwSystemInterruptValueAddr);
        }

        pPort->bLink = ICSS_PRUSS_MDIO_GetLinkStatus(dwMdioAddr, pPort->dwPruNum);

        // Let firmware resume/stop receiving/transmitting data via the port.
        if( pPort->bLink )
        {
            *pbyFwPortControl = FIRMWARE_PORT_CONTROL_ENABLE;
            *pbyFwPortStatus  = 1;

            // Set firmware port speed.
            PHY_SPEED wPhySpeed = ICSS_PHY_GetSpeed(pPort);
            ICSS_FIRMWARE_SetPortSpeed(pPort, wPhySpeed);
        }
        else
        {
            *pbyFwPortControl = FIRMWARE_PORT_CONTROL_DISABLE;
            *pbyFwPortStatus  = 0;
        }
    }

    return pPort->bLink;
}

/*
 * Function: ICSS_SendPacket
 * ---------------------------------------------------
 * Send frame.
 *
 * pPort[in]:          pointer to port structure.
 * pbyPacket[out]:     pointer to data block with frame.
 * dwPacketLength[in]: size of the packet to be sent.
 *
 * returns: bool, if packet has been successfully queued.
 */
EC_T_BOOL ICSS_SendPacket(ICSS_ADAPTER_INTERNAL* pAdapter, EC_T_BYTE* pbyPacket,
                          EC_T_DWORD dwPacketLength)
{
    if( dwPacketLength > FIRMWARE_MAXMTU)
    {
        SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): packetLength is too big.\n"));
        return EC_FALSE;
    }
    if( dwPacketLength < FIRMWARE_MINMTU)
    {
        SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): packetLength is short.\n"));
        return EC_FALSE;
    }
    if( pAdapter->bLink != EC_TRUE )
    {
        SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): no connection.\n"));
        return EC_FALSE;
    }

    EC_T_DWORD dwPriority            = 0; // In this implementation we do not use
                                          // other firmware packet priorities.
    ICSS_QUEUE_INFORMATION *pQueue = &pAdapter->sendQueue[dwPriority];
    EC_T_DWORD         dwDramBase  = pAdapter->pPru->dram.dwVirt;
    EC_T_DWORD         dwQueueDescriptorAddr = dwDramBase + pQueue->wQueueDescriptorsOffset;
    T_FIRMWARE_QUEUE_DESCRIPTOR* pQueueDescriptor      = (T_FIRMWARE_QUEUE_DESCRIPTOR*) dwQueueDescriptorAddr;

    // Set busy_s flag for the queue, so firmware should not touch it.
    HW_WR_REG8(&pQueueDescriptor->byBusy, 1);

    EC_T_BOOL bPacketPadding   = EC_FALSE;
    EC_T_WORD wOriginalLengthOfPacket = 0;

    if(dwPacketLength < FIRMWARE_MINIMUM_ETHERNET_FRAME_SIZE)
    {
        wOriginalLengthOfPacket = dwPacketLength;
        bPacketPadding          = EC_TRUE;
        dwPacketLength = FIRMWARE_MINIMUM_ETHERNET_FRAME_SIZE;
    }


    // Determine count of required 32-byte blocks to store the packet in the pQueue.
    EC_T_DWORD dwPacketBufferCount = (dwPacketLength>>5);
    if((dwPacketLength & 0x1f) != 0)
    {
        dwPacketBufferCount++;
    }

    EC_T_DWORD dwWriteBdOffset = HW_GET16_NO_BARRIER(&pQueueDescriptor->wWriteBdOffset);
    EC_T_DWORD dwReadBdOffset  = HW_GET16_NO_BARRIER(&pQueueDescriptor->wReadBdOffset);
    EC_T_DWORD dwNewWriteBdOffset = dwWriteBdOffset + dwPacketBufferCount*4;

    // No more space in the Bd Table?
    if( ((dwWriteBdOffset + 4) % pQueue->wBdTableEndOffset) == 0 )
    {
        if(dwReadBdOffset == pQueue->wBdTableOffset)
        {
            HW_WR_REG8(&pQueueDescriptor->byBusy, 0);
            SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): no more space in sending pQueue.\n"));
            return EC_FALSE;
        }
    }
    if( (dwWriteBdOffset + 4) == dwReadBdOffset)
    {
        HW_WR_REG8(&pQueueDescriptor->byBusy, 0);
        SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): no more space in sending pQueue.\n"));
        return EC_FALSE;
    }
    if( dwReadBdOffset == dwWriteBdOffset )
    {
        if( dwNewWriteBdOffset >= pQueue->wBdTableEndOffset )
        {
            dwNewWriteBdOffset = (dwNewWriteBdOffset % pQueue->wBdTableEndOffset);
            dwNewWriteBdOffset = dwNewWriteBdOffset + pQueue->wBdTableOffset;
        }
    }
    else if(dwWriteBdOffset > dwReadBdOffset)
    {
        if( dwNewWriteBdOffset >= pQueue->wBdTableEndOffset )
        {
            dwNewWriteBdOffset = (dwNewWriteBdOffset % pQueue->wBdTableEndOffset);
            dwNewWriteBdOffset = dwNewWriteBdOffset + pQueue->wBdTableOffset;

            if( dwNewWriteBdOffset >= dwReadBdOffset)
            {
                HW_WR_REG8(&pQueueDescriptor->byBusy, 0);
                SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): no more space in sending pQueue.\n"));

                return EC_FALSE;
            }
        }
    }
    else
    {
        if( dwNewWriteBdOffset >= dwReadBdOffset )
        {
            HW_WR_REG8(&pQueueDescriptor->byBusy, 0);
            SYS_ERRORMSGC(("ICSS_FIRMWARE_SendPacket(): no more space in sending pQueue.\n"));

            return EC_FALSE;
        }
    }

    EC_T_DWORD dwOcmcBufferOffset = pQueue->wBufferOffset +
                                    (dwWriteBdOffset - pQueue->wBdTableOffset)*8;
    EC_T_DWORD dwOcmcBufferAddr   = pAdapter->pPruss->l3ocmc.dwVirt + dwOcmcBufferOffset;
    EC_T_DWORD i = 0;

    // Check if queue wrap around has happened.
    // If yes then data can't be stored sequentially.
    if( (dwNewWriteBdOffset < dwWriteBdOffset) &&
        (dwNewWriteBdOffset != pQueue->wBdTableOffset) )
    {
        // Size of buffer from the write position to the end of buffer (in bytes).
        EC_T_DWORD dwRemainingBufferSize = ((pQueue->wBdTableEndOffset - dwWriteBdOffset)>>2)*32;

        if( EC_TRUE == bPacketPadding )
        {
            // Check if Padding has to be done. If yes then Pad with Zero's to reach
            // the minimum size for the Ethernet frame.
            if( dwRemainingBufferSize <= wOriginalLengthOfPacket )
            {
                LinkOsMemcpy(dwOcmcBufferAddr, pbyPacket, dwRemainingBufferSize);
            }
            else
            {
                // Copy the valid packet data first and then Pad with zero's.
                LinkOsMemcpy(dwOcmcBufferAddr, pbyPacket, wOriginalLengthOfPacket);
                for(i=0; i< (dwRemainingBufferSize - wOriginalLengthOfPacket); i++)
                {
                    HW_SET8_NO_BARRIER(dwOcmcBufferAddr + wOriginalLengthOfPacket + i, 0);
                }
            }
        }
        else
        {
            LinkOsMemcpy(dwOcmcBufferAddr, pbyPacket, dwRemainingBufferSize);
        }

        // Copy second part after wrapping.
        EC_T_DWORD dwSecondPartSize      = dwPacketLength - dwRemainingBufferSize;
        EC_T_DWORD dwOcmcBufferStartAddr = pAdapter->pPruss->l3ocmc.dwVirt + pQueue->wBufferOffset;
        pbyPacket = pbyPacket + dwRemainingBufferSize;
        if( EC_TRUE == bPacketPadding )
        {
            if( wOriginalLengthOfPacket <= dwRemainingBufferSize )
            {
                // Pad all the remaining bytes with Zero's
                for(i=0; i< dwSecondPartSize; i++)
                {
                    HW_SET8_NO_BARRIER(dwOcmcBufferStartAddr + i, 0);
                }
            }
            else
            {
                // Fill the frame data
                EC_T_DWORD remainingValidFrameDataLength = (wOriginalLengthOfPacket- dwRemainingBufferSize);
                LinkOsMemcpy(dwOcmcBufferStartAddr, pbyPacket, remainingValidFrameDataLength);

                // Pad the remaining bytes with Zero's.
                for(i=0; i< (dwSecondPartSize - remainingValidFrameDataLength); i++)
                {
                    HW_SET8_NO_BARRIER(dwOcmcBufferStartAddr + remainingValidFrameDataLength + i, 0);
                }
            }
        }
        else
        {
            LinkOsMemcpy(dwOcmcBufferStartAddr, pbyPacket, dwSecondPartSize);
        }
    }
    else
    {
        if( EC_TRUE == bPacketPadding )
        {
            LinkOsMemcpy(dwOcmcBufferAddr, pbyPacket, wOriginalLengthOfPacket);
            // Pad the remaining bytes with Zero's
            for(i=0; i< dwPacketLength - wOriginalLengthOfPacket; i++)
            {
                HW_SET8_NO_BARRIER(dwOcmcBufferAddr + wOriginalLengthOfPacket + i, 0);
            }
        }
        else
        {
            LinkOsMemcpy(dwOcmcBufferAddr, pbyPacket, dwPacketLength);
        }
    }

    // 18..28 bits of buffer descriptor contain buffer length.
    HW_SET32_NO_BARRIER(dwDramBase + dwWriteBdOffset, (dwPacketLength << 18));
    HW_SET16_NO_BARRIER(&pQueueDescriptor->wWriteBdOffset, dwNewWriteBdOffset);
    HW_WR_REG8(&pQueueDescriptor->byBusy, 1);

    return EC_TRUE;
}

/*
 * Function: ICSS_ReceiveGetPendingPacketSize
 * ---------------------------------------------------
 * Checks an incoming queue if it contains a packet.
 *
 * pPort[in]:          pointer to port structure.
 *
 * returns: size of first waiting packet in queue, 0 - instead.
 */
EC_T_DWORD ICSS_ReceiveGetPendingPacketSize(ICSS_ADAPTER_INTERNAL* pPort)
{
    const EC_T_DWORD  dwReceivingQueueCount = 2;  // only 2 receiving host queues -
                                                  // low priority and high priority.
    const EC_T_DWORD  dwQueueDescriptorsAddress  = pPort->pPruss->shrdram2.dwVirt +
                                                   pPort->receiveQueue[0].wQueueDescriptorsOffset;

    EC_T_DWORD         dwQueue           = 0;
    EC_T_BOOL          bPacketFound     = EC_FALSE;
    T_FIRMWARE_QUEUE_DESCRIPTOR *pQueueDescriptor = EC_NULL;
    EC_T_DWORD         dwBufferDescriptor= 0;
    EC_T_DWORD*        pBufferDescriptor = EC_NULL;
    EC_T_DWORD         dwPacketLength    = 0;

    while( bPacketFound == EC_FALSE && dwQueue < dwReceivingQueueCount )
    {
        pQueueDescriptor =  (T_FIRMWARE_QUEUE_DESCRIPTOR *)(dwQueueDescriptorsAddress +
                                                  dwQueue * sizeof(T_FIRMWARE_QUEUE_DESCRIPTOR));

        if( HW_GET16_NO_BARRIER(&pQueueDescriptor->wReadBdOffset) ==
            HW_GET16_NO_BARRIER(&pQueueDescriptor->wWriteBdOffset) )
        {
            // no pending packets in this queue.
            dwQueue++;
            continue;
        }

        pBufferDescriptor   = (EC_T_DWORD*)(pPort->pPruss->shrdram2.dwVirt +
                                            HW_GET16_NO_BARRIER(&pQueueDescriptor->wReadBdOffset));
        dwBufferDescriptor  = HW_GET32_NO_BARRIER(pBufferDescriptor);

        dwPacketLength = ((0x1ffc0000U & dwBufferDescriptor)>>18U);
        bPacketFound   = EC_TRUE;
    }

    return dwPacketLength;
}

/*
 * Function: ICSS_ReceivePacket
 * ---------------------------------------------------
 * Fetches first waiting packet from the incoming queue.
 *
 * pPort[in]:            pointer to port structure.
 * pUserBuffer[out]:     pointer to user buffer where packet should be copied.
 * dwUserBufferSize[in]: size of the user buffer. Used to make sure, it large enough.
 * pbMore[out]:          true, if the incoming queue has more packets.
 *
 * returns: size of first waiting packet in queue, 0 - instead.
 */
EC_T_DWORD ICSS_ReceivePacket(ICSS_ADAPTER_INTERNAL* pAdapter, EC_T_BYTE* pUserBuffer,
                              EC_T_DWORD dwUserBufferSize, EC_T_BOOL *pbMore)
{
    EC_T_DWORD        dwQueueDescriptorsAddress  = pAdapter->pPruss->shrdram2.dwVirt +
            pAdapter->receiveQueue[0].wQueueDescriptorsOffset;
    EC_T_DWORD        dwReceivingQueueCount = 2;
    EC_T_BOOL         bPacketFound      = EC_FALSE;
    T_FIRMWARE_QUEUE_DESCRIPTOR*pQueueDescriptor  = EC_NULL;
    EC_T_DWORD        dwBufferDescriptor= 0;
    EC_T_DWORD*       pBufferDescriptor = EC_NULL;
    EC_T_DWORD        dwQueue           = 0;
    EC_T_DWORD        dwPacketLength    = 0;

    // Look for a first packet in the queues.
    while( bPacketFound == EC_FALSE && dwQueue < dwReceivingQueueCount )
    {
        pQueueDescriptor =  (T_FIRMWARE_QUEUE_DESCRIPTOR *)(dwQueueDescriptorsAddress +
                                                  dwQueue*sizeof(T_FIRMWARE_QUEUE_DESCRIPTOR));

        if( pQueueDescriptor->wReadBdOffset == pQueueDescriptor->wWriteBdOffset )
        {
            // no pending packets in this dwQueue.
            dwQueue++;
            continue;
        }

        pBufferDescriptor  = (EC_T_DWORD*)(pAdapter->pPruss->shrdram2.dwVirt +
                                           pQueueDescriptor->wReadBdOffset);
        dwBufferDescriptor = *pBufferDescriptor;

        dwPacketLength = FIRMWARE_GET_PAKET_SIZE_FROM_BD(dwBufferDescriptor);
        bPacketFound = EC_TRUE;
    }
    if( EC_FALSE == bPacketFound)
    {
        SYS_ERRORMSGC(("ICSS_ReceivePacket(): warning, no packet in the queue.\n"));

        return 0;
    }
    if( dwPacketLength > FIRMWARE_MAXMTU)
    {
        SYS_ERRORMSGC(("ICSS_ReceivePacket(): warning! packet in the receiving queue is larger than Max MTU!\n"));
    }
    if( dwPacketLength > dwUserBufferSize )
    {
        SYS_ERRORMSGC(("ICSS_ReceivePacket(): User buffer is too small.\n"));
        return 0;
    }

    ICSS_QUEUE_INFORMATION* pHostQueue = &pAdapter->receiveQueue[dwQueue];
    EC_T_DWORD dwOcmcBufferAddress  = pAdapter->pPruss->l3ocmc.dwVirt +
                                      pHostQueue->wBufferOffset +
                                      (((pQueueDescriptor->wReadBdOffset - pHostQueue->wBdTableOffset)>>2)<<5);
    EC_T_DWORD dwNewReadBdOffset = 0;

    // Compute the end of the buffer descriptor list in the bd table for the packet.
    dwNewReadBdOffset = pQueueDescriptor->wReadBdOffset + ((dwPacketLength>>5)<<2);
    if( (dwPacketLength & 0x1f) != 0 )
    {
        dwNewReadBdOffset += 4;
    }

    // Check if the buffer has been wrapped.
    if( dwNewReadBdOffset >= pHostQueue->wBdTableEndOffset )
    {
        dwNewReadBdOffset = dwNewReadBdOffset - (pHostQueue->wBdTableEndOffset - pHostQueue->wBdTableOffset);
    }

    // Copy data from On-Chip Memory to DDR Memory (where user buffers are located).
    if( dwNewReadBdOffset < pQueueDescriptor->wReadBdOffset && dwNewReadBdOffset != pHostQueue->wBdTableOffset )
    {
        //
        // Buffer was wrapped.
        //
        EC_T_DWORD dwSizeOfWrappedBuffer = 0;

        // copy first part of wrapped buffer.
        dwSizeOfWrappedBuffer = (((pHostQueue->wBdTableEndOffset - pQueueDescriptor->wReadBdOffset)>>2)<<5);
        LinkOsMemcpy(pUserBuffer, dwOcmcBufferAddress, dwSizeOfWrappedBuffer);

        // cope second part of wrapped buffer.
        LinkOsMemcpy(pUserBuffer + dwSizeOfWrappedBuffer,
                     pAdapter->pPruss->l3ocmc.dwVirt + pHostQueue->wBufferOffset,
                     dwPacketLength - dwSizeOfWrappedBuffer);
    }
    else
    {
        // Buffer was not wrapped. Copy it "as is".
        LinkOsMemcpy(pUserBuffer, dwOcmcBufferAddress, dwPacketLength);
    }

    //
    // Shift read pointer in queue descriptor.
    //

    pQueueDescriptor->wReadBdOffset = dwNewReadBdOffset;
    if( dwNewReadBdOffset != pQueueDescriptor->wWriteBdOffset )
    {
        *pbMore = EC_TRUE;
    }
    else
    {
        *pbMore = EC_FALSE;
    }


    return dwPacketLength;
}

/*-----------------------------------------------------------------------------
 *
 *
 * Internal Functions.
 *
 *
 *---------------------------------------------------------------------------*/

/*
 * Function: ICSS_PRUSS_MapIoMem
 * ----------------------------------------
 *   For a specific PRUSS1 or PRUSS2 maps all needed physical memory into user space,
 *   and fill board configuration data.
 *
 *   returns: EC_TRUE, if memory has been mapped.
 */
static EC_T_BOOL ICSS_PRUSS_MapIoMem(ICSS_ADAPTER_INTERNAL* pAdapter,
                                       ICSS_PRUSS_CONFIGURATION* pPruss,
                                       EC_T_DWORD dwPruss)
{
    //
    // Mapping PRUSS sections.
    //

    if( !ICSS_MapMemoryBank(pAdapter, "intc", &pPruss->interruptController) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "shrdram2", &pPruss->shrdram2) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "cfg", &pPruss->cfg) )
    {
        return EC_FALSE;
    }
    
    
    if( !ICSS_MapMemoryBank(pAdapter, "IEP", &pPruss->iep) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "MiiRtCfg", &pPruss->miiRtCfg) )
    {
        return EC_FALSE;
    }

    //
    // Mapping specific sections.
    //

    if( !ICSS_MapMemoryBank(pAdapter, "mdio", &pPruss->mdio) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "l3ocmc", &pPruss->l3ocmc) )
    {
        return EC_FALSE;
    }

    //
    // Mapping PRU0/PRU1 sections.
    //

    if( !ICSS_MapMemoryBank(pAdapter, "dram0", &pPruss->pru[0].dram) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "dram1", &pPruss->pru[1].dram) )
    {
        return EC_FALSE;
    }

    if( !ICSS_MapMemoryBank(pAdapter, "iram0", &pPruss->pru[0].iram) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "iram1", &pPruss->pru[1].iram) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "control0", &pPruss->pru[0].control) )
    {
        return EC_FALSE;
    }
    if( !ICSS_MapMemoryBank(pAdapter, "control1", &pPruss->pru[1].control) )
    {
        return EC_FALSE;
    }

    return EC_TRUE;
}

/*
 * Function: ICSS_MapMemoryBank
 * ----------------------------------------
 *   Maps physical memory into virtual
 *
 *   returns: EC_TRUE, if memory has been mapped.
 */
static EC_T_BOOL ICSS_MapMemoryBank(ICSS_ADAPTER_INTERNAL* pAdapter,
                                const EC_T_CHAR* pszBankName, ICSS_IO_ADDRESS* pIoAddress)
{
    pIoAddress->dwVirt = (EC_T_DWORD) LinkOsMapMemory(
            (EC_T_BYTE*)pIoAddress->dwPhys, pIoAddress->dwSize);

    if (pIoAddress->dwVirt == 0)
    {
        SYS_ERRORMSGC(("ICSS_MapMemoryBank(): unable to map IO memory %s into user space.\n", pszBankName));
        SYS_ERRORMSGC(("ICSS_MapMemoryBank(): dwLosalHandle=0x%X.\n", pAdapter->dwLosalHandle));
        SYS_ERRORMSGC(("ICSS_MapMemoryBank(): dwPhys=0x%X.\n", pIoAddress->dwPhys));
        SYS_ERRORMSGC(("ICSS_MapMemoryBank(): dwSize=%d.\n", pIoAddress->dwSize));
        return EC_FALSE;
    }

    return EC_TRUE;
}

/*
 * Function: ICSS_PRUSS_MapIoMem
 * ----------------------------------------
 *   Unmaps all PRUSSx memory.
 *
 *   returns: void.
 */
static EC_T_VOID ICSS_PRUSS_UnmapIoMem(ICSS_ADAPTER_INTERNAL* pAdapter,
                                       ICSS_PRUSS_CONFIGURATION* pPruss )
{
    //
    // Unmapping PRUSS sections.
    //

    ICSS_UnmapMemoryBank(pAdapter, &pPruss->interruptController);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->shrdram2);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->cfg);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->iep);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->miiRtCfg);

    //
    // Unmapping specific sections.
    //

    ICSS_UnmapMemoryBank(pAdapter, &pPruss->mdio);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->l3ocmc);

    //
    // Unmapping PRU0/PRU1 sections.
    //

    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[0].dram);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[1].dram);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[0].iram);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[1].iram);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[0].control);
    ICSS_UnmapMemoryBank(pAdapter, &pPruss->pru[1].control);
}

/*
 * Function: ICSS_UnmapMemoryBank
 * ----------------------------------------
 *   Unmaps virtual memory.
 *
 *   returns: void.
 */
static EC_T_VOID ICSS_UnmapMemoryBank(ICSS_ADAPTER_INTERNAL* pAdapter,
                                      ICSS_IO_ADDRESS* pIoAddress)
{
    if( pIoAddress->dwVirt != 0 )
    {
        LinkOsUnmapMemory((EC_T_BYTE *)pIoAddress->dwPhys, pIoAddress->dwSize);
        pIoAddress->dwPhys = 0;
        pIoAddress->dwSize = 0;
    }
}

/*
 * Function: ICSS_PRUSS_ClearMemory
 * --------------------------------------
 * Clears SharedMem & OCMC Memory.
 *
 * Todo: this function should be selective. Clear only parts of memory, dedicated to the PRU only!
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */

static EC_T_VOID ICSS_PRUSS_ClearMemory(ICSS_ADAPTER_INTERNAL* pPort)
{
    const EC_T_DWORD dwSharedMemorySize = PRUSS_SHARED_MEMORY_SIZE;
    const EC_T_DWORD dwOcmcMemorySize   = BOARD_L3OCMC1_MEMORY_SIZE;

    EC_T_DWORD *pdwTemp;
    EC_T_DWORD i;

    // Clear all registers, clear all Shared RAM (12K)
    pdwTemp = (EC_T_DWORD*)(pPort->pPruss->shrdram2.dwVirt);
    for (i=0; i<(dwSharedMemorySize/4U); i++)
    {
        *(pdwTemp + i) = 0x00000000U;
    }

    // Clear on chip memory for queues
    pdwTemp = (EC_T_DWORD*)(pPort->pPruss->l3ocmc.dwVirt);
    for (i=0; i<(dwOcmcMemorySize/4U); i++)
    {
        *(pdwTemp + i) = 0x00000000U;
    }
}

/*
 * Function: ICSS_PRUSS_WakeUp
 * --------------------------------------
 * Enabled power domain for specified PRUICSS sub-system 
 * and disables idle & standby modes.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_WakeUp(ICSS_BOARD_CONFIGURATION *pBoard, EC_T_DWORD dwPrussNum)
{
    EC_T_DWORD dwPrussClkCtrlAddr    = pBoard->cmCore.dwVirt + 
                                       CM_CORE_L4PER2_PRUSS_CLKCTRL(dwPrussNum);
    
    EC_T_DWORD dwL4Per2ClkStCtrlAddr = pBoard->cmCore.dwVirt + 
                                       CM_CORE_L4PER2_CLKSTCTRL;
    //
    // On some operating system, like VxWorks, there is a minimal boot DTB configuration.
    // So the PRUICSS subsystem is not initialized and remains switched off. 
    //
    
    // Switch off PRUSS module in L4PER2 clock domain.
    HW_WR_REG32(dwPrussClkCtrlAddr, 0);
    // Power on PRUSS, set clock and enable all clock dividers of upper levels.
    HW_WR_FIELD32(dwPrussClkCtrlAddr, 
            CM_CORE_L4PER2_PRUSS_CLKCTRL_MODULEMODE,
            CM_CORE_L4PER2_PRUSS_CLKCTRL_MODULEMODE_ENABLE);
    // Wait until module is ready
    while(HW_RD_FIELD32(dwPrussClkCtrlAddr, CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST)
            != CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST_MODULE_FULLY_FINCTIONAL);
    // Wait until ICSS_CLK clock is definitely running / gating on-going.
    while(HW_RD_FIELD32(dwL4Per2ClkStCtrlAddr, CM_CORE_L4PER2_CLKSTCTRL_ICSS_CLK)
            != CM_CORE_L4PER2_CLKSTCTRL_CLOCK_RUNNING);
    // Wait until PER_192M_GFCLK clock is definitely running / gating on-going.
    while(HW_RD_FIELD32(dwL4Per2ClkStCtrlAddr, CM_CORE_L4PER2_CLKSTCTRL_PER_192M_GFCLK)
            != CM_CORE_L4PER2_CLKSTCTRL_CLOCK_RUNNING);
    // Wait until ICSS_IEP_CLK clock is definitely running / gating on-going.
    while(HW_RD_FIELD32(dwL4Per2ClkStCtrlAddr, CM_CORE_L4PER2_CLKSTCTRL_ICSS_IEP_CLK)
            != CM_CORE_L4PER2_CLKSTCTRL_CLOCK_RUNNING);
    
    LinkOsSleep(50);

    
    //
    // PRUSS memory in L4 is now accessible.
    // Tell PRUSS not to sleep anymore.
    //
    
    EC_T_DWORD dwSysCfgAddr = (pBoard->pruss[dwPrussNum].cfg.dwVirt) + PRUSS_CFG_SYSCFG;

    // Initiate standby sequence for PRUSS.
    HW_WR_FIELD32(dwSysCfgAddr,
                        PRUSS_CFG_SYSCFG_STANDBY_INIT ,
                        PRUSS_CFG_SYSCFG_STANDBY_INIT_INITIATE_STANDBY);
    // Disable Standby mode.
    HW_WR_FIELD32(dwSysCfgAddr,
                        PRUSS_CFG_SYSCFG_STANDBY_MODE,
                        PRUSS_CFG_SYSCFG_STANDBY_MODE_NO_STANDBY_MODE);
    // Disables Idle mode for PRUSS.
    HW_WR_FIELD32(dwSysCfgAddr,
                  PRUSS_CFG_SYSCFG_IDLE_MODE,
                  PRUSS_CFG_SYSCFG_IDLE_MODE_NO_IDLE_MODE);
}

/*
 * Function: ICSS_BOARD_WakeUp
 * --------------------------------------
 * Enables power and clocks to both PRUSS devices and to corresponding 
 * power domains.
 *
 * pBoard[in]: pointer to board configuration.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_BOARD_WakeUp(ICSS_BOARD_CONFIGURATION *pBoard)
{
    ICSS_PRUSS_WakeUp(pBoard, 0);
    ICSS_PRUSS_WakeUp(pBoard, 1);
}

/*
 * Function: ICSS_PRUSS_InitializeGpiMode
 * --------------------------------------
 * Selects PRU0&PRU1 GPI_MODE to MII_RT mode.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_InitializeGpiMode(ICSS_ADAPTER_INTERNAL* pPort)
{
    if( pPort->pPru->bMaster )
    {
        // Set MII_RT mode for PRU0.
        HW_WR_FIELD32((pPort->pPruss->cfg.dwVirt) + PRUSS_CFG_GPCFG0,
                            PRUSS_CFG_GPCFG_GPI_MODE,
                            PRUSS_CFG_GPCFG_GPI_MODE_MII_RT);

        // Set MII_RT mode for PRU1.
        HW_WR_FIELD32((pPort->pPruss->cfg.dwVirt) + PRUSS_CFG_GPCFG1,
                            PRUSS_CFG_GPCFG_GPI_MODE,
                            PRUSS_CFG_GPCFG_GPI_MODE_MII_RT);
    }
}

/*
 * Function: ICSS_PRUSS_InitializeGpMux
 * --------------------------------------
 * Controls icss_wrap mux select.
 * Important on am571x board, where 4 ports are supported.
 *
 * Note: for some unknown reason it is not possible to configure icss_wrap
 *       individually for each PRUSS separatelly. Both PRUSS must be
 *       configured.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_InitializeGpMux(ICSS_ADAPTER_INTERNAL* pPort)
{
    if( pPort->pPru->bMaster == EC_TRUE )
    {
        // AM571x family supports internal multiplexing (and 4 ports instead of 2).
        if( pPort->eBoardType == EcLinkIcssBoard_am571x )
        {
            HW_WR_FIELD32(pPort->platform.pruss[0].cfg.dwVirt + PRUSS_CFG_GPCFG1,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL_MII2);

            HW_WR_FIELD32(pPort->platform.pruss[1].cfg.dwVirt + PRUSS_CFG_GPCFG0,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL_MII2);
            HW_WR_FIELD32(pPort->platform.pruss[1].cfg.dwVirt + PRUSS_CFG_GPCFG1,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL,
                                PRUSS_CFG_GPCFG_GP_MUX_SEL_MII2);
        }
    }
}

/*
 * Function: ICSS_PRUSS_InitializeSetIepClockSource
 * ---------------------------------------------------
 * Initializes IEP module and sets clock source.
 * PRUSS has two clock sources: PRUSS_IEP_CLK (default) and PRUSS_GICLK.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_InitializeSetIepClockSource(ICSS_ADAPTER_INTERNAL* pPort)
{
    // Select PRUSS_GICLK as clock source for IEP module of PRUSS.
    HW_WR_FIELD32((pPort->pPruss->cfg.dwVirt) + PRUSS_CFG_IEPCLK,
                  PRUSS_CFG_IEPCLK_OCP_EN , PRUSS_CFG_IEPCLK_OCP_EN_ICLK);
}

/*
 * Function: ICSS_PRUSS_InitializeEnableMiiRtMapping
 * ----------------------------------------------
 * Enables MII_RT Event Enable Register to enable MII_RT events mapping to INTC.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_InitializeEnableMiiRtMapping(ICSS_ADAPTER_INTERNAL* pPort)
{
    HW_WR_FIELD32((pPort->pPruss->cfg.dwVirt) + PRUSS_CFG_MII_RT,
                  PRUSS_CFG_MII_RT_MII_RT_EVENT_EN,
                  PRUSS_CFG_MII_RT_MII_RT_EVENT_EN_ENABLE);
}

/*
 * Function: ICSS_PRUSS_InitializeEnableSppXfrShift
 * ---------------------------------------------------
 * Enables XIN XOUT shift functionality in Scratch Pad. When enabled,
 * R0[4:0] (internal to PRU) defines the 32-bit offset for XIN
 * and XOUT operations with the scratch pad. 0x0 =
 * Disabled. 0x1 = Enabled
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_InitializeEnableSppXfrShift(ICSS_ADAPTER_INTERNAL* pPort)
{
    HW_WR_FIELD32((pPort->pPruss->cfg.dwVirt) + PRUSS_CFG_SPP,
                  PRUSS_CFG_SPP_SPP_XFR_SHIFT_EN ,
                  PRUSS_CFG_SPP_SPP_XFR_SHIFT_EN_ENABLE);
}

/*
 * Function: ICSS_PRUSS_INTC_MapInterrupts
 * ---------------------------------------------------
 * Maps 64 system interrupts to 10 channels of PRUSS Interrupt Controller,
 * and then maps to PRUx (called here "hosts") events.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_INTC_MapInterrupts(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwIntcBaseAddr = pPort->pPruss->interruptController.dwVirt;
    EC_T_DWORD i = 0, dwMask1 = 0, dwMask2 = 0;
    EC_T_DWORD dwTempAddr = 0U;
    PRUSS_INTC_MAPPING intcMapping = PRUSS_INTC_INITDATA;

    //
    // Set polarity (CSL_ICSSINTC_SIPRx) and pulse type (CSL_ICSSINTC_SITRn) for system interrupts.
    // All 64 system interrupts are have always high polarity and pulse type.
    // There are two 32-bit registers for polarity and type. Each 32-bit register serves 32 system interrupts.
    //

    /// polarity is always high (always 1 to each bit).
    HW_WR_REG32(dwIntcBaseAddr + PRUSS_INTC_SIPR0, PRUSS_INTC_SIPR_HIGH_POLARITY_ALL);
    HW_WR_REG32(dwIntcBaseAddr + PRUSS_INTC_SIPR1, PRUSS_INTC_SIPR_HIGH_POLARITY_ALL);
    // type is always pulse (always 0)
    HW_WR_REG32(dwIntcBaseAddr + PRUSS_INTC_SITR0, PRUSS_INTC_SITR_PULSE_ALL);
    HW_WR_REG32(dwIntcBaseAddr + PRUSS_INTC_SITR1, PRUSS_INTC_SITR_PULSE_ALL);

    //
    // Reset mapping of system interrupts to INTC channels.
    // There are 16 32-bit CMR registers. Each is responsible for assigning 4 channels for 4 interrupts.
    //

    for (i = 0; i < (BOARD_SYSTEM_INTERRUPT_COUNT>>2); i++)
    {
        dwTempAddr = ((dwIntcBaseAddr + PRUSS_INTC_CMR0) + (i<<2));
        HW_WR_REG32(dwTempAddr, 0);
    }

    //
    // Map system interrupts to INTC channels.
    // 4 system interrupts are mapped to every channel.
    //

    for (i = 0;
         ((intcMapping.mapSystemInterruptToChannel[i].bySysevt != 0xFF)
         && (intcMapping.mapSystemInterruptToChannel[i].byChannel != 0xFF));
         i++)
    {
        ICSS_PRUSS_INTC_MapInterruptToChannel(intcMapping.mapSystemInterruptToChannel[i].bySysevt,
                           intcMapping.mapSystemInterruptToChannel[i].byChannel,
                           dwIntcBaseAddr);
    }

    //
    // Clear mapping of interrupt channels to host interrupts.
    // There are 3 32-bit HMR registers. Each is responsible for assigning 4 channels to 4 interrupts.
    //
    for (i = 0; i < (PRUSS_INTC_CHANNEL_COUNT>>2); i++)
    {
        dwTempAddr = (dwIntcBaseAddr + PRUSS_INTC_HMR0 + (i<<2));
        HW_WR_REG32(dwTempAddr, 0);
    }

    //
    // Map interrupt channels to PRU interrupts.
    //

    for (i = 0;
         ((intcMapping.mapChannelToPruEvent[i].wChannel != 0xFF)
         && (intcMapping.mapChannelToPruEvent[i].wHost != 0xFF) && (i<PRUSS_INTC_CHANNEL_COUNT)) ;
         i++)
    {
        ICSS_PRUSS_INTC_MapChannelToInterrupt(intcMapping.mapChannelToPruEvent[i].wChannel,
                           intcMapping.mapChannelToPruEvent[i].wHost,
                           dwIntcBaseAddr);
    }

    //
    // Calculate 32-bit masks for system interrupts, which should be enabled.
    //

    dwMask1 = dwMask2 = 0;
    for (i = 0; intcMapping.bSystemInterruptEnabled[i] != 0xFFU; i++)
    {
        if (intcMapping.bSystemInterruptEnabled[i] < 32)
        {
            dwMask1 = dwMask1 + (((EC_T_DWORD)1U) << (intcMapping.bSystemInterruptEnabled[i]));
        }
        else if (intcMapping.bSystemInterruptEnabled[i] < 64)
        {
            dwMask2 = dwMask2 + (((EC_T_DWORD)1U) << (intcMapping.bSystemInterruptEnabled[i] - 32));
        }
    }

    // Enable system interrupts, which are interested for us.
    // System interrupt enables system interrupts 0 to 63.
    // Writing a 1 in a bit position to set that enable.
    HW_WR_FIELD32((dwIntcBaseAddr + PRUSS_INTC_ESR0),PRUSS_INTC_ESR_ENABLE_SET_31_0, dwMask1);
    HW_WR_FIELD32((dwIntcBaseAddr + PRUSS_INTC_ERS1),PRUSS_INTC_ESR_ENABLE_SET_63_32, dwMask2);

    // Reset raised system interrupts, which are supported.
    // Writing a 1 in a bit position to clear the status of the system interrupt.
    HW_WR_FIELD32((dwIntcBaseAddr + PRUSS_INTC_SECR0),PRUSS_INTC_ENA_STATUS_31_0, dwMask1);
    HW_WR_FIELD32((dwIntcBaseAddr + PRUSS_INTC_SECR1),PRUSS_INTC_ENA_STATUS_63_32, dwMask2);

    // Enable individual host (PRU) interrupts.
    // The Host Interrupt Enable Registers enable or disable individual host interrupts. These work
    // separately from the global enables. There is one bit per host interrupt. These bits are updated
    // when writing to the Host Interrupt Enable Index Set and Host Interrupt Enable Index Clear
    // registers.
    HW_WR_FIELD32((dwIntcBaseAddr  + PRUSS_INTC_HIER),PRUSS_INTC_HIER_ENABLE_HINT,intcMapping.dwPruEventBitmask);

    // Globally enable all host (PRU) interrupts.
    // The Global Host Interrupt Enable Register enables all the host interrupts. Individual host interrupts
    // are still enabled or disabled from their individual enables and are not overridden by the global
    // enable.
    HW_WR_FIELD32((dwIntcBaseAddr  + PRUSS_INTC_GER), PRUSS_INTC_GER_ENABLE_HINT_ANY, 0x1);
}

/*
 * Function: ICSS_PRUSS_INTC_MapInterruptToChannel
 * ---------------------------------------------------
 * Maps a system interrupt to a INTC channel.
 *
 * dwSystemInterrupt[in]: 0..63, if of system interrupt to be mapped.
 * dwIntcChannel[in]:     INT channel, where to map the system interrupt.
 * dwIntcBaseAddr[in]:    address of INTC memory.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_INTC_MapInterruptToChannel( EC_T_DWORD dwSystemInterrupt,
                                         EC_T_DWORD dwIntcChannel,
                                         EC_T_DWORD dwIntcBaseAddr)
{
     EC_T_DWORD dwCmrRegisterBaseAddr  = (dwIntcBaseAddr + PRUSS_INTC_CMR0);
     EC_T_DWORD dwRegisterNo           = (dwSystemInterrupt>>2);
     EC_T_DWORD dwCmrRegisterAddr      = (dwCmrRegisterBaseAddr + (dwRegisterNo<<2));
     EC_T_DWORD dwValue = 0;

     // Shift channel number to the appropriate position in the current register.
     dwValue = (dwIntcChannel << ((dwSystemInterrupt - (dwRegisterNo<<2))<<3));
     HW_WROR_REG32(dwCmrRegisterAddr, dwValue);
}

/*
 * Function: ICSS_PRUSS_INTC_MapChannelToInterrupt
 * ---------------------------------------------------
 * Maps a INTC channel to PRU event.
 *
 * dwIntcChannel[in]:   INTC channel.
 * dwHostInterrupt[in]: PRU event, where to map.
 * dwBaseAddr[in]:      Address of INTC memory.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_INTC_MapChannelToInterrupt( EC_T_DWORD  dwIntcChannel,
                                         EC_T_DWORD  dwHostInterrupt,
                                         EC_T_DWORD  dwBaseAddr)
{
     EC_T_DWORD dwRegisterAddr = 0U;

     // there are HMR0,HMR1 and HMR2 32-bit registers.
     dwRegisterAddr = ((dwBaseAddr) + (PRUSS_INTC_HMR0 + ((dwIntcChannel) & ~((EC_T_DWORD)0x3U))));
     HW_WROR_REG32(dwRegisterAddr, ((dwHostInterrupt) << ((dwIntcChannel & ((EC_T_DWORD)0x3U)) << 3U)));
}

/*
 * Function: ICSS_PRUSS_EnableIepCounter
 * ---------------------------------------------------
 * Enables IEP Counter + resets default increment value,
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static void ICSS_PRUSS_EnableIepCounter(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwTempAddr = 0U;

    dwTempAddr = (pPort->pPruss->iep.dwVirt + PRUSS_IEP_GLOBAL_CFG);

    /*Enable IEP Counter*/
    HW_WR_REG16(dwTempAddr, PRUSS_IEP_GLOBAL_CFG_VALUE);
 }

/*
 * Function: ICSS_PRUSS_MDIO_InitializeStateMachine
 * ---------------------------------------------------
 * Enable MDIO state machine and sets its speed 1.25 Mhz
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_MDIO_InitializeStateMachine(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwValue = 0;

    // Enable MDIO state machine.
    HW_SET_FIELD32(dwValue, PRUSS_MDIO_CONTROL_ENABLE, 1);
    // Set clock divider to 1.25 Mhz (200/(0x9F+1) = 1.25 MHz)
    HW_SET_FIELD32(dwValue, PRUSS_MDIO_CONTROL_CLKDIV, PRUSS_MDIO_CONTROL_CLKDIV_DEFVALUE);

    // Set all above bits as an atomic operation to the register.
    HW_WR_REG32(pPort->pPruss->mdio.dwVirt + PRUSS_MDIO_CONTROL, dwValue);

    LinkOsSleep(50);
}

/*
 * Function: ICSS_FIRMWARE_CalculateReceiveQueueOffsets
 * ----------------------------------------------------
 * Calculates offset to buffer data in OCMC memory and
 * to buffer descriptors in Shared Memory.
 *
 * dwCore[in]:                 0 for PRU0, 1 for PRU1.
 * adwBufferTableOffsets[out]: array for buffer offsets.
 * adwBdTableOffsets[out]:     array for buffer descriptors.
 *
 * returns: void.
 */
static void ICSS_FIRMWARE_CalculateReceiveQueueOffsets(EC_T_DWORD dwCore,
                   EC_T_DWORD adwBufferTableOffsets[FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT],
                   EC_T_DWORD adwBdTableOffsets[FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT])
{
    EC_T_DWORD dwFirstBufferOffset  = FIRMWARE_OMCMC_DATA_BUFFERS_OFFSET;
    EC_T_DWORD dwFirstBdTableOffset = FIRMWARE_SHAREDRAM_BD_TABLES_OFFSET;

    if( dwCore == 1 )
    {
        dwFirstBufferOffset  += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*
                                FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*
                                FIRMWARE_DATABLOCK_SIZE;

        dwFirstBdTableOffset += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*
                                FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*
                                FIRMWARE_BD_SIZE;
    }

     adwBufferTableOffsets[0] = dwFirstBufferOffset;
     adwBufferTableOffsets[1] = adwBufferTableOffsets[0] +
                                FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE;

     adwBdTableOffsets[0] = dwFirstBdTableOffset;
     adwBdTableOffsets[1] = adwBdTableOffsets[0] +
                            FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE;
}

/*
 * Function: ICSS_FIRMWARE_InitializeReceiveQueues
 * ----------------------------------------------------
 * Configures firmware receiving queues.
 *
 * Each port has 2 receiving queues (with different priority, based on VLAN tag)
 *
 * TODO: use structures here
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */

static EC_T_VOID ICSS_FIRMWARE_InitializeReceiveQueues(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD adwBufferTableOffsets[FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT];
    EC_T_DWORD adwBdTableOffsets[FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT];
    EC_T_DWORD dwSharedDramBase = pPort->pPruss->shrdram2.dwVirt;

    //
    // Calculate queue structures location in SharedRam & OCMC Ram for Firmware.
    //

    EC_T_DWORD dwQueueSizeTableAddr       = dwSharedDramBase + FIRMWARE_SHAREDFRAM_QUEUE_SIZES_OFFSET;
    EC_T_DWORD dwRxContextTableAddr       = dwSharedDramBase + FIRMWARE_SHAREDRAM_RX_CONTEXTS_OFFSET;
    EC_T_DWORD dwBdOffsetTableAddr        = dwSharedDramBase + FIRMWARE_SHAREDRAM_OFFSETS_TO_BD_TABLES_OFFSET;
    EC_T_DWORD dwBufferOffsetTableAddr    = dwSharedDramBase + FIRMWARE_SHAREDRAM_BUFFER_OFFSETS_OFFSET;
    EC_T_DWORD dwQueueDescriptorTableAddr = dwSharedDramBase + FIRMWARE_SHAREDRAM_QUEUE_DESCRIPTORS_OFFSET;

    // Get offsets (in SharedRam) to port queue1 & queue2
    ICSS_FIRMWARE_CalculateReceiveQueueOffsets(pPort->dwPruNum, adwBufferTableOffsets, adwBdTableOffsets);

    // Shift offsets to second PRU.
    if( pPort->dwPruNum > 0 )
    {
        dwQueueSizeTableAddr        += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*(sizeof(EC_T_WORD));
        dwRxContextTableAddr        += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*sizeof(T_FIRMWARE_RX_CONTEXT);
        dwBdOffsetTableAddr         += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*(sizeof(EC_T_WORD));
        dwBufferOffsetTableAddr     += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*(sizeof(EC_T_WORD));
        dwQueueDescriptorTableAddr  += FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*(sizeof(T_FIRMWARE_QUEUE_DESCRIPTOR));
    }

    //
    // Configure queues in Firmware.
    //

    EC_T_DWORD dwQueue = 0;
    EC_T_WORD* pwQueueSizes            = (EC_T_WORD*) dwQueueSizeTableAddr;
    EC_T_WORD* pwOffsetsToBdTable      = (EC_T_WORD*) dwBdOffsetTableAddr;
    EC_T_WORD* pwOffsetsToDataBuffer   = (EC_T_WORD*) dwBufferOffsetTableAddr;
    T_FIRMWARE_RX_CONTEXT       *pRxContexts      = (T_FIRMWARE_RX_CONTEXT*) dwRxContextTableAddr;
    T_FIRMWARE_QUEUE_DESCRIPTOR *pQueueDescriptor = (T_FIRMWARE_QUEUE_DESCRIPTOR*) dwQueueDescriptorTableAddr;

    for( dwQueue = 0; dwQueue < FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT; dwQueue++ )
    {
        // Queue size
        HW_SET16_NO_BARRIER(&pwQueueSizes[dwQueue], FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS);

        // Rx Context (in SharedRam)
        HW_SET16_NO_BARRIER(&pRxContexts[dwQueue].wOffsetToDataBuffer, adwBufferTableOffsets[dwQueue]);

        HW_SET16_NO_BARRIER(&pRxContexts[dwQueue].wOffsetToQueueDescriptors,
                            (EC_T_DWORD)&pQueueDescriptor[dwQueue] - dwSharedDramBase);

        HW_SET16_NO_BARRIER(&pRxContexts[dwQueue].wOffsetToBdTable,
                            adwBdTableOffsets[dwQueue]);

        HW_SET16_NO_BARRIER(&pRxContexts[dwQueue].wOffsetToLastBd,
                            FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE +
                            adwBdTableOffsets[dwQueue] - FIRMWARE_BD_SIZE);


        // Offset to BdTable for queue (in SharedRam)
        HW_SET16_NO_BARRIER(&pwOffsetsToBdTable[dwQueue], adwBdTableOffsets[dwQueue]);

        // Offset to queue (in OCMC RAM).
        HW_SET16_NO_BARRIER(&pwOffsetsToDataBuffer[dwQueue], adwBufferTableOffsets[dwQueue]);

        // Queue descriptor (in SharedRam)
        HW_SET16_NO_BARRIER(&pQueueDescriptor[dwQueue].wReadBdOffset, adwBdTableOffsets[dwQueue]);
        HW_SET16_NO_BARRIER(&pQueueDescriptor[dwQueue].wWriteBdOffset, adwBdTableOffsets[dwQueue]);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byBusy, EC_FALSE);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byStatus, 0);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byMaxFillLevel, 0);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byOverflowCount, 0);

        // Configure copy of Queue Descriptors in our own DDR Ram.
        pPort->receiveQueue[dwQueue].wBufferOffset           = adwBufferTableOffsets[dwQueue];
        pPort->receiveQueue[dwQueue].wBdTableOffset          = adwBdTableOffsets[dwQueue];
        pPort->receiveQueue[dwQueue].wQueueDescriptorsOffset = dwQueueDescriptorTableAddr - dwSharedDramBase;
        pPort->receiveQueue[dwQueue].wBdTableEndOffset       = (FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE) +
                                                               adwBdTableOffsets[dwQueue]; // end of queue
    }
}

/*
 * Function: ICSS_PRUSS_PRU_ClearMemory
 * ----------------------------------------------------
 * Clears DRAM memory.
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */
static void ICSS_PRUSS_PRU_ClearMemory(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD *pdwTemp = (EC_T_DWORD *) pPort->pPru->dram.dwVirt;
    EC_T_DWORD i;

    // Clear all registers 8kB.
    for (i=0; i<(0x2000U/4U); i++)
    {
        if(pdwTemp != NULL)
        {
             *(pdwTemp + i) = 0x00000000U;
        }
    }
}

/*
 * Function: ICSS_PRUSS_PRU_InitializeMiiRt
 * ---------------------------------------------------
 * Configures MII_RT for the specified port. RX & TX Paths, delays, preamble, etc.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_InitializeMiiRt(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwMiiRtCfgRegBase = pPort->pPruss->miiRtCfg.dwVirt;
    EC_T_DWORD dwMiiRtRxCfgAddr = 0;
    EC_T_DWORD dwMiiRtTxCfgAddr = 0;
    EC_T_DWORD dwMiiRtTxIpgAddr = 0;

    if( pPort->dwPruNum == 0 )
    {
        dwMiiRtRxCfgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_RXCFG0;
        dwMiiRtTxCfgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_TXCFG0;
        dwMiiRtTxIpgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_TX_IPG0;
    }
    else
    {
        dwMiiRtRxCfgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_RXCFG1;
        dwMiiRtTxCfgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_TXCFG1;
        dwMiiRtTxIpgAddr = dwMiiRtCfgRegBase + PRUSS_MII_RT_TX_IPG1;
    }

    // Enable the receive traffic currently selected by RX_MUX_SELECT
    HW_WR_FIELD32(dwMiiRtRxCfgAddr, PRUSS_MII_RT_RXCFG_RX_ENABLE, 1);
    // Configure bit 16 in R31 to TX_EOF.
    HW_WR_FIELD32(dwMiiRtRxCfgAddr,
                   PRUSS_MII_RT_RXCFG_RX_DATA_RDY_MODE_DIS,
                   PRUSS_MII_RT_RXCFG_RX_DATA_RDY_MODE_DIS_TX_EOF_MODE);
    // Select receive data source port 0.
    HW_WR_FIELD32(dwMiiRtRxCfgAddr, PRUSS_MII_RT_RXCFG_RX_MUX_SEL, pPort->dwPruNum);
    // Enable L2 buffer.
    HW_WR_FIELD32(dwMiiRtRxCfgAddr, PRUSS_MII_RT_RXCFG_RX_L2_EN, 1);
    // Suppress preamble and sync frame delimiter.
    HW_WR_FIELD32(dwMiiRtRxCfgAddr, PRUSS_MII_RT_RXCFG0_RX_CUT_PREAMBLE, 1);
    // Disable self-clearing RX_EOF flag in R31.
    HW_WR_FIELD32(dwMiiRtRxCfgAddr, PRUSS_MII_RT_RXCFG_RX_L2_EOF_SCLR_DIS, 1);

    // Configure the minimum inter packet gap for the Tx.
    HW_WR_REG8(dwMiiRtTxIpgAddr, PRUSS_MII_RT_TX_IPG_TX_IPG_VALUE);
    // Enable traffic on TX Port. The frame will start once the IPG counter
    // expired and TX Start Delay counter has expired.
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_ENABLE, 1);
    // TX FIFO should insert preamble automatically.
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_AUTO_PREAMBLE, 1);
    // Enable 32,16 and 8-bit Data Push mode.
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_32_MODE_EN, 1);
    // Select PRU0 as transmit data source.
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_MUX_SEL, pPort->dwPruNum);
    // Reset the minimum time interval between RXDV of the frame and start if transmit.
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_START_DELAY, PRUSS_MII_RT_TXCFG_TX_START_DELAY_RESET);
    // Det delay to synchronize MII_RT with PRU (200 Mhz)
    HW_WR_FIELD32(dwMiiRtTxCfgAddr, PRUSS_MII_RT_TXCFG_TX_CLK_DELAY, PRUSS_MII_RT_TXCFG_TX_CLK_DELAY_VALUE);
}

/*
 * Function: ICSS_PRUSS_PRU_Halt
 * ---------------------------------------------------
 * Halts PRU.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_Halt(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwBaseAddr = pPort->pPru->control.dwVirt;

    HW_WR_FIELD32(dwBaseAddr, PRUSS_PRU_CONTROL_ENABLE, PRUSS_PRU_CONTROL_ENABLE_DISABLEVAL);
}

/*
 * Function: ICSS_PRUSS_PRU_Start
 * ---------------------------------------------------
 * Starts PRU.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_Start(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwBaseAddr = pPort->pPru->control.dwVirt;

    HW_WR_FIELD32(dwBaseAddr, PRUSS_PRU_CONTROL_ENABLE, PRUSS_PRU_CONTROL_ENABLE_ENABLEVAL);
}

/*
 * Function: ICSS_PRUSS_PRU_WaitForStableConnection
 * ----------------------------------------------------
 * Wait until firmware completely started.
 * It can often happen, that link is available at the very beginning, but then it
 * disappears for several milliseconds and then appears again and then
 * it is stable. We should address this situation.
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */
static EC_T_BOOL ICSS_PRUSS_PRU_WaitForStableConnection(ICSS_ADAPTER_INTERNAL* pPort, EC_T_DWORD dwMaxWaitTimeUsec)
{
    EC_T_DWORD dwTimePassed = 0;
    EC_T_DWORD dwSleepStep = 50;
    EC_T_BOOL  bLastStatus = EC_FALSE;
    EC_T_BOOL  bActualStatus = EC_FALSE;
    EC_T_BOOL  bDropDetected = EC_FALSE;

    while( dwTimePassed < dwMaxWaitTimeUsec )
    {
        bActualStatus = ICSS_UpdatePortLinkStatus(pPort);
        if( bActualStatus != bLastStatus )
        {
            if( bActualStatus == EC_FALSE )
            {
                bDropDetected = EC_TRUE;
            }
            else
            {
                if( bDropDetected )
                {
                    // Exit immediately, link restored.
                    return EC_TRUE;
                }
            }
            bLastStatus = bActualStatus;
        }
        dwTimePassed += dwSleepStep;
        LinkOsSleep(dwSleepStep);
    }

    return bActualStatus;
}

/*
 * Function: ICSS_FIRMWARE_SetMacAddr
 * ----------------------------------------------------
 * Sets MAC address to port.
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */

static EC_T_VOID ICSS_FIRMWARE_SetMacAddr(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwDramBase = pPort->pPru->dram.dwVirt;
    EC_T_BYTE* pbyMacAddr = (EC_T_BYTE*)(dwDramBase + FIRMWARE_DRAM_PORT_MAC_OFFSET);
    EC_T_INT   n;

    for(n=0; n < 6; n++)
    {
        pbyMacAddr[n] = pPort->abyMac[n];
    }
}

/*
 * Function: ICSS_FIRMWARE_CalculateSendQueueOffsets
 * ----------------------------------------------------
 * Calculates offset to buffer data in OCMC memory and
 * to buffer descriptors in Dram Memory.
 *
 * dwCore[in]:                 0 for PRU0, 1 for PRU1.
 * adwBufferTableOffsets[out]: array for buffer offsets.
 * adwBdTableOffsets[out]:     array for buffer descriptors.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_FIRMWARE_CalculateSendQueueOffsets(EC_T_DWORD dwCore,
                                             EC_T_DWORD adwBufferOffsets[FIRMWARE_SEND_QUEUE_COUNT_PER_PORT],
                                             EC_T_DWORD adwBdTableOffsets[FIRMWARE_SEND_QUEUE_COUNT_PER_PORT])
{
    EC_T_DWORD dwBufferOffset  = FIRMWARE_OMCMC_DATA_BUFFERS_OFFSET +
                                 PRUSS_PRU_COUNT*FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*
                                 FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE;

    EC_T_DWORD dwBdTableOffset = FIRMWARE_DRAM_BD_TABLES_OFFSET +
                                 PRUSS_PRU_COUNT*FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT*
                                 FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE;

    if( dwCore > 0  )
    {
        dwBufferOffset      += FIRMWARE_SEND_QUEUE_COUNT_PER_PORT*
                               FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*
                               FIRMWARE_DATABLOCK_SIZE;

        dwBdTableOffset     += FIRMWARE_SEND_QUEUE_COUNT_PER_PORT*
                               FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*
                               FIRMWARE_BD_SIZE;
    }

     adwBufferOffsets[0] = dwBufferOffset;
     adwBufferOffsets[1] = adwBufferOffsets[0] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE;
     adwBufferOffsets[2] = adwBufferOffsets[1] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE;
     adwBufferOffsets[3] = adwBufferOffsets[2] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE;

     adwBdTableOffsets[0] = dwBdTableOffset;
     adwBdTableOffsets[1] = adwBdTableOffsets[0] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE;
     adwBdTableOffsets[2] = adwBdTableOffsets[1] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE;
     adwBdTableOffsets[3] = adwBdTableOffsets[2] + FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE;
}

/*
 * Function: ICSS_FIRMWARE_InitializeSendQueues
 * ----------------------------------------------------
 * Each port in Firmware has 4 sending queues (with different priority)
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_FIRMWARE_InitializeSendQueues(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD adwQueueBufferOffsets[FIRMWARE_SEND_QUEUE_COUNT_PER_PORT];
    EC_T_DWORD adwBdTableOffsets[FIRMWARE_SEND_QUEUE_COUNT_PER_PORT];

    EC_T_DWORD dwTxContextAddr       = pPort->pPru->dram.dwVirt + FIRMWARE_DRAM_TX_CONTEXTS_OFFSET;
    EC_T_DWORD dwQueueDescriptorAddr = pPort->pPru->dram.dwVirt + FIRMWARE_DRAM_QUEUE_DESCRIPTORS_OFFSET;

    // Calculate offsets (in L3 OCMC RAM and in PRU DRAM) to port queue1 & queue2
    ICSS_FIRMWARE_CalculateSendQueueOffsets(pPort->dwPruNum, adwQueueBufferOffsets, adwBdTableOffsets);

    //
    // Configure queues in Firmware.
    //

    EC_T_DWORD dwQueue = 0;
    T_FIRMWARE_TX_CONTEXT       *pTxContexts      = (T_FIRMWARE_TX_CONTEXT*) dwTxContextAddr;
    T_FIRMWARE_QUEUE_DESCRIPTOR *pQueueDescriptor = (T_FIRMWARE_QUEUE_DESCRIPTOR*) dwQueueDescriptorAddr;

    for( dwQueue = 0; dwQueue < FIRMWARE_SEND_QUEUE_COUNT_PER_PORT; dwQueue++ )
    {
        //
        // TX Context (in PRU DRAM).
        //

        HW_SET16_NO_BARRIER(&pTxContexts[dwQueue].wOffsetToDataBuffer, adwQueueBufferOffsets[dwQueue]);

        HW_SET16_NO_BARRIER(&pTxContexts[dwQueue].wOffsetToLastDataBuffer, adwQueueBufferOffsets[dwQueue] +
                            FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_DATABLOCK_SIZE -
                            FIRMWARE_DATABLOCK_SIZE);

        HW_SET16_NO_BARRIER(&pTxContexts[dwQueue].wOffsetToBdTable, adwBdTableOffsets[dwQueue]);

        HW_SET16_NO_BARRIER(&pTxContexts[dwQueue].wOffsetToLastBd, adwBdTableOffsets[dwQueue] +
                            FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*FIRMWARE_BD_SIZE -
                            FIRMWARE_BD_SIZE);

        //
        // Queue descriptor (in PRU DRAM)
        //

        HW_SET16_NO_BARRIER(&pQueueDescriptor[dwQueue].wReadBdOffset, adwBdTableOffsets[dwQueue]);
        HW_SET16_NO_BARRIER(&pQueueDescriptor[dwQueue].wWriteBdOffset, adwBdTableOffsets[dwQueue]);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byBusy, EC_FALSE);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byStatus, 0);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byMaxFillLevel, 0);
        HW_SET8_NO_BARRIER(&pQueueDescriptor[dwQueue].byOverflowCount, 0);

        //
        // Configure own copy of sending queue in DDR.
        //

        pPort->sendQueue[dwQueue].wBufferOffset           = adwQueueBufferOffsets[dwQueue];
        pPort->sendQueue[dwQueue].wBdTableOffset          = adwBdTableOffsets[dwQueue];
        pPort->sendQueue[dwQueue].wQueueDescriptorsOffset = dwQueueDescriptorAddr - pPort->pPru->dram.dwVirt;
        pPort->sendQueue[dwQueue].wBdTableEndOffset       = adwBdTableOffsets[dwQueue] +
                                                            FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS*
                                                            FIRMWARE_BD_SIZE;                      // end of queue
   }
}

/*
 * Function: ICSS_FIRMWARE_EnableRx
 * ----------------------------------------------------
 * Notifies firmware that port should be polled.
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_FIRMWARE_EnableRx(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwPortControlAddr =  (pPort->pPru->dram.dwVirt +
                                     FIRMWARE_DRAM_PORT_CONTROL_OFFSET);

    HW_SET8_NO_BARRIER(dwPortControlAddr, FIRMWARE_DRAM_PORT_CONTROL_ENABLE_RX);
}

/*
 * Function: ICSS_PRUSS_PRU_InitializeConstantTable
 * ---------------------------------------------------
 * Initializes C28,C30 registers, which point to SharedMem and OCMC Mem.
 * Addresses here need to be physical addresses as they are passed to ICSS, and in Linux case
 * icssEmacBaseAddressHandle->l3OcmcBaseAddr is a virtual address
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_InitializeConstantTable(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwTempAddr = 0U;
    EC_T_DWORD dwSharedDataRamOffset = PRUSS_SHARED_MEMORY_PHYS_OFFSET;
    EC_T_DWORD dwL3OcmcBase          = pPort->pPruss->l3ocmc.dwPhys;
    EC_T_DWORD dwControlRegisters    = pPort->pPru->control.dwVirt;

    // Set in the constant table C28 pointer to ICSS Shared memory 0x10000
    // Shared memory address is relative to the PRUSS address space
    // Dram0 goes usually first in PRUSS, so it is used as a PRUSS memory base.

    dwTempAddr = (dwControlRegisters + PRUSS_PRU_CTPPR0);
    HW_WR_REG32(dwTempAddr, (dwSharedDataRamOffset & 0x000FFFFFU) >> 8U); // size in 256byte pages

    // Set in constant table C30 pointer to PRUSS On Chip Memory
    // Its physical address is usually 0x40300000.
    // We remove the leading "4", because C30 contains
    // already 0x40nnnn00 and we can control only nnnn
    dwTempAddr = (dwControlRegisters + PRUSS_PRU_CTPPR1);
    HW_WR_REG32(dwTempAddr, (dwL3OcmcBase & 0x00FFFF00U) >> 8U);
}

/*
 * Function: ICSS_PRUSS_MDIO_EnableLinkInterrupt
 * ---------------------------------------------------
 * Enables link change interrupt for port.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_MDIO_EnableLinkInterrupt(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD dwPhySelRegAddr = pPort->pPruss->mdio.dwVirt +
                            PRUSS_MDIO_USERPHYLSEL(pPort->dwPruNum);
    EC_T_DWORD dwPhySelValue = 0;

    // Enable link change interrupt.
    dwPhySelValue = PRUSS_MDIO_USERPHYLSEL_ENABLE_LINK_INTERRUPT_VALUE(pPort->dwPruNum);

    HW_WR_REG32(dwPhySelRegAddr, dwPhySelValue);
}

/*
 * Function: ICSS_PRUSS_PRU_InitializePinmuxGpioInfo
 * ---------------------------------------------------
 * Configures gpio3/gpio5 - pinmux.
 *
 * pPort[in]: port data.
 *
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo)
{
    if( EcLinkIcssBoard_am572x == pPort->eBoardType || EcLinkIcssBoard_am572x_emerson == pPort->eBoardType)
    {
        ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am572x(pPort, pPhyResetInfo, pPhyIntInfo);
    }
    else
    {
        ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am571x(pPort, pPhyResetInfo, pPhyIntInfo);
    }
}

static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am572x(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo)
{
    EC_T_DWORD dwPruss = pPort->dwPrussNum;
    EC_T_DWORD dwPru   = pPort->dwPruNum;
    EC_T_DWORD dwPhyPort = dwPruss*2 + dwPru;

    switch( dwPhyPort )
    {
    case 0:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 6;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR4;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 28;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_CLK0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    case 1:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 7;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR5;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 29;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_DE0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    case 2:
        if (EcLinkIcssBoard_am572x_emerson == pPort->eBoardType)
        {
            pPhyResetInfo->dwInstance = 4;
            pPhyResetInfo->dwPin = 5;
            pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_D4;
            pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio4.dwVirt;
        }
        else
        {
            pPhyResetInfo->dwInstance = 5;
            pPhyResetInfo->dwPin = 8;
            pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR6;
            pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;
        }
        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 30;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_FLD0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD) pPort->platform.mpuGpio3.dwVirt;
        break;
    case 3:
        if (EcLinkIcssBoard_am572x_emerson == pPort->eBoardType)
        {
            pPhyResetInfo->dwInstance = 4;
            pPhyResetInfo->dwPin = 5;
            pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_D4;
            pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio4.dwVirt;
        }
        else
        {
            pPhyResetInfo->dwInstance = 5;
            pPhyResetInfo->dwPin = 9;
            pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR7;
            pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;
        }
        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 31;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_HSYNC0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    default:
        pPhyResetInfo->dwInstance=0;
        pPhyResetInfo->dwPin=0;
        pPhyResetInfo->dwPinMux=0;
        pPhyResetInfo->dwBaseAddr= EC_NULL;

        pPhyIntInfo->dwInstance=0;
        pPhyIntInfo->dwPin=0;
        pPhyIntInfo->dwPinMux=0;
        pPhyIntInfo->dwBaseAddr= EC_NULL;
    }
}

static EC_T_VOID ICSS_PRUSS_PRU_InitializePinmuxGpioInfo_am571x(ICSS_ADAPTER_INTERNAL*  pPort,
                                            GPIO_PINMUX_INFORMATION* pPhyResetInfo,
                                            GPIO_PINMUX_INFORMATION* pPhyIntInfo)
{
    EC_T_DWORD dwPruss = pPort->dwPrussNum;
    EC_T_DWORD dwPru   = pPort->dwPruNum;
    EC_T_DWORD dwPhyPort = dwPruss*2 + dwPru;

    switch( dwPhyPort )
    {
    case 0:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 8;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR6;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 28;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_CLK0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    case 1:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 8;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR6;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 29;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_DE0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    case 2:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 9;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR7;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 30;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_FLD0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD) pPort->platform.mpuGpio3.dwVirt;
        break;
    case 3:
        pPhyResetInfo->dwInstance = 5;
        pPhyResetInfo->dwPin = 9;
        pPhyResetInfo->dwPinMux = CTRL_CORE_PAD_MCASP1_AXR7;
        pPhyResetInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio5.dwVirt;

        pPhyIntInfo->dwInstance = 3;
        pPhyIntInfo->dwPin = 31;
        pPhyIntInfo->dwPinMux = CTRL_CORE_PAD_VIN2A_HSYNC0;
        pPhyIntInfo->dwBaseAddr = (EC_T_DWORD)pPort->platform.mpuGpio3.dwVirt;
        break;
    default:
        pPhyResetInfo->dwInstance=0;
        pPhyResetInfo->dwPin=0;
        pPhyResetInfo->dwPinMux=0;
        pPhyResetInfo->dwBaseAddr= EC_NULL;

        pPhyIntInfo->dwInstance=0;
        pPhyIntInfo->dwPin=0;
        pPhyIntInfo->dwPinMux=0;
        pPhyIntInfo->dwBaseAddr= EC_NULL;
    }
}

/*
 * Function: ICSS_GPIO_Enable
 * ---------------------------------------------------
 * Enables specified GPIO module.
 *
 * baseAdd[in]: address of GPIO module memory.
 *
 *
 * returns: void.
 */
static EC_T_VOID ICSS_GPIO_Enable(EC_T_DWORD baseAdd)
{
    // Clearing the DISABLEMODULE bit in the Control(CTRL) register.
    HW_WR_FIELD32(baseAdd + GPIO_CTRL, GPIO_CTRL_DISABLEMODULE,
                  GPIO_CTRL_DISABLEMODULE_ENABLED);
}

/*
 * Function: ICSS_GPIO_Enable
 * ---------------------------------------------------
 * Configure the specified PIN to be output or input.
 *
 * baseAdd[in]: address of GPIO module memory.
 * dwPinNumber[in]:    pin number, which should be configured.
 * dwPinDirection[in]: direction for pin.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_GPIO_ConfigurePinMode(EC_T_DWORD dwBaseAdd,
                    EC_T_DWORD dwPinNumber,
                    EC_T_DWORD dwPinDirection)
{
    EC_T_DWORD gpioOeValue = HW_RD_REG32(dwBaseAdd + GPIO_OE);

    // Checking if pin is required to be an output pin.
    if (GPIO_PIN_OUTPUT == dwPinDirection)
    {
        gpioOeValue &= ~((EC_T_DWORD) 1<<dwPinNumber);
    }
    else
    {
        gpioOeValue |= (EC_T_DWORD) 1<<dwPinNumber;
    }

    HW_WR_REG32(dwBaseAdd + GPIO_OE, gpioOeValue);
}

/*
 * Function: ICSS_GPIO_PinWrite
 * ---------------------------------------------------
 * Sets high/low pulse for the pin.
 *
 * dwBaseAddr[in]:  gpio memory address.
 * dwPinNumber[in]: pin number, which should be configured.
 * dwPinValue[in]:  high/low.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_GPIO_PinWrite(EC_T_DWORD dwBaseAddr,
                  EC_T_DWORD dwPinNumber,
                  EC_T_DWORD dwPinValue)
{
    if (GPIO_PIN_HIGH == dwPinValue)
    {
        HW_WR_REG32(dwBaseAddr + GPIO_SETDATAOUT, (uint32_t) 1<<dwPinNumber);
    }
    else
    {
        HW_WR_REG32(dwBaseAddr + GPIO_CLEARDATAOUT, (uint32_t) 1<<dwPinNumber);
    }
}

/*
 * Function: ICSS_PHY_Reset
 * ---------------------------------------------------
 * Sets reset pin.
 *
 * pPort[in]:  port data.
 * pICSS_PHY_ResetInfo[in]: pinmux data.
 *
 * returns: void.
 */

static EC_T_VOID ICSS_PHY_Reset(ICSS_ADAPTER_INTERNAL*  pPort,
                         GPIO_PINMUX_INFORMATION* pPhyResetInfo)
{
    /* Do GPIO pin mux */
    HW_WR_REG32(pPort->platform.mpuScm.dwVirt + pPhyResetInfo->dwPinMux, (0x20000 | 0x0E));

    ICSS_GPIO_Enable(pPhyResetInfo->dwBaseAddr);
    ICSS_GPIO_ConfigurePinMode(pPhyResetInfo->dwBaseAddr, pPhyResetInfo->dwPin, GPIO_PIN_OUTPUT);

    ICSS_GPIO_PinWrite(pPhyResetInfo->dwBaseAddr, pPhyResetInfo->dwPin, GPIO_PIN_HIGH);
    ICSS_GPIO_PinWrite(pPhyResetInfo->dwBaseAddr, pPhyResetInfo->dwPin, GPIO_PIN_LOW);
    LinkOsSleep(100);

    ICSS_GPIO_PinWrite(pPhyResetInfo->dwBaseAddr, pPhyResetInfo->dwPin, GPIO_PIN_HIGH);
    LinkOsSleep(100);
}

/*
 * Function: ICSS_PHY_IntConfig
 * ---------------------------------------------------
 * Sets interrupt mode for pin.
 *
 * pPort[in]:       port data.
 * pPhyIntInfo[in]: interrupt data.
 *
 * returns: void.
 */

static EC_T_VOID ICSS_PHY_IntConfig(ICSS_ADAPTER_INTERNAL*  pPort,
                        GPIO_PINMUX_INFORMATION* pPhyIntInfo)
{
    /* Do GPIO pin mux */
    HW_WR_REG32(pPort->platform.mpuScm.dwVirt + pPhyIntInfo->dwPinMux, (0x20000 | 0x0E));

    ICSS_GPIO_Enable(pPhyIntInfo->dwBaseAddr);
    ICSS_GPIO_ConfigurePinMode(pPhyIntInfo->dwBaseAddr, pPhyIntInfo->dwPin, GPIO_PIN_INPUT);
}

/*
 * Function: ICSS_GPIO_InitializePinmux
 * ---------------------------------------------------
 * Initializes pinmux data for reset/interrupt pins.
 *
 * pPort[in]:  port data.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_GPIO_InitializePinmux(ICSS_ADAPTER_INTERNAL* pPort)
{
    GPIO_PINMUX_INFORMATION phyResetInfo;
    GPIO_PINMUX_INFORMATION phyIntInfo;

    ICSS_PRUSS_PRU_InitializePinmuxGpioInfo(pPort, &phyResetInfo, &phyIntInfo);

    //reset phy config
    ICSS_PHY_Reset(pPort, &phyResetInfo);

    // PHY Interrupt lines are configured as GPIO Input in order to avoid any
    // spurious power down signal to TLK105L.
    ICSS_PHY_IntConfig(pPort, &phyIntInfo);
}

/*
 * Function: ICSS_PRU_WriteMemory
 * ---------------------------------------------------
 * Writes data to PRU memory. Used to write firmware.
 *
 * dwMemoryAddr[in]: memory address, where to write.
 * dwWordoffset[in]: offset in words, where to write.
 * dwSourceAddr[in]: from where to copy data.
 * dwBytelength[in]: source buffer size.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRU_WriteMemory(EC_T_DWORD dwMemoryAddr,
                               EC_T_DWORD dwWordoffset,
                               EC_T_DWORD dwSourceAddr,
                               EC_T_DWORD dwBytelength)
{
    EC_T_DWORD i, dwWordlength;
    dwWordlength = (dwBytelength + 3U)>>2U;
    EC_T_DWORD dwTempAddr;

    for (i = 0; i < dwWordlength; i++)
    {
        dwTempAddr = (dwMemoryAddr + (i<<2) + dwWordoffset);
        HW_WR_REG32(dwTempAddr, ((EC_T_DWORD*)dwSourceAddr)[i]);
    }
}

/*
 * Function: ICSS_PRUSS_PRU_LoadFirmware
 * ---------------------------------------------------
 * Writes firmware to PRU code memory.
 *
 * pPort[in]: port data.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_PRUSS_PRU_LoadFirmware(ICSS_ADAPTER_INTERNAL* pPort)
{
    if( pPort->dwPruNum == 0 )
    {
        ICSS_PRU_WriteMemory(pPort->pPru->iram.dwVirt, 0,
                       (EC_T_DWORD) &PRU0_FIRMWARE, sizeof(PRU0_FIRMWARE));
    }
    else
    {
        ICSS_PRU_WriteMemory(pPort->pPru->iram.dwVirt, 0,
                       (EC_T_DWORD) &PRU1_FIRMWARE, sizeof(PRU1_FIRMWARE));
    }
}

/*
 * Function: ICSS_MDIO_ReadPhyReg
 * ---------------------------------------------------
 * Reads PHY status register from TLK105L using MDIO_USERACCESSn register in MDIO.
 * http://www.ti.com/product/TLK105L/datasheet/detailed-description#SLLS9013743
 *
 * dwMdioAddr[in]:   MDIO memory address.
 * dwPhy[in]:        PHY number in MDIO.
 * dwRegisterNo[in]: Register which should be accessed.
 * dwPortNum[in]:    PRU0 or PRU1
 *
 * returns: true, if operation completed successfully.
 */
static EC_T_BOOL ICSS_MDIO_ReadPhyReg(EC_T_DWORD dwMdioAddr,
                               EC_T_DWORD dwPhy,
                               EC_T_DWORD dwRegisterNo,
                               EC_T_WORD  *pData,
                               EC_T_DWORD dwPortNum)
{
    EC_T_DWORD dwRegVal = 0U;
    EC_T_BOOL  bRetVal  = EC_FALSE;

    // Wait till MDIO is busy and performing an operation.
    while(  PRUSS_MDIO_USERACCESS_GO_0x1_VAL ==
            HW_RD_FIELD32(dwMdioAddr + PRUSS_MDIO_USERACCESS(dwPortNum),PRUSS_MDIO_USERACCESS_GO))
    {}

    // Start an MDIO operation.
    HW_SET_FIELD32(dwRegVal, PRUSS_MDIO_USERACCESS_GO, PRUSS_MDIO_USERACCESS_GO_0x1_VAL);
    // it is a read operation.
    HW_SET_FIELD32(dwRegVal, PRUSS_MDIO_USERACCESS_WRITE, PRUSS_MDIO_USERACCESS_WRITE_READ_VAL);
    // Specify PHY number for this operation.
    HW_SET_FIELD32(dwRegVal, PRUSS_MDIO_USERACCESS_PHYADR, dwPhy);
    // Specify the PHY register to be accessed.
    HW_SET_FIELD32(dwRegVal, PRUSS_MDIO_USERACCESS_REGADR, dwRegisterNo);
    // Set all above bits as an atomic operation to the register.
    HW_WR_REG32(dwMdioAddr + PRUSS_MDIO_USERACCESS(dwPortNum), dwRegVal);

    // Wait for command completion.
    while(PRUSS_MDIO_USERACCESS_GO_0x1_VAL ==
        HW_RD_FIELD32(dwMdioAddr + PRUSS_MDIO_USERACCESS(dwPortNum),
                      PRUSS_MDIO_USERACCESS_GO))
    {}

    // Check if the operation was completed.
    if(PRUSS_MDIO_USERACCESS_ACK_PASS_VAL ==
        HW_RD_FIELD32(dwMdioAddr + PRUSS_MDIO_USERACCESS(dwPortNum),
                      PRUSS_MDIO_USERACCESS_ACK))
    {
        *pData = (EC_T_WORD)(HW_RD_FIELD32(dwMdioAddr + PRUSS_MDIO_USERACCESS(dwPortNum),
                                           PRUSS_MDIO_USERACCESS_DATA));
        bRetVal = EC_TRUE;
    }
    else
    {
        bRetVal = EC_FALSE;
    }
    return(bRetVal);
}

/*
 * Function: ICSS_MDIO_ReadPhyReg
 * ---------------------------------------------------
 * This function reads 100mb/10mb speed flag from PHY.
 *
 * pPort[in]: port data.
 *
 * returns: PHY_SPEED, current port speed.
 */
static PHY_SPEED ICSS_PHY_GetSpeed(ICSS_ADAPTER_INTERNAL* pPort)
{
    EC_T_DWORD  dwMdioMemory = pPort->pPruss->mdio.dwVirt;

    EC_T_WORD  dwRegStatus = 0;
    PHY_SPEED  phyStat = PHY_SPEED_100FD;

    // Ask MDIO to read PHY status from the corresponding TLK105L
    // (MDIO has connection to two TLK).
    ICSS_MDIO_ReadPhyReg(dwMdioMemory, pPort->dwPruNum, PHY_PHYSTS, &dwRegStatus, pPort->dwPruNum);

    if( dwRegStatus & PHY_PHYSTS_SPEED_STATUS )
    {
        // Speed is 10.
        if (dwRegStatus & PHY_PHYSTS_DUPLEX_STATUS)
        {
            phyStat = PHY_SPEED_10FD;
        }
        else
        {
            phyStat =  PHY_SPEED_10HD;
        }
    }
    else
    {
        // Speed is 100.
        if (dwRegStatus & PHY_PHYSTS_DUPLEX_STATUS)
        {
            phyStat =  PHY_SPEED_100FD;
        }
        else
        {
            phyStat = PHY_SPEED_100HD;
        }
    }

    return phyStat;
}

/*
 * Function: ICSS_FIRMWARE_SetPortSpeed
 * ---------------------------------------------------
 * Notifies firmware of the current port speed.
 *
 * pPort[in]: port data.
 * speed[in:  port speed.
 *
 * returns: void.
 */
static EC_T_VOID ICSS_FIRMWARE_SetPortSpeed(ICSS_ADAPTER_INTERNAL* pPort, PHY_SPEED speed)
{
    EC_T_BYTE* fwPortSpeedAddr  = (EC_T_BYTE*)(pPort->pPru->dram.dwVirt + FIRMWARE_DRAM_PHY_SPEED_OFFSET);

    switch(speed)
    {
        case PHY_SPEED_100FD:
            *fwPortSpeedAddr = FIRMWARE_PORT_SPEED_100MB;
            break;
        case PHY_SPEED_100HD:
            *(fwPortSpeedAddr) = FIRMWARE_PORT_SPEED_100MB;
            break;
        case PHY_SPEED_10FD:
            *(fwPortSpeedAddr) = FIRMWARE_PORT_SPEED_10MB;
            break;
        case PHY_SPEED_10HD:
            *(fwPortSpeedAddr) = FIRMWARE_PORT_SPEED_10MB;
            break;
        default:
            *(fwPortSpeedAddr) = FIRMWARE_PORT_SPEED_100MB;
            break;
    }
}

/*
 * Function: ICSS_PRUSS_MDIO_GetLinkStatus
 * ---------------------------------------------------
 * Checks MDIO, if the specified port has link.
 *
 * returns: bool, if link present.
 */
static EC_T_BOOL ICSS_PRUSS_MDIO_GetLinkStatus(uint32_t baseAddr, uint32_t phyAddr)
{
    // Check the link bit for the selected PHY Port 0 .. 1
    if(0U != ((HW_RD_REG32(baseAddr + PRUSS_MDIO_LINK)) & ((1U)<<phyAddr)))
    {
        return EC_TRUE;
    }

    return EC_FALSE;
}
