/* gfxTiDss.c - TI DSS Graphics Controller driver initialization */

/*
 * Copyright (c) 2013-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
27jul17,s_m  Modified for TI AM5728 FB implementation
24jan14,mgc  Modified for VxWorks 7 release
22mar13,af  Written
*/

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <vmLib.h>
#include <gfxTiDssDrv.h>
#include <gfxTiDss.h>
#include <gfxTiAm35xx.h>
#include <gfxTiAm37xx.h>

#include <pmapLib.h>

/* defines */

#ifndef GFX_PMAP_ATTR
#define GFX_PMAP_ATTR               (MMU_ATTR_VALID |         \
                                     MMU_ATTR_SUP_RW |        \
                                     MMU_ATTR_USR_RW |        \
                                     MMU_ATTR_CACHE_OFF)
#endif


#define GFX_DELAY(_loops)               \
    {                                   \
    volatile int _delayCnt = 0;         \
    while (_delayCnt++ < _loops);       \
    } while (0)

/* locals */
/* create global variable to select HDMI config paramaters based on screen resolution */
BOOL gHdmiConfig_480x800 = FALSE;

LOCAL GFX_TIDSS_TIMING gfxTiAm35xxHdmiTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   0,   0,   0,   48,  244, 250, 23,  9,   6,   1,   9,   432, 12, 1024, 768}, /* 1024 * 768 */
    {  0,   0,   0,   0,   63,  254,  76, 15,  23,  8,   1,   9,   432, 12, 1280, 720}, /* 1280 * 720 */
    {  0,   0,   0,   0,  153,  219, 254,  5,  15,  8,   1,   9,   432, 12, 1280, 800} /* 1280 * 800 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiAm57xxHdmiTiming[] =
    {
    {  0,   1,   1,   1,   3,   22,  3,   2,   1,   1,   5,   7,   432, 12, 480, 800}, /* 480 * 800 */
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */
    {  0,   0,   0,   0,  0,  0, 0,  2,  5, 9,   1,   9,   432, 12, 1920, 1080}, /* 1920 * 1080 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiAm35xxLcdTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   0,   1,   1,   2,   1,   41,   2,   3,   10,   8, 9,   432, 12, 480, 272} /* 480 * 272 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiAm37xxDviTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   0,   1,   0,   90,  90,  64,  3,   6,   29,  2,   7,   864, 12, 1024, 768}, /* 1024 * 768 */
    {  0,   0,   1,   0,   34,  28,  30,  20,  6,   3,   2,   7,   864, 12, 1280, 720}, /* 1280 * 720 */
    {  0,   0,   1,   0,   24,  18,  20,  3,   6,   20,  2,   7,   864, 12, 1280, 800} /* 1280 * 800 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiAm37xxLcdTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   1,   1,   1,   3,   22,  3,   2,   1,   1,   5,   7,   864, 12, 480, 640} /* 480 * 640 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiOmap35xxDviTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   0,   1,   0,   90,  90,  64,  3,   6,   29,  2,   7,   432, 12, 1024, 768}, /* 1024 * 768 */
    {  0,   0,   1,   0,   34,  28,  30,  20,  6,   3,   2,   7,   432, 12, 1280, 720}, /* 1280 * 720 */
    {  0,   0,   1,   0,   24,  18,  20,  3,   6,   20,  2,   7,   432, 12, 1280, 800} /* 1280 * 800 */
    };

LOCAL GFX_TIDSS_TIMING gfxTiOmap35xxLcdTiming[] =
    {
    /* rf   ipc  ihs  ivs  hfp  hbp  hsw  vfp  vbp  vsw  pcd  dcd  p4m  p4d xres yres */

    {  0,   1,   1,   1,   3,   22,  3,   2,   1,   1,   5,   7,   432, 12, 480, 640} /* 480 * 640 */
    };

/* forward declarations */

IMPORT void gfxTiAm35xxEnable (void);
IMPORT void gfxTiAm35xxLcdPowerOff (void);
IMPORT void gfxTiAm35xxLcdPowerOn (void);
IMPORT void gfxTiAm35xxLcdBLPowerOff (void);
IMPORT void gfxTiAm35xxLcdBLPowerOn (void);

IMPORT void gfxTiAm37xxEnable (GFX_FBDEV*);
IMPORT void gfxTiAm37xxDviDisable (void);
IMPORT void gfxTiAm37xxDviEnable (void);
IMPORT void gfxTiAm37xxLcdDisable (void);
IMPORT void gfxTiAm37xxLcdEnable (void);
IMPORT void gfxTiAm37xxLcdVgaModeDisable (void);
IMPORT void gfxTiAm37xxLcdVgaModeEnable (void);

void hardCodeHdmi(void);

VIRT_ADDR       virtMuxAddr;
VIRT_ADDR       virtDispcAddr;
VIRT_ADDR       virtDssAddr;
VIRT_ADDR       virtClkAddr;
VIRT_ADDR       virtHdmiAddr;
VIRT_ADDR       virtHdmiPhyAddr;
VIRT_ADDR       virtHdmiPllAddr;
VIRT_ADDR       virtHdmiCoreAddr;
/*******************************************************************************
 *
 * gfxTiDssDispcCleanIrq - clean IRQ status
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssDispcCleanIrq
    (
    UINT32  irqNum
    )
    {
    GFX_WRITE_BIT32 (GFX_TI_DISPC_IRQSTATUS, irqNum, 1);
    }

/*******************************************************************************
 *
 * gfxTiDssDispcWaitIrq - wait IRQ status
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssDispcWaitIrq
    (
    UINT32  irqNum
    )
    {
    while (0 == GFX_READ_BIT32 (GFX_TI_DISPC_IRQSTATUS, irqNum));
    }

/*******************************************************************************
 *
 * gfxTiDssEnableVsync - Enable Vsync interrupt
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssEnableVsync
    (
    GFX_FBDEV*      pDev
    )
    {
    UINT32 regData;

    /* AM5728 - skip gfxTiDssEnableVsync */
    return;

