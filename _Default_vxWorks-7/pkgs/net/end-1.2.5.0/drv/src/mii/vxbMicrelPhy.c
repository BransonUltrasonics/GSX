/* vxbMicrelPhy.c - driver for Micrel PHY chips */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27may16,dlk  Merge from SR0470. In user-space build, leave
             board-specific micrelParams in place for now.
28mar16,wap  Fix parameter handling, remove unneeded includes
22mar16,wap  Fix static analysis issues
17mar16,wap  Switch to using vxbParamLib (F1921)
18nov15,wap  Clean up compiler warnings in user-side build
09oct15,wap  Convert to use vxbParamLib instead of direct FDT calls
05oct15,wap  Add support for user-space VxBus interface (F2165)
17aug15,m_w  support dynamic skew pad config of ksz9021. (V7PRO-2344)
03aug15,m_w  add micrelPhyShutdown.
08jul15,wap  Fix memory leak in detach method
15jun15,wap  Add shutdown method, fix detach method (F3973, V7PRO-1989)
08oct14,m_w  removed vxbFdtDev from PHY_HARDWARE. (V7PRO-1297)
02sep14,m_w  add support for ksz9031.(US40646)
15aug14,m_w  written.(V7PRO-1195)
*/

/*
DESCRIPTION
This module implements Micrel Phys: Giga phy ksz9021
                                    Giga phy ksz9031

The initialization method resets always resets the PHY to a known
state, usually by toggling the power down bit in the control register
and then setting the reset bit. Then it programs the PHY for
autonegotiation. Chip-specific drivers may also do additional
setup or fixups if necessary.

The mode set routine can be used to program the PHY for a specific
media configuration. The mode word is based on the generic ifmedia
mode definitions, i.e. IFM_AUTO, IFM_100_TX, IFM_10_T, IFM_FDX and
IFM_HDX. Selecting a specific mode (other than auto) programs the
PHY for just that mode. This can be used to force the chip to run
at 10, 100Mbps or 1000Mbps and full or half duplex.

The mode get routine retrieves the current link speed and duplex
mode (i.e. what the link setting is right now) and the link state.
If the IFM_ACTIVE bit is not set in the link state, then the mode
information is set to IFM_NONE (no link available).

SEE ALSO: vxBus, miiBus, ifLib, endLib
\tb "MICREL KSZ9021GN Gigabit Ethernet Transceiver with GMII/MII
     Support"
\tb "MICREL KSZ9021RL/RN Gigabit Ethernet Transceiver with RGMII
     Support(Revision 1.2)"
\tb "MICREL KSZ9031MNX Gigabit Ethernet Transceiver with GMII/MII
     Support(Revision 2.2)"
\tb "MICREL KSZ9031RNX Gigabit Ethernet Transceiver with RGMII
     Support(Revision 2.2)"
*/

#include <vxWorks.h>
#include <hwif/vxBus.h>
#include <hwif/util/vxbParamLib.h>
#ifdef _WRS_KERNEL
#include <hwif/buslib/vxbFdtLib.h>
#endif /* _WRS_KERNEL */
#include <vxbMiiLib.h>
#include "vxbMicrelPhy.h"

/* defines */

/* debug macro */

#define MICREL_PHY_DEBUG
#ifdef MICREL_PHY_DEBUG
#include <private/kwriteLibP.h>         /* _func_kprintf */

#define PHY_PROBE_MAX_RETRY		5
#define DBG_OFF             0x00000000
#define DBG_WARN            0x00000001
#define DBG_ERR             0x00000002
#define DBG_INFO            0x00000004
#define DBG_ALL             0xffffffff
LOCAL UINT32 dbgMask = DBG_ALL;

#undef DBG_MSG
#define DBG_MSG(mask,...)                          \
do                                                 \
{                                                  \
    if ((dbgMask & (mask)) || ((mask) == DBG_ALL)) \
    {                                              \
        if (_func_kprintf != NULL)                 \
        {                                          \
        (* _func_kprintf)("%s,%d, ",__FUNCTION__,__LINE__);\
        (* _func_kprintf)(__VA_ARGS__);            \
        }                                          \
    }                                              \
}while (0)
#else
#define DBG_MSG(...)
#endif  /* MICREL_PHY_DEBUG */

