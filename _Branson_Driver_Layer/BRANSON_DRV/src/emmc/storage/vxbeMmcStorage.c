/* vxbMmcStorage.c - MMC driver file for vxBus */

/*
 * Copyright (c) 2012 - 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
13aug18,r_r  create, add eMMC support. Driver adapted from Windriver SD Card driver
*/

/*
DESCRIPTION
This module implements a driver for EMMC card. This driver checks the device name 
to probe driver. If success, the driver will identify this device and mounts the
device to xbd layer.

\ie
SEE ALSO: vxBus
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <ioLib.h>
#include <string.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <cacheLib.h>

#include <hwif/vxBus.h>
#include <subsys/timer/vxbTimerLib.h>

#include <vxbeMmcLib.h>
#include <emmcXbd.h>

#include <vxbeMmcStorage.h>

/* defines */

#ifdef __GNUC__
#define FUNCTION_NOT_USED __attribute__ ((unused))
#else
#define FUNCTION_NOT_USED
#endif /* __GNUC__ */

#ifdef  MMC_CARD_DBG_ON

#ifdef  LOCAL
#undef  LOCAL
#define LOCAL
#endif

#define MMC_CARD_DBG_INIT           0x00000001
#define MMC_CARD_DBG_RW             0x00000002
#define MMC_CARD_DBG_SET            0x00000004
#define MMC_CARD_DBG_ALL            0xffffffff
#define MMC_CARD_DBG_OFF            0x00000000

LOCAL UINT32 emmcCardDbgMask = MMC_CARD_DBG_OFF;

IMPORT FUNCPTR _func_logMsg;

#define MMC_CARD_DBG(mask, string, a, b, c, d, e, f)            \
    if ((emmcCardDbgMask & mask) || (mask == MMC_CARD_DBG_ALL))  \
        if (_func_logMsg != NULL)                               \
           (* _func_logMsg)(string, a, b, c, d, e, f)
#else
#   define MMC_CARD_DBG(mask, string, a, b, c, d, e, f)
#endif  /* MMC_DBG_ON */

typedef struct 
{
	 UINT32 LifeTimeEstA;
     UINT32 LifeTimeEstB;
     UINT32 PreEOLInfo;
 } EST_LIFE;
   
   EST_LIFE Dvr_EmmcEstLife;
   
typedef struct 
{
    UINT32 PartSupport;     // 160
	UINT32 MaxEnhSize2;     // 159
	UINT32 MaxEnhSize1;     // 158
	UINT32 MaxEnhSize0;     // 157
	UINT32 EnhAttrGPP;      // 156
	UINT32 ParSetComplete;  // 155
	UINT32 GPparSzMult42;   // 154
	UINT32 GPparSzMult41;   // 153
	UINT32 GPparSzMult40;   // 152
	UINT32 GPparSzMult32;   // 151
	UINT32 GPparSzMult31;   // 150
	UINT32 GPparSzMult30;   // 149
	UINT32 GPparSzMult22;   // 148
	UINT32 GPparSzMult21;   // 147
	UINT32 GPparSzMult20;   // 146
	UINT32 GPparSzMult12;   // 145
	UINT32 GPparSzMult11;   // 144
	UINT32 GPparSzMult10;   // 143
	UINT32 EnhSzMult2;      // 142
	UINT32 EnhSzMult1;      // 141
	UINT32 EnhSzMult0;      // 140
	UINT32 EnhStartAddr3;   // 139     
	UINT32 EnhStartAddr2;   // 138     
	UINT32 EnhStartAddr1;   // 137     
	UINT32 EnhStartAddr0;   // 136 
  } pSLC;
  
  pSLC Dvr_pSLC;
   
LOCAL STATUS emmcStorageBlkRead (void *, XBD_DATA *);
LOCAL STATUS emmcStorageBlkWrite (void *, XBD_DATA *);
LOCAL STATUS emmcStorageBlkRelWrite (void *, XBD_DATA * );
LOCAL STATUS emmcIdentify (VXB_DEV_HANDLE);
LOCAL STATUS emmcStatusWaitReadyForData(VXB_DEV_HANDLE);
LOCAL STATUS emmcStorageInstUnlink (VXB_DEV_HANDLE);
LOCAL int emmcStorageIoctl (void *, int, void *);
LOCAL STATUS emmcExtCsdDecode (VXB_DEV_HANDLE);
LOCAL STATUS emmcCacheFlush (VXB_DEV_HANDLE);
LOCAL STATUS emmcErase (VXB_DEV_HANDLE, SECTOR_RANGE *);
#ifdef NEED_TO_COMPILE
LOCAL STATUS emmcExtCsdSetPowClass (VXB_DEV_HANDLE) FUNCTION_NOT_USED;
LOCAL STATUS emmcCacheEn (VXB_DEV_HANDLE, UINT8) FUNCTION_NOT_USED;
LOCAL STATUS emmcSecErase (VXB_DEV_HANDLE, SECTOR_RANGE *) FUNCTION_NOT_USED;
LOCAL STATUS emmcHpiInterrput (VXB_DEV_HANDLE) FUNCTION_NOT_USED;
#endif
LOCAL STATUS emmcCardIdxAlloc (MMC_CARD_CTRL *);
LOCAL void emmcCardIdxFree (MMC_CARD_CTRL *);

LOCAL STATUS emmcStorageAttach (VXB_DEV_HANDLE);
LOCAL STATUS emmcStoragDevProbe (VXB_DEV_HANDLE);
LOCAL STATUS emmcStorageShutdown (VXB_DEV_HANDLE);

/* locals */

LOCAL VXB_DRV_METHOD vxbeMmcStorage_methods[] =
    {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), emmcStoragDevProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), emmcStorageAttach },
    { VXB_DEVMETHOD_CALL(vxbDevShutdown), emmcStorageShutdown },
    { VXB_DEVMETHOD_CALL(vxbDevDetach), emmcStorageInstUnlink },
    VXB_DEVMETHOD_END
    };

/* ns16550 openfirmware driver */

VXB_DRV	vxbeMmcStorageDrv =
    {
    { NULL } ,
	EMMC_CARD_NAME,			/* Name */
    "EMMC Card driver",		/* Description */
    VXB_BUSID_SDMMC,		/* Class */
    0,						/* Flags */
    0,						/* Reference count */
    vxbeMmcStorage_methods	/* Method table */
    };

VXB_DRV_DEF(vxbeMmcStorageDrv)

/* SD/MMC card dev index */

LOCAL UINT32 emmcDevIdx;

/*
 * The following two tables defines the maximum data transfer rate
 * per one date line - TRAN_SPEED
 */

LOCAL UINT32 emmcCsdTranSpdfUnit[] = {
    10000, 100000, 1000000, 10000000
};

LOCAL UINT32 emmcCsdTranSpdTime[] = {
     0, 10, 12, 13, 15, 20, 26, 30,
    35, 40, 45, 52, 55, 60, 70, 80,
};

#ifndef _WRS_CONFIG_SMP
IMPORT volatile UINT64 vxAbsTicks;
#else
IMPORT UINT64 vxAbsTicksGet (void);
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* emmcStoragDevProbe - vxbus probe function
*
* This function is called by vxBus to probe device.
*
* RETURNS: TRUE if probe passes and assumed a valid SD memory card
* (or compatible) device. FALSE otherwise.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStoragDevProbe
    (
    VXB_DEV_HANDLE pDev
    )
    {
	STATUS rc;
	rc = vxbDevMatch (pDev);
	if(rc == ERROR)
	{
		MMC_CARD_DBG(MMC_CARD_DBG_INIT, 
		                     "sdStorageAttach() -  malloc MMC_CARD_CTRL fault\n",
		                     0, 0, 0, 0, 0, 0);
	}
    return rc;
    }

/*******************************************************************************
*
* emmcStorageAttach - VxBus instInit handler
*
* This function implements the VxBus instInit handler for MMC card
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageAttach
    (
    VXB_DEV_HANDLE pDev
    )
    {
    STATUS rc;
    MMC_CARD_CTRL * pDrvCtrl;
    pDrvCtrl = (MMC_CARD_CTRL *) vxbMemAlloc (sizeof (MMC_CARD_CTRL));

    VXB_DECODE_EXT_CSD	*pDecodeEcsd;

    if (pDrvCtrl == NULL)
        {
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, 
                     "sdStorageAttach() -  malloc MMC_CARD_CTRL fault\n",
                     0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    vxbDevSoftcSet(pDev, pDrvCtrl);


    /* save instance ID */

    pDrvCtrl->pInst = pDev;
    
    /* Register driver instance and decode Ext CSD routine with the library */
    pDecodeEcsd = (VXB_DECODE_EXT_CSD *) vxbMemAlloc (sizeof (VXB_DECODE_EXT_CSD));
    pDecodeEcsd->decodeExtCSD = emmcExtCsdDecode;	
    registerDecodeEcsd(pDev, pDecodeEcsd);
	
    //kprintf("EMMC identify\n");
    rc = emmcIdentify(pDev);

    return (rc);
    }

