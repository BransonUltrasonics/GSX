/*-----------------------------------------------------------------------------
 * sysLoSalAdd.c            file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2008/1/30::13:53
 *---------------------------------------------------------------------------*/

#if !defined(EXCLUDE_SYS_LOSAL) /* due to PPC integrated build problem */

/*-INCLUDES------------------------------------------------------------------*/

/*-MACROS-------------------------------------------------------------------*/

/* VxWorks 5.4 doesn't defines these macros */
#ifndef PCI_NET_ETHERNET_CLASS
#define PCI_NET_ETHERNET_CLASS  0x20000
#endif

#ifndef PCI_HEADER_PCI_CARDBUS
#define PCI_HEADER_PCI_CARDBUS  0x02
#endif

#ifndef PCI_MEMIO2LOCAL
#define PCI_MEMIO2LOCAL(x) (x)
#endif

#ifndef INT_NUM_GET
#ifdef  INT_VEC_GET
#   define INT_NUM_GET(irq) INT_VEC_GET(irq)
#else
#   define INT_NUM_GET(irq) (irq)
#endif
#endif

/*-DEFINES------------------------------------------------------------------*/

/* Entries in PCI map (Mapped PCI devices) */
#define BUSMAP_CNT  64

/* device IDSs */
#if !defined(LOSAL_DEVICEIDS_DEFINED)
#define LOSAL_DEVICEIDS_DEFINED 1

#define PCI_VENDOR_INTEL                    ((UINT16)0x8086)

/* Intel PRO-1000 PCI specific definitions */
#define PCI_DEVICE_I82540EM_DESKTOP         ((UINT16)0x100E)
/*      PCI_DEVICE_I82545EM_COPPER          ((UINT16)0x100F) */
#define PCI_DEVICE_I82546EB_COPPER_DUAL     ((UINT16)0x1010)
/*      PCI_DEVICE_I82545EM_FIBER           ((UINT16)0x1011) */
/*      PCI_DEVICE_I82546EB_FIBER_DUAL      ((UINT16)0x1012) */
#define PCI_DEVICE_I82541EI_COPPER          ((UINT16)0x1013)
/*      PCI_DEVICE_I82540EM_MOBILE          ((UINT16)0x1015) */
/*      PCI_DEVICE_I82540EP_MOBILE          ((UINT16)0x1016) */
/*      PCI_DEVICE_I82540EP_DESKTOP         ((UINT16)0x1017) */
/*      PCI_DEVICE_I82541EI_MOBILE          ((UINT16)0x1018) */
#define PCI_DEVICE_I82547GI_COPPER          ((UINT16)0x1019)
/*      PCI_DEVICE_I82547EI_MOBILE          ((UINT16)0x101A) */
/*      PCI_DEVICE_I82546EB_COPPER_QUAD     ((UINT16)0x101D) */
#define PCI_DEVICE_I82545GM_COPPER          ((UINT16)0x1026)
/*      PCI_DEVICE_I82545GM_FIBER           ((UINT16)0x1027) */
/*      PCI_DEVICE_I82545GM_SERDES          ((UINT16)0x1028) */
#define PCI_DEVICE_I82566DM                 ((UINT16)0x104A)
#define PCI_DEVICE_I82566MC                 ((UINT16)0x104D)
#define PCI_DEVICE_N1E5132_SERVER           ((UINT16)0x105E)
#define PCI_DEVICE_I82547EI                 ((UINT16)0x1075)
#define PCI_DEVICE_I82541GI_COPPER          ((UINT16)0x1076)
#define PCI_DEVICE_I82541GI_MOBILE          ((UINT16)0x1077)
#define PCI_DEVICE_I82541ER                 ((UINT16)0x1078)
#define PCI_DEVICE_I82546GB_COPPER_DUAL     ((UINT16)0x1079)
/*      PCI_DEVICE_I82546GB_FIBER_DUAL      ((UINT16)0x107A) */
/*      PCI_DEVICE_I82546GB_SERDES_DUAL     ((UINT16)0x107B) */
#define PCI_DEVICE_I82541PI_DESKTOP         ((UINT16)0x107C)
#define PCI_DEVICE_I82572EI                 ((UINT16)0x107D)
#define PCI_DEVICE_I82573E                  ((UINT16)0x108B)
#define PCI_DEVICE_I82573                   ((UINT16)0x108C)
#define PCI_DEVICE_I82573L                  ((UINT16)0x109A)
#define PCI_DEVICE_I82571GB_QUAD            ((UINT16)0x10A4)
#define PCI_DEVICE_I82575_ZOAR              ((UINT16)0x10A7)
#define PCI_DEVICE_I82572GI                 ((UINT16)0x10B9)
#define PCI_DEVICE_I82571GB_QUAD_2          ((UINT16)0x10BC)
#define PCI_DEVICE_I82566L                  ((UINT16)0x10BD)
#define PCI_DEVICE_I82576                   ((UINT16)0x10C9)
#define PCI_DEVICE_I82567V                  ((UINT16)0x10CE)
#define PCI_DEVICE_I82574L                  ((UINT16)0x10D3)
#define PCI_DEVICE_I82567LM3                ((UINT16)0x10DE)
#define PCI_DEVICE_I82577LM                 ((UINT16)0x10EA)
#define PCI_DEVICE_I82577LC                 ((UINT16)0x10EB)
#define PCI_DEVICE_I82578DM                 ((UINT16)0x10EF)
#define PCI_DEVICE_I82578DC                 ((UINT16)0x10F0)
#define PCI_DEVICE_I82567LM                 ((UINT16)0x10F5)
/*      PCI_DEVICE_I82544EI                 ((UINT16)0x1107) */
/*      PCI_DEVICE_I82544GC                 ((UINT16)0x1112) */
#define PCI_DEVICE_I82567V3                 ((UINT16)0x1501)
#define PCI_DEVICE_I82579LM                 ((UINT16)0x1502)
#define PCI_DEVICE_I82579V                  ((UINT16)0x1503)
#define PCI_DEVICE_I82583V                  ((UINT16)0x150C)
#define PCI_DEVICE_I82580_QUAD              ((UINT16)0x150E)
#define PCI_DEVICE_I350                     ((UINT16)0x1521)
#define PCI_DEVICE_I82576_ET2               ((UINT16)0x1526)
#define PCI_DEVICE_I82580_QUAD_FIBRE        ((UINT16)0x1527)
#define PCI_DEVICE_I210                     ((UINT16)0x1533)
#define PCI_DEVICE_I211AT                   ((UINT16)0x1539)

/* Intel PRO-100 specific definitions */

#define I82801DB_DEVICE_ID                  ((UINT16)0x103a) /* PCI device ID, Intel 82801DB(M) (ICH4) LAN Controller */
#define I8255X_DEVICE_ID                    ((UINT16)0x1229) /* 82557 device ID */
#define I8255X_ER_DEVICE_ID                 ((UINT16)0x1209) /* 82557 ER device ID */
#define I8255X_VE_DEVICE_ID                 ((UINT16)0x1050) /* PRO/100 VE device ID */
#define I82562_VM_DEVICE_ID                 ((UINT16)0x1039) /* 82562 VE/VM device ID */
#define I82559_ER_DEVICE_ID                 ((UINT16)0x2449) /* 82559 ER device ID */
#define I8255X_VE2_DEVICE_ID                ((UINT16)0x27DC) /* PRO/100 VE device ID */
#define I82551_QM_DEVICE_ID                 ((UINT16)0x1059) /* Mobile version of 1229 */
#define I8255X_VE3_DEVICE_ID                ((UINT16)0x1092) /* PRO/100 VE device ID */


/* Intel EG20T specific definitions */

#define EG20T_DEVICE_ID                      ((UINT16)0x8802) /* Intel Platform Controller Hub EG20T—Gigabit Ethernet MAC */

/* RealTek RTL8139 specific Definitions */

#define PCI_VENDOR_REALTEK                  ((UINT16)0x10EC)
#define PCI_VENDOR_DLINK                    ((UINT16)0x1186)

#define RTL8139_D_DEVICE_ID                 ((UINT16)0x8139) /* RealTek RTL8139 */
#define RTL8139_D_DEVICE_ID_DL              ((UINT16)0x1300) /* D-Link RTL8139 */


/* RealTek RTL8169 specific Definitions */

#define RTL8169_DEVICE_ID                   ((UINT16)0x8169)  /* RTL8169 (PCI) */
#define RTL8168_DEVICE_ID                   ((UINT16)0x8168)  /* RTL8168 (PCIe) */
#define RTL8169_SC_DEVICE_ID                ((UINT16)0x8167)  /* RTL8169SC */
#define RTL8169_DEVICE_ID_DL                ((UINT16)0x4300)  /* D-Link RTL8169 */
#define RTL8103_DEVICE_ID                   ((UINT16)0x8136)  /* RTL8103 (PCIe 100Mbit) */

/* Beckhoff CCAT specific Definitions */
#define PCI_VENDOR_BECKHOFF                 ((UINT16)0x15EC)

#define CCAT_DEVICE_ID                      ((UINT16)0x5000)

/* RDC */
#define PCI_VENDOR_RDC                      ((UINT16)0x17F3)

#define R6040_DEVICE_ID                     ((UINT16)0x6040)

/* internal device codes */
#if !(defined DEVICEID_UNKNOWN)
#define DEVICEID_UNKNOWN                    ((UINT16)0x0000)
#endif

/* I825-MAC Generation 4 - PCI, 1 Gbit controllers */
#define DEVICEID_I82540                     ((UINT16)0x1000) /* Family ID */
#define DEVICEID_I82541                     ((UINT16)0x1001)
#define DEVICEID_I82544                     ((UINT16)0x1004)
#define DEVICEID_I82545                     ((UINT16)0x1005)
#define DEVICEID_I82546                     ((UINT16)0x1006)
#define DEVICEID_I82547                     ((UINT16)0x1007)
/* I825-MAC Generation 6 - PCIe, ICH8 / ICH9 / ICH10, 1 Gbit controllers */
#define DEVICEID_I82560                     ((UINT16)0x1020) /* Family ID */
#define DEVICEID_I82566                     ((UINT16)0x1026) /* ICH8 / ICH9 */
#define DEVICEID_I82567                     ((UINT16)0x1027) /* ICH9 / ICH10 */
/* I825-MAC Generation 7 - PCIe, 1 Gbit controllers */
#define DEVICEID_I82570                     ((UINT16)0x1030) /* Family ID */
#define DEVICEID_I82571                     ((UINT16)0x1031)
#define DEVICEID_I82572                     ((UINT16)0x1032)
#define DEVICEID_I82573                     ((UINT16)0x1033)
#define DEVICEID_I82574                     ((UINT16)0x1034)
#define DEVICEID_I82575                     ((UINT16)0x1035)
#define DEVICEID_I82576                     ((UINT16)0x1036)
#define DEVICEID_I82577                     ((UINT16)0x1037) /* PCH on Intel 5 Series (Core i3,i5,i7, first gen) */
#define DEVICEID_I82579                     ((UINT16)0x1039) /* PCH on Intel 6 Series (Sandy Bridge, Core i3,i5,i7, 2nd gen) */
/* I825-MAC Generation 8 - PCIe, 1 Gbit controllers */
#define DEVICEID_I82580                     ((UINT16)0x1040) /* Family ID */
/* MAC "I" Generation - PCIe, 1 Gbit controllers */
#define DEVICEID_IXXX                       ((UINT16)0x1050) /* Family ID - I2xx, I3xx, ... */
#define DEVICEID_I210                       ((UINT16)0x1051)
#define DEVICEID_I211                       ((UINT16)0x1052) /* Low cost variant of I210 */
#define DEVICEID_I350                       ((UINT16)0x1053)


/* I825-MAC Generation 5 - 100 Mbit controllers */
#define DEVICEID_I8255X                     ((UINT16)0x2000)


/* Realtek-MAC - 100 Mbit controllers */
#define DEVICEID_RTL8139                    ((UINT16)0x3000)


/* Realtek-MAC - 100 Mbit / 1 Gbit controllers */
#define DEVICEID_RTL8168                    ((UINT16)0x4000)
#define DEVICEID_RTL8169                    ((UINT16)0x4101)
#define DEVICEID_RTL8169_SC                 ((UINT16)0x4102)
#define DEVICEID_RTL810X                    ((UINT16)0x4103) /* 100Mbit */

/* Beckhoff CCAT - 100 Mbit controllers */
#define DEVICEID_CCAT                       ((UINT16)0x5000)

/* Intel EG20T (Atom) */
#define DEVICEID_EG20T                      ((UINT16)0x6000)

