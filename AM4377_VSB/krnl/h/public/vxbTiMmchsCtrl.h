/* vxbTiMmchsCtrl.h - TI MMCHS driver header */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21oct16,y_y  add IRQ_ERR bin in interrupt register. (V7STO-697)
31mar16,myt  fix invalid use for EDMA on TI arch board (V7STO-610)
26feb16,zly  fix eMMC can not work well on am335x-bbb board(V7STO-269)
29jul15,swu  fix read coherence error. (V7PRO-2289)
13aug14,txu  add eMMC support. (US43877)
13may14,lxj  add vxbus Gen2 model support. (US29286)
18sep13,e_d  written.
*/

#ifndef __INCvxbTiMmchsCtrlh
#define __INCvxbTiMmchsCtrlh

#ifdef __cplusplus
extern "C" {
#endif

#include <vxbSdMmcLib.h>

/* MMCHS driver name */

#define MMCHS_NAME                              "tiMmchs"

/* MMCHS card monitor task name */

#define MMCHS_CARD_MON_TASK_NAME                "tiCardMon"

/* MMCHS card monitor task priority */

#define MMCHS_CARD_MON_TASK_PRI                 (100)

/* Command delay */

#define MMCHS_CMD_DELAY                         (10000)

/* reset timeout */

#define MMCHS_TIMEOUT                           (20000)

/* MMCHS card monitor task stack size */

#define MMCHS_CARD_MON_TASK_STACK               (16384)

/* MMCHS DMA mode supported */

#define MMCHS_DMA_MODE_DEFAULT                  (0)
#define MMCHS_DMA_MODE_SDMA                     (0)
#define MMCHS_DMA_MODE_PIO                      (1)

/* MMCHS DMA frame element size */

#define MMC_FRAME_ELEMENT_SIZE                  (4)

/* MMCHS GPIO extern define for cd and wp */

#ifndef GPIO_EXISTS_PIN
#define GPIO_EXISTS_PIN                         (0xfffe)
#endif /* GPIO_EXISTS_PIN for on-board eMMC chip */

#ifndef GPIO_NULL_PIN
#define GPIO_NULL_PIN                           (0xffff)
#endif /* GPIO_NULL_PIN */

/* seconds to wait for SD/MMC command or data done */

#define SDMMC_CMD_WAIT_IN_SECS                  (20)

/* omap35xx DMA MMC1 request */

#define OMAP35XX_DMA_MMC1_TX                    (61)
#define OMAP35XX_DMA_MMC1_RX                    (62)

/* MMCHS reference clock frequence */

#define MMCHS_REF_CLK_FREQ                      (96000000)
#define MMCHS_MAX_CLK_FREQ                      48000000

/* MMCHS register offset */

#define MMCHS_REG_BASE_OFFSET                   0x100

#define MMCHS_SYSCONFIG                         0x010
#define MMCHS_SYSSTATUS                         0x014
#define MMCHS_CON                               0x02C
#define MMCHS_BLK                               0x104
#define MMCHS_ARG                               0x108
#define MMCHS_CMD                               0x10C
#define MMCHS_RSP10                             0x110
#define MMCHS_RSP32                             0x114
#define MMCHS_RSP54                             0x118
#define MMCHS_RSP76                             0x11C
#define MMCHS_DATA                              0x120
#define MMCHS_PSTATE                            0x124
#define MMCHS_HCTL                              0x128
#define MMCHS_SYSCTL                            0x12C
#define MMCHS_STAT                              0x130
#define MMCHS_IE                                0x134
#define MMCHS_ISE                               0x138
#define MMCHS_AC12                              0x13C
#define MMCHS_CAPA                              0x140
#define MMCHS_CUR_CAPA                          0x148
#define MMCHS_REV                               0x1FC

#define MMCHS_SYSCONFIG_AUTOIDLE_MASK           (0x1 << 0)
#define MMCHS_SYSCONFIG_SOFTRESET_MASK          (0x1 << 1)
#define MMCHS_SYSCONFIG_ENAWAKEUP_MASK          (0x1 << 2)
#define MMCHS_SYSCONFIG_SIDLEMODE_MASK          (0x3 << 3)
#define MMCHS_SYSCONFIG_SIDLEMODE_SHIFT         (3)
#define MMCHS_SYSCONFIG_SIDLEMODE               (0x2)
#define MMCHS_SYSCONFIG_CLOCKACTIVITY_MASK      (0x3 << 8)
#define MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT     (8)
#define MMCHS_SYSCONFIG_CLOCKACTIVITY           (0x3)

#define MMCHS_SYSSTATUS_RESETDONE_MASK          (0x1 << 0)

#define MMCHS_SYSCTL_ICE_MASK                   (0x1 << 0)
#define MMCHS_SYSCTL_ICS_MASK                   (0x1 << 1)
#define MMCHS_SYSCTL_CEN_MASK                   (0x1 << 2)
#define MMCHS_SYSCTL_CLKD_MASK                  (0x3ff << 6)
#define MMCHS_SYSCTL_CLKD                       (0x258)
#define MMCHS_SYSCTL_CLKD_SHIFT                 (6)
#define MMCHS_SYSCTL_DTO_MASK                   (0xf << 16)
#define MMCHS_SYSCTL_DTO_SHIFT                  (16)
#define MMCHS_SYSCTL_DTO                        (0xe)
#define MMCHS_SYSCTL_SRA_MASK                   (0x1 << 24)
#define MMCHS_SYSCTL_SRC_MASK                   (0x1 << 25)

#define MMCHS_CAPA_VS30_MASK                    (0x1 << 25)
#define MMCHS_CAPA_VS18_MASK                    (0x1 << 26)

#define MMCHS_HCTL_SDVS_MASK                    (0x7 << 9)
#define MMCHS_HCTL_SDVS                         (0x6)
#define MMCHS_HCTL_SDVS_SHIFT                   (9)
#define MMCHS_HCTL_DTW_MASK                     (0x1 << 1)
#define MMCHS_HCTL_SDBP_MASK                    (0x1 << 8)
#define MMCHS_HCTL_IWE_MASK                     (0x1 << 24)

#define MMCHS_CON_OD_MASK                       (0x1 << 0)
#define MMCHS_CON_INIT_MASK                     (0x1 << 1)
#define MMCHS_CON_DVAL_MASK                     (0x3 << 9)
#define MMCHS_CON_DVAL                          (0x3)
#define MMCHS_CON_DVAL_SHIFT                    (9)
#define MMCHS_CON_DW8                           (0x1 << 5)

#define MMCHS_STAT_CC_MASK                      (0x1 << 0)
#define MMCHS_STAT_TC_MASK                      (0x1 << 1)

#define MMCHS_CMD0                              (0x0)

/* Block Attributes Register */

#define BLK_CNT_SHIFT                           (16)

/* SD System Control Register */

#define SYSCTL_ICE_SHIFT                        (0)
#define SYSCTL_ICE_MASK                         (0x1 << 0)
#define SYSCTL_ICS_SHIFT                        (1)
#define SYSCTL_ICS_MASK                         (0x1 << 1)
#define SYSCTL_CEN_SHIFT                        (2)
#define SYSCTL_CEN_MASK                         (0x1 << 2)
#define SYSCTL_CLKD_MASK                        (0x3ff << 6)
#define SYSCTL_CLKD_SHIFT                       (6)
#define SYSCTL_DTO_MAX                          (0xe)
#define SYSCTL_DTO_SHIFT                        (16)
#define SYSCTL_DTO_MASK                         (0xf << 16)

#define CLK_FREQ_400K                           (400000)
#define MMC_CLKD_MASK                           (0x3ff)

/* Transfter Type Register */

#define CMD_CMDINX(idx)                         (((idx) & 0x3f) << 24)
#define CMD_CMDTYP_NORMAL                       0x00000000
#define CMD_CMDTYP_SUSPEND                      0x00400000
#define CMD_CMDTYP_RESUME                       0x00800000
#define CMD_CMDTYP_ABORT                        0x00c00000
#define CMD_DATA_PRESENT                        0x00200000
#define CMD_CICEN                               0x00100000
#define CMD_CCCEN                               0x00080000
#define CMD_RSPTYP_NORSP                        0x00000000
#define CMD_RSPTYP_LEN136                       0x00010000
#define CMD_RSPTYP_LEN48                        0x00020000
#define CMD_RSPTYP_LEN48B                       0x00030000
#define CMD_MULTI_BLK                           0x00000020
#define CMD_DIR_READ                            0x00000010
#define CMD_AC12EN                              0x00000004
#define CMD_BCEN                                0x00000002
#define CMD_DMAEN                               0x00000001

/* Present State Register */

#define PSTATE_WPSPL                            0x00080000
#define PSTATE_CDPL                             0x00040000
#define PSTATE_CINS                             0x00010000
#define PSTATE_BREN                             0x00000800
#define PSTATE_BWEN                             0x00000400
#define PSTATE_DLA                              0x00000004
#define PSTATE_CDIHB                            0x00000002
#define PSTATE_CIHB                             0x00000001

/* Protocol Control Register */

#define HCTL_EMODE_MASK                         0x00000030
#define HCTL_EMODE_SHIFT                        (4)
#define HCTL_D3CD                               0x00000008
#define HCTL_DTW_1BIT                           0x00000000
#define HCTL_DTW_4BIT                           0x00000002
#define HCTL_DTW_8BIT                           0x00000004

/* System Control Register */

#define SYSCTL_INITA                            0x08000000
#define SYSCTL_RSTD                             0x04000000
#define SYSCTL_RSTC                             0x02000000
#define SYSCTL_RSTA                             0x01000000
#define SYSCTL_DTOCV_MASK                       0x000f0000
#define SYSCTL_DTOCV_SHIFT                      (16)
#define SYSCTL_DTOCV_MAX                        (14)
#define SYSCTL_CLK_MASK                         0x0000fff0
#define SYSCTL_FS_SHIFT                         (8)
#define SYSCTL_FS_MAX                           (256)
#define SYSCTL_FS_MIN                           (2)
#define SYSCTL_DVS_SHIFT                        (4)
#define SYSCTL_DVS_MAX                          (16)
#define SYSCTL_DVS_MIN                          (1)
#define SYSCTL_PEREN                            0x00000004
#define SYSCTL_HCKEN                            0x00000002
#define SYSCTL_IPGEN                            0x00000001

/* Interrupt Register */

#define IRQ_CERRE                               0x10000000
#define IRQ_BADAE                               0x20000000
#define IRQ_AC12E                               0x01000000
#define IRQ_DEBE                                0x00400000
#define IRQ_DCE                                 0x00200000
#define IRQ_DTOE                                0x00100000
#define IRQ_CIE                                 0x00080000
#define IRQ_CEBE                                0x00040000
#define IRQ_CCE                                 0x00020000
#define IRQ_CTOE                                0x00010000
#define IRQ_ERR                                 0x00008000
#define IRQ_CINT                                0x00000100
#define IRQ_BRR                                 0x00000020
#define IRQ_BWR                                 0x00000010
#define IRQ_BGE                                 0x00000004
#define IRQ_TC                                  0x00000002
#define IRQ_CC                                  0x00000001

#define IRQ_MISC_ERR                            (IRQ_CERRE | IRQ_BADAE)
#define IRQ_DATA_ERR                            (IRQ_DEBE | IRQ_DCE | IRQ_DTOE)
#define IRQ_CMD_ERR                             (IRQ_CIE | IRQ_CEBE | \
                                                IRQ_CCE | IRQ_CTOE)