/*******************************************************************************
*
* emmcIdentify - identify MMC card in the system
*
* This routine implements the card identify process according to the MMC spec.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcIdentify
    (
    VXB_DEV_HANDLE pDev
    )
    {
    STATUS rc;
    UINT8 regValue = 0;
    UINT32 csize, csizeMulti;
    UINT64 blkNum;
    SD_HARDWARE * pSdHardWare;
    MMC_CARD_CTRL * pDrvCtrl;
    SD_HOST_SPEC * pHostSpec;
    MMC_INFO *pMmcInfo;
    device_t xbdSts;
    EMMC_XBD_PARAMS emmcXbdParams;
 
/* Check initialization start */
    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL(pDev);
    if (pDrvCtrl == NULL)
    {
        return ERROR;
    }

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
    {
        return ERROR;
    }

    pHostSpec = (SD_HOST_SPEC *)(pSdHardWare->pHostSpec);
    if (pHostSpec == NULL)
    {
        return ERROR;
    }

    pDrvCtrl->pInfo = vxbMemAlloc(sizeof(MMC_INFO));
    if (pDrvCtrl->pInfo == NULL)
    {
        goto err;
    }
/* Check initialization end */
    
    bzero ((char *)(pDrvCtrl->pInfo), sizeof(MMC_INFO));

    pMmcInfo = (MMC_INFO *)(pDrvCtrl->pInfo);
    bcopy((char *)(&pSdHardWare->cidValue[0]), (char *)(&pMmcInfo->cid), CID_SIZE);
    
    vxbUsDelay(1);

    /* CMD2, Retrieve card identification register */
    {
		if ((pSdHardWare->ocrValue & OCR_MMC_SECTORMODE) != 0x0)
			pDrvCtrl->highCapacity = TRUE;
		else
			pDrvCtrl->highCapacity = FALSE;
		
			
		(void)emmcCmdAllSendCid(pDev, (void *) (&pSdHardWare->cidValue[0]));
		MMC_CARD_DBG(MMC_CARD_DBG_INIT, 
		                 "emmcIdentify() -  *pSdHardWare->cidValue = 0x%x\n",
						 *pSdHardWare->cidValue, 0, 0, 0, 0, 0);
    }
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, 
                 "emmcIdentify() -  pDrvCtrl->highCapacity = 0x%x\n",
                 pDrvCtrl->highCapacity, 0, 0, 0, 0, 0);
    
    vxbUsDelay(1);
    
    /* CMD3: set card's RCA */
    {

		pSdHardWare->rcaValue = 2U;//(UINT16)(MMC_RCA_BASE + pDrvCtrl->idx);
		MMC_CARD_DBG(MMC_CARD_DBG_INIT, 
					 "emmcIdentify() -  pDrvCtrl->idx = 0x%x\n",
					 pDrvCtrl->idx, 0, 0, 0, 0, 0);
		
		rc = emmcCmdSetRelativeAddr (pDev);
		if (rc == ERROR)
			{
			MMC_CARD_DBG(MMC_CARD_DBG_INIT,
						 "emmcIdentify() -  sdMmcCmdSetRelativeAddr fault\n",
						 0, 0, 0, 0, 0, 0);
			goto err;
			}
    }
    
    vxbUsDelay(1);

    /* CMD9: request card to send CSD */
    rc = emmcCmdSendCsd (pDev, &(pMmcInfo->csd));
    if (rc == ERROR)
        {
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  sdCmdSendCsd fault\n",
                     0, 0, 0, 0, 0, 0);
        goto err;
        }
    else
        {
        pMmcInfo->csd.commClass = (UINT16)be16toh(pMmcInfo->csd.commClass);
        pMmcInfo->csd.eraseSize = (UINT16)be16toh(pMmcInfo->csd.eraseSize);
        pMmcInfo->csd.r2wFactor = (UINT16)be16toh(pMmcInfo->csd.r2wFactor);
        pMmcInfo->csd.fileFormat = (UINT16)be16toh(pMmcInfo->csd.fileFormat);
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  commClass = 0x%x\n"
                     "eraseSize = 0x%x\n"
                     "r2wFactor = 0x%x\n"
                     "fileFormat = 0x%x\n"
        			 "pMmcInfo->csd.taac = 0x%x"
        			 "pMmcInfo->csd.nsac= 0x%x",
                     pMmcInfo->csd.commClass, pMmcInfo->csd.eraseSize,
                     pMmcInfo->csd.r2wFactor, pMmcInfo->csd.fileFormat, pMmcInfo->csd.taac, pMmcInfo->csd.nsac);
        
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "tranSpeed = 0x%x\n",
                     pMmcInfo->csd.tranSpeed,0,0,0,0,0);

        regValue = (UINT8)(MMC_CSD_SPEC_VERS(pMmcInfo->csd.structure));
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmc csd spec version = %d \n",
                     regValue, 0, 0, 0, 0, 0);

        if (regValue >= MMC_CSD_SPEC_40)
            {
            pDrvCtrl->highSpeed = TRUE; 
            pDrvCtrl->isExendCsd = TRUE;
            MMC_CARD_DBG(MMC_CARD_DBG_INIT,
            		"MMC_CSD_SPEC_40\n",
                                 0, 0, 0, 0, 0, 0);
            }
        else
            {
            pDrvCtrl->highSpeed = FALSE;
            pDrvCtrl->isExendCsd = FALSE;
            }

        /* write protect model */
        if ((pMmcInfo->csd.fileFormat >> 12) & 0x1)
            pDrvCtrl->tmpWp = TRUE;
        if ((pMmcInfo->csd.fileFormat >> 13) & 0x1)
            pDrvCtrl->permWp = TRUE; 
        }
    
    vxbUsDelay(1);

    /* CMD7: select one card and put it into transfer state */
    {
		rc = emmcCmdSelectCard (pDev);
		if (rc == ERROR)
		{
			MMC_CARD_DBG(MMC_CARD_DBG_INIT,
									"GOTO error\n",
												 0, 0, 0, 0, 0, 0);
			goto err;
		}
    }
    
    vxbUsDelay(100);
    
    /* CMD 8: request card to send its ext csd */

    if (regValue >= MMC_CSD_SPEC_40)
        {
        rc = emmcCmdReadExtCsd (pDev, pMmcInfo->extCsd);
        if (rc == ERROR)
        {
        	MMC_CARD_DBG(MMC_CARD_DBG_INIT,
        	    	                		"MMC version is < 2\n",
        	    	                                     0, 0, 0, 0, 0, 0);
            goto err;
        }
        (void)emmcExtCsdDecode (pDev);
        }
    vxbMsDelay(100);

	MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  pSLC Conversion\n",
				 0, 0, 0, 0, 0, 0);

    Dvr_pSLC.EnhSzMult0 = pMmcInfo->extCsd[EXT_CSD_ENH_SIZE_MULT];      // 140
    Dvr_pSLC.EnhSzMult1 = pMmcInfo->extCsd[EXT_CSD_ENH_SIZE_MULT_1];    // 141
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n EnhSzMult1 = 0x%x \n EnhSzMult0 = 0x%x \n",
    		Dvr_pSLC.EnhSzMult1, Dvr_pSLC.EnhSzMult0, 0, 0, 0, 0); 
    
