/* gfxTiAm37xx.c - TI AM37xx Graphics Controller ios driver */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */
    
/*
modification history
--------------------
24jan14,mgc  Modified for VxWorks 7 release
*/

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <gfxTiDssDrv.h>
#include <gfxTiDss.h>
#include <gfxTiAm37xx.h>

/* forward declarations */

IMPORT STATUS gfxTiDssI2cRead (UINT8, UINT8, char*);
IMPORT STATUS gfxTiDssI2cWrite (UINT8, char*);

/*******************************************************************************
 *
 * gfxTiAm37xxPinMux - set graphics pins mux mode
 *
 * This rountine sets graphics pins mux mode.
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiAm37xxPinMux
    (
    GFX_FBDEV*     pDev
    )
    {
    volatile UINT16 regData;
    UINT8           buf[2];

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_INI);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_INI, regData);

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_ENVDD);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_ENVDD, regData);

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_QVGA);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_QVGA, regData);

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_RESB);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_RESB, regData);

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_LR);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_LR, regData);

    regData = GFX_READ_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_UD);
    regData &= OMAP35XX_CONTROL_PADCONF_PIN_MASK;
    regData |= OMAP35XX_CONTROL_PADCONF_GPIO_ON;
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_LCD_UD, regData);

    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D0,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D1,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D2,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D3,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D4,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D5,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D6,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D7,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D8,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D9,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D10,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D11,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D12,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D13,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D14,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D15,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D16,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D17,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);

    switch (pDev->disp)
        {
        case GFX_DISP_TIAM37XX_DVI:
        case GFX_DISP_TIAM37XX_FLAT_PANEL:

        /* AM37XX uses DSS_D18 - DSS_D23 as DSS_D0 - DSS_D5 */

        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D18,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D19,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D20,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D21,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D22,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D23,
                              (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));

        break;

        case GFX_DISP_TIOMAP35XX_DVI:
        case GFX_DISP_TIOMAP35XX_FLAT_PANEL:

        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D18,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D19,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D20,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D21,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D22,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D23,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);

        /* enable output pins power */

        buf[0] = OMAP35XX_POWER_IC_VPLL2_DEV_GRP;
        buf[1] = 0xE0;
        gfxTiDssI2cWrite (OMAP35XX_POWER_IC_PM_ADDR, (char *)buf);

        buf[0] = OMAP35XX_POWER_IC_VPLL2_DEDICATED;
        buf[1] = 0x05;
        gfxTiDssI2cWrite (OMAP35XX_POWER_IC_PM_ADDR, (char *)buf);

        break;

        default:
        return;
        }

    if (pDev->disp == GFX_DISP_TIAM37XX_DVI)
        {
        /* AM37XX uses DSS_D18_2 - DSS_D23_2 as DSS_D18 - DSS_D23 */

        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D18_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D19_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D20_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D21_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D22_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_D23_2,
                                  (OMAP35XX_CONTROL_PADCONF_LCD_ON | 0x3));
        }

    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_PCLK,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_VSYNC,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_HSYNC,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    GFX_WRITE_REG16 (OMAP35XX_CONTROL_PADCONF_DSS_ACBIAS,
                              OMAP35XX_CONTROL_PADCONF_LCD_ON);
    }

