/* vxbFdtUsbPhyAltSocGen5.h - Altera Cyclone-V USB hardware definitions */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17mar15,pkr  update boot timing code
12sep14,tnk  created
*/

#ifndef __INCvxbFdtUsbPhyAltSocGen5h
#define __INCvxbFdtUsbPhyAltSocGen5h

#ifdef __cplusplus
extern "C"
{
#endif

#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <vxbUsbPhyLib.h>

#ifdef _WRS_CONFIG_BOOT_TIMING
#include <bootTiming.h>
#else
#define BTT(str) 
#endif

#define ALT_SOC_GEN5_USB_PHY_NAME "altr-usb-phy"

#define USB_DWC_GOTGCTL        0x000
#define USB_DWC_GOTGINT        0x004
#define USB_DWC_GAHBCFG        0x008
#define USB_DWC_GUSBCFG        0x00C
#define USB_DWC_GRSTCTL        0x010
#define USB_DWC_GINTSTS        0x014
#define USB_DWC_GINTMSK        0x018
#define USB_DWC_GHWCFG1        0x044
#define USB_DWC_GHWCFG2        0x048
#define USB_DWC_GHWCFG3        0x04c
#define USB_DWC_GHWCFG4        0x050
#define USB_DWC_HPTXFSIZ       0x100

#define USB_DWC_HCFG           0x400
#define USB_DWC_HPRT           0x440
#define USB_DWC_PCGCCTL        0xe00

#define USB_DWC_DPTX_FSIZ_DIPTXF(x)         (0x104 + (x) * 4)    /* 15 => x > 1 */

#define USB_DWC_AHBCFG_BURST_LEN(x)         (x << 1)

#define USB_DWC_GAHBCFG_INT_DMA_BURST_INCR  1

#define USB_DWC_USBCFG_FRC_HST_MODE         (1 << 29)
#define USB_DWC_USBCFG_TERM_SEL_DL_PULSE    (1 << 22)
#define USB_DWC_USBCFG_ULPI_EXT_VBUS_DRV    (1 << 20)
#define USB_DWC_USBCFG_ULPI_CLK_SUS_M       (1 << 19)
#define USB_DWC_USBCFG_ULPI_FSLS            (1 << 17)
#define USB_DWC_USBCFG_HNP_CAP              (1 << 9)
#define USB_DWC_USBCFG_SRP_CAP              (1 << 8)
#define USB_DWC_USBCFG_DDRSEL               (1 << 7)
#define USB_DWC_USBCFG_ULPI_UTMI_SEL        (1 << 4)
#define USB_DWC_USBCFG_PHYIF                (1 << 3)

#define USB_DWC_AHBCFG_DMA_ENA              (1 << 5)
#define USB_DWC_AHBCFG_GLBL_INT_MASK        (1 << 0)

#define USB_DWC_INTMSK_WKP                  (1 << 31)
#define USB_DWC_INTMSK_NEW_SES_DET          (1 << 30)
#define USB_DWC_INTMSK_SES_DISCON_DET       (1 << 29)
#define USB_DWC_INTMSK_CON_ID_STS_CHG       (1 << 28)
#define USB_DWC_INTMSK_USB_SUSP             (1 << 11)
#define USB_DWC_INTMSK_RXFIFO_NOT_EMPT      (1 << 4)
#define USB_DWC_INTMSK_OTG                  (1 << 2)
#define USB_DWC_INTMSK_MODE_MISMTC          (1 << 1)

#define USB_DWC_RSTCTL_AHB_IDLE             (1 << 31)
#define USB_DWC_RSTCTL_SFT_RST              (1 << 1)

#define USB_PHY_RST_AHB_IDLE_TIMEOUT        (100000)
#define USB_PHY_RST_TIMEOUT                 (10000)

typedef struct alt_soc_gen5_usb_phy_ctrl
    {
    VXB_DEV_ID       pDev;
    void *           regBase;
    void *           regHandle;
    } ALT_SOC_GEN5_USB_PHY_CTRL;

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbFdtUsbPhyAltSocGen5h */
