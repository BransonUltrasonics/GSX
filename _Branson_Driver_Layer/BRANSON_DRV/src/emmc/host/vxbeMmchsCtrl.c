/* vxbeMmchsCtrl.c - MMCHS driver */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
13aug18,r_r  create, add eMMC support. Driver adapted from Windriver SD Card driver
*/

/*
DESCRIPTION

This is the vxbus compliant MMC/SD/SDIO Host Controller (MMCHS)
driver which implements the functionality specific to the TI SOCs.

The MMCHS provides an interface between the host system and SD/MMC memory cards.

To add the driver to the vxWorks image, add the following component to the
kernel configuration.

\cs
vxprj component add DRV_EMMCHS_CTRL
\ce

VXWORK-7 MODE
This driver is bound to device tree, and the device tree node must specify
below parameters:

\is

\i <comptible>
This parameter specifies the name of the MMCHS controller driver.
It must be "ti,mmchs".

\i <reg>
This parameter specifies the register base address and length of this module.

\i <interrupts>
This parameter specifies the interrupt number of this module.

\i <interrupt-parent>
This parameter specifies the offset of interrupt controller.

\i <pinmux-0>
This parameter specifies the list of phandles that each pointing at a PinMux
configuration node.

\i <clocks>
This parameter specifies the list of clocks that this module use.

\i <clock-names>
This parameter specifies the name of every clock in clocks list.

\i <dma-mode>
This parameter specifies the mode of data transfer, 0 means using SDMA,
and 1 means using PIO mode.

\i <cd-pin>
This parameter specifies the card detect GPIO pin index.

\i <wp-pin>
This parameter specifies the write protect GPIO pin index.
Using 0xffff means the target device is always writeable.

\i <debounce-time>
This parameter specifies the debounce time for the card detect GPIO pin.

\ie

An example of device node is show below:
\cs
    sdmmc0@48060000
        {
        #address-cells = <1>;
        #size-cells = <0>;
        compatible = "ti,mmchs";
        reg = <0x48060000 0x1000>;
        interrupts = <64>;
        interrupt-parent = <&intc>;
        pinmux-0 = <&mmc0_pads>;
        clocks = <&clk 143>,<&clk 144>,<&clk 145>;
        clock-names = "mmc_clk","ick","fck";
        dma-mode = <0>;
        cd-pin = <6>;
        wp-pin = <0xffff>;
        debounce-time = <7936>;
        };
\ce

SEE ALSO: vxBus
\tb "AM572x_TRM"
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>

#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/gpio/vxbGpioLib.h>
#include <subsys/dma/vxbDmaLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>

#include <vxbeMmcLib.h>
#include <vxbeMmchsCtrl.h>

/* defines */
//#define EMMCHS_DBG_ON

#ifdef  EMMCHS_DBG_ON

#   ifdef  LOCAL
#   undef  LOCAL
#   define LOCAL
#   endif

#   define MMCHS_DBG_IRQ            0x00000001
#   define MMCHS_DBG_RW             0x00000002
#   define MMCHS_DBG_XBD            0x00000004
#   define MMCHS_DBG_ERR            0x00000008
#   define MMCHS_DBG_ALL            0xffffffff
#   define MMCHS_DBG_OFF            0x00000000

UINT32 eMmchsDbgMask = MMCHS_DBG_OFF;

IMPORT FUNCPTR _func_logMsg;

#   define EMMCHS_DBG(mask, string, a, b, c, d, e, f)           \
        if ((eMmchsDbgMask & mask) || (mask == MMCHS_DBG_ALL))  \
			if ( _func_logMsg != NULL)							\
				(* _func_logMsg)(string, a, b, c, d, e, f)
#else
#   define EMMCHS_DBG(mask, string, a, b, c, d, e, f)
#endif  /* EMMCHS_DBG_ON */

/* forward declarations */

LOCAL STATUS eMmchsCtrlProbe  (VXB_DEV_HANDLE);
LOCAL STATUS eMmchsCtrlAttach (VXB_DEV_HANDLE);

LOCAL STATUS eMmchsInstConnect (VXB_DEV_HANDLE);
LOCAL STATUS eMmchsWaitStatus (VXB_DEV_HANDLE, UINT32, UINT32, BOOL);
LOCAL void eMmchsHardwareInit (VXB_DEV_HANDLE);
LOCAL void eMmchsDevInit (VXB_DEV_HANDLE);

LOCAL void eMmchsIsr (VXB_DEV_HANDLE);
LOCAL void eMmchsCardMonTask (VXB_DEV_HANDLE);

LOCAL void eMmchsCmdPrepare (VXB_DEV_HANDLE, SD_CMD *);
LOCAL STATUS eMmchsCmdIssue (VXB_DEV_HANDLE, SD_CMD *);
LOCAL STATUS eMmchsClkFreqSetup (VXB_DEV_HANDLE, UINT32);
LOCAL STATUS eMmchsBusWidthSetup (VXB_DEV_HANDLE, UINT32);
LOCAL BOOL eMmchsWpCheck (VXB_DEV_HANDLE);

LOCAL STATUS eMmchsPioRead (VXB_DEV_HANDLE);
LOCAL STATUS eMmchsPioWrite (VXB_DEV_HANDLE);

LOCAL STATUS eMmchsSpecInfoGet (VXB_DEV_HANDLE, void **, VXB_SD_CMDISSUE_FUNC *);

LOCAL VXB_DRV_METHOD vxbeMmchsCtrl_methods[] =
    {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), eMmchsCtrlProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), eMmchsCtrlAttach },
    { VXB_DEVMETHOD_CALL(vxbSdspecinfoGet), eMmchsSpecInfoGet},
    VXB_DEVMETHOD_END
    };

typedef struct eMmchsCfg
        {
        UINT32    flag;       /* config flag */
        }TIMMCHS_CFG;

LOCAL const TIMMCHS_CFG  eMmchsCfg =
    {
    0
    };

#define  FIRST_INIT    0

LOCAL const TIMMCHS_CFG eMmchsEmmcCfg =
   {
   FIRST_INIT
   };

VXB_DRV vxbeMmchsCtrlDrv =
    {
    { NULL } ,
    "eMmchs",  					/* Name */
    "Branson MMCHS driver",		/* Description */
    VXB_BUSID_FDT,				/* Class */
    0,							/* Flags */
    0,							/* Reference count */
    vxbeMmchsCtrl_methods		/* Method table */
    };

LOCAL const VXB_FDT_DEV_MATCH_ENTRY eMmchsMatch[] =
    {
        {
            "branson, emmchs",
       	    (void *)&eMmchsEmmcCfg
       	}, 
       	{} /* Empty terminated list */
    };

VXB_DRV_DEF(vxbeMmchsCtrlDrv)

/* externs */

IMPORT UCHAR erfLibInitialized;
IMPORT void vxbUsDelay (int);
IMPORT void bzero (char *, int);