/* RDC R6040 (Vortex86MX) */
#define DEVICEID_R6040                      ((UINT16)0x7000)

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
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I210,                   DEVICEID_I210   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566DM,               DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I211AT,                 DEVICEID_I211   }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82566MC,               DEVICEID_I82566 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82578DM,               DEVICEID_I82577 }, \
    {  PCI_VENDOR_INTEL, PCI_DEVICE_I82578DC,               DEVICEID_I82577 }, \
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

#endif /* !defined(LOSAL_DEVICEIDS_DEFINED) */


/*-TYPEDEFS------------------------------------------------------------------*/
#if !defined(LOSAL_TYPE_DEFINED)
#define LOSAL_TYPE_DEFINED 1
typedef struct _LOSAL_T_CARD_IDENTIFY
{
    UINT16    wVendorId;
    UINT16    wDeviceId;
    UINT16    wHWId;
} LOSAL_T_CARD_IDENTIFY;
#endif /* !defined(LOSAL_TYPE_DEFINED) */

typedef struct _T_PCIDESC
{
   UINT16 wVendorId;
   UINT16 wDeviceId;
   UINT8  byBusNo;
   UINT8  byDeviceNo;
   UINT8  byFunctionNo;
} T_PCIDESC;

typedef STATUS (*SLOSAL_PCIFOREACH_FUNC)(INT32 nBus, INT32 nDevice, INT32 nFunction, void* vpArg);

#ifndef SLOSAL_NOT_INBSP /* BSP Build */

#define SLSL_ROUND_UP(x, align) (((int) (x) + (align - 1)) & ~(align - 1))
#define SLSL_ROUND_DOWN(x, align)  ((int)(x) & ~(align - 1))

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/
static LOSAL_T_CARD_IDENTIFY S_oBSPKnownGeiCards[]     = LOSAL_KNOWN_GEI_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oBSPKnownFeiCards[]     = LOSAL_KNOWN_FEI_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oBSPKnownRtlCards[]     = LOSAL_KNOWN_RTL_CARDS;
static LOSAL_T_CARD_IDENTIFY S_oBSPKnownRtl8169Cards[] = LOSAL_KNOWN_RTL8169_CARDS;

/* This two variables are global for debugging reasons in VxWorks shell. I.e. use:
 * -> S_dwPciBusMapCnt
 *    S_dwPciBusMapCnt = 0x413d63c: value = 2 = 0x2
 * -> d &S_oPciBusMap
 *    0413d690:  8086 10c9 0001 0001 8086 1079 0104 0001   *..........y.....*
 *    0413d6a0:  0000 0000 0000 0000 0000 0000 0000 0000   *................*
 */
UINT32                       G_dwPciBusMapCnt;
T_PCIDESC                    G_oPciBusMap[BUSMAP_CNT];

/*-FUNCTIONS-----------------------------------------------------------------*/
#ifdef INCLUDE_VXBUS
STATUS SysLoSalIntConnect(int iLevel, VOIDFUNCPTR pfIsr, int parameter)
{
    return pciIntConnect(INUM_TO_IVEC(INT_NUM_GET(iLevel)), pfIsr, parameter);
}
STATUS SysLoSalIntDisconnect(int iLevel, VOIDFUNCPTR pfIsr, int parameter)
{
    return pciIntDisconnect2(INUM_TO_IVEC(INT_NUM_GET(iLevel)), pfIsr, parameter);
}
#else /* INCLUDE_VXBUS */
STATUS SysLoSalIntConnect(int iLevel, VOIDFUNCPTR pfIsr, int parameter)
{
    return pciIntConnect(INUM_TO_IVEC(INT_NUM_GET(iLevel)), pfIsr, parameter);
}
STATUS SysLoSalIntDisconnect(int iLevel, VOIDFUNCPTR pfIsr, int parameter)
{
    return pciIntDisconnect2(INUM_TO_IVEC(INT_NUM_GET(iLevel)), pfIsr, parameter);
}
#endif /* INCLUDE_VXBUS */