if((Dvr_pSLC.EnhSzMult0 == 0) && (Dvr_pSLC.EnhSzMult1 == 0))
{    
    /* enable pSLC mode, eMMC size down to half */
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  pDrvCtrl->capacity = 0x%X\n",
                     pDrvCtrl->capacity, 0, 0, 0, 0, 0);
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtIdentify() -  pDrvCtrl->capacity2 = 0x%X\n",
                     (pDrvCtrl->capacity)>>32, 0, 0, 0, 0, 0);

    
    /* Use high-capacity erase unit size, high-capacity time out, and high-capacity protect group size definition */
    rc = emmcSwitch(pDev, EXT_CSD_ERASE_GROUP_DEF, MMC_ERASE_GRP_ENABLE, 0x0); 
       if (rc == ERROR)
           {
           MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Erase Group Def enable fault\n",
                        0, 0, 0, 0, 0, 0);
           }
       vxbMsDelay(100);
    
       /* strat address from 0x00000000 */
    rc = emmcSwitch(pDev, EXT_CSD_ENH_START_ADDR_3,  0x0, 0x0); 
       if (rc == ERROR)
           {
           MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Start Address 3 fault\n",
                           0, 0, 0, 0, 0, 0);
           }
       vxbMsDelay(100);
       
    rc = emmcSwitch(pDev, EXT_CSD_ENH_START_ADDR_2,  0x0, 0x0); 
	   if (rc == ERROR)
		   {
		   MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Start Address 2 fault\n",
						  0, 0, 0, 0, 0, 0);
		   }
	   vxbMsDelay(100);
       
	rc = emmcSwitch(pDev, EXT_CSD_ENH_START_ADDR_1,  0x0, 0x0); 
	    if (rc == ERROR)
		    {
		    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Start Address 1 fault\n",
						   0, 0, 0, 0, 0, 0);
		    }
	    vxbMsDelay(100);
	   
	 rc = emmcSwitch(pDev, EXT_CSD_ENH_START_ADDR_0,  0x0, 0x0); 
	    if (rc == ERROR)
		    {
		    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Start Address 0 fault\n",
						  0, 0, 0, 0, 0, 0);
		    }
	    vxbMsDelay(100);
		   
    Dvr_pSLC.MaxEnhSize2 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT2];      //159
    rc = emmcSwitch(pDev, EXT_CSD_ENH_SIZE_MULT_2, Dvr_pSLC.MaxEnhSize2, 0x0); //142
   	   if (rc == ERROR)
   		   {
   		   MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Area Size2 fault\n",
   					   0, 0, 0, 0, 0, 0);
   		   }
   	   vxbMsDelay(100);

   	Dvr_pSLC.MaxEnhSize1 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT1];      //158
    //rc = emmcSwitch(pDev, EXT_CSD_ENH_SIZE_MULT_1, Dvr_pSLC.MaxEnhSize1, 0x0); //141
    rc = emmcSwitch(pDev, EXT_CSD_ENH_SIZE_MULT_1, Dvr_pSLC.MaxEnhSize1, 0x0); //141
	   if (rc == ERROR)
		   {
		   MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Area Size1 fault\n",
					   0, 0, 0, 0, 0, 0);
		   }
	   vxbMsDelay(100);
       
    Dvr_pSLC.MaxEnhSize0 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT];      // 157
    rc = emmcSwitch(pDev, EXT_CSD_ENH_SIZE_MULT, Dvr_pSLC.MaxEnhSize0, 0x0); //140
       if (rc == ERROR)
           {
           MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced User Data Area Size0 fault\n",
                        0, 0, 0, 0, 0, 0);
           }
       vxbMsDelay(100);
    

    rc = emmcSwitch(pDev, EXT_CSD_PARTITION_ATTRIBUTE, MMC_PARTITION_ENH_USR_SET, 0x0); 
       if (rc == ERROR)
           {
           MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Set Enhanced attribute in User Data Area fault\n",
                        0, 0, 0, 0, 0, 0);
           }
       vxbMsDelay(100);

	rc = emmcSwitch(pDev, EXT_CSD_PARTITIONS_SET, MMC_PARTITION_COMPLITION_SET, 0x0); 
	   if (rc == ERROR)
		   {
		   MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  Completion Bit Set fault\n",
					   0, 0, 0, 0, 0, 0);
		   }
	   vxbMsDelay(100);
}
	    
	 /* CMD 8: request card to send its ext csd */

	 if (regValue >= MMC_CSD_SPEC_40)
	 	{
	 	rc = emmcCmdReadExtCsd (pDev, pMmcInfo->extCsd);
		if (rc == ERROR)
		{
			MMC_CARD_DBG(MMC_CARD_DBG_INIT,
											"MMC version is < 2\n",
														 0, 0, 0, 0, 0, 0);
			goto err;
		}
		(void)emmcExtCsdDecode (pDev);
		}
	vxbMsDelay(100);
    pSdHardWare->blockSize = SDMMC_BLOCK_SIZE;
           
    if (pDrvCtrl->highSpeed)
        {
        UINT8 extCsdWord196, extCsdWord192, extCsdWord194;
        extCsdWord196 = pMmcInfo->extCsd[196];
        extCsdWord192 = pMmcInfo->extCsd[192];
        extCsdWord194 = pMmcInfo->extCsd[194];
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  CSD Device type = %ld\n"
                     "CSD Version = %ld CSD Structure = %ld\n",
					 extCsdWord196, extCsdWord192, extCsdWord194, 0, 0, 0);
        
        rc = emmcSwitch(pDev, EXT_CSD_HS_TIMING, MMC_EXT_CSD_HSTIME_ENABLE, 0x0);
        if (rc == ERROR)
            {
            MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch enable HSTIME fault\n",
                         0, 0, 0, 0, 0, 0);
            return ERROR;
            }
        
        vxbMsDelay(1);
        
        if (pHostSpec->vxbSdClkFreqSetup != NULL)
            {
            /* set work frequen(CSD ver in our case=7), enable HS 200 */
            
        	pDrvCtrl->tranSpeed = MMC_CLK_FREQ_52MHZ;        	
            pHostSpec->vxbSdClkFreqSetup(pSdHardWare->pHostDev, pDrvCtrl->tranSpeed);
            }
        vxbMsDelay(1);
        
            {
        	pHostSpec->busWidth = 8;
            switch (pHostSpec->busWidth)
                {
                case 8:
                	
                    rc = emmcSwitch(pDev, EXT_CSD_BUS_WIDTH, MMC_EXT_CSD_BUSWIDTH_8BIT, 0x0); 
                       if (rc == ERROR)
                           {
                           MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch bus width 8bit fault\n",
                                        0, 0, 0, 0, 0, 0);
                           }
                       vxbMsDelay(100);

            
                    if (pHostSpec->vxbSdBusWidthSetup != NULL)
                        {
                        pHostSpec->vxbSdBusWidthSetup(pSdHardWare->pHostDev, SDMMC_BUS_WIDTH_8BIT);
                        } 
                    vxbMsDelay(100);        
                    break;
                    
                case 4:
                    rc = emmcSwitch(pDev, EXT_CSD_BUS_WIDTH, MMC_EXT_CSD_BUSWIDTH_4BIT,0x0); 
                    if (rc == ERROR)
                        {
                        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch bus width 4bit fault\n",
                                     0, 0, 0, 0, 0, 0);
                        }
                        
                    if (pHostSpec->vxbSdBusWidthSetup != NULL)
                        {
                        pHostSpec->vxbSdBusWidthSetup(pSdHardWare->pHostDev, SDMMC_BUS_WIDTH_4BIT);
                        }
                        
                    break;
                    
                case 1:
                    rc = emmcSwitch(pDev, EXT_CSD_BUS_WIDTH, MMC_EXT_CSD_BUSWIDTH_1BIT,0x0); 
                    if (rc == ERROR)
                        {
                        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch bus width 1bit fault\n",
                                     0, 0, 0, 0, 0, 0);
                        }
                        
                    if (pHostSpec->vxbSdBusWidthSetup != NULL)
                        {
                        pHostSpec->vxbSdBusWidthSetup(pSdHardWare->pHostDev, SDMMC_BUS_WIDTH_1BIT);
                        }
                        
                    break;
                    
                default:
                    rc = ENOTSUP;
                    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch bus width %dbit invalid\n",
                                     pHostSpec->busWidth, 0, 0, 0, 0, 0);
                    break;                    
                }
            if (rc == ENOTSUP)
                {
                return (ERROR);
                }
            
            vxbMsDelay(1);            
            }
        }
    else
        {
        if (pHostSpec->vxbSdClkFreqSetup != NULL)
            {
            pHostSpec->vxbSdClkFreqSetup(pSdHardWare->pHostDev, pDrvCtrl->tranSpeed);
            }
        }
    
	    // Reinitialize the card    
	    rc = emmcSwitch(pDev, 162, MMC_EXT_CSD_HSTIME_ENABLE, 0x0);
		if (rc == ERROR)
			{
			MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcSwitch unable to reset\n",
						 0, 0, 0, 0, 0, 0);
			return ERROR;
			}		
		vxbMsDelay(100);
	
	
	rc = emmcCardIdxAlloc (pDrvCtrl);
	if (rc != OK)
		{
		MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcIdentify() -  emmcCardIdxAlloc fault\n",
					 0, 0, 0, 0, 0, 0);
		goto err;
		}

    pDrvCtrl->attached = TRUE;

    /* set xbd params */
    emmcXbdParams.xbdOps.blkRead = emmcStorageBlkRead;
    emmcXbdParams.xbdOps.blkWrite = emmcStorageBlkWrite;
    emmcXbdParams.xbdOps.blkDump = NULL;
    emmcXbdParams.xbdOps.blkIoctl = emmcStorageIoctl;
    emmcXbdParams.xbdOps.xferReq  = NULL;
    emmcXbdParams.maxActiveReqs = 1;
    emmcXbdParams.maxBiosPerReq = 1;
    emmcXbdParams.maxXferBlks = MMC_MAX_RW_SECTORS;
    emmcXbdParams.directModeFlag = pHostSpec->directBio;
    emmcXbdParams.numBlks = (sector_t)(pDrvCtrl->capacity / pSdHardWare->blockSize);
    emmcXbdParams.blkSize = pSdHardWare->blockSize;
    emmcXbdParams.svcTskPri = SDMMC_XBD_SVC_TASK_PRI;
    emmcXbdParams.pDev = (void *)pDev;
    (void)snprintf ((char *)&(emmcXbdParams.devName[0]), sizeof(emmcXbdParams.devName),
                    "/emmc%d", pHostSpec->unitNumber);
     (void)taskDelay (sysClkRateGet ());
    xbdSts = emmcXbdDevCreate (&(pDrvCtrl->xbdDev), &emmcXbdParams);

    if (xbdSts == NULLDEV)
        {
        pDrvCtrl->attached = FALSE;
        return ERROR;
        }

    return OK;
err:
    if (pDrvCtrl->pInfo != NULL)
        vxbMemFree (pDrvCtrl->pInfo);
    return ERROR;
    }