#if 0
    regData = GFX_READ_REG32 (GFX_TI_DISPC_IRQENABLE);
    regData = regData | 0x2;
    GFX_WRITE_REG32(GFX_TI_DISPC_IRQENABLE, regData);
#endif
    }

/*******************************************************************************
 *
 * gfxTiDssInit - initialize DSS
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssInit (void)
    {
    volatile UINT32 regData;

#if 0 /* Do not write GFX_TI_CM..the am35xx Clock Manager replaced with virtClkAddrs code */
    if (ERROR == vmPageMap (NULL, (VIRT_ADDR)GFX_TI_CM, (PHYS_ADDR)GFX_TI_CM,
                            GFX_TI_CM_REG_SIZE,
                            GFX_TI_REG_STATE_MASK, GFX_TI_REG_STATE))
        {
        if (S_vmLib_PHYS_PAGES_ALREADY_MAPPED != errno)
            {
            (void)fprintf (stderr, "Unable to map GFX_TI_CM\n");
            return ERROR;
            }
        }

    if (ERROR == vmPageMap (NULL, (VIRT_ADDR)GFX_TI_DSS, (PHYS_ADDR)GFX_TI_DSS,
                            GFX_TI_DSS_REG_SIZE,
                            GFX_TI_REG_STATE_MASK, GFX_TI_REG_STATE))
        {
        if (S_vmLib_PHYS_PAGES_ALREADY_MAPPED != errno)
            {
            (void)fprintf (stderr, "Unable to map GFX_TI_DSS\n");
            return ERROR;
            }
        }

    /* enable DSS functional and interface clock */
#else
    virtClkAddr = (VIRT_ADDR)pmapGlobalMap (0x4A000000, 0x100000, GFX_PMAP_ATTR);
    if (virtClkAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtClkAddr pmapGlobalMap failed\n");
        return ERROR;
        }

     /* Write CM_DIV_H12_DPLL_PER */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x815c) = 0x204;

     /* set bit 0 CTRL_CORE_CONTROL_IO_2 */ 
     *(UINT32 *)((unsigned int)virtClkAddr + 0x2558);
     *(UINT32 *)((unsigned int)virtClkAddr + 0x2558) = (*(UINT32 *)((unsigned int)virtClkAddr + 0x2558) | 0x1);

     /* Write CM_DSS_CLKSTCTRL */ 
     *(UINT32 *)((unsigned int)virtClkAddr + 0x9100) = /*0x40B03*/0x40B02;
     /* Write CM_DSS_DSS_CLKCTRL */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x9120) =0x000102;
     while(0x10000 & *(UINT32 *)((unsigned int)virtClkAddr + 0x9120))
     {
         taskDelay(0);
     }
     /* Write CTRL_CORE_DSS_PLL_CONTROL */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x2538) = 0x2AB;
     *(UINT32 *)((unsigned int)virtClkAddr + 0x9120) =0x000702;

     /* Write CM_DIV_H13_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5140) = 0x23E;
     /* Write CM_DIV_H14_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5144) = 0x205;
     /* Write CM_CLKSEL_DPLL_DSP */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5240) = 0x4B01;
     /* Write CM_CLKMODE_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5120) = 0x7;
     /* Write CM_IDLEST_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5124) = 0x1F;
     /* Write CM_CLKSEL_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x512c) = 0x10A04;
     /* Write CM_AUTOIDLE_DPLL_CORE */
     *(UINT32 *)((unsigned int)virtClkAddr + 0x5128) = 0x1;


    virtDispcAddr = (VIRT_ADDR)pmapGlobalMap (GFX_TI_DISPC, GFX_TI_DSS_REG_SIZE, GFX_PMAP_ATTR);
    if (virtDispcAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtDispcAddr pmapGlobalMap failed\n");
        return ERROR;
        }
    virtDssAddr = (VIRT_ADDR)pmapGlobalMap (GFX_TI_DSS, GFX_TI_DSS_REG_SIZE, GFX_PMAP_ATTR);
    if (virtDssAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtDispcDSSAddr pmapGlobalMap failed\n");
        return ERROR;
        }
#endif

#if 0 /* am57xx GFX_TI_CM replacced with virtClkAddr */

    GFX_WRITE_BIT32 (GFX_TI_CM_FCLKEN_DSS, GFX_TI_CM_FCLKEN_DSS_EN_DSS1, 0x1);
    GFX_WRITE_BIT32 (GFX_TI_CM_FCLKEN_DSS, GFX_TI_CM_FCLKEN_DSS_EN_DSS2, 0x1);
    GFX_WRITE_BIT32 (GFX_TI_CM_FCLKEN_DSS, GFX_TI_CM_FCLKEN_DSS_EN_TV, 0x1);
    GFX_WRITE_BIT32 (GFX_TI_CM_ICLKEN_DSS, GFX_TI_CM_ICLKEN_DSS_EN_DSS, 0x1);
#endif

    virtHdmiAddr = (VIRT_ADDR)pmapGlobalMap (0x58040000, 0x10000, GFX_PMAP_ATTR);
    if (virtHdmiAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtHdmiAddr pmapGlobalMap failed\n");
        return ERROR;
        }

    virtHdmiPhyAddr = (VIRT_ADDR)pmapGlobalMap (0x58040300, 0x100, GFX_PMAP_ATTR);
    if (virtHdmiPhyAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtHdmiPhyAddr pmapGlobalMap failed\n");
        return ERROR;
        }
    virtHdmiPllAddr = (VIRT_ADDR)pmapGlobalMap (0x58040200, 0x100, GFX_PMAP_ATTR);
    if (virtHdmiPllAddr == (VIRT_ADDR)PMAP_FAILED)
        {
        (void)fprintf (stderr, "virtHdmiPllAddr pmapGlobalMap failed\n");
        return ERROR;
        }
    /* stop LCD */
