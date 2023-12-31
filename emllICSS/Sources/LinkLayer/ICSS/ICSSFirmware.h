/*-----------------------------------------------------------------------------
 * firmware.h           header file
 * Copyright            acontis technologies GmbH, Weingarten, Germany
 * Response             Mikhail Ledyaev
 * Description          Firmware registers for ICSS LLD.
 *---------------------------------------------------------------------------*/

#ifndef FIRMWARE_H
#define FIRMWARE_H

/*
 * Common parameters.
 */

/* One buffer descriptor is 4 bytes */
#define FIRMWARE_BD_SIZE                                      (4U)
/* Size of each data block */
#define FIRMWARE_DATABLOCK_SIZE                               (32U)

/* Firmware has 4 priority queues to transmit data */
#define FIRMWARE_SEND_QUEUE_COUNT_PER_PORT                    (4)
/* Firmware has 2 priority queues (VLAN) to receive data */
#define FIRMWARE_RECEIVE_QUEUE_COUNT_PER_PORT                 (2)



/*
 * PRUSS Dram0/Dram1 memory 12kb map. Contains data for port0&port 1.
 * Used to store:
 *   Buffer Descriptor tables for each sending queue.
 *   Port control registers.
 *   Queue Descriptor structure for each queue.
 *   TxContext structure for each queue.
 */

/* size of sending queue in blocks  */
#define FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS                (97U)
/* Size in bytes for all bd tables of one port */
#define FIRMWARE_DRAM_TRANSMIT_BD_TABLES_TOTALSIZE            (FIRMWARE_TRANSMIT_QUEUE_SIZE_IN_BLOCKS \
                                                               * 4U                                        \
                                                               * 2U                                        \
                                                               * FIRMWARE_BD_SIZE)

/* Buffer Descriptors are located here */
#define FIRMWARE_DRAM_BD_TABLES_OFFSET                        ((EC_T_DWORD)0x400U)
#define FIRMWARE_DRAM_BD_TABLES_EOF_OFFSET                    (FIRMWARE_DRAM_BD_TABLES_OFFSET + \
                                                               FIRMWARE_SHAREDRAM_RECEIVE_BD_TABLES_TOTALSIZE + \
                                                               FIRMWARE_DRAM_TRANSMIT_BD_TABLES_TOTALSIZE )

#define FIRMWARE_RELEASE_2_OFFSET                             (FIRMWARE_DRAM_BD_TABLES_EOF_OFFSET + 4U)


/* Queue Descriptors */
#define FIRMWARE_DRAM_QUEUE_DESCRIPTORS_OFFSET                (0x1EC0U)

/* Tx Contexts */
#define FIRMWARE_DRAM_TX_CONTEXTS_OFFSET                      (FIRMWARE_DRAM_QUEUE_DESCRIPTORS_OFFSET + 32U)

/* Statistics */
#define FIRMWARE_DRAM_STATISTICS_OFFSET                       (0x1F00U)

#define FIRMWARE_DRAM_STAT_SIZE                               (0x90U)

#define FIRMWARE_DRAM_PHY_SPEED_OFFSET                        (FIRMWARE_DRAM_STATISTICS_OFFSET + \
                                                               FIRMWARE_DRAM_STAT_SIZE           \
                                                               + 4U)

#define FIRMWARE_DRAM_PORT_STATUS_OFFSET                      (FIRMWARE_DRAM_STATISTICS_OFFSET + \
                                                               FIRMWARE_DRAM_STAT_SIZE +         \
                                                               8U)

#define FIRMWARE_DRAM_PORT_CONTROL_OFFSET                     (FIRMWARE_DRAM_STATISTICS_OFFSET + \
                                                               FIRMWARE_DRAM_STAT_SIZE +         \
                                                               14U)
#define FIRMWARE_DRAM_PORT_CONTROL_ENABLE_RX                  (0)

#define FIRMWARE_DRAM_PORT_MAC_OFFSET                         (FIRMWARE_DRAM_STATISTICS_OFFSET + \
                                                               FIRMWARE_DRAM_STAT_SIZE +         \
                                                               18U)

#define FIRMWARE_DRAM_RX_INT_STATUS_OFFSET                    (FIRMWARE_DRAM_STATISTICS_OFFSET + \
                                                               FIRMWARE_DRAM_STAT_SIZE +         \
                                                               24U)


/*
 * PRUSS Shared Memory 32kb (shrdram2) map.
 * Used to store:
 *   Buffer Descriptor tables for each receiving queue.
 *   RxContext structure for each queue.
 *   Offsets to Buffer Descriptor tables.
 *   Offsets to data buffers in L3 OCMC.
 *   Queue descriptor structure for each queue.
 *   Queue sizes.
 */

/* size of receiving queue in blocks */
#define FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS                 (194U)
/* Size in bytes for all bd tables of one port */
#define FIRMWARE_SHAREDRAM_RECEIVE_BD_TABLES_TOTALSIZE        (FIRMWARE_RECEIVE_QUEUE_SIZE_IN_BLOCKS \
                                                               * 4                                             \
                                                               * FIRMWARE_BD_SIZE)

/* Buffer Descriptors are located here */
#define FIRMWARE_SHAREDRAM_BD_TABLES_OFFSET                   ((EC_T_DWORD)0x400U)


/* Host Port Rx Context OFFSET, strange relationship to sending queues.
 * Probably because of switch queues.*/
#define FIRMWARE_SHAREDRAM_RX_CONTEXTS_OFFSET                 (FIRMWARE_RELEASE_2_OFFSET + 4)

/* For each queue there is a pointer to its bd table */
#define FIRMWARE_SHAREDRAM_OFFSETS_TO_BD_TABLES_OFFSET        (FIRMWARE_SHAREDRAM_RX_CONTEXTS_OFFSET + 32U)

/* For each queue there is a pointer to its data buffer in ocmc */
#define FIRMWARE_SHAREDRAM_BUFFER_OFFSETS_OFFSET              (FIRMWARE_SHAREDRAM_RX_CONTEXTS_OFFSET + 40U)

/* for each queue there is a structure, describing this queue */
#define FIRMWARE_SHAREDRAM_QUEUE_DESCRIPTORS_OFFSET           (FIRMWARE_SHAREDRAM_RX_CONTEXTS_OFFSET + 64U)