/*******************************************************************************
*
* emmcExtCsdDecode - identify SD/MMC card in the system
*
* This routine implements the card identify process according to the SD spec.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcExtCsdDecode
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    MMC_INFO * pMmcInfo;    
    SD_HOST_SPEC * pHostSpec;
    SD_HARDWARE * pSdHardWare;
    UINT32 csize, csizeMulti;
    UINT64 blkNum;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        return ERROR;

    pMmcInfo = (MMC_INFO *)(pDrvCtrl->pInfo);
    if (pMmcInfo == NULL)
        return (ERROR);

    pHostSpec = (SD_HOST_SPEC *)(pSdHardWare->pHostSpec);
    if (pHostSpec == NULL)
        return ERROR;

    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  extCsdInfo = 0x%x\n",
                 pMmcInfo->extCsd, 0, 0, 0, 0, 0);
	
    /* EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A and EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B */
    Dvr_EmmcEstLife.LifeTimeEstA = pMmcInfo->extCsd[EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A];
    Dvr_EmmcEstLife.LifeTimeEstB = pMmcInfo->extCsd[EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B];
    Dvr_EmmcEstLife.PreEOLInfo   = pMmcInfo->extCsd[EXT_CSD_DEVICE_PRE_EOL_INFO];
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\nemmcExtCsdDecode() -  \nLifeTimeEstimatedUsageA = 0x%x \n LifeTimeEstimatedUsageB = 0x%x \n LifeTimeEstimatedAverage = 0x%x \n",
    		Dvr_EmmcEstLife.LifeTimeEstA, Dvr_EmmcEstLife.LifeTimeEstB, Dvr_EmmcEstLife.PreEOLInfo, 0, 0, 0); 

	
    kprintf("\nemmcExtCsdDecode() - \n LifeTimeEstimatedUsageA = 0x%x \n LifeTimeEstimatedUsageB = 0x%x \n LifeTimeEstimatedAverage = 0x%x \n", Dvr_EmmcEstLife.LifeTimeEstA, Dvr_EmmcEstLife.LifeTimeEstB, Dvr_EmmcEstLife.PreEOLInfo) ;   
    
	Dvr_pSLC.PartSupport = pMmcInfo->extCsd[EXT_CSD_PARTITION_SUPPORT];    // 160
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n pSLC: \n PartSupport = 0x%x \n", Dvr_pSLC.PartSupport, 0, 0, 0, 0, 0); 
    Dvr_pSLC.MaxEnhSize2 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT2];    // 159
    Dvr_pSLC.MaxEnhSize1 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT1];    // 158
    Dvr_pSLC.MaxEnhSize0 = pMmcInfo->extCsd[EXT_CSD_MAX_ENH_SIZE_MULT];     // 157
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n MaxEnhSize2 = 0x%x \n MaxEnhSize1 = 0x%x \n MaxEnhSize0 = 0x%x \n",
    		Dvr_pSLC.MaxEnhSize2, Dvr_pSLC.MaxEnhSize1, Dvr_pSLC.MaxEnhSize0, 0, 0, 0); 
    Dvr_pSLC.EnhAttrGPP = pMmcInfo->extCsd[EXT_CSD_PARTITION_ATTRIBUTE];     // 156
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n EnhAttrGPP = 0x%x \n", Dvr_pSLC.EnhAttrGPP, 0, 0, 0, 0, 0); 
    Dvr_pSLC.ParSetComplete = pMmcInfo->extCsd[EXT_CSD_PARTITIONS_SET];    // 155
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n ParSetComplete = 0x%x \n", Dvr_pSLC.ParSetComplete, 0, 0, 0, 0, 0); 
    Dvr_pSLC.GPparSzMult42 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_4_2];   // 154
    Dvr_pSLC.GPparSzMult41 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_4_1];   // 153
    Dvr_pSLC.GPparSzMult40 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_4_0];   // 152
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n GPparSzMult42 = 0x%x \n GPparSzMult41 = 0x%x \n GPparSzMult40 = 0x%x \n",
    		Dvr_pSLC.GPparSzMult42, Dvr_pSLC.GPparSzMult41, Dvr_pSLC.GPparSzMult40, 0, 0, 0); 
    Dvr_pSLC.GPparSzMult32 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_3_2];   // 151
    Dvr_pSLC.GPparSzMult31 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_3_1];   // 150
    Dvr_pSLC.GPparSzMult30 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_3_0];   // 149
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n GPparSzMult32 = 0x%x \n GPparSzMult31 = 0x%x \n GPparSzMult30 = 0x%x \n",
    		Dvr_pSLC.GPparSzMult32, Dvr_pSLC.GPparSzMult31, Dvr_pSLC.GPparSzMult30, 0, 0, 0); 
    Dvr_pSLC.GPparSzMult22 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_2_2];   // 148
    Dvr_pSLC.GPparSzMult21 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_2_1];   // 147
    Dvr_pSLC.GPparSzMult20 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_2_0];   // 146
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n GPparSzMult22 = 0x%x \n GPparSzMult21 = 0x%x \n GPparSzMult20 = 0x%x \n",
    		Dvr_pSLC.GPparSzMult22, Dvr_pSLC.GPparSzMult21, Dvr_pSLC.GPparSzMult20, 0, 0, 0); 
    Dvr_pSLC.GPparSzMult12 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_1_2];   // 145
    Dvr_pSLC.GPparSzMult11 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT_1_1];   // 144
    Dvr_pSLC.GPparSzMult10 = pMmcInfo->extCsd[EXT_CSD_GP_SIZE_MULT];       // 143
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n GPparSzMult12 = 0x%x \n GPparSzMult11 = 0x%x \n GPparSzMult10 = 0x%x \n",
    		Dvr_pSLC.GPparSzMult12, Dvr_pSLC.GPparSzMult11, Dvr_pSLC.GPparSzMult10, 0, 0, 0); 
    Dvr_pSLC.EnhSzMult2 = pMmcInfo->extCsd[EXT_CSD_ENH_SIZE_MULT_2];      // 142
    Dvr_pSLC.EnhSzMult1 = pMmcInfo->extCsd[EXT_CSD_ENH_SIZE_MULT_1];      // 141
    Dvr_pSLC.EnhSzMult0 = pMmcInfo->extCsd[EXT_CSD_ENH_SIZE_MULT];        // 140
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n EnhSzMult2 = 0x%x \n EnhSzMult1 = 0x%x \n EnhSzMult0 = 0x%x \n",
    		Dvr_pSLC.EnhSzMult2, Dvr_pSLC.EnhSzMult1, Dvr_pSLC.EnhSzMult0, 0, 0, 0); 
		
	kprintf("\nemmcExtCsdDecode() - \n EnhSzMult2 = 0x%x \n EnhSzMult1 = 0x%x \n EnhSzMult0 = 0x%x \n", Dvr_pSLC.EnhSzMult2, Dvr_pSLC.EnhSzMult1, Dvr_pSLC.EnhSzMult0);   
		
    Dvr_pSLC.EnhStartAddr3 = pMmcInfo->extCsd[EXT_CSD_ENH_START_ADDR_3];   // 139     
    Dvr_pSLC.EnhStartAddr2 = pMmcInfo->extCsd[EXT_CSD_ENH_START_ADDR_2];   // 138     
    Dvr_pSLC.EnhStartAddr1 = pMmcInfo->extCsd[EXT_CSD_ENH_START_ADDR_1];   // 137     
    Dvr_pSLC.EnhStartAddr0 = pMmcInfo->extCsd[EXT_CSD_ENH_START_ADDR_0];   // 136  
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n EnhStartAddr3 = 0x%x \n EnhStartAddr2 = 0x%x \n EnhStartAddr1 = 0x%x \n EnhStartAddr0 = 0x%x \n",
    		Dvr_pSLC.EnhStartAddr3, Dvr_pSLC.EnhStartAddr2, Dvr_pSLC.EnhStartAddr1, Dvr_pSLC.EnhStartAddr0, 0, 0); 
    
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "\n HC_WP_GRP_SIZE = 0x%x \n ERASE_TIMEOUT_MULT = 0x%x \n HC_ERASE_GRP_SIZE = 0x%x \n",
    		pMmcInfo->extCsd[EXT_CSD_HC_WP_GRP_SIZE], pMmcInfo->extCsd[EXT_CSD_ERASE_TIMEOUT_MULT], pMmcInfo->extCsd[EXT_CSD_HC_ERASE_GRP_SIZE], 0, 0, 0); 
			
	kprintf("\nemmcExtCsdDecode() - \n HC_WP_GRP_SIZE = 0x%x \n HC_ERASE_GRP_SIZE = 0x%x \n", pMmcInfo->extCsd[EXT_CSD_HC_WP_GRP_SIZE], pMmcInfo->extCsd[EXT_CSD_HC_ERASE_GRP_SIZE]);   

				 
    /* WR_REL_PARAM */
    pDrvCtrl->extCsdInfo.relParam = pMmcInfo->extCsd[EXT_CSD_WR_REL_PARAM];
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  extCsdInfo.relParam = 0x%x\n",
                 pDrvCtrl->extCsdInfo.relParam, 0, 0, 0, 0, 0);

    /* EXT_CSD_REL_WR_SEC_C */
    pDrvCtrl->extCsdInfo.relSectors = pMmcInfo->extCsd[EXT_CSD_REL_WR_SEC_C];
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  extCsdInfo.relSectors = %d\n",
                 pDrvCtrl->extCsdInfo.relSectors, 0, 0, 0, 0, 0);

    /* EXT_CSD_WR_REL_SET */
    pDrvCtrl->extCsdInfo.relSet = pMmcInfo->extCsd[EXT_CSD_WR_REL_SET];
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  extCsdInfo.relSet = 0x%x\n",
                 pDrvCtrl->extCsdInfo.relSet, 0, 0, 0, 0, 0);

    pDrvCtrl->sectorSize = MMC_DEFAULT_SECTOR_SIZE;
    pDrvCtrl->extCsdInfo.rev = pMmcInfo->extCsd[EXT_CSD_REV];
    if (pDrvCtrl->extCsdInfo.rev > MMC_EXT_CSD_VERSION_2)
        {
        pDrvCtrl->extCsdInfo.sectors = pMmcInfo->extCsd[EXT_CSD_SEC_CNT + 0] << 0 |
                                       pMmcInfo->extCsd[EXT_CSD_SEC_CNT + 1] << 8 |
                                       pMmcInfo->extCsd[EXT_CSD_SEC_CNT + 2] << 16 |
                                       pMmcInfo->extCsd[EXT_CSD_SEC_CNT + 3] << 24;
        if (pDrvCtrl->extCsdInfo.sectors > (UINT32)(MMC_2G_SECTORS))
            pDrvCtrl->isBlockDevice = TRUE;
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  extCsdInfo.sectors = %d\n",
                     pDrvCtrl->extCsdInfo.sectors, 0, 0, 0, 0, 0);
        }
    else
        {

        /* maximum read block length */
        pDrvCtrl->readBlkLen = 1 << (pMmcInfo->csd.commClass & 0x0f);

        /* per SD spec, the maximum write block length is equal to read block */
        pDrvCtrl->writeBlkLen = pDrvCtrl->readBlkLen;
        pDrvCtrl->sectorSize = pDrvCtrl->readBlkLen;

        csize = ((UINT32)(pMmcInfo->csd.resvData0[0] & 0x03) << 10) |
                ((UINT32)(pMmcInfo->csd.resvData0[1]) << 2) |
                ((pMmcInfo->csd.resvData0[2] >> 6) & 0x03);
        csizeMulti = ((pMmcInfo->csd.resvData0[3] & 0x03) << 1) |
                     ((pMmcInfo->csd.eraseSize >> 15) & 0x01);
        blkNum = (csize + 1) << (csizeMulti + 2);
        pDrvCtrl->blkNum = blkNum;
        pDrvCtrl->capacity = blkNum * pDrvCtrl->sectorSize;

        if (pDrvCtrl->capacity < (2 * 1024 * 1024))
            pDrvCtrl->highCapacity = FALSE;
        MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  pDrvCtrl->blkNum = %d\n"
                     "pDrvCtrl->capacity = %d\n",
                     pDrvCtrl->blkNum, pDrvCtrl->capacity, 0, 0, 0, 0);
        }

    pDrvCtrl->extCsdInfo.deviceType = pMmcInfo->extCsd[EXT_CSD_DEVICE_TYPE] &
                                      EXT_CSD_DEVICE_TYPE_MASK;
    switch (pDrvCtrl->extCsdInfo.deviceType)
        {
        case EXT_CSD_DEVICE_TYPE_SDR_ALL:
        case EXT_CSD_DEVICE_TYPE_SDR_ALL_DDR18V:
        case EXT_CSD_DEVICE_TYPE_SDR_ALL_DDR_12V:
        case EXT_CSD_DEVICE_TYPE_SDR_ALL_DDR_52:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_200MHZ;
            pDrvCtrl->voltage = (MMC_DEVICE_VOLTAGE_12V | MMC_DEVICE_VOLTAGE_18V);
            break;
        case EXT_CSD_DEVICE_TYPE_SDR12V_ALL:
        case EXT_CSD_DEVICE_TYPE_SDR12V_DDR18V:
        case EXT_CSD_DEVICE_TYPE_SDR12V_DDR12V:
        case EXT_CSD_DEVICE_TYPE_SDR12V_DDR_52:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_200MHZ;
            pDrvCtrl->voltage = MMC_DEVICE_VOLTAGE_12V;
            break;
        case EXT_CSD_DEVICE_TYPE_SDR18V_ALL:
        case EXT_CSD_DEVICE_TYPE_SDR18V_DDR18V:
        case EXT_CSD_DEVICE_TYPE_SDR18V_DDR12V:
        case EXT_CSD_DEVICE_TYPE_SDR18V_DDR_52:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_200MHZ;
            pDrvCtrl->voltage = MMC_DEVICE_VOLTAGE_18V;
            break;
        case EXT_CSD_DEVICE_TYPE_DDR_52 | EXT_CSD_DEVICE_TYPE_52 | 
             EXT_CSD_DEVICE_TYPE_26:
        case EXT_CSD_DEVICE_TYPE_52 | EXT_CSD_DEVICE_TYPE_26:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_52MHZ;
            break;
        case EXT_CSD_DEVICE_TYPE_DDR_12V | EXT_CSD_DEVICE_TYPE_52 |
             EXT_CSD_DEVICE_TYPE_26:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_52MHZ;
            pDrvCtrl->voltage = MMC_DEVICE_VOLTAGE_12V;
            break;
        case EXT_CSD_DEVICE_TYPE_DDR_18V | EXT_CSD_DEVICE_TYPE_52 |
             EXT_CSD_DEVICE_TYPE_26:
            pDrvCtrl->highSpeed = TRUE;
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_52MHZ;
            pDrvCtrl->voltage = MMC_DEVICE_VOLTAGE_18V;
            break;
        case EXT_CSD_DEVICE_TYPE_26:
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_26MHZ;
            break;
        default:
            pDrvCtrl->tranSpeed = MMC_CLK_FREQ_26MHZ;
            pDrvCtrl->highSpeed = FALSE;
            break;            
        }

    /* check for the device transmission speed does not exceed max controller speed */
    if (pHostSpec->maxTranSpeed != 0 && pDrvCtrl->tranSpeed > pHostSpec->maxTranSpeed)
        {
        pDrvCtrl->tranSpeed = pHostSpec->maxTranSpeed;
        }

    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  pDrvCtrl->tranSpeed = %d\n",
                 pDrvCtrl->tranSpeed, 0, 0, 0, 0, 0);

    pDrvCtrl->extCsdInfo.eraseTimeoutMult = pMmcInfo->extCsd[EXT_CSD_ERASE_TIMEOUT_MULT];
    pDrvCtrl->extCsdInfo.hcEraseSize = pMmcInfo->extCsd[EXT_CSD_HC_ERASE_GRP_SIZE];

    if (pDrvCtrl->extCsdInfo.rev >= MMC_EXT_CSD_VERSION_3)
        {
        pDrvCtrl->extCsdInfo.partConfig = pMmcInfo->extCsd[EXT_CSD_PART_CONFIG];
        pDrvCtrl->extCsdInfo.partTime = MMC_EXT_CSD_TIMEUNIT *
                                        pMmcInfo->extCsd[EXT_CSD_PART_SWITCH_TIME];
        pDrvCtrl->extCsdInfo.saTimeout = 1 << pMmcInfo->extCsd[EXT_CSD_S_A_TIMEOUT];
        pDrvCtrl->extCsdInfo.eraseGroupDef = pMmcInfo->extCsd[EXT_CSD_ERASE_GROUP_DEF];
        pDrvCtrl->extCsdInfo.hcEraseTimeout = MMC_EXT_CSD_HCERASETIMEOUT_UNIT *
                                              pMmcInfo->extCsd[EXT_CSD_ERASE_TIMEOUT_MULT];
        pDrvCtrl->extCsdInfo.hcEraseSize = pMmcInfo->extCsd[EXT_CSD_HC_ERASE_GRP_SIZE] << 10;
        }

    if (pDrvCtrl->extCsdInfo.rev >= MMC_EXT_CSD_VERSION_4)
        {
        pDrvCtrl->extCsdInfo.secTrimMult = pMmcInfo->extCsd[EXT_CSD_SEC_TRIM_MULT];
        pDrvCtrl->extCsdInfo.secEraseMult = pMmcInfo->extCsd[EXT_CSD_SEC_ERASE_MULT];
        pDrvCtrl->extCsdInfo.fetureSupport = pMmcInfo->extCsd[EXT_CSD_SEC_FEATURE_SUPPORT];
        pDrvCtrl->extCsdInfo.trimTimeout = MMC_EXT_CSD_HCERASETIMEOUT_UNIT *
                                           pMmcInfo->extCsd[EXT_CSD_TRIM_MULT]; 
        pDrvCtrl->extCsdInfo.bootLock = pMmcInfo->extCsd[EXT_CSD_BOOT_WP];
        pDrvCtrl->extCsdInfo.bootLockEnable = TRUE;
        }

    if (pDrvCtrl->extCsdInfo.rev >= MMC_EXT_CSD_VERSION_5)
        {

        /* check the eMMC card supports HPI status */

        if ((pMmcInfo->extCsd[EXT_CSD_HPI_FEATURES] & MMC_EXT_HPI_SUPPORT) != 0x0)
            {
            pDrvCtrl->extCsdInfo.hpiSupport = TRUE;
            if ((pMmcInfo->extCsd[EXT_CSD_HPI_FEATURES]
                & MMC_EXT_HPI_IMPLEMENTATION) != 0x0)
                pDrvCtrl->extCsdInfo.hpiCmdSet = SDMMC_CMD_STOP_TRANSMISSION;
            else
                pDrvCtrl->extCsdInfo.hpiCmdSet = SDMMC_CMD_SEND_STATUS;

            pDrvCtrl->extCsdInfo.outIntTimeout = (UINT8)(MMC_EXT_CSD_TIMEUNIT *
                                                         pMmcInfo->extCsd[EXT_CSD_OUT_OF_INTERRUPT_TIME]);
            }
        }

    /* eMMC v4.5 or later */

    if (pDrvCtrl->extCsdInfo.rev >= MMC_EXT_CSD_VERSION_6)
        {
        pDrvCtrl->extCsdInfo.genericCmd6Time = MMC_EXT_CSD_TIMEUNIT *
                                               pMmcInfo->extCsd[EXT_CSD_GENERIC_CMD6_TIME];
        pDrvCtrl->extCsdInfo.powerOffLongtime = MMC_EXT_CSD_TIMEUNIT *
                                                pMmcInfo->extCsd[EXT_CSD_POWER_OFF_LONG_TIME];

        pDrvCtrl->extCsdInfo.cacheSize = pMmcInfo->extCsd[EXT_CSD_CACHE_SIZE + 0] << 0 |
                                         pMmcInfo->extCsd[EXT_CSD_CACHE_SIZE + 1] << 8 |
                                         pMmcInfo->extCsd[EXT_CSD_CACHE_SIZE + 2] << 16 |
                                         pMmcInfo->extCsd[EXT_CSD_CACHE_SIZE + 3] << 24;

        pDrvCtrl->extCsdInfo.cacheCtrl = pMmcInfo->extCsd[EXT_CSD_CACHE_CTRL];
        if (pMmcInfo->extCsd[EXT_CSD_DATA_SECTOR_SIZE] == 1)
            pDrvCtrl->sectorSize = 4096;
        else
            pDrvCtrl->sectorSize = 512;
        }

    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  pDrvCtrl->sectorSize = %d\n",
                 pDrvCtrl->sectorSize, 0, 0, 0, 0, 0);
    
    if (pDrvCtrl->extCsdInfo.rev > MMC_EXT_CSD_VERSION_2)
        pDrvCtrl->capacity = (UINT64)(pDrvCtrl->extCsdInfo.sectors) *
                             (UINT64)(pDrvCtrl->sectorSize);
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  pDrvCtrl->capacity = 0x%X\n",
                     pDrvCtrl->capacity, 0, 0, 0, 0, 0);
    MMC_CARD_DBG(MMC_CARD_DBG_INIT, "emmcExtCsdDecode() -  pDrvCtrl->capacity2 = 0x%X\n",
                     (pDrvCtrl->capacity)>>32, 0, 0, 0, 0, 0);
 
    return (OK);
    }