#if 0 /* WRS:NOTE - comment out for AM57XX */
    if (GFX_READ_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_LCDENABLE) == 0x1)
        {
        GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_LCDENABLE, 0);

        gfxTiDssDispcCleanIrq (GFX_TI_DISPC_IRQSTATUS_FRAMEDONE);
        gfxTiDssDispcWaitIrq (GFX_TI_DISPC_IRQSTATUS_FRAMEDONE);
        }

    GFX_WRITE_REG32 (GFX_TI_DISPC_IRQENABLE, 0);

    /* reset DSS */

    GFX_WRITE_BIT32 (GFX_TI_DSS_SYSCONFIG, GFX_TI_DSS_SYSCONFIG_SOFTRESET, 1);

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_SYSCONFIG_MIDLEMODE, 1);
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_SYSCONFIG_SIDLEMODE, 1);

    while (GFX_READ_BIT32 (GFX_TI_DSS_SYSSTATUS, GFX_TI_DSS_SYSSTATUS_RESETDONE)
           != 0x1)
        {
        (void)taskDelay (10);
        }

    regData = GFX_DISPC_LOAD_MODE << GFX_TI_DISPC_CONFIG_LOADMODE;
    GFX_WRITE_REG32 (GFX_TI_DISPC_CONFIG, regData);
#else
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_SYSCONFIG_MIDLEMODE, 1);
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_SYSCONFIG_SIDLEMODE, 1);
    regData = GFX_DISPC_LOAD_MODE << GFX_TI_DISPC_CONFIG_LOADMODE;
    GFX_WRITE_REG32 (GFX_TI_DISPC_CONFIG, regData);
#endif

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssDispcSetGpio - set the output value of GPIO
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssDispcSetGpio
    (
    UINT8   pin,
    UINT8   value
    )
    {
#if 0 /* WRS:NOTE - comment out for AM57XX */
    if (0 == pin)
        {
        GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GPOUT0, value);
        }
    else
        {
        GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GPOUT1, value);
        }
#endif
    }

/*******************************************************************************
 *
 * gfxTiDssVid1SetAlpha - set VID1 global alpha
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssVid1SetAlpha
    (
    UINT8   value
    )
    {
    volatile UINT32 regData;

    regData = GFX_READ_REG32 (GFX_TI_DISPC_GLOBAL_ALPHA);
    regData &= ~(0xFF << GFX_TI_DISPC_GLOBAL_VID);
    regData |= value << GFX_TI_DISPC_GLOBAL_VID;
    GFX_WRITE_REG32 (GFX_TI_DISPC_GLOBAL_ALPHA, regData);
    }

/*******************************************************************************
 *
 * gfxTiDssGfxSetAlpha - set GFX global alpha
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssGfxSetAlpha
    (
    UINT8   value
    )
    {
    volatile UINT32 regData;

    regData = GFX_READ_REG32 (GFX_TI_DISPC_GLOBAL_ALPHA);
    regData &= ~(0xFF << GFX_TI_DISPC_GLOBAL_GFX);
    regData |= value << GFX_TI_DISPC_GLOBAL_GFX;
    GFX_WRITE_REG32 (GFX_TI_DISPC_GLOBAL_ALPHA, regData);
    }

/*******************************************************************************
 *
 * gfxTiDssDispcLcdSetOverlay - configurate LCD overlay
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssDispcLcdSetOverlay
    (
    GFX_TI_DISPC_OVERLAY_INFO* pOverlayInfo
    )
    {
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_CONFIG_LCDALPHABLENDEREN,
                        pOverlayInfo->lcdAlphaEn);
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_CONFIG_TCKLCDENABLE,
                        pOverlayInfo->lcdTckEn);
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONFIG, GFX_TI_DISPC_CONFIG_TCKLCDSELECTION,
                        pOverlayInfo->lcdTckSel);
    }

/*******************************************************************************
 *
 * gfxTiDssRotInit - initialize hardware rotation
 *
 * RETURNS: N/A
 *
 */
