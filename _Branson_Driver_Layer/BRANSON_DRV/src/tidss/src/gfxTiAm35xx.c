/* gfxTiAm35xx.c - TI AM35xx Graphics Controller ios driver */

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
22mar13,af  Written
*/

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <gfxTiDssDrv.h>
#include <gfxTiDss.h>
#include <gfxTiAm35xx.h>

/*******************************************************************************
 *
 * gfxTiAm35xxPinMux - set graphics pins mux mode
 *
 * This rountine sets graphics pins mux mode.
 *
 * RETURNS: N/A
 *
 */
LOCAL void gfxTiAm35xxPinMux
    (
    void
    )
    {
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D0_ADDR, AM35XX_PIN_DSS_D0_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D1_ADDR, AM35XX_PIN_DSS_D1_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D2_ADDR, AM35XX_PIN_DSS_D2_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D3_ADDR, AM35XX_PIN_DSS_D3_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D4_ADDR, AM35XX_PIN_DSS_D4_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D5_ADDR, AM35XX_PIN_DSS_D5_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D6_ADDR, AM35XX_PIN_DSS_D6_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D7_ADDR, AM35XX_PIN_DSS_D7_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D8_ADDR, AM35XX_PIN_DSS_D8_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D9_ADDR, AM35XX_PIN_DSS_D9_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D10_ADDR, AM35XX_PIN_DSS_D10_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D11_ADDR, AM35XX_PIN_DSS_D11_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D12_ADDR, AM35XX_PIN_DSS_D12_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D13_ADDR, AM35XX_PIN_DSS_D13_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D14_ADDR, AM35XX_PIN_DSS_D14_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D15_ADDR, AM35XX_PIN_DSS_D15_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D16_ADDR, AM35XX_PIN_DSS_D16_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D17_ADDR, AM35XX_PIN_DSS_D17_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D18_ADDR, AM35XX_PIN_DSS_D18_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D19_ADDR, AM35XX_PIN_DSS_D19_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D20_ADDR, AM35XX_PIN_DSS_D20_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D21_ADDR, AM35XX_PIN_DSS_D21_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D22_ADDR, AM35XX_PIN_DSS_D22_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_D23_ADDR, AM35XX_PIN_DSS_D23_VALUE);

    GFX_WRITE_REG16(AM35XX_PIN_DSS_VSYNC_ADDR, AM35XX_PIN_DSS_VSYNC_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_AC_ADDR, AM35XX_PIN_DSS_AC_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_HSYNC_ADDR, AM35XX_PIN_DSS_HSYNC_VALUE);
    GFX_WRITE_REG16(AM35XX_PIN_DSS_PCLK_ADDR, AM35XX_PIN_DSS_PCLK_VALUE);

    /* gpio_181 mcspi2_cs0:low  16bit */

    GFX_WRITE_REG16(AM35XX_PIN_DSS_PWM_ADDR, AM35XX_PIN_DSS_PWM_VALUE);

    /* gpio_182 mcspi2_cs0:high 16bit */

    GFX_WRITE_REG16(AM35XX_PIN_DSS_BL_PWR_ADDR, AM35XX_PIN_DSS_BL_PWR_VALUE);

    /* gpio_176 mcspi1_cs1;high 16bit */

    GFX_WRITE_REG16(AM35XX_PIN_DSS_PWR_ADDR, AM35XX_PIN_DSS_PWR_VALUE);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxEnable - enable TI display interface
 *
 * This rountine implements enable display interface and related pins. If use
 * OMAP flat panel, it will initialize the LCD.
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm35xxEnable
    (
    void
    )
    {
    gfxTiAm35xxPinMux ();
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdPowerOff - LCD power off
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm35xxLcdPowerOff (void)
    {
    UINT32  regData;

    regData = (AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_PWR_OFFSET);

    GFX_CLR_BIT32(AM35XX_GPIO6_OE_ADDR, regData);
    GFX_CLR_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdPowerOn -  LCD power on
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm35xxLcdPowerOn (void)
    {
    UINT32  regData;

    /* config lcd gpio dir */

    regData = (AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_PWR_OFFSET);
    GFX_CLR_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);
    GFX_SET_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);

    regData = (AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_PWR_OFFSET);
    GFX_CLR_BIT32(AM35XX_GPIO6_OE_ADDR, regData);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdBLPowerOff - LCD backlight power off
 *
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm35xxLcdBLPowerOff (void)
    {
    UINT32  regData;

    regData = ((AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_BL_PWR_OFFSET)|
               (AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_BL_PWR_OFFSET));

    GFX_CLR_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);
    GFX_CLR_BIT32(AM35XX_GPIO6_OE_ADDR, regData);
    }

/*******************************************************************************
 *
 * gfxTiAm35xxLcdBLPowerOn -  LCD backlight power on
 *
 * RETURNS: N/A
 *
 */
void gfxTiAm35xxLcdBLPowerOn (void)
    {
    UINT32  regData;

    /* config lcd back light gpio  */

    regData = ((AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_PWM_OFFSET) |
               (AM35XX_BIT_ENABLE << AM35XX_GPIO6_LCD_BL_PWR_OFFSET));
    GFX_CLR_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);
    GFX_SET_BIT32(AM35XX_GPIO6_DATAOUT_ADDR, regData);

    GFX_CLR_BIT32(AM35XX_GPIO6_OE_ADDR, regData);
    }