LOCAL STATUS micrelPhyModeSet (VXB_DEV_ID, UINT32);
LOCAL STATUS micrelPhyModeGet (VXB_DEV_ID, UINT32 *, UINT32 *);
LOCAL void   micrelPhyInit    (VXB_DEV_ID);
LOCAL STATUS micrelPhyProbe   (VXB_DEV_ID);
LOCAL STATUS micrelPhyAttach  (VXB_DEV_ID);
LOCAL STATUS micrelPhyDetach  (VXB_DEV_ID);
LOCAL STATUS micrelPhyShutdown(VXB_DEV_ID);
LOCAL void   ksz9021PhyExtendedWrite(VXB_DEV_ID, UINT32, UINT16);
LOCAL UINT16 ksz9021PhyExtendedRead(VXB_DEV_ID, UINT32);
LOCAL void   ksz9021LoadSkewValues(VXB_DEV_ID,UINT16,UINT8,char **,UINT8);
LOCAL void   ksz9021ConfigInit(VXB_DEV_ID);
LOCAL void   ksz9031PhyExtendedWrite(VXB_DEV_ID, UINT8, UINT32, UINT32, UINT16);
LOCAL UINT16 ksz9031PhyExtendedRead(VXB_DEV_ID, UINT8, UINT32, UINT32);
LOCAL void   ksz9031LoadSkewValues(VXB_DEV_ID,UINT16,UINT8,char **,UINT8);
LOCAL void   ksz9031ConfigInit(VXB_DEV_ID);
#ifdef MICREL_PHY_DEBUG
LOCAL void   micrelPhyInfoShow(VXB_DEV_ID);
#endif /* MICREL_PHY_DEBUG */

LOCAL VXB_DRV_METHOD micrelPhyMethods[] =
    {
    /* DEVICE API */
    { VXB_DEVMETHOD_CALL(vxbDevProbe),    micrelPhyProbe},
    { VXB_DEVMETHOD_CALL(vxbDevAttach),   micrelPhyAttach},
    { VXB_DEVMETHOD_CALL(vxbDevDetach),   micrelPhyDetach},
    { VXB_DEVMETHOD_CALL(vxbDevShutdown), micrelPhyShutdown},

    /* MII API */
    { VXB_DEVMETHOD_CALL(miiModeGet),     micrelPhyModeGet},
    { VXB_DEVMETHOD_CALL(miiModeSet),     micrelPhyModeSet},
    VXB_DEVMETHOD_END
    };

#ifdef _WRS_KERNEL

LOCAL VXB_PARAMS micrelParams[] =
    {
        { "txen-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "txc-skew-ps",  VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "rxdv-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "rxc-skew-ps",  VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },

        { "rxd0-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "rxd1-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "rxd2-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "rxd3-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },

        { "txd0-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "txd1-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "txd2-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { "txd3-skew-ps", VXB_PARAM_INT32, {(void *)MICREL_SKEW_NONE} },
        { NULL, VXB_PARAM_END_OF_LIST, { NULL } }
    };

#else /* _WRS_KERNEL */

/*
 * FIXME: The use of these hard-coded and board-specific parameters
 * here is a temporary workaround to set them for the user-mode stack
 * on i.MX6.
 */

LOCAL VXB_PARAMS micrelParams[] =
    {
       /*
        * Note, ALL vxbus parameters supported by the driver must have a
        * default entry here, in order for the parameter system to work.
        */
        { "txen-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "txc-skew-ps", VXB_PARAM_INT32, { (void *)3000 } },
        { "rxdv-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "rxc-skew-ps", VXB_PARAM_INT32, { (void *)3000 } },

        { "rxd0-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "rxd1-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "rxd2-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "rxd3-skew-ps", VXB_PARAM_INT32, { (void *)0 } },

        { "txd0-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "txd1-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "txd2-skew-ps", VXB_PARAM_INT32, { (void *)0 } },
        { "txd3-skew-ps", VXB_PARAM_INT32, { (void *)0 } },

        { NULL, VXB_PARAM_END_OF_LIST, { NULL } }
    };

#endif /* _WRS_KERNEL */

VXB_DRV  micrelPhyDrv =
    {
    { NULL } ,
    "micrelPhy",		/* Name */
    "Micrel PHY driver",	/* Description */
    VXB_BUSID_MII,		/* Class */
    VXB_DRVFLAG_PARAM,		/* Flags */
    0,				/* Reference count */
    micrelPhyMethods,		/* Method table */
    micrelParams		/* Parameters */
    };

VXB_DRV_DEF(micrelPhyDrv)

/*******************************************************************************
*
* micrelPhyProbe - vxBus probe handler
*
* This routine will read the PHY ID from hardware and compare it with
* pre-defined PHY ID to determine if this is a device we supportted.
*
* RETURNS: OK when matched exactly or ERROR when not macthed.
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyProbe
    (
    VXB_DEV_ID pDev
    )
    {
    PHY_HARDWARE  * pPhyDev;
    UINT32          miiOui;
	UINT32			i;
    pPhyDev = vxbDevIvarsGet(pDev);
	for(i=0; i< PHY_PROBE_MAX_RETRY; i++)
		{
		(void)miiBusRead (pDev, pPhyDev->phyAddr, MII_PHY_ID1_REG, &pPhyDev->phyId1);
		(void)miiBusRead (pDev, pPhyDev->phyAddr, MII_PHY_ID2_REG, &pPhyDev->phyId2);

#ifdef MICREL_PHY_DEBUG
		micrelPhyInfoShow(pDev);
#endif

		miiOui = MII_OUI(pPhyDev->phyId1,pPhyDev->phyId2);

		if( miiOui == MICREL_OUI_ID )
			{
			/* match Micrel PHY ID exactly */
			
			return OK;
			}
		else
			{
			(* _func_kprintf)("retry count = %d\n", i);
			continue;
			}
		}// retry for loop
    return ERROR;
    }

/*******************************************************************************
*
* micrelPhyShutdown - VxBus shutdown handler
*
* This function implements the VxBus shutdown method for this driver.
* We power off the phy here.
*
* RETURNS: OK always
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyShutdown
    (
    VXB_DEV_ID  pDev
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16         miiVal;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet (pDev);

    /* power off the phy */

    miiVal = MII_CR_POWER_DOWN;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    return OK;
    }

