/* vxbSdStorage.h - SD card driver header file for vxBus */

/*
 * Copyright (c) 2012, 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23Feb16,myt  fix some implement shortage based on spec (V7STO-568)
22may14,e_d  add UHS-I card support on imx6 AI board. (VXW6-80729)
07may14,e_d  add vxbus Gen2 model support. (US29286)
01dec12,e_d  created
*/

#ifndef __INCvxbSdStorageh
#define __INCvxbSdStorageh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* SD/MMC XBD service task priority */

#define SDMMC_XBD_SVC_TASK_PRI      50

/* forward declarations */

#define SDCARD_NAME                 "sdStorage"
#define SDHC_MAX_RW_SECTORS         2048

/* SSR define */

typedef struct
    {
    UINT8 ssrData[64];
    } SD_SSR;

/* SD card */

typedef struct sdInfo
    {
    UINT32          sdSpec;             /* SD spec version */
    UINT32          sdSec;              /* SD security verison */
    UINT32          auSize;             /* AU size */
    UINT32          eraseSize;          /* Erase size */
    UINT32          eraseTimeout;       /* Erase timeout */
    UINT32          eraseOffset;        /* Erase offset */       
    UINT32          uhsSpeed;           /* UHS-I speed */
    UINT32          uhsDevType;         /* UHS-I device type */
    UINT32          currLimit;          /* UHS-I current limit */
    UINT8           speedClass;         /* Class speed */
    BOOL            dat4Bit;            /* DAT bus width */
    BOOL            uhsSupport;         /* UHS-I card support */
    SD_CID          cid;                /* CID register */
    SDMMC_CSD       csd;                /* CSD register */
    SDMMC_SCR       scr;                /* SCR register */
    SD_SSR          ssr;                /* SSR register */  
    UINT16          dsr;                /* DSR register */
    } SD_INFO;

typedef struct sdCardCtrl
    {
    BLK_XBD_DEV     xbdDev;             /* XBD block device */
    VXB_DEV_HANDLE  pInst;              /* device handler   */ 
    void *          pInfo;              /* detailed information */
    BOOL            attached;           /* attached to host */
    BOOL            isWp;               /* write protected */
    UINT32          idx;                /* card index */
    BOOL            highCapacity;       /* high capacity > 2GB */
    UINT32          tranSpeed;          /* transfer speed */
    BOOL            highSpeed;          /* 50 MHz */
    UINT32          readBlkLen;         /* read block length */
    UINT32          writeBlkLen;        /* write block length */
    BOOL            hasDsr;             /* DSR implemented */
    UINT64          blkNum;             /* number of blocks */
    UINT64          capacity;           /* user data capacity */
    UINT32          permWp;             /* permanent write protection */
    UINT32          tmpWp;              /* temporary write protection */
    } SD_CARD_CTRL;

#ifdef __cplusplus
        }
#endif /* __cplusplus */
    
#endif /* __INCvxbSdStorageh */