/*******************************************************************************
*
* emmcCacheFlush - send command to flush MMC device's cache
*
* This routine sends command to flush MMC device's cache.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcCacheFlush
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    STATUS rc;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if ((pDrvCtrl->extCsdInfo.cacheCtrl & MMC_EXT_CSD_CACHE_EN) && 
        (pDrvCtrl->extCsdInfo.cacheSize > 0))
        {
        rc = emmcSwitch(pDev, EXT_CSD_FLUSH_CACHE, 1, 0x0);
        if (rc == ERROR)
            {
            MMC_CARD_DBG(MMC_CARD_DBG_INIT,
                         "emmcCacheFlush() -  emmcSwitch set cache flush fault\n",
                         0, 0, 0, 0, 0, 0);
            return (ERROR);
            }
        }
    return OK;
    }

/*******************************************************************************
*
* sdMmcStatusWaitReadyForData - check SD/MMC card ready for data status
*
* This routine checks SD/MMC card ready for data status.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStatusWaitReadyForData
    (
    VXB_DEV_HANDLE pDev
    )
    {
    UINT32 rspValue = 0;
    STATUS rc;

    while (1)
        {
        rc = emmcCmdSendStatus (pDev, &rspValue);
        if (rc == ERROR)
            return ERROR;

        if (rspValue & CARD_STS_READY_FOR_DATA)
            break;
        }
    return OK;
    }

/*******************************************************************************
*
* emmcErase - send erase/discard/trim command to emmc card
*
* This routine sends erase/discard/trim command to emmc card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcErase
    (
    VXB_DEV_HANDLE pDev,
    SECTOR_RANGE * pRange
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    STATUS rc;
    UINT32 eraseArg = 0;
    long long sectorNum;

    if (pRange == NULL)
        return (ERROR);

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if (pDrvCtrl->highCapacity)
        sectorNum = pRange->startSector;
    else
        sectorNum = (pRange->startSector) * pDrvCtrl->sectorSize;

    rc = emmcCmdSeEraseGrpStart(pDev, (UINT32)sectorNum);
    if (rc != OK)
        return (rc);
    
    if (pDrvCtrl->highCapacity)
        sectorNum = pRange->startSector + pRange->nSectors - 1;
    else
        sectorNum = (pRange->startSector + pRange->nSectors) * pDrvCtrl->sectorSize;

    rc = emmcCmdSeEraseGrpEnd (pDev, (UINT32)sectorNum);
    if (rc != OK)
        return (rc);

    if (pDrvCtrl->extCsdInfo.rev >= 6)
        eraseArg = MMC_ERASE_DISCARD;
    if ((pDrvCtrl->extCsdInfo.fetureSupport & MMC_EXT_CSD_FEA_SEC_GB_CL_EN) != 0x0)
        eraseArg = MMC_ERASE_TRIM;
    rc = emmcCmdSetEraseGrp(pDev, eraseArg);
    return (rc);
    }

#ifdef NEED_TO_COMPILE
/*******************************************************************************
*
* emmcExtCsdSetPowClass - set MMC device power class
*
* This routine sets MMC device power class.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcExtCsdSetPowClass
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    MMC_INFO * pMmcInfo;    
    SD_HARDWARE * pHardWare;
    UINT8 powClassIndex = 0;
    STATUS rc = OK;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    pHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pHardWare == NULL)
        {
        MMC_CARD_DBG (MMC_CARD_DBG_INIT, "emmcExtCsdSetPowClass() - pSdHardWare is NULL\n",
                      0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pMmcInfo = (MMC_INFO *)(pDrvCtrl->pInfo);
    if (pMmcInfo == NULL)
        return (ERROR);

    if ((pMmcInfo->csd.structure & 0x3C) < MMC_CSD_SPEC_VERS_4)
        return (ERROR);
    if ((pDrvCtrl->voltage & MMC_DEVICE_VOLTAGE_30V) != 0x0)
        {
        if (pDrvCtrl->tranSpeed <= MMC_CLK_FREQ_26MHZ)
            powClassIndex = pMmcInfo->extCsd[EXT_CSD_PWR_CL_26_360];
        else if (pDrvCtrl->tranSpeed <= MMC_CLK_FREQ_52MHZ)
            powClassIndex = pMmcInfo->extCsd[EXT_CSD_PWR_CL_52_360];            
        }
    else
        {
        if (pDrvCtrl->tranSpeed <= MMC_CLK_FREQ_26MHZ)
            powClassIndex = pMmcInfo->extCsd[EXT_CSD_PWR_CL_26_360];
        else if (pDrvCtrl->tranSpeed <= MMC_CLK_FREQ_52MHZ)
            powClassIndex = pMmcInfo->extCsd[EXT_CSD_PWR_CL_52_360];
        }
    if (powClassIndex != 0)
        {
        powClassIndex = (powClassIndex & MMC_EXT_CSD_PWR_8BIT_MASK) >>
                         MMC_EXT_CSD_PWR_8BIT_SHIFT;
        if (powClassIndex == 0x0)
            powClassIndex = (UINT8)(powClassIndex & MMC_EXT_CSD_PWR_4BIT_MASK);            
        }
    if (powClassIndex > 0)
        {
        rc = emmcSwitch(pDev, EXT_CSD_POWER_CLASS, powClassIndex, 0x0);
        if (rc == ERROR)
            return ERROR;
        }
    return (OK);
    }
    
/*******************************************************************************
*
* emmcCacheEn - send command to enable/disable MMC device's cache
*
* This routine sends command to enable/disable MMC device's cache.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcCacheEn
    (
    VXB_DEV_HANDLE pDev,
    UINT8 bEnable
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    STATUS rc = OK;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if (pDrvCtrl->extCsdInfo.cacheSize > 0)
        {
        rc = emmcSwitch(pDev, EXT_CSD_CACHE_CTRL, bEnable, 0x0);
        if (rc == ERROR)
            {
            MMC_CARD_DBG(MMC_CARD_DBG_INIT,
                         "emmcCacheFlush() -  emmcSwitch set cache flush fault\n",
                         0, 0, 0, 0, 0, 0);
            return (ERROR);
            }
        }
    
    return (rc);
    }
    
/*******************************************************************************
*
* emmcSecErase - send secure erase/discard/trim command to emmc card
*
* This routine sends erase/discard/trim command to emmc card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcSecErase
    (
    VXB_DEV_HANDLE pDev,
    SECTOR_RANGE * pRange
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    STATUS rc;
    UINT32 eraseArg = 0;
    long long sectorNum;

    if (pRange == NULL)
        return (ERROR);

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if (pDrvCtrl->highCapacity)
        sectorNum = pRange->startSector;
    else
        sectorNum = (pRange->startSector) * pDrvCtrl->sectorSize;

    rc = emmcCmdSeEraseGrpStart(pDev, (UINT32)sectorNum);
    if (rc != OK)
        return (rc);
    
    if (pDrvCtrl->highCapacity)
        sectorNum = pRange->startSector + pRange->nSectors - 1;
    else
        sectorNum = (pRange->startSector + pRange->nSectors) * pDrvCtrl->sectorSize;

    rc = emmcCmdSeEraseGrpEnd (pDev, (UINT32)sectorNum);
    if (rc != OK)
        return (rc);

    if (pDrvCtrl->extCsdInfo.rev >= 6)
        eraseArg = MMC_SECURE_ERASE;
    if ((pDrvCtrl->extCsdInfo.fetureSupport & MMC_EXT_CSD_FEA_SEC_GB_CL_EN) != 0x0)
        eraseArg = MMC_SECURE_TRIMARG1;
    rc = emmcCmdSetEraseGrp(pDev, eraseArg);
    if (rc == ERROR)
        return (rc);
    else if (eraseArg == MMC_SECURE_TRIMARG1)
        {
        eraseArg = MMC_SECURE_TRIMARG2;
        rc = emmcCmdSetEraseGrp(pDev, eraseArg);
        }

    return (rc);
    }

/*******************************************************************************
*
* emmcHpiInterrput - send HPI command to device and check status
*
* This routine sends HPI command to device and check status.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcHpiInterrput
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMC_CARD_CTRL * pDrvCtrl;
    STATUS rc = OK;
    UINT32 rspValue = 0;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if (!pDrvCtrl->extCsdInfo.hpiSupport)
        return (ERROR);

    rc = sdCmdSendStatus (pDev, &rspValue);
    if (rc == ERROR)
        {
        MMC_CARD_DBG (MMC_CARD_DBG_INIT, "emmcHpiInterrput - first get status fault\n",
                      0, 0, 0, 0, 0, 0);
        return (ERROR);
        }
    if (CARD_STS_CUR_GET(rspValue) == CARD_STS_PRG)
        {
        do
            {

            /*
             * We need check SD return status to know the HPI 
             * command is finished or not.
             */

            rc = emmcSendHpi (pDev, pDrvCtrl->extCsdInfo.hpiCmdSet, &rspValue);
            if (rc == ERROR)
                {
                MMC_CARD_DBG (MMC_CARD_DBG_INIT, "emmcHpiInterrput - emmcSendHpi Cmd fault\n",
                              0, 0, 0, 0, 0, 0);
                return (ERROR);
                }

            rc = sdCmdSendStatus (pDev, &rspValue);
            if (rc == ERROR)
                {
                MMC_CARD_DBG (MMC_CARD_DBG_INIT, "emmcHpiInterrput - sdCmdSendStatus fault\n",
                              0, 0, 0, 0, 0, 0);
                return (ERROR);
                }
            } while (CARD_STS_CUR_GET(rspValue) == CARD_STS_PRG);
        }
    else
        {
        MMC_CARD_DBG (MMC_CARD_DBG_INIT, "emmcHpiInterrput - the status is not in program \n",
                      0, 0, 0, 0, 0, 0);
        return (OK);
        }
    return (rc);
    }
