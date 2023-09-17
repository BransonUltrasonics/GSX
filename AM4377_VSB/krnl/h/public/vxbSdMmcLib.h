/* vxbSdMmcLib.h - SD/SDIO bus header file for vxBus */

/*
 * Copyright (c) 2012-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
28jul16,j_x  use workFreq instead of maxTransSpeed for quark SD work frequency
01jul16,myt  add the card check for reading data (V7STO-634)
06may16,myt  change the type of cidValue to UINT8 (V7STO-626)
06may16,myt  fix some code/comment errors (V7STO-625)
31mar16,myt  fix invalid use for EDMA on TI arch board (V7STO-610) 
17mar16,myt  add support for SD/MMC on AMD Steppe Eagle board (US77313)
23Feb16,myt  fix some implement shortage based on spec (V7STO-568)
26jan16,myt  add support for direct bio mode (V7STO-572)
14jan16,myt  add the support for eMMC card on zynq7K board (V7STO-529)
20nvo15,myt  adapt to LP64 (V7STO-484)
07nov15,myt  recode for V7STO-488
04nov15,myt  add device parameter for zynq sdhc (V7STO-333)
02nov15,myt  add the support of assign name to different sd card
             for FDT bus device (V7STO-488)
26oct15,myt  add the support of reliable write for eMMC (V7STO-500)
26jun15,zly  ACMD42 is not included in SD card driver.(V7STO-401)
28nov14,myt  add support for Bay Trail (V7STO-260)
22may14,e_d  add UHS-I card support on imx6 AI board. (VXW6-80729)
14may14,lxj  add child device into host spec info field. (US29286)
07may14,e_d  add vxbus Gen2 model support. (US29286)
24sep13,e_d  add some advanced functionalities for SD/MMC. (WIND00437987)  
06aug12,e_d  created
*/

#ifndef __INCvxbSdLibh
#define __INCvxbSdLibh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include "dllLib.h"
#include <hwif/util/vxbDmaBufLib.h>

/* defines */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
typedef VXB_DEVICE_ID   VXB_DEV_HANDLE;
#define GET_DRVCTRL(x)  (x->pDrvCtrl)
#else
typedef VXB_DEV_ID      VXB_DEV_HANDLE;
BUSTYPE_DECL(vxbSdMmcBus);
#define VXB_BUSID_SDMMC     BUSTYPE_ID(vxbSdMmcBus)/* Nexus class device */
#define GET_DRVCTRL(x)      vxbDevSoftcGet(x)
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

extern BOOL mmcHrfsInclude;

#define MAX_HOST_SLOT          6
#define MAX_TARGET_DEV         8
#define SDMMC_COMMAND_TIMEOUT  1000
#define SDMMC_CMD_RETRY        10

#define SD_CARD_NAME           "sdStorage"
#define MMC_CARD_NAME          "mmcStorage"

/*
 * SD/MMC command index (a binary coded number between 0 and 63)
 *
 * The command set of the SD Memory Card system is divided into
 * several classes. Each class supports a set of card functionalities.
 * Class 0,2,4,5,7,8 are mandatory and shall be supported by all SD
 * memory cards. The other classes are optional. The supported Card
 * Command Classes (CCC) are coded as a parameter in the card specific
 * data (CSD) register of each card.
 */

/*
 * Card Command Classes (CCC)
 */

#define CCC_BASIC           (1<<0)  /* Basic */
#define CCC_RESERVED        (1<<1)  /* reserved commands */
#define CCC_BLOCK_READ      (1<<2)  /* Block read commands */
#define CCC_RESERVED2       (1<<3)  /* reserved commands */
#define CCC_BLOCK_WRITE     (1<<4)  /* Block write commands */
#define CCC_ERASE           (1<<5)  /* Ability to erase blocks */
#define CCC_WRITE_PROT      (1<<6)  /* Ability to write protect blocks */
#define CCC_LOCK_CARD       (1<<7)  /* Ability to lock down card */
#define CCC_APP_SPEC        (1<<8)  /* Application specific */
#define CCC_IO_MODE         (1<<9)  /* I/O mode */
#define CCC_SWITCH          (1<<10) /* High speed switch */
#define CCC_RESERVED3       (1<<11) /* reserved commands */

/* basic commands (class 0) */

#define SDMMC_CMD_GO_IDLE_STATE         0

#define MMC_CMD_SEND_OP_COND            1

#define SDMMC_CMD_ALL_SEND_CID          2

#define SD_CMD_SEND_RELATIVE_ADDR       3
#define MMC_CMD_SET_RELATIVE_ADDR       3

#define SDMMC_CMD_SET_DSR               4
#define SDIO_CMD_SEND_OPCOND            5

#define MMC_CMD_SWITCH                  6
#define SD_CMD_SWITCH_FUNC              6   /* defined in SD verion 1.10 */

#define SDMMC_CMD_SELECT_CARD           7

#define MMC_CMD_SEND_EXT_CSD            8
#define SD_CMD_SEND_IF_COND             8

#define SDMMC_CMD_SEND_CSD              9
#define SDMMC_CMD_SEND_CID              10
#define SD_CMD_SWITCH_VOLTAGE           11
#define SDMMC_CMD_STOP_TRANSMISSION     12
#define SDMMC_CMD_SEND_STATUS           13

#define MMC_CMD_BUSTEST_R               14

#define SDMMC_CMD_GO_INACTIVE_STATE     15

/* stream read commands (class 1) */

#define MMC_CMD_READ_DAT_UNTIL_STOP     11

/* block-oriented read commands (class 2) */

#define SDMMC_CMD_SET_BLOCKLEN          16
#define SDMMC_CMD_READ_SINGLE_BLOCK     17
#define SDMMC_CMD_READ_MULTI_BLOCK      18

#define SDMMC_CMD_SEND_TUNING           19

