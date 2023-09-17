/*-----------------------------------------------------------------------------
 * hardware.h           header file
 * Copyright            acontis technologies GmbH, Weingarten, Germany
 * Response             Mikhail Ledyaev
 * Description          Hardware registers for Sitara Board.
 *---------------------------------------------------------------------------*/

#ifndef HARDWARE_H
#define HARDWARE_H
#include "vxAtomicArchLibCommon.h"
/*
 * PRUSS_CFG Registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* PRUSS_SYSCFG register for PRUSS standby control. */
#define PRUSS_CFG_SYSCFG                                      (0x4U)

/* IDLE_MODE bits */
#define PRUSS_CFG_SYSCFG_IDLE_MODE_MASK                       (0x00000003U)
#define PRUSS_CFG_SYSCFG_IDLE_MODE_SHIFT                      (0U)
#define PRUSS_CFG_SYSCFG_IDLE_MODE_NO_IDLE_MODE               (1U)

/* STANDBY_INIT bits */
#define PRUSS_CFG_SYSCFG_STANDBY_INIT_MASK                    (0x00000010U)
#define PRUSS_CFG_SYSCFG_STANDBY_INIT_SHIFT                   (4U)
#define PRUSS_CFG_SYSCFG_STANDBY_INIT_INITIATE_STANDBY        (0x00000001U)

/* STANDBY_MODE bits */
#define PRUSS_CFG_SYSCFG_STANDBY_MODE_MASK                    (0x0000000CU)
#define PRUSS_CFG_SYSCFG_STANDBY_MODE_SHIFT                   (2U)
#define PRUSS_CFG_SYSCFG_STANDBY_MODE_NO_STANDBY_MODE         (0x00000001U)


/* PRUSS_GPCFG0 & GPCFG1 registers for PRUSS GPI_MODE. */
#define PRUSS_CFG_GPCFG0                                      (0x8U)
#define PRUSS_CFG_GPCFG1                                      (0xCU)

/* PRUx_GPI_MODE bits. */
#define PRUSS_CFG_GPCFG_GPI_MODE_MASK                         (0x00000003U)
#define PRUSS_CFG_GPCFG_GPI_MODE_SHIFT                        (0U)
#define PRUSS_CFG_GPCFG_GPI_MODE_MII_RT                       (0x00000003U)

/* PRUx_GP_MUX_SEL bits. */
#define PRUSS_CFG_GPCFG_GP_MUX_SEL_MASK                       (0x3C000000U)
#define PRUSS_CFG_GPCFG_GP_MUX_SEL_SHIFT                      (26U)
#define PRUSS_CFG_GPCFG_GP_MUX_SEL_MII2                       (0x00000004U)

/* IEPCLK register for PRUSS clock source control. */
#define PRUSS_CFG_IEPCLK                                      (0x30U)

/* OCP_EN bits. */
#define PRUSS_CFG_IEPCLK_OCP_EN_MASK                          (0x00000001U)
#define PRUSS_CFG_IEPCLK_OCP_EN_SHIFT                         (0U)
#define PRUSS_CFG_IEPCLK_OCP_EN_ICLK                          (0x00000001U)


/* MII_RT register. */
#define PRUSS_CFG_MII_RT                                      (0x2CU)

#define PRUSS_CFG_MII_RT_MII_RT_EVENT_EN_MASK                 (0x00000001U)
#define PRUSS_CFG_MII_RT_MII_RT_EVENT_EN_SHIFT                (0U)
#define PRUSS_CFG_MII_RT_MII_RT_EVENT_EN_ENABLE               (0x00000001U)


/* Scratch Pad register. */
#define PRUSS_CFG_SPP                                         (0x34U)

/* XFR_SHIFT_EN bit */
#define PRUSS_CFG_SPP_SPP_XFR_SHIFT_EN_MASK                   (0x00000002U)
#define PRUSS_CFG_SPP_SPP_XFR_SHIFT_EN_SHIFT                  (1U)
#define PRUSS_CFG_SPP_SPP_XFR_SHIFT_EN_ENABLE                 (0x00000001U)



/*
 * PRUSS_INTC Registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* SIPR registers to define polarity of system interrupts for PRUSSx. */
#define PRUSS_INTC_SIPR0                                      (0xD00U)
#define PRUSS_INTC_SIPR1                                      (0xD04U)
#define PRUSS_INTC_SIPR_HIGH_POLARITY_ALL                     (0xFFFFFFFFU)

/* SITR registers to define pulse type of system interrupts for PRUSSx. */
#define PRUSS_INTC_SITR0                                      (0xD80U)
#define PRUSS_INTC_SITR1                                      (0xD84U)
#define PRUSS_INTC_SITR_PULSE_ALL                             (0x0U)


/* CMRx register to map INTC channels to system interrupts.  */
#define PRUSS_INTC_CMR0                                       (0x400U)


/* HMRx register to map INTC channels to Host interrupts.  */
#define PRUSS_INTC_HMR0                                       (0x800U)


/* ESR register to enable system interrupts. */
#define PRUSS_INTC_ESR0                                       (0x300U)
#define PRUSS_INTC_ERS1                                       (0x304U)