#endif

/*******************************************************************************
*
* emmcStorageBlkRead - read block from MMC card
*
* This routine reads block from MMC card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageBlkRead
    (
    void * pDrv,
    XBD_DATA * pXbdData
    )
    {
    SD_HARDWARE * pSdHardWare;
    STATUS rc;
    ULONG ix, round, remain;
    void *bufAddr;
    sector_t blkAddr;
    MMC_CARD_CTRL * pDrvCtrl;
    SD_HOST_SPEC * pHostSpec;
    VXB_DEV_HANDLE pDev;
    
    pDev = (VXB_DEV_HANDLE)pDrv;
    if (pDev == NULL)
        return (ERROR);

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        MMC_CARD_DBG (MMC_CARD_DBG_RW,
                      "emmcStorageBlkRead() - pSdHardWare is NULL\n",
                      0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pHostSpec = (SD_HOST_SPEC *)(pSdHardWare->pHostSpec);
    if (pHostSpec == NULL)
        return ERROR;

    /* split the transfer */

    round = pXbdData->numBlks / MMC_MAX_RW_SECTORS;
    remain = pXbdData->numBlks % MMC_MAX_RW_SECTORS;

    MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRead() -  round = %d\n"
                 "remain = %d \n",
                 round, remain, 0, 0, 0, 0);

    for (ix = 0; ix < round; ix ++)
        {
        blkAddr = pXbdData->blkNum + ix * MMC_MAX_RW_SECTORS;
        bufAddr = (void *)((ULONG)pXbdData->buffer + ix * MMC_MAX_RW_SECTORS *
                           pSdHardWare->blockSize);
        MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRead() -  blkAddr = 0x%x\n"
                     "bufAddr = 0x%x \n",
                     blkAddr, bufAddr, 0, 0, 0, 0);

        rc = emmcCmdSetBlockNum(pDev, MMC_MAX_RW_SECTORS);
        if (rc == ERROR)
            return (rc);  
        rc = emmcCmdReadBlock (pDev, bufAddr, (UINT64)blkAddr,
                             	 MMC_MAX_RW_SECTORS, pDrvCtrl->highCapacity);
        if (rc == ERROR)
            return ERROR;

        if (pHostSpec->cardCheck)
            {
            rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
            if (rc == ERROR)
                return ERROR;
            }
        }

     if (remain != 0)
         {
         blkAddr = pXbdData->blkNum + round * MMC_MAX_RW_SECTORS;
         bufAddr = (void *)((ULONG)pXbdData->buffer +
                            round * MMC_MAX_RW_SECTORS *
                            pSdHardWare->blockSize);
         MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRead() -  blkAddr = 0x%x\n"
                      "bufAddr = 0x%x \n",
                      blkAddr, bufAddr, 0, 0, 0, 0);

        if (remain > 1)
            {
            rc = emmcCmdSetBlockNum(pDev, (UINT32)remain);
            if (rc == ERROR)
				{
				return (rc);  
				}
            }
         rc = emmcCmdReadBlock (pDev, bufAddr, (UINT64)blkAddr,
                              (UINT32)remain, pDrvCtrl->highCapacity);
         if (rc == ERROR)
         {
             return ERROR;
         }

        if (pHostSpec->cardCheck)
            {
            rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
            if (rc == ERROR)
            {
                return ERROR;
            }
            }
         }

    return OK;
    }

/*******************************************************************************
*
* emmcStorageBlkWrite - write block to SD/MMC card
*
* This routine writes block to SD/MMC card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageBlkWrite
    (
    void * pDrv,
    XBD_DATA * pXbdData
    )
    {
    SD_HARDWARE * pSdHardWare;
    STATUS rc;
    ULONG ix, round, remain;
    void *bufAddr;
    sector_t blkAddr;
    MMC_CARD_CTRL * pDrvCtrl;
    VXB_DEV_HANDLE pDev;

    pDev = (VXB_DEV_HANDLE)pDrv;
    if (pDev == NULL)
        return (ERROR);

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    if (pDrvCtrl->tmpWp == TRUE || pDrvCtrl->permWp == TRUE)
        return ERROR;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        MMC_CARD_DBG (MMC_CARD_DBG_RW,
                      "emmcStorageBlkWrite() - pSdHardWare is NULL\n",
                      0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    /* split the transfer */

    round = pXbdData->numBlks / MMC_MAX_RW_SECTORS;
    remain = pXbdData->numBlks % MMC_MAX_RW_SECTORS;

    MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkWrite() -  round = %d\n"
              "remain = %d \n",
               round, remain, 0, 0, 0, 0);

    for (ix = 0; ix < round; ix ++)
        {
        bufAddr = (void *)((ULONG)pXbdData->buffer + ix * MMC_MAX_RW_SECTORS *
                           pSdHardWare->blockSize);
        blkAddr = pXbdData->blkNum + ix * MMC_MAX_RW_SECTORS;

        MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRead() -  blkAddr = 0x%x\n"
                     "bufAddr = 0x%x \n",
                     blkAddr, bufAddr, 0, 0, 0, 0);

        rc = emmcCmdSetBlockNum(pDev, MMC_MAX_RW_SECTORS);
        if (rc == ERROR)
            return (rc);  
        rc = emmcCmdWriteBlock (pDrvCtrl->pInst, bufAddr, (UINT64)blkAddr,
                              MMC_MAX_RW_SECTORS, pDrvCtrl->highCapacity);
        if (rc == ERROR )
            return ERROR;

        rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
        if (rc == ERROR)
            return ERROR;
        }

    if (remain != 0)
        {
        bufAddr = (void *)((ULONG)pXbdData->buffer + round * MMC_MAX_RW_SECTORS *
                            pSdHardWare->blockSize);
        blkAddr = pXbdData->blkNum + round * MMC_MAX_RW_SECTORS;

        MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRead() -  blkAddr = 0x%x\n"
                     "bufAddr = 0x%x \n",
                     blkAddr, bufAddr, 0, 0, 0, 0);

        if (remain > 1)
            {
            rc = emmcCmdSetBlockNum(pDev, (UINT32)remain);
            if (rc == ERROR)
                return (rc);  
            }
        rc = emmcCmdWriteBlock (pDrvCtrl->pInst, bufAddr, (UINT64)blkAddr,
                              (UINT32)remain, pDrvCtrl->highCapacity);
        if (rc == ERROR )
            return ERROR;

        rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
        if (rc == ERROR)
            return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* emmcStorageBlkRelWrite - Reliable write block to SD/MMC card