/* stream write commands (class 3) */

#define MMC_CMD_WRITE_DAT_UNTIL_STOP    20

/* block-oriented write commands (class 4) */

#define MMC_CMD_SET_BLOCK_COUNT         23

#define SDMMC_CMD_WRITE_SINGLE_BLOCK    24
#define SDMMC_CMD_WRITE_MULTI_BLOCK     25

#define MMC_CMD_PROGRAM_CID             26

#define SDMMC_CMD_PROGRAM_CSD           27

/* block-oriented write protection commands (class 6) */

#define SDMMC_CMD_SET_WRITE_PROT        28
#define SDMMC_CMD_CLR_WRITE_PROT        29
#define SDMMC_CMD_SEND_WRITE_PROT       30

/* erase commands (class 5) */

#define SD_CMD_ERASE_WR_BLK_START       32
#define SD_CMD_ERASE_WR_BLK_END         33

#define MMC_CMD_ERASE_GROUP_START       35
#define MMC_CMD_ERASE_GROUP_END         36

#define SDMMC_CMD_ERASE                 38

/* lock card commands (class 7) */

#define SDMMC_CMD_LOCK_UNLOCK           42

/* application-specific commands (class 8) */

#define SDMMC_CMD_APP_CMD               55
#define SDMMC_CMD_GEN_CMD               56

/* SD ACMD index */

#define SD_ACMD_SET_BUS_WIDTH           6
#define SD_ACMD_SD_STATUS               13
#define SD_ACMD_SEND_NUM_WR_BLOCKS      22
#define SD_ACMD_SEND_OP_COND            41
#define SD_ACMD_SET_CLR_CARD_DETECT     42
#define SD_ACMD_SEND_SCR                51

#define SD_ACMD6_SET_FUNC               0x80000000 
#define SD_ACMD6_GET_FUNC               0x00000000 
#define SD_ACMD6_GROUP_ACCESS_MODE      0
#define SD_ACMD6_GROUP_CMD_SYSTEM       1
#define SD_ACMD6_GROUP_DRV_STRENGTH     2
#define SD_ACMD6_GROUP_CURRENT_LIMIT    3

/* SDIO READ/WRITE data index */

#define SDIO_CMD_DIRECT_IO              52
#define SDIO_CMD_DIRECT_EXT             53
#define SDIO_DIRECT_IO_RW               (1 << 31)
#define SDIO_DIRECT_IO_FUNC(x)          ((x & 0x7) << 28)
#define SDIO_DIRECT_IO_RAW              (1 << 27)
#define SDIO_DIRECT_IO_REGADRS(x)       ((x & 0x1FFFF) << 9)
#define SDIO_DIRECT_IOEXT_BLK           (1 << 27)
#define SDIO_DIRECT_IOEXT_OP            (1 << 26)
#define SDIO_DIRECT_IO_STUFF            (1 << 8)
#define SDIO_DIRECTIO_RWOPS_RW          (1 << 0)
#define SDIO_DIRECTIO_RWOPS_RAW         (1 << 1)

/* SDIO RW data response flags */

#define SDIO_DIRECTIO_CRCERR            (1 << 7)
#define SDIO_DIRECTIO_ILLCMD            (1 << 6)
#define SDIO_DIRECTIO_ERR               (1 << 3)
#define SDIO_DIRECTIO_RFU               (1 << 2)
#define SDIO_DIRECTIO_FUNCERR           (1 << 1)
#define SDIO_DIRECTIO_OUTRANGE          (1 << 0)
#define SDIO_DIRECTIO_STATE_DIS         (0 << 4)
#define SDIO_DIRECTIO_STATE_CMD         (1 << 4)
#define SDIO_DIRECTIO_STATE_TRN         (2 << 4)
#define SDIO_DIRECTIO_STATE_RFU         (3 << 4)

#define SDIO_VERSION_CCCR               0x0
#define SDIO_SPECVER_CCCR               0x1
#define SDIO_IOEN_CCCR                  0x2
#define SDIO_IOREADY_CCCR               0x3
#define SDIO_INTEN_CCCR                 0x4
#define SDIO_INTPEND_CCCR               0x5
#define SDIO_IOABORT_CCCR               0x6
#define SDIO_BUSCTRL_CCCR               0x7
#define SDIO_CARDCAPA_CCCR              0x8
#define SDIO_CISPOINT1_CCCR             0x9
#define SDIO_CISPOINT2_CCCR             0xA
#define SDIO_CISPOINT3_CCCR             0xB
#define SDIO_BUSSUSPEND_CCCR            0xC
#define SDIO_FUNCSELEC_CCCR             0xD
#define SDIO_EXECFLAGS_CCCR             0xE
#define SDIO_READYFLAG_CCCR             0xF
#define SDIO_FN0BLKSIZE_CCCR            0x10
#define SDIO_POWERCTRL_CCCR             0x12
#define SDIO_HIGHSEED_CCCR              0x13

#define SDIO_FBR_BASE(x)                ((x) * 0x100)
#define SDIO_FBR_STD_IF                 0x00

#define SDIO_FBR_SUPPORTS_CSA           0x40    /* supports Code Storage Area */
#define SDIO_FBR_ENABLE_CSA             0x80    /* enable Code Storage Area */

#define SDIO_FBR_STD_IF_EXT             0x01

#define SDIO_FBR_POWER                  0x02

#define SDIO_FBR_POWER_SPS              0x01    /* Supports Power Selection */
#define SDIO_FBR_POWER_EPS              0x02    /* Enable (low) Power Selection */

#define SDIO_FBR_CIS                    0x09    /* CIS pointer (3 bytes) */


#define SDIO_FBR_CSA                    0x0C    /* CSA pointer (3 bytes) */
#define SDIO_FBR_CSA_DATA               0x0F
#define SDIO_FBR_BLKSIZE                0x10    /* block size (2 bytes) */