LOCAL STATUS gfxTiDssRotInit (void)
    {
#if 0 /* WRS:NOTE - comment out for AM57XX */
    if (ERROR == vmPageMap (NULL, (VIRT_ADDR)GFX_TI_SMS_BASE,
                            (PHYS_ADDR)GFX_TI_SMS_BASE,
                            GFX_SMS_REG_SIZE,
                            GFX_TI_REG_STATE_MASK, GFX_TI_REG_STATE))
        {
        if (S_vmLib_PHYS_PAGES_ALREADY_MAPPED != errno)
            {
            (void)fprintf (stderr, "Unable to map GFX_TI_SMS_BASE\n");
            return ERROR;
            }
        }
#endif
    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssDispcClockConfig - configurate Dispc Clock
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssDispcClockConfig
    (
    GFX_TI_DISPC_LCD_INFO* pLcdInfo
    )
    {
    volatile UINT32 regData;

    /* reset 96MHz clock for M2 */

#if 0 /* WRS:NOTE - comment out for AM57XX */
    regData = (GFX_READ_REG32 (GFX_TI_CM_CLKSEL3_PLL) &
              (~GFX_TI_CM_CLKSEL3_PLL_DIV_96M_MASK)) |
              (GFX_TI_CM_CLKSEL3_PLL_DIV_96M);
    GFX_WRITE_REG32 (GFX_TI_CM_CLKSEL3_PLL, regData);

    /* set DPLL4 ALWON functional clock */
    regData = GFX_READ_REG32 (GFX_TI_CM_CLKSEL2_PLL) &
              (~(GFX_TI_CM_CLKSEL2_PLL_DIV_MASK | GFX_TI_CM_CLKSEL2_PLL_MULT_MASK));

    GFX_WRITE_REG32 (GFX_TI_CM_CLKSEL2_PLL, regData |
                    (pLcdInfo->pTiming->p4m << GFX_TI_CM_CLKSEL2_PLL_MULT_SHIFT) |
                    pLcdInfo->pTiming->p4d);

    regData = GFX_READ_REG32 (GFX_TI_DSS_CONTROL) &
              (~(GFX_TI_DISPC_CLK_SWITCH | GFX_TI_DSI_CLK_SWITCH));
    GFX_WRITE_REG32 (GFX_TI_DSS_CONTROL, regData);
#else
    /* use DSS CLK (from PRCM) */
    GFX_WRITE_REG32 (GFX_TI_DSS_CONTROL, 0);
#endif

    regData = GFX_TI_DSS_SYSCONFIG_AUTOIDEL;
    GFX_WRITE_REG32 (GFX_TI_DSS_SYSCONFIG, regData);

#if 0 /* WRS:NOTE - comment out for AM57XX */
    GFX_WRITE_BIT32 (GFX_TI_CM_FCLKEN_DSS, GFX_TI_CM_FCLKEN_DSS_EN_DSS1, 0x0);

    /* set DSS1 ALWON functional clock */

    GFX_WRITE_REG32 (GFX_TI_CM_CLKSEL_DSS, pLcdInfo->pTiming->dcd);

    GFX_WRITE_BIT32 (GFX_TI_CM_FCLKEN_DSS, GFX_TI_CM_FCLKEN_DSS_EN_DSS1, 0x1);
#endif

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssDispcLcdConfig - configurate LCD
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssDispcLcdConfig
    (
    GFX_TI_DISPC_LCD_INFO* pLcdInfo,
    int xres,
    int yres
    )
    {
    volatile UINT32 regData;


    if ((NULL == pLcdInfo) || (NULL == pLcdInfo->pTiming))
        return ERROR;

    regData = (0x1 << GFX_TI_DISPC_DIVISOR_LCD)
              | (pLcdInfo->pTiming->pcd << GFX_TI_DISPC_DIVISOR_PCD);
    GFX_WRITE_REG32 (GFX_TI_DISPC_DIVISOR, regData);

    regData = (pLcdInfo->pTiming->ipc << GFX_TI_DISPC_POL_FREQ_IPC)
              | (pLcdInfo->pTiming->ihs << GFX_TI_DISPC_POL_FREQ_IHS)
              | (pLcdInfo->pTiming->ivs << GFX_TI_DISPC_POL_FREQ_IVS)
              | (0x1 << GFX_TI_DISPC_POL_FREQ_ONOFF)
              | (pLcdInfo->pTiming->rf << GFX_TI_DISPC_POL_FREQ_RF);
    GFX_WRITE_REG32 (GFX_TI_DISPC_POL_FREQ, regData);

    regData = (pLcdInfo->pTiming->hbp << GFX_TI_DISPC_TIMING_H_HBP)
              | (pLcdInfo->pTiming->hfp << GFX_TI_DISPC_TIMING_H_HFP)
              | (pLcdInfo->pTiming->hsw << GFX_TI_DISPC_TIMING_H_HSW);
#if 0 /* FIXME - hardcode timing params */
    GFX_WRITE_REG32 (GFX_TI_DISPC_TIMING_H, regData);
#else
    GFX_WRITE_REG32 (GFX_TI_DISPC_TIMING_H, 0x300307);
#endif

    regData = (pLcdInfo->pTiming->vbp << GFX_TI_DISPC_TIMING_V_VBP)
              | (pLcdInfo->pTiming->vfp << GFX_TI_DISPC_TIMING_V_VFP)
              | (pLcdInfo->pTiming->vsw << GFX_TI_DISPC_TIMING_V_VSW);
    GFX_WRITE_REG32 (GFX_TI_DISPC_TIMING_V, regData);

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_STNTFT,
                        pLcdInfo->isTft);

    regData = GFX_READ_REG32 (GFX_TI_DISPC_CONTROL);
    regData &= ~(0x3 << GFX_TI_DISPC_CONTROL_TFTDATALINES);
    regData |= pLcdInfo->dataLines << GFX_TI_DISPC_CONTROL_TFTDATALINES;
    GFX_WRITE_REG32 (GFX_TI_DISPC_CONTROL, regData);

    regData = ((yres - 1) << GFX_TI_DISPC_SIZE_LCD_LPP)
              | ((xres - 1) << GFX_TI_DISPC_SIZE_LCD_PPL);
    GFX_WRITE_REG32 (GFX_TI_DISPC_SIZE_LCD, regData);
    GFX_WRITE_REG32 (((unsigned int)virtDispcAddr + 0x078), regData);

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssVid1Dis - disable VID1 output function of DISPC
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssVid1Dis (void)
    {
    GFX_WRITE_BIT32 (GFX_TI_DSS_VID1_ATTRIBUTES, GFX_TI_DSS_VID1_ATTR_ENABLE, 0x0);

    /* finished programming the shadow registers */

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GOLCD, 0x1);
    }

/*******************************************************************************
 *
 * gfxTiDssVid1En - enable VID1 output function of DISPC
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssVid1En (void)
    {
    GFX_WRITE_BIT32 (GFX_TI_DSS_VID1_ATTRIBUTES, GFX_TI_DSS_VID1_ATTR_ENABLE, 0x1);

    /* finished programming the shadow registers */

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GOLCD, 0x1);
    }

/*******************************************************************************
 *
 * gfxTiDssVid1Config - configurate VID1
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssVid1Config
    (
    GFX_TI_DISPC_PIPE_INFO* pVidInfo
    )
    {
    volatile UINT32 regData;

    gfxTiDssVid1Dis ();

    regData = (VID1_FIFO_THRESHOLD_HIGH << GFX_TI_DSS_VID1_FIFO_HIGH)
              | (VID1_FIFO_THRESHOLD_LOW << GFX_TI_DSS_VID1_FIFO_LOW);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_FIFO_THRESHOLD, regData);

    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_BA0, pVidInfo->fbAddress);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_ROW_INC, pVidInfo->rowInc);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_PIXEL_INC, pVidInfo->pixelInc);

    regData = ((pVidInfo->yres - 1)
              << GFX_TI_DSS_VID1_SIZE_Y)
              | ((pVidInfo->xres - 1)
                << GFX_TI_DSS_VID1_SIZE_X);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_SIZE, regData);

    regData = ((pVidInfo->yres - 1)
              << GFX_TI_DSS_VID1_SIZE_Y)
              | ((pVidInfo->xres - 1)
                << GFX_TI_DSS_VID1_SIZE_X);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_PICTURE_SIZE, regData);

    regData = (pVidInfo->format << GFX_TI_DSS_VID1_ATTR_FORMAT)
              | (VID1_BURST_SIZE << GFX_TI_DSS_VID1_ATTR_BURSTSIZE)
              | (0x1 << GFX_TI_DSS_VID1_ATTR_REPLICATION);
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_ATTRIBUTES, regData);

    gfxTiDssVid1En ();

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssDispcLcdEn - enable LCD output function
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssDispcLcdEn
    (
    void
    )
    {
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_LCDENABLE, 0x1);

    /* finished programming the shadow registers */

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GOLCD, 0x1);
    }