/* For each queue there is a its size in blocks */
#define FIRMWARE_SHAREDFRAM_QUEUE_SIZES_OFFSET                (0x1E30U)

/*
 * RxContext structure.
 *
 * Description: Each of two receive queues for each port has own RxContext.
 */
typedef struct _T_FIRMWARE_RX_CONTEXT
{
    EC_T_WORD wOffsetToDataBuffer;         /* Offset to queue data buffer in L3 OCMC */
    EC_T_WORD wOffsetToQueueDescriptors;   /* Offset to queue descriptor in SharedRam */
    EC_T_WORD wOffsetToBdTable;            /* Offset to queue buffer descriptor table in SharedRam */
    EC_T_WORD wOffsetToLastBd;             /* Offset to last buffer descriptor in bd table of queue */
} T_FIRMWARE_RX_CONTEXT;

/*
 * TxContext structure.
 *
 * Description: Each of four sending queues for each port has own TxContext.
 */
typedef struct _T_FIRMWARE_TX_CONTEXT
{
    EC_T_WORD wOffsetToDataBuffer;         /* Offset to queue data buffer in L3 OCMC */
    EC_T_WORD wOffsetToLastDataBuffer;     /* Offset to last 32b block of the queue data buffer in L3 OCMC */
    EC_T_WORD wOffsetToBdTable;            /* Offset to queue buffer descriptor table in PRU DRAM */
    EC_T_WORD wOffsetToLastBd;             /* Offset to last buffer descriptor in BD table of queue in PRU DRAM*/
} T_FIRMWARE_TX_CONTEXT;

/*
 * Queue Descriptor structure.
 *
 * Description: has pointers to active buffer descriptors, flags.
 * Located in SharedMem for receiving queues and in Dram0/Dram1 for
 * sending queues.
 */
typedef struct _FIRMWARE_QUEUE_DESCRIPTOR
{
    EC_T_WORD wReadBdOffset;               /* Offset to buffer descriptor of next packet to be read */
    EC_T_WORD wWriteBdOffset;              /* Offset to buffer descriptor of next packet to be write */
    EC_T_BYTE byBusy;                      /* Flag indicating that queue is being currently modified */
    EC_T_BYTE byStatus;                    /* not used */
    EC_T_BYTE byMaxFillLevel;              /* not used */
    EC_T_BYTE byOverflowCount;             /* not used */
} T_FIRMWARE_QUEUE_DESCRIPTOR;

/*
 * L3 OCMC Memory 512kb (mmio-sram) map.
 * Used to store:
 *   Data Buffers for receiving queues.
 */

#define FIRMWARE_OMCMC_DATA_BUFFERS_OFFSET                    (0x0000U)



/*
 * Other firmware related data.
 *
 */

/* Maximum size of a packet */
#define FIRMWARE_MAXMTU                                       (1518U)
/* Minimum valid size ( DA + SA + Ethertype) */
#define FIRMWARE_MINMTU                                       (14U)
/**Minimum supported size of Ethernet frame*/
#define FIRMWARE_MINIMUM_ETHERNET_FRAME_SIZE                  (60U)


/* FIRMWARE_DRAM_PORT_CONTROL_ADDR */
#define FIRMWARE_PORT_CONTROL_DISABLE                         (0u)
#define FIRMWARE_PORT_CONTROL_ENABLE                          (1u)


/* FIRMWARE_DRAM_PHY_SPEED_OFFSET */
#define FIRMWARE_PORT_SPEED_10MB                              (0xa)
#define FIRMWARE_PORT_SPEED_100MB 0x64


/* Parse Buffer Descriptor */
#define FIRMWARE_GET_PAKET_SIZE_FROM_BD(dwBufferDescriptor)   ((0x1ffc0000U&dwBufferDescriptor)>>18U);



/*
 * Firmware Binary.
 *
 */