/*******************************************************************************
*
* micrelPhyDetach - VxBus detach handler
*
* This function implements the VxBus unlink method for this driver.
* We delete each media type that was originally added to the MII bus
* instance by this device and take ourselves off the miiMonitor task
* list.
*
* RETURNS: OK if detach succeeds, else ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyDetach
    (
    VXB_DEV_ID pDev
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiSts;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet (pDev);

    if (pDrvCtrl->miiInitialized == FALSE)
        return (ERROR);

    /* Remove ourselves from the miiMonitor task list. */

    miiDevListDel (pDev);

    /* Remove media list entries. */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    if ((miiSts & MII_SR_EXT_STS) !=0 )
        {
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_1000_T);
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_1000_T|IFM_FDX);
        }

    if (miiSts & MII_SR_TX_HALF_DPX)
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_100_TX);
    if (miiSts & MII_SR_TX_FULL_DPX)
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_100_TX|IFM_FDX);
    if (miiSts & MII_SR_10T_HALF_DPX)
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_10_T);
    if (miiSts & MII_SR_10T_FULL_DPX)
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_10_T|IFM_FDX);
    if (miiSts & MII_SR_AUTO_SEL)
        (void)miiBusMediaDel (pDev, IFM_ETHER|IFM_AUTO);

    pDrvCtrl->miiInitialized = FALSE;

    vxbDevSoftcSet (pDev, NULL);

    vxbMemFree ((char *)pDrvCtrl->miiMediaList);
    vxbMemFree ((char *)pDrvCtrl);

    return (OK);
    }

