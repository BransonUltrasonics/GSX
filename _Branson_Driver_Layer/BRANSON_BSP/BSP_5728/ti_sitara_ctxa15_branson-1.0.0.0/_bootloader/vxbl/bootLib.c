/* bootLib.c - hardware initialization and boot for ti_sitara_ctxa15 */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15mar16,jnl  created (US75898)
*/

/*
DESCRIPTION
This file contains the entry code of ti_sitara_ctxa15's first stage bootloader .
*/

#include <vxboot.h>
#include <stdlib.h>
#include <stdio.h>
#include <memdesc.h>

#include "ti_am572x.h"

static struct mem_desc tiam572xMemDesc[] =
    {
        {
        .addr = (uint64_t)_WRS_CONFIG_LOCAL_MEM_ADRS,
        .size = (uint64_t)_WRS_CONFIG_LOCAL_MEM_SIZE
        }
    };

#define sysInLong(a)                         (*(volatile unsigned int *)(a))
#define sysOutLong(a, v)                     (*(volatile unsigned int *)(a) = (v))
#define sysInWord(a)                         (*(volatile unsigned short *)(a))
#define sysOutWord(a, v)                     (*(volatile unsigned short *)(a) = (v))
#define sysInByte(a)                         (*(volatile unsigned char *)(a))
#define sysOutByte(a, v)                     (*(volatile unsigned char *)(a) = (v))

#define WSPR                                 0x4AE14048
#define WWPS                                 0x4AE14034
#define CTRL_CORE_PAD_UART2_CTSN             0x4A0037F8
#define CTRL_CORE_PAD_UART2_RTSN             0x4A0037FC
#define UART_MDR1                            0x48020020
#define UART_EFR                             0x48020008
#define UART_LCR                             0x4802000C
#define UART_DLL                             0x48020000
#define UART_IER                             0x48020004

#define MA_LISA_MAP_3                        0x482AF04C
#define DMM_LISA_MAP_3                       0x4E00004C
#define MAP_EMIF                             0x80740300

#define MAC_MII_SEL                          0x4A002554

#define EMIF1_CFG_BASE                       0x4C000000
#define EMIF2_CFG_BASE                       0x4D000000
#define EMIF_MOD_ID_REV(base)                ((base) + 0x0)
#define EMIF_SDRAM_STATUS(base)              ((base) + 0x04)
#define EMIF_SDRAM_CFG(base)                 ((base) + 0x08)
#define EMIF_SDRAM_CFG2(base)                ((base) + 0x0C)
#define EMIF_SDRAM_REF_CTRL(base)            ((base) + 0x10)
#define EMIF_SDRAM_REF_CTRL_SHADOW(base)     ((base) + 0x14)
#define EMIF_SDRAM_TIM_1(base)               ((base) + 0x18)
#define EMIF_SDRAM_TIM_1_SHADOW(base)        ((base) + 0x1C)
#define EMIF_SDRAM_TIM_2(base)               ((base) + 0x20)
#define EMIF_SDRAM_TIM_2_SHADOW(base)        ((base) + 0x24)
#define EMIF_SDRAM_TIM_3(base)               ((base) + 0x28)
#define EMIF_SDRAM_TIM_3_SHADOW(base)        ((base) + 0x2C)
#define EMIF_SDRAM_MGMT_CTRL(base)           ((base) + 0x38)
#define EMIF_SDRAM_MGMT_CTRL_SHD(base)       ((base) + 0x3C)
#define EMIF_OCP_CFG(base)                   ((base) + 0x54)
#define EMIF_IODFT_TLGC(base)                ((base) + 0x60)
#define EMIF_DLL_CALIB_CTRL(base)            ((base) + 0x98)
#define EMIF_DLL_CALIB_CTRL_SHADOW(base)     ((base) + 0x9C)
#define EMIF_ZQ_CFG(base)                    ((base) + 0xC8)
#define EMIF_TEMP_ALERT_CFG(base)            ((base) + 0xCC)
#define EMIF_RD_WR_LVL_RMP_CTL(base)         ((base) + 0xD8)
#define EMIF_RD_WR_LVL_CTL(base)             ((base) + 0xDC)
#define EMIF_DDR_PHY_CTRL_1(base)            ((base) + 0xE4)
#define EMIF_DDR_PHY_CTRL_1_SHADOW(base)     ((base) + 0xE8)
#define EMIF_DDR_PHY_CTRL_2(base)            ((base) + 0xEC)
#define EMIF_RD_WT_EXE_THRESHOLD(base)       ((base) + 0x120)
#define EMIF_PHY_STATUS(base, num)           ((base) + 0x144 + (4 * (num)))
#define EMIF_EXT_PHY_CTRL(base, num)         ((base) + 0x200 + (8 * (num)))
#define EMIF_EXT_PHY_CTRL_SHADOW(base, num)  ((base) + 0x204 + (8 * (num)))

