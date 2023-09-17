/* vxbFdtTiPrussMdioEnd.h - header file for TI PRU ICSS MDIO driver */

/*
 * TDB
 *  */

/*
modification history
--------------------
8Mar17,Veerabhadra S written
*/

#ifndef __INCTiPrussMdioh
#define __INCTiPrussMdioh

#include <vxWorks.h>
#include <semLib.h>
#include <hwif/vxBus.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PRU_MDIO_CONTROL            (0x4)
#define PRU_USERACCESSn(n)          (0x80 + (n) * 0x8)
#define PRU_MDIO_CLK_DIV            (0xff)
#define PRU_MDIO_EN                 (0x40000000)
#define PRU_MDIO_GO                 (0x80000000)
#define PRU_MDIO_WRITE              (0x40000000)
#define PRU_MDIO_ACK                (0x20000000)
#define PRU_PHY_ADDR_SHIFT          (16)
#define PRU_REG_ADDR_SHIFT          (21)
#define MDIO_USERINTRAW             (0x20)

#define MDIO_USERPHYSEL(n)          (0x84U + (n * 0x8U))

#define MDIO_LINKINTRAW              0x10
#define MDIO_LINKINTMASKED           0x14


#define MDIO_USERINTMASKED           0x24

#define MDIO_USERINTMASKSET          0x28
#define MDIO_USERINTMASKCLR          0x2c


//shared RAM register addresses
#define	ESC_ADDR_MI_ECAT_ACCESS	    0x516
#define	ESC_ADDR_MI_PDI_ACCESS	    0x517
#define PRU_ICSS_SHARED_RAM         0x10000
#define ESC_ADDR_TI_PORT0_PHYADDR	0xE08
#define ESC_ADDR_TI_PORT1_PHYADDR	0xE09
#define	ESC_ADDR_TI_PDI_ISR_PINSEL	0xE0A
#define PDI_ISR_EDIO_NUM            7
#define	ESC_ADDR_TI_PHY_LINK_POLARITY	0XE0C


#define TLKPHY_PHYCR_REG                (0x19)

#define TLKPHY_AUTOMDIX_ENABLE          (1u<<15)

#define PHY_PHYIDR1_REG                 (0x2)


#define EXT_REG_ADDRESS_ACCESS          0x001F

#define EXT_REG_DATA_NORMAL_ACCESS      0x401F



//TLK PHY registers
#define TLKPHY_REGCR_REG                0x000D
#define TLKPHY_ADDR_REG                 0x000E
#define TLK105_EXT_MLEDCR_REG           (0x0025)
#define MDIO_PHY_SOFTRESET	            0x8000
#define MDIO_PHY_RESTART	            0x4000
#define TLKPHY_CR2_REG                  0xA
#define TLKPHY_LEDCR_REG                0x18
#define TLKPHY_CR3_REG                  0xB
#define TLKPHY_CR1_REG                  0x9

#define TLK105_PHY0 0
#define TLK105_PHY1 1

#define MDIO_CTRL_PREAMBLE_SHIFT                                                   (20U)
#define MDIO_CTRL_PREAMBLE_MASK                                                    (0x00100000U)
#define MDIO_CTRL_PREAMBLE_EN_0X0                                                   (0U)
#define MDIO_CTRL_PREAMBLE_EN_0X1                                                   (1U)
#define MDIO_CTRL_FAULTENB_MASK                                                    (0x00040000U)
       
#define TIESC_MDIO_CLKDIV   79 //For 2.5MHz MDIO clock: 200/(TIESC_MDIO_CLKDIV+1)

#define GPIO_BANK4_PIN20 	0x94

/* debug macro */

#undef PRUICSS_MDIO_DEBUG
#ifdef PRUICSS_MDIO_DEBUG
#include <private/kwriteLibP.h> /* _func_kprintf */

#define PRUICSS_MDIO_DBG_OFF             0x00000000
#define PRUICSS_MDIO_DBG_WARN            0x00000001
#define PRUICSS_MDIO_DBG_ERR             0x00000002
#define PRUICSS_MDIO_DBG_INFO            0x00000004
#define PRUICSS_MDIO_DBG_ALL             0xffffffff
LOCAL UINT32 dbgMask      = PRUICSS_MDIO_DBG_ALL;

#undef PRUICSS_MDIO_DBG_MSG

#define PRUICSS_MDIO_DBG_MSG(mask,...)                                      \
do                                                             \
{                                                              \
    if ((dbgMask & (mask)) || ((mask) == PRUICSS_MDIO_DBG_ALL))             \
        {                                                      \
        if (_func_kprintf != NULL)                             \
            {                                                  \
            (* _func_kprintf)(__VA_ARGS__);                    \
            }                                                  \
        }                                                      \
}while (0)

#define PRUICSS_MDIO_DBG_MSG(fmt, args...)                       \
            do       {                                        \
                        if (_func_kprintf != NULL)            \
                            _func_kprintf (fmt, ##args);      \
                        } while (FALSE)
                        

#else
#define PRUICSS_MDIO_DBG_MSG(...)
#endif  /* PRUICSS_MDIO_DEBUG */



typedef struct mdio_drv_ctrl
    {
    VXB_DEV_ID	pDev;
    SEM_ID      fmMiiSem;
    UINT32      clkFreq;
    VXB_RESOURCE     * pRes;
    void *      handle;
    void *      regBase;
    VXB_RESOURCE     * shdpRes;
    void *      shdhandle;
    void *      shdregBase;
        
    } MDIO_DRV_CTRL;

    LOCAL STATUS            pruicssStringLookup (int                offset,
                                                         char *              propName,
                                                         char *              string);
    
#ifdef __cplusplus
}
#endif

#endif /* __INCTiPrussMdioh */