/*******************************************************************************
*
* micrelPhyAttach - attach micreltric phy device
*
* This routine attach micreltric phy device with micrelPhyDrv,it creats device
* and assign resource for it.
*
* \NOMANUAL
*
* RETURNS: OK if attached successfully, otherwise ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyAttach
    (
    VXB_DEV_ID pDev
    )
    {
    MII_DRV_CTRL   * pDrvCtrl;
    PHY_HARDWARE   * pPhyDev;
    END_MEDIALIST  * pMediaList;
    UINT16           miiSts;

    pPhyDev = vxbDevIvarsGet(pDev);
    if (pPhyDev == NULL)
        {
        DBG_MSG(DBG_ERR,"vxbDevIvarsGet() failed\n");
        return ERROR;
        }

    pDrvCtrl = vxbMemAlloc (sizeof(MII_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return ERROR;

    pMediaList = vxbMemAlloc (sizeof(END_MEDIALIST));

    if (pMediaList == NULL)
        {
        vxbMemFree (pDrvCtrl);
        return (ERROR);
        }

    pDrvCtrl->miiParent = NULL;
    pDrvCtrl->miiSelf = pDev;
    pDrvCtrl->miiPhyAddr = pPhyDev->phyAddr;
    pDrvCtrl->miiMediaList = pMediaList;
    pDrvCtrl->pInst = pDev;
    pDrvCtrl->pEndInst = vxbDevParent(pDev);

    vxbDevSoftcSet(pDev, pDrvCtrl);

    miiDevListAdd (pDev);

    /* Select unit number. */

    pDrvCtrl->miiPhyList[0] = pDev;

    if (pDrvCtrl->miiInitialized == TRUE)
        return OK;

    pDrvCtrl->miiInitialized = TRUE;

    /* Tell miiBus about the media we support. */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    if ((miiSts & MII_SR_EXT_STS) != 0)
        {
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_1000_T);
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_1000_T|IFM_FDX);
        }

    if (miiSts & MII_SR_TX_HALF_DPX)
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_100_TX);
    if (miiSts & MII_SR_TX_FULL_DPX)
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_100_TX|IFM_FDX);
    if (miiSts & MII_SR_10T_HALF_DPX)
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_10_T);
    if (miiSts & MII_SR_10T_FULL_DPX)
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_10_T|IFM_FDX);
    if (miiSts & MII_SR_AUTO_SEL)
        (void)miiBusMediaAdd (pDev, IFM_ETHER|IFM_AUTO);

    (void)miiBusMediaDefaultSet (pDev, IFM_ETHER|IFM_AUTO);

    /* Initialize the PHY. */

    micrelPhyInit (pDev);

    return OK;
    }

/*******************************************************************************
*
* micrelPhyInit - Init phy
*
* This routine checks init micrel phy
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL void micrelPhyInit
    (
    VXB_DEV_ID pDev
    )
    {
    MII_DRV_CTRL  * pDrvCtrl;
    PHY_HARDWARE  * pPhyDev;
    UINT16 miiSts;
    UINT16 miiCtl;
    UINT16 miiVal;
    UINT16 miiModel;
    int i;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);
    pPhyDev  = (PHY_HARDWARE *)vxbDevIvarsGet(pDev);

    /* Get status register so we can look for extended capabilities. */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    miiVal = MII_CR_POWER_DOWN;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    miiVal = 0;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    /* Set reset bit and then wait for it to clear. */

    miiVal = MII_CR_RESET;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    for (i = 0; i < 1000; i++)
        {
        (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiCtl);
        if (!(miiCtl & MII_CR_RESET))
            break;
        }

    /*
     * If the extended capabilities bit is set, this is a gigE
     * PHY, so make sure we advertise gigE modes.
     */

    if ((miiSts & MII_SR_EXT_STS) != 0)
        {
        /* Enable advertisement of gigE modes. */

        miiVal = MII_MASSLA_CTRL_1000T_FD|MII_MASSLA_CTRL_1000T_HD;
        (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_CTRL_REG, miiVal);
        }

    /* Setup the vendor extented register or special requirement */

    miiModel = (UINT16)MII_MODEL(pPhyDev->phyId2);

    /*
     * Some boards require special tuning values, particularly
     * when it comes to clock delays.
     */

    if (miiModel == PHY_ID_KSZ9021)
        {
        ksz9021ConfigInit(pDev);
        }
    else if (miiModel == PHY_ID_KSZ9031)
        {
        ksz9031ConfigInit(pDev);
        }

    /*
     * Some PHYs come out of reset with their isolate bit set. Make
     * sure we don't write that bit back when setting the control
     * register.
     */

    miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiCtl);

    return;
    }

/*******************************************************************************
*
* micrelPhyModeSet - set the PHY mode
*
* This function programs the PHY for a given media mode. Modes are specified
* using ifmedia settings, as follows:
*
* IFM_10_T              10Mbps
* IFM_10_T|IFM_FDX      10Mbps full duplex
* IFM_100_TX            100Mbps
* IFM_100_TX|IFM_FDX    100Mbps full duplex
* IFM_1000_T            1000Mbps
* IFM_AUTO              autonegotiate
*
* To implement 1000Mbps only mode, we program the chip for autonegotiation
* and advertise support only for 1000Mbps modes. This is done because to
* really force 1000Mbps mode manually, you must program the PHY on one
* side of the link to be master and the other to be slave (the master
* generates a timing signal to which the slave must synchronize). The
* master/slave setting will automatically be resolved with autonegotiation,
* so most manufacturers recommend using autoneg instead of manual selection.
*
* RETURNS: OK if setting the mode succeeded, or ERROR it's unsupported
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyModeSet
    (
    VXB_DEV_ID pDev,
    UINT32 mode
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal;
    UINT16 miiAnar = 0;
    UINT16 gmiiAnar = 0;
    UINT16 miiCtl = 0;
    UINT16 miiSts;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    /* Get status register so we can look for extended capabilities. */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    switch(IFM_SUBTYPE(mode)) {
    case IFM_AUTO:

        /* Set autoneg advertisement to advertise all modes. */

        miiAnar = MII_ANAR_10TX_HD|MII_ANAR_10TX_FD|
                  MII_ANAR_100TX_HD|MII_ANAR_100TX_FD;
        if (miiSts & MII_SR_EXT_STS)
            gmiiAnar = MII_MASSLA_CTRL_1000T_FD|MII_MASSLA_CTRL_1000T_HD;
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        break;
    case IFM_1000_T:
        if (!(miiSts & MII_SR_EXT_STS))
            return(ERROR);
        gmiiAnar = MII_MASSLA_CTRL_1000T_FD|MII_MASSLA_CTRL_1000T_HD;
        miiAnar = 0;
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        break;
    case IFM_100_TX:
        miiCtl = MII_CR_100|MII_CR_AUTO_EN|MII_CR_RESTART;
        if ((mode & IFM_GMASK) == IFM_FDX)
            {
            miiAnar = MII_ANAR_100TX_FD;
            miiCtl |= MII_CR_FDX;
            }
        else
            miiAnar = MII_ANAR_100TX_HD;
        gmiiAnar = 0;
        break;
    case IFM_10_T:
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        if ((mode & IFM_GMASK) == IFM_FDX)
            {
            miiAnar = MII_ANAR_10TX_FD;
            miiCtl |= MII_CR_FDX;
            }
        else
            miiAnar = MII_ANAR_10TX_HD;
        gmiiAnar = 0;
        break;
    default:
        return (ERROR);
        /* NOTREACHED */
        break;
    }

    micrelPhyInit (pDev);

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_AN_ADS_REG, &miiVal);
    miiVal &= (UINT16)(~(MII_ANAR_10TX_HD|MII_ANAR_10TX_FD |
                         MII_ANAR_100TX_HD|MII_ANAR_100TX_FD));
    miiVal |= miiAnar;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_AN_ADS_REG, miiVal);

    if (miiSts & MII_SR_EXT_STS)
        {
        (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr,
                          MII_MASSLA_CTRL_REG, &miiVal);
        miiVal &= (UINT16)(~(MII_MASSLA_CTRL_1000T_HD |
                             MII_MASSLA_CTRL_1000T_FD));
        miiVal |= gmiiAnar;
        (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,
                           MII_MASSLA_CTRL_REG, miiVal);
        }

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
    miiVal &= (UINT16)(~(MII_CR_FDX|MII_CR_100|MII_CR_AUTO_EN|MII_CR_RESTART));
    miiVal |= miiCtl;
    (void)miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    return(OK);
    }

/*******************************************************************************
*
* micrelPhyModeGet - read the current PHY mode and link status
*
* This function returns the current state of the PHY to the caller.
* If the link is up, the link status will be returned as
* IFM_AVALID|IFM_ACTIVE, and the mode will reflect the current
* link speed and duplex setting. This function always returns
* the real time link state.
*
* RETURNS: OK if querying the PHY succeeds, or ERROR otherwise
*
* ERRNO: N/A
*/