/* ENABLE_SET_31_0, ENABLE_SET_63_32 bits */
#define PRUSS_INTC_ESR_ENABLE_SET_31_0_MASK                   (0xFFFFFFFFU)
#define PRUSS_INTC_ESR_ENABLE_SET_31_0_SHIFT                  (0U)
#define PRUSS_INTC_ESR_ENABLE_SET_63_32_MASK                  (0xFFFFFFFFU)
#define PRUSS_INTC_ESR_ENABLE_SET_63_32_SHIFT                 (0U)


/* SECRx register to poll system interrupt status */
#define PRUSS_INTC_SECR0                                      (0x280U)
#define PRUSS_INTC_SECR1                                      (0x284U)
#define PRUSS_INTC_SECR1_LINK0_PRU_EVT_MASK                   (0x200U)
#define PRUSS_INTC_SECR1_LINK1_PRU_EVT_MASK                   (0x200000U)


/* ENA_STATUS_31_0, ENA_STATUS_63_32 bits. */
#define PRUSS_INTC_ENA_STATUS_31_0_MASK                       (0xFFFFFFFFU)
#define PRUSS_INTC_ENA_STATUS_31_0_SHIFT                      (0U)
#define PRUSS_INTC_ENA_STATUS_63_32_MASK                      (0xFFFFFFFFU)
#define PRUSS_INTC_ENA_STATUS_63_32_SHIFT                     (0U)


/* HIER register to enable/disable individual interrupts. */
#define PRUSS_INTC_HIER                                       (0x1500U)

/* ENABLE_HINT bits. */
#define PRUSS_INTC_HIER_ENABLE_HINT_MASK                      (0x000003FFU)
#define PRUSS_INTC_HIER_ENABLE_HINT_SHIFT                     (0U)


/* GER register to enable global interrupts (ignore individual settings). */
#define PRUSS_INTC_GER                                        (0x10U)

#define PRUSS_INTC_GER_ENABLE_HINT_ANY_MASK                   (0x00000001U)
#define PRUSS_INTC_GER_ENABLE_HINT_ANY_SHIFT                  (0U)



/*
 * PRUSS_MII_RT Registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* PRUSS_MII_RT_RXCFG0 register has configuration variables for the RX path. */
#define PRUSS_MII_RT_RXCFG0                                   (0x0U)
#define PRUSS_MII_RT_RXCFG1                                   (0x4U)

/* RX_ENABLE bits */
#define PRUSS_MII_RT_RXCFG_RX_ENABLE_MASK                     (0x00000001U)
#define PRUSS_MII_RT_RXCFG_RX_ENABLE_SHIFT                    (0U)

/* RX_DATA_RDY_MODE_DIS bits */
#define PRUSS_MII_RT_RXCFG_RX_DATA_RDY_MODE_DIS_SHIFT         (1U)
#define PRUSS_MII_RT_RXCFG_RX_DATA_RDY_MODE_DIS_MASK          (0x00000002U)
#define PRUSS_MII_RT_RXCFG_RX_DATA_RDY_MODE_DIS_TX_EOF_MODE   (1)

/* RX_MUX_SEL bits */
#define PRUSS_MII_RT_RXCFG_RX_MUX_SEL_MASK                    (0x00000008U)
#define PRUSS_MII_RT_RXCFG_RX_MUX_SEL_SHIFT                   (3U)

/* RX_L2_EN bits */
#define PRUSS_MII_RT_RXCFG_RX_L2_EN_MASK                      (0x00000010U)
#define PRUSS_MII_RT_RXCFG_RX_L2_EN_SHIFT                     (4U)

/* RX_CUT_PREAMBLE */
#define PRUSS_MII_RT_RXCFG0_RX_CUT_PREAMBLE_MASK              (0x00000004U)
#define PRUSS_MII_RT_RXCFG0_RX_CUT_PREAMBLE_SHIFT             (2U)

#define PRUSS_MII_RT_RXCFG_RX_L2_EOF_SCLR_DIS_SHIFT           (9U)
#define PRUSS_MII_RT_RXCFG_RX_L2_EOF_SCLR_DIS_MASK            (0x00000200U)


/* TXCFG registers to control TX path for port 0 & port 1 */
#define PRUSS_MII_RT_TXCFG0                                   (0x10U)
#define PRUSS_MII_RT_TXCFG1                                   (0x14U)

/* TX_ENABLE bits */
#define PRUSS_MII_RT_TXCFG_TX_ENABLE_MASK                     (0x00000001U)
#define PRUSS_MII_RT_TXCFG_TX_ENABLE_SHIFT                    (0U)

/* TX_AUTO_PREAMBLE bits */
#define PRUSS_MII_RT_TXCFG_TX_AUTO_PREAMBLE_MASK              (0x00000002U)
#define PRUSS_MII_RT_TXCFG_TX_AUTO_PREAMBLE_SHIFT             (1U)

/* TX_32_MODE_EN bits */
#define PRUSS_MII_RT_TXCFG_TX_32_MODE_EN_MASK                 (0x00000800U)
#define PRUSS_MII_RT_TXCFG_TX_32_MODE_EN_SHIFT                (11U)

