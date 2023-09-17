/*-----------------------------------------------------------------------------
 * ICSS.h               header file
 * Copyright            acontis technologies GmbH, Weingarten, Germany
 * Response             Mikhail Ledyaev
 * Description          Hardware/Firmware low level functions.
 *---------------------------------------------------------------------------*/
#ifndef ICSS_H
#define ICSS_H

#include <LinkOsLayer.h>
#include <EcLink.h>
#include <EcError.h>
#include "ICSSHardware.h"
#include "ICSSFirmware.h"

#define EXCLUDE_LL_DMA 1
#include <LinkLayerDma.h>

/* For internal memory allocation function */
#define ICSS_MEMORY_POOL_BUFFER_COUNT                         64
/* More that maximum size of a Ethernet packet */
#define ICSS_MEMORY_POOL_BUFFER_SIZE                          0x600

#ifndef PRINT
#  if defined __GNUC__
#    define PRINT(msg, ...) LinkOsDbgMsg((msg), ##__VA_ARGS__)
#  else
#    define PRINT(msg, ...) LinkOsDbgMsg((msg), __VA_ARGS__)
#  endif
#endif

#ifdef DEBUG
#  if defined __GNUC__
#    define DBG(msg, ...) PRINT(" DBG: " msg, ##__VA_ARGS__)
#  else
#    define DBG(msg, ...) PRINT(" DBG: " msg, __VA_ARGS__)
#  endif
#else
#  define DBG(msg, ...) (void)0
#endif

/*
 * Memory address triple.
 */

typedef struct
{
    EC_T_DWORD dwPhys;
    EC_T_DWORD dwSize;
    EC_T_DWORD dwVirt;

} ICSS_IO_ADDRESS;

/*
 * PRUx memory.
 */

typedef struct
{
    ICSS_IO_ADDRESS iram;
    ICSS_IO_ADDRESS control;
    ICSS_IO_ADDRESS dram;
    EC_T_BOOL       bMaster;
} ICSS_PRU_CONFIGURATION;

/*
 * PRUSSx memory.
 */

typedef struct
{
    ICSS_PRU_CONFIGURATION pru[2];

    ICSS_IO_ADDRESS interruptController;
    ICSS_IO_ADDRESS cfg;
    ICSS_IO_ADDRESS iep;
    ICSS_IO_ADDRESS miiRtCfg;
    ICSS_IO_ADDRESS mdio;
    ICSS_IO_ADDRESS shrdram2;
    ICSS_IO_ADDRESS l3ocmc;
} ICSS_PRUSS_CONFIGURATION;

/*
 * Board memory.
 */

typedef struct
{
    ICSS_PRUSS_CONFIGURATION pruss[2];                    /* PRUSS1 & PRUSS2 memory */
    ICSS_IO_ADDRESS mpuGpio3;                             /* GPIO3 */
    ICSS_IO_ADDRESS mpuGpio4;                             /* GPIO4 */
    ICSS_IO_ADDRESS mpuGpio5;                             /* GPIO5 */
    ICSS_IO_ADDRESS mpuScm;                               /* System Control Module for PinMux */
    ICSS_IO_ADDRESS cmCore;                               /* CM_CORE PRCM submodule */
} ICSS_BOARD_CONFIGURATION;


/*
 * Queue Information for port instance structure.
 */

typedef struct _ICSS_T_QUEUE_INFORMATION
{
    EC_T_WORD wBufferOffset;                                /* Offset to queue memory buffer in L3 OCMC RAM */
    EC_T_WORD wBdTableOffset;                               /* Offset to queue buffer descriptor table */
    EC_T_WORD wQueueDescriptorsOffset;                      /* Offset to queue descriptor in SharedRam/DRAM */
    EC_T_WORD wBdTableEndOffset;                            /* End of BD Table for this queue */
} ICSS_QUEUE_INFORMATION;

/*
 * Port instance data.
 */

typedef struct _ICSS_INSTANCE_INTERNAL
{
    EC_T_LINK_ICSS_BOARD               eBoardType;
    LinkLayerBuffers                   memoryPool;        /* We use own memory allocation function */
    ICSS_BOARD_CONFIGURATION           platform;          /* Addresses (mapped to user space) of each hardware module */
    ICSS_PRU_CONFIGURATION            *pPru;              /* Port PRU pointer to platform */
    ICSS_PRUSS_CONFIGURATION          *pPruss;            /* Port PRUSS pointer t platform */
    EC_T_DWORD                         dwPrussNum;        /* PRUSS 0..1 */
    EC_T_DWORD                         dwPruNum;          /* PRU   0..1 */
    EC_T_BYTE                          abyMac[6];         /* Port MAC address */
    EC_T_BOOL                          bLink;             /* Link exists */
    ICSS_QUEUE_INFORMATION             receiveQueue[2];   /* port receiving queues */
    ICSS_QUEUE_INFORMATION             sendQueue[4];      /* port sending queues */
    EC_T_DWORD                         dwLosalHandle;     /* ATEMSYS handle */
    EC_T_VOID*                         pvLinkOsContext;   /* ATEMSYS context */
    EC_T_LINK_PARMS_ICSS               oInitParms;
} ICSS_ADAPTER_INTERNAL;


/*
 * Function: ICSS_MapIoMemory
 * -----------------------------------------------
 *   Maps all needed board memory into user space.
 *
 *   returns: EC_TRUE, if memory has been mapped.
 */
EC_T_BOOL ICSS_MapIoMemory(ICSS_ADAPTER_INTERNAL* pPort);
EC_T_VOID ICSS_UnmapIoMemory(ICSS_ADAPTER_INTERNAL* pPort);

/*
 * Function: ICSS_InitializePruss
 * -------------------------------
 *   Initializes PRUSS if not yet initialized in another instance.
 *
 *   pPort[in]: port information structure.
 *
 *   returns: EC_TRUE, if PRUSS was initialized.
 */
EC_T_BOOL  ICSS_InitializePruss(ICSS_ADAPTER_INTERNAL*);

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
EC_T_BOOL  ICSS_InitializePru(ICSS_ADAPTER_INTERNAL*);

/*
 * Function: ICSS_HaltPru
 * ---------------------------------------------------
 * Halts PRU.
 *
 * pPort[in]: pointer to port structure.
 *
 * returns: void.
 */
EC_T_VOID  ICSS_HaltPru(ICSS_ADAPTER_INTERNAL* port);

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
EC_T_BOOL  ICSS_UpdatePortLinkStatus(ICSS_ADAPTER_INTERNAL* port);

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
EC_T_BOOL  ICSS_SendPacket(ICSS_ADAPTER_INTERNAL* pPort, EC_T_BYTE* pbyPacket, EC_T_DWORD dwPacketLength);

/*
 * Function: ICSS_ReceiveGetPendingPacketSize
 * ---------------------------------------------------
 * Checks an incoming queue if it contains a packet.
 *
 * pPort[in]:          pointer to port structure.
 *
 * returns: size of first waiting packet in queue, 0 - instead.
 */
EC_T_DWORD ICSS_ReceiveGetPendingPacketSize(ICSS_ADAPTER_INTERNAL* port);

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
EC_T_DWORD ICSS_ReceivePacket(ICSS_ADAPTER_INTERNAL* port, EC_T_BYTE* userBuffer, EC_T_DWORD userBufferSize, EC_T_BOOL *pbMore);
#endif