LOCAL STATUS micrelPhyModeGet
    (
    VXB_DEV_ID pDev,
    UINT32 * mode,
    UINT32 * status
    )
    {
    UINT16 miiSts;
    UINT16 miiCtl;
    UINT16 miiAnar;
    UINT16 miiLpar;
    UINT16 gmiiAnar = 0;
    UINT16 gmiiLpar = 0;
    UINT16 anlpar;
    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    *mode = IFM_ETHER;
    *status = IFM_AVALID;

    /* read MII status register once to unlatch link status bit */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    /* read again to know its current value */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_STAT_REG, &miiSts);

    /* no link bit means no carrier. */
    if (!(miiSts & MII_SR_LINK_STATUS))
        {
        *mode |= IFM_NONE;

        return (OK);
        }

    *status |= IFM_ACTIVE;

    /*
     * read the control, ability advertisement and link
     * partner advertisement registers.
     */

    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiCtl);
    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_AN_ADS_REG, &miiAnar);
    (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_AN_PRTN_REG, &miiLpar);

    if ((miiSts & MII_SR_EXT_STS) != 0)
        {
        (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_CTRL_REG, &gmiiAnar);
        (void)miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_STAT_REG, &gmiiLpar);
        }

    /*
     * If autoneg is on, figure out the link settings from the
     * advertisement and partner ability registers. If autoneg is
     * off, use the settings in the control register.
     */

    if ((miiCtl & MII_CR_AUTO_EN) != 0)
        {
        anlpar = miiAnar & miiLpar;
        if (gmiiAnar & MII_MASSLA_CTRL_1000T_FD &&
            gmiiLpar & MII_MASSLA_STAT_LP1000T_FD)
            *mode |= IFM_1000_T|IFM_FDX;
        else if (gmiiAnar & MII_MASSLA_CTRL_1000T_HD &&
                 gmiiLpar & MII_MASSLA_STAT_LP1000T_HD)
            *mode |= IFM_1000_T|IFM_HDX;
        else if (anlpar & MII_ANAR_100TX_FD)
            *mode |= IFM_100_TX|IFM_FDX;
        else if (anlpar & MII_ANAR_100TX_HD)
            *mode |= IFM_100_TX|IFM_HDX;
        else if (anlpar & MII_ANAR_10TX_FD)
            *mode |= IFM_10_T|IFM_FDX;
        else if (anlpar & MII_ANAR_10TX_HD)
            *mode |= IFM_10_T|IFM_HDX;
        else
            *mode |= IFM_NONE;

        }
    else
        {
        if (miiCtl & MII_CR_FDX)
            *mode |= IFM_FDX;
        else
            *mode |= IFM_HDX;
        if ((miiCtl & (MII_CR_100 | MII_CR_1000)) == (MII_CR_100 | MII_CR_1000))
            *mode |= IFM_1000_T;
        else if (miiCtl & MII_CR_100)
            *mode |= IFM_100_TX;
        else
            *mode |= IFM_10_T;

        }

    return (OK);
    }

/*******************************************************************************
*
* ksz9021PhyExtendedWrite - write value to ksz9021 extended phy register
*
* This function write value to ksz9021 extended phy register.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ksz9021PhyExtendedWrite
    (
    VXB_DEV_ID          pDev,
    UINT32              regNum,
    UINT16              val
    )
    {
    MII_DRV_CTRL *      pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9021_EXTREG_CTRL,
                      (UINT16)(KSZ9021_EXTREG_WRITE_FLAG | regNum));

    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9021_EXTREG_DATA_WRITE, val);
    }

/*******************************************************************************
*
* ksz9021PhyExtendedRead - read value from ksz9021 extended phy register
*
* This function read value from ksz9021 extended phy register.
*
* RETURNS: value from register.
*
* ERRNO: N/A
*/

LOCAL UINT16 ksz9021PhyExtendedRead
    (
    VXB_DEV_ID  pDev,
    UINT32      regNum
    )
    {
    MII_DRV_CTRL  * pDrvCtrl;
    UINT16          regVal;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9021_EXTREG_CTRL, (UINT16)regNum);

    (void)miiBusRead(pDev, pDrvCtrl->miiPhyAddr, KSZ9021_EXTREG_DATA_READ, &regVal);

    return regVal;
    }

/*******************************************************************************
*
* ksz9021LoadSkewValues - loads skew value from DTS into hardware for ksz9021
*
* This routine loads skew value from DTS into hardware for ksz9021.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL void ksz9021LoadSkewValues
    (
    VXB_DEV_ID  pDev,
    UINT16      reg,
    UINT8       fieldSz,
    char      * field[],
    UINT8       numfields
    )
    {
    INT32 val[4]  = {-1, -1, -1, -1};
    int    i;
    int    matches = 0;
    UINT16 mask;
    UINT16 maxval = 0xF;
    UINT16 newval;
    VXB_PARAM_VALUE vxbVal;

    /* read skew from DTS if exists */

    for (i = 0; i < numfields; i++)
        {
        if (vxbParamGet (pDev, field[i], VXB_PARAM_INT32, &vxbVal) == OK)
            {
            if (vxbVal.int32Val != MICREL_SKEW_NONE)
                {
                /* found a skew config line in DTS */

                val[i] = vxbVal.int32Val;
                matches++;
                }
            }
        }

#ifdef MICREL_PHY_DEBUG
    for (i = 0; i < numfields; i++)
        {
        DBG_MSG(DBG_INFO,"%s = %d\n",field[i], val[i]);
        }
#endif

    /* if the skew was not find in DTS, just return */

    if (matches == 0)
        {
        DBG_MSG(DBG_INFO,"no skew value found in DTS, reg = %d\n", reg);
        return;
        }

    if (matches < numfields)
        newval = ksz9021PhyExtendedRead(pDev, reg);
    else
        newval = 0;

    for (i = 0; i < numfields; i++)
        {
        if (val[i] != -1)
            {
            /* not equal -1 means val was read from DTS */

            mask   = (UINT16)0xffff;
            mask  ^= ((UINT16)(maxval << (fieldSz * i)));
            newval = (UINT16)((newval & mask) |
                              (((val[i] / KSZ9021_PS_TO_REG) & maxval) <<
                               (fieldSz * i)));
            }
        }

    DBG_MSG (DBG_INFO, "reg = 0x%02x[%d],newval = 0x%04x[%d]\n",
             reg, reg, newval, newval);

    /* write value to extended register */

    ksz9021PhyExtendedWrite(pDev, reg, newval);
    }

/*******************************************************************************
*
* ksz9021ConfigInit - config micrel ksz9021 phy
*
* This routine config micrel ksz9021 phy.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL void ksz9021ConfigInit
    (
    VXB_DEV_ID pDev
    )
    {
    char *clkCtrlSkews[4]= {"txen-skew-ps", "txc-skew-ps",
                            "rxdv-skew-ps", "rxc-skew-ps"};

    char *rxDataSkews[4] = {"rxd0-skew-ps", "rxd1-skew-ps",
                            "rxd2-skew-ps", "rxd3-skew-ps"};

    char *txDataSkews[4] = {"txd0-skew-ps", "txd1-skew-ps",
                            "txd2-skew-ps", "txd3-skew-ps"};

    ksz9021LoadSkewValues(pDev,KSZ9021_CLK_CTRL_PAD_SKEW,4,clkCtrlSkews,4);
    ksz9021LoadSkewValues(pDev,KSZ9021_RX_DATA_PAD_SKEW, 4,rxDataSkews, 4);
    ksz9021LoadSkewValues(pDev,KSZ9021_TX_DATA_PAD_SKEW, 4,txDataSkews, 4);
    }

/*******************************************************************************
*
* ksz9031PhyExtendedWrite - write value to ksz9031 extended phy register
*
* This function write value to ksz9031 extended phy register.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ksz9031PhyExtendedWrite
    (
    VXB_DEV_ID  pDev,
    UINT8       mode,
    UINT32      devAddr,
    UINT32      regNum,
    UINT16      val
    )
    {
    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_CTRL_REG, (UINT16)devAddr);
    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_REGDATA_REG, (UINT16)regNum);
    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_CTRL_REG, (UINT16)((mode << 14) | devAddr));
    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_REGDATA_REG, val);
    }

/*******************************************************************************
*
* ksz9031PhyExtendedRead - read value from ksz9031 extended phy register
*
* This function read value from ksz9031 extended phy register.
*
* RETURNS: value from register.
*
* ERRNO: N/A
*/

LOCAL UINT16 ksz9031PhyExtendedRead
    (
    VXB_DEV_ID  pDev,
    UINT8       mode,
    UINT32      devAddr,
    UINT32      regNum
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16         regVal;

    pDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(pDev);

    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_CTRL_REG, (UINT16)devAddr);
    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_REGDATA_REG, (UINT16)regNum);
    (void)miiBusWrite(pDev, pDrvCtrl->miiPhyAddr,
                      KSZ9031RN_MMD_CTRL_REG, (UINT16)((mode << 14) | devAddr));
    (void)miiBusRead(pDev, pDrvCtrl->miiPhyAddr,
                     KSZ9031RN_MMD_REGDATA_REG,&regVal);

    return regVal;
    }

/*******************************************************************************
*
* ksz9031LoadSkewValues - loads skew value from DTS into hardware for ksz9031
*
* This routine loads skew value from DTS into hardware for ksz9031.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL void ksz9031LoadSkewValues
    (
    VXB_DEV_ID  pDev,
    UINT16      reg,
    UINT8       fieldSz,
    char      * field[],
    UINT8       numfields
    )
    {
    INT32 val[4]  = {-1, -1, -1, -1};
    int    i;
    int    matches = 0;
    UINT16 mask;
    UINT16 maxval;
    UINT16 newval;
    VXB_PARAM_VALUE vxbVal;

    /* read skew from DTS if exists */

    for (i = 0; i < numfields; i++)
        {
        if (vxbParamGet (pDev, field[i], VXB_PARAM_INT32, &vxbVal) == OK)
            {
            if (vxbVal.int32Val != MICREL_SKEW_NONE)
                {
                /* found a skew config line in DTS */

                val[i] = vxbVal.int32Val;
                matches++;
                }
            }
        }