static STATUS SLOSAL_PciForEachFunc(INT32 nBus, SLOSAL_PCIFOREACH_FUNC pfnFunc, void *pvArg)
{
STATUS status       = ERROR;
UINT8  byTmp        = 0;
INT32  nDevice      = 0;
INT32  nFunction    = 0;
UINT16 wClass       = 0;
UINT16 wVendorId    = 0;

    /* walk through the devices on the bus */
    for (nDevice = 0; nDevice < PCI_MAX_DEV; nDevice++)
    {
        /* walk through the functions of the device */
        for (nFunction = 0; nFunction < PCI_MAX_FUNC; nFunction++)
        {
            /* get PCI vendor and device IDs */
            pciConfigInWord(nBus, nDevice, nFunction, PCI_CFG_VENDOR_ID, &wVendorId);

            /* check for a valid vendor number */
            if ((0xFFFF == wVendorId) || (0x0000 == wVendorId))
            {
                if (0 == nFunction)
                {
                    break;      /* non-existent device, go to next device */
                }
                else
                {
                    continue;   /* function empty, try the next function */
                }
            }
            /* get PCI sub class */
            pciConfigInWord(nBus, nDevice, nFunction, PCI_CFG_SUBCLASS, &wClass);

            /* handle non P2P bridges */
            if (((PCI_CLASS_BRIDGE_CTLR << 8) | PCI_SUBCLASS_HOST_PCI_BRIDGE) != wClass)
            {
               /* call specified function */
               status = (*pfnFunc)(nBus, nDevice, nFunction, pvArg);
               if (OK != status)
               {
                  goto Exit;
               }
            }
            /* if P2P bridge, explore that bus recursively */
            if ((((PCI_CLASS_BRIDGE_CTLR << 8) + PCI_SUBCLASS_P2P_BRIDGE)     == wClass)
             || (((PCI_CLASS_BRIDGE_CTLR << 8) + PCI_SUBCLASS_CARDBUS_BRIDGE) == wClass))
            {
                pciConfigInByte(nBus, nDevice, nFunction, PCI_CFG_SECONDARY_BUS, &byTmp);
                if (0 < byTmp)
                {
                    status = SLOSAL_PciForEachFunc(byTmp, pfnFunc, pvArg);
                    if (OK != status)
                    {
                        goto Exit;
                    }
                }
            }
            /* proceed to next device if single function device */
            if (0 == nFunction)
            {
                pciConfigInByte(nBus, nDevice, nFunction, PCI_CFG_HEADER_TYPE, &byTmp);
                if (0 == (byTmp & PCI_HEADER_MULTI_FUNC))
                {
                    break;      /* no more functions - proceed to next PCI device */
                }
            }
        }
    }
    /* no error */
    status = OK;

Exit:
    return status;
}