#define SDIO_CISTPL_MANFID              0x20
#define SDIO_OPREG_TIMEOUT              0x1000

/*
 * SD/MMC command response types
 * There are 5 types of responses for the SD/MMC Memory Card.
 * The SDIO Card supports additional response types named R4 and R5.
 */

#define SDMMC_CMD_RSP_LEN48     0x00000001
#define SDMMC_CMD_RSP_LEN136    0x00000002
#define SDMMC_CMD_RSP_CMDIDX    0x00000004
#define SDMMC_CMD_RSP_CRC       0x00000008
#define SDMMC_CMD_RSP_BUSY      0x00000010
#define SDMMC_CMD_AUTOCMD12     0x80000000

#define SDMMC_CMD_RSP_NONE      0

#define SDMMC_CMD_RSP_R1        (SDMMC_CMD_RSP_LEN48 | SDMMC_CMD_RSP_CMDIDX | \
                                 SDMMC_CMD_RSP_CRC)
#define SDMMC_CMD_RSP_R1B       (SDMMC_CMD_RSP_LEN48 | SDMMC_CMD_RSP_CMDIDX | \
                                 SDMMC_CMD_RSP_CRC | SDMMC_CMD_RSP_BUSY)
#define SDMMC_CMD_RSP_R2        (SDMMC_CMD_RSP_LEN136 | SDMMC_CMD_RSP_CRC)
#define SDMMC_CMD_RSP_R3        (SDMMC_CMD_RSP_LEN48)
#define SDMMC_CMD_RSP_R4        (SDMMC_CMD_RSP_LEN48)
#define SDMMC_CMD_RSP_R5        (SDMMC_CMD_RSP_LEN48 | SDMMC_CMD_RSP_CMDIDX | \
                                 SDMMC_CMD_RSP_CRC)
#define SDMMC_CMD_RSP_R6        (SDMMC_CMD_RSP_LEN48 | SDMMC_CMD_RSP_CMDIDX | \
                                 SDMMC_CMD_RSP_CRC)
#define SDMMC_CMD_RSP_R7        (SDMMC_CMD_RSP_LEN48 | SDMMC_CMD_RSP_CMDIDX | \
                                 SDMMC_CMD_RSP_CRC)
/* card status in R1 response */
#define R1_OUT_OF_RANGE          0x80000000
#define R1_ADDRESS_ERROR         0x40000000
#define R1_BLOCK_LEN_ERROR       0x20000000
#define R1_ERASE_SEQ_ERROR       0x10000000
#define R1_ERASE_PARAM           0x08000000
#define R1_WP_VIOLATION          0x04000000
#define R1_CARD_IS_LOCKED        0x02000000
#define R1_LOCK_UNLOCK_FAILED    0x01000000
#define R1_COM_CRC_ERROR         0x00800000
#define R1_ILLEGAL_COMMAND       0x00400000
#define R1_CARD_ECC_FAILED       0x00200000
#define R1_CC_ERROR              0x00100000
#define R1_ERROR                 0x00080000
#define R1_UNDERRUN              0x00040000
#define R1_OVERRUN               0x00020000
#define R1_CID_CSD_OVERWRITE     0x00010000
#define R1_WP_ERASE_SKIP         0x00008000
#define R1_CARD_ECC_DISABLED     0x00004000
#define R1_ERASE_RESET           0x00002000
#define R1_CURRENT_STATE(x)      ((x & 0x00001E00) >> 9)
#define R1_READY_FOR_DATA        0x00000100
#define R1_SWITCH_ERROR          0x00000080	 
#define R1_EXCEPTION_EVENT       0x00000040
#define R1_APP_CMD               0x00000020

#define CARD_ERROR_STATUS        (R1_OUT_OF_RANGE | R1_ADDRESS_ERROR | \
                                  R1_BLOCK_LEN_ERROR | R1_ERASE_SEQ_ERROR | \
                                  R1_ERASE_PARAM | R1_WP_VIOLATION | \
                                  R1_LOCK_UNLOCK_FAILED | R1_COM_CRC_ERROR | \
                                  R1_ILLEGAL_COMMAND | R1_CARD_ECC_FAILED | \
                                  R1_CC_ERROR | R1_ERROR | R1_CID_CSD_OVERWRITE | \
                                  R1_WP_ERASE_SKIP)

/* SD function switch */

#define UHS_SDR12_BUS_SPEED             0
#define HIGH_SPEED_BUS_SPEED            1
#define UHS_SDR25_BUS_SPEED             HIGH_SPEED_BUS_SPEED
#define UHS_SDR50_BUS_SPEED             2
#define UHS_SDR104_BUS_SPEED            3
#define UHS_DDR50_BUS_SPEED             4

#define SD_MODE_HIGH_SPEED              (1 << HIGH_SPEED_BUS_SPEED)
#define SD_MODE_UHS_SDR12               (1 << UHS_SDR12_BUS_SPEED)
#define SD_MODE_UHS_SDR25               (1 << UHS_SDR25_BUS_SPEED)
#define SD_MODE_UHS_SDR50               (1 << UHS_SDR50_BUS_SPEED)
#define SD_MODE_UHS_SDR104              (1 << UHS_SDR104_BUS_SPEED)
#define SD_MODE_UHS_DDR50               (1 << UHS_DDR50_BUS_SPEED)

#define SD_DRIVER_TYPE_B                0x01
#define SD_DRIVER_TYPE_A                0x02
#define SD_DRIVER_TYPE_C                0x04
#define SD_DRIVER_TYPE_D                0x08

#define SD_SET_CURRENT_LIMIT_200MA      0
#define SD_SET_CURRENT_LIMIT_400MA      1
#define SD_SET_CURRENT_LIMIT_600MA      2
#define SD_SET_CURRENT_LIMIT_800MA      3