#define CM_CORE_BASE                         0x4A005120
#define CM_PER_BASE                          0x4A008140
#define CM_MPU_BASE                          0x4A005160
#define CM_DDR_BASE                          0x4A005210
#define CM_L4PER_CLKSTCTRL                   0x4A009700
#define CM_L4PER_UART3_CLKCTRL               0x4A009850
#define CM_L4PER_GPIO6_CLKCTRL               0x4A009780
#define CM_CLKMODE_DPLL_CORE                 0x4A005110
#define CM_CLKMODE_DPLL(base)                (base)
#define CM_IDLEST_DPLL(base)                 ((base) + 0x4)
#define CM_CLKSEL_DPLL(base)                 ((base) + 0xc)
#define CM_DIV_M2_DPLL(base)                 ((base) + 0x10)
#define CM_DIV_H11_DPLL(base)                ((base) + 0x18)
#define CM_DIV_H12_DPLL(base)                ((base) + 0x1C)
#define CM_DIV_H13_DPLL(base)                ((base) + 0x20)
#define CM_DIV_H14_DPLL(base)                ((base) + 0x24)
#define CM_DIV_H22_DPLL(base)                ((base) + 0x34)
#define CM_DIV_H23_DPLL(base)                ((base) + 0x38)
#define CM_DIV_H24_DPLL(base)                ((base) + 0x3C)

#define HW_AUTO                              0x3
#define SW_WKUP                              0x2
#define UART_CLK_EN                          0x2
#define UART_CLK_DIS                         0x30000
#define UART_RX_EN                           0x40002
#define UART_TX_EN                           0x40001
#define UART_EN                              0x7
#define UART_DIS                             0x0
#define UART_SW_FLOW_CONTROL                 0x6
#define UART_CHAR_LENGTH                     0x3
#define UART_DIV_EN                          0x80
#define UART_CLOCK_LSB_VAL                   0x1A
#define UART_DIS_INT                         0x0
#define GPIO6_EN                             0x101

#define EMIF_SDCFG                           0x61851b32
#define EMIF_SDCFG2                          0x8000000
#define EMIF_SDREF                           0x000040F1
#define EMIF_SDREF_FINAL                     0x00001035
#define EMIF_TIME1                           0xcccf36ab
#define EMIF_TIME2                           0x308f7fda
#define EMIF_TIME3                           0x409f88a8
#define EMIF_READ_IDLE_CTRL                  0x00090000
#define EMIF_ZQ_CFG_VAL                      0x5007190b
#define EMIF_ALERT_CFG                       0x00000000
#define EMIF_DDR_PHY_CTLR1_INIT              0x0024400b
#define EMIF_DDR_PHY_CTLR1                   0x0e24400b
#define EMIF_DDR_EXT_PHY_CTRL_1              0x10040100
#define EMIF_DDR_EXT_PHY_CTRL_2              0x00910091
#define EMIF_DDR_EXT_PHY_CTRL_3              0x00950095
#define EMIF_DDR_EXT_PHY_CTRL_4              0x009b009b
#define EMIF_DDR_EXT_PHY_CTRL_5              0x009e009e
#define EMIF_RD_WR_LVL_RMP_CTL_VAL           0x80000000
#define EMIF_RD_WR_LVL_CTL_VAL               0x00000000
#define EMIF_RD_WR_EXE_THRESH                0x00000305
#define EMIF_INITREF_DIS                     (1 << 31)
#define EMIF_OCP_CFG_VAL                     0x0A500000
#define EMIF_MGMT_CTRL_VAL                   0x0000f2f0
#define EMIF_MGMT_CTRL_SHD_VAL               0x0000f0f0 
#define EMIF_DIS_LVL                         0x00000000
#define EMIF_PHY_FIFO_WE_IN_MISALINED_CLR    (1 << 8)
#define EMIF_REG_INITREF_DIS_MASK            (1 << 31)
#define EMIF_FULL_LVL                        (1 << 31)
#define EMIF_TIMEOUT                         (7 << 4)
#define EMIF_LVL_DQ_RATIO_FST                16
#define EMIF_LVL_DQ_RATIO_LEN                10
#define EMIF_LVL_FIFOWEIN_RATIO_FST          11
#define EMIF_LVL_FIFOWEIN_RATIO_LEN          5