*
* This routine writes block to SD/MMC card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageBlkRelWrite
    (
    void * pDrv,
    XBD_DATA * pXbdData
    )
    {
    SD_HARDWARE * pSdHardWare;
    STATUS rc;
    ULONG ix, round, remain;
    void *bufAddr;
    sector_t blkAddr;
    MMC_CARD_CTRL * pDrvCtrl;
    VXB_DEV_HANDLE pDev;
    UINT32 arg;
    UINT8 enRelWr;

    pDev = (VXB_DEV_HANDLE)pDrv;
    if (pDev == NULL)
        return (ERROR);

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return (ERROR);

    if (pDrvCtrl->tmpWp == TRUE || pDrvCtrl->permWp == TRUE)
        return ERROR;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);

    if (pSdHardWare == NULL)
        {
        MMC_CARD_DBG (MMC_CARD_DBG_RW,
                      "emmcStorageBlkRelWrite() - pSdHardWare is NULL\n",
                      0, 0, 0, 0, 0, 0);
        return ERROR;
        }
	
    enRelWr = (UINT8)((pDrvCtrl->extCsdInfo.relParam >> 2) & 0x1);
    MMC_CARD_DBG (MMC_CARD_DBG_RW,
                  "emmcStorageBlkRelWrite() - enRelWr:%d relParam:0x%x\n",
                  enRelWr, pDrvCtrl->extCsdInfo.relParam, 0, 0, 0, 0);
	
    /* legacy implementation for reliable write */
    if (enRelWr == 0)
    	{
        /* set reliable write flag and block count */
        arg = (1 << 31 | 1);
		
        for (ix = 0; ix < pXbdData->numBlks; ix ++)
        	{
            bufAddr = (void *)((ULONG)pXbdData->buffer + ix *
                           pSdHardWare->blockSize);
            blkAddr = pXbdData->blkNum + ix;
			
            MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRelWrite() -  blkAddr = %ld\n"
                         "bufAddr = 0x%x \n",
                         blkAddr, bufAddr, 0, 0, 0, 0);
			
			rc = emmcCmdSetBlockNum(pDev, arg);
			if (rc == ERROR)
				return (rc);  
			rc = emmcCmdWriteBlock (pDrvCtrl->pInst, bufAddr, (UINT64)blkAddr,
								  1, pDrvCtrl->highCapacity);
			if (rc == ERROR )
				return ERROR;
			
			rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
			if (rc == ERROR)
				return ERROR;
            }
        }
    else  /* enhance implementation for reliable write */
        {
        /* split the transfer */
        round = pXbdData->numBlks / MMC_MAX_RW_SECTORS;
        remain = pXbdData->numBlks % MMC_MAX_RW_SECTORS;
		
		MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRelWrite() -  round = %d\n"
				  "remain = %d \n",
				   round, remain, 0, 0, 0, 0);

		/* set reliable write flag and block count */
        arg = (1 << 31 | MMC_MAX_RW_SECTORS);

        for (ix = 0; ix < round; ix ++)
            {
            bufAddr = (void *)((ULONG)pXbdData->buffer + ix * MMC_MAX_RW_SECTORS *
                               pSdHardWare->blockSize);
            blkAddr = pXbdData->blkNum + ix * MMC_MAX_RW_SECTORS;

            MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRelWrite() -  blkAddr = %ld\n"
                         "bufAddr = 0x%x \n",
                         blkAddr, bufAddr, 0, 0, 0, 0);      

            rc = emmcCmdSetBlockNum(pDev, arg);
            if (rc == ERROR)
                return (rc);  
            rc = emmcCmdWriteBlock (pDrvCtrl->pInst, bufAddr, (UINT64)blkAddr,
                                  MMC_MAX_RW_SECTORS, pDrvCtrl->highCapacity);
            if (rc == ERROR )
                return ERROR;

            rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
            if (rc == ERROR)
                return ERROR;			
            }

		if (remain != 0)
			{
			bufAddr = (void *)((ULONG)pXbdData->buffer + round * MMC_MAX_RW_SECTORS *
								pSdHardWare->blockSize);
			blkAddr = pXbdData->blkNum + round * MMC_MAX_RW_SECTORS;
		
			MMC_CARD_DBG(MMC_CARD_DBG_RW, "emmcStorageBlkRelWrite() -  blkAddr = %ld\n"
						 "bufAddr = 0x%x \n",
						 blkAddr, bufAddr, 0, 0, 0, 0);

            /* set reliable write flag and block count */
            arg = (UINT32)((UINT32)(1 << 31) | remain);

		    if (remain > 1)
		        {
				rc = emmcCmdSetBlockNum(pDev, arg);
				if (rc == ERROR)
				    return (rc);  
                }
            rc = emmcCmdWriteBlock (pDrvCtrl->pInst, bufAddr, (UINT64)blkAddr,
                                  (UINT32)remain, pDrvCtrl->highCapacity);
            if (rc == ERROR )
                return ERROR;
		
            rc = emmcStatusWaitReadyForData(pDrvCtrl->pInst);
            if (rc == ERROR)
                return ERROR;
			}
        }

    return OK;
    }

/*******************************************************************************
*
* emmcStorageInstUnlink - unmount emmc memory card from filesystem and XBD layer
*
* This routine unmount emmc memory card from filesystem and XBD layer.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageInstUnlink
    (
    VXB_DEV_HANDLE pDev
    )
    {
    STATUS rc;
    MMC_CARD_CTRL * pDrvCtrl;

    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    pDrvCtrl->attached = FALSE;

    pDrvCtrl->xbdDev.xbdInserted = FALSE;
    rc = emmcXbdDevDelete (&(pDrvCtrl->xbdDev));
    emmcCardIdxFree (pDrvCtrl);
    vxbMemFree (pDrvCtrl->pInfo);
    vxbMemFree (pDrvCtrl);
    vxbDevSoftcSet(pDev, NULL);

    return (rc);
    }

/*******************************************************************************
*
* emmcStorageShutdown - shutdown emmc memory card
*
* This routine shutdown emmc memory card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcStorageShutdown
    (
    VXB_DEV_HANDLE pDev
    )
    {

    return OK;
    }

/*******************************************************************************
*
* emmcCardIdxAlloc - allocate a global card index
*
* This routine allocates a global card index.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS emmcCardIdxAlloc
    (
    MMC_CARD_CTRL * pDrvCtrl
    )
    {
    UINT32 * cardIdx;
    int idx;

    cardIdx = &emmcDevIdx;

    for (idx = 0; idx < SDMMC_CARD_MAX_NUM; idx++)
        {
        if (!((*cardIdx) & (1 << idx)))
            {
            (*cardIdx) |= (1 << idx);
            break;
            }
        }

    if (idx == SDMMC_CARD_MAX_NUM)
        {
        return ERROR;
        }
    else
        {
        pDrvCtrl->idx = idx;
        return OK;
        }
    }

/*******************************************************************************
*
* emmcCardIdxFree - free a global card index
*
* This routine frees a global card index.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void emmcCardIdxFree
    (
    MMC_CARD_CTRL * pDrvCtrl
    )
    {
    UINT32 * cardIdx;

    cardIdx = &emmcDevIdx;
    (*cardIdx) &= ~(1 << pDrvCtrl->idx);
    }

/*******************************************************************************
*
* emmcStorageIoctl - XBD block device ioctl routine
*
* This routine handles a yet to completed list of ioctl calls.
*
* RETURN: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int emmcStorageIoctl
    (
    void *   pDrv,
    int      cmd,
    void *   arg
    )
    {
    int rc = OK;
    VXB_DEV_HANDLE pDev;
    MMC_CARD_CTRL * pDrvCtrl;
    SECTOR_RANGE  * pRange;

    if (pDrv == NULL)
        return (ERROR);

    pDev = (VXB_DEV_HANDLE)pDrv;
    pDrvCtrl = (MMC_CARD_CTRL *)GET_DRVCTRL (pDev);
    if (pDrvCtrl == NULL)
        return ERROR;

    switch (cmd)
        {
        case XBD_GETGEOMETRY:
            {
            XBD_GEOMETRY * geo;
            geo = (XBD_GEOMETRY *)arg;
            if (geo == NULL)
                return ERROR;

            geo->total_blocks = (sector_t)(pDrvCtrl->capacity / SDMMC_BLOCK_SIZE);
            geo->blocksize = SDMMC_BLOCK_SIZE;

            /* setup dummy CHS information */

            geo->heads = 1;
            geo->secs_per_track = 1;
            geo->cylinders = (pDrvCtrl->capacity / SDMMC_BLOCK_SIZE);
            rc = OK;
            break;
            }
        case XBD_TEST:
            {
            if (!pDrvCtrl->attached)
                rc = ERROR;
            else
                rc = OK;

            break;
            }

        case XBD_GETINFO:
            {
            XBD_INFO * pInfo;

            pInfo = (XBD_INFO *)arg;

            if (pDrvCtrl->isWp == TRUE)
                pInfo->properties |= XBD_INFO_READONLY;

            rc = OK;
            break;
            }
        case FIODISCARDGET:
            *(int *)arg = 0;
            break;
        case FIODISCARD:       /* file system layer is discarding sectors */
            pRange = (SECTOR_RANGE *)arg;
            if (pRange == NULL)
                return (ERROR);
            rc = emmcErase (pDev, pRange);
            break;
        case XBD_SYNC:
            {
            rc = emmcCacheFlush(pDev);
            break;
            }
        default:
            {
            rc = ENOTSUP;
            break;
            }
        }

    return (rc);
    }