#define SD_MAX_CURRENT_200MA            (1 << SD_SET_CURRENT_LIMIT_200MA)
#define SD_MAX_CURRENT_400MA            (1 << SD_SET_CURRENT_LIMIT_400MA)
#define SD_MAX_CURRENT_600MA            (1 << SD_SET_CURRENT_LIMIT_600MA)
#define SD_MAX_CURRENT_800MA            (1 << SD_SET_CURRENT_LIMIT_800MA)

#define SD_VOLTAGE_S18                  1
#define SD_VOLTAGE_P33                  2

#define SDHC_TUNING_BLOCK_PATTERN_LEN   64

/*
 * Card Registers
 *
 * Six registers are defined within the card interface:
 * OCR, CID, CSD, RCA, DSR and SCR. These can be accessed only
 * by corresponding commands. The OCR, CID, CSD and SCR registers
 * carry the card/content specific information, while the RCA and
 * DSR registers are configuration registers storing actual
 * configuration parameters. In order to enable future extension,
 * the card shall return 0 in the reserved bits of the registers.
 */

/* OCR */

#define OCD_VDD_VOL_LOW         (1 << 7)
#define OCR_VDD_VOL_27_28       (1 << 15)
#define OCR_VDD_VOL_28_29       (1 << 16)
#define OCR_VDD_VOL_29_30       (1 << 17)
#define OCR_VDD_VOL_30_31       (1 << 18)
#define OCR_VDD_VOL_31_32       (1 << 19)
#define OCR_VDD_VOL_32_33       (1 << 20)
#define OCR_VDD_VOL_33_34       (1 << 21)
#define OCR_VDD_VOL_34_35       (1 << 22)
#define OCR_VDD_VOL_35_36       (1 << 23)
#define OCR_VDD_VOL_S18A        (1 << 24)
#define OCR_VDD_VOL_MASK        0x01FFFFFF
#define OCR_CARD_CAP_STS        (1 << 30)
#define OCR_CARD_PWRUP_STS      (1 << 31)

#define MMC_OCR_VDD_VOL_16_19   0x00000080
#define MMC_OCR_VDD_VOL_27_36   0x00FF8000

#define OCR_MMC_SECTORMODE      (1 << 30)
#define CID_PRV_MAJOR(prv)      (((prv) & 0xF0) >> 4)
#define CID_PRV_MINOR(prv)      ((prv) & 0x0F)

#define CID_MDT_YEAR(mdt1, mdt2)    \
    (((((mdt1) & 0x0F) << 4) | (((mdt2) & 0xF0) >> 4 )) + 2000)
#define CID_MDT_MONTH(mdt2)     ((mdt2) & 0x0F)
#define CID_SIZE                16

/* CSD */

#define CSD_VER1                0
#define CSD_VER2                1
#define CSD_VER(ver)            ((ver) >> 6)

#define CSD_TRAN_SPD_NORMAL     0x32    /* 25 MHz */
#define CSD_TRAN_SPD_HIGH       0x5A    /* 50 MHz */
#define CSD_TRAN_SPD_UHS50      0x0B    /* UHS50 MHz */
#define CSD_TRAN_SPD_UHS104     0x2B    /* UHS104 MHz */

#define CSD_TRAN_SPD_25MHZ      25000000
#define CSD_TRAN_SPD_50MHZ      50000000
#define CSD_TRAN_SPD_UHS50MHZ   100000000
#define CSD_TRAN_SPD_UHS104MHZ  200000000

#define SCR_SD_BUS_WIDTH_1BIT   0x01
#define SCR_SD_BUS_WIDTH_4BIT   0x04
#define SCR_SD_SPEC3_MASK       0x80

#define MMC_CSD_SPEC_VERS(x)    (((x) >> 2) & 0x0F)
#define MMC_CSD_SPEC_40         4
#define SDMMC_CMD_ARG_RCA(rca)  (((UINT32)rca) << 16)

#define MMC_CSD_SPEC_VERS_0     0
#define MMC_CSD_SPEC_VERS_1     1
#define MMC_CSD_SPEC_VERS_2     2
#define MMC_CSD_SPEC_VERS_3     3
#define MMC_CSD_SPEC_VERS_4     4

#define CSD_COMMANDCLASS_MASK   0xFFF0

/*
 * EXT_CSD fields
 */