/* TX_MUX_SEL bits */
#define PRUSS_MII_RT_TXCFG_TX_MUX_SEL_MASK                    (0x00000100U)
#define PRUSS_MII_RT_TXCFG_TX_MUX_SEL_SHIFT                   (8U)

/* TX_START_DELAY bits */
#define PRUSS_MII_RT_TXCFG_TX_START_DELAY_MASK                (0x03FF0000U)
#define PRUSS_MII_RT_TXCFG_TX_START_DELAY_SHIFT               (16U)
#define PRUSS_MII_RT_TXCFG_TX_START_DELAY_RESET               (0x40U)

/* TX_CLK_DELAY bits */
#define PRUSS_MII_RT_TXCFG_TX_CLK_DELAY_MASK                  (0x70000000U)
#define PRUSS_MII_RT_TXCFG_TX_CLK_DELAY_SHIFT                 (28U)
#define PRUSS_MII_RT_TXCFG_TX_CLK_DELAY_VALUE                 (6U)


/* PRUSS_MII_RT_TX_IPGx register to define minimum of transmit Inter Packet Gap. */
#define PRUSS_MII_RT_TX_IPG0                                  (0x30U)
#define PRUSS_MII_RT_TX_IPG1                                  (0x34U)
#define PRUSS_MII_RT_TX_IPG_TX_IPG_VALUE                      (0xb8U)



/*
 * PRUSS_IEP registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* PRUSS_IEP_GLOBAL_CFG register to control counter. */
#define PRUSS_IEP_GLOBAL_CFG                                  (0x0U)
#define PRUSS_IEP_GLOBAL_CFG_VALUE                            (0x0551U)



/*
 * PRUSS_PRU_CTRL registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* PRU_CONTROL register */
#define PRUSS_PRU_CONTROL                                     (0x00000000U)

/* ENABLE bits */
#define PRUSS_PRU_CONTROL_ENABLE_MASK                         (0x00000002U)
#define PRUSS_PRU_CONTROL_ENABLE_SHIFT                        (1U)
#define PRUSS_PRU_CONTROL_ENABLE_ENABLEVAL                    (0x00000001U)
#define PRUSS_PRU_CONTROL_ENABLE_DISABLEVAL                   (0x00000000U)


/* PRU_CTPPR0/PRU_CTPPR1 registers */
#define PRUSS_PRU_CTPPR0                                      (0x28U)
#define PRUSS_PRU_CTPPR1                                      (0x2CU)



/*
 * PRUSS_MDIO registers.
 * Bit masks for HW_WR_FIELD32 macro.
 */

/* MDIO_USERPHYSELx registers to control link change interrupt */
#define PRUSS_MDIO_USERPHYLSEL(n)                             ((EC_T_DWORD)0x84U + ((n) * ((EC_T_DWORD)(0x8U))))
#define PRUSS_MDIO_USERPHYLSEL_ENABLE_LINK_INTERRUPT_VALUE(port) (port | 0x40)


/* MDIO_USERACCESSx registers */
#define PRUSS_MDIO_USERACCESS(n)                              ((EC_T_DWORD)0x80U + ((n) * ((EC_T_DWORD)(0x8U))))

/* GO bits */
#define PRUSS_MDIO_USERACCESS_GO_MASK                         ((EC_T_DWORD)(0x80000000U))
#define PRUSS_MDIO_USERACCESS_GO_SHIFT                        ((EC_T_DWORD)(31U))
#define PRUSS_MDIO_USERACCESS_GO_0x1_VAL                      ((EC_T_DWORD)(1U))
#define PRUSS_MDIO_USERACCESS_GO_0x0_VAL                      ((EC_T_DWORD)(0U))

/* WRITE bits */
#define PRUSS_MDIO_USERACCESS_WRITE_MASK                      ((EC_T_DWORD)(0x40000000U))
#define PRUSS_MDIO_USERACCESS_WRITE_SHIFT                     ((EC_T_DWORD)(30U))
#define PRUSS_MDIO_USERACCESS_WRITE_WRITE_VAL                 ((EC_T_DWORD)(0x1U))
#define PRUSS_MDIO_USERACCESS_WRITE_READ_VAL                  ((EC_T_DWORD)(0x0U))

/* PHYADR bits */
#define PRUSS_MDIO_USERACCESS_PHYADR_MASK                     ((EC_T_DWORD)(0x001F0000U))
#define PRUSS_MDIO_USERACCESS_PHYADR_SHIFT                    ((EC_T_DWORD)(16U))

/* REGADR bits */
#define PRUSS_MDIO_USERACCESS_REGADR_MASK                     ((EC_T_DWORD)(0x03E00000U))
#define PRUSS_MDIO_USERACCESS_REGADR_SHIFT                    ((EC_T_DWORD)(21U))

/* ACK bits */
#define PRUSS_MDIO_USERACCESS_ACK_MASK                        ((EC_T_DWORD)(0x20000000U))
#define PRUSS_MDIO_USERACCESS_ACK_SHIFT                       ((EC_T_DWORD)(29U))
#define PRUSS_MDIO_USERACCESS_ACK_PASS_VAL                    ((EC_T_DWORD)(1U))
#define PRUSS_MDIO_USERACCESS_ACK_FAIL_VAL                    ((EC_T_DWORD)(0U))