#define IRQ_DATA                                (IRQ_DATA_ERR | IRQ_TC)
#define IRQ_CMD                                 (IRQ_CMD_ERR | IRQ_CC)

#define IRQ_ALL                                 (0xffffffff)
#define MMCHS_IE_EN_DEFUALT                     (0x307f0033)


/* Auto CMD12 Status Register */

#define AUTOC12ERR_CNIBAC12E                    0x00000080
#define AUTOC12ERR_AC12IE                       0x00000010
#define AUTOC12ERR_AC12CE                       0x00000008
#define AUTOC12ERR_AC12EBE                      0x00000004
#define AUTOC12ERR_AC12TOE                      0x00000002
#define AUTOC12ERR_AC12NE                       0x00000001

/* Host Controller Capabilities Register */

#define CAPA_VS18                               0x04000000
#define CAPA_VS30                               0x02000000
#define CAPA_VS33                               0x01000000
#define CAPA_DMAS                               0x00400000
#define CAPA_HSS                                0x00200000
#define CAPA_MBL_MASK                           0x00070000
#define CAPA_MBL_SHIFT                          (16)
#define CAPA_MBL_512                            0x00000000
#define CAPA_MBL_1024                           0x00010000
#define CAPA_MBL_2048                           0x00020000
#define CAPA_MBL_4096                           0x00030000