/*******************************************************************************
 *
 * gfxTiDssGfxDis - disable GFX output function of DSS
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssGfxDis (void)
    {
    GFX_WRITE_BIT32 (GFX_TI_DSS_GFX_ATTRIBUTES, GFX_TI_DSS_GFX_ATTR_GFXENABLE,
                        0x0);
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_TVENABLE, 0x1);

    /* finished programming the shadow registers */

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GOLCD, 0x1);
    }

/*******************************************************************************
 *
 * gfxTiDssGfxEn - enable GFX output function of DSS
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiDssGfxEn (void)
    {
    GFX_WRITE_BIT32 (GFX_TI_DSS_GFX_ATTRIBUTES, GFX_TI_DSS_GFX_ATTR_GFXENABLE,
                        0x1);

    /* finished programming the shadow registers */

    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_GOTV, 0x1);
    }

/*******************************************************************************
 *
 * gfxTiDssGfxConfig - configurate GFX
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssGfxConfig
    (
    GFX_TI_DISPC_PIPE_INFO* pGfxInfo
    )
    {
    volatile UINT32 regData;

    gfxTiDssGfxDis ();

    regData = (GFX_FIFO_THRESHOLD_HIGH << GFX_TI_DSS_GFX_FIFO_HIGH)
              | (GFX_FIFO_THRESHOLD_LOW << GFX_TI_DSS_GFX_FIFO_LOW);
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_FIFO_THRESHOLD, regData);

    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_BA0, (UINT32)pGfxInfo->fbAddress);
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_ROW_INC, pGfxInfo->rowInc);
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_PIXEL_INC, pGfxInfo->pixelInc);

    regData = ((pGfxInfo->yres - 1)
                << GFX_TI_DSS_GFX_SIZE_GFXSIZEY)
              | ((pGfxInfo->xres- 1)
                  << GFX_TI_DSS_GFX_SIZE_GFXSIZEX);
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_SIZE, regData);

#if 0 /* FIXME - HARD CODE GFX Position for 1920x1080 */
    regData = (((UINT32)(pGfxInfo->yres / 2) & GFX_POSITION_Y_MAX)
               << GFX_TI_DSS_GFX_POSITION_Y)
              | (((UINT32)(pGfxInfo->xres / 2) & GFX_POSITION_X_MAX)
                 << GFX_TI_DSS_GFX_POSITION_X);
#else
    regData = 0x100000;
#endif
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_POSITION, regData);

    regData = (pGfxInfo->format << GFX_TI_DSS_GFX_ATTR_GFXFORMAT)
              | (GFX_BURST_SIZE << GFX_TI_DSS_GFX_ATTR_GFXBURSTSIZE)
              | 0x100
              | (0x1 << GFX_TI_DSS_GFX_ATTR_REPLICATION);
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_ATTRIBUTES, regData);

    gfxTiDssGfxEn ();

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssSetMode - set the display mode
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssSetMode
    (
    GFX_FBDEV*      pDev,
    FB_VIDEO_MODE*  pFbMode,
    GFX_TIDSS_INFO* pDssInfo
    )
    {
    STATUS result;
    GFX_TI_DISPC_PIPE_INFO pipeInfo;
    GFX_TI_DISPC_LCD_INFO  lcdInfo;

    bzero ((char *)&pipeInfo, sizeof (GFX_TI_DISPC_PIPE_INFO));
    bzero ((char *)&lcdInfo, sizeof (GFX_TI_DISPC_LCD_INFO));

    lcdInfo.dataLines   = pDssInfo->lcdInfo.dataLines;
    lcdInfo.isTft       = pDssInfo->lcdInfo.isTft;
    lcdInfo.pTiming     = pDssInfo->lcdInfo.pTiming;

    pipeInfo.fbAddress = *(UINT32*)&(pDev->frontPhysAddr);
    pipeInfo.rowInc = 1;
    pipeInfo.pixelInc = 1;
    pipeInfo.xres = pFbMode->xres;
    pipeInfo.yres = pFbMode->yres;
    pipeInfo.format = (pFbMode->bpp == 32) ? GFX_TI_DSS_VID1_RGB24UP :
                                             GFX_TI_DSS_VID1_RGB16;

    if (ERROR == gfxTiDssDispcClockConfig (&lcdInfo))
        return ERROR;

    if (ERROR == gfxTiDssDispcLcdConfig (&lcdInfo,
                                         pFbMode->xres,
                                         pFbMode->yres))
        return ERROR;

    switch (pDssInfo->pipe)
        {
        case GFX_TI_DISPC_PIPE_GFX:
            result = gfxTiDssGfxConfig (&pipeInfo);
            break;

        case GFX_TI_DISPC_PIPE_VID1:
            result = gfxTiDssVid1Config (&pipeInfo);
            break;

        case GFX_TI_DISPC_PIPE_VID2:
            result = ERROR;
            break;

        }

printf("set hard code hdmi bits\n");
#if 1
    hardCodeHdmi();
#endif
    gfxTiDssDispcLcdEn ();

    return result;
    }