/* DATA bits */
#define PRUSS_MDIO_USERACCESS_DATA_MASK                       ((EC_T_DWORD)(0x0000FFFFU))
#define PRUSS_MDIO_USERACCESS_DATA_SHIFT                      ((EC_T_DWORD)(0U))


/* MDIO LINK register to check if PHY link is present */
#define PRUSS_MDIO_LINK                                       ((EC_T_DWORD)(0xCU))


/* MDIO_LINKINTMASKED to clear signalled interrupts */
#define PRUSS_MDIO_LINKINTMASKED                              ((EC_T_DWORD)0x14U)


/* MDIO_CONTROL register to control state machine state */
#define PRUSS_MDIO_CONTROL                                    (0x4U)

/* ENABLE bits */
#define PRUSS_MDIO_CONTROL_ENABLE_MASK                        ((EC_T_DWORD)(0x40000000U))
#define PRUSS_MDIO_CONTROL_ENABLE_SHIFT                       ((EC_T_DWORD)(30U))

/* CLKDIV bits */
#define PRUSS_MDIO_CONTROL_CLKDIV_MASK                        ((EC_T_DWORD)(0xFFU))
#define PRUSS_MDIO_CONTROL_CLKDIV_SHIFT                       ((EC_T_DWORD)(0U))
#define PRUSS_MDIO_CONTROL_CLKDIV_DEFVALUE                    ((EC_T_DWORD)(0x9FU))



/*
 * GPIO3 & GPIO 5 registers.
 */

/* GPIO_CTRL register */
#define GPIO_CTRL                                             (0x130U)

/* DISABLEMODULE bit */
#define GPIO_CTRL_DISABLEMODULE_SHIFT                         (0U)
#define GPIO_CTRL_DISABLEMODULE_MASK                          (0x00000001U)
#define GPIO_CTRL_DISABLEMODULE_ENABLED                       (0U)
#define GPIO_CTRL_DISABLEMODULE_DISABLED                      (1U)


/* GPIO_OE register. */
#define GPIO_OE                                               (0x134U)
#define GPIO_PIN_INPUT                                        (EC_T_DWORD) (0x1U)
#define GPIO_PIN_OUTPUT                                       (EC_T_DWORD) (0x0U)


/* GPIO_SETDATAOUT register to set pulse mode for pin interrupt */
#define GPIO_SETDATAOUT                                       (0x194U)
#define GPIO_PIN_HIGH                                         (EC_T_DWORD) (0x1U)


/* GPIO_CLEARDATAOUT register to set pulse mode to low */
#define GPIO_CLEARDATAOUT                                     (0x190U)
#define GPIO_PIN_LOW                                          (EC_T_DWORD) (0x0U)



/*
 * CTRL_MODULE_CORE registers.
 * PAD_IO - starts at 0x1400 offset.
 * dra7_pmx_core: pinmux@1400 ---> Pinmux registers start address.
 */

#define CTRL_CORE_PAD_MCASP1_AXR4                             (0x16C4U)
#define CTRL_CORE_PAD_MCASP1_AXR6                             (0x16CCU)
#define CTRL_CORE_PAD_VIN2A_CLK0                              (0x1554U)
#define CTRL_CORE_PAD_MCASP1_AXR5                             (0x16C8U)
#define CTRL_CORE_PAD_VIN2A_DE0                               (0x1558U)
#define CTRL_CORE_PAD_MCASP1_AXR6                             (0x16CCU)
#define CTRL_CORE_PAD_VIN2A_HSYNC0                            (0x1560U)
#define CTRL_CORE_PAD_VIN2A_FLD0                              (0x155CU)
#define CTRL_CORE_PAD_MCASP1_AXR7                             (0x16D0U)
#define CTRL_CORE_PAD_VIN2A_D4                                (0x1578U)



/*
 * CM_ clock & power domain control manager (PRCM).
 */

/* CM_L4PER clock domain register group offset to CM_CORE */
#define CM_CORE_L4PER_OFFSET                                  (0x1700)


/* PRUSS_CLKCTRL - clock control register for PRUSS2 in L4PER2 domain */
#define CM_CORE_L4PER2_PRUSS_CLKCTRL(nPruss)                  (CM_CORE_L4PER_OFFSET + 0x18 + nPruss*8)

/* MODULEMODE bits */
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_MODULEMODE_MASK          ((EC_T_DWORD)(0x3U))
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_MODULEMODE_SHIFT         ((EC_T_DWORD)(0U))
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_MODULEMODE_ENABLE        ((EC_T_DWORD)(0x2U))

/* IDLEST bits */
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST_MASK              ((EC_T_DWORD)(0x300U))
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST_SHIFT             ((EC_T_DWORD)(16U))
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST_MODULE_DISABLED   ((EC_T_DWORD)(0x3U))
#define CM_CORE_L4PER2_PRUSS_CLKCTRL_IDLEST_MODULE_FULLY_FINCTIONAL ((EC_T_DWORD)(0x0U))