static STATUS SLOSAL_PciInit(INT32 nBus, INT32 nDevice, INT32 nFunction, void* pvArg)
{
INT32  nLoop      = 0;
UINT32 dwTmp      = 0;
UINT32 dwClass    = 0;
UINT32 dwRevision = 0;
UINT16 wVendorId  = 0;
UINT16 wDeviceId  = 0;
LOSAL_T_CARD_IDENTIFY* aKnownCards   = (LOSAL_T_CARD_IDENTIFY*)pvArg;
LOSAL_T_CARD_IDENTIFY* pKnownCardCur = NULL;

    /* get PCI class */
    pciConfigInLong(nBus, nDevice, nFunction, PCI_CFG_REVISION, &dwTmp);
    dwClass    = (dwTmp >> 8) & 0x00ffffff;
    dwRevision = dwTmp & 0x00ffffff;

    /* should be net class */
    if (PCI_NET_ETHERNET_CLASS != dwClass)
    {
        goto Exit;
    }
    
#ifdef vmfDevicePciIsForRtos
    {
    BOOL   bIsForRtos = FALSE;
    UINT32 dwResult   = 0;

    /* Check if this card is assigned to VxWin */
    dwResult = vmfDevicePciIsForRtos( nBus, nDevice, nFunction, 0, &bIsForRtos );
    if (! ( (dwResult == RTE_SUCCESS) && bIsForRtos ) )
    {
       /* Do not map card's memory */
       goto Exit;
    }
    }
 #endif
    
    /* get PCI vendor and device IDs */
    pciConfigInLong(nBus, nDevice, nFunction, PCI_CFG_VENDOR_ID, &dwTmp);
    wDeviceId = (dwTmp >> 16) & 0x0000ffff;
    wVendorId = dwTmp & 0x0000ffff;
     
    /* walk through known cards table */
    pKnownCardCur = aKnownCards;
    while (!((0 == pKnownCardCur->wVendorId) && (0 == pKnownCardCur->wDeviceId) && (0 == pKnownCardCur->wHWId)))
    {
        if ((wVendorId == pKnownCardCur->wVendorId) && (wDeviceId == pKnownCardCur->wDeviceId))
        {
        UINT8  byHeaderType = 0;
        INT32  nBARs        = 0;
        UINT32 dwBAR        = 0;
        BOOL   bAddr64      = FALSE;
        UINT32 dwBase       = 0;
        UINT32 dwLength     = 0;
        UINT32 dwSpace      = 0;
        UINT32 dwType       = 0;
        
            /* Cache card info into memory to accelerate next scans */
            /*assert (G_dwPciBusMapCnt < BUSMAP_CNT);*/
            G_oPciBusMap[G_dwPciBusMapCnt].byBusNo      = (UINT32) nBus;
            G_oPciBusMap[G_dwPciBusMapCnt].byDeviceNo   = (UINT32) nDevice;
            G_oPciBusMap[G_dwPciBusMapCnt].byFunctionNo = (UINT32) nFunction;
            G_oPciBusMap[G_dwPciBusMapCnt].wVendorId    = wVendorId;
            G_oPciBusMap[G_dwPciBusMapCnt].wDeviceId    = wDeviceId;
            G_dwPciBusMapCnt++;

            /* get BAR count */
            pciConfigInByte(nBus, nDevice, nFunction, PCI_CFG_HEADER_TYPE, &byHeaderType);
            byHeaderType = byHeaderType & PCI_HEADER_TYPE_MASK;
            switch (byHeaderType)
            {
            case PCI_HEADER_TYPE0:
                nBARs = 6;
                break;
            case PCI_HEADER_PCI_PCI:
                nBARs = 2;
                break;
            case PCI_HEADER_PCI_CARDBUS:
                nBARs = 1;
                break;
            default:
                nBARs = 0;
                break;
            }
            /* walk through the BARs */
            for (nLoop = 0; nLoop < nBARs; nLoop++)
            {
                /* calculate BAR address */
                dwBAR = PCI_CFG_BASE_ADDRESS_0 + nLoop*sizeof(UINT32);

                /* handle 64 bit BAR */
                if (bAddr64)
                {
                    /* previous BAR was 64bit, skip  current BAR */
                    bAddr64 = FALSE;
                    continue;
                }
                /* read BAR */
                pciConfigInLong(nBus, nDevice, nFunction, dwBAR, &dwTmp);
                if ((0x00000000 == dwTmp) || (0xFFFFFFFF == dwTmp))
                {
                    /* BAR is not implemented */
                    continue;
                }
                /* read the length */
                pciConfigOutLong(nBus, nDevice, nFunction, dwBAR, 0xFFFFFFFF);
                pciConfigInLong( nBus, nDevice, nFunction, dwBAR, &dwLength);
                pciConfigOutLong(nBus, nDevice, nFunction, dwBAR, dwTmp);
                dwLength = (dwLength ^ 0xFFFFFFFF) + 1;

                /* read BAR informations */
                dwSpace  = dwTmp & PCI_BAR_SPACE_MASK;
                dwType   = dwTmp & PCI_BAR_MEM_TYPE_MASK;
                dwBase   = dwTmp & PCI_MEMBASE_MASK;

                /* only map memory BAR */
                if (dwSpace != PCI_BAR_SPACE_MEM)
                {
                    continue;
                }
                /* process address attribute info */
                switch (dwType)
                {
                case PCI_BAR_MEM_ADDR64:
                    /* currently physical addresses above 4Gb are not supported,
                     * so for right now we'll skip the next base address register
                     * which belongs to the 64-bit pair of 32-bit base address
                     * registers.
                     */
                    bAddr64 = TRUE;
                    break;
                case PCI_BAR_MEM_ADDR32:
                case PCI_BAR_MEM_BELOW_1MB:
                    break;
                case PCI_BAR_MEM_RESERVED:
                default:
                    continue;
                }
#ifdef VM_STATE_FOR_PCI
                /* map the memory-mapped IO (CSR) space into host CPU address space */
                sysMmuMapAdd(
                      (void *)(PCI_MEMIO2LOCAL( SLSL_ROUND_DOWN(dwBase, 0x1000) )), /* Align to page boundary */
                      SLSL_ROUND_UP(dwLength, 0x1000),                              /* Round up to page boundary */
                      VM_STATE_MASK_FOR_ALL, VM_STATE_FOR_PCI);
#endif
            }
        }
        /* prepare next loop */
        pKnownCardCur++;
    }
Exit:
    return OK;
}