/*******************************************************************************
 *
 * gfxTiDssHwInit - DSS Init
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
STATUS gfxTiDssHwInit
    (
    GFX_FBDEV*      pDev
    )
    {
    GFX_TI_DISPC_OVERLAY_INFO overlayInfo;

    /* enable dss */

    if (ERROR == gfxTiDssInit ())
        return ERROR;

    /* configurate GPIO state of dss */

    gfxTiDssDispcSetGpio (0, 1);
    gfxTiDssDispcSetGpio (1, 1);

    /* configurate alpha blend */

    gfxTiDssGfxSetAlpha (GFX_TI_ALPHA_FULLY_OPAQUE);
    gfxTiDssVid1SetAlpha (GFX_TI_ALPHA_FULLY_OPAQUE);

    overlayInfo.lcdAlphaEn  = TRUE;
    overlayInfo.lcdTckEn    = FALSE;
    overlayInfo.lcdTckSel   = FALSE; /* select GFX */

    gfxTiDssDispcLcdSetOverlay (&overlayInfo);

    /* configurate hardware rotation */

    if (ERROR == gfxTiDssRotInit ())
        return ERROR;

    /* select GFX as graphics layer */

    pDev->info.lcdInfo.isTft = TRUE;
    pDev->info.pipe          = GFX_TI_DISPC_PIPE_GFX;

    switch (pDev->disp)
        {
        case GFX_DISP_TIAM35XX_HDMI:
        case GFX_DISP_TIAM35XX_FLAT_PANEL:
            gfxTiAm35xxEnable ();
            break;
        case GFX_DISP_TIAM37XX_DVI:
        case GFX_DISP_TIAM37XX_FLAT_PANEL:
        case GFX_DISP_TIOMAP35XX_DVI:
        case GFX_DISP_TIOMAP35XX_FLAT_PANEL:
            gfxTiAm37xxEnable (pDev);
            break;
        }

    gfxTiDssEnableVsync (pDev);

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssSetVideoModeEx - set the display mode
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
STATUS gfxTiDssSetVideoModeEx
    (
    GFX_FBDEV*      pDev,
    FB_VIDEO_MODE*  pFbMode
    )
    {
    int i;
    GFX_TIDSS_INFO info;

    /* get previous info */
    bcopy (&(pDev->info), &info, sizeof (GFX_TIDSS_INFO));

    /* select timing parameter */
    switch (pDev->disp)
        {
        case GFX_DISP_TIAM35XX_FLAT_PANEL:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_16;
            info.lcdInfo.pTiming = &(gfxTiAm35xxLcdTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiAm35xxLcdTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                (info.lcdInfo).pTiming = &(gfxTiAm35xxLcdTiming[i+1]);
                }
            break;

        case GFX_DISP_TIAM35XX_HDMI:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_24;
            info.lcdInfo.pTiming = &(gfxTiAm35xxHdmiTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiAm35xxHdmiTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                info.lcdInfo.pTiming = &(gfxTiAm35xxHdmiTiming[i+1]);
                }
            break;

        case GFX_DISP_TIAM57XX_HDMI:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_24;
            info.lcdInfo.pTiming = &(gfxTiAm57xxHdmiTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiAm57xxHdmiTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                info.lcdInfo.pTiming = &(gfxTiAm57xxHdmiTiming[i+1]);
                }
            if (info.lcdInfo.pTiming->xres == 480)
		{
		    gHdmiConfig_480x800=TRUE;
		}
            break;

        case GFX_DISP_TIAM37XX_FLAT_PANEL:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_18;
            info.lcdInfo.pTiming = &(gfxTiAm37xxLcdTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiAm37xxLcdTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                (info.lcdInfo).pTiming = &(gfxTiAm37xxLcdTiming[i+1]);
                }
            break;

        case GFX_DISP_TIAM37XX_DVI:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_24;
            info.lcdInfo.pTiming = &(gfxTiAm37xxDviTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiAm37xxDviTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                info.lcdInfo.pTiming = &(gfxTiAm37xxDviTiming[i+1]);
                }
            break;

        case GFX_DISP_TIOMAP35XX_FLAT_PANEL:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_18;
            info.lcdInfo.pTiming = &(gfxTiOmap35xxLcdTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiOmap35xxLcdTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                (info.lcdInfo).pTiming = &(gfxTiOmap35xxLcdTiming[i+1]);
                }
            break;

        case GFX_DISP_TIOMAP35XX_DVI:
            info.lcdInfo.dataLines = GFX_TIDSS_DATA_LINES_24;
            info.lcdInfo.pTiming = &(gfxTiOmap35xxDviTiming[0]);
            for (i = 0; i < NELEMENTS (gfxTiOmap35xxDviTiming); i ++)
                {
                if ((info.lcdInfo.pTiming->xres == pFbMode->xres) &&
                    (info.lcdInfo.pTiming->yres == pFbMode->yres))
                    break;
                info.lcdInfo.pTiming = &(gfxTiOmap35xxDviTiming[i+1]);
                }
            break;
        }

    gfxTiDssSetMode (pDev, pFbMode, &info);

    switch (pDev->disp)
        {
        case GFX_DISP_TIAM35XX_FLAT_PANEL:
            gfxTiAm35xxLcdPowerOn ();
            (void)taskDelay (100);
            gfxTiAm35xxLcdBLPowerOn ();
            break;

        case GFX_DISP_TIAM35XX_HDMI:
            gfxTiAm35xxLcdBLPowerOff ();
            (void)taskDelay (100);
            gfxTiAm35xxLcdPowerOff ();
            break;

        case GFX_DISP_TIAM37XX_FLAT_PANEL:
        case GFX_DISP_TIOMAP35XX_FLAT_PANEL:
            gfxTiAm37xxDviDisable ();
            if (pFbMode->xres > OMAP35XX_LCD_QVGA_WIDTH)
                gfxTiAm37xxLcdVgaModeEnable ();
            else
                gfxTiAm37xxLcdVgaModeDisable ();
            gfxTiAm37xxLcdEnable ();
            break;

        case GFX_DISP_TIAM37XX_DVI:
        case GFX_DISP_TIOMAP35XX_DVI:
            gfxTiAm37xxLcdVgaModeDisable ();
            gfxTiAm37xxLcdDisable ();
            gfxTiAm37xxDviEnable ();
            break;
        }

    bcopy (&info, &(pDev->info), sizeof (GFX_TIDSS_INFO));

    return OK;
    }

/*******************************************************************************
 *
 * gfxTiDssSetFbAddr - sets the frame buffer address
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
STATUS gfxTiDssSetFbAddr
    (
    GFX_FBDEV*      pDev
    )
    {
    GFX_WRITE_REG32 (GFX_TI_DSS_VID1_BA0, *(UINT32*)&(pDev->frontPhysAddr));
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_BA0, *(UINT32*)&(pDev->frontPhysAddr));
    gfxTiDssGfxEn ();
    GFX_DELAY(500000);

    return OK;
    }

#if defined(GFX_VSYNC_IRQ)
/*******************************************************************************
 *
 * gfxTiDssIntHandlerVsync - interrupt handler
 *
 * RETURNS: N/A
 */
void gfxTiDssIntHandlerVsync
    (
    GFX_FBDEV*      pDev
    )
    {
    if (pDev->needSwap)
        {
        pDev->whenSwap--;
        if (pDev->whenSwap <= 0)
            {
            pDev->needSwap = FALSE;
            gfxTiDssSetFbAddr (pDev);
            (void)semGive ((SEM_ID)pDev->vsync);
            }
        }
    else if (pDev->needVsync)
        {
        pDev->needVsync = FALSE;
        (void)semGive ((SEM_ID)pDev->vsync);
        }

    gfxTiDssDispcCleanIrq (GFX_TI_DISPC_IRQSTATUS_VSYNC);
    }
#endif




/*******************************************************************************
 *
 * hardCodeHdmi - Hard Code HDMI registers
 *
 * HDMI PHY and Core Documentation for AM5728 is not available to public
 * Due to hdmi.org licensing.
 * Use hard coded values here instead of commented code.
 *
 * RETURNS: N/A
 */
void hardCodeHdmi(void)
{
int count=0;
/* First...set a few pin mux */
/* Set pin mux */

if (gHdmiConfig_480x800 != TRUE)
{
   *(UINT32 *)((unsigned int) virtClkAddr +0x37bc)=0xe006;
}
else
{
   *(UINT32 *)((unsigned int) virtClkAddr +0x37bc)=0xe0006;
}
*(UINT32 *)((unsigned int) virtClkAddr +0x3808)=0x50001;
*(UINT32 *)((unsigned int) virtClkAddr +0x380c)=0x50001;

taskDelay(0);
*(UINT32 *)((unsigned int) virtDssAddr +0x040)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtDssAddr +0x040)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x010)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157ec)=0x0;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e4)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f4)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f8)=0x7b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157fc)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15800)=0x90;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15804)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15808)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1580c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15810)=0x23;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15814)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157c8)=0x50;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e8)=0x30;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0x8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0x8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f0)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x11;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x12;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x13;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x14;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x15;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x16;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x17;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x18;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x19;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x21;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x22;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x23;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x24;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x25;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x26;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x27;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x28;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x29;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x30;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x31;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x32;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x33;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x34;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x35;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x36;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x37;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x38;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x39;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x40;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x41;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x42;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x43;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x44;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x45;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x47;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x48;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x49;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x50;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x51;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x52;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x53;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x54;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x55;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x56;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x57;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x58;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x59;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x60;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x61;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x62;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x63;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x64;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x65;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x66;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x67;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x68;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x69;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x70;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x71;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x72;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x73;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x74;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x75;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x76;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x77;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x78;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x79;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f0)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x80;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x81;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x82;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x83;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x84;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x85;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x86;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x87;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x89;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x90;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x91;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x92;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x93;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x94;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x95;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x96;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x97;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x98;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x99;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xaa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xab;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xac;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xad;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xae;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xaf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xba;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xca;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xce;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xda;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xde;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xea;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xeb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xec;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xed;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xee;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xef;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
}
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xc8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x010)=0xc;
taskDelay(0);
taskDelay(0);
taskDelay(0);
/* FIXME
[drm] Enabling DMM ywrap scrolling
taskDelay(0);
*/
/* XXX missing this *(UINT32 *)((unsigned int) ds0xf18c4040)=0x0; */

*(UINT32 *)((unsigned int) virtHdmiAddr + 0x030)=0xffffffff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x028)=0x0;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x040)=0x8;
taskDelay(0);
taskDelay(0);
/* write Hdmi PLL regs */
if (gHdmiConfig_480x800 != TRUE)
{
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0xc)=0x4a40e;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x10)=0x602004;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x14)=0x1800;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x20)=0x40000;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x8)=0x1; /* debug said 0x40000 */
}
else
{
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0xc)=0x2580e;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x10)=0x602004;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x14)=0xc00;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x20)=0xc0000;
   *(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x8)=0x1; /* debug said 0xc0000 */
} 
count=0;
while(*(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x8) & 0x1)
{
   if(count++==5)
      break;
   taskDelay(30);
/* wait for GO to clear */
}
if(count == 5)
   printf("wait for GO to clear %d 0f 5\n",count);

count=0;
while(!(*(UINT32 *)((unsigned int) virtHdmiPllAddr + 0x4) & 0x2))
{
if(count++==5)
	break;
taskDelay(30);
}
if(count == 5)
   printf("cannot lock DSS DPLL %d 0f 5\n",count);

taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x1c)=0x800;
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
   *(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x00)=0x40000000;
}
else
{
   *(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x00)=0x0;
}
*(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x04)=0xf0000000;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x0c)=0x100000;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiPhyAddr + 0x0c)=0x100000;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x040)=0x4a;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x07fc)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x201c)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4348)=0xe7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4358)=0xfb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4368)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0xc408)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0xd418)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x17408)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xc8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0xc018)=0xf3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0410)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x041c)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0400)=0xe7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0404)=0xfb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0408)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x040c)=0x7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0418)=0x7f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0410)=0xff;
if (gHdmiConfig_480x800 != TRUE)
{
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x068)=0x940582b;
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x06c)=0x2400405;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x100;
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x060)=0x4380780;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x1c1;
}
else
{
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x068)=0xf00605;
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x06c)=0x300303;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x100;
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x060)=0x32001e0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x1f1;
}
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10408)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1040c)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10410)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10414)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10418)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1041c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10420)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10424)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10428)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1042c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10430)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10434)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10438)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1043c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10440)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10444)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10448)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1044c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10450)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10454)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10458)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1045c)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10460)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10464)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10010)=0x1;
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4000)=0x70;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4008)=0x7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4004)=0x80;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4018)=0x4;
}
else
{
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4000)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4008)=0x01;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4004)=0xe0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4018)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4014)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4010)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x400c)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x401c)=0x9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4024)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4020)=0x6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4030)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x402c)=0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4028)=0x6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4034)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4000)=0x10;
taskDelay(0);
}



