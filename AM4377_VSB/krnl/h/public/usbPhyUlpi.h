/* usbPhyUlpi.h - USB ULPI PHY Common Header File */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,09mar11,w_x  add ULPI DEBUG register bit masks
01b,18feb11,w_x  remove duplicated definitions
01a,30may10,w_x  written
*/

#ifndef __INCusbPhyUlpih
#define __INCusbPhyUlpih

#include <vxWorks.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* Define ULPI Phy Register Address */

#define USB_ULPI_VID_LOW            0x00 /* Vendor ID Low */
#define USB_ULPI_VID_HIGH           0x01 /* Vendor ID High */
#define USB_ULPI_PID_LOW            0x02 /* Product ID Low */
#define USB_ULPI_PID_HIGH           0x03 /* Product ID High */
#define USB_ULPI_FUNCTION_CTL       0x04 /* Function Control */
#define USB_ULPI_INTERFACE_CTL      0x07 /* Interface Control */
#define USB_ULPI_OTG_CTL            0x0A /* OTG Control */
#define USB_ULPI_INTR_EN_RISING     0x0D /* USB Interrupt Enable Rising */
#define USB_ULPI_INTR_EN_FALLING    0x10 /* USB Interrupt Enable Falling */
#define USB_ULPI_INTR_STATUS        0x13 /* USB Interrupt Status */
#define USB_ULPI_INTR_LATCH         0x14 /* USB Interrupt Latch */
#define USB_ULPI_DEBUG              0x15 /* Debug */
#define USB_ULPI_SCRATCH            0x16 /* Scratch Register */
#define USB_ULPI_CARKIT_CTL         0x19 /* Carkit Control (Optional) */
#define USB_ULPI_CARKIT_INTR_DELAY  0x1C /* Carkit Interrupt Delay (Optional) */
#define USB_ULPI_CARKIT_INTR_EN     0x1D /* Carkit Interrupt Enable (Optional) */
#define USB_ULPI_CARKIT_INTR_STATUS 0x20 /* Carkit Interrupt Status (Optional) */
#define USB_ULPI_CARKIT_INTR_LATCH  0x21 /* Carkit Interrupt Latch (Optional) */
#define USB_ULPI_CARKIT_PULSE_CTL   0x22 /* Carkit Pulse Control (Optional) */
#define USB_ULPI_TRANS_POS_WIIDTH   0x25 /* Transmit Positive Width (Optional) */
#define USB_ULPI_TRANS_NEG_WIIDTH   0x26 /* Transmit Negative Width (Optional) */
#define USB_ULPI_RECEV_POL_RECOVERY 0x27 /* Receive Polarity Recovery (Optional) */

#define USB_ULPI_SET_ADJ            0x01
#define USB_ULPI_CLR_ADJ            0x02

/* ULPI PHY OTG Control Register bits */

#define ULPI_OTG_CTL_USE_EXT_VBUS_IND   (1 << 7)    /* Use ext. Vbus indicator */
#define ULPI_OTG_CTL_DRV_VBUS_EXT       (1 << 6)    /* Drive Vbus external */
#define ULPI_OTG_CTL_DRV_VBUS           (1 << 5)    /* Drive Vbus */
#define ULPI_OTG_CTL_CHRG_VBUS          (1 << 4)    /* Charge Vbus */
#define ULPI_OTG_CTL_DISCHRG_VBUS       (1 << 3)    /* Discharge Vbus */
#define ULPI_OTG_CTL_DM_PULL_DOWN       (1 << 2)    /* enable DM Pull Down */
#define ULPI_OTG_CTL_DP_PULL_DOWN       (1 << 1)    /* enable DP Pull Down */
#define ULPI_OTG_CTL_ID_PULL_UP         (1 << 0)    /* enable ID Pull Up */

/* FUNC_CTRL */