#ifdef MICREL_PHY_DEBUG
    for (i = 0; i < numfields; i++)
        {
        DBG_MSG(DBG_INFO,"%s = %d\n",field[i], val[i]);
        }
#endif

    /* if the skew was not find in DTS, just return */

    if (matches == 0)
        return;

    if (matches < numfields)
        newval = ksz9031PhyExtendedRead(pDev, KSZ9031_OP_DATA, 2, reg);
    else
        newval = 0;

    maxval = (fieldSz == 4) ? 0xf : 0x1f;
    for (i = 0; i < numfields; i++)
        {
        if (val[i] != -1)
            {
            /* not equal -1 means val was read from DTS */

            mask = (UINT16)0xffff;
            mask ^= ((UINT16)(maxval << (fieldSz * i)));
            newval = (UINT16)((newval & mask) |
                              (((val[i] / KSZ9031_PS_TO_REG) & maxval) <<
                               (fieldSz * i)));
            }
        }

    DBG_MSG(DBG_INFO, "reg = 0x%02x[%d],newval = 0x%04x[%d]\n",
            reg, reg, newval, newval);

    ksz9031PhyExtendedWrite(pDev, KSZ9031_OP_DATA, 2, reg, newval);
    }

/*******************************************************************************
*
* ksz9031ConfigInit - config micrel ksz9031 phy
*
* This routine config micrel ksz9031 phy.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL void ksz9031ConfigInit
    (
    VXB_DEV_ID pDev
    )
    {
    char *clkSkews[2]    = {"rxc-skew-ps",  "txc-skew-ps"};
    char *ctrlSkews[2]   = {"txen-skew-ps", "rxdv-skew-ps"};
    char *rxDataSkews[4] = {"rxd0-skew-ps", "rxd1-skew-ps",
                            "rxd2-skew-ps", "rxd3-skew-ps"};
    char *txDataSkews[4] = {"txd0-skew-ps", "txd1-skew-ps",
                            "txd2-skew-ps", "txd3-skew-ps"};

    ksz9031LoadSkewValues(pDev,KSZ9031RN_CLK_PAD_SKEW, 5,clkSkews,2);
    ksz9031LoadSkewValues(pDev,KSZ9031RN_CTRL_PAD_SKEW,4,ctrlSkews,2);
    ksz9031LoadSkewValues(pDev,KSZ9031RN_RX_DATA_PAD_SKEW,4,rxDataSkews,4);
    ksz9031LoadSkewValues(pDev,KSZ9031RN_TX_DATA_PAD_SKEW,4,txDataSkews,4);
    }

#ifdef MICREL_PHY_DEBUG
/*********************************************************************
*
* micrelPhyInfoShow - show the micrel phy info for debug
*
* This routine show the micrel phy info for debug.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void micrelPhyInfoShow
    (
    VXB_DEV_ID pDev
    )
    {
    PHY_HARDWARE  * pPhyDev;
    UINT32          miiOui;
    UINT16          miiModel = 0xFFFF;
    UINT16          miiRevNum  = 0xFFFF;

    pPhyDev    = vxbDevIvarsGet(pDev);
    miiOui     = MII_OUI(pPhyDev->phyId1,pPhyDev->phyId2);
    miiModel   = (UINT16)MII_MODEL(pPhyDev->phyId2);
    miiRevNum  = MII_REV(pPhyDev->phyId2);

    (* _func_kprintf)("phyId1 = 0x%04x\n",pPhyDev->phyId1);
    (* _func_kprintf)("phyId2 = 0x%04x\n",pPhyDev->phyId2);
    (* _func_kprintf)("miiOui = 0x%08x\n",miiOui);
    (* _func_kprintf)("miiModel = 0x%04x\n",miiModel);
    (* _func_kprintf)("miiRevNum = 0x%04x\n\n",miiRevNum);

    if (miiOui == MICREL_OUI_ID)
        {
        (* _func_kprintf)("PHY manufacturer: MICREL");

        if (miiModel == PHY_ID_KSZ9021)
            (* _func_kprintf)(", Mode: KSZ9021");
        else if (miiModel == PHY_ID_KSZ9031)
            (* _func_kprintf)(", Mode: KSZ9031");

        (* _func_kprintf)(", Rev: %d\n\n",miiRevNum);
        }
    }
#endif /* MICREL_PHY_DEBUG */