/******************************************************************************
*
* eMmchsCtrlProbe - probe TI MMCHS host controller
*
* Check for TI MMCHS (or compatible) device at the specified base
* address.  We assume one is present at that address, but
* we need to verify.
*
* RETURNS: OK if probe passes and assumed a valid for TI MMCHS
*             (or compatible) device.
*          ERROR otherwise.
*
* ERRNO: N/A 
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsCtrlProbe
    (
    struct vxbDev * pDev /* Device information */
    )
    {
    VXB_FDT_DEV_MATCH_ENTRY *pMatch;
    TIMMCHS_CFG * pCfg;
    STATUS rc;

    rc = vxbFdtDevMatch (pDev, eMmchsMatch, &pMatch);
    if (rc == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Vxb Fdt Mmchs Probe failed\n",
                     0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pCfg = (TIMMCHS_CFG *)pMatch->data;
    if (pCfg != NULL)
        {
        vxbDevDrvDataSet(pDev,(void *)pCfg);
        }

    return OK;
    }

/*******************************************************************************
*
* eMmchsCardDetect - Card detect hook function
*
* This routine performs card detect operation. For mmchs host controller,
* GPIO operation should be employed to detect the card state.
*
* RETURNS: TRUE if a card be detected, or FALSE if no card.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL BOOL eMmchsCardDetect
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32           value;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);
    if (pDrvCtrl == NULL)
        {
        return FALSE;
        }

    if (GPIO_EXISTS_PIN == pDrvCtrl->cdGpioPin)
        {
        return TRUE;
        }

    if (pDrvCtrl->cdGpioPin == GPIO_NULL_PIN)
        {
        return FALSE;
        }

    value = vxbGpioGetValue (pDrvCtrl->cdGpioPin);
    if (value == GPIO_VALUE_LOW)
        {
        return TRUE;
        }
    else
        {
        return FALSE;
        }
    }

/*******************************************************************************
*
* eMmchsWPDetect - Write protect detect hook function
*
* This routine performs write protect detect operation.
* For mmchs host controller, GPIO operation should be employed to
* detect the write protect state.
*
* RETURNS: TRUE if the card is write protect, or FALSE if is writeable.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL BOOL eMmchsWPDetect
    (
    VXB_DEV_HANDLE pDev
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32           value;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);
    if (pDrvCtrl == NULL)
        {
        return TRUE;
        }

    if (pDrvCtrl->wpGpioPin == GPIO_NULL_PIN)
        {
        return FALSE;
        }

    value = vxbGpioGetValue (pDrvCtrl->wpGpioPin);
    if (value == GPIO_VALUE_HIGH)
        {
        return TRUE;
        }
    else
        {
        return FALSE;
        }
    }

/*******************************************************************************
*
* eMmchsCardPowerOn - Power on the card hook function
*
* This routine performs power on operation for target card.
*
* RETURNS: TRUE if the power of card on.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL BOOL eMmchsCardPowerOn
    (
    VXB_DEV_HANDLE pDev
    )
    {
    UINT32 value;

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value |= MMCHS_HCTL_SDBP_MASK;
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);

    do
        {
        value = CSR_READ_4 (pDev, MMCHS_HCTL);
        vxbUsDelay(1000);
        } while (!(value & MMCHS_HCTL_SDBP_MASK));

    return TRUE;
    }

/*******************************************************************************
*
* eMmchsCdIsr - Card detect Isr hook function
*
* This routine respond the interrupt of card-detect.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsCdIsr
    (
    MMCHS_DRV_CTRL * pDrvCtrl
    )
    {
    (void)semGive (pDrvCtrl->devChange);
    }

/*******************************************************************************
*
* eMmchsDmaChanFree - Dma channel free hook function
*
* This routine performs tx and rx dma channel free.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsDmaChanFree
    (
    void * pCookie
    )
    {
    VXB_DEV_HANDLE   pDev;
    MMCHS_DRV_CTRL * pDrvCtrl;

    pDev = (VXB_DEV_HANDLE) pCookie;
    if (pDev)
        {
        pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);
        vxbDmaChanFree (pDrvCtrl->dmaTxRes);
        vxbDmaChanFree (pDrvCtrl->dmaRxRes);
        pDrvCtrl->dmaTxRes = NULL;
        pDrvCtrl->dmaRxRes = NULL;
        }

    return OK;
    }

/*******************************************************************************
*
* eMmchsDmaChanAlloc - Dma channel allocate hook function
*
* This routine performs tx and rx dma channel allocation.
*
* RETURNS: OK if allocate success or ERROR if failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsDmaChanAlloc
    (
    void * pCookie
    )
    {
    MMCHS_DRV_CTRL *     pDrvCtrl;
    VXB_DEV_HANDLE       pDev;

    pDev     = (VXB_DEV_HANDLE) pCookie;
    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    if (pDrvCtrl->dmaTxRes != NULL || pDrvCtrl->dmaRxRes != NULL)
        {
        return OK;
        }

    pDrvCtrl->dmaTxRes = vxbDmaChanAlloc(0);
    pDrvCtrl->dmaRxRes = vxbDmaChanAlloc(0);

    if (pDrvCtrl->dmaTxRes == NULL || pDrvCtrl->dmaRxRes == NULL)
        {
        (void)eMmchsDmaChanFree (pCookie);
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* eMmchsDmaCallback - Dma channel Isr callback function
*
* This routine respond tx and rx dma isr.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsDmaCallback
    (
    void * pArg
    )
    {
    SD_CMD * pSdCmd = pArg;

    if (pSdCmd->cmdData.isRead && pSdCmd->hasData)
        {
        CACHE_USER_INVALIDATE (pSdCmd->cmdData.buffer,
                               pSdCmd->cmdData.blkSize * pSdCmd->cmdData.blkNum);
        }
  
    (void)semGive(pSdCmd->dmaDone);
    }

/*******************************************************************************
*
* eMmchsDmaStart - Dma channel start a transaction hook function
*
* This routine start a tx or rx dma transaction.
*
* RETURNS: OK if start success or ERROR if failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsDmaStart
    (
    VXB_DEV_HANDLE pDev,
    SD_CMD * pSdCmd
    )
    {
    MMCHS_DRV_CTRL *     pDrvCtrl;
    VXB_DMA_SLAVE_CONFIG chanConfig;
    VXB_DMA_TX *         dmaTx;
    STATUS               rc;

    bzero ((char*)&chanConfig, sizeof(chanConfig));

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    if (pSdCmd->cmdData.isRead)
        {
        chanConfig.dmaEvent      = pDrvCtrl->dmaRxEvt;
        chanConfig.direction     = VXB_DMA_DEV_TO_MEM;
        chanConfig.srcAddrWidth  = sizeof (UINT32);
        chanConfig.srcMaxBurst   = pSdCmd->cmdData.blkSize;

        CACHE_USER_INVALIDATE (pSdCmd->cmdData.buffer,
                               pSdCmd->cmdData.blkSize * pSdCmd->cmdData.blkNum);

        rc = vxbDmaChanControl (pDrvCtrl->dmaRxRes,
                                VXB_DMA_CONFIG, &chanConfig);

        dmaTx = vxbDmaChanPreMemCpy (pDrvCtrl->dmaRxRes,
                                     (VIRT_ADDR)((UINT32)MMCHS_BAR(pDev) + MMCHS_DATA),
                                     (VIRT_ADDR)pSdCmd->cmdData.buffer,
                                     pSdCmd->cmdData.blkSize * pSdCmd->cmdData.blkNum,
                                     0);
        }
    else
        {
        chanConfig.dmaEvent      = pDrvCtrl->dmaTxEvt;
        chanConfig.direction     = VXB_DMA_MEM_TO_DEV;
        chanConfig.destAddrWidth = sizeof (UINT32);
        chanConfig.destMaxBurst  = pSdCmd->cmdData.blkSize;

        CACHE_USER_FLUSH (pSdCmd->cmdData.buffer,
                          pSdCmd->cmdData.blkSize * pSdCmd->cmdData.blkNum);
             
        rc = vxbDmaChanControl (pDrvCtrl->dmaTxRes,
                                VXB_DMA_CONFIG, &chanConfig);

        dmaTx = vxbDmaChanPreMemCpy (pDrvCtrl->dmaTxRes,
                                     (VIRT_ADDR)pSdCmd->cmdData.buffer,
                                     (VIRT_ADDR)((UINT32)MMCHS_BAR(pDev) + MMCHS_DATA),
                                     pSdCmd->cmdData.blkSize * pSdCmd->cmdData.blkNum,
                                     0);
        }

    dmaTx->fn   = (VXB_DMA_COMPLETE_FN) eMmchsDmaCallback;
    dmaTx->pArg = pSdCmd;

    rc = vxbDmaChanStart (dmaTx);

    return rc;
    }

/******************************************************************************
*
* eMmchsCtrlAttach - attach TI MMCHS host controller
*
* This is the TI MMCHS host controller initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsCtrlAttach
    (
    VXB_DEV_HANDLE pDev /* Device information */
    )
    {
    VXB_FDT_DEV *      pFdtDev;
    TIMMCHS_CFG *      pCfg;
    MMCHS_DRV_CTRL *   pDrvCtrl;
    VXB_RESOURCE_ADR * pResAdr = NULL;
    VXB_RESOURCE     * pRes;
    const void *       pValue;
    int                len;
    STATUS             rc;
    UINT32             fdtVal;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbMemAlloc(sizeof(MMCHS_DRV_CTRL));
    if (pDrvCtrl == NULL)
        {
        return ERROR;
        }
    bzero ((char *)pDrvCtrl, sizeof(MMCHS_DRV_CTRL));

    vxbDevSoftcSet(pDev, pDrvCtrl);

    pFdtDev = vxbFdtDevGet(pDev);
    if (pFdtDev == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "vxbFdtDevGet() failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pValue = vxFdtPropGet(pFdtDev->offset, "dma-mode", &len);
    if(pValue == NULL)
        goto errHandle;

    pDrvCtrl->dmaMode = vxFdt32ToCpu(* (UINT32*)pValue);

    rc = vxbClkEnableAll (pDev);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "vxbClkEnableAll() failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    rc = vxbPinMuxEnable(pDev);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "vxbPinMuxEnable() failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Alloc memory resource failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if (pResAdr == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Alloc memory resource failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->regHandle = ((VXB_RESOURCE_ADR *)pRes->pRes)->pHandle;
    pDrvCtrl->regBase   = (void *)((char*)pResAdr->virtual
                                    + MMCHS_REG_BASE_OFFSET);

    pValue = vxFdtPropGet(pFdtDev->offset, "dma-rxevt", &len);
    if (pValue == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "No dma Rx Event for SDcard in DTS\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->dmaRxEvt = vxFdt32ToCpu(*(UINT32 *)pValue);

    pValue = vxFdtPropGet(pFdtDev->offset, "dma-txevt", &len);
    if (pValue == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "No dma Tx Event for SDcard in DTS\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->dmaTxEvt = vxFdt32ToCpu(*(UINT32 *)pValue);

    pValue = vxFdtPropGet(pFdtDev->offset, "cd-pin", &len);
    if (pValue == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "No card detect pin in DTS\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->cdGpioPin = vxFdt32ToCpu(*(UINT32 *)pValue);

    pValue = vxFdtPropGet(pFdtDev->offset, "wp-pin", &len);
    if (pValue == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "No write protect pin in DTS\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->wpGpioPin = vxFdt32ToCpu(*(UINT32 *)pValue);

    /* For on-board eMMC chip, we do not need cd pin. */
    if (GPIO_EXISTS_PIN == pDrvCtrl->cdGpioPin)
        {
        goto cdFinished;
        }

    rc = vxbGpioAlloc (pDrvCtrl->cdGpioPin);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Card detect pin alloc failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    rc = vxbGpioSetDir (pDrvCtrl->cdGpioPin, GPIO_DIR_INPUT);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Card detect pin set direction failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pValue = vxFdtPropGet(pFdtDev->offset, "debounce-time", &len);
    if (pValue != NULL)
        {
        fdtVal = vxFdt32ToCpu(*(UINT32*)pValue);
        }
    else
        {
        fdtVal = 2000;
        }
    rc = vxbGpioSetDebounce (pDrvCtrl->cdGpioPin, fdtVal);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Card detect pin set debounce failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }
    rc  = vxbGpioIntConfig (pDrvCtrl->cdGpioPin,
                            INTR_TRIGGER_EDGE, INTR_POLARITY_BOTH);
    rc |= vxbGpioIntConnect(pDrvCtrl->cdGpioPin, eMmchsCdIsr,(void *)pDrvCtrl);
    rc |= vxbGpioIntEnable(pDrvCtrl->cdGpioPin, eMmchsCdIsr,(void *)pDrvCtrl);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "Card detect interrupt configure failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

cdFinished:
    pRes = vxbResourceAlloc(pDev, VXB_RES_IRQ, 0);
    if (pRes == NULL)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "SDHC interrupt alloc failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    pDrvCtrl->intRes = pRes;

    pDrvCtrl->cardDetect  = eMmchsCardDetect;
    pDrvCtrl->cardWpCheck = eMmchsWPDetect;
    pDrvCtrl->cardPowerOn = eMmchsCardPowerOn;
    pDrvCtrl->dmaAlloc    = eMmchsDmaChanAlloc;
    pDrvCtrl->dmaFree     = eMmchsDmaChanFree;
    pDrvCtrl->dmaStart    = eMmchsDmaStart;

    /* internal buffer data port is little endian */

    pDrvCtrl->pioHandle = pDrvCtrl->regHandle;

    pCfg = (TIMMCHS_CFG *)vxbDevDrvDataGet(pDev);

    pDrvCtrl->flags = pCfg->flag;

    (void)eMmchsDevInit (pDev);

    rc = eMmchsInstConnect (pDev);
    if (rc != OK)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "SDHC device connect failed\n",
                                      0, 0, 0, 0, 0, 0);
        goto errHandle;
        }

    return (OK);

errHandle:
	EMMCHS_DBG (MMCHS_DBG_ERR, "eMmchsCtrlAttach GOTO ERROR\n",
                               0, 0, 0, 0, 0, 0);
    (void)vxbGpioFree (pDrvCtrl->cdGpioPin);
    vxbMemFree (pDrvCtrl);
    return ERROR;
    }

/*******************************************************************************
*
* eMmchsInstConnect - third level initialization routine of TI Mmchs
*
* This routine performs the third level initialization of the TI Mmchs.
*
* RETURNS: OK or ERROR if failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsInstConnect
    (
    VXB_DEV_HANDLE   pInst
    )
    {
    FUNCPTR cardMon;
    MMCHS_DRV_CTRL * pDrvCtrl;
    STATUS rc = OK;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet (pInst);

    /* create a monitor task that handles card state change */

    cardMon = (FUNCPTR)eMmchsCardMonTask;

    rc = taskSpawn (MMCHS_CARD_MON_TASK_NAME, MMCHS_CARD_MON_TASK_PRI, 0,
                    MMCHS_CARD_MON_TASK_STACK, cardMon, (_Vx_usr_arg_t)pInst,
                    0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (rc == ERROR)
        return ERROR;

    /* setup the interrupt mask */

    pDrvCtrl->intMask = (IRQ_DATA | IRQ_CMD);
    pDrvCtrl->intMask |= IRQ_AC12E;

    if (pDrvCtrl->dmaMode == MMCHS_DMA_MODE_PIO)
        {
        pDrvCtrl->intMask |= (IRQ_BRR | IRQ_BWR);
        }

    CSR_WRITE_4 (pInst, MMCHS_IE, pDrvCtrl->intMask);

    /* connect and enable interrupt */

    rc = vxbIntConnect (pInst, pDrvCtrl->intRes, eMmchsIsr, pInst);
    if (rc == ERROR)
        return ERROR;
    rc = vxbIntEnable (pInst, pDrvCtrl->intRes);
    if (rc == ERROR)
        return ERROR;

    /* enable MMCHS interrupts */

    CSR_WRITE_4 (pInst, MMCHS_ISE, pDrvCtrl->intMask);

    return OK;
    }

/*******************************************************************************
*
* eMmchsWaitStatus - wait MMCHS register for the specified status
*
* This routine waits MMCHS register for the specified status.
*
* RETURNS: OK, or ERROR if timeout.
*
* ERRNO: N/A
*/

LOCAL STATUS eMmchsWaitStatus
    (
    VXB_DEV_HANDLE  pDev,
    UINT32          addr,
    UINT32          staMsk,
    BOOL            set
    )
    {
    UINT32          timeout = 0;
    UINT32          status;
    BOOL            stat;

    do  {
        status = CSR_READ_4 (pDev, addr);
        stat = set ^ ((status & staMsk) == 0);
        vxbUsDelay(1);
        if (stat)
            {
            return OK;
            }
        }while (timeout++ < MMCHS_TIMEOUT);

    EMMCHS_DBG (MMCHS_DBG_ERR,
                  "eMmchsWaitStatus: wait=%d, status=0x%x,"
                  "statMsk=0x%x,result=%d\n",
                  set, status, staMsk, stat, 0, 0);
    return ERROR;
    }

/*******************************************************************************
*
* eMmchsHardwareInit - initialize eMmchs chip
*
* This routine initializes eMmchs.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsHardwareInit
    (
    VXB_DEV_HANDLE   pDev
    )
    {
    volatile UINT32 value;

    /* HSMMC1 software reset */

    value = CSR_READ_4 (pDev, MMCHS_SYSCONFIG);
    value |= MMCHS_SYSCONFIG_SOFTRESET_MASK;
    CSR_WRITE_4 (pDev, MMCHS_SYSCONFIG, value);

    if (eMmchsWaitStatus (pDev, MMCHS_SYSSTATUS, 
                           MMCHS_SYSSTATUS_RESETDONE_MASK, TRUE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "reset done error\n",
                      0, 0, 0, 0, 0, 0);
        return ;    
        }

    /* HSMMC1 software reset for all */

    value = CSR_READ_4 (pDev, MMCHS_SYSCTL);
    value |= MMCHS_SYSCTL_SRA_MASK;
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, value);
    
    if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                           MMCHS_SYSCTL_SRA_MASK, FALSE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "reset all error\n",
                      0, 0, 0, 0, 0, 0);
        return ;    
        }

    /* 1.8V Supported, 3.0V supported */

    value = CSR_READ_4 (pDev, MMCHS_CAPA);
    value |= MMCHS_CAPA_VS30_MASK | MMCHS_CAPA_VS18_MASK;
    CSR_WRITE_4 (pDev, MMCHS_CAPA, value);

    /* SD bus voltage select, 3.0V */

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value &= ~MMCHS_HCTL_SDVS_MASK_3_3;
    value |= (MMCHS_HCTL_SDVS << MMCHS_HCTL_SDVS_SHIFT);
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);

    /* Debounce time for the card detect signal */

    value = CSR_READ_4 (pDev, MMCHS_CON);
    value &= ~MMCHS_CON_DVAL_MASK;
    value |= (MMCHS_CON_DVAL << MMCHS_CON_DVAL_SHIFT);
    CSR_WRITE_4 (pDev, MMCHS_CON, value);

    /* 1-bit Data width */

    CSR_CLRBIT_4 (pDev, MMCHS_CON, MMCHS_CON_DW8);

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value &= ~MMCHS_HCTL_DTW_MASK;
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);

    /* SD bus power off */

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value &= ~MMCHS_HCTL_SDBP_MASK;
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);

    /* Internal clock is stopped, clock is not provided to the card */

    value = CSR_READ_4 (pDev, MMCHS_SYSCTL);
    value &= ~(MMCHS_SYSCTL_ICE_MASK | \
               MMCHS_SYSCTL_CEN_MASK | MMCHS_SYSCTL_DTO_MASK);
    value |= (MMCHS_SYSCTL_DTO << MMCHS_SYSCTL_DTO_SHIFT);
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, value);

    /* Clock frequency select for initialization stream, enable clock */

    value = CSR_READ_4 (pDev, MMCHS_SYSCTL);
    value &= ~MMCHS_SYSCTL_CLKD_MASK;
    value |= MMCHS_SYSCTL_ICE_MASK | \
            (MMCHS_SYSCTL_CLKD << MMCHS_SYSCTL_CLKD_SHIFT);
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, value);

    /* Wait clock to be stable */
    
    if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                           MMCHS_SYSCTL_ICS_MASK, TRUE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "Wait clock to be stable error\n",
                      0, 0, 0, 0, 0, 0);
        return ;    
        }

    /* The clock is provided to the card */

    value = CSR_READ_4 (pDev, MMCHS_SYSCTL);
    value |= MMCHS_SYSCTL_CEN_MASK;
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, value);

    /* SD bus power on */

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value |= MMCHS_HCTL_SDBP_MASK;
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);
    
    if (eMmchsWaitStatus (pDev, MMCHS_HCTL, 
                           MMCHS_HCTL_SDBP_MASK, TRUE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "SD bus power on error\n",
                      0, 0, 0, 0, 0, 0);
        return ;    
        }

    /* Enable MMC1 ENAWAKEUP */

    value = CSR_READ_4 (pDev, MMCHS_SYSCONFIG);
    value |= MMCHS_SYSCONFIG_ENAWAKEUP_MASK;
    CSR_WRITE_4 (pDev, MMCHS_SYSCONFIG, value);

    /* MMC1_HCTL IWE */

    value = CSR_READ_4 (pDev, MMCHS_HCTL);
    value |= MMCHS_HCTL_IWE_MASK;
    CSR_WRITE_4 (pDev, MMCHS_HCTL, value);

    /* MMC1 SIDLEMODE CLOCKACTIVITY */

    value = CSR_READ_4 (pDev, MMCHS_SYSCONFIG);
    value |= (MMCHS_SYSCONFIG_SIDLEMODE << MMCHS_SYSCONFIG_SIDLEMODE_SHIFT) | \
             (MMCHS_SYSCONFIG_CLOCKACTIVITY << MMCHS_SYSCONFIG_CLOCKACTIVITY_SHIFT);
    CSR_WRITE_4 (pDev, MMCHS_SYSCONFIG, value);

    /* Enable MMC1 AUTOIDLE */

    value = CSR_READ_4 (pDev, MMCHS_SYSCONFIG);
    value |= MMCHS_SYSCONFIG_AUTOIDLE_MASK;
    CSR_WRITE_4 (pDev, MMCHS_SYSCONFIG, value);

    /* Enable interrupts */

    value = MMCHS_IE_EN_DEFUALT;
    CSR_WRITE_4 (pDev, MMCHS_IE, value);

    /* Send initialization stream */
    value = CSR_READ_4 (pDev, MMCHS_CON);
    value |= MMCHS_CON_INIT_MASK;
    CSR_WRITE_4 (pDev, MMCHS_CON, value);

    /* Send CMD0 */
    value = MMCHS_CMD0;
    CSR_WRITE_4 (pDev, MMCHS_CMD, value);

    /* Wait command to complete */    
    if (eMmchsWaitStatus (pDev, MMCHS_STAT, 
                           MMCHS_STAT_CC_MASK, TRUE) == ERROR)
        {
        return ;    
        }

    CSR_WRITE_4 (pDev, MMCHS_STAT, MMCHS_STAT_CC_MASK);

    /* Send CMD0 */
    value = MMCHS_CMD0;
    CSR_WRITE_4 (pDev, MMCHS_CMD, value);

    /* Wait command to complete */    
    if (eMmchsWaitStatus (pDev, MMCHS_STAT, 
                           MMCHS_STAT_CC_MASK, TRUE) == ERROR)
        {
        return ;    
        }
    CSR_WRITE_4 (pDev, MMCHS_STAT, value);

    /* Initialization stream finished */
    value = CSR_READ_4 (pDev, MMCHS_CON);
    value &= ~MMCHS_STAT_TC_MASK;
    CSR_WRITE_4 (pDev, MMCHS_CON, value);

    /* Clear MMCHS_STAT register */
    CSR_WRITE_4 (pDev, MMCHS_STAT, IRQ_ALL);

    /* Disable interrupt */
    CSR_WRITE_4 (pDev, MMCHS_IE, 0);
    
}

/*******************************************************************************
*
* eMmchsDevInit - the MMCHS per device specific initialization
*
* This routine performs per device specific initialization of MMCHS.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsDevInit
    (
    VXB_DEV_HANDLE   pInst
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32 hostCap;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet (pInst);

    /* initialize MMCHS hardware */

    eMmchsHardwareInit (pInst);

    /* get host controller version */

    pDrvCtrl->hcVer = CSR_READ_4 (pInst, MMCHS_REV) & 0xffff0000;

    /* get host controller capabilities */

    hostCap = CSR_READ_4 (pInst, MMCHS_CAPA);

    /* determind DMA mode supported */

    if (!(hostCap & CAPA_DMAS))
        {
        pDrvCtrl->dmaMode = MMCHS_DMA_MODE_PIO;
        }

    /* fill in SDMMC_HOST structure */

    pDrvCtrl->pDev = pInst;

    /* determine voltage supported */

    if (hostCap & CAPA_VS30)
        {
        pDrvCtrl->sdHostSpec.capbility |= (OCR_VDD_VOL_29_30 | OCR_VDD_VOL_30_31);
        }

    if (hostCap & CAPA_VS33)
        {
        pDrvCtrl->sdHostSpec.capbility |= (OCR_VDD_VOL_32_33 | OCR_VDD_VOL_33_34);
        }
    
    /* high capacity SD card is supported */

    pDrvCtrl->sdHostSpec.capbility |= OCR_CARD_CAP_STS;
    pDrvCtrl->sdHostSpec.vxbSdBusWidthSetup = eMmchsBusWidthSetup;
    pDrvCtrl->sdHostSpec.vxbSdCardWpCheck = eMmchsWpCheck;
    pDrvCtrl->sdHostSpec.vxbSdClkFreqSetup = eMmchsClkFreqSetup;
    pDrvCtrl->sdHostSpec.vxbSdResumeSet = NULL;
    pDrvCtrl->sdHostSpec.vxbSdVddSetup = NULL;

    /* determine high speed SD card supported */

    if (hostCap & CAPA_HSS)
        {
        pDrvCtrl->highSpeed = TRUE;
        }

    /*
     * The devChange semaphore is used by the interrupt service routine
     * to inform the card monitor task that a state change has occurred.
     */

    pDrvCtrl->devChange = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    pDrvCtrl->cmdDone = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    pDrvCtrl->dataDone = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    pDrvCtrl->dmaDone = semBCreate (SEM_Q_PRIORITY /*| SEM_EVENTSEND_ERR_NOTIFY*/, SEM_EMPTY);

    }

/*******************************************************************************
*
* eMmchsIsr - interrupt service routine of TI Mmchs
*
* This routine handles interrupts of TI MMCHS.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsIsr
    (
    VXB_DEV_HANDLE   pDev
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32 intSts;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet (pDev);

    intSts = CSR_READ_4 (pDev, MMCHS_STAT);
    pDrvCtrl->intSts = intSts;

    if (!intSts)
        {
        return;
        }

    if (intSts & IRQ_ERR)
        {
        if (intSts & (IRQ_CTOE | IRQ_CCE))
            {
            /*  MMC controller internal state machines reset */

            CSR_SETBIT_4 (pDev, MMCHS_SYSCTL, MMCHS_SYSCTL_SRC_MASK);
     
            if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                                   MMCHS_SYSCTL_SRC_MASK, TRUE) == ERROR)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR,
                              "wait reset assert error\n", 0, 0, 0, 0, 0, 0);
                }
     
            if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                                   MMCHS_SYSCTL_SRC_MASK, FALSE) == ERROR)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR,
                              "wait reset release error\n", 0, 0, 0, 0, 0, 0);
                }
            }
        }

    /* clear interrupts */

    CSR_WRITE_4 (pDev, MMCHS_STAT, intSts);

    /* command interrupts */

    if (intSts & IRQ_CMD)
        {
        if (intSts & IRQ_CC)
            {
            EMMCHS_DBG (MMCHS_DBG_IRQ, "cmd complete\n",
                          0, 0, 0, 0, 0, 0);
            }

        (void)semGive (pDrvCtrl->cmdDone);
        }

    /* data interrupts */

    if (intSts & IRQ_DATA)
        {
        if (intSts & IRQ_TC)
            {
            EMMCHS_DBG (MMCHS_DBG_IRQ, "CMD data complete\n",
                          0, 0, 0, 0, 0, 0);
            }

        (void)semGive (pDrvCtrl->dataDone);
        }

    /* PIO mode */

    if (intSts & IRQ_BRR)
        {
        EMMCHS_DBG (MMCHS_DBG_IRQ, "CMD buffer read ready\n",
                      0, 0, 0, 0, 0, 0);

        if (CSR_READ_4 (pDev, MMCHS_PSTATE) & PSTATE_BREN)
            (void)eMmchsPioRead (pDev);
        }

    if (intSts & IRQ_BWR)
        {
        EMMCHS_DBG (MMCHS_DBG_IRQ, "CMD buffer write ready\n",
                      0, 0, 0, 0, 0, 0);

        if (CSR_READ_4 (pDev, MMCHS_PSTATE) & PSTATE_BWEN)
            (void)eMmchsPioWrite (pDev);
        }

    /* other errors */

    if (intSts & IRQ_AC12E)
        {
        (void) CSR_READ_4 (pDev, MMCHS_AC12);
        EMMCHS_DBG (MMCHS_DBG_IRQ, "Auto CMD12 error\n",
                      0, 0, 0, 0, 0, 0);

        /* FIXME: error recovery */
        }
    }

/*******************************************************************************
*
* eMmchsCardMonTask - card status monitor task
*
* This routine is the task loop to handle card insersion and removal.
*
* RETURN: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsCardMonTask
    (
    VXB_DEV_HANDLE   pDev
    )
    {
    VXB_DEV_HANDLE pChildDev;
    MMCHS_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet (pDev);

    while (!erfLibInitialized)
        (void)taskDelay (sysClkRateGet ());

    FOREVER
        {
        if (pDrvCtrl->cardDetect(pDev) && !pDrvCtrl->cardIns)
            {
            /* power on MMC1 */

            pDrvCtrl->cardPowerOn (pDev);

            if (pDrvCtrl->dmaMode == MMCHS_DMA_MODE_SDMA)
                {
                if (pDrvCtrl->dmaAlloc (pDev) != OK)
                    {
                    EMMCHS_DBG (MMCHS_DBG_XBD,
                                  "SD/MMC DMA channel allocation failed\n",
                                  0, 0, 0, 0, 0, 0);
                    continue;
                    }
                }
            eMmchsClkFreqSetup(pDev, SDMMC_CLK_FREQ_400KHZ);
            (void)eMmchsBusWidthSetup(pDev, SDMMC_BUS_WIDTH_1BIT);

            (void)emmcDevCreate(pDev, NULL);

            pDrvCtrl->attached = TRUE;
            pDrvCtrl->cardIns = TRUE;
            }

        if (!pDrvCtrl->cardDetect (pDev) && pDrvCtrl->cardIns)
            {
            if (pDrvCtrl->attached == FALSE)
                continue;
			
			pDrvCtrl->attached = FALSE;
            pDrvCtrl->cardIns = FALSE;

            if (pDrvCtrl->dmaMode == MMCHS_DMA_MODE_SDMA)
                {
                /* free the allocated DMA channel */

                pDrvCtrl->dmaFree (pDev); 
                }

            pChildDev = pDrvCtrl->sdHostSpec.childDev;
            if (pChildDev)
                {
                (void)vxbDevRemove(pChildDev);
                pDrvCtrl->sdHostSpec.childDev = NULL;
                }
            }

        while (semTake (pDrvCtrl->devChange, WAIT_FOREVER) != OK)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR,
                          "semTake () failed\n",
                          0, 0, 0, 0, 0, 0);
            }
        }
    }

/*******************************************************************************
*
* eMmchsCmdPrepare - prepare the command to be sent
*
* This routine prepares the command to be sent.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void eMmchsCmdPrepare
    (
    VXB_DEV_HANDLE pDev,
    SD_CMD * pSdCmd
    )
    {
    STATUS ret;
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32 xferTyp = 0;
    UINT32 mask = 0x00000001;

    if (pDev == NULL)
        return;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);
    if (pDrvCtrl == NULL)
        return;
 

    /* check PRSSTAT[CDIHB] and PRSSTAT[CIHB] before writing to XFERTYP */

    if (eMmchsWaitStatus (pDev, MMCHS_PSTATE, 
                           PSTATE_CDIHB, FALSE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "wait CDIHB error\n", 0, 0, 0, 0, 0, 0);
        return ;
        }

    if (eMmchsWaitStatus (pDev, MMCHS_PSTATE, 
                           PSTATE_CIHB, FALSE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "wait CIHB error\n", 0, 0, 0, 0, 0, 0);
        return ;        
        }

    if (eMmchsWaitStatus (pDev, MMCHS_PSTATE, 
                           PSTATE_DLA, FALSE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "wait DLA error\n", 0, 0, 0, 0, 0, 0);
        return ;        
        }
    
    /* CMD0, CMD12, CMD13 doesn't need to wait for data */
    if (/*pSdCmd->cmdIdx == SDMMC_CMD_GO_IDLE_STATE ||*/
        pSdCmd->cmdIdx == SDMMC_CMD_STOP_TRANSMISSION ||
        pSdCmd->cmdIdx == SDMMC_CMD_SEND_STATUS)
    {
        mask &= ~(02);
        CSR_WRITE_4(pDev, MMCHS_AC12, mask);
    }
    
#if 0    
    int i = 0;

    while (CSR_READ_4(pDev, MMCHS_PSTATE) & mask)
        {
        vxbUsDelay(1);
        i++;
        if (i > 10000)
            return ERROR;
        }
#endif

    /* setup command */

    xferTyp |= CMD_CMDINX (pSdCmd->cmdIdx);

    /* setup command fields */

    if (pSdCmd->rspType & SDMMC_CMD_RSP_CMDIDX)
        {
        xferTyp |= CMD_CICEN;
        }

    if (pSdCmd->rspType & SDMMC_CMD_RSP_CRC)
        {
        xferTyp |= CMD_CCCEN;
        }

    if (pSdCmd->rspType & SDMMC_CMD_RSP_LEN136)
        {
        xferTyp |= CMD_RSPTYP_LEN136;
        }
    else if (pSdCmd->rspType & SDMMC_CMD_RSP_BUSY)
        {
        xferTyp |= CMD_RSPTYP_LEN48B;
        }
    else if (pSdCmd->rspType & SDMMC_CMD_RSP_LEN48)
        {
        xferTyp |= CMD_RSPTYP_LEN48;
        }

    /* setup data fields */

    if (pSdCmd->hasData)
        {
        xferTyp |= CMD_DATA_PRESENT;

        /* direction */

        if (pSdCmd->cmdData.isRead)
        {
            xferTyp |= CMD_DIR_READ;
        }

        if (pDrvCtrl->dmaMode != MMCHS_DMA_MODE_PIO)
        {
            xferTyp |= CMD_DMAEN;
        }

        /* multipule block transfer */

        if (pSdCmd->cmdData.blkNum > 1)
            {
            xferTyp |= (CMD_MULTI_BLK | CMD_BCEN);
            }

        if (pDrvCtrl->dmaMode != MMCHS_DMA_MODE_PIO)
            {

            ret = pDrvCtrl->dmaStart (pDev, pSdCmd);

            if (ret != OK)
                {
                //EMMCHS_DBG (MMCHS_DBG_ERR,
                kprintf("Mmchs DmaStart () failed with %d\n",
                              ret);

                return;
                }
            }
        else
            {
            pDrvCtrl->pioDesc.bufOri = pSdCmd->cmdData.buffer;
            pDrvCtrl->pioDesc.bufCur = pSdCmd->cmdData.buffer;
            pDrvCtrl->pioDesc.blkSize = pSdCmd->cmdData.blkSize;
            }

        CSR_WRITE_4 (pDev, MMCHS_BLK,
                     (pSdCmd->cmdData.blkNum << BLK_CNT_SHIFT) |
                     pSdCmd->cmdData.blkSize);

        /* setup transfer timeouts */

        CSR_CLRBIT_4 (pDev, MMCHS_SYSCTL, SYSCTL_DTOCV_MASK);
        CSR_SETBIT_4 (pDev, MMCHS_SYSCTL,
                      SYSCTL_DTOCV_MAX << SYSCTL_DTOCV_SHIFT);
        }
    
    CSR_WRITE_4 (pDev, MMCHS_ARG, (UINT32)pSdCmd->cmdArg);    
    CSR_WRITE_4 (pDev, MMCHS_CMD, /*0x1A3*/xferTyp);

    if (pSdCmd->hasData && (pDrvCtrl->dmaMode != MMCHS_DMA_MODE_PIO))
        {
        
        ret = semTake(pDrvCtrl->dmaDone, (sysClkRateGet() * SDMMC_CMD_WAIT_IN_SECS));
        if (ret == ERROR)
           {
            kprintf("Mmchs dma timeout\n");

           pSdCmd->dataErr |= SDMMC_DATA_ERR_TIMEOUT;
            return;
            }
        
        }
    }

/*******************************************************************************
*
* eMmchsCmdIssue - issue the command
*
* This routine issues the command.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsCmdIssue
    (
    VXB_DEV_HANDLE pDev,
    SD_CMD * pSdCmd
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    STATUS rc;
    
    UINT32 mask = 0x00000001;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    pSdCmd->cmdErr = 0x0;
    pSdCmd->dataErr = 0x0;
    pSdCmd->dmaDone = pDrvCtrl->dmaDone;
    
    eMmchsCmdPrepare (pDev, pSdCmd);

    rc = semTake (pDrvCtrl->cmdDone, /*NO_WAIT*/
                  (sysClkRateGet() * SDMMC_CMD_WAIT_IN_SECS));
    if (rc == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "[eMmchsCmdIssue]: CMD%d cmd timeout\n",
                      pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
        pSdCmd->cmdErr |= SDMMC_CMD_ERR_TIMEOUT;
        return (rc);
        }

    if (pDrvCtrl->intSts & IRQ_CMD_ERR)
        {
        if (pDrvCtrl->intSts & IRQ_CIE)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR, "CMD%d cmd index error\n",
                          pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
            pSdCmd->cmdErr |= SDMMC_CMD_ERR_PROTOCOL;
            }

        if (pDrvCtrl->intSts & IRQ_CEBE)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR,
                          "CMD%d cmd end of bit error\n",
                          pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
            pSdCmd->cmdErr |= SDMMC_CMD_ERR_PROTOCOL;
            }

        if (pDrvCtrl->intSts & IRQ_CCE)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR, "CMD%d cmd crc error\n",
                          pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
            pSdCmd->cmdErr |= SDMMC_CMD_ERR_PROTOCOL;
            }
        
        if (pDrvCtrl->intSts & IRQ_CTOE)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR, "CMD%d cmd timeout error\n",
                          pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
            pSdCmd->cmdErr |= SDMMC_CMD_ERR_TIMEOUT;
            }
        }

    if (pSdCmd->cmdErr)
        {

        /* Software reset for mmci_cmd line */

        if (pSdCmd->cmdErr & SDMMC_CMD_ERR_TIMEOUT)
            {

            CSR_SETBIT_4 (pDev, MMCHS_SYSCTL, MMCHS_SYSCTL_SRC_MASK);

            if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                                   MMCHS_SYSCTL_SRC_MASK, TRUE) == ERROR)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR,
                              "wait reset assert error\n", 0, 0, 0, 0, 0, 0);
                return ERROR;
                }

            if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                                   MMCHS_SYSCTL_SRC_MASK, FALSE) == ERROR)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR,
                              "wait reset release error\n", 0, 0, 0, 0, 0, 0);
                return ERROR;
                }
            }
        }

    if (pSdCmd->rspType != SDMMC_CMD_RSP_NONE)
        {
        if (pSdCmd->rspType & SDMMC_CMD_RSP_LEN136)
            {
            UINT32 cmdRsp[4];

            cmdRsp[0] = CSR_READ_4 (pDev, MMCHS_RSP10);
            cmdRsp[1] = CSR_READ_4 (pDev, MMCHS_RSP32);
            cmdRsp[2] = CSR_READ_4 (pDev, MMCHS_RSP54);
            cmdRsp[3] = CSR_READ_4 (pDev, MMCHS_RSP76 );

            pSdCmd->cmdRsp[0] = be32toh (cmdRsp[3]);
            pSdCmd->cmdRsp[1] = be32toh (cmdRsp[2]);
            pSdCmd->cmdRsp[2] = be32toh (cmdRsp[1]);
            pSdCmd->cmdRsp[3] = be32toh (cmdRsp[0]);
            }
        else
            {
            pSdCmd->cmdRsp[0] = CSR_READ_4 (pDev, MMCHS_RSP10);
            }
        }

    if ((pSdCmd->rspType & SDMMC_CMD_RSP_BUSY) && (pSdCmd->hasData == FALSE))
        {

        /* NO data tran but has data int ack. such as sdCmdSelectCard */

        (void)semTake (pDrvCtrl->dataDone, sysClkRateGet() / 4);
        return OK;
        }

    if (pSdCmd->hasData)
        {
        rc = semTake (pDrvCtrl->dataDone, /*NO_WAIT*/
                       (sysClkRateGet() * SDMMC_CMD_WAIT_IN_SECS));
        if (rc == ERROR)
            {
            EMMCHS_DBG (MMCHS_DBG_ERR,
                          "[eMmchsCmdIssue]: CMD%d data timeout\n",
                          pSdCmd->cmdIdx, 0, 0, 0, 0, 0);

            pSdCmd->dataErr |= SDMMC_DATA_ERR_TIMEOUT;
            return (rc);
            }

        if (pDrvCtrl->intSts & IRQ_DATA_ERR)
            {
            if (pDrvCtrl->intSts & IRQ_DEBE)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR,
                              "CMD%d data end of bit error\n",
                              pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
                pSdCmd->dataErr |= SDMMC_DATA_ERR_PROTOCOL;
                }
        
            if (pDrvCtrl->intSts & IRQ_DCE)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR, "CMD%d data crc error\n",
                              pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
                pSdCmd->dataErr |= SDMMC_DATA_ERR_PROTOCOL;
                }
        
            if (pDrvCtrl->intSts & IRQ_DTOE)
                {
                EMMCHS_DBG (MMCHS_DBG_ERR, "CMD%d data timeout error\n",
                              pSdCmd->cmdIdx, 0, 0, 0, 0, 0);
                pSdCmd->dataErr |= SDMMC_DATA_ERR_TIMEOUT;
                }
            }
        }

    if ((pSdCmd->dataErr != 0x0) || (pSdCmd->cmdErr != 0x0))
        {
        EMMCHS_DBG (MMCHS_DBG_ERR,
                      "[eMmchsCmdIssue]: CMD%d cmd->dataErr = 0x%x cmd->cmdErr = 0x%x\n",
                      pSdCmd->cmdIdx, pSdCmd->dataErr, pSdCmd->cmdErr, 0, 0, 0);
        rc = ERROR;
        }

    return (rc);
    }

/*******************************************************************************
*
* eMmchsClkFreqSetup - setup the clock frequency
*
* This routine setup the clock frequency.
*
* RETURNS: Always OK;
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsClkFreqSetup
    (
    VXB_DEV_HANDLE pDev,
    UINT32       clk
    )
    {
    UINT32 sdClk, value;

    if (clk > MMCHS_MAX_CLK_FREQ)
    {
        clk = MMCHS_MAX_CLK_FREQ;
    }

    if (clk == CLK_FREQ_400K)
        {
        sdClk = (MMCHS_REF_CLK_FREQ / clk);
        }
    else
        {
        sdClk = (MMCHS_REF_CLK_FREQ / clk);

        if (sdClk == 0)
        {
            sdClk = 1;        
        }

        /* make sure generated frequency <= expected frequency */

        if ((MMCHS_REF_CLK_FREQ / sdClk) > clk)
            {
            sdClk = sdClk + 1;
            }
        }
    sdClk &= MMC_CLKD_MASK;

    /* stop clock output */
    CSR_CLRBIT_4 (pDev, MMCHS_SYSCTL, SYSCTL_CEN_MASK);

    value = CSR_READ_4 (pDev, MMCHS_SYSCTL);
    value &= ~(SYSCTL_DTO_MASK | SYSCTL_CLKD_MASK);
    value |= (SYSCTL_DTO_MAX << SYSCTL_DTO_SHIFT) | \
            (sdClk << SYSCTL_CLKD_SHIFT);
    
    /*0x2: MMCi_FCLK / 2 */    
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, value);
    /* Enable internal clock */
    CSR_WRITE_4 (pDev, MMCHS_SYSCTL, CSR_READ_4 (pDev, MMCHS_SYSCTL) | 
                 SYSCTL_ICE_MASK);
                 
    if (eMmchsWaitStatus (pDev, MMCHS_SYSCTL, 
                           SYSCTL_ICS_MASK, TRUE) == ERROR)
        {
        EMMCHS_DBG (MMCHS_DBG_ERR, "wait ICS error\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    /* SYSCTL_CEN_MASK(0x1): The clock is provided to the card and can be automatically gated when
	 * MMCHS_SYSCONFIG[AUTOIDLE] is set to 1 (default value)
	 */
    CSR_SETBIT_4 (pDev, MMCHS_SYSCTL, SYSCTL_CEN_MASK);

    return OK;
    }

/*******************************************************************************
*
* eMmchsBusWidthSetup - setup the bus width
*
* This routine setup the bus width.
*
* RETURNS: Always OK.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsBusWidthSetup
    (
    VXB_DEV_HANDLE   pDev,
    UINT32          width
    )
    {
    if (width == SDMMC_BUS_WIDTH_1BIT)
        {
        CSR_CLRBIT_4 (pDev, MMCHS_CON, MMCHS_CON_DW8);
        CSR_CLRBIT_4 (pDev, MMCHS_HCTL, HCTL_DTW_4BIT);
        }
    else if (width == SDMMC_BUS_WIDTH_4BIT)
        {
        CSR_CLRBIT_4 (pDev, MMCHS_CON, MMCHS_CON_DW8);
        CSR_SETBIT_4 (pDev, MMCHS_HCTL, HCTL_DTW_4BIT);
        }
    else if (width == SDMMC_BUS_WIDTH_8BIT)
	   {
    	CSR_SETBIT_4 (pDev, MMCHS_CON, MMCHS_CON_DW8);
	   }


    return OK;
    }

/*******************************************************************************
*
* eMmchsWpCheck - Check the write protect status
*
* This routine checks the write protect status.
*
* RETURNS: TRUE or FALSE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL BOOL eMmchsWpCheck
    (
    VXB_DEV_HANDLE pDev
    )
    {
    BOOL rc;
    MMCHS_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    if (pDrvCtrl == NULL)
        return FALSE;
    if (pDrvCtrl->cardWpCheck == NULL)
        return FALSE;

    rc = pDrvCtrl->cardWpCheck(pDev);
    return (rc);
    }

/*******************************************************************************
*
* eMmchsPioRead - read data from SD/MMC card using PIO
*
* This routine reads data from SD/MMC card using PIO.
*
* RETURNS: OK
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsPioRead
    (
    VXB_DEV_HANDLE   pDev
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32 blkSize;
    char * buffer;
    UINT32 data = 0;
    UINT32 i;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    blkSize = pDrvCtrl->pioDesc.blkSize;
    buffer = pDrvCtrl->pioDesc.bufCur;

    for (i = 0; i < blkSize; i += 4)
        {
        data = PIO_READ_4 (pDev, MMCHS_DATA);
        *(UINT32 *)buffer = data;
        buffer += 4;
        }

    pDrvCtrl->pioDesc.bufCur = buffer;

    EMMCHS_DBG (MMCHS_DBG_RW,
                  "[PIO_READ]: bufOri (%08x) bufCur (%08x)\n",
                  pDrvCtrl->pioDesc.bufOri, pDrvCtrl->pioDesc.bufCur,
                  0, 0, 0, 0);

    return OK;
    }

/*******************************************************************************
*
* eMmchsPioWrite - write data to SD/MMC card using PIO
*
* This routine writes data to SD/MMC card using PIO.
*
* RETURNS: OK
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL STATUS eMmchsPioWrite
    (
    VXB_DEV_HANDLE   pDev
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;
    UINT32 blkSize, data, i;
    char * buffer;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    blkSize = pDrvCtrl->pioDesc.blkSize;
    buffer = pDrvCtrl->pioDesc.bufCur;

    for (i = 0; i < blkSize; i += 4)
        {
        data = *(UINT32 *)buffer;
        buffer += 4;
        PIO_WRITE_4 (pDev, MMCHS_DATA, data);
        }

    pDrvCtrl->pioDesc.bufCur = buffer;

    EMMCHS_DBG (MMCHS_DBG_RW,
                        "[PIO_WRITE]: bufOri (%08x) bufCur (%08x)\n",
                        pDrvCtrl->pioDesc.bufOri, pDrvCtrl->pioDesc.bufCur,
                        0, 0, 0, 0);

    return OK;
    }

/*******************************************************************************
*
* eMmchsSpecInfoGet - get host controller spec info
*
* This routine gets host controller spec info.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS eMmchsSpecInfoGet
    (
    VXB_DEV_HANDLE pDev,
    void ** pHostSpec,
    VXB_SD_CMDISSUE_FUNC * pCmdIssue
    )
    {
    MMCHS_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MMCHS_DRV_CTRL *)vxbDevSoftcGet(pDev);

    *pHostSpec = (void *)(&(pDrvCtrl->sdHostSpec));
    *pCmdIssue = eMmchsCmdIssue;

    return (OK);
    }