#define ULPI_FUNC_CTRL_SUSPENDM        (1 << 6)    /* Active low PHY suspend.*/
#define ULPI_FUNC_CTRL_RESET           (1 << 5)    /* Active high transceiver reset.*/
#define ULPI_FUNC_CTRL_OPMODE_MSK      (3 << 3)    /* Select the required bit encoding style.*/
#define ULPI_FUNC_CTRL_XCVRSELECT_MSK  (3 << 0)    /* Select the required transceiver speed.*/
#define ULPI_FUNC_CTRL_TERMSELECT      (1 << 2)    /* Controls the internal 1.5-k pullup resistor 
                                                    * and 45 HS terminations */

/* IFC_CTRL - Enables alternate interfaces and PHY features */

#define ULPI_IFC_CTRL_INTERFACE_PROTECT_DISABLE (1 << 7)
#define ULPI_IFC_CTRL_AUTORESUME                (1 << 4)
#define ULPI_IFC_CTRL_CLOCK_SUSPENDM            (1 << 3)
#define ULPI_IFC_CTRL_CARKITMODE                (1 << 2)
#define ULPI_IFC_CTRL_FSLSSERIALMODE_3PIN       (1 << 1)

/*
 * 0x0: Normal operation
 * 0x1: Nondriving
 * 0x2: Disable bit-stuff and NRZI encoding
 * 0x3: Reserved
 */

#define ULPI_OPMODE_NORMAL             ((0) << 3)
#define ULPI_OPMODE_NONDRIVING         ((1) << 3)
#define ULPI_OPMODE_NO_BITSTUFF_NRZI   ((2) << 3)

/*
 * 0x0: Enable HS transceiver
 * 0x1: Enable FS transceiver
 * 0x2: Enable LS transceiver
 * 0x3: Enable FS transceiver for LS packets
 * (FS preamble is automatically prepended.)
 */

#define ULPI_XCVRSELECT_HS      0
#define ULPI_XCVRSELECT_FS      1
#define ULPI_XCVRSELECT_LS      2
#define ULPI_XCVRSELECT_FS_LS   3

/*
 * Current value of UTMI+ Hostdisconnect output.
 * Applicable only in host mode. Automatically reset to 0b when
 * Low Power Mode is entered.
 */

#define ULPI_INTR_Hostdisconnect     (UINT8)(1 << 0)

/*
 * Current value of UTMI+ VbusValid output.
 */

#define ULPI_INTR_VbusValid          (UINT8)(1 << 1)


/*
 * Current value of UTMI+ SessValid output. SessValid is
 * the same as UTMI+ AValid.
 */

#define ULPI_INTR_SessValid          (UINT8)(1 << 2)

/* Current value of UTMI+ SessEnd output. */

#define ULPI_INTR_SessEnd            (UINT8)(1 << 3)

/*
 * Current value of UTMI+ IdGnd output. IdGnd is valid
 * 50ms after IdPullup is set to 1b, otherwise IdGnd is
 * undefined and should be ignored.
 */

#define ULPI_INTR_IdGnd              (UINT8)(1 << 4)

#define ULPI_INTR_ALL                        \
            (ULPI_INTR_Hostdisconnect |      \
             ULPI_INTR_VbusValid |           \
             ULPI_INTR_SessValid |           \
             ULPI_INTR_SessEnd |             \
             ULPI_INTR_IdGnd)

/* ULPI DEBUG register bit masks */

#define ULPI_DEBUG_LINESTATE0         (UINT8)(1 << 0)
#define ULPI_DEBUG_LINESTATE1         (UINT8)(1 << 1)
#define ULPI_DEBUG_LINESTATE_SE0      (UINT8)(0)
#define ULPI_DEBUG_LINESTATE_FSK      (UINT8)(1)
#define ULPI_DEBUG_LINESTATE_FSJ      (UINT8)(2)
#define ULPI_DEBUG_LINESTATE_SE1      (UINT8)(3)
#define ULPI_DEBUG_LINESTATE_MSK      (UINT8)(3)

#ifdef __cplusplus
}
#endif

#endif /* __INCusbPhyUlpih */