/* Host Controller Version Register */

#define REV_VVN_MASK                            0x0000ff00
#define REV_VVN_SHIFT                           (8)
#define REV_SVN_MASK                            0x000000ff

#define REV_VVN_10                              0x00000000
#define REV_VVN_20                              0x00000100
#define REV_SVN_10                              0x00000000
#define REV_SVN_20                              0x00000001

#define SDHC_MAX_RW_SECTORS                     0xFFFF

#ifdef _WRS_CONFIG_VXBUS_LEGACY
typedef BOOL (*TIMMCHS_FUNCPTR) (void);
#else
typedef BOOL (*TIMMCHS_FUNCPTR) (VXB_DEV_HANDLE);
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

typedef struct
    {
    UINT8   mid;            /* manufacturer ID */
    char    oid[2];         /* OEM/application ID */
    char    pnm[5];         /* product name */
    UINT8   prv;            /* product revision */
    UINT8   psn[4];         /* product serial number */
    UINT8   mdt1;           /* manufacturing date 1 */
    UINT8   mdt2;           /* manufacturing date 2 */
    UINT8   crc;            /* CRC7 checksum */
    } SDMMC_CID;

/* MMCHS PIO Description */

typedef struct mmchsPioDesc
    {
    char *          bufOri;
    char *          bufCur;
    UINT32          blkSize;
    } MMCHS_PIO_DESC;