#define CM_DPLL_CLK_MASK                     1
#define RGMII_MODE_ENABLE                    0x3322

/*******************************************************************************
*
* sysDelay - busy loop sysDelay
*
* This routine does a busy loop sysDelay
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysDelay
    (
    int loops
    )
    {
    volatile int l = loops;

    while (l--);
    }

/******************************************************************************
*
* ti572xClockInit - initiaize necessary peripheral clocks
*
* This routine initiaizes necessary peripheral clocks
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xClockInit (void)
    {

    /* Disable wdg timer */

    sysOutLong(WSPR, 0xaaaa);
    while(sysInLong(WWPS) & 0x10);
    sysOutLong(WSPR, 0x5555);
    while(sysInLong(WWPS) & 0x10);

    /* Setup uart clock */    

    sysOutLong(CM_L4PER_CLKSTCTRL, SW_WKUP);
    
    sysOutLong(CM_L4PER_UART3_CLKCTRL, UART_CLK_EN);
    while (sysInLong(CM_L4PER_UART3_CLKCTRL) & UART_CLK_DIS);

    sysOutLong(CM_L4PER_CLKSTCTRL, HW_AUTO);

    /* Setup GPIO mode */

    sysOutLong(CM_L4PER_GPIO6_CLKCTRL, GPIO6_EN);

    /* set emac to rgmii mode */

    sysOutLong(MAC_MII_SEL, RGMII_MODE_ENABLE);
    }

/******************************************************************************
*
* ti572xMuxInit - setups mux pins
*
* This routine setups mux pins
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xMuxInit (void)
    {
    sysOutLong(CTRL_CORE_PAD_UART2_CTSN, UART_RX_EN);
    sysOutLong(CTRL_CORE_PAD_UART2_RTSN, UART_TX_EN);
    }

/*******************************************************************************
*
* ti572xUartInit - initialize uart 3
*
* This routine initialize uart 3 in TI AM572X. Currently, this routine only
* set uart3 into uart mode and nothing else. If you want to enable early
* serial output support, you can add more initialization code here.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

static void ti572xUartInit (void)
    {
    sysOutLong(UART_MDR1, UART_EN);
    sysOutLong(UART_EFR, UART_SW_FLOW_CONTROL);
    sysOutLong(UART_LCR, UART_CHAR_LENGTH | UART_DIV_EN);
    sysOutLong(UART_DLL, UART_CLOCK_LSB_VAL);
    sysOutLong(UART_IER, UART_DIS_INT);
    sysOutLong(UART_LCR, UART_CHAR_LENGTH);
    sysOutLong(UART_MDR1, UART_DIS);
    }

/******************************************************************************
*
* ti572xDdr3Init - initialize Ddr3
*
* This routine initializes Ddr3 for TI AM572x
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xDdr3Init
    (
    unsigned int base
    )
    {
    unsigned int val;
    unsigned int i;
    unsigned int extPhyIndex;

    /* Configure ext phy ctrl regs */

    static const unsigned int emifExtPhyVal[] = 
        {
        0x10040100, 0x00910091, 0x00950095, 0x009B009B, 0x009E009E, 0x00980098,
        0x00340034, 0x00350035, 0x00340034, 0x00310031, 0x00340034, 0x007F007F,
        0x007F007F, 0x007F007F, 0x007F007F, 0x007F007F, 0x00480048, 0x004A004A,
        0x00520052, 0x00550055, 0x00500050, 0x00000000, 0x00600020, 0x40011080,
        0x08102040, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 
        };

    sysOutLong(EMIF_EXT_PHY_CTRL(base, 0), emifExtPhyVal[0]);
    sysOutLong(EMIF_EXT_PHY_CTRL_SHADOW(base, 0), emifExtPhyVal[0]);

    for (i = 21; i < 35; i++) 
        {
        sysOutLong(EMIF_EXT_PHY_CTRL(base, i), emifExtPhyVal[i]);
        sysOutLong(EMIF_EXT_PHY_CTRL_SHADOW(base, i), emifExtPhyVal[i]);
        }

    sysOutLong(EMIF_SDRAM_REF_CTRL(base), EMIF_SDREF | EMIF_INITREF_DIS);
    sysOutLong(EMIF_SDRAM_TIM_1(base), EMIF_TIME1);
    sysOutLong(EMIF_SDRAM_TIM_2(base), EMIF_TIME2);
    sysOutLong(EMIF_SDRAM_TIM_3(base), EMIF_TIME3);
    sysOutLong(EMIF_OCP_CFG(base), EMIF_OCP_CFG_VAL);
    sysOutLong(EMIF_DLL_CALIB_CTRL(base), EMIF_READ_IDLE_CTRL);
    sysOutLong(EMIF_ZQ_CFG(base), EMIF_ZQ_CFG_VAL);
    sysOutLong(EMIF_TEMP_ALERT_CFG(base), EMIF_ALERT_CFG);
    sysOutLong(EMIF_RD_WR_LVL_RMP_CTL(base), EMIF_RD_WR_LVL_RMP_CTL_VAL);
    sysOutLong(EMIF_RD_WR_LVL_CTL(base), EMIF_RD_WR_LVL_CTL_VAL);
    sysOutLong(EMIF_DDR_PHY_CTRL_1(base), EMIF_DDR_PHY_CTLR1_INIT);
    sysOutLong(EMIF_RD_WT_EXE_THRESHOLD(base), EMIF_RD_WR_EXE_THRESH);
    sysOutLong(EMIF_SDRAM_REF_CTRL(base), EMIF_SDREF);
    sysOutLong(EMIF_SDRAM_CFG2(base), EMIF_SDCFG2);
    sysOutLong(EMIF_SDRAM_CFG(base), EMIF_SDCFG);

    sysOutLong(EMIF_SDRAM_REF_CTRL(base), EMIF_SDREF_FINAL);

    /* Clears the phy_reg_fifo_we_in_misaligned_sticky status */

    sysOutLong(EMIF_EXT_PHY_CTRL(base, 35), 
               sysInLong(EMIF_EXT_PHY_CTRL(base, 35)) | 
               EMIF_PHY_FIFO_WE_IN_MISALINED_CLR);

    sysOutLong(EMIF_EXT_PHY_CTRL_SHADOW(base, 35), 
               sysInLong(EMIF_EXT_PHY_CTRL_SHADOW(base, 35)) | 
               EMIF_PHY_FIFO_WE_IN_MISALINED_CLR);

    /* Disable SDRAM initialization and refreshes */
    
    sysOutLong(EMIF_SDRAM_REF_CTRL(base),
               sysInLong(EMIF_SDRAM_REF_CTRL(base)) | 
               EMIF_REG_INITREF_DIS_MASK);

    /* Trigger full leveling  */

    sysOutLong(EMIF_RD_WR_LVL_CTL(base), EMIF_FULL_LVL);

    if (sysInLong(EMIF_SDRAM_STATUS(base)) & EMIF_TIMEOUT)
        {
        return;
        }

    sysOutLong(EMIF_SDRAM_REF_CTRL(base),
               sysInLong(EMIF_SDRAM_REF_CTRL(base)) & 
               (~EMIF_REG_INITREF_DIS_MASK));

    extPhyIndex = 1;

    for (i = EMIF_LVL_FIFOWEIN_RATIO_FST; i < (EMIF_LVL_FIFOWEIN_RATIO_FST +
         EMIF_LVL_FIFOWEIN_RATIO_LEN); i++)
        {
        val = sysInLong(EMIF_PHY_STATUS(base, i));
        sysOutLong(EMIF_EXT_PHY_CTRL(base, extPhyIndex), val);
        sysOutLong(EMIF_EXT_PHY_CTRL_SHADOW(base, extPhyIndex), val);
        extPhyIndex++;
        }

    extPhyIndex = 11;
    for (i = EMIF_LVL_DQ_RATIO_FST; i < (EMIF_LVL_DQ_RATIO_FST +
         EMIF_LVL_DQ_RATIO_LEN); i++)
        {
        val = sysInLong(EMIF_PHY_STATUS(base, i));
        sysOutLong(EMIF_EXT_PHY_CTRL(base, extPhyIndex), val);
        sysOutLong(EMIF_EXT_PHY_CTRL_SHADOW(base, extPhyIndex), val);
        extPhyIndex++;
        }

    sysOutLong(EMIF_DDR_PHY_CTRL_1(base), EMIF_DDR_PHY_CTLR1);
    sysOutLong(EMIF_DDR_PHY_CTRL_1_SHADOW(base), EMIF_DDR_PHY_CTLR1);
    sysOutLong(EMIF_RD_WR_LVL_RMP_CTL(base), EMIF_DIS_LVL);
    }