#define EXT_CSD_FLUSH_CACHE             32  /* W */
#define EXT_CSD_CACHE_CTRL              33  /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION  34  /* R/W */
#define EXT_CSD_DATA_SECTOR_SIZE        61  /* R */
#define EXT_CSD_SEC_BAD_BLK_MGMNT       134
#define EXT_CSD_ENH_SIZE_MULT           140
#define EXT_CSD_GP_SIZE_MULT            143 /* R/W */
#define EXT_CSD_PARTITIONS_SET          155
#define EXT_CSD_PARTITION_ATTRIBUTE     156 /* R/W */
#define EXT_CSD_MAX_ENH_SIZE_MULT       157
#define EXT_CSD_PARTITION_SUPPORT       160 /* RO */
#define EXT_CSD_HPI_MGMT                161 /* R/W */
#define EXT_CSD_RST_N_FUNCTION          162 /* R/W */
#define EXT_CSD_SANITIZE_START          165 /* W */
#define EXT_CSD_WR_REL_PARAM            166 /* RO */
#define EXT_CSD_WR_REL_SET              167 /* R/W */
#define EXT_CSD_RPMB_SIZE_MULT          168
#define EXT_CSD_FW_CONFIG               169
#define EXT_CSD_USER_WP                 171
#define EXT_CSD_BOOT_WP                 173 /* R/W */
#define EXT_CSD_ERASE_GROUP_DEF         175 /* R/W */
#define EXT_CSD_PART_CONFIG             179 /* R/W */
#define EXT_CSD_ERASED_MEM_CONT         181 /* RO */
#define EXT_CSD_BUS_WIDTH               183 /* R/W */
#define EXT_CSD_HS_TIMING               185 /* R/W */
#define EXT_CSD_POWER_CLASS             187 /* R/W */
#define EXT_CSD_REV                     192 /* RO */
#define EXT_CSD_STRUCTURE               194 /* RO */
#define EXT_CSD_DEVICE_TYPE             196 /* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME   198 /* RO */
#define EXT_CSD_PART_SWITCH_TIME        199 /* RO */
#define EXT_CSD_PWR_CL_52_195           200 /* RO */
#define EXT_CSD_PWR_CL_26_195           201 /* RO */
#define EXT_CSD_PWR_CL_52_360           202 /* RO */
#define EXT_CSD_PWR_CL_26_360           203 /* RO */
#define EXT_CSD_SEC_CNT                 212 /* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT             217 /* RO */
#define EXT_CSD_REL_WR_SEC_C            222 /* RO */
#define EXT_CSD_HC_WP_GRP_SIZE          221 /* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT      223 /* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE       224 /* RO */
#define EXT_CSD_BOOT_MULT               226 /* RO */
#define EXT_CSD_SEC_TRIM_MULT           229 /* RO */
#define EXT_CSD_SEC_ERASE_MULT          230 /* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT     231 /* RO */
#define EXT_CSD_TRIM_MULT               232 /* RO */
#define EXT_CSD_PWR_CL_200_195          236 /* RO */
#define EXT_CSD_PWR_CL_200_360          237 /* RO */
#define EXT_CSD_PWR_CL_DDR_52_195       238 /* RO */
#define EXT_CSD_PWR_CL_DDR_52_360       239 /* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME     247 /* RO */
#define EXT_CSD_GENERIC_CMD6_TIME       248 /* RO */
#define EXT_CSD_CACHE_SIZE              249 /* RO, 4 bytes */
#define EXT_CSD_TAG_UNIT_SIZE           498 /* RO */
#define EXT_CSD_DATA_TAG_SUPPORT        499 /* RO */
#define EXT_CSD_HPI_FEATURES            503 /* RO */

#define MMC_EXT_CSD_TIMEUNIT            10
#define MMC_EXT_CSD_HCERASETIMEOUT_UNIT 300

#define MMC_EXT_HPI_IMPLEMENTATION      0x2
#define MMC_EXT_HPI_SUPPORT             0x1
#define MMC_EXT_CSD_FEA_SEC_GB_CL_EN    (1 << 4)

#define MMC_EXT_CSD_PWR_8BIT_MASK       0xF0
#define MMC_EXT_CSD_PWR_4BIT_MASK       0x0F
#define MMC_EXT_CSD_PWR_8BIT_SHIFT      4
#define MMC_EXT_CSD_PWR_4BIT_SHIFT      0

#define MMC_EXT_CSD_BUSWIDTH_1BIT       0x0
#define MMC_EXT_CSD_BUSWIDTH_4BIT       0x1
#define MMC_EXT_CSD_BUSWIDTH_8BIT       0x2

#define MMC_EXT_CSD_HSTIME_ENABLE       0x1

#define MMC_EXT_CSD_REL_WRITE_ENABLE    0x1F
/* Mmc switch access modes */

#define MMC_SWITCH_MODE_CMD_SET         0x00    /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS        0x01    /* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS      0x02    /* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_WRITE_BYTE      0x03    /* Set target to value */

/* MMC earse parameter */

#define MMC_ERASE_NORMAL                0x0
#define MMC_ERASE_TRIM                  0x00000001
#define MMC_ERASE_DISCARD               0x00000003
#define MMC_SECURE_ERASE                0x80000000
#define MMC_SECURE_TRIMARG1             0x80000001
#define MMC_SECURE_TRIMARG2             0x80008000

/* command associated errors */

#define SDMMC_CMD_ERR_PROTOCOL          0x00000001
#define SDMMC_CMD_ERR_TIMEOUT           0x00000002
#define SDMMC_CMD_ERR_NF                0x00000003

/* data associated errors */

#define SDMMC_DATA_ERR_PROTOCOL         0x00000001
#define SDMMC_DATA_ERR_TIMEOUT          0x00000002

/* SD command argument defines */

/* CMD8: send interface condition */

#define SD_CMD8_ARG_VHS_27_36           1
#define SD_CMD8_ARG_VHS_LOW_VOLTAGE     2
#define SD_CMD8_ARG_VHS_SHIFT           8
#define SD_CMD8_ARG_CHK_PATTERN         0xAA

/* ACMD6: set bus width */

#define SD_ACMD6_ARG_1BIT_BUS           0
#define SD_ACMD6_ARG_4BIT_BUS           2

/* clock frequency */

#define SDMMC_CLK_FREQ_400KHZ           400000
#define SDMMC_CLK_FREQ_25MHZ            25000000
#define MMC_CLK_FREQ_26MHZ              26000000
#define SDMMC_CLK_FREQ_50MHZ            50000000
#define MMC_CLK_FREQ_52MHZ              52000000
#define MMC_CLK_FREQ_200MHZ             200000000

/* bus width */

#define SDMMC_BUS_WIDTH_1BIT            0
#define SDMMC_BUS_WIDTH_4BIT            1
#define SDMMC_BUS_WIDTH_8BIT            2
#define SDMMC_BUS_WIDTH_MAX             3

/* ACMD42: cnenect/disconnect 50KOhm pull-up resistor */

#define SD_ACMD42_ARG_CLR_CARD_DETECT        0
#define SD_ACMD42_ARG_SET_CARD_DETECT        1

/* SCR SD version */