if (gHdmiConfig_480x800 != TRUE)
{
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10408)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1040c)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10410)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10414)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10418)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1041c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10420)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10424)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10428)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1042c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10430)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10434)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10438)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1043c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10440)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10444)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10448)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1044c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10450)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10454)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10458)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1045c)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10460)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10464)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10010)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4000)=0x70;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4008)=0x7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4004)=0x80;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4018)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4014)=0x38;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4010)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x400c)=0x18;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x401c)=0x2d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4024)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4020)=0x58;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4030)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x402c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4028)=0x2c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4034)=0x5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4000)=0x78;
taskDelay(0);
}

*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4380)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2004)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2010)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2010)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2010)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2008)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2008)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x2010)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10404)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0800)=0x1;
if (gHdmiConfig_480x800 != TRUE)
{
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4064)=0x60;
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4068)=0x28;
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x406c)=0x4;
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4070)=0x10;
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x405c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4380)=0x10;
}
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4044)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4048)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x404c)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4050)=0xb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4054)=0x16;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x4058)=0x21;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10004)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x10004)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x07fc)=0x0;
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x40)=0x18308;
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x78)=0x437077f;
   taskDelay(0);
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x40)=0x1830a;
}
else
{
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x40)=0x18308;
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x78)=0x031F01DF;
   taskDelay(0);
   *(UINT32 *)((unsigned int)virtDispcAddr + 0x40)=0x1830a;
}
taskDelay(0);
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
   *(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x800101c1;
}
else
{
   *(UINT32 *)((unsigned int) virtHdmiAddr + 0x050)=0x800101f1;
}
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x02c)=0x6000000;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x028)=0x2000000;
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x040)=0x9a;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
#if 0 /* FIXME WRSFCE */
*(UINT32 *)((unsigned int)virtDispcAddr + 0x1c)=0x10d640;
#endif
taskDelay(0);
/*
Console: switching to colour frame buffer device 240x67
omapdrm omapdrm.0: fb0: omapdrm frame buffer device
*/
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x010)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157ec)=0x0;
taskDelay(0);
#if 0 /* FIXME WRSFCE */
*(UINT32 *)((unsigned int)virtDispcAddr + 0x1c)=0x1694d64e;
taskDelay(0);
#endif
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e4)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f4)=0x0;
taskDelay(0);
/*
[drm] Initialized omapdrm 1.0.0 20110917 on minor 0
*/
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f8)=0x7b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157fc)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15800)=0x90;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15804)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15808)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x1580c)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15810)=0x23;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x15814)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157c8)=0x50;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e8)=0x30;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x8c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x8c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0x8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f0)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x10;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x11;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x12;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x13;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x14;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x15;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x16;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x17;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x18;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x19;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x1f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x20;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x21;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x22;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x23;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x24;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x25;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x26;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x27;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x28;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x29;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x2f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x30;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x31;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x32;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x33;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x34;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x35;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x36;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x37;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x38;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x39;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x3f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x40;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x41;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x42;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x43;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x44;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x45;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x46;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x47;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x48;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x49;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x4f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x50;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x51;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x52;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x53;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x54;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x55;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x56;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x57;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x58;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x59;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x5f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x60;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x61;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x62;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x63;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x64;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x65;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x66;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x67;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x68;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x69;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x6f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x70;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x71;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x72;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x73;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x74;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x75;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x76;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x77;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x78;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x79;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x7f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
if (gHdmiConfig_480x800 != TRUE)
{
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157f0)=0x0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x80;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x81;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x82;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x83;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x84;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x85;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x86;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x87;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x88;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x89;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x8f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x90;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x91;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x92;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x93;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x94;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x95;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x96;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x97;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x98;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x99;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9a;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9b;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9c;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9d;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9e;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0x9f;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xa9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xaa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xab;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xac;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xad;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xae;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xaf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xb9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xba;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xbf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xc9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xca;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xce;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xcf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xd9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xda;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xde;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xdf;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xe9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xea;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xeb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xec;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xed;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xee;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xef;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf0;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf1;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf2;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf4;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf5;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf6;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf7;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xf9;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfa;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfb;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfd;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xfe;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr+0x0414)=0x3;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157cc)=0xff;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157d8)=0x1;
taskDelay(0);
}
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xc8;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157e0)=0xcc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiCoreAddr +0x157dc)=0xc;
taskDelay(0);
*(UINT32 *)((unsigned int) virtHdmiAddr + 0x010)=0xc;
taskDelay(0);
}






unsigned int getGfxPos(void)
{
    return (GFX_READ_REG32 (GFX_TI_DSS_GFX_POSITION));
}
 
int setGfxPos(unsigned int val)
{          
int reg_val;
 
    /* Disable screen by clearing GFX_TI_DISPC_CONTROL_LCDENABLE bit */
    reg_val =    GFX_READ_REG32 (GFX_TI_DISPC_CONTROL);
    GFX_WRITE_REG32 (GFX_TI_DISPC_CONTROL,  reg_val & 0xfffffffe);
 
    taskDelay(20);
 
    /* set new screen x/y pos */
    GFX_WRITE_REG32 (GFX_TI_DSS_GFX_POSITION,val);
    taskDelay(20);
 
    /* Re-enable screen */
    GFX_WRITE_BIT32 (GFX_TI_DISPC_CONTROL, GFX_TI_DISPC_CONTROL_LCDENABLE, 0x1);
    return (0);
}