/***************************************************************************************************/
/**
\brief  Scan PCI bus for a card with vendor, device and function IDs given 

\return TRUE if card is found, FALSE otherwise.
*/
BOOL SysLoSalPCIFindDev(
    UINT32                 dwVendorId,   /**< [in]   PCI vendor ID */
    UINT32                 dwDeviceId,   /**< [in]   PCI device ID */
    UINT32                 dwIndex,      /**< [in]   desired instance of device. 1..2.. */
    UINT32*                pdwBus,       /**< [out]  PCI Bus number */
    UINT32*                pdwDevice,    /**< [out]  PCI Device number */
    UINT32*                pdwFunction   /**< [out]  PCI Function number */
                             )
{
    UINT32             dwUnitCur       = 0;
    BOOL               bFound          = FALSE;
    UINT32             i;
    T_PCIDESC*         pPci;

    /* check parameters */
    if ( NULL == pdwBus || NULL == pdwDevice || NULL == pdwFunction )
    {
        goto Exit;
    }

    /* Scan bus for this card */
    for (i = 0; (i < G_dwPciBusMapCnt) && (! bFound); ++i)
    {
       pPci = &G_oPciBusMap[i];

       if (   (dwVendorId == pPci->wVendorId)
           && (dwDeviceId == pPci->wDeviceId)
          )
       {
          /* check for right instance */
          dwUnitCur++;
          if (dwUnitCur == dwIndex)
          {
             *pdwBus      = pPci->byBusNo;
             *pdwDevice   = pPci->byDeviceNo;
             *pdwFunction = pPci->byFunctionNo;
             bFound       = TRUE;
             goto Exit;
          }
       }
    } /* for(...) */

Exit:
   return bFound;
}

static STATUS SLOSAL_sysInit(LOSAL_T_CARD_IDENTIFY* aKnownCards)
{
   /* Scan PCI bus, build in memory cache and map known cards */
   return SLOSAL_PciForEachFunc(0, (SLOSAL_PCIFOREACH_FUNC)SLOSAL_PciInit, (void*)aKnownCards);
}


/* 
 *  - The following guideline is for VxWorks 5.5 or later (NOT VxWorks 5.4)
 *  - DO NOT follow items 5 - 7 with VxWorks 6.6 or other VxWorks systems with VXBUS
 *  
 *  1.) copy this File (sysLoSalAdd.c) to your BSP Directory
 *  2.) open the file sysLib.c
 *  3.) search the function : sysHwInit
 *  4.) right before sysHwInit insert this line:
 *      #include "sysLoSalAdd.c"
 *  5.) within the function sysHwInit locate the call:
 *      pciConfigForeachFunc (0, TRUE, (PCI_FOREACH_FUNC) sysNetPciInit, NULL);
 *  6.) right after the next line (#endif /o INCLUDE_NETWORK o/) insert this call:
 *      SLOSAL_sysInitFEI() (or one of the other following functions);
 *  7.) This call maps your PRO/100 card into the MMU table of your VxWorks target and allows to use the AT-EM LinkLayer.
 *  8.) Take care the Define "INCLUDE_FEI" is not set for your kernel image, by excluding the FEI END Package from your
 *      project.
 *  9.) Rebuild your Kernel Image
 * 10.) Download to Target
 * 11.) That's it.
 */
static STATUS SLOSAL_sysInitGEI(void)
{
    return SLOSAL_sysInit(S_oBSPKnownGeiCards);
}
static STATUS SLOSAL_sysInitFEI(void)
{
    return SLOSAL_sysInit(S_oBSPKnownFeiCards);
}
static STATUS SLOSAL_sysInitRTL(void)
{
    return SLOSAL_sysInit(S_oBSPKnownRtlCards);
}
static STATUS SLOSAL_sysInitRTL8169(void)
{
    return SLOSAL_sysInit(S_oBSPKnownRtl8169Cards);
}

#endif /*  defined(SLOSAL_NOT_INBSP) */

#endif /* !defined(EXCLUDE_SYS_LOSAL) */

/*-END OF SOURCE FILE--------------------------------------------------------*/