#define SD_VERSION_100                  1
#define SD_VERSION_110                  2
#define SD_VERSION_200                  3
#define SD_VERSION_300                  4
#define SD_VERSION_UNKNOWN              0

#define SD_SSR_SIZE                     64

/* SD/MMC card status bits */

#define CARD_STS_CUR_STATE_MASK         0x00001e00
#define CARD_STS_CUR_STATE_SHIFT        9
#define CARD_STS_CUR_GET(x)             (((x) & 0x00001e00) >> 9)
#define CARD_STS_ERASE_SEQ_ERROR        0x10000000
#define CARD_STS_ERASE_PARAM            0x08000000
#define CARD_STS_READY_FOR_DATA         0x00000100

#define CARD_STS_IDLE                   0
#define CARD_STS_READY                  1
#define CARD_STS_IDENT                  2
#define CARD_STS_STBY                   3
#define CARD_STS_TRAN                   4
#define CARD_STS_DATA                   5
#define CARD_STS_RCV                    6
#define CARD_STS_PRG                    7
#define CARD_STS_DIS                    8

/* SD/MMC card max numbers */

#define SDMMC_CARD_MAX_NUM              32

/* SD/MMC block size is 512 bytes by default */

#define SDMMC_BLOCK_SIZE                512

/* various feature flags, the upper 8 bits are for SDHC controller type */

#define SDHC_PIO_NEED_DELAY                 0x1
#define SDHC_HW_SNOOP                       0x2
#define SDHC_FIFO_ENDIANESS_REVERSE         0x4
#define SDHC_HOST_VER_REVERSE               0x8
#define SDHC_MISS_CARD_INS_INT_WHEN_INIT    0x10
#define SDHC_SDCD_GPIO_DEBOUNCE             0x20
#define SDHC_INVERTED_WRITE_PROTECT         0x40
#define SDHC_MANUAL_SDMA_ADDRESS            0x80
#define HOST_SUPPORT_18V                    0x100

#define SDHC_HOST_CTRL_FREESCALE            0x01000000
#define SDHC_HOST_CTRL_CAVIUM               0x02000000
#define SDHC_HOST_CTRL_INTEL                0x03000000
#define SDHC_HOST_CTRL_APM                  0x04000000
#define SDHC_HOST_CTRL_IMX                  0x05000000

#define SDHC_HOST_CTRL_TYPE(val)            (val & 0xFF000000)

/* SD/SDIO CID define */

typedef struct
    {
    UINT8       mid;              /* manufacturer ID */
    char        oid[2];           /* OEM/application ID */
    char        pnm[5];           /* product name */
    UINT8       prv;              /* product revision */
    UINT8       psn[4];           /* product serial number */
    UINT8       mdt1;             /* manufacturing date 1 */
    UINT8       mdt2;             /* manufacturing date 2 */
    UINT8       crc;              /* CRC7 checksum */
    } SD_CID;

/* SD/SDIO CID define */

typedef struct
    {
    UINT8       mid;              /* manufacturer ID */
    UINT8       bga;              /* CARD/BGA */
    UINT8       oid;              /* OEM/application ID */
    char        pnm[6];           /* product name */
    UINT8       prv;              /* product revision */
    UINT8       psn[4];           /* product serial number */
    UINT8       mdt;              /* manufacturing date */
    UINT8       crc;              /* CRC7 checksum */
    } MMC_CID;

/* CSD is different with version 1.0 and version 2.0 */

typedef struct
    {
    UINT8       structure;         /* csd struct */
    UINT8       taac;              /* data read access-time */
    UINT8       nsac;              /* data read access-time in CLK */
    UINT8       tranSpeed;         /* data transfer rate */
    UINT16      commClass;         /* command class */
    UINT8       resvData0[4];      /* reserved data */
    UINT16      eraseSize;         /* erase size */
    UINT16      r2wFactor;         /* write speed factor */
    UINT16      fileFormat;        /* file format */
    } SDMMC_CSD;

/* SCR define */

typedef struct
    {
    UINT8       spec;              /* SCR structure & spec version */
    UINT8       config;            /* various configuration */
    UINT8       expConfig;         /* version 3.0 extern config */
    UINT8       rsvd[5];           /* reserved */
    } SDMMC_SCR;

/* SDIO function tuple link define */

typedef struct sdioFuncTuple
    {
    UINT8       code;
    UINT8       size;
    UINT8   *   pData;
    struct sdioFuncTuple * next;   /* next pointer */
    } SDIO_FUNC_TUPLE;

/* The data of SD cmd define */

typedef struct
    {
    void *      buffer;
    UINT32      blkNum;
    UINT32      blkSize;
    BOOL        isRead;
    } SDDATA;

/* SD command define */

typedef struct
    {
    UINT32      cmdIdx;
    UINT64      cmdArg;
    UINT32      rspType;
    UINT32      cmdRsp[4];
    UINT32      cmdErr;
    UINT32      dataErr;
    BOOL        hasData;
    SDDATA      cmdData;
    SEM_ID      dmaDone;
    } SD_CMD;

/* SD host controller function define */

typedef STATUS (*VXB_SD_CMDPREPARE_FUNC) (VXB_DEV_HANDLE, SD_CMD * pCmd);
typedef STATUS (*VXB_SD_CMDISSUE_FUNC) (VXB_DEV_HANDLE, SD_CMD * pCmd);
typedef STATUS (*VXB_SD_CLKSETUP_FUNC) (VXB_DEV_HANDLE, UINT32);
typedef STATUS (*VXB_SD_VDDSETUP_FUNC) (VXB_DEV_HANDLE, UINT32);
typedef STATUS (*VXB_SD_BUSWIDTHSETUP_FUNC) (VXB_DEV_HANDLE, UINT32);
typedef STATUS (*VXB_SD_BOARDTUNING_FUNC) (VXB_DEV_HANDLE, UINT32);  
typedef BOOL   (*VXB_SD_CARDWPCHECK_FUNC) (VXB_DEV_HANDLE);
typedef BOOL   (*VXB_SD_CARDINSERTSTS_FUNC) (VXB_DEV_HANDLE);
typedef STATUS (*VXB_SD_HOSTCTRLINIT_FUNC) (VXB_DEV_HANDLE);
typedef STATUS (*VXB_SD_ISR) (VXB_DEV_HANDLE);
typedef STATUS (*VXB_SD_RESUMESET_FUNC) (VXB_DEV_HANDLE, BOOL);
typedef STATUS (*VXB_SD_TUNINGSET_FUNC) (VXB_DEV_HANDLE);

