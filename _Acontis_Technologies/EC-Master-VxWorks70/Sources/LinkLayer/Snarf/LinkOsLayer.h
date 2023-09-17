/*-----------------------------------------------------------------------------
 * LinkOsLayer.h            header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Andreas Willig
 * Description              os layer for EtherCAT link layer
 *---------------------------------------------------------------------------*/

#ifndef INC_LINK_OS_LAYER
#define INC_LINK_OS_LAYER

#if !(defined ONLY_DEFINE_DEVICEIDS)
/*-INCLUDE-------------------------------------------------------------------*/
#if (defined INSTRUMENT_LL)
#include "LinkOsInstr.h"
#endif
#ifndef INC_ECOS
#include "EcOs.h"
#endif
#ifndef INC_LINK_OS_PLATFORM
#include "LinkOsPlatform.h"
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#if (defined __cplusplus)
extern "C" {
#endif /* __cplusplus */

#ifndef LINKOSLAYER_API
#define LINKOSLAYER_API
#endif

#endif /* ONLY_DEFINE_DEVICEIDS */

/*-DEFINES-------------------------------------------------------------------*/

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 1
#endif

#define LINK_MAX_FRAMEBUF_LEN       1536

/* device IDSs */
#if !defined(LOSAL_DEVICEIDS_DEFINED)
#define LOSAL_DEVICEIDS_DEFINED 1

#define PCI_VENDOR_INTEL                    ((EC_T_WORD)0x8086)

/* Intel PRO-1000 PCI specific definitions */
#define PCI_DEVICE_I82540EM_DESKTOP         ((EC_T_WORD)0x100E)
/*      PCI_DEVICE_I82545EM_COPPER          ((EC_T_WORD)0x100F) */
#define PCI_DEVICE_I82546EB_COPPER_DUAL     ((EC_T_WORD)0x1010)
/*      PCI_DEVICE_I82545EM_FIBER           ((EC_T_WORD)0x1011) */
/*      PCI_DEVICE_I82546EB_FIBER_DUAL      ((EC_T_WORD)0x1012) */
#define PCI_DEVICE_I82541EI_COPPER          ((EC_T_WORD)0x1013)
/*      PCI_DEVICE_I82540EM_MOBILE          ((EC_T_WORD)0x1015) */
/*      PCI_DEVICE_I82540EP_MOBILE          ((EC_T_WORD)0x1016) */
/*      PCI_DEVICE_I82540EP_DESKTOP         ((EC_T_WORD)0x1017) */
/*      PCI_DEVICE_I82541EI_MOBILE          ((EC_T_WORD)0x1018) */
#define PCI_DEVICE_I82547GI_COPPER          ((EC_T_WORD)0x1019)
/*      PCI_DEVICE_I82547EI_MOBILE          ((EC_T_WORD)0x101A) */
/*      PCI_DEVICE_I82546EB_COPPER_QUAD     ((EC_T_WORD)0x101D) */
#define PCI_DEVICE_I82545GM_COPPER          ((EC_T_WORD)0x1026)
/*      PCI_DEVICE_I82545GM_FIBER           ((EC_T_WORD)0x1027) */
/*      PCI_DEVICE_I82545GM_SERDES          ((EC_T_WORD)0x1028) */
#define PCI_DEVICE_I82566MM                 ((EC_T_WORD)0x1049)
#define PCI_DEVICE_I82566DM                 ((EC_T_WORD)0x104A)
#define PCI_DEVICE_I82566MC                 ((EC_T_WORD)0x104D)
#define PCI_DEVICE_N1E5132_SERVER           ((EC_T_WORD)0x105E)
#define PCI_DEVICE_I82547EI                 ((EC_T_WORD)0x1075)
#define PCI_DEVICE_I82541GI_COPPER          ((EC_T_WORD)0x1076)
#define PCI_DEVICE_I82541GI_MOBILE          ((EC_T_WORD)0x1077)
#define PCI_DEVICE_I82541ER                 ((EC_T_WORD)0x1078)
#define PCI_DEVICE_I82546GB_COPPER_DUAL     ((EC_T_WORD)0x1079)
/*      PCI_DEVICE_I82546GB_FIBER_DUAL      ((EC_T_WORD)0x107A) */
/*      PCI_DEVICE_I82546GB_SERDES_DUAL     ((EC_T_WORD)0x107B) */
#define PCI_DEVICE_I82541PI_DESKTOP         ((EC_T_WORD)0x107C)
#define PCI_DEVICE_I82572EI                 ((EC_T_WORD)0x107D)
#define PCI_DEVICE_I82573E                  ((EC_T_WORD)0x108B)
#define PCI_DEVICE_I82573                   ((EC_T_WORD)0x108C)
#define PCI_DEVICE_I82573L                  ((EC_T_WORD)0x109A)
#define PCI_DEVICE_I82571GB_QUAD            ((EC_T_WORD)0x10A4)
#define PCI_DEVICE_I82575_ZOAR              ((EC_T_WORD)0x10A7)
#define PCI_DEVICE_I82572GI                 ((EC_T_WORD)0x10B9)
#define PCI_DEVICE_I82571GB_QUAD_2          ((EC_T_WORD)0x10BC)
#define PCI_DEVICE_I82566L                  ((EC_T_WORD)0x10BD)
#define PCI_DEVICE_I82576                   ((EC_T_WORD)0x10C9)
#define PCI_DEVICE_I82567V                  ((EC_T_WORD)0x10CE)
#define PCI_DEVICE_I82574L                  ((EC_T_WORD)0x10D3)
#define PCI_DEVICE_I82567LM3                ((EC_T_WORD)0x10DE)
#define PCI_DEVICE_I82577LM                 ((EC_T_WORD)0x10EA)
#define PCI_DEVICE_I82577LC                 ((EC_T_WORD)0x10EB)
#define PCI_DEVICE_I82578DM                 ((EC_T_WORD)0x10EF)
#define PCI_DEVICE_I82578DC                 ((EC_T_WORD)0x10F0)
#define PCI_DEVICE_I82567LM                 ((EC_T_WORD)0x10F5)
/*      PCI_DEVICE_I82544EI                 ((EC_T_WORD)0x1107) */
/*      PCI_DEVICE_I82544GC                 ((EC_T_WORD)0x1112) */
#define PCI_DEVICE_I82567V3                 ((EC_T_WORD)0x1501)
#define PCI_DEVICE_I82579LM                 ((EC_T_WORD)0x1502)
#define PCI_DEVICE_I82579V                  ((EC_T_WORD)0x1503)
#define PCI_DEVICE_I82576NS                 ((EC_T_WORD)0x150A)
#define PCI_DEVICE_I82583V                  ((EC_T_WORD)0x150C)
#define PCI_DEVICE_I82580_QUAD              ((EC_T_WORD)0x150E)
#define PCI_DEVICE_I350                     ((EC_T_WORD)0x1521)
#define PCI_DEVICE_I82576_ET2               ((EC_T_WORD)0x1526)
#define PCI_DEVICE_I82580_QUAD_FIBRE        ((EC_T_WORD)0x1527)
#define PCI_DEVICE_I210AT                   ((EC_T_WORD)0x1531)
#define PCI_DEVICE_I210AT_2                 ((EC_T_WORD)0x1532)
#define PCI_DEVICE_I210_COPPER              ((EC_T_WORD)0x1533)
#define PCI_DEVICE_I211AT                   ((EC_T_WORD)0x1539)
#define PCI_DEVICE_I210_COPPER_FLASHLESS    ((EC_T_WORD)0x157B)
#define PCI_DEVICE_I217LM                   ((EC_T_WORD)0x153A)
#define PCI_DEVICE_I217V                    ((EC_T_WORD)0x153B)
#define PCI_DEVICE_I218LM                   ((EC_T_WORD)0x155A)
#define PCI_DEVICE_I218V                    ((EC_T_WORD)0x1559)
#define PCI_DEVICE_I218V_2                  ((EC_T_WORD)0x15A1)
#define PCI_DEVICE_I218V_3                  ((EC_T_WORD)0x15A3)

#define PCI_DEVICE_I219LM                   ((EC_T_WORD)0x156F)
#define PCI_DEVICE_I219LM_2                 ((EC_T_WORD)0x15B7)
#define PCI_DEVICE_I219LM_3                 ((EC_T_WORD)0x15B9)
#define PCI_DEVICE_I219LM_4                 ((EC_T_WORD)0x15D7)
#define PCI_DEVICE_I219LM_5                 ((EC_T_WORD)0x15E3)
#define PCI_DEVICE_I219LM_6                 ((EC_T_WORD)0x15BD)
#define PCI_DEVICE_I219LM_7                 ((EC_T_WORD)0x15BB)
#define PCI_DEVICE_I219LM_8                 ((EC_T_WORD)0x15DF)
#define PCI_DEVICE_I219LM_9                 ((EC_T_WORD)0x15E1)

#define PCI_DEVICE_I219V                    ((EC_T_WORD)0x1570)
#define PCI_DEVICE_I219V_2                  ((EC_T_WORD)0x15B8)
#define PCI_DEVICE_I219V_4                  ((EC_T_WORD)0x15D8)
#define PCI_DEVICE_I219V_5                  ((EC_T_WORD)0x15D6)
#define PCI_DEVICE_I219V_6                  ((EC_T_WORD)0x15BE)
#define PCI_DEVICE_I219V_7                  ((EC_T_WORD)0x15BC)
#define PCI_DEVICE_I219V_8                  ((EC_T_WORD)0x15E0)
#define PCI_DEVICE_I219V_9                  ((EC_T_WORD)0x15E2)

/* Intel PRO-100 specific definitions */

#define I82801DB_DEVICE_ID                  ((EC_T_WORD)0x103a) /* PCI device ID, Intel 82801DB(M) (ICH4) LAN Controller */
#define I8255X_DEVICE_ID                    ((EC_T_WORD)0x1229) /* 82557 device ID */
#define I8255X_ER_DEVICE_ID                 ((EC_T_WORD)0x1209) /* 82557 ER device ID */
#define I8255X_VE_DEVICE_ID                 ((EC_T_WORD)0x1050) /* PRO/100 VE device ID */
#define I82562_VM_DEVICE_ID                 ((EC_T_WORD)0x1039) /* 82562 VE/VM device ID */
#define I82559_ER_DEVICE_ID                 ((EC_T_WORD)0x2449) /* 82559 ER device ID */
#define I8255X_VE2_DEVICE_ID                ((EC_T_WORD)0x27DC) /* PRO/100 VE device ID */
#define I82551_QM_DEVICE_ID                 ((EC_T_WORD)0x1059) /* Mobile version of 1229 */
#define I8255X_VE3_DEVICE_ID                ((EC_T_WORD)0x1092) /* PRO/100 VE device ID */


/* Intel EG20T specific definitions */

#define EG20T_DEVICE_ID                      ((EC_T_WORD)0x8802) /* Intel Platform Controller Hub EG20T Gigabit Ethernet MAC */

/* RealTek RTL8139 specific Definitions */

#define PCI_VENDOR_REALTEK                  ((EC_T_WORD)0x10EC)
#define PCI_VENDOR_DLINK                    ((EC_T_WORD)0x1186)

#define RTL8139_D_DEVICE_ID                 ((EC_T_WORD)0x8139) /* RealTek RTL8139 */
#define RTL8139_D_DEVICE_ID_DL              ((EC_T_WORD)0x1300) /* D-Link RTL8139 */


/* RealTek RTL8169 specific Definitions */

#define RTL8169_DEVICE_ID                   ((EC_T_WORD)0x8169)  /* RTL8169 (PCI) */
#define RTL8168_DEVICE_ID                   ((EC_T_WORD)0x8168)  /* RTL8168 (PCIe) */
#define RTL8169_SC_DEVICE_ID                ((EC_T_WORD)0x8167)  /* RTL8169SC */
#define RTL8169_DEVICE_ID_DL                ((EC_T_WORD)0x4300)  /* D-Link RTL8169 */
#define RTL8103_DEVICE_ID                   ((EC_T_WORD)0x8136)  /* RTL8103 (PCIe 100Mbit) */

/* Beckhoff CCAT specific Definitions */
#define PCI_VENDOR_BECKHOFF                 ((EC_T_WORD)0x15EC)

#define CCAT_DEVICE_ID                      ((EC_T_WORD)0x5000)

/* Intel FPGA Triple-Speed Ethernet IP Core */
#define PCI_VENDOR_ALTERATSE                ((EC_T_WORD)0x1172)
#define ALTERATSE_DEVICE_ID                 ((EC_T_WORD)0xDC10)


/* RDC */
#define PCI_VENDOR_RDC                      ((EC_T_WORD)0x17F3)

#define R6040_DEVICE_ID                     ((EC_T_WORD)0x6040)

/* internal device codes */
#if !(defined DEVICEID_UNKNOWN)
#define DEVICEID_UNKNOWN                    ((EC_T_WORD)0x0000)
#endif

/* I825-MAC Generation 4 - PCI, 1 Gbit controllers */
#define DEVICEID_I82540                     ((EC_T_WORD)0x1000) /* Family ID */
#define DEVICEID_I82541                     ((EC_T_WORD)0x1001)
#define DEVICEID_I82544                     ((EC_T_WORD)0x1004)
#define DEVICEID_I82545                     ((EC_T_WORD)0x1005)
#define DEVICEID_I82546                     ((EC_T_WORD)0x1006)
#define DEVICEID_I82547                     ((EC_T_WORD)0x1007)
/* I825-MAC Generation 6 - PCIe, ICH8 / ICH9 / ICH10, 1 Gbit controllers */
#define DEVICEID_I82560                     ((EC_T_WORD)0x1020) /* Family ID */
#define DEVICEID_I82566                     ((EC_T_WORD)0x1026) /* ICH8 / ICH9 */
#define DEVICEID_I82567                     ((EC_T_WORD)0x1027) /* ICH9 / ICH10 */
/* I825-MAC Generation 7 - PCIe, 1 Gbit controllers */
#define DEVICEID_I82570                     ((EC_T_WORD)0x1030) /* Family ID */
#define DEVICEID_I82571                     ((EC_T_WORD)0x1031)
#define DEVICEID_I82572                     ((EC_T_WORD)0x1032)
#define DEVICEID_I82573                     ((EC_T_WORD)0x1033)
#define DEVICEID_I82574                     ((EC_T_WORD)0x1034)
#define DEVICEID_I82575                     ((EC_T_WORD)0x1035)
#define DEVICEID_I82576                     ((EC_T_WORD)0x1036)
#define DEVICEID_I82577                     ((EC_T_WORD)0x1037) /* PCH on Intel 5 Series (Core i3,i5,i7, first gen) */
#define DEVICEID_I82579                     ((EC_T_WORD)0x1039) /* PCH on Intel 6 Series (Sandy Bridge, Core i3,i5,i7, 2nd gen) */
/* I825-MAC Generation 8 - PCIe, 1 Gbit controllers */
#define DEVICEID_I82580                     ((EC_T_WORD)0x1040) /* Family ID */
/* MAC "I" Generation - PCIe, 1 Gbit controllers */
#define DEVICEID_IXXX                       ((EC_T_WORD)0x1050) /* Family ID - I2xx, I3xx, ... */
#define DEVICEID_I210                       ((EC_T_WORD)0x1051)
#define DEVICEID_I211                       ((EC_T_WORD)0x1052) /* Low cost variant of I210 */
#define DEVICEID_I217                       ((EC_T_WORD)0x1053)
#define DEVICEID_I218                       ((EC_T_WORD)0x1054)
#define DEVICEID_I219                       ((EC_T_WORD)0x1055)
#define DEVICEID_I350                       ((EC_T_WORD)0x1060)

/* I825-MAC Generation 5 - 100 Mbit controllers */
#define DEVICEID_I8255X                     ((EC_T_WORD)0x2000)


/* Realtek-MAC - 100 Mbit controllers */
#define DEVICEID_RTL8139                    ((EC_T_WORD)0x3000)


/* Realtek-MAC - 100 Mbit / 1 Gbit controllers */
#define DEVICEID_RTL8168                    ((EC_T_WORD)0x4000)
#define DEVICEID_RTL8169                    ((EC_T_WORD)0x4101)
#define DEVICEID_RTL8169_SC                 ((EC_T_WORD)0x4102)
#define DEVICEID_RTL810X                    ((EC_T_WORD)0x4103) /* 100Mbit */

/* Beckhoff CCAT - 100 Mbit controllers */
#define DEVICEID_CCAT                       ((EC_T_WORD)0x5000)

/* Intel EG20T (Atom) */
#define DEVICEID_EG20T                      ((EC_T_WORD)0x6000)

/* RDC R6040 (Vortex86MX) */
#define DEVICEID_R6040                      ((EC_T_WORD)0x7000)

/* identifier tables, based on LOSAL_T_CARD_IDENTIFY structur, first    */
/* entry is a placeholder for user defined card, last entry should be 0 */
#define LOSAL_KNOWN_GEI_CARDS                                                  \
{   /* wVendorId,        wDeviceId,                         wHwId (MAC) */     \
    {  PCI_VENDOR_INTEL, 0xFFFF,                            0               }, \
    {  PCI_VENDOR_INTEL, 0xABB1,                            DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, 0xABB2,                            DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82541EI_COPPER,        DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82541ER,               DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82541GI_COPPER,        DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82541PI_DESKTOP,       DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82547GI_COPPER,        DEVICEID_I82547 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82545GM_COPPER,        DEVICEID_I82545 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566L,                DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82577LM,               DEVICEID_I82577 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82577LC,               DEVICEID_I82577 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82575_ZOAR,            DEVICEID_I82575 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82547EI,               DEVICEID_I82547 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82573,                 DEVICEID_I82573 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82573L,                DEVICEID_I82573 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82572GI,               DEVICEID_I82572 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82571GB_QUAD,          DEVICEID_I82571 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82574L,                DEVICEID_I82574 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82546EB_COPPER_DUAL,   DEVICEID_I82546 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82546GB_COPPER_DUAL,   DEVICEID_I82546 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82540EM_DESKTOP,       DEVICEID_I82540 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_N1E5132_SERVER,         DEVICEID_I82541 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82571GB_QUAD_2,        DEVICEID_I82571 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82576,                 DEVICEID_I82576 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82567V,                DEVICEID_I82567 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82580_QUAD,            DEVICEID_I82580 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82567LM,               DEVICEID_I82567 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82567LM3,              DEVICEID_I82567 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82580_QUAD_FIBRE,      DEVICEID_I82580 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82567V3,               DEVICEID_I82567 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82573E,                DEVICEID_I82573 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82572EI,               DEVICEID_I82572 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82583V,                DEVICEID_I82580 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82579LM,               DEVICEID_I82579 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82579V,                DEVICEID_I82579 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82576_ET2,             DEVICEID_I82576 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I350,                   DEVICEID_I350   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I210_COPPER,            DEVICEID_I210   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566DM,               DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I211AT,                 DEVICEID_I211   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566MC,               DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82578DM,               DEVICEID_I82577 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82578DC,               DEVICEID_I82577 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I210_COPPER_FLASHLESS,  DEVICEID_I210   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I217LM,                 DEVICEID_I217   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I217V,                  DEVICEID_I217   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I218LM,                 DEVICEID_I218   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I218V,                  DEVICEID_I218   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I218V_2,                DEVICEID_I218   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I218V_3,                DEVICEID_I218   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566MM,               DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM,                 DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_2,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_3,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_4,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_5,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82576NS,               DEVICEID_I82576 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V,                  DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_2,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_4,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I210AT,                 DEVICEID_I210   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I210AT_2,               DEVICEID_I210   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_6,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_7,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_8,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219LM_9,               DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_5,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_6,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_7,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_8,                DEVICEID_I219   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I219V_9,                DEVICEID_I219   }, \
    {  0,                0,                                 0               }, \
}

#define LOSAL_KNOWN_FEI_CARDS                                                  \
{   /* wVendorId,        wDeviceId,                         wHwId */           \
    {  PCI_VENDOR_INTEL, 0xFFFF,                            0               }, \
    {  PCI_VENDOR_INTEL, I82801DB_DEVICE_ID,                DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I8255X_DEVICE_ID,                  DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I8255X_ER_DEVICE_ID,               DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I8255X_VE_DEVICE_ID,               DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I82562_VM_DEVICE_ID,               DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I82559_ER_DEVICE_ID,               DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I8255X_VE2_DEVICE_ID,              DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I82551_QM_DEVICE_ID,               DEVICEID_I8255X }, \
    {  PCI_VENDOR_INTEL, I8255X_VE3_DEVICE_ID,              DEVICEID_I8255X }, \
    {  0,                0,                                 0               }, \
}

#define LOSAL_KNOWN_RTL_CARDS                                                  \
{   /* wVendorId,           wDeviceId,                      wHwId */           \
    {  PCI_VENDOR_REALTEK,  0xFFFF,                         0               }, \
    {  PCI_VENDOR_REALTEK,  RTL8139_D_DEVICE_ID,            DEVICEID_RTL8139}, \
    {  PCI_VENDOR_DLINK,    RTL8139_D_DEVICE_ID_DL,         DEVICEID_RTL8139}, \
    {  0,                0,                                 0               }, \
}

#define LOSAL_KNOWN_RTL8169_CARDS                                              \
{   /* wVendorId,           wDeviceId,                      wHwId */           \
    {  PCI_VENDOR_REALTEK,  0xFFFF,                         0                },\
    {  PCI_VENDOR_REALTEK,  RTL8169_DEVICE_ID,              DEVICEID_RTL8169 },\
    {  PCI_VENDOR_REALTEK,  RTL8168_DEVICE_ID,              DEVICEID_RTL8168 },\
    {  PCI_VENDOR_DLINK,    RTL8169_DEVICE_ID_DL,           DEVICEID_RTL8169 },\
    {  PCI_VENDOR_REALTEK,  RTL8169_SC_DEVICE_ID,           DEVICEID_RTL8169 },\
    {  PCI_VENDOR_REALTEK,  RTL8103_DEVICE_ID,              DEVICEID_RTL810X },\
    {  0,                   0,                              0                },\
}

#define LOSAL_KNOWN_CCAT_CARDS                                                 \
{   /* wVendorId,           wDeviceId,                      wHwId */           \
    {  PCI_VENDOR_BECKHOFF, 0xFFFF,                         0                },\
    {  PCI_VENDOR_BECKHOFF, CCAT_DEVICE_ID,                 DEVICEID_CCAT    },\
    {  0,                   0,                              0                },\
}

#define LOSAL_KNOWN_EG20T_CARDS                                                \
{   /* wVendorId,           wDeviceId,                      wHwId */           \
    {  PCI_VENDOR_INTEL,    0xFFFF,                         0                },\
    {  PCI_VENDOR_INTEL,    EG20T_DEVICE_ID,                DEVICEID_EG20T   },\
    {  0,                   0,                              0                },\
}

#define LOSAL_KNOWN_R6040_CARDS                                                \
{   /* wVendorId,           wDeviceId,                      wHwId */           \
    {  PCI_VENDOR_RDC,      0xFFFF,                         0                },\
    {  PCI_VENDOR_RDC,      R6040_DEVICE_ID,                DEVICEID_R6040   },\
    {  0,                   0,                              0                },\
}

#define LOSAL_KNOWN_ALTERATSE_CARDS                                              \
{   /* wVendorId,           wDeviceId,                      wHwId */             \
    {  PCI_VENDOR_ALTERATSE,   0xFFFF,                      0                  },\
    {  PCI_VENDOR_ALTERATSE,   ALTERATSE_DEVICE_ID,         0                  },\
    {  0,                   0,                              0                  },\
}

#define NUM_INIC_DEVICES   10

#endif /* !defined(LOSAL_DEVICEIDS_DEFINED) */

#if !(defined ONLY_DEFINE_DEVICEIDS)

/*-MACROS--------------------------------------------------------------------*/

/* data access */

#define SYS_ERRORMSG(Msg)  LinkOsDbgMsg Msg; LinkOsDbgAssert(EC_FALSE)   /* print an error message and jump into debugger */
#define SYS_ERRORMSGC(Msg) LinkOsDbgMsg Msg                              /* print an error message and continue */

#define ECLINKOS_UNREFPARM(p)     {(p)=(p);}


/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
#if !defined(LOSAL_TYPE_DEFINED)
#define LOSAL_TYPE_DEFINED 1
typedef struct _LOSAL_T_CARD_IDENTIFY
{
    EC_T_WORD wVendorId;
    EC_T_WORD wDeviceId;
    EC_T_WORD wHWId;
} LOSAL_T_CARD_IDENTIFY;
#endif /* !defined(LOSAL_TYPE_DEFINED) */

typedef EC_T_VOID (*EC_PF_LINKTHREADENTRY)(EC_T_VOID* pvParams);

typedef EC_T_BOOL (*EC_PF_LINKOSDBGMSGHK)(const EC_T_CHAR* szFormat, EC_T_LINKVALIST vaArgs);

/* SMP support */
#ifndef EC_LINKCPUSET_DEFINED
#define EC_LINKCPUSET_DEFINED	1
typedef unsigned long   EC_T_LINKCPUSET;        /* CPU-set for SMP systems */
#endif

/* IRQ support */
typedef EC_T_DWORD (*EC_T_PFLINKOSIRQFUNC)( EC_T_VOID* pvContext);

typedef enum
{
   INTSOURCE_PCI, /* PCI interrupt (default) */
   INTSOURCE_INTERNAL /* Internal interrupt (i.e. INT's in SoC devices like eTSEC) */
} EC_T_INTSOURCE;

typedef EC_T_VOID(*PF_emllISRCallback)(EC_T_VOID* pvInstance, EC_T_VOID* pvContext);

typedef struct _EC_T_LINKOS_IRQ_PARM
{
    EC_T_VOID*              pvAdapter;          /* Adapter handle */
    EC_T_DWORD              dwSysIrq;           /* OS SysIRQ number */

    EC_T_PFLINKOSIRQFUNC    pfIrqOpen;          /* Activate Card IRQs */
    EC_T_PFLINKOSIRQFUNC    pfIrqAckAll;        /* Acknowledge all pending card IRQs */
    EC_T_PFLINKOSIRQFUNC    pfIrqClose;         /* Mask all Card IRQs */

    EC_T_PFLINKOSIRQFUNC    pfIstFunction;      /* Function which is called by IST (not called if ISR is active)
                                                 * when using private ISR, IST is NOT started implicit !!! */
    EC_T_DWORD              dwIstPriority;      /* IST Thread Priority */
    EC_T_DWORD              dwIstStackSize;     /* IST Thread Stack size */

    PF_emllISRCallback      pfIsrFunction;      /* Function called during ISR, if externally provided, IST task
                                                 * is NOT spawned */
    EC_T_VOID*              pvIsrFuncCtxt;      /* Context for ISR callback */
    EC_T_VOID*              pvLOSALContext;     /* pointer for LinkOS internal handling */
    EC_T_DWORD              dwLosalHandle;
   
    EC_T_INTSOURCE          eIntSource;         /* Interrupt source (PCI, non-PCI) */

    T_PCIINFO*              pPciInfo;
} EC_T_LINKOS_IRQ_PARM;


/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-GLOBAL VARIABLES-----------------------------------------------------------*/
extern LOSAL_T_CARD_IDENTIFY G_oKnownGeiCards[];


/*-MACROS/INLINES------------------------------------------------------------*/
static EC_INLINESTART EC_T_VOID* EcPtrAlignDown(EC_T_VOID* pvVal, EC_T_DWORD dwAlignment)
	{ return (EC_T_VOID*)((EC_T_BYTE*)pvVal - (((EC_T_BYTE*)pvVal - (EC_T_BYTE*)0) % (unsigned)dwAlignment)); } EC_INLINESTOP

static EC_INLINESTART EC_T_VOID* EcPtrAlignUp(EC_T_VOID* pvVal, EC_T_DWORD dwAlignment)
	{ return (EC_T_VOID*)((EC_T_BYTE*)EcPtrAlignDown(pvVal, dwAlignment) + (unsigned)dwAlignment); } EC_INLINESTOP

static EC_INLINESTART EC_T_DWORD EcSizeAlignDown(EC_T_DWORD dwVal, EC_T_DWORD dwAlignment)
	{ return ((dwVal)&(~((dwAlignment)-1))); } EC_INLINESTOP

static EC_INLINESTART EC_T_DWORD EcSizeAlignUp(EC_T_DWORD dwVal, EC_T_DWORD dwAlignment)
	{ return (((dwVal)+((dwAlignment)-1))&(~((dwAlignment)-1))); } EC_INLINESTOP

/*-FUNCTION DECLARATION------------------------------------------------------*/

#ifndef LinkOsOpen
#define LinkOsOpen() 0
#endif

#ifndef LinkOsClose
#define LinkOsClose() (void)0
#endif


#ifndef LinkOsCreateContext
#define LinkOsCreateContext(x) (EC_T_PVOID)EC_NULL
#endif

#ifndef LinkOsReleaseContext
#define LinkOsReleaseContext(x)
#endif

#ifndef LinkOsCacheDataSync
#define LinkOsCacheDataSync(va, pa, dwSize)
#endif
#ifndef LinkOsCacheDataInvalidateBuff
#define LinkOsCacheDataInvalidateBuff(va, pa, dwSize)
#endif
#ifndef LinkOsCacheFlushPipe
#define LinkOsCacheFlushPipe()
#endif

#ifndef LinkOsMalloc
LINKOSLAYER_API  EC_T_VOID*  LinkOsMalloc(EC_T_INT nSize);
#endif
#ifndef LinkOsRealloc
LINKOSLAYER_API  EC_T_VOID*  LinkOsRealloc(EC_T_VOID* pvMem, EC_T_INT nSize);
#endif
#ifndef LinkOsFree
LINKOSLAYER_API  EC_T_VOID   LinkOsFree(EC_T_VOID* pvMem);
#endif

#ifndef LinkOsSafeDelete
#define LinkOsSafeDelete(p)  {if (p) {delete(p); (p) = EC_NULL;}}
#endif

#ifndef LinkOsMemset
LINKOSLAYER_API  EC_T_VOID*  LinkOsMemset(EC_T_VOID* pvMem, EC_T_INT nVal, EC_T_INT nSize);
#endif
#ifndef LinkOsMemcpy
LINKOSLAYER_API  EC_T_VOID*  LinkOsMemcpy(EC_T_VOID* pvDst, const EC_T_VOID* pvSrc, EC_T_INT nSize);
#endif
#ifndef LinkOsMemcmp
LINKOSLAYER_API  EC_T_INT    LinkOsMemcmp(const EC_T_VOID* pvMem1, const EC_T_VOID* pvMem2, EC_T_INT nSize);
#endif
#ifndef LinkOsMemmove
LINKOSLAYER_API  EC_T_VOID*  LinkOsMemmove(EC_T_VOID* pvDst, const EC_T_VOID* pvSrc, EC_T_INT nSize);
#endif

#ifndef LinkOsStrlen
LINKOSLAYER_API  EC_T_INT    LinkOsStrlen(const EC_T_CHAR* szString);
#endif

#ifndef LinkOsStrcmp
LINKOSLAYER_API  EC_T_INT    LinkOsStrcmp(const EC_T_CHAR* szString1, const EC_T_CHAR* szString2);
#endif

LINKOSLAYER_API  EC_T_VOID*  LinkOsCreateEvent(EC_T_VOID);
LINKOSLAYER_API  EC_T_VOID   LinkOsDeleteEvent(EC_T_VOID* pvEvent);
#ifndef LinkOsWaitForEvent
LINKOSLAYER_API  EC_T_DWORD  LinkOsWaitForEvent(EC_T_VOID* pvEvent, EC_T_DWORD dwTimeout);
#endif
#ifndef LinkOsSetEvent
LINKOSLAYER_API  EC_T_VOID   LinkOsSetEvent(EC_T_VOID* pvEvent);
#endif
/*
#ifndef LinkOsResetEvent
LINKOSLAYER_API  EC_T_VOID   LinkOsResetEvent(EC_T_VOID* pvEvent);
#endif
*/

LINKOSLAYER_API  EC_T_VOID*  LinkOsCreateLock(EC_T_OS_LOCK_TYPE   eLockType);
LINKOSLAYER_API  EC_T_VOID   LinkOsDeleteLock(EC_T_VOID* pvLock);
#ifndef LinkOsLock
LINKOSLAYER_API  EC_T_VOID   LinkOsLock(EC_T_VOID* pvLock);
#endif
#ifndef LinkOsUnlock
LINKOSLAYER_API  EC_T_VOID   LinkOsUnlock(EC_T_VOID* pvLock);
#endif
#ifndef OsLock
#define OsLock LinkOsLock 
#endif
#ifndef OsUnlock
#define OsUnlock LinkOsUnlock 
#endif

LINKOSLAYER_API  EC_T_VOID*  LinkOsCreateThread(EC_T_CHAR* szThreadName, EC_PF_LINKTHREADENTRY pfThreadEntry, EC_T_DWORD dwPrio, EC_T_DWORD dwStackSize, EC_T_VOID* pvParams );
LINKOSLAYER_API  EC_T_VOID   LinkOsDeleteThreadHandle(EC_T_VOID* pvThreadObject);
LINKOSLAYER_API  EC_T_VOID   LinkOsSetThreadPriority( EC_T_VOID* pvThreadObject, EC_T_DWORD dwPrio );
LINKOSLAYER_API  EC_T_BOOL   LinkOsSetThreadAffinity( EC_T_VOID* pvThreadObject, EC_T_LINKCPUSET CpuSet );
LINKOSLAYER_API  EC_T_BOOL   LinkOsGetThreadAffinity( EC_T_VOID* pvThreadObject, EC_T_LINKCPUSET* pCpuSet );

#ifndef LinkOsSleep
LINKOSLAYER_API  EC_T_VOID   LinkOsSleep(EC_T_DWORD dwMsec);
#endif

#ifndef LinkOsDbgMsg
LINKOSLAYER_API  EC_T_VOID   LinkOsDbgMsg(const EC_T_CHAR* szFormat, ...);
#endif

#ifndef LinkOsDbgAssert
LINKOSLAYER_API  EC_T_VOID   LinkOsDbgAssert(EC_T_BOOL bAssertCondition);
#endif

#ifndef LinkOsAddDbgMsgHook
LINKOSLAYER_API  EC_T_VOID   LinkOsAddDbgMsgHook(EC_PF_LINKOSDBGMSGHK pfOsDbgMsgHook);
#endif

EC_T_VOID   LinkOsSysDelay (            EC_T_DWORD  dwTimeUsec      );

#ifndef LinkOsAllocDmaBuffer
EC_T_VOID   LinkOsAllocDmaBuffer(       EC_T_VOID*  pvDmaObject
                                       ,EC_T_BYTE** ppbyVirtAddrCached
                                       ,EC_T_BYTE** ppbyVirtAddrUncached
                                       ,EC_T_BYTE** ppbyPhysAddr
                                       ,EC_T_DWORD  dwSize          );
#endif

#ifndef LinkOsFreeDmaBuffer
EC_T_VOID   LinkOsFreeDmaBuffer(        EC_T_VOID*  pvDmaObject
                                       ,EC_T_BYTE*  pbyPhysAddr
                                       ,EC_T_BYTE*  pbyVirtAddrCached
                                       ,EC_T_BYTE*  pbyVirtAddrUncached
                                       ,EC_T_DWORD  dwSize          );
#endif

EC_T_BOOL   LinkOsGetPciInfo(           T_PCIINFO*         pInfo
                                       ,const EC_T_CHAR*   szDeviceName
                                       ,EC_T_DWORD         dwUnit   );

#ifndef LinkOsPciConfigure
EC_T_BOOL   LinkOsPciConfigure(         EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,T_PCIINFO*         pInfo);
#endif
#ifndef LinkOsPciDeconfigure
EC_T_BOOL   LinkOsPciDeconfigure(       T_PCIINFO*         pInfo);
#endif

EC_T_VOID   LinkOsPciInit(EC_T_VOID);
EC_T_VOID   LinkOsPciCleanup(EC_T_VOID);
EC_T_BOOL   LinkOsPciFindDev(           EC_T_DWORD         dwVendorID
                                       ,EC_T_DWORD         dwDeviceID
                                       ,EC_T_DWORD         dwIdx
                                       ,EC_T_DWORD*        pdwBusNum
                                       ,EC_T_DWORD*        pdwDevNum
                                       ,EC_T_DWORD*        pdwFuncNum);
EC_T_BOOL   LinkOsPciRead8(             EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,EC_T_VOID*         pvBuf     );
EC_T_BOOL   LinkOsPciRead16(            EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,EC_T_VOID*         pvBuf     );
EC_T_BOOL   LinkOsPciRead32(            EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,EC_T_VOID*         pvBuf     );
EC_T_BOOL   LinkOsPciWrite8(            EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,const EC_T_VOID*   pvBuf     );
EC_T_BOOL   LinkOsPciWrite16(           EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,const EC_T_VOID*   pvBuf     );
EC_T_BOOL   LinkOsPciWrite32(           EC_T_DWORD         dwBusNum
                                       ,EC_T_DWORD         dwDevNum
                                       ,EC_T_DWORD         dwFunNum
                                       ,EC_T_DWORD         dwRegOffs
                                       ,const EC_T_VOID*   pvBuf     );

#ifndef LinkOsMapMemory
EC_T_BYTE*  LinkOsMapMemory(            EC_T_BYTE*  pbyPhysical
                                       ,EC_T_DWORD  dwLen           );
#endif

#ifndef LinkOsPlatformInit
EC_T_VOID LinkOsPlatformInit();
#endif

#ifndef LinkOsUnmapMemory
EC_T_VOID   LinkOsUnmapMemory(          EC_T_BYTE*  pbyVirtual
                                       ,EC_T_DWORD  dwLen           );
#endif

#ifndef LinkOsInterruptInitialize
EC_T_BOOL LinkOsInterruptInitialize( EC_T_LINKOS_IRQ_PARM* pInterruptParm );
#endif

#ifndef LinkOsInterruptDone
EC_T_VOID LinkOsInterruptDone(EC_T_LINKOS_IRQ_PARM* pInterruptParm);
#endif

#ifndef LinkOsInterruptDisable
EC_T_VOID LinkOsInterruptDisable(EC_T_LINKOS_IRQ_PARM* pInterruptParm);
#endif

#ifndef LinkOsGetExternal64bitTimer
#define LinkOsGetExternal64bitTimer(x)
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#if (defined __cplusplus)
};
#endif /* __cplusplus */

#endif /* ONLY_DEFINE_DEVICEIDS */

#endif /* INC_LINK_OS_LAYER */

/*-END OF SOURCE FILE--------------------------------------------------------*/