/* Power state transition for L4PER2 clock domain */
#define CM_CORE_L4PER2_CLKSTCTRL                              (CM_CORE_L4PER_OFFSET + 0x1FC)

#define CM_CORE_L4PER2_CLKSTCTRL_ICSS_CLK_MASK                ((EC_T_DWORD)(0x100U))
#define CM_CORE_L4PER2_CLKSTCTRL_ICSS_CLK_SHIFT               ((EC_T_DWORD)(8U))
#define CM_CORE_L4PER2_CLKSTCTRL_PER_192M_GFCLK_MASK          ((EC_T_DWORD)(0x2000U))
#define CM_CORE_L4PER2_CLKSTCTRL_PER_192M_GFCLK_SHIFT         ((EC_T_DWORD)(13U))
#define CM_CORE_L4PER2_CLKSTCTRL_ICSS_IEP_CLK_MASK            ((EC_T_DWORD)(0x4000U))
#define CM_CORE_L4PER2_CLKSTCTRL_ICSS_IEP_CLK_SHIFT           ((EC_T_DWORD)(14U))
#define CM_CORE_L4PER2_CLKSTCTRL_CLOCK_RUNNING                ((EC_T_DWORD)(0x1U))


/*
 * PHY (TLK).
 */

/* PHYSTS register to get all general information */
#define PHY_PHYSTS                                            ((EC_T_DWORD)0x10U)

/* SPEED_STATUS bit */
#define PHY_PHYSTS_SPEED_STATUS                               (((EC_T_DWORD)1U) << 1)
/* DUPLEX_STATUS bit */
#define PHY_PHYSTS_DUPLEX_STATUS                              (((EC_T_DWORD)1U) << 2)

/* PHY speed */
enum PHY_SPEED
{
   PHY_SPEED_100FD = 0x1,
   PHY_SPEED_10FD,
   PHY_SPEED_100HD,
   PHY_SPEED_10HD
};



/*
 * PRUSS_INTC Interrupts->Channels->Events mapping.
 */

/* Total number of system interrupts */
#define BOARD_SYSTEM_INTERRUPT_COUNT                          (64U)
/* Number of channels in each INTC instance */
#define PRUSS_INTC_CHANNEL_COUNT                              (10U)
/* Number of PRU events */
#define PRUSS_PRU_HOST_EVENT_COUNT                            (10U)


/* System Interrupts */
#define IEP_TIM_CAP_CMP_EVENT                                 7
#define SYNC1_OUT_EVENT                                       13
#define SYNC0_OUT_EVENT                                       14
#define PRU_ARM_EVENT0                                        20 //SYS_EVT_AL_EVENT_REQUEST
#define PRU_ARM_EVENT1                                        21 //
#define PRU_ARM_EVENT2                                        22
#define PRU_ARM_EVENT3                                        23
#define PRU_ARM_EVENT4                                        24
#define PRU_ARM_EVENT5                                        25
#define PRU_ARM_EVENT6                                        26
#define PRU_ARM_EVENT7                                        27
#define ARM_PRU_EVENT                                         16 //SYS_EVT_HOST_CMD
#define PRU0_RX_ERR32_EVENT                                   33
#define PORT1_TX_UNDERFLOW                                    39
#define PORT1_TX_OVERFLOW                                     40
#define MII_LINK0_EVENT                                       41
#define PORT1_RX_EOF_EVENT                                    42
#define PRU1_RX_ERR32_EVENT                                   45
#define PORT2_TX_UNDERFLOW                                    51
#define PORT2_TX_OVERFLOW                                     53
#define PORT2_RX_EOF_EVENT                                    54
#define MII_LINK1_EVENT                                       53

/* Interrupt controller channels */
#define CHANNEL0                                              0
#define CHANNEL1                                              1
#define CHANNEL2                                              2
#define CHANNEL3                                              3
#define CHANNEL4                                              4
#define CHANNEL5                                              5
#define CHANNEL6                                              6
#define CHANNEL7                                              7
#define CHANNEL8                                              8
#define CHANNEL9                                              9

/* Pru hosts events */
#define PRU0                                                  0
#define PRU1                                                  1
#define PRU_EVTOUT0                                           2
#define PRU_EVTOUT1                                           3
#define PRU_EVTOUT2                                           4
#define PRU_EVTOUT3                                           5
#define PRU_EVTOUT4                                           6
#define PRU_EVTOUT5                                           7
#define PRU_EVTOUT6                                           8
#define PRU_EVTOUT7                                           9

/* Masks for host events */
#define PRU0_HOSTEN_MASK                                      0x0001
#define PRU1_HOSTEN_MASK                                      0x0002
#define PRU_EVTOUT0_HOSTEN_MASK                               0x0004
#define PRU_EVTOUT1_HOSTEN_MASK                               0x0008
#define PRU_EVTOUT2_HOSTEN_MASK                               0x0010
#define PRU_EVTOUT3_HOSTEN_MASK                               0x0020
#define PRU_EVTOUT4_HOSTEN_MASK                               0x0040
#define PRU_EVTOUT5_HOSTEN_MASK                               0x0080
#define PRU_EVTOUT6_HOSTEN_MASK                               0x0100
#define PRU_EVTOUT7_HOSTEN_MASK                               0x0200

#define SYS_EVT_POLARITY_LOW                                  0
#define SYS_EVT_POLARITY_HIGH                                 1

#define SYS_EVT_TYPE_PULSE                                    0
#define SYS_EVT_TYPE_EDGE                                     1



/*
 * Macros to read/write hardware memory/registers.
 */

inline void HW_MEM_BARRIER(void)
{
    LinkOsMemoryBarrier();
}


/* Without memory barrier */
#define HW_SET_FIELD32(regVal, REG_FIELD, fieldVal)                            \
    ((regVal) = ((regVal) & (uint32_t) (~(uint32_t) REG_FIELD##_MASK)) |       \
                    ((((uint32_t) (fieldVal)) << (uint32_t) REG_FIELD##_SHIFT) \
                        & (uint32_t) REG_FIELD##_MASK))
#define HW_SET32_NO_BARRIER(x, y)                                              \
        (*((volatile EC_T_DWORD *)(x))) = (y)
#define HW_SET16_NO_BARRIER(x, y)                                              \
        (*((volatile EC_T_WORD *)(x))) = (y)
#define HW_SET8_NO_BARRIER(x, y)                                               \
        (*((volatile EC_T_BYTE *)(x))) = (y)

#define HW_GET16_NO_BARRIER(x)                                                 \
        (*((volatile EC_T_WORD *)(x)))
#define HW_GET32_NO_BARRIER(x)                                                 \
        (*((volatile EC_T_DWORD *)(x)))


/* With memory barrier */
#define HW_WR_REG8(addr, value)                                                \
     (HW_WR_REG8_RAW((EC_T_DWORD) (addr), (EC_T_BYTE) (value)))

#define HW_WR_REG16(addr, value)                                               \
     (HW_WR_REG16_RAW((EC_T_DWORD) (addr), (EC_T_WORD) (value)))

#define HW_WR_REG32(addr, value)                                               \
    (HW_WR_REG32_RAW((EC_T_DWORD) (addr), (EC_T_DWORD) (value)))

#define HW_WROR_REG32(addr, value)                                             \
    (HW_WROR_REG32_RAW((EC_T_DWORD) (addr), (EC_T_DWORD) (value)))

#define HW_WR_FIELD32(regAddr, REG_FIELD, fieldVal)                            \
    (HW_WR_FIELD32_RAW((EC_T_DWORD) (regAddr), ((EC_T_DWORD)REG_FIELD##_MASK), \
                          ((EC_T_DWORD)REG_FIELD##_SHIFT), (EC_T_DWORD)(fieldVal)))

#define HW_RD_REG16(addr)                                                      \
    (HW_RD_REG16_RAW((EC_T_DWORD) (addr)))

#define HW_RD_REG32(addr)                                                      \
    (HW_RD_REG32_RAW((EC_T_DWORD) (addr)))

#define HW_RD_FIELD32(regAddr, REG_FIELD)                                      \
    (HW_RD_FIELD32_RAW((EC_T_DWORD) (regAddr), ((EC_T_DWORD) REG_FIELD##_MASK),\
                          ((EC_T_DWORD) REG_FIELD##_SHIFT)))


static inline EC_T_WORD HW_RD_REG16_RAW(EC_T_DWORD dwAddr)
{
    EC_T_WORD dwRegVal = *(volatile EC_T_WORD *) dwAddr;
    HW_MEM_BARRIER();
    return (dwRegVal);
}

static inline EC_T_DWORD HW_RD_REG32_RAW(EC_T_DWORD dwAddr)
{
    EC_T_DWORD dwRegVal = *(volatile EC_T_DWORD *) dwAddr;
    HW_MEM_BARRIER();
    return (dwRegVal);
}

static inline EC_T_DWORD HW_RD_FIELD32_RAW(EC_T_DWORD dwAddr,
                                           EC_T_DWORD dwMask,
                                           EC_T_DWORD dwShift)
{
    EC_T_DWORD regVal = *(volatile EC_T_DWORD *) dwAddr;
    regVal = (regVal & dwMask) >> dwShift;
    HW_MEM_BARRIER();
    return (regVal);
}

inline void HW_WR_REG8_RAW(EC_T_DWORD dwAddr, EC_T_BYTE byValue)
{
     *(volatile EC_T_BYTE *) dwAddr = byValue;
     HW_MEM_BARRIER();
     return;
}

inline void HW_WR_REG16_RAW(EC_T_DWORD addr, EC_T_WORD value)
{
    *(volatile EC_T_WORD *) addr = value;
    HW_MEM_BARRIER();
    return;
}

inline void HW_WR_REG32_RAW(EC_T_DWORD addr, EC_T_DWORD value)
{
    *(volatile EC_T_DWORD *) addr = value;
    HW_MEM_BARRIER();
    return;
}

inline void HW_WROR_REG32_RAW(EC_T_DWORD addr, EC_T_DWORD value)
{
    *(volatile EC_T_DWORD *) addr |= value;
    HW_MEM_BARRIER();
    return;
}

inline void HW_WR_FIELD32_RAW(EC_T_DWORD dwAddr,
                                     EC_T_DWORD dwMask,
                                     EC_T_DWORD dwShift,
                                     EC_T_DWORD dwValue)
{
	EC_T_DWORD regVal = *(volatile EC_T_DWORD *) dwAddr;
    regVal &= (~dwMask);
    regVal |= (dwValue << dwShift) & dwMask;
    *(volatile EC_T_DWORD *) dwAddr = regVal;
    HW_MEM_BARRIER();
    return;
}



/*
 * Other defines.
 */

#define PRUSS_PRU_COUNT                                       (2)

#define PRUSS_SHARED_MEMORY_SIZE                              (12*1024)
/* Relative offset from the point of view of PRUSS */
#define PRUSS_SHARED_MEMORY_PHYS_OFFSET                       (0x10000)

#define BOARD_L3OCMC1_MEMORY_SIZE                             (512*1024)



/*
 * PRUSS_INTC_SYSEVT_TO_CHANNEL_MAP.
 *
 * Description: Mapping system interrupt to INTC channel.
 *
 */
struct PRUSS_INTC_SYSEVT_TO_CHANNEL_MAP
{
    EC_T_BYTE bySysevt;                    /* System event number 0..63 */
    EC_T_BYTE byChannel;                   /* INTC Channel number 0..9 */
    EC_T_BYTE byPolarity;                  /* Interrupt polarity [ 0 - active low , 1 - active high ] */
    EC_T_BYTE byType;                      /* Interrupt type [ 0 - level or pulse interrupt , 1 - edge interrupt ] */
};

/*
 * PRUSS_INTC_CHANNEL_TO_HOST_MAP.
 *
 * Description: Mapping controller channel to PRU event.
 *
 */
struct PRUSS_INTC_CHANNEL_TO_HOST_MAP 
{
     EC_T_WORD wChannel;                   /* Controller channel 0..9 */
     EC_T_WORD wHost;                      /* PRU event 0..9 */
};

/*
 * ICSS_INTC_DATA.
 *
 * Description: Describing all system interrupts and mapping all of them.
 *
 */
struct PRUSS_INTC_MAPPING
{
    /* Range:0..63, -1 indicates end of list. */
    EC_T_BYTE      bSystemInterruptEnabled[BOARD_SYSTEM_INTERRUPT_COUNT];

    /* System interrupt to INTC Channel map. {-1, -1} indicates end of list. */
    PRUSS_INTC_SYSEVT_TO_CHANNEL_MAP
                   mapSystemInterruptToChannel[BOARD_SYSTEM_INTERRUPT_COUNT];

    /* Channel to Host map. {-1, -1} indicates end of list. */
    PRUSS_INTC_CHANNEL_TO_HOST_MAP
                   mapChannelToPruEvent[PRUSS_PRU_HOST_EVENT_COUNT];

    /* 10-bit mask - Enable Host0-Host9 events. */
    EC_T_DWORD     dwPruEventBitmask;
};

/*
 * PRUSS_INTC_INITDATA.
 *
 * All specific mapping of all interrupts, channels and events.
 *
 */
#define PRUSS_INTC_INITDATA                                                       \
{                                                                                 \
    /*.bSystemInterruptEnabled =*/ { PRU_ARM_EVENT0,                              \
                                 PRU_ARM_EVENT1,                                  \
                                 PRU_ARM_EVENT2,                                  \
                                 PRU_ARM_EVENT3,                                  \
                                 PRU_ARM_EVENT4,                                  \
                                 PRU_ARM_EVENT5,                                  \
                                 PORT1_RX_EOF_EVENT,                              \
                                 PORT2_RX_EOF_EVENT,                              \
                                 MII_LINK0_EVENT,                                 \
                                 MII_LINK1_EVENT,                                 \
                                 0xFF },                                          \
                                                                                  \
    /*.mapSystemInterruptToChannel = */                                           \
    {                                                                             \
        {PORT1_RX_EOF_EVENT,CHANNEL0, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE}, \
        {PORT2_RX_EOF_EVENT,CHANNEL1, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE}, \
        {PRU_ARM_EVENT0,CHANNEL2, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},     \
        {PRU_ARM_EVENT1, CHANNEL3, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},    \
        {PRU_ARM_EVENT2,CHANNEL4, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},     \
        {PRU_ARM_EVENT3, CHANNEL5, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},    \
        {PRU_ARM_EVENT4, CHANNEL4, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},    \
        {MII_LINK0_EVENT, CHANNEL7, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},   \
        {MII_LINK1_EVENT, CHANNEL8, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},   \
        {PRU_ARM_EVENT5, CHANNEL5, SYS_EVT_POLARITY_HIGH ,SYS_EVT_TYPE_PULSE},    \
        {0xFF,0xFF,0xFF,0xFF}                                                     \
     },                                                                           \
                                                                                  \
     /*.mapChannelToPruEvent =*/ { {CHANNEL0,PRU0},                               \
                               {CHANNEL1, PRU1},                                  \
                               {CHANNEL2, PRU_EVTOUT0},                           \
                               {CHANNEL3, PRU_EVTOUT1},                           \
                               {CHANNEL4, PRU_EVTOUT2},                           \
                               {CHANNEL5, PRU_EVTOUT3},                           \
                               {CHANNEL6, PRU_EVTOUT4},                           \
                               {CHANNEL7, PRU_EVTOUT6},                           \
                               {CHANNEL8, PRU_EVTOUT7},                           \
                               {0xFF, 0xFF}                                       \
                             },                                                   \
                                                                                  \
      /*.dwPruEventBitmask =*/   ( PRU0_HOSTEN_MASK |                                 \
                               PRU1_HOSTEN_MASK |                                 \
                               PRU_EVTOUT0_HOSTEN_MASK |                          \
                               PRU_EVTOUT1_HOSTEN_MASK |                          \
                               PRU_EVTOUT2_HOSTEN_MASK |                          \
                               PRU_EVTOUT3_HOSTEN_MASK |                          \
                               PRU_EVTOUT4_HOSTEN_MASK |                          \
                               PRU_EVTOUT5_HOSTEN_MASK |                          \
                               PRU_EVTOUT6_HOSTEN_MASK |                          \
                               PRU_EVTOUT7_HOSTEN_MASK)                           \
}



/*
 * GPIO_PINMUX_INFORMATION
 *
 * Description: It is used to specify interrupt/reset modes for GPIO pins.
 */

struct GPIO_PINMUX_INFORMATION 
{
    EC_T_DWORD dwInstance;
    EC_T_DWORD dwPin;
    EC_T_DWORD dwPinMux;
    EC_T_DWORD dwBaseAddr;
};


/*
 * AM57XX_HARDWARE_INITDATA
 *
 * Description: physical memory for AM572x/AM571x IDK board description.
 *              EC_T_LINK_PLATFORMCONFIGURATION_ICSS structure is used.
 *
 */

#define AM57XX_HARDWARE_INITDATA                                               \
{                                                                              \
    /*.pruss =  */                                                             \
    {                                                                          \
        {                                                                      \
            /*.pru = */                                                        \
            {                                                                  \
             {                                                                 \
                /*.iram            =*/ {0x4B234000, 0x3000, 0},                \
                /*.control         =*/ {0x4B222000, 0x1000, 0},                \
                /*.dram            =*/ {0x4B200000, 0x2000, 0}                 \
             },                                                                \
             {                                                                 \
                 /*.iram            =*/ {0x4B238000, 0x3000, 0},               \
                 /*.control         =*/ {0x4B224000, 0x1000, 0},               \
                 /*.dram            =*/ {0x4B202000, 0x2000, 0}                \
             }                                                                 \
            },                                                                 \
            /*.interruptController =*/ {0x4B220000, 0x2000, 0},                \
            /*.cfg                 =*/ {0x4B226000, 0x1000, 0},                \
            /*.iep                 =*/ {0x4B22e000, 0x1000, 0},                \
            /*.miiRtCfg            =*/ {0x4B232000, 0x1000, 0},                \
            /*.mdio                =*/ {0x4B232400, 0x1000, 0},                \
            /*.shrdram2            =*/ {0x4B210000, 0x8000, 0},                \
            /*.l3ocmc              =*/ {0x40300000, 0x80000, 0}                \
        },                                                                     \
        {                                                                      \
            /*.pru =  */                                                       \
            {                                                                  \
                {                                                              \
                   /*.iram            =*/ {0x4B2b4000, 0x3000, 0},             \
                   /*.control         =*/ {0x4B2a2000, 0x1000, 0},             \
                   /*.dram            =*/ {0x4B280000, 0x2000, 0}              \
                },                                                             \
                {                                                              \
                    /*.iram            =*/ {0x4B2b8000, 0x3000, 0},            \
                    /*.control         =*/ {0x4B2a4000, 0x1000, 0},            \
                    /*.dram            =*/ {0x4B282000, 0x2000, 0}             \
                }                                                              \
            },                                                                 \
            /*.interruptController =*/ {0x4B2A0000, 0x2000, 0},                \
            /*.cfg                 =*/ {0x4B2a6000, 0x1000, 0},                \
            /*.iep                 =*/ {0x4B2ae000, 0x1000, 0},                \
            /*.miiRtCfg            =*/ {0x4B2b2000, 0x1000, 0},                \
            /*.mdio                =*/ {0x4B2b2400, 0x1000, 0},                \
            /*.shrdram2            =*/ {0x4B290000, 0x8000, 0},                \
            /*.l3ocmc              =*/ {0x40300000, 0x80000, 0}                \
        },                                                                     \
    },                                                                         \
    /*.mpuGpio3 =*/ {0x48057000, 0x1000, 0},                                   \
    /*.mpuGpio4 =*/ {0x48059000, 0x1000, 0},                                   \
    /*.mpuGpio5 =*/ {0x4805b000, 0x1000, 0},                                   \
    /*.mpuScm =*/   {0x4a002000, 0x2000, 0},                                   \
    /*.cmCore =*/   {0x4a008000, 0x3000, 0}                                    \
}

#endif