const unsigned int PRU0_FIRMWARE[] =  {
     0x21000300,
     0x00000111,
     0x83010004,
     0x240111e0,
     0x241c4081,
     0x80813c80,
     0x248301c0,
     0x24000480,
     0x241c4481,
     0x80813c80,
     0x2effbd80,
     0x240000ff,
     0x244004df,
     0x1f1ff6f6,
     0x24010093,
     0x240002d3,
     0x24004c41,
     0x241ea894,
     0x90941854,
     0xcb005477,
     0x1f15f6f6,
     0xc914ff08,
     0x2300d0d8,
     0x2e8a2182,
     0xc9050a07,
     0x61121202,
     0x230052d8,
     0x694c4104,
     0x01014141,
     0xd101f702,
     0x7f0000f9,
     0x2c204102,
     0x514e4103,
     0x01014141,
     0x21002400,
     0x24004c41,
     0x51000214,
     0x51010214,
     0x51020228,
     0x2a000000,
     0xc915f607,
     0x91741a15,
     0xc9031505,
     0x241ea8d5,
     0x90d51815,
     0x1d021515,
     0x80d51815,
     0xd71ff6e2,
     0xcf10ffe1,
     0x240026ff,
     0xc915f604,
     0x91101b14,
     0x13011414,
     0x81101b14,
     0x1f1ff6f6,
     0x21001100,
     0x2100a600,
     0xcf1ff6ef,
     0xc915f612,
     0xd114f608,
     0x241ea8d5,
     0x90d51815,
     0x1f021515,
     0x80d51815,
     0xc9041502,
     0x240028ff,
     0x21015d00,
     0x91741a14,
     0xc9051408,
     0x241ea8d5,
     0x90d51815,
     0x1f021515,
     0x80d51815,
     0xc9041502,
     0x240028ff,
     0x21015d00,
     0x2301ccd8,
     0x21002800,
     0xd102f702,
     0xc903f702,
     0x230301d8,
     0x21002800,
     0x2eff8999,
     0x241f9e8a,
     0x908a988a,
     0xc900ea18,
     0x1f01f7f7,
     0x91501b0d,
     0xc901ed04,
     0x1f01eded,
     0x81501b0d,
     0x7900000e,
     0xc900ed08,
     0x1f00eded,
     0x81501b0d,
     0x7900000a,
     0xc913ff04,
     0x240080df,
     0x241f68ed,
     0x79000007,
     0x688bc30b,
     0x68cb840a,
     0x688cc409,
     0x241f84ed,
     0x79000002,
     0x241f6ced,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0x1f041919,
     0x79000025,
     0xd1000207,
     0x68ebe204,
     0x688c8303,
     0x1f001919,
     0x79000017,
     0x241f84ed,
     0x7f0000f5,
     0x241f90ed,
     0x90ed3895,
     0xc900f50a,
     0x0b08f5f5,
     0x61019506,
     0x0501f5f5,
     0x0908f5f5,
     0x1f00f5f5,
     0x80ed3895,
     0x21008200,
     0x241f64ed,
     0x7f0000e9,
     0x2eff0194,
     0x68f4e205,
     0x68948304,
     0x1f1cf6f6,
     0x1f001919,
     0x79000003,
     0x1f001919,
     0x1f1bf6f6,
     0x2e8a0012,
     0x2e8a1782,
     0x24000179,
     0x24008194,
     0x68948505,
     0x0b054514,
     0x61041403,
     0x24000079,
     0x79000001,
     0x69007903,
     0x241c4882,
     0x79000008,
     0x69017903,
     0x241c5082,
     0x79000005,
     0x69027903,
     0x241c5882,
     0x79000002,
     0x241c6082,
     0xc9001902,
     0x90827c9c,
     0x24000000,
     0x2f058999,
     0x2e8a0012,
     0x61201203,
     0x24004d41,
     0x2100a600,
     0x20d80000,
     0x24000000,
     0x2e858999,
     0xc901f726,
     0xd1041925,
     0xd1031904,
     0x2e8a0012,
     0x2e8a1782,
     0x79000003,
     0x2e8a8012,
     0x2e8a9782,
     0xd1031903,
     0x61201219,
     0x79000003,
     0x59201217,
     0x24000037,
     0x24000861,
     0x69009a07,
     0x90dc3c94,
     0x1094949b,
     0x10d4d4da,
     0x049ddaf5,
     0x0903f5f5,
     0x00f59cdb,
     0x82dbfe82,
     0x01209a9a,
     0x68ddda04,
     0x109d9dda,
     0x109c9cdb,
     0x79000003,
     0x0120dbdb,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0x15081919,
     0x109898d8,
     0x24000000,
     0x2f058999,
     0xd114ff02,
     0x21002800,
     0x2100d000,
     0xd101f716,
     0x241f68ed,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0xc913ff02,
     0x240080df,
     0x91501b0d,
     0xc901ed04,
     0x1f01eded,
     0x81501b0d,
     0x79000004,
     0xc900ed07,
     0x1f00eded,
     0x81501b0d,
     0x241f6ced,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0x1d01f7f7,
     0x240004df,
     0x79000070,
     0x24000000,
     0x2e858999,
     0xd1041942,
     0xd1031905,
     0x2e8a0012,
     0x2e8a1782,
     0x71201217,
     0x79000004,
     0x2e8a8012,
     0x2e8a9782,
     0x59201211,
     0x82dbfe82,
     0x01209a9a,
     0x68ddda04,
     0x109d9dda,
     0x109c9cdb,
     0x79000003,
     0x0120dbdb,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0xc9031903,
     0x2e8a0f82,
     0x79000004,
     0x2e8a9782,
     0x0520f220,
     0x79000002,
     0x10f2f220,
     0x5100200c,
     0x8edbde82,
     0x00209a9a,
     0x71042009,
     0x68ddda03,
     0x109d9dda,
     0x79000002,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0x24020494,
     0x90942095,
     0xc905f506,
     0x241f60ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x1f041919,
     0xc904f506,
     0x241f80ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x1f041919,
     0x91501b15,
     0xc9021503,
     0x241f7ced,
     0x79000003,
     0xc9031508,
     0x241f78ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x24000c35,
     0x81501b35,
     0x1f041919,
     0x240004df,
     0xc9041904,
     0x1d1cf6f6,
     0x1d1bf6f6,
     0x21014a00,
     0x24000000,
     0x2f058999,
     0x24001800,
     0x2e858782,
     0x241f9af5,
     0x80f51882,
     0x90c43c94,
     0x24000095,
     0x050482d5,
     0x0902d5d5,
     0x1f00d5d5,
     0x80d43c95,
     0x6083c203,
     0x0483c295,
     0x21014200,
     0x0483c595,
     0x01049595,
     0x0485c2d5,
     0x00d59595,
     0x0b029595,
     0x0106c494,
     0x90941c55,
     0x70551503,
     0x10151555,
     0x80941c55,
     0x0102c494,
     0x80941cc2,
     0x0105dc94,
     0x90943c15,
     0x11fc1515,
     0xc9065903,
     0x13041515,
     0x01015555,
     0x80943c15,
     0x1d01f7f7,
     0xd1041903,
     0x1f03f7f7,
     0x240024ff,
     0x24028483,
     0x240010e2,
     0x80832082,
     0x1f16ffff,
     0xc900f703,
     0x24004e41,
     0x21024800,
     0x21002800,
     0xc914f60b,
     0xc911f60a,
     0xd112f609,
     0x241ea894,
     0x90941854,
     0x1f015454,
     0x80941854,
     0x241eac94,
     0x90943895,
     0x0101f5f5,
     0x80943895,
     0x1d12f6f6,
     0xd114f606,
     0x2301c285,
     0x91701a06,
     0x11af0606,
     0x81701a06,
     0x2301c885,
     0x24002806,
     0x81741a06,
     0x241e9884,
     0x90847882,
     0x81907a82,
     0x241ea084,
     0x90843884,
     0x00e4e2e2,
     0x0300e3e3,
     0x241e9884,
     0x80847882,
     0x241ea484,
     0x90843884,
     0x04e4e2e2,
     0x0700e3e3,
     0x81a07a82,
     0x2301c285,
     0x91701a06,
     0x13500606,
     0x81701a06,
     0x2301c885,
     0x24001082,
     0x90821b00,
     0x13040000,
     0x80821b00,
     0x1f14f6f6,
     0x21004c00,
     0xcd15f68b,
     0x117f3636,
     0x11c05656,
     0x2301c285,
     0x91701a86,
     0x11af0606,
     0x81701a86,
     0x24002806,
     0x81741a06,
     0x2301c885,
     0x2eff8385,
     0x81907a85,
     0x81a07a85,
     0x24001082,
     0x90821b00,
     0x11fb0000,
     0x80821b00,
     0x21001500,
     0x1082828f,
     0x908f388b,
     0x0903cbce,
     0x0b05cece,
     0x010ccef5,
     0x0906f5e5,
     0x0904f5f5,
     0x00f5e5f5,
     0x10f5f5e0,
     0x91107a94,
     0x91a07a85,
     0x48f5e603,
     0x64f5e680,
     0x74f4e57f,
     0x04f4e5f4,
     0x06f5e6f5,
     0x70f4e005,
     0x5901f504,
     0xcd13f67a,
     0x241ef894,
     0x2101ef00,
     0xc913f604,
     0x1f12f6f6,
     0x1f11f6f6,
     0x21020300,
     0xc911f60a,
     0xd112f609,
     0x241ea894,
     0x90941854,
     0x1f015454,
     0x80941854,
     0x241eac94,
     0x90943895,
     0x0101f5f5,
     0x80943895,
     0x1f12f6f6,
     0x1f11f6f6,
     0x21020300,
     0x24000000,
     0x2e850015,
     0xd700f5ff,
     0x1f00f4f4,
     0x2f050014,
     0x20850000,
     0x24000000,
     0x1d00f4f4,
     0x2f050014,
     0x20850000,
     0x241f98c0,
     0x90c01800,
     0xd101e003,
     0x1d17f6f6,
     0x79000002,
     0x1f17f6f6,
     0xd100f776,
     0xd100e019,
     0x79000017,
     0x1d00f7f7,
     0x1f1effff,
     0x241f99e0,
     0x90e01802,
     0x01010202,
     0x80e01802,
     0x1d1af6f6,
     0x1d19f6f6,
     0xd116f609,
     0x7900000d,
     0x1d00f7f7,
     0x241f70e0,
     0x79000007,
     0x241f99e3,
     0x24000002,
     0x80e31802,
     0x79000003,
     0x1d16f6f6,
     0x241f50e0,
     0x90e03882,
     0x0101e2e2,
     0x80e03882,
     0x21002800,
     0x241ef894,
     0x241eb8cd,
     0x241ed884,
     0xc917f603,
     0x91381b00,
     0xd701e0ee,
     0x569484f9,
     0x0108cdcd,
     0x90cd7882,
     0x01088484,
     0x5682c2f9,
     0xc915f60c,
     0x241ee0d5,
     0x68d58408,
     0xd712f6f5,
     0x1f13f6f6,
     0x241ea8d5,
     0x90d51815,
     0x1d021515,
     0x80d51815,
     0x21019c00,
     0x1d13f6f6,
     0x21019c00,
     0x90847890,
     0x1f00f7f7,
     0x0491828f,
     0x09038f8f,
     0x008f90cf,
     0x1082828f,
     0x908f388b,
     0x0903cbce,
     0x0b05cece,
     0x2400008e,
     0x241f9482,
     0x908218c2,
     0x1f07f7f7,
     0x51644202,
     0x1d07f7f7,
     0xd114ff34,
     0x92cffe82,
     0xc917f604,
     0x91381b00,
     0xc901e002,
     0x7f0000c8,
     0xd100e202,
     0x79000007,
     0x2eff0180,
     0x68e0e204,
     0x68808303,
     0x1f1af6f6,
     0x79000002,
     0x1f19f6f6,
     0xc915f604,
     0x91407a94,
     0x241eb080,
     0x80807894,
     0x24000861,
     0x31070003,
     0x2c2261fe,
     0x01046161,
     0x0120cfcf,
     0x01208e8e,
     0x1f01eded,
     0x2400002d,
     0x24000000,
     0x2f05898d,
     0xc917f604,
     0x91381b00,
     0xc900e002,
     0x7f0000a4,
     0xd114ff15,
     0x68d18f04,
     0x1091918f,
     0x1d01eded,
     0x109090cf,
     0x92cffe82,
     0x24000861,
     0x31060003,
     0x2c2261fe,
     0x01046161,
     0x011ccfcf,
     0x011c8e8e,
     0x24001c2d,
     0x24000000,
     0x2f05898d,
     0xc917f604,
     0x91381b00,
     0xc900e002,
     0x7f000091,
     0x21002800,
     0x1d00f7f7,
     0x2300d0d8,
     0x241f98c0,
     0x90c01800,
     0xd100e005,
     0x1d00f7f7,
     0x1f1effff,
     0x1f1ff6f6,
     0x21002800,
     0x24000000,
     0x2e85898d,
     0x241f98c2,
     0x90c21c02,
     0xd100e203,
     0x1d17f6f6,
     0x79000002,
     0x1f17f6f6,
     0xc915f60f,
     0x91741a15,
     0xd1031502,
     0x21002800,
     0x241eb095,
     0x90957894,
     0x91407a82,
     0x68f4e202,
     0x56f5e3fe,
     0x241ea8d5,
     0x90d51815,
     0xc9031504,
     0xc913f603,
     0x241eb880,
     0x80807882,
     0xc917f604,
     0x91381b00,
     0xc900e002,
     0x7f00006c,
     0x91683b80,
     0x0b01e0e0,
     0x71600006,
     0x241f8884,
     0x90843882,
     0x0101e2e2,
     0x80843882,
     0x2102af00,
     0x24028084,
     0x90842082,
     0xc907e209,
     0x240000e2,
     0x1f07e2e2,
     0x80842082,
     0x241f8c84,
     0x90843882,
     0x0101e2e2,
     0x80843882,
     0x2102af00,
     0x0d60004a,
     0x51004a2f,
     0x71204a02,
     0x2400204a,
     0xd114ff2d,
     0xc901ed08,
     0x68d18f05,
     0x1091918f,
     0x1d01eded,
     0x109090cf,
     0x21028a00,
     0x01048f8f,
     0x1d01eded,
     0x048ece8a,
     0x49208a04,
     0x24000000,
     0x2f05898d,
     0x2102b200,
     0x58cfd005,
     0x04d0cf02,
     0x0d200222,
     0x584a2202,
     0x1022224a,
     0x92cffe82,
     0x24000861,
     0x0b024a8b,
     0x308b0003,
     0x2c2261fe,
     0x01046161,
     0xc917f604,
     0x91381b00,
     0xc900e002,
     0x7f000038,
     0x11034a14,
     0x51001406,
     0x51011404,
     0x2c21619e,
     0x01026161,
     0x51021402,
     0x2c20611e,
     0x004a2d2d,
     0x61202d03,
     0x1f01eded,
     0x05202d2d,
     0x004acfcf,
     0x004a8e8e,
     0x24000000,
     0x2f05898d,
     0x21002800,
     0x2300d0d8,
     0x1f1effff,
     0x1d00f7f7,
     0x21002800,
     0x048ece6a,
     0x51006a30,
     0x01046a0b,
     0x704a0b04,
     0x1f1ef6f6,
     0x704a6a02,
     0x104a4a6a,
     0x58cfd005,
     0x04d0cf02,
     0x0d200222,
     0x586a2202,
     0x1022226a,
     0xc917f605,
     0x91381b00,
     0xc900e003,
     0x1f16f6f6,
     0x7f000013,
     0xd114ff3d,
     0x106a6a00,
     0x9ecfde02,
     0x24000861,
     0x0b026a8b,
     0x308b0003,
     0x2c2261fe,
     0x01046161,
     0x11036a14,
     0x51001406,
     0x51011404,
     0x2c21619e,
     0x01026161,
     0x51021402,
     0x2c20611e,
     0x006a8e8e,
     0x046a4a4a,
     0x006acfcf,
     0x006a2d2d,
     0x61202d07,
     0x05202d2d,
     0x68d18f04,
     0x1091918f,
     0x109090cf,
     0x2102dd00,
     0x01048f8f,
     0xd11ef602,
     0x50ce8e05,
     0x1d1ef6f6,
     0x24000000,
     0x2f05898d,
     0x21002800,
     0xd107f703,
     0x59044a02,
     0x21002800,
     0x242c00df,
     0x1d1ff6f6,
     0x1d1ef6f6,
     0x24000000,
     0x2f05898d,
     0x1f02f7f7,
     0x51002d05,
     0x68d18f03,
     0x1091918f,
     0x2102f100,
     0x01048f8f,
     0x80cd188f,
     0x1d00f7f7,
     0xc917f60c,
     0x241f99e0,
     0x90e01804,
     0x4901e404,
     0x5100e408,
     0x241f54e0,
     0x7d0000e9,
     0x490fe403,
     0x241f58e0,
     0x7d0000e6,
     0x241f5ce0,
     0x7d0000e4,
     0x21002800,
     0x2300d0d8,
     0x10d8d898,
     0xc902f72b,
     0x241f00e4,
     0x24000000,
     0x2e85838d,
     0x0104cece,
     0x010ce4e8,
     0x90e83882,
     0x00cee2e2,
     0x80e83882,
     0x4940ce03,
     0x0120e4e8,
     0x21031d00,
     0x497fce03,
     0x0124e4e8,
     0x21031d00,
     0x49ffce03,
     0x0128e4e8,
     0x21031d00,
     0x2401ffe3,
     0x48e3ce03,
     0x012ce4e8,
     0x21031d00,
     0x2403ffe3,
     0x48e3ce03,
     0x0130e4e8,
     0x21031d00,
     0x0134e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0xc91af604,
     0x0100e4e8,
     0x1d1af6f6,
     0x79000006,
     0xc919f604,
     0x0104e4e8,
     0x1d19f6f6,
     0x79000002,
     0x0108e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x1d02f7f7,
     0xc903f72c,
     0x241f9ae2,
     0x90e21889,
     0x241f00e4,
     0xc91cf603,
     0x0110e4e8,
     0x79000005,
     0xc91bf603,
     0x0114e4e8,
     0x79000002,
     0x0118e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x011ce4e8,
     0x90e83882,
     0x0089e2e2,
     0x80e83882,
     0x49409a03,
     0x0138e4e8,
     0x21035100,
     0x497f8903,
     0x013ce4e8,
     0x21035100,
     0x49ff8903,
     0x0140e4e8,
     0x21035100,
     0x2401ffe3,
     0x48e38903,
     0x0144e4e8,
     0x21035100,
     0x2403ffe3,
     0x48e38903,
     0x0148e4e8,
     0x21035100,
     0x014ce4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x10e8e8e3,
     0x1d1cf6f6,
     0x1d1bf6f6,
     0x1d03f7f7,
     0x79000001,
     0x109898d8,
     0x20d80000 };

const unsigned int PRU1_FIRMWARE[] =  {
     0x21000300,
     0x00000111,
     0x83010004,
     0x240111e0,
     0x241c4081,
     0x80813c80,
     0x248301c0,
     0x24000480,
     0x241c4481,
     0x80813c80,
     0x2effbd80,
     0x240000ff,
     0x244004df,
     0x1f1ff6f6,
     0x24010093,
     0x240002d3,
     0x24004c41,
     0x241ea894,
     0x90941854,
     0xcb005478,
     0x1f15f6f6,
     0xc914ff08,
     0x2300d0d8,
     0x2e8a2182,
     0xc9050a07,
     0x61121202,
     0x230052d8,
     0x694c4104,
     0x01014141,
     0xd101f702,
     0x7f0000f9,
     0x2c204102,
     0x514e4103,
     0x01014141,
     0x21002400,
     0x24004c41,
     0x51000214,
     0x51010214,
     0x51020228,
     0x2a000000,
     0xc915f607,
     0x91741a15,
     0xc9041505,
     0x241ea8d5,
     0x90d51815,
     0x1d021515,
     0x80d51815,
     0xd71ff6e2,
     0xcf10ffe1,
     0x240027ff,
     0xc915f604,
     0x91141b14,
     0x13011414,
     0x81141b14,
     0x1f1ff6f6,
     0x21001100,
     0x2100a600,
     0xcf1ff6ef,
     0xc915f612,
     0xd114f608,
     0x241ea8d5,
     0x90d51815,
     0x1f021515,
     0x80d51815,
     0xc9041502,
     0x240029ff,
     0x21015e00,
     0x91741a14,
     0xc9061408,
     0x241ea8d5,
     0x90d51815,
     0x1f021515,
     0x80d51815,
     0xc9041502,
     0x240029ff,
     0x21015e00,
     0x2301d4d8,
     0x21002800,
     0xd102f702,
     0xc903f702,
     0x230309d8,
     0x21002800,
     0x2eff8999,
     0x241f9e8a,
     0x908a988a,
     0xc900ea18,
     0x1f01f7f7,
     0x91541b0d,
     0xc901ed04,
     0x1f01eded,
     0x81541b0d,
     0x7900000e,
     0xc900ed08,
     0x1f00eded,
     0x81541b0d,
     0x7900000a,
     0xc913ff04,
     0x240080df,
     0x241f68ed,
     0x79000007,
     0x688bc30b,
     0x68cb840a,
     0x688cc409,
     0x241f84ed,
     0x79000002,
     0x241f6ced,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0x1f041919,
     0x79000025,
     0xd1000207,
     0x68ebe204,
     0x688c8303,
     0x1f001919,
     0x79000017,
     0x241f84ed,
     0x7f0000f5,
     0x241f90ed,
     0x90ed3895,
     0xc900f50a,
     0x0b08f5f5,
     0x61019506,
     0x0501f5f5,
     0x0908f5f5,
     0x1f00f5f5,
     0x80ed3895,
     0x21008200,
     0x241f64ed,
     0x7f0000e9,
     0x2eff0194,
     0x68f4e205,
     0x68948304,
     0x1f1cf6f6,
     0x1f001919,
     0x79000003,
     0x1f001919,
     0x1f1bf6f6,
     0x2e8a0012,
     0x2e8a1782,
     0x24000379,
     0x24008194,
     0x68948505,
     0x0b054514,
     0x61041403,
     0x24000279,
     0x79000001,
     0x69007903,
     0x241c4882,
     0x79000008,
     0x69017903,
     0x241c5082,
     0x79000005,
     0x69027903,
     0x241c5882,
     0x79000002,
     0x241c6082,
     0xc9001902,
     0x90827c9c,
     0x24000000,
     0x2f060999,
     0x2e8a0012,
     0x61201203,
     0x24004d41,
     0x2100a600,
     0x20d80000,
     0x24000000,
     0x2e860999,
     0xc901f726,
     0xd1041925,
     0xd1031904,
     0x2e8a0012,
     0x2e8a1782,
     0x79000003,
     0x2e8a8012,
     0x2e8a9782,
     0xd1031903,
     0x61201219,
     0x79000003,
     0x59201217,
     0x24000037,
     0x24000861,
     0x69009a07,
     0x90dc3c94,
     0x1094949b,
     0x10d4d4da,
     0x049ddaf5,
     0x0903f5f5,
     0x00f59cdb,
     0x82dbfe82,
     0x01209a9a,
     0x68ddda04,
     0x109d9dda,
     0x109c9cdb,
     0x79000003,
     0x0120dbdb,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0x15081919,
     0x109898d8,
     0x24000000,
     0x2f060999,
     0xd114ff02,
     0x21002800,
     0x2100d000,
     0xd101f716,
     0x241f68ed,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0xc913ff02,
     0x240080df,
     0x91541b0d,
     0xc901ed04,
     0x1f01eded,
     0x81541b0d,
     0x79000004,
     0xc900ed07,
     0x1f00eded,
     0x81541b0d,
     0x241f6ced,
     0x90ed3895,
     0x0101f5f5,
     0x80ed3895,
     0x1d01f7f7,
     0x240004df,
     0x79000070,
     0x24000000,
     0x2e860999,
     0xd1041942,
     0xd1031905,
     0x2e8a0012,
     0x2e8a1782,
     0x71201217,
     0x79000004,
     0x2e8a8012,
     0x2e8a9782,
     0x59201211,
     0x82dbfe82,
     0x01209a9a,
     0x68ddda04,
     0x109d9dda,
     0x109c9cdb,
     0x79000003,
     0x0120dbdb,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0xc9031903,
     0x2e8a0f82,
     0x79000004,
     0x2e8a9782,
     0x0520f220,
     0x79000002,
     0x10f2f220,
     0x5100200c,
     0x8edbde82,
     0x00209a9a,
     0x71042009,
     0x68ddda03,
     0x109d9dda,
     0x79000002,
     0x0104dada,
     0x689bda04,
     0x1d001919,
     0x1f065959,
     0x1f041919,
     0x24020494,
     0x90942095,
     0xc911f506,
     0x241f60ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x1f041919,
     0xc910f506,
     0x241f80ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x1f041919,
     0x91541b15,
     0xc9021503,
     0x241f7ced,
     0x79000003,
     0xc9031508,
     0x241f78ed,
     0x90ed3894,
     0x0101f4f4,
     0x80ed3894,
     0x24000c35,
     0x81541b35,
     0x1f041919,
     0x240004df,
     0xc9041904,
     0x1d1cf6f6,
     0x1d1bf6f6,
     0x21014a00,
     0x24000000,
     0x2f060999,
     0x24001800,
     0x2e860782,
     0x241f9af5,
     0x80f51882,
     0x90c43c94,
     0x24000095,
     0x050482d5,
     0x0902d5d5,
     0x1f01d5d5,
     0x80d43c95,
     0x6083c203,
     0x0483c295,
     0x21014200,
     0x0483c595,
     0x01049595,
     0x0485c2d5,
     0x00d59595,
     0x0b029595,
     0x0106c494,
     0x90941c55,
     0x70551503,
     0x10151555,
     0x80941c55,
     0x0102c494,
     0x80941cc2,
     0x0104dc94,
     0x90943c95,
     0x24000015,
     0xc9065903,
     0x13043535,
     0x01017575,
     0x80943c95,
     0x1d01f7f7,
     0xd1041903,
     0x1f03f7f7,
     0x240025ff,
     0x24028483,
     0x240001c2,
     0x24000082,
     0x80832082,
     0x1f16ffff,
     0xc900f703,
     0x24004e41,
     0x21025000,
     0x21002800,
     0xc914f60b,
     0xc911f60a,
     0xd112f609,
     0x241ea894,
     0x90941854,
     0x1f015454,
     0x80941854,
     0x241eac94,
     0x90943895,
     0x0101f5f5,
     0x80943895,
     0x1d12f6f6,
     0xd114f606,
     0x2301c385,
     0x91701a06,
     0x115f0606,
     0x81701a06,
     0x2301d085,
     0x24005006,
     0x81741a06,
     0x241e9884,
     0x90847882,
     0x81987a82,
     0x241ea084,
     0x90843884,
     0x00e4e2e2,
     0x0300e3e3,
     0x241e9884,
     0x80847882,
     0x241ea484,
     0x90843884,
     0x04e4e2e2,
     0x0700e3e3,
     0x81a87a82,
     0x2301c385,
     0x91701a06,
     0x13a00606,
     0x81701a06,
     0x2301d085,
     0x24001482,
     0x90821b00,
     0x13040000,
     0x80821b00,
     0x1f14f6f6,
     0x21004c00,
     0xcd15f68a,
     0x117f3636,
     0x11c05656,
     0x2301c385,
     0x91701a86,
     0x115f0606,
     0x81701a86,
     0x24005006,
     0x81741a06,
     0x2301d085,
     0x2eff8385,
     0x81987a85,
     0x81a87a85,
     0x24001482,
     0x90821b00,
     0x11fb0000,
     0x80821b00,
     0x21001500,
     0x1082828f,
     0x908f388b,
     0x0903cbce,
     0x0b05cece,
     0x010ccef5,
     0x0906f5e5,
     0x0904f5f5,
     0x00f5e5f5,
     0x10f5f5e0,
     0x91107a94,
     0x91a87a85,
     0x48f5e603,
     0x64f5e67f,
     0x74f4e57e,
     0x04f4e5f4,
     0x06f5e6f5,
     0x70f4e005,
     0x5901f504,
     0xcd13f679,
     0x241ef894,
     0x2101f700,
     0xc913f604,
     0x1f12f6f6,
     0x1f11f6f6,
     0x21020b00,
     0xc911f60a,
     0xd112f609,
     0x241ea894,
     0x90941854,
     0x1f015454,
     0x80941854,
     0x241eac94,
     0x90943895,
     0x0101f5f5,
     0x80943895,
     0x1f12f6f6,
     0x1f11f6f6,
     0x21020b00,
     0x24000000,
     0x2e850014,
     0xd700f4ff,
     0x1f00f5f5,
     0x2f050015,
     0x310a0002,
     0x0100e0e0,
     0x2e850014,
     0xc900f404,
     0x1d00f5f5,
     0x2f050015,
     0x7f0000f6,
     0x20850000,
     0x24000000,
     0x1d00f5f5,
     0x2f050015,
     0x20850000,
     0x241f98c0,
     0x90c01800,
     0xd101e003,
     0x1d17f6f6,
     0x79000002,
     0x1f17f6f6,
     0xd100f776,
     0xd100e019,
     0x79000017,
     0x1d00f7f7,
     0x1f1effff,
     0x241f99e0,
     0x90e01802,
     0x01010202,
     0x80e01802,
     0x1d1af6f6,
     0x1d19f6f6,
     0xd116f609,
     0x7900000d,
     0x1d00f7f7,
     0x241f70e0,
     0x79000007,
     0x241f99e3,
     0x24000002,
     0x80e31802,
     0x79000003,
     0x1d16f6f6,
     0x241f50e0,
     0x90e03882,
     0x0101e2e2,
     0x80e03882,
     0x21002800,
     0x241ef894,
     0x241eb8cd,
     0x241ed884,
     0xc917f603,
     0x913c1b00,
     0xd701e0ee,
     0x569484f9,
     0x0108cdcd,
     0x90cd7882,
     0x01088484,
     0x5682c2f9,
     0xc915f60c,
     0x241ee0d5,
     0x68d58408,
     0xd712f6f5,
     0x1f13f6f6,
     0x241ea8d5,
     0x90d51815,
     0x1d021515,
     0x80d51815,
     0x21019d00,
     0x1d13f6f6,
     0x21019d00,
     0x90847890,
     0x1f00f7f7,
     0x0491828f,
     0x09038f8f,
     0x008f90cf,
     0x1082828f,
     0x908f388b,
     0x0903cbce,
     0x0b05cece,
     0x2400008e,
     0x241f9482,
     0x908218c2,
     0x1f07f7f7,
     0x51644202,
     0x1d07f7f7,
     0xd114ff34,
     0x92cffe82,
     0xc917f604,
     0x913c1b00,
     0xc901e002,
     0x7f0000c8,
     0xd100e202,
     0x79000007,
     0x2eff0180,
     0x68e0e204,
     0x68808303,
     0x1f1af6f6,
     0x79000002,
     0x1f19f6f6,
     0xc915f604,
     0x91487a94,
     0x241eb080,
     0x80807894,
     0x24000861,
     0x31070003,
     0x2c2261fe,
     0x01046161,
     0x0120cfcf,
     0x01208e8e,
     0x1f01eded,
     0x2400002d,
     0x24000000,
     0x2f06098d,
     0xc917f604,
     0x913c1b00,
     0xc900e002,
     0x7f0000a4,
     0xd114ff15,
     0x68d18f04,
     0x1091918f,
     0x1d01eded,
     0x109090cf,
     0x92cffe82,
     0x24000861,
     0x31060003,
     0x2c2261fe,
     0x01046161,
     0x011ccfcf,
     0x011c8e8e,
     0x24001c2d,
     0x24000000,
     0x2f06098d,
     0xc917f604,
     0x913c1b00,
     0xc900e002,
     0x7f000091,
     0x21002800,
     0x1d00f7f7,
     0x2300d0d8,
     0x241f98c0,
     0x90c01800,
     0xd100e005,
     0x1d00f7f7,
     0x1f1effff,
     0x1f1ff6f6,
     0x21002800,
     0x24000000,
     0x2e86098d,
     0x241f98c2,
     0x90c21c02,
     0xd100e203,
     0x1d17f6f6,
     0x79000002,
     0x1f17f6f6,
     0xc915f60f,
     0x91741a15,
     0xd1041502,
     0x21002800,
     0x241eb095,
     0x90957894,
     0x91487a82,
     0x68f4e202,
     0x56f5e3fe,
     0x241ea8d5,
     0x90d51815,
     0xc9031504,
     0xc913f603,
     0x241eb880,
     0x80807882,
     0xc917f604,
     0x913c1b00,
     0xc900e002,
     0x7f00006c,
     0x916c3b80,
     0x0b01e0e0,
     0x71600006,
     0x241f8884,
     0x90843882,
     0x0101e2e2,
     0x80843882,
     0x2102b700,
     0x24028084,
     0x90842082,
     0xc913e209,
     0x240000e2,
     0x1f13e2e2,
     0x80842082,
     0x241f8c84,
     0x90843882,
     0x0101e2e2,
     0x80843882,
     0x2102b700,
     0x0d60004a,
     0x51004a2f,
     0x71204a02,
     0x2400204a,
     0xd114ff2d,
     0xc901ed08,
     0x68d18f05,
     0x1091918f,
     0x1d01eded,
     0x109090cf,
     0x21029200,
     0x01048f8f,
     0x1d01eded,
     0x048ece8a,
     0x49208a04,
     0x24000000,
     0x2f06098d,
     0x2102ba00,
     0x58cfd005,
     0x04d0cf02,
     0x0d200222,
     0x584a2202,
     0x1022224a,
     0x92cffe82,
     0x24000861,
     0x0b024a8b,
     0x308b0003,
     0x2c2261fe,
     0x01046161,
     0xc917f604,
     0x913c1b00,
     0xc900e002,
     0x7f000038,
     0x11034a14,
     0x51001406,
     0x51011404,
     0x2c21619e,
     0x01026161,
     0x51021402,
     0x2c20611e,
     0x004a2d2d,
     0x61202d03,
     0x1f01eded,
     0x05202d2d,
     0x004acfcf,
     0x004a8e8e,
     0x24000000,
     0x2f06098d,
     0x21002800,
     0x2300d0d8,
     0x1f1effff,
     0x1d00f7f7,
     0x21002800,
     0x048ece6a,
     0x51006a30,
     0x01046a0b,
     0x704a0b04,
     0x1f1ef6f6,
     0x704a6a02,
     0x104a4a6a,
     0x58cfd005,
     0x04d0cf02,
     0x0d200222,
     0x586a2202,
     0x1022226a,
     0xc917f605,
     0x913c1b00,
     0xc900e003,
     0x1f16f6f6,
     0x7f000013,
     0xd114ff3d,
     0x106a6a00,
     0x9ecfde02,
     0x24000861,
     0x0b026a8b,
     0x308b0003,
     0x2c2261fe,
     0x01046161,
     0x11036a14,
     0x51001406,
     0x51011404,
     0x2c21619e,
     0x01026161,
     0x51021402,
     0x2c20611e,
     0x006a8e8e,
     0x046a4a4a,
     0x006acfcf,
     0x006a2d2d,
     0x61202d07,
     0x05202d2d,
     0x68d18f04,
     0x1091918f,
     0x109090cf,
     0x2102e500,
     0x01048f8f,
     0xd11ef602,
     0x50ce8e05,
     0x1d1ef6f6,
     0x24000000,
     0x2f06098d,
     0x21002800,
     0xd107f703,
     0x59044a02,
     0x21002800,
     0x242c00df,
     0x1d1ff6f6,
     0x1d1ef6f6,
     0x24000000,
     0x2f06098d,
     0x1f02f7f7,
     0x51002d05,
     0x68d18f03,
     0x1091918f,
     0x2102f900,
     0x01048f8f,
     0x80cd188f,
     0x1d00f7f7,
     0xc917f60c,
     0x241f99e0,
     0x90e01804,
     0x4901e404,
     0x5100e408,
     0x241f54e0,
     0x7d0000e9,
     0x490fe403,
     0x241f58e0,
     0x7d0000e6,
     0x241f5ce0,
     0x7d0000e4,
     0x21002800,
     0x2300d0d8,
     0x10d8d898,
     0xc902f72b,
     0x241f00e4,
     0x24000000,
     0x2e86038d,
     0x0104cece,
     0x010ce4e8,
     0x90e83882,
     0x00cee2e2,
     0x80e83882,
     0x4940ce03,
     0x0120e4e8,
     0x21032500,
     0x497fce03,
     0x0124e4e8,
     0x21032500,
     0x49ffce03,
     0x0128e4e8,
     0x21032500,
     0x2401ffe3,
     0x48e3ce03,
     0x012ce4e8,
     0x21032500,
     0x2403ffe3,
     0x48e3ce03,
     0x0130e4e8,
     0x21032500,
     0x0134e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0xc91af604,
     0x0100e4e8,
     0x1d1af6f6,
     0x79000006,
     0xc919f604,
     0x0104e4e8,
     0x1d19f6f6,
     0x79000002,
     0x0108e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x1d02f7f7,
     0xc903f72c,
     0x241f9ae2,
     0x90e21889,
     0x241f00e4,
     0xc91cf603,
     0x0110e4e8,
     0x79000005,
     0xc91bf603,
     0x0114e4e8,
     0x79000002,
     0x0118e4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x011ce4e8,
     0x90e83882,
     0x0089e2e2,
     0x80e83882,
     0x49409a03,
     0x0138e4e8,
     0x21035900,
     0x497f8903,
     0x013ce4e8,
     0x21035900,
     0x49ff8903,
     0x0140e4e8,
     0x21035900,
     0x2401ffe3,
     0x48e38903,
     0x0144e4e8,
     0x21035900,
     0x2403ffe3,
     0x48e38903,
     0x0148e4e8,
     0x21035900,
     0x014ce4e8,
     0x90e83882,
     0x0101e2e2,
     0x80e83882,
     0x10e8e8e3,
     0x1d1cf6f6,
     0x1d1bf6f6,
     0x1d03f7f7,
     0x79000001,
     0x109898d8,
     0x20d80000 };

#endif