/* SD host controller ops define */

typedef struct
    {
    VXB_SD_CMDPREPARE_FUNC     vxbSdCmdPrepare;     /* prepare command */
    VXB_SD_CLKSETUP_FUNC       vxbSdClkFreqSetup;   /* setup the clock frequency */
    VXB_SD_VDDSETUP_FUNC       vxbSdVddSetup;       /* sd power vdd setup routine */
    VXB_SD_BUSWIDTHSETUP_FUNC  vxbSdBusWidthSetup;  /* setup the bus width */
    VXB_SD_CARDWPCHECK_FUNC    vxbSdCardWpCheck;    /* check if card is write protected */
    VXB_SD_CARDINSERTSTS_FUNC  vxbSdCardInsertSts;  /* card insert status check */
    VXB_SD_HOSTCTRLINIT_FUNC   vxbSdHostCtrlInit;   /* spec host controller's initialization */
    VXB_SD_ISR                 vxbSdIsr;            /* set isr bit enable */
    VXB_SD_RESUMESET_FUNC      vxbSdResumeSet;      /* resume Set function */
    VXB_SD_BOARDTUNING_FUNC    vxbSdBoardTuning;    /* board tuning function */ 
    } SD_HOST_OPS;

typedef struct
    {
    VXB_SD_CLKSETUP_FUNC       vxbSdClkFreqSetup;   /* setup the clock frequency */
    VXB_SD_VDDSETUP_FUNC       vxbSdVddSetup;       /* sd power vdd setup routine */
    VXB_SD_BUSWIDTHSETUP_FUNC  vxbSdBusWidthSetup;  /* setup the bus width */
    VXB_SD_CARDWPCHECK_FUNC    vxbSdCardWpCheck;    /* check if card is write protected */
    VXB_SD_RESUMESET_FUNC      vxbSdResumeSet;      /* resume Set function */
    VXB_SD_TUNINGSET_FUNC      vxbSdTuningSet;      /* tuning set function */
    VXB_DEV_HANDLE             childDev;            /* child device of host controller */
    UINT32                     capbility;           /* host controller capbility */
    UINT32                     maxTranSpeed;        /* maximum transsion speed supported by the controller */
    UINT32                     workFreq;            /* target work frequency */
    UINT32                     unitNumber;          /* host control unit number */    
    UINT32                     busWidth;            /* bus Width */        
    UINT32                     directBio;           /* direct bio mode */
    UINT32                     cardCheck;           /* eMMC card check for read data */
    } SD_HOST_SPEC;

/* SDIO id define */

typedef struct
    {
    UINT16  sdioDevId;
    UINT16  sdioVenderId;
    } VXB_SDIO_ID;

/* Target hardware attribute define */

typedef struct
    {
    VXB_SDIO_ID   vxbSdioId;
    UINT8         version;
    UINT8         cardSupAttr;
    UINT16        rcaValue;
    UINT8         cidValue[CID_SIZE];
    UINT32        ocrValue;
    UINT32        voltage;
    UINT32        cmdErr;
    UINT32        dataErr;
    UINT32        funcNum;
    UINT32        blockSize;
    BOOL          isSpi;
    BOOL          isSdio;
    BOOL          isComboCard;
    BOOL          isMmc;
    void    *     pSpec;
    void    *     pHostSpec;
    SDIO_FUNC_TUPLE * pSdioFuncTuple;
    VXB_SD_CMDISSUE_FUNC   vxbSdCmdIssue;
    VXB_DEV_HANDLE pHostDev;
    }SD_HARDWARE; /* SD_HARDWARE */

/*
 * SDHC max slot num is set 6 see SD Host
 * Controller Simplified Specification c.3.3.
 */

typedef struct sdHostCtrl
    {
    VXB_DEV_HANDLE  pDev;
    SD_HOST_OPS     sdHostOps;
    SD_HOST_SPEC    sdHostSpec;
    UINT8           speedType;
    UINT8           slotNum;
    UINT8           slotTatalNum;
    BOOL            isResume;
    BOOL            isInsert;
    BOOL            polling;
    BOOL            attached;
    UINT32          curClkFreq;
    UINT32          vdd;
    UINT32          curBusWidth;
    UINT32          capbility;
    UINT32          dataTimeOutDelay;
    UINT32          dmaMode;
    UINT32          cmdErr;
    UINT32          dataErr;
    UINT32          version;
    UINT32          maxTranSpeed;
    UINT32          directBio;
    SEM_ID          devChange;
    SEM_ID          cmdDone;
    SEM_ID          dataDone;
    SEM_ID          pioReady;
    SEM_ID          hostDevSem;
    VXB_DEV_HANDLE  pChildDev;
    VXB_DMA_TAG_ID  sdHostDmaTag;       /* sata host controller dma map tag */
    VXB_DMA_TAG_ID  sdHostDmaParentTag; /* sata host dma map parent tag */
    VXB_DMA_MAP_ID  sdHostDmaMap;       /* sata pRD memory map */
    struct sdHostCtrl * pSdHostCtrl;
    } SD_HOST_CTRL;

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#define GET_HARDWAREINFO(x)    (x->pBusSpecificDevInfo)