/*******************************************************************************
 *
 * gfxTiAm37xxEnable - enable TI display interface
 *
 * This rountine implements enable display interface and related pins. If use
 * OMAP flat panel, it will initialize the LCD.
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxEnable
    (
    GFX_FBDEV*     pDev
    )
    {
    gfxTiAm37xxPinMux (pDev);
    }

/*******************************************************************************
 *
 * gfxTiAm37xxDviDisable - disable DVI
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxDviDisable (void)
    {
    UINT8   buf[2];
    UINT8   oldValue = 0;

    gfxTiDssI2cRead (OMAP35XX_POWER_IC_IO_ADDR, OMAP35XX_POWER_IC_IO_DIR,
              (char *)&oldValue);

    buf[0] = OMAP35XX_POWER_IC_IO_DIR;
    buf[1] = 0x80 | oldValue;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_IO_ADDR, (char *)buf);
                                                           
    gfxTiDssI2cRead (OMAP35XX_POWER_IC_IO_ADDR, OMAP35XX_POWER_IC_IO_DATA,
              (char *)&oldValue);

    buf[0] = OMAP35XX_POWER_IC_IO_DATA;
    buf[1] = oldValue & 0x7F;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_IO_ADDR, (char *)buf);
    }

/*******************************************************************************
 *
 * gfxTiAm37xxDviEnable - enable DVI
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxDviEnable (void)
    {
    UINT8   buf[2];
    UINT8   oldValue = 0;

    gfxTiDssI2cRead (OMAP35XX_POWER_IC_IO_ADDR, OMAP35XX_POWER_IC_IO_DIR,
              (char *)&oldValue);

    buf[0] = OMAP35XX_POWER_IC_IO_DIR;
    buf[1] = 0x80 | oldValue;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_IO_ADDR, (char *)buf);

    gfxTiDssI2cRead (OMAP35XX_POWER_IC_IO_ADDR, OMAP35XX_POWER_IC_IO_DATA,
              (char *)&oldValue);

    buf[0] = OMAP35XX_POWER_IC_IO_DATA;
    buf[1] = 0x80 | oldValue;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_IO_ADDR, (char *)buf);
    }

/*******************************************************************************
 *
 * gfx37xxLcdDisable - disable LCD
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxLcdDisable (void)
    {
    UINT8   buf[2];

    /* disable back light */
    buf[0] = OMAP35XX_POWER_IC_LED_PWMAON;
    buf[1] = 0x7F;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);

    buf[0] = OMAP35XX_POWER_IC_LED_PWMAOFF;
    buf[1] = 0x01;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);

    buf[0] = OMAP35XX_POWER_IC_LED_LEDEN;
    buf[1] = 0x33;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);

    /* disable LCD power */

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_ENVDD_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_ENVDD_BIT, 1);

    /* disable power on control to LCD */

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_INI_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_INI_BIT, 0);
    }

/*******************************************************************************
 *
 * gfxTiAm37xxLcdEnable - enable LCD
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxLcdEnable (void)
    {
    UINT8   buf[2];

    /* disable LCD first */

    gfxTiAm37xxLcdDisable ();

    (void)taskDelay (100);

    /* enable LCD power */

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_ENVDD_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_ENVDD_BIT, 0);

    /* enable power on control to LCD */

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_INI_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_INI_BIT, 1);

    /* reset LCD */

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_RESET_BIT, 0);

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_RESET_BIT, 1);
    (void)taskDelay (100);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_RESET_BIT, 0);
    (void)taskDelay (100);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_RESET_BIT, 1);

    /* select the scanning directions */

    GFX_WRITE_BIT32 (OMAP35XX_CLOCK_MANAGER_FCLKEN_WKUP,
                                  OMAP35XX_CM_FCLKEN_WKUP_EN_GPIO1, 1);
    GFX_WRITE_BIT32 (OMAP35XX_CLOCK_MANAGER_ICLKEN_WKUP,
                                  OMAP35XX_CM_ICLKEN_WKUP_EN_GPIO1, 1);

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO1 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_LR_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO1 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_LR_BIT, 1);

    GFX_WRITE_BIT32 ((OMAP35XX_GPIO1 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_UD_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO1 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_UD_BIT, 1);

    /* enable back light */

    buf[0] = OMAP35XX_POWER_IC_LED_PWMAON;
    buf[1] = 0x01;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);

    buf[0] = OMAP35XX_POWER_IC_LED_PWMAOFF;
    buf[1] = 0x7F;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);

    buf[0] = OMAP35XX_POWER_IC_LED_LEDEN;
    buf[1] = 0x33;
    gfxTiDssI2cWrite (OMAP35XX_POWER_IC_LED_ADDR, (char *)buf);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdVgaModeDisable - disable LCD VGA mode
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxLcdVgaModeDisable (void)
    {
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_QVGA_NVGA_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_QVGA_NVGA_BIT, 1);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdVgaModeEnable - enable LCD VGA mode
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm37xxLcdVgaModeEnable (void)
    {
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_OE),
                                  OMAP35XX_LCD_QVGA_NVGA_BIT, 0);
    GFX_WRITE_BIT32 ((OMAP35XX_GPIO5 + OMAP35XX_GPIO_DATAOUT),
                                  OMAP35XX_LCD_QVGA_NVGA_BIT, 0);
    }