/******************************************************************************
*
* ti572xEmifUpdateTimings - update emif timings
*
* This routine update emif timings for TI AM572x
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xEmifUpdateTimings
    (
    unsigned int base
    )
    {
    sysOutLong(EMIF_SDRAM_REF_CTRL_SHADOW(base), EMIF_SDREF_FINAL);
    sysOutLong(EMIF_SDRAM_TIM_1_SHADOW(base), EMIF_TIME1);
    sysOutLong(EMIF_SDRAM_TIM_2_SHADOW(base), EMIF_TIME2);
    sysOutLong(EMIF_SDRAM_TIM_3_SHADOW(base), EMIF_TIME3);
    sysOutLong(EMIF_SDRAM_MGMT_CTRL(base), EMIF_MGMT_CTRL_VAL);
    sysOutLong(EMIF_SDRAM_MGMT_CTRL_SHD(base), EMIF_MGMT_CTRL_SHD_VAL);
    sysOutLong(EMIF_DLL_CALIB_CTRL_SHADOW(base), EMIF_READ_IDLE_CTRL);
    sysOutLong(EMIF_ZQ_CFG(base), EMIF_ZQ_CFG_VAL);
    sysOutLong(EMIF_TEMP_ALERT_CFG(base), EMIF_ALERT_CFG);
    sysOutLong(EMIF_DDR_PHY_CTRL_1_SHADOW(base), EMIF_DDR_PHY_CTLR1);
    sysOutLong(EMIF_OCP_CFG(base), EMIF_OCP_CFG_VAL);
    }

/******************************************************************************
*
* ti572xSdramInit - initialize DRAM
*
* This routine initializes DRAM for TI AM572x
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xSdramInit
    (
    unsigned int base
    )
    {
    ti572xDdr3Init(base);

    ti572xEmifUpdateTimings(base);
    }

/******************************************************************************
*
* ti572xDdrInit - initialize DRAM
*
* This routine initializes DRAM for TI AM572x
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xDdrInit (void)
    {
    sysOutLong(CM_CLKMODE_DPLL_CORE, 0);

    sysOutLong(DMM_LISA_MAP_3, 0);
    sysOutLong(DMM_LISA_MAP_3, MAP_EMIF);
    sysOutLong(MA_LISA_MAP_3, MAP_EMIF);

    ti572xSdramInit(EMIF1_CFG_BASE);

    ti572xSdramInit(EMIF2_CFG_BASE);
    }

/******************************************************************************
*
* ti572xPllInit - configure PLL settings
*
* This routine configures PLL settings for TI AM572X
*
* NOTE: this routine should never be called by user
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void ti572xPllInit
    (
    const unsigned int base
    )
    {
    sysOutLong(CM_CLKMODE_DPLL(base), 0x6);

    if (base == CM_CORE_BASE)
        {
        sysOutLong(CM_CLKSEL_DPLL(base), 0x00010a04);
        sysOutLong(CM_CLKMODE_DPLL(base), 0x7);
        sysOutLong(CM_DIV_M2_DPLL(base), 2);
        sysOutLong(CM_DIV_H12_DPLL(base), 4);
        sysOutLong(CM_DIV_H13_DPLL(base), 62);
        sysOutLong(CM_DIV_H14_DPLL(base), 5);
        sysOutLong(CM_DIV_H22_DPLL(base), 5);
        sysOutLong(CM_DIV_H23_DPLL(base), 4);
        sysOutLong(CM_DIV_H24_DPLL(base), 6);
        }
    else if (base == CM_PER_BASE)
        {
        sysOutLong(CM_CLKSEL_DPLL(base), 0x00078063);
        sysOutLong(CM_CLKMODE_DPLL(base), 0x7);
        sysOutLong(CM_DIV_M2_DPLL(base), 0x804);
        sysOutLong(CM_DIV_H11_DPLL(base), 0x3);
        sysOutLong(CM_DIV_H12_DPLL(base), 0x4);
        sysOutLong(CM_DIV_H13_DPLL(base), 0x4);
        sysOutLong(CM_DIV_H14_DPLL(base), 0x2);
        }
    else if (base == CM_MPU_BASE)
        {
        sysOutLong(CM_CLKSEL_DPLL(base), 0x0001f409);
        sysOutLong(CM_CLKMODE_DPLL(base), 0x7);
        sysOutLong(CM_DIV_M2_DPLL(base), 0x201);
        }
    else if (base == CM_DDR_BASE)
        {
        sysOutLong(CM_CLKSEL_DPLL(base), 0x00010a04);
        sysOutLong(CM_CLKMODE_DPLL(base), 0x7);
        sysOutLong(CM_DIV_M2_DPLL(base), 2);
        sysOutLong(CM_DIV_H11_DPLL(base), 8);
        }

    /* Wait till the DPLL locks */

    while ((sysInLong(CM_IDLEST_DPLL(base)) & 0x00000001) !=
           CM_DPLL_CLK_MASK);
    }