struct vxbSdRegister
    {
    struct vxbDevRegInfo b;
    int             idListLen;
    VXB_SDIO_ID *   idList;
    };

#else
#define GET_HARDWAREINFO(x)    vxbDevIvarsGet (x)
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

#define SD_CMD_FORM(cmd, idx, arg, type, data)       \
    do                                               \
        {                                            \
        bzero ((char *)&cmd, sizeof(SD_CMD));        \
        cmd.cmdIdx  = (idx);                         \
        cmd.cmdArg  = (arg);                         \
        cmd.rspType = (type);                        \
        cmd.hasData = (data);                        \
        } while ((0));

#define SD_DATA_FORM(cmd, buf, num, size, read)     \
    do                                              \
        {                                           \
        cmd.cmdData.buffer = buf;                   \
        cmd.cmdData.blkNum = num;                   \
        cmd.cmdData.blkSize = size;                 \
        cmd.cmdData.isRead = read;                  \
        } while ((0));

/* forward declarations */

STATUS sdDeviceCreate (VXB_DEV_HANDLE, void *);
STATUS sdCmdGoIdleState (VXB_DEV_HANDLE);
STATUS sdMmcCmdSendOpCond (VXB_DEV_HANDLE, UINT32);
STATUS sdCmdAllSendCid (VXB_DEV_HANDLE, void *);
STATUS sdCmdSendRelativeAddr (VXB_DEV_HANDLE);
STATUS sdCmdSetDsr (VXB_DEV_HANDLE, UINT32);
STATUS sdioCmdIoSendOpCond (VXB_DEV_HANDLE, UINT32);
STATUS sdCmdSelectCard (VXB_DEV_HANDLE);
STATUS sdCmdDeselectCard (VXB_DEV_HANDLE);
STATUS sdCmdSendIfCond (VXB_DEV_HANDLE, UINT32);
STATUS mmcSwitch (VXB_DEV_HANDLE, UINT8, UINT8, UINT8);
STATUS mmcCmdSeEraseGrpStart (VXB_DEV_HANDLE, UINT32);
STATUS mmcCmdSeEraseGrpEnd (VXB_DEV_HANDLE, UINT32);
STATUS mmcCmdSetEraseGrp (VXB_DEV_HANDLE, UINT);
STATUS mmcCmdReadExtCsd (VXB_DEV_HANDLE, UINT8 *);
STATUS mmcCmdSetBlockNum (VXB_DEV_HANDLE, UINT32);
STATUS mmcSendHpi (VXB_DEV_HANDLE, UINT32, UINT32 *);
STATUS sdACmdSwitchFunc (VXB_DEV_HANDLE, void *, UINT32, int, int, int);
STATUS sdACmdSendWriteBlkNum (VXB_DEV_HANDLE, UINT32 *);
STATUS sdACmdSendSsr (VXB_DEV_HANDLE, UINT8 *);
STATUS sdCmdSwitchVoltage (VXB_DEV_HANDLE);
STATUS sdSendTuning (VXB_DEV_HANDLE);
STATUS sdCmdSendCsd (VXB_DEV_HANDLE, SDMMC_CSD *);
STATUS sdCmdSendCid (VXB_DEV_HANDLE, void *);
STATUS sdCmdStopTransmission (VXB_DEV_HANDLE);
STATUS sdCmdSendStatus (VXB_DEV_HANDLE, UINT32 *);
STATUS sdCmdGoInactiveState (VXB_DEV_HANDLE);
STATUS sdCmdSetBlockLen (VXB_DEV_HANDLE, UINT32);
STATUS sdCmdReadSingleBlock (VXB_DEV_HANDLE, void *, UINT32, BOOL);
STATUS sdCmdReadMultipleBlock (VXB_DEV_HANDLE, void *, UINT32, UINT32, BOOL);
STATUS sdCmdReadBlock (VXB_DEV_HANDLE, void *, UINT64, UINT32, BOOL);
STATUS sdCmdWriteSingleBlock (VXB_DEV_HANDLE, void *, UINT32, BOOL);
STATUS sdCmdWriteMultipleBlock (VXB_DEV_HANDLE, void *, UINT32, UINT32, BOOL);
STATUS sdCmdWriteBlock (VXB_DEV_HANDLE, void *, UINT64, UINT32, BOOL);
STATUS sdCmdAppCmd (VXB_DEV_HANDLE);
STATUS sdACmdSetBusWidth (VXB_DEV_HANDLE, UINT32);
STATUS sdACmdSendOpCond (VXB_DEV_HANDLE, UINT32, UINT32);
STATUS sdACmdClrCardDetect (VXB_DEV_HANDLE, UINT32);
STATUS sdACmdSendScr (VXB_DEV_HANDLE, SDMMC_SCR *);
STATUS sdMmcCmdSetRelativeAddr (VXB_DEV_HANDLE);
STATUS sdioCmdIoRwDirect (VXB_DEV_HANDLE, UINT32, UINT32, UINT32, UINT8 *);
STATUS sdioCmdIoRwExtend (VXB_DEV_HANDLE, UINT32, UINT32, UINT32, SDDATA *);
STATUS sdCmdIssue (VXB_DEV_HANDLE, SD_CMD *);
STATUS vxbSdioReadCis(VXB_DEV_HANDLE);
STATUS vxbSdioSetFunc(VXB_DEV_HANDLE, UINT8, BOOL);

#ifdef _WRS_CONFIG_VXBUS_LEGACY
void sdBusAnnounceDevices (VXB_DEV_HANDLE pDev, void * pCookie);
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#ifdef _WRS_CONFIG_VXBUS
#include <hwif/methods/vxbSdMmcMethod.h>
#endif

#endif /* __INCvxbSdLibh */

