/* vxbAm3Prcm.h - TI Power and Clock management header */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17apr14,sye  moved to platform header directory. (US36393)
29sep13,c_t  written.
*/

#ifndef __INCvxbAm3PrcmH
#define __INCvxbAm3PrcmH

#include <vxWorks.h>
#include <hwif/vxBus.h>
#include <dllLib.h>
#include <subsys/clk/vxbClkLib.h>

#ifdef __cplusplus
        extern "C" {
#endif

#define AM335X_CM_PER                       (0x000)
#define AM335X_CM_WKUP                      (0x400)
#define AM335X_CM_DPLL                      (0x500)
#define AM335X_CM_MPU                       (0x600)
#define AM335X_CM_DEVICE                    (0x700)
#define AM335X_CM_RTC                       (0x800)
#define AM335X_CM_GFX                       (0x900)
#define AM335X_CM_CEFUSE                    (0xA00)

#define AM335X_PRM_IRQ                      (0xB00)
#define AM335X_PRM_PER                      (0xC00)
#define AM335X_PRM_WKUP                     (0xD00)
#define AM335X_PRM_MPU                      (0xE00)
#define AM335X_PRM_DEVICE                   (0xF00)
#define AM335X_PRM_RTC                      (0x1000)
#define AM335X_PRM_GFX                      (0x1100)
#define AM335X_PRM_CEFUSE                   (0x1200)

#define AM335X_PRM_IRQSTATUS_MPU            (AM335X_PRM_IRQ + 0x0004)
#define AM335X_PRM_IRQENABLE_MPU            (AM335X_PRM_IRQ + 0x0008)
#define AM335X_PRM_IRQSTATUS_M3             (AM335X_PRM_IRQ + 0x000c)
#define AM335X_PRM_IRQENABLE_M3             (AM335X_PRM_IRQ + 0x0010)

#define AM335X_PM_PER_RSTCTRL               (AM335X_PRM_PER + 0x0000)
#define AM335X_PM_PER_RSTST                 (AM335X_PRM_PER + 0x0004)
#define AM335X_PM_PER_PWRSTST               (AM335X_PRM_PER + 0x0008)
#define AM335X_PM_PER_PWRSTCTRL             (AM335X_PRM_PER + 0x000c)

#define AM335X_PM_WKUP_RSTCTRL              (AM335X_PRM_WKUP + 0x0000)
#define AM335X_PM_WKUP_PWRSTCTRL            (AM335X_PRM_WKUP + 0x0004)
#define AM335X_PM_WKUP_PWRSTST              (AM335X_PRM_WKUP + 0x0008)
#define AM335X_PM_WKUP_RSTST                (AM335X_PRM_WKUP + 0x000c)

#define AM335X_PM_MPU_PWRSTCTRL             (AM335X_PRM_MPU + 0x0000)
#define AM335X_PM_MPU_PWRSTST               (AM335X_PRM_MPU + 0x0004)
#define AM335X_PM_MPU_RSTST                 (AM335X_PRM_MPU + 0x0008)

#define AM335X_PM_RSTCTRL                   (AM335X_PRM_DEVICE + 0x0000)
#define AM335X_PM_RSTTIME                   (AM335X_PRM_DEVICE + 0x0004)
#define AM335X_PM_RSTST                     (AM335X_PRM_DEVICE + 0x0008)
#define AM335X_PM_SRAM_COUNT                (AM335X_PRM_DEVICE + 0x000c)
#define AM335X_PM_LDO_SRAM_CORE_SETUP       (AM335X_PRM_DEVICE + 0x0010)
#define AM335X_PM_LDO_SRAM_CORE_CTRL        (AM335X_PRM_DEVICE + 0x0014)
#define AM335X_PM_LDO_SRAM_MPU_SETUP        (AM335X_PRM_DEVICE + 0x0018)
#define AM335X_PM_LDO_SRAM_MPU_CTRL         (AM335X_PRM_DEVICE + 0x001c)

#define AM335X_PM_RTC_PWRSTCTRL             (AM335X_PRM_RTC + 0x0000)
#define AM335X_PM_RTC_PWRSTST               (AM335X_PRM_RTC + 0x0004)

#define AM335X_PM_GFX_PWRSTCTRL             (AM335X_PRM_GFX + 0x0000)
#define AM335X_PM_GFX_RSTCTRL               (AM335X_PRM_GFX + 0x0004)
#define AM335X_PM_GFX_PWRSTST               (AM335X_PRM_GFX + 0x0010)
#define AM335X_PM_GFX_RSTST                 (AM335X_PRM_GFX + 0x0014)

#define AM335X_PM_CEFUSE_PWRSTCTRL          (AM335X_PRM_CEFUSE + 0x0000)
#define AM335X_PM_CEFUSE_PWRSTST            (AM335X_PRM_CEFUSE + 0x0004)

#define AM335X_CM_PER_L4LS_CLKSTCTRL        (AM335X_CM_PER +  0x0000)
#define AM335X_CM_PER_L3S_CLKSTCTRL         (AM335X_CM_PER +  0x0004)
#define AM335X_CM_PER_L4FW_CLKSTCTRL        (AM335X_CM_PER +  0x0008)
#define AM335X_CM_PER_L3_CLKSTCTRL          (AM335X_CM_PER +  0x000c)
#define AM335X_CM_PER_CPGMAC0_CLKCTRL       (AM335X_CM_PER +  0x0014)
#define AM335X_CM_PER_LCDC_CLKCTRL          (AM335X_CM_PER +  0x0018)
#define AM335X_CM_PER_USB0_CLKCTRL          (AM335X_CM_PER +  0x001c)
#define AM335X_CM_PER_MLB_CLKCTRL           (AM335X_CM_PER +  0x0020)
#define AM335X_CM_PER_TPTC0_CLKCTRL         (AM335X_CM_PER +  0x0024)
#define AM335X_CM_PER_EMIF_CLKCTRL          (AM335X_CM_PER +  0x0028)
#define AM335X_CM_PER_OCMCRAM_CLKCTRL       (AM335X_CM_PER +  0x002c)
#define AM335X_CM_PER_GPMC_CLKCTRL          (AM335X_CM_PER +  0x0030)
#define AM335X_CM_PER_MCASP0_CLKCTRL        (AM335X_CM_PER +  0x0034)
#define AM335X_CM_PER_UART5_CLKCTRL         (AM335X_CM_PER +  0x0038)
#define AM335X_CM_PER_MMC0_CLKCTRL          (AM335X_CM_PER +  0x003c)
#define AM335X_CM_PER_ELM_CLKCTRL           (AM335X_CM_PER +  0x0040)
#define AM335X_CM_PER_I2C2_CLKCTRL          (AM335X_CM_PER +  0x0044)
#define AM335X_CM_PER_I2C1_CLKCTRL          (AM335X_CM_PER +  0x0048)
#define AM335X_CM_PER_SPI0_CLKCTRL          (AM335X_CM_PER +  0x004c)
#define AM335X_CM_PER_SPI1_CLKCTRL          (AM335X_CM_PER +  0x0050)
#define AM335X_CM_PER_SPI2_CLKCTRL          (AM335X_CM_PER +  0x0054)
#define AM335X_CM_PER_SPI3_CLKCTRL          (AM335X_CM_PER +  0x0058)
#define AM335X_CM_PER_L4LS_CLKCTRL          (AM335X_CM_PER +  0x0060)
#define AM335X_CM_PER_L4FW_CLKCTRL          (AM335X_CM_PER +  0x0064)
#define AM335X_CM_PER_MCASP1_CLKCTRL        (AM335X_CM_PER +  0x0068)
#define AM335X_CM_PER_UART1_CLKCTRL         (AM335X_CM_PER +  0x006c)
#define AM335X_CM_PER_UART2_CLKCTRL         (AM335X_CM_PER +  0x0070)
#define AM335X_CM_PER_UART3_CLKCTRL         (AM335X_CM_PER +  0x0074)
#define AM335X_CM_PER_UART4_CLKCTRL         (AM335X_CM_PER +  0x0078)
#define AM335X_CM_PER_TIMER7_CLKCTRL        (AM335X_CM_PER +  0x007c)
#define AM335X_CM_PER_TIMER2_CLKCTRL        (AM335X_CM_PER +  0x0080)
#define AM335X_CM_PER_TIMER3_CLKCTRL        (AM335X_CM_PER +  0x0084)
#define AM335X_CM_PER_TIMER4_CLKCTRL        (AM335X_CM_PER +  0x0088)
#define AM335X_CM_PER_MCASP2_CLKCTRL        (AM335X_CM_PER +  0x008c)
#define AM335X_CM_PER_RNG_CLKCTRL           (AM335X_CM_PER +  0x0090)
#define AM335X_CM_PER_AES0_CLKCTRL          (AM335X_CM_PER +  0x0094)
#define AM335X_CM_PER_AES1_CLKCTRL          (AM335X_CM_PER +  0x0098)
#define AM335X_CM_PER_DES_CLKCTRL           (AM335X_CM_PER +  0x009c)
#define AM335X_CM_PER_SHA0_CLKCTRL          (AM335X_CM_PER +  0x00a0)
#define AM335X_CM_PER_PKA_CLKCTRL           (AM335X_CM_PER +  0x00a4)
#define AM335X_CM_PER_GPIO6_CLKCTRL         (AM335X_CM_PER +  0x00a8)
#define AM335X_CM_PER_GPIO1_CLKCTRL         (AM335X_CM_PER +  0x00ac)
#define AM335X_CM_PER_GPIO2_CLKCTRL         (AM335X_CM_PER +  0x00b0)
#define AM335X_CM_PER_GPIO3_CLKCTRL         (AM335X_CM_PER +  0x00b4)
#define AM335X_CM_PER_GPIO4_CLKCTRL         (AM335X_CM_PER +  0x00b8)
#define AM335X_CM_PER_TPCC_CLKCTRL          (AM335X_CM_PER +  0x00bc)
#define AM335X_CM_PER_DCAN0_CLKCTRL         (AM335X_CM_PER +  0x00c0)
#define AM335X_CM_PER_DCAN1_CLKCTRL         (AM335X_CM_PER +  0x00c4)
#define AM335X_CM_PER_EPWMSS1_CLKCTRL       (AM335X_CM_PER +  0x00cc)
#define AM335X_CM_PER_EMIF_FW_CLKCTRL       (AM335X_CM_PER +  0x00d0)
#define AM335X_CM_PER_EPWMSS0_CLKCTRL       (AM335X_CM_PER +  0x00d4)
#define AM335X_CM_PER_EPWMSS2_CLKCTRL       (AM335X_CM_PER +  0x00d8)
#define AM335X_CM_PER_L3_INSTR_CLKCTRL      (AM335X_CM_PER +  0x00dc)
#define AM335X_CM_PER_L3_CLKCTRL            (AM335X_CM_PER +  0x00e0)
#define AM335X_CM_PER_IEEE5000_CLKCTRL      (AM335X_CM_PER +  0x00e4)
#define AM335X_CM_PER_PRUSS_CLKCTRL         (AM335X_CM_PER +  0x00e8)
#define AM335X_CM_PER_TIMER5_CLKCTRL        (AM335X_CM_PER +  0x00ec)
#define AM335X_CM_PER_TIMER6_CLKCTRL        (AM335X_CM_PER +  0x00f0)
#define AM335X_CM_PER_MMC1_CLKCTRL          (AM335X_CM_PER +  0x00f4)
#define AM335X_CM_PER_MMC2_CLKCTRL          (AM335X_CM_PER +  0x00f8)
#define AM335X_CM_PER_TPTC1_CLKCTRL         (AM335X_CM_PER +  0x00fc)
#define AM335X_CM_PER_TPTC2_CLKCTRL         (AM335X_CM_PER +  0x0100)
#define AM335X_CM_PER_GPIO5_CLKCTRL         (AM335X_CM_PER +  0x0104)
#define AM335X_CM_PER_SPINLOCK_CLKCTRL      (AM335X_CM_PER +  0x010c)
#define AM335X_CM_PER_MAILBOX0_CLKCTRL      (AM335X_CM_PER +  0x0110)
#define AM335X_CM_PER_L4HS_CLKSTCTRL        (AM335X_CM_PER +  0x011c)
#define AM335X_CM_PER_L4HS_CLKCTRL          (AM335X_CM_PER +  0x0120)
#define AM335X_CM_PER_MSTR_EXPS_CLKCTRL     (AM335X_CM_PER +  0x0124)
#define AM335X_CM_PER_SLV_EXPS_CLKCTRL      (AM335X_CM_PER +  0x0128)
#define AM335X_CM_PER_OCPWP_L3_CLKSTCTRL    (AM335X_CM_PER +  0x012c)
#define AM335X_CM_PER_OCPWP_CLKCTRL         (AM335X_CM_PER +  0x0130)
#define AM335X_CM_PER_MAILBOX1_CLKCTRL      (AM335X_CM_PER +  0x0134)
#define AM335X_CM_PER_PRUSS_CLKSTCTRL       (AM335X_CM_PER +  0x0140)
#define AM335X_CM_PER_CPSW_CLKSTCTRL        (AM335X_CM_PER +  0x0144)
#define AM335X_CM_PER_LCDC_CLKSTCTRL        (AM335X_CM_PER +  0x0148)
#define AM335X_CM_PER_CLKDIV32K_CLKCTRL     (AM335X_CM_PER +  0x014c)
#define AM335X_CM_PER_CLK_24MHZ_CLKSTCTRL   (AM335X_CM_PER +  0x0150)

#define AM335X_CM_WKUP_CLKSTCTRL            (AM335X_CM_WKUP +  0x0000)
#define AM335X_CM_WKUP_CONTROL_CLKCTRL      (AM335X_CM_WKUP +  0x0004)
#define AM335X_CM_WKUP_GPIO0_CLKCTRL        (AM335X_CM_WKUP +  0x0008)
#define AM335X_CM_WKUP_L4WKUP_CLKCTRL       (AM335X_CM_WKUP +  0x000c)
#define AM335X_CM_WKUP_TIMER0_CLKCTRL       (AM335X_CM_WKUP +  0x0010)
#define AM335X_CM_WKUP_DEBUGSS_CLKCTRL      (AM335X_CM_WKUP +  0x0014)
#define AM335X_CM_L3_AON_CLKSTCTRL          (AM335X_CM_WKUP +  0x0018)
#define AM335X_CM_AUTOIDLE_DPLL_MPU         (AM335X_CM_WKUP +  0x001c)
#define AM335X_CM_IDLEST_DPLL_MPU           (AM335X_CM_WKUP +  0x0020)
#define AM335X_CM_SSC_DELTAMSTEP_DPLL_MPU   (AM335X_CM_WKUP +  0x0024)
#define AM335X_CM_SSC_MODFREQDIV_DPLL_MPU   (AM335X_CM_WKUP +  0x0028)
#define AM335X_CM_CLKSEL_DPLL_MPU           (AM335X_CM_WKUP +  0x002c)
#define AM335X_CM_AUTOIDLE_DPLL_DDR         (AM335X_CM_WKUP +  0x0030)
#define AM335X_CM_IDLEST_DPLL_DDR           (AM335X_CM_WKUP +  0x0034)
#define AM335X_CM_SSC_DELTAMSTEP_DPLL_DDR   (AM335X_CM_WKUP +  0x0038)
#define AM335X_CM_SSC_MODFREQDIV_DPLL_DDR   (AM335X_CM_WKUP +  0x003c)
#define AM335X_CM_CLKSEL_DPLL_DDR           (AM335X_CM_WKUP +  0x0040)
#define AM335X_CM_AUTOIDLE_DPLL_DISP        (AM335X_CM_WKUP +  0x0044)
#define AM335X_CM_IDLEST_DPLL_DISP          (AM335X_CM_WKUP +  0x0048)
#define AM335X_CM_SSC_DELTAMSTEP_DPLL_DISP  (AM335X_CM_WKUP +  0x004c)
#define AM335X_CM_SSC_MODFREQDIV_DPLL_DISP  (AM335X_CM_WKUP +  0x0050)
#define AM335X_CM_CLKSEL_DPLL_DISP          (AM335X_CM_WKUP +  0x0054)
#define AM335X_CM_AUTOIDLE_DPLL_CORE        (AM335X_CM_WKUP +  0x0058)
#define AM335X_CM_IDLEST_DPLL_CORE          (AM335X_CM_WKUP +  0x005c)
#define AM335X_CM_SSC_DELTAMSTEP_DPLL_CORE  (AM335X_CM_WKUP +  0x0060)
#define AM335X_CM_SSC_MODFREQDIV_DPLL_CORE  (AM335X_CM_WKUP +  0x0064)
#define AM335X_CM_CLKSEL_DPLL_CORE          (AM335X_CM_WKUP +  0x0068)
#define AM335X_CM_AUTOIDLE_DPLL_PER         (AM335X_CM_WKUP +  0x006c)
#define AM335X_CM_IDLEST_DPLL_PER           (AM335X_CM_WKUP +  0x0070)
#define AM335X_CM_SSC_DELTAMSTEP_DPLL_PER   (AM335X_CM_WKUP +  0x0074)
#define AM335X_CM_SSC_MODFREQDIV_DPLL_PER   (AM335X_CM_WKUP +  0x0078)
#define AM335X_CM_CLKDCOLDO_DPLL_PER        (AM335X_CM_WKUP +  0x007c)
#define AM335X_CM_DIV_M4_DPLL_CORE          (AM335X_CM_WKUP +  0x0080)
#define AM335X_CM_DIV_M5_DPLL_CORE          (AM335X_CM_WKUP +  0x0084)
#define AM335X_CM_CLKMODE_DPLL_MPU          (AM335X_CM_WKUP +  0x0088)
#define AM335X_CM_CLKMODE_DPLL_PER          (AM335X_CM_WKUP +  0x008c)
#define AM335X_CM_CLKMODE_DPLL_CORE         (AM335X_CM_WKUP +  0x0090)
#define AM335X_CM_CLKMODE_DPLL_DDR          (AM335X_CM_WKUP +  0x0094)
#define AM335X_CM_CLKMODE_DPLL_DISP         (AM335X_CM_WKUP +  0x0098)
#define AM335X_CM_CLKSEL_DPLL_PERIPH        (AM335X_CM_WKUP +  0x009c)
#define AM335X_CM_DIV_M2_DPLL_DDR           (AM335X_CM_WKUP +  0x00a0)
#define AM335X_CM_DIV_M2_DPLL_DISP          (AM335X_CM_WKUP +  0x00a4)
#define AM335X_CM_DIV_M2_DPLL_MPU           (AM335X_CM_WKUP +  0x00a8)
#define AM335X_CM_DIV_M2_DPLL_PER           (AM335X_CM_WKUP +  0x00ac)
#define AM335X_CM_WKUP_WKUP_M3_CLKCTRL      (AM335X_CM_WKUP +  0x00b0)
#define AM335X_CM_WKUP_UART0_CLKCTRL        (AM335X_CM_WKUP +  0x00b4)
#define AM335X_CM_WKUP_I2C0_CLKCTRL         (AM335X_CM_WKUP +  0x00b8)
#define AM335X_CM_WKUP_ADC_TSC_CLKCTRL      (AM335X_CM_WKUP +  0x00bc)
#define AM335X_CM_WKUP_SMARTREFLEX0_CLKCTRL (AM335X_CM_WKUP +  0x00c0)
#define AM335X_CM_WKUP_TIMER1_CLKCTRL       (AM335X_CM_WKUP +  0x00c4)
#define AM335X_CM_WKUP_SMARTREFLEX1_CLKCTRL (AM335X_CM_WKUP +  0x00c8)
#define AM335X_CM_L4_WKUP_AON_CLKSTCTRL     (AM335X_CM_WKUP +  0x00cc)
#define AM335X_CM_WKUP_WDT0_CLKCTRL         (AM335X_CM_WKUP +  0x00d0)
#define AM335X_CM_WKUP_WDT1_CLKCTRL         (AM335X_CM_WKUP +  0x00d4)
#define AM335X_CM_DIV_M6_DPLL_CORE          (AM335X_CM_WKUP +  0x00d8)

#define AM335X_CLKSEL_TIMER7_CLK            (AM335X_CM_DPLL +  0x0004)
#define AM335X_CLKSEL_TIMER2_CLK            (AM335X_CM_DPLL +  0x0008)
#define AM335X_CLKSEL_TIMER3_CLK            (AM335X_CM_DPLL +  0x000c)
#define AM335X_CLKSEL_TIMER4_CLK            (AM335X_CM_DPLL +  0x0010)
#define AM335X_CM_MAC_CLKSEL                (AM335X_CM_DPLL +  0x0014)
#define AM335X_CLKSEL_TIMER5_CLK            (AM335X_CM_DPLL +  0x0018)
#define AM335X_CLKSEL_TIMER6_CLK            (AM335X_CM_DPLL +  0x001c)
#define AM335X_CM_CPTS_RFT_CLKSEL           (AM335X_CM_DPLL +  0x0020)
#define AM335X_CLKSEL_TIMER1MS_CLK          (AM335X_CM_DPLL +  0x0028)
#define AM335X_CLKSEL_GFX_FCLK              (AM335X_CM_DPLL +  0x002c)
#define AM335X_CLKSEL_PRUSS_OCP_CLK         (AM335X_CM_DPLL +  0x0030)
#define AM335X_CLKSEL_LCDC_PIXEL_CLK        (AM335X_CM_DPLL +  0x0034)
#define AM335X_CLKSEL_WDT1_CLK              (AM335X_CM_DPLL +  0x0038)
#define AM335X_CLKSEL_GPIO0_DBCLK           (AM335X_CM_DPLL +  0x003c)

#define AM335X_CM_MPU_CLKSTCTRL             (AM335X_CM_MPU + 0x0000)
#define AM335X_CM_MPU_MPU_CLKCTRL           (AM335X_CM_MPU + 0x0004)

#define AM335X_CM_CLKOUT_CTRL               (AM335X_CM_DEVICE + 0x0000)

#define AM335X_CM_RTC_RTC_CLKCTRL           (AM335X_CM_RTC + 0x0000)
#define AM335X_CM_RTC_CLKSTCTRL             (AM335X_CM_RTC + 0x0004)

#define AM335X_CM_GFX_L3_CLKSTCTRL          (AM335X_CM_GFX + 0x0000)
#define AM335X_CM_GFX_GFX_CLKCTRL           (AM335X_CM_GFX + 0x0004)
#define AM335X_CM_GFX_BITBLT_CLKCTRL        (AM335X_CM_GFX + 0x0008)
#define AM335X_CM_GFX_L4LS_GFX_CLKSTCTRL    (AM335X_CM_GFX + 0x000c)
#define AM335X_CM_GFX_MMUCFG_CLKCTRL        (AM335X_CM_GFX + 0x0010)
#define AM335X_CM_GFX_MMUDATA_CLKCTRL       (AM335X_CM_GFX + 0x0014)

#define AM335X_CM_CEFUSE_CLKSTCTRL          (AM335X_CM_CEFUSE + 0x0000)
#define AM335X_CM_CEFUSE_CEFUSE_CLKCTRL     (AM335X_CM_CEFUSE + 0x0020)

#define AM335X_CONTROL_PWMSS_CTRL_OFS           0x664
#define AM335X_PWMSS0_TBCLKEN                   0x0
#define AM335X_PWMSS1_TBCLKEN                   0x1
#define AM335X_PWMSS2_TBCLKEN                   0x2
#define AM335X_PWMSS0_TBCLKDIS                  0x0
#define AM335X_PWMSS1_TBCLKDIS                  0x0
#define AM335X_PWMSS2_TBCLKDIS                  0x0
#define AM335X_PWMSS0_TBCLKEN_MASK              0x1
#define AM335X_PWMSS1_TBCLKEN_MASK              0x2
#define AM335X_PWMSS2_TBCLKEN_MASK              0x4

#define AM335X_CONTROL_PWMSS_CTRL               (AM335X_CTRL_BASE + AM335X_CONTROL_PWMSS_CTRL_OFS)
#define AM335X_MODULEMODE_HWCTRL                0
#define AM335X_MODULEMODE_SWCTRL                1

#define AM335X_OPTFCLKEN_GPIO_GDBCLK_MASK       (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO0_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO1_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO2_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO3_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO4_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO5_GDBCLK_EN        (0x1 << 18)
#define AM335X_OPTFCLKEN_GPIO6_GDBCLK_EN        (0x1 << 18)
#define AM335X_DPLL_CLKDCOLDO_GATE_CTRL_MASK    (0x1 << 8)
#define AM335X_DPLL_CLKDCOLDO_GATE_CTRL_EN      (0x1 << 8)
#define AM335X_DPLL_CLKDCOLDO_GATE_CTRL_DIS     (0x0 << 8)
#define AM335X_CLKOUT2MASK                      (0x1 << 7)
#define AM335X_CLKOUT2EN                        (0x1 << 7)
#define AM335X_CLKOUT2DIS                       (0x0 << 7)
#define AM335X_MODULEMODE_MASK                  (0x3 << 0)
#define AM335X_MODULEMODE_EN                    (0x2)
#define AM335X_MODULEMODE_DIS                   (0x0)
#define AM335X_IDLEST_MASK                      (0x3 << 16)
#define AM335X_IDLEST_EN                        (0x0 << 16)
#define AM335X_IDLEST_DIS                       (0x3 << 16)
#define AM335X_HSDIVIDER_CLKOUT_DIV_MASK        (0x1f)
#define AM335X_DPLL_CLKOUT_DIV_MASK             (0x1f)

#define AM335X_CTRL_BASE                        (0x10000)
#define AM335X_CTRL_STATUS                      (AM335X_CTRL_BASE + 0x40)
#define AM335X_DPLL_DIV_MASK                    (0x7f)
#define AM335X_DPLL_DIV_SHIFT                   (0)
#define AM335X_DPLL_MULT_MASK                   (0x7ff << 8)
#define AM335X_DPLL_MULT_SHIFT                  (8)

#define AM335x_DPLL_M_MAX                       (2047)
#define AM335x_DPLL_M_MIN                       (2)
#define AM335x_DPLL_N_MAX                       (127)
#define AM335x_DPLL_N_MIN                       (0)

#define AM335x_DPLL_PER_M_MAX                   (4095)
#define AM335x_DPLL_PER_M_MIN                   (2)
#define AM335x_DPLL_PER_N_MAX                   (255)
#define AM335x_DPLL_PER_N_MIN                   (0)

#define AM335X_DPLL_MODE_MASK                   (0x7)
#define AM335X_DPLL_EN                          (0x7)
#define AM335X_DPLL_BYPASS                      (0x4)

#define AM335X_DPLL_ST_MN_BYPASS                (0x100)
#define AM335X_DPLL_ST_DPLL_CLK                 (0x1)

#define AM335X_CLKSEL_MASK                      (0x3)

#define AM335X_TXDEV_EOI                        (AM335X_CTRL_BASE + 0x1324)

typedef struct am3PrcmDrvCtrl
    {
    VXB_DEV_ID      pInst;
    void*           pRegbase;
    void*           pHandle;
    UINT32          numOfOmap2Clocks;
    spinlockIsr_t   vxbAm3PrcmOpsLock;
    } AM3_PRCM_DRVCTRL;

struct am3_clock_regs
    {
    UINT32 regBase;
    UINT32 mask;
    UINT32 enable;
    UINT32 disable;
    UINT32 idlestMask;
    UINT32 idlestEn;
    UINT32 idlestDis;
    UINT32 timeout;
    };
typedef struct am3_clock_regs AM3_CLOCK_REGS;

struct am3_clock_data
    {
    union
        {
        int   divider;
        void* selector;
        } u;
    UINT32 flags;
    void * regs;
    };
typedef struct am3_clock_data AM3_CLOCK_DATA;

struct am3_clock_sel
    {
    UINT32 regBase;
    UINT32 sel;
    UINT32 mask;
    UINT32 div;
    VXB_CLK* clock;
    };

typedef struct am3_clock_sel AM3_CLOCK_SEL;

struct am3_dpll_regs
    {
    UINT32 idlest;
    UINT32 clksel;
    UINT32 clkmode;

    UINT32 divmask;
    UINT32 divshift;
    UINT32 multmask;
    UINT32 multshift;

    UINT32 divmax;
    UINT32 divmin;
    UINT32 multmax;
    UINT32 multmin;

    UINT32 modemask;
    UINT32 enable;
    UINT32 bypass;
    UINT32 bypassst;
    UINT32 lockedst;
    };

typedef struct am3_dpll_regs AM3_DPLL_REGS;

#define AM3_CLK_NO_WAIT           0x40
#define AM3_CLK_DFLT_TIMEOUT      0xffff

#define AM3_CLKSEL_CLK            0x10
#define AM3_CLKSEL_FIXED_DIV      0x20
#define AM3_CLKSEL_DPLL           0x40
#define AM3_CLKSEL_DPLL_DIV       0x80
#define AM3_CLKSEL_IGNORE_PARENT  0x100
#define AM3_CLK_ROOT              0x200

#define AM3_CLK_UNKNOWN           CLOCK_STATUS_UNKNOWN
#define AM3_CLK_ENABLED           CLOCK_STATUS_ENABLED
#define AM3_CLK_GATED             CLOCK_STATUS_GATED

#define NO_SLEEP    (0) /* disable sleep state */
#define SW_SLEEP    (1) /* force sleep */
#define SW_WKUP     (2) /* force wake up */
#define HW_AUTO     (3) /* automatic state change */

#define DEFINE_CLOCK_REG(name, regBase, mask, enable, disable, idlestmask, idlesten, idlestdis, timeout) \
static struct am3_clock_regs name = \
    { \
    regBase, \
    mask, \
    enable, \
    disable, \
    idlestmask, \
    idlesten, \
    idlestdis, \
    timeout \
    };

#define DEFINE_DPLL_REG(name, idlest, clksel, clkmode,\
                            divmask, divshift, multmask, multshift,\
                            divmax, divmin, multimax, multmin,\
                            modemask, enable, bypass, bypassst, lockedst) \
        struct am3_dpll_regs name = \
            {\
            idlest,\
            clksel,\
            clkmode, \
            divmask, divshift, multmask, multshift, \
            divmax, divmin, multimax, multmin,\
            modemask, \
            enable, \
            bypass, \
            bypassst,\
            lockedst\
            };

#define DEFINE_CLOCK_CTX(name, val, flags, regs) \
            LOCAL struct am3_clock_data name =   \
                    {                            \
                    { (int)val },                \
                    flags,                       \
                    regs                         \
                    };

#define DEFINE_CLOCK_SEL_ONE(regBase, sel, mask, div, clock) \
                            {regBase, sel, mask, div, clock},

#define CLOCK_SEL_END() \
                        {0, 0, 0, 0, NULL}

#ifdef __cplusplus
}
#endif
                                
#endif  /* __INCvxbAm3PrcmH */