/******************************************************************************
*
* sysMemDescGet - return system RAM configuration
*
* This routine returns system RAM configuration (starting address and size)
* through output parameter 'outp' and 'size'
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMemDescGet
    (
    struct mem_desc * * outp,
    int * size
    )
    {
    *outp = tiam572xMemDesc;
    *size = 1;
    }

/******************************************************************************
*
* sysUartClkRateGet - return console clock rate
*
* This routine returns system console clock rate
*
* RETURNS: system console clock rate
*
* ERRNO: N/A
*/

uint32_t sysUartClkRateGet(void)
    {
    return 48000000;
    }

/******************************************************************************
*
* sysHwInit - initialize the system
*
* This routine initializes the system and load VxWorks image to
* target address, then jumps to the entry address. If anything goes wrong,
* it will fall through to a dead loop in start.s
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit(void)
    {
    /* Setup the PLLs and the clocks for the peripherals */

    ti572xPllInit(CM_CORE_BASE);
    ti572xPllInit(CM_PER_BASE);
    ti572xPllInit(CM_MPU_BASE);
    ti572xPllInit(CM_DDR_BASE);

    /* Initialize neccessary module clocks */

    ti572xClockInit();

    /* Setup mux pins */

    ti572xMuxInit();

    /* Initialize UART */

    ti572xUartInit();

    /* Initialize DDR */

    ti572xDdrInit();

    /* Main entry point */

    romStart();
    }