/* MMCHS driver control */

typedef struct mmchsDrvCtrl
    {
    VXB_DEV_HANDLE          pDev;
    void *                  regBase;
    void *                  regHandle;
    void *                  pioHandle;
    UINT32                  dmaMode;

#ifdef _WRS_CONFIG_VXBUS_LEGACY
    VXB_DMA_RESOURCE_ID     dmaTxRes;
    VXB_DMA_RESOURCE_ID     dmaRxRes;
#else
    VXB_DMA_CHAN *          dmaTxRes;
    VXB_DMA_CHAN *          dmaRxRes;
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

    BOOL                    highSpeed;
    BOOL                    polling;
    SEM_ID                  devChange;
    SEM_ID                  cmdDone;
    SEM_ID                  dataDone;
    SEM_ID                  dmaDone;
    BOOL                    cardIns;
    BOOL                    attached;
    UINT32                  intSts;
    volatile UINT32         intMask;
#ifndef _WRS_CONFIG_VXBUS_LEGACY
    VXB_RESOURCE *          intRes;
    UINT32                  flags;
    UINT32                  cdGpioPin;
    UINT32                  wpGpioPin;
#endif /* _WRS_CONFIG_VXBUS_LEGACY */
    UINT32                  hcVer;
    struct vxbSdioInt *     pIntInfo;
    MMCHS_PIO_DESC          pioDesc;
    TIMMCHS_FUNCPTR         cardDetect;
    TIMMCHS_FUNCPTR         cardWpCheck;
    TIMMCHS_FUNCPTR         cardPowerOn;

    /* DMA operations */
    UINT32                  dmaRxEvt;
    UINT32                  dmaTxEvt;

    FUNCPTR                 dmaStart;
    FUNCPTR                 dmaAlloc;
    FUNCPTR                 dmaFree;
    SD_HOST_SPEC            sdHostSpec;   
    } MMCHS_DRV_CTRL;

/* register low level access routines */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#define MMCHS_BAR(p)         ((MMCHS_DRV_CTRL *)(p)->pDrvCtrl)->regBase
#define MMCHS_HANDLE(p)      ((MMCHS_DRV_CTRL *)(p)->pDrvCtrl)->regHandle
#else
#define MMCHS_BAR(p)         ((MMCHS_DRV_CTRL *)vxbDevSoftcGet(p))->regBase
#define MMCHS_HANDLE(p)      ((MMCHS_DRV_CTRL *)vxbDevSoftcGet(p))->regHandle
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

#define CSR_READ_4(pDev, addr)              \
        vxbRead32(MMCHS_HANDLE(pDev),       \
                  (UINT32 *)((char *)MMCHS_BAR(pDev) + addr))

#define CSR_WRITE_4(pDev, addr, data)       \
        vxbWrite32(MMCHS_HANDLE(pDev),      \
                   (UINT32 *)((char *)MMCHS_BAR(pDev) + addr), data)

#define CSR_SETBIT_4(pDev, offset, val)     \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val)     \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))

/* internal buffer data port access routines */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#define PIO_HANDLE(p)   ((MMCHS_DRV_CTRL *)(p)->pDrvCtrl)->pioHandle
#else
#define PIO_HANDLE(p)   ((MMCHS_DRV_CTRL *)vxbDevSoftcGet(p))->pioHandle
#endif /* _WRS_CONFIG_VXBUS_LEGACY */

#define PIO_READ_4(pDev, addr)              \
        vxbRead32(PIO_HANDLE(pDev),         \
                  (UINT32 *)((char *)MMCHS_BAR(pDev) + addr))

#define PIO_WRITE_4(pDev, addr, data)       \
        vxbWrite32(PIO_HANDLE(pDev),        \
                (UINT32 *)((char *)MMCHS_BAR(pDev) + addr), data)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCvxbTiMmchsCtrlh */
