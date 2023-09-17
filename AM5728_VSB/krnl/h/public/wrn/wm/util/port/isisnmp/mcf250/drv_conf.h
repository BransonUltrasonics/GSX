/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/isisnmp/mcf250/drv_conf.h,v 1.1.1.1 2001/11/05 17:47:10 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: drv_conf.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:10  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/02/24 16:05:55  josh
 * configuration files for pSOS for ColdFire
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/


/* @(#) pSOSystem PowerPC/V2.5.0-BETA1: apps.v22/xxxx/drv_conf.c 3.24 (snmpsa) 98/06/03 15:46:01 */
/***********************************************************************/
/*                                                                     */
/*   MODULE:  drv_conf.c                                               */
/*   DATE:    98/06/03                                                 */
/*   PURPOSE: pSOSystem driver configuration and initialization        */
/*                                                                     */
/*            There are two functions in this file:                    */
/*                                                                     */
/*            SetUpDrivers to setup drivers except for Network drivers */
/*            SetUpNI      to setup Network drivers                    */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*           Copyright 1993 - 1997, Integrated Systems, Inc.           */
/*                      ALL RIGHTS RESERVED                            */
/*                                                                     */
/*   Permission is hereby granted to licensees of Integrated Systems,  */
/*   Inc. products to use or abstract this computer program for the    */
/*   sole purpose of implementing a product based on Integrated        */
/*   Systems, Inc. products.   No other rights to reproduce, use,      */
/*   or disseminate this computer program, whether in part or in       */
/*   whole, are granted.                                               */
/*                                                                     */
/*   Integrated Systems, Inc. makes no representation or warranties    */
/*   with respect to the performance of this computer program, and     */
/*   specifically disclaims any responsibility for any damages,        */
/*   special or consequential, connected with the use of this program. */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#include <sys_conf.h>
#include "bsp.h"
#include <bspfuncs.h>
#include <configs.h>
#include <sysvars.h>
#include <pna.h>

#ifdef SC_INIT_DNTABLE
#include <stdarg.h>
static long GenerateString(char *dest, char *format, ...);
#endif

#if SC_DEV_SPI
    #include "spi.h"
#endif

#if (SC_PSE)
#include <psecfg.h>
#include <sys/stream.h>
#endif

#if (SC_DEV_DLPI)
#include <sys/enetdlpi.h>
#endif

#if (SC_DEV_NULL)
extern void NullDevInit(struct ioparms *);
extern void NullDevRead(struct ioparms *);
extern void NullDevWrite(struct ioparms *);
extern void NullDevCntrl(struct ioparms *);
#endif

#define NULLF (ULONG) ((void (*)()) 0)

static const char RarpErrTimeout[] =
  "RARP request failed: Check for a RARP server";
static const char RarpErrNet[] =
  "RARP request failed: Network Error";

#if SC_PSOS || SC_PSOSM
/***********************************************************************/
/* SetUpDrivers: installs drivers in PsosIO table via a call to        */
/*               InstallDriver and then calls driver setup function if */
/*               needed. NOTE: If the driver is to be part of the      */
/*               Network Interface then it should be placed in the     */
/*               SetUpNI function.                                     */
/*                                                                     */
/*       INPUT: FreeMemPtr = A pointer to unused memory that can be    */
/*                           used to allocate space for a driver.      */
/*                           NOTE: Be sure to advance FreeMemPtr if    */
/*                           memory is allocates!                      */
/*                                                                     */
/*     RETURNS: The new address of free memory after the drivers have  */
/*              allocated space.                                       */
/*                                                                     */
/*        NOTE: This function does many precompile checks so errors    */
/*              can be found during compile-time rather then at run-   */
/*              time.                                                  */
/*                                                                     */
/* To add a non-Network driver to pSOSystem, call InstallDriver().     */
/* This adds a device driver to the PsosIO table.  InstallDriver()     */
/* takes the following arguments:                                      */
/*                                                                     */
/*     unsigned short major_number    device major number              */
/*     void (*dev_init)(),            device init procedure            */
/*     void (*dev_open)(),            device open procedure            */
/*     void (*dev_close)(),           device close procedure           */
/*     void (*dev_read)(),            device read procedure            */
/*     void (*dev_write)(),           device write procedure           */
/*     void (*dev_ioctl)(),           device control procedure         */
/*     unsigned long dev_param,       Used for STREAMS modules         */
/*     unsigned short rsvd2)          reserved                         */
/*     unsigned short flags;          If set to IO_AUTOINIT pSOS will  */
/*                                    automatically call the           */
/*                                    drivers initialization           */
/*                                    function. The SC_AUTOINIT        */
/*                                    #define which is set in          */
/*                                    sys_conf.h can be used to set    */
/*                                    this element.                    */
/*                                                                     */
/* InstallDriver() does not return a value.  InstallDriver() assumes   */
/* SC_DEVMAX is defined in sys_conf.h and is useful only before pSOS+  */
/* has been initialized.                                               */
/*                                                                     */
/* If you are adding a driver that needs to be initialized before      */
/* either pSOS+ is initialized or the driver's init function is        */
/* called, then you can call a setup function (that you create) for    */
/* the driver (for example, see the CnslSetup call for the serial      */
/* device driver).  If your initialization function needs to allocate  */
/* memory, you may pass it the FreeMemPtr.  The function should return */
/* a new FreeMemPtr which points to the end of the memory it has       */
/* allocated.                                                          */
/*                                                                     */
/* When adding code to install a new driver, it is a good idea to add  */
/* precompile checks so errors can be found during compile-time rather */
/* then at run-time.  Precompile checks are used, for example, in the  */
/* code that installs the pSOSystem serial device driver below.        */
/*                                                                     */
/***********************************************************************/
UCHAR *SetUpDrivers(UCHAR *FreeMemPtr)
{
extern ULONG PsosNumDntEnts;

/*---------------------------------------------------------------------*/
/* Install the pSOSystem serial device driver.                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SERIAL && BSP_SERIAL)
    #if(SC_DEV_SERIAL > SC_DEVMAX)
        #error "SC_DEV_SERIAL cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_SERIAL, CnslInit, NULLF, NULLF,CnslRead,
                  CnslWrite, CnslCntrl, 0, 0, SC_AUTOINIT);
    FreeMemPtr = CnslSetup(FreeMemPtr, SysVars.DefBaud,
                           SC_APP_CONSOLE);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install the pSEUDO console driver                                   */
/*---------------------------------------------------------------------*/
#if (DEV_PSCONSOLE)
    InstallDriver(SC_DEV_PSCONSOLE, PsConsoleInit, PsConsoleOpen,
                  PsConsoleClose, PsConsoleRead, PsConsoleWrite,
                  PsConsoleCntrl, 0, 0, 0);
    PsosNumDntEnts +=4;
#endif

/*--------------------------------------------------------------------*/
/* Setup pSOSystem PC console device driver                           */
/*--------------------------------------------------------------------*/
#if (SC_DEV_CONSOLE && BSP_CONSOLE)
    #if (SC_DEV_CONSOLE >SC_DEVMAX)
    #error "SC_DEV_CONSOLE cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_CONSOLE, PcCnslInit, NULLF, NULLF,
                  PcCnslRead, PcCnslWrite, PcCnslCntrl, 0, 0,
                  SC_AUTOINIT);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install the pSOSystem tick timer.                                   */
/*---------------------------------------------------------------------*/
#if SC_DEV_TIMER
    #if(SC_DEV_TIMER > SC_DEVMAX)
        #error "SC_DEV_TIMER cannot be > SC_DEVMAX"
    #endif
    #if (!SC_PSOS && !SC_PSOSM)
        #error "Must have pSOS+ or pSOS+m in system to use tick timer!"
    #endif
    InstallDriver(SC_DEV_TIMER, RtcInit, NULLF, NULLF, NULLF, NULLF,
                  NULLF, 0, 0, SC_AUTOINIT);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install DevNull driver                                              */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_NULL)
    InstallDriver(SC_DEV_NULL, NullDevInit, NULLF, NULLF,NullDevRead,
                  NullDevWrite, NullDevCntrl, 0, 0, 0);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install the pSOSystem ramdisk driver.                               */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if SC_DEV_RAMDISK
    #if(SC_DEV_RAMDISK > SC_DEVMAX)
        #error "SC_DEV_RAMDISK cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_RAMDISK, RdskInit, RdskOpen, RdskClose, RdskRead,
                  RdskWrite, RdskCntrl, 0, 0, 0);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install the pSOSystem SCSI driver.                                  */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SCSI && BSP_SCSI)
    #if(SC_DEV_SCSI > SC_DEVMAX)
        #error "SC_DEV_SCSI cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_SCSI, SdrvInit, SdskOpen, SdskClose, SdskRead,
                  SdskWrite, SdrvCntrl, 0, 0, 0);
    PsosNumDntEnts += (8*5);
#endif

/*---------------------------------------------------------------------*/
/* Install the pSOSystem SCSI Tape driver.                             */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SCSI_TAPE && BSP_SCSI_TAPE)
    #if(SC_DEV_SCSI_TAPE > SC_DEVMAX)
        #error "SC_DEV_SCSI_TAPE cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_SCSI_TAPE, SdrvInit, StapeOpen, StapeClose,
                  StapeRead, StapeWrite, SdrvCntrl, 0, 0, 0);

    PsosNumDntEnts += 8;
    /*-----------------------------------------------------------------*/
    /* Don't need to call SdrvSetup this time since it was done before */
    /*-----------------------------------------------------------------*/
    /* SdrvSetup(); */
#endif

/*---------------------------------------------------------------------*/
/* Setup pSOSystem CMOS clock driver                                   */
/*---------------------------------------------------------------------*/
#if (SC_DEV_CMOS && BSP_CLOCK)
    #if (SC_DEV_CMOS >SC_DEVMAX)
    #error "SC_DEV_CMOS cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_CMOS, NULLF, NULLF, NULLF, CmcRead,
                  CmcWrite, NULLF, 0, 0, 0);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Setup pSOSystem printer driver                                      */
/*---------------------------------------------------------------------*/
#if (SC_DEV_PARALLEL && BSP_PARALLEL)
    #if (SC_DEV_PARALLEL >SC_DEVMAX)
    #error "SC_DEV_PARALLEL cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_PARALLEL, ParInit, ParOpen, ParClose, NULLF,
                  ParWrite, ParCntrl, 0, 0, 0);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Setup pSOSystem floppy diskette driver                              */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_FLOPPY && BSP_FLOPPY)
    #if (SC_DEV_FLOPPY >SC_DEVMAX)
    #error "SC_DEV_FLOPPY cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_FLOPPY, FlpInit, NULLF, NULLF, FlpRead,
                  FlpWrite, NULLF, 0, 0, 0);
    PsosNumDntEnts += 2;
#endif

/*---------------------------------------------------------------------*/
/* Setup pSOSystem IDE disk driver                                     */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_IDE && BSP_IDE)
    #if (SC_DEV_IDE >SC_DEVMAX)
    #error "SC_DEV_IDE cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_IDE, IDEInit, NULLF, NULLF, IDERead,
                  IDEWrite, IDECntrl, 0, 0, 0);
    PsosNumDntEnts += (4*5);
#endif

/*---------------------------------------------------------------------*/
/* Install the pSOSystem TFTP driver.                                  */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_TFTP)
    #if(SC_DEV_TFTP > SC_DEVMAX)
        #error "SC_DEV_TFTP cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_TFTP, TftpInit, TftpOpen, TftpClose, TftpRead,
                  NULLF, TftpCntl, 0, 0, 0);
    PsosNumDntEnts += SC_MAX_TFTP_CHAN;
#endif

/*---------------------------------------------------------------------*/
/* Install the SPI driver                                              */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SPI)
    #if(SC_DEV_SPI > SC_DEVMAX)
        #error "SC_DEV_SPI cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_SPI, SPI_init, SPI_open, SPI_close, NULLF,
                  NULLF, SPI_io, 0, 0, 0);
    PsosNumDntEnts++;
#endif

/*---------------------------------------------------------------------*/
/* Install DLPI drivers                                                */
/*---------------------------------------------------------------------*/

#if (SC_DEV_DLPI || SC_DEV_PPP || SC_DEV_SLIP)
    {
#include "enetcfg.h"

int dlpi_setinitroutine = 0;
int dlpi_nicount = 0;	/* number of ni's below this DLPI */
void (*init_fn)();	/* Ethernet driver initialization routine */
                        /* Call this function only once */
char *DlpiErrMsg = "DlpiAddNI failed: NI Error";

#if (SC_DEV_DLPI)
    {
    #if(SC_DEV_DLPI> SC_DEVMAX)
        #error "SC_DEV_DLPI cannot be > SC_DEVMAX"
    #endif
    static const psedrvparam_t eth0param = {"eth0", DT_CLONE, &enetinfo, 0, 0};
    init_fn = NULLF;
    if (!dlpi_setinitroutine)
    {
        init_fn = (void (*)())enetinit;
        dlpi_setinitroutine = 1;
    }
    InstallDriver(SC_DEV_DLPI, init_fn, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&eth0param, 0, 0);
    dlpi_nicount++;
    PsosNumDntEnts++;
    }
#endif /* SC_DEV_DLPI */

#if SC_DEV_DLPI_PPP
    {
    #if(SC_DEV_DLPI_PPP > SC_DEVMAX)
        #error "SC_DEV_DLPI_PPP cannot be > SC_DEVMAX"
    #endif
    static const psedrvparam_t pppparam = {"ppp", DT_CLONE, &enetinfo, 0, 0};
    init_fn = NULLF;
    if (!dlpi_setinitroutine)
    {
        init_fn = (void (*)())enetinit;
        dlpi_setinitroutine = 1;
    }
    InstallDriver(SC_DEV_DLPI_PPP, init_fn, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&pppparam, 0, 0);
    dlpi_nicount++;
    PsosNumDntEnts++;
    }
#endif /* SC_DEV_DLPI_PPP */

#if SC_DEV_DLPI_SLIP
    {
    #if(SC_DEV_DLPI_SLIP > SC_DEVMAX)
        #error "SC_DEV_DLPI_SLIP cannot be > SC_DEVMAX"
    #endif
    static const psedrvparam_t slipparam = {"slip", DT_CLONE, &enetinfo, 0, 0};
    init_fn = NULLF;
    if (!dlpi_setinitroutine)
    {
        init_fn = (void (*)())enetinit;
        dlpi_setinitroutine = 1;
    }
    InstallDriver(SC_DEV_DLPI_SLIP, init_fn, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&slipparam, 0, 0);
    dlpi_nicount++;
    PsosNumDntEnts++;
    }
#endif /* SC_DEV_DLPI_SLIP */

/*
 * User has defined other DLPI drivers locally. Include user
 * file drv_conf.idl that installs user's own ENETDLPI drivers
 */
#if SC_DEV_DLPI_OTHER
#include "drv_conf.idl"
#endif

    /*-------------------------------------------------------------*/
    /* Call DlpiInit to initialize the Streams DLPI driver         */
    /*   Arguments passed to DlpiInit are:                         */
    /*     1. Free memory address                                  */
    /*     2. Maximum NI interface number                          */
    /*     3. Maximum number of streams per interface              */
    /*-------------------------------------------------------------*/
    FreeMemPtr = DlpiInit(FreeMemPtr, dlpi_nicount, SE_DLPI_MAXSTR_PERNI);

    /*-------------------------------------------------------------*/
    /* Call DlpiAddNI to add an NI interface to the Streams DLPI   */
    /* driver.  DlpiAddNI must be called for each NI driver        */
    /* interface used with the Streams DLPI driver.                */
    /*   Arguments passed to DlpiAddNI are:                        */
    /*     1. NI interface number (should correspond to the pNA+   */
    /*          interface number if driver is shared with pNA+)    */
    /*     2. NI entry point                                       */
    /*     3. NI flags                                             */
    /*          DLPI_IFF_SELFLOOP 0x0001  loopback for selfaddress */
    /*          DLPI_IFF_POLL_NI  0x0002  driver must be polled    */
    /*          DLPI_IFF_RAWMEM   0x2000  driver accepts mblk      */
    /*          DLPI_IFF_NONIINIT 0x4000  Don't send NI_INIT       */
    /*                                    during enetinit() call   */
    /*     4. NI major device number                               */
    /*     5. NI multicast address (NOT SUPPORTED)                 */
    /*     6. NI poll frequency (every # of elapsed ticks)         */
    /*-------------------------------------------------------------*/

#if (SC_DEV_DLPI)
    /*
     * If this lan NI is not shared, use a loopback DLPI with interface
     * number 0. NI entry point is set to DlpiNullNI().
     */
#if (SE_SHARE_NI)
    if (DlpiAddNI(SE_DLPI_ETH_IFNO, (long(*)())BSP_LAN1_ENTRY,
                  SE_DLPI_ETH_IFFLAGS, SC_DEV_DLPI, 0, SE_DLPI_ETH_POLLFREQ))
        SysInitFail(DlpiErrMsg);
#else
    if (DlpiAddNI(0, (long(*)())DlpiNullNI, 0, SC_DEV_DLPI, 0, 0))
        SysInitFail(DlpiErrMsg);
#endif /* SE_SHARE_NI */

#endif /* SC_DEV_DLPI */

#if (SC_DEV_DLPI_PPP)
    if (DlpiAddNI(SE_DLPI_PPP_IFNO, SE_DLPI_PPP_ENTRY, SE_DLPI_PPP_IFFLAGS,
                  SC_DEV_DLPI_PPP, 0, SE_DLPI_PPP_POLLFREQ))
        SysInitFail(DlpiErrMsg);
    if (DlpiAddParams(SE_DLPI_PPP_IFNO, SE_DLPI_PPP_ADDRSIZE,
                      SE_DLPI_PPP_MAXMTU, SE_DLPI_PPP_MINMTU, (char *)NULL))
        SysInitFail(DlpiErrMsg);
#endif /* SC_DEV_DLPI_PPP */

#if (SC_DEV_DLPI_SLIP)
    if (DlpiAddNI(SE_DLPI_SLIP_IFNO,SE_DLPI_SLIP_ENTRY, SE_DLPI_SLIP_IFFLAGS,
                  SC_DEV_DLPI_SLIP, 0, SE_DLPI_SLIP_POLLFREQ))
        SysInitFail(DlpiErrMsg);
    if (DlpiAddParams(SE_DLPI_SLIP_IFNO, SE_DLPI_SLIP_ADDRSIZE,
                      SE_DLPI_SLIP_MAXMTU, SE_DLPI_SLIP_MINMTU, (char *)NULL))
        SysInitFail(DlpiErrMsg);
#endif /* SC_DEV_DLPI_SLIP */

/*
 * User has defined other DLPI drivers locally. Include user
 * file drv_conf.adl that adds user Dlpi drivers to the system.
 */
#if SC_DEV_DLPI_OTHER
#include "drv_conf.adl"
#endif

    }
#endif /* (SC_DEV_DLPI || SC_DEV_PPP || SC_DEV_SLIP) */

#if (SC_DEV_LOG)
  {
    extern void loginit();
    extern struct streamtab loginfo;
    static const psedrvparam_t log_drv = {"log", DT_CLONE,   &loginfo, 0, 0};

    InstallDriver(SC_DEV_LOG,   loginit,       NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&log_drv, 0, 0);
    PsosNumDntEnts++;
  }
#endif

/*-------------------------------------------------------------------*/
/* Install pSOSystem Watchdog driver                                 */
/*-------------------------------------------------------------------*/
#if (SC_DEV_WATCHDOG)
    #if(SC_DEV_WATCHDOG > SC_DEVMAX)
    #error "SC_DEV_WATCHDOG cannot be > SC_DEVMAX"
    #endif
    InstallDriver(SC_DEV_WATCHDOG, WdtInit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, 0, 0, 0);
    PsosNumDntEnts++;
#endif

#if (SC_DEV_OTCP)
    {
extern void otcp_ipspace_init(), otcp_arpspace_init(), otcp_rawspace_init(),
	    otcp_udpspace_init(), otcp_tcpspace_init(), otcp_loopspace_init();
extern struct streamtab otcp_ipinfo, otcp_arpinfo, otcp_rawinfo, otcp_udpinfo,
			otcp_tcpinfo, otcp_loopinfo;

#if (SC_IP)
    static const psedrvparam_t ip_drv   = {"ip",   DT_CLONE, &otcp_ipinfo, 0, 0};
#endif
#if (SC_ARP)
    static const psedrvparam_t arp_drv  = {"arp",  DT_CLONE, &otcp_arpinfo, 0, 0};
#endif
#if (SC_TCP)
    static const psedrvparam_t tcp_drv  = {"tcp",  DT_CLONE, &otcp_tcpinfo, 0, 0};
#endif
#if (SC_UDP)
    static const psedrvparam_t udp_drv  = {"udp",  DT_CLONE, &otcp_udpinfo, 0, 0};
#endif
#if (SC_RAW)
    static const psedrvparam_t raw_drv  = {"raw",  DT_CLONE, &otcp_rawinfo, 0, 0};
#endif
#if (SC_LOOP)
    static const psedrvparam_t loop_drv = {"loop", DT_CLONE, &otcp_loopinfo, 0, 0};
#endif

#if (SC_IP)
    InstallDriver(SC_IP, otcp_ipspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&ip_drv, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_ARP)
    InstallDriver(SC_ARP, otcp_arpspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&arp_drv, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_RAW)
    InstallDriver(SC_RAW, otcp_rawspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&raw_drv, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_UDP)
    InstallDriver(SC_UDP, otcp_udpspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&udp_drv, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_TCP)
    InstallDriver(SC_TCP, otcp_tcpspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&tcp_drv, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_LOOP)
    InstallDriver(SC_LOOP, otcp_loopspace_init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&loop_drv, 0, 0);
    PsosNumDntEnts++;
#endif

    }
#endif /* (SC_DEV_OTCP) */

#if (SC_DEV_OLAP)
  {
    extern struct streamtab phpiinfo, twfrinfo ;
    extern void   phpiinit(), twfrinit();
#if (SC_PHPI)
    static const psedrvparam_t phpiparam={"phpi",DT_CLONE,&phpiinfo,  0, 0};
#endif
#if (SC_LAPB)
    static const psedrvparam_t lapbparam ={"lapb", DT_REGULAR,&twfrinfo,0, 0};
#endif

#if (SC_PHPI)
    InstallDriver(SC_PHPI, phpiinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&phpiparam, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_LAPB)
    InstallDriver(SC_LAPB, twfrinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&lapbparam, 0, 0);
    PsosNumDntEnts++;
#endif
  }
#endif

#if (SC_DEV_OX25)
  {
    extern struct streamtab phpiinfo, twfrinfo, twpkinfo, twsninfo, ipconvinfo;
    extern void twpkinit(), twsninit(), tcpsninit();
#if (SC_X25)
    static const psedrvparam_t x25param  ={"x25",  DT_DRIVER, &twpkinfo, 0, 0};
#endif
#if (SC_SNDCF)
    static const psedrvparam_t sndfparam ={"sndf", DT_REGULAR, &twsninfo, 0, 0};
#endif
#if (SC_IPCONV)
    static const psedrvparam_t ipconvparam={"ipsn",DT_REGULAR, &ipconvinfo, 0, 0};
#endif

#if (SC_X25)
    InstallDriver(SC_X25, twpkinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&x25param, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_SNDCF)
    InstallDriver(SC_SNDCF, twsninit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&sndfparam, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_IPCONV)
    InstallDriver(SC_IPCONV, tcpsninit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&ipconvparam, 0, 0);
    PsosNumDntEnts++;
#endif
  }
#endif

#if (SC_DEV_ISDN)
  {
    extern struct streamtab phinfo, q921info, ipcdinfo ;
    extern void q921init(), phinit();

#if (SC_PH)
    static const psedrvparam_t phpiparam={"ph",DT_REGULAR,&phinfo,  0, 0};
#endif
#if (SC_LAPD)
    static const psedrvparam_t lapbparam={"q921",DT_REGULAR,&q921info,0,0};
#endif
#if (SC_IPCD)
    static const psedrvparam_t ipcdparam={"ipcd",DT_REGULAR,&ipcdinfo,0,0};
#endif

#if (SC_PH)
    InstallDriver(SC_PH, phinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&phpiparam, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_LAPD)
    InstallDriver(SC_LAPD, q921init, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&lapbparam, 0, 0);
    PsosNumDntEnts++;
#endif
#if (SC_IPCD)
    InstallDriver(SC_IPCD, NULLF, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&ipcdparam, 0, 0);
    PsosNumDntEnts++;
#endif
  }
#endif

#if (SC_DEV_MLPP)
  {
    extern struct streamtab frmuxtab, pppstream, Spimtab;
    extern void frmuxinit();

#if (SC_FRMUX)
    static const psedrvparam_t fmuxparam ={"cfmux",DT_REGULAR,&frmuxtab,0,0};
#endif
#if (SC_PPP)
    static const psedrvparam_t pppparam ={"cppp",DT_REGULAR,&pppstream,0,0};
#endif
#if (SC_PIM)
    static const psedrvparam_t pimparam ={"cpim",DT_REGULAR,&Spimtab,0,0};
#endif

#if (SC_FRMUX)
    InstallDriver(SC_FRMUX, frmuxinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&fmuxparam, 0, 0);
#endif
#if (SC_PPP)
    InstallDriver(SC_PPP, NULLF, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&pppparam, 0, 0);
#endif
#if (SC_PIM)
    InstallDriver(SC_PIM, NULLF, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&pimparam, 0, 0);
#endif
  }
#endif

#if defined(SC_DEV_PSMUX) && SC_DEV_PSMUX
#include "drv_conf.app"
#endif

/*---------------------------------------------------------------------*/
/* Additional drivers may be installed here.                           */
/*---------------------------------------------------------------------*/

return FreeMemPtr;
}

/***********************************************************************/
/* DrvSysStartCO : This is called from sysinit.c "SysStartCO".         */
/*                                                                     */
/*      INPUTS: None                                                   */
/*                                                                     */
/*     RETURNS: None                                                   */
/*     OUTPUTS: None                                                   */
/*     NOTE(S): Currently used to set-up the Initial Device Name Table */
/*              Additional code can be added here to clean-up the      */
/*              driver specific data area.                             */
/*                                                                     */
/***********************************************************************/
void DrvSysStartCO()
{
    long i, j;
    char namebuf[KC_DNLEN+1];

/*---------------------------------------------------------------------*/
/* Following are the call-outs which will be called. If your driver    */
/* needs pSOS+ "GS" (Warm Startup) notification to clear your driver   */
/* data structures, register here, so that it will be called.          */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SERIAL && BSP_SERIAL)
        DevSerialCO();
#endif

/*---------------------------------------------------------------------*/
/* Default DEV_SERIAL is passed as the DEFAULT CONSOLE for Re-Direction*/
/*---------------------------------------------------------------------*/
#if SC_DEV_PSCONSOLE
        PsConsoleSysStartCo(DEV_SERIAL);
#endif

#if SC_DEV_SCSI && BSP_SCSI
        DevScsiCO();
#endif

#if SC_DEV_SCSI_TAPE && BSP_SCSI_TAPE
        DevScsiCO();
#endif

#if SC_DEV_IDE && BSP_IDE
        DevIdeCO();
#endif

#if SC_DEV_PARALLEL
        DevParallelCO();
#endif

#if SC_DEV_FLOPPY && BSP_FLOPPY
        DevFloppyCO();
#endif

#if SC_DEV_TFTP
        DevTftpCO();
#endif

#if SC_DEV_PSEUDO
        DevPseudoCO();
#endif

/*---------------------------------------------------------------------*/
/* This code is used to build the initial table of device names used   */
/* in the pSOSystem.                                                   */
/*---------------------------------------------------------------------*/
#if SC_INIT_DNTABLE
    /*-----------------------------------------------------------------*/
    /* Make one name for the entrie pSOSystem Serial channels.         */
    /*-----------------------------------------------------------------*/
    #if (SC_DEV_SERIAL && BSP_SERIAL)
            dnt_add("tty", DEV_SERIAL);
    #endif /* SC_DEV_SERIAL */

    /*-----------------------------------------------------------------*/
    /* Null Driver.                                                    */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_NULL
            dnt_add("null", DEV_NULL);
    #endif

    /*-----------------------------------------------------------------*/
    /* There is only one timer in the system. Make a name for it.      */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_TIMER
        dnt_add("timer", DEV_TIMER + 0);
    #endif /* SC_DEV_TIMER */

    /*-----------------------------------------------------------------*/
    /* Add DNT entries for all pSEUDO console minor devices            */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_PSCONSOLE
        dnt_add("psconsole", DEV_PSCONSOLE + SYSCONSOLE);
        dnt_add("stdin",     DEV_PSCONSOLE + STDIN_DEV);
        dnt_add("stdout",    DEV_PSCONSOLE + STDOUT_DEV);
        dnt_add("stderr",    DEV_PSCONSOLE + STDERR_DEV);
    #endif

    /*-----------------------------------------------------------------*/
    /* There is only one ramdisk in the system. Make a name for it.    */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_RAMDISK
        dnt_add("ramdisk", DEV_RAMDISK + 0);
    #endif /* SC_DEV_RAMDISK */

    /*-----------------------------------------------------------------*/
    /* Make the default names for the SCSI disks supported by          */
    /* pSOSystem. The names are specified as rsdXYZZ, where X is a     */
    /* single hex digit specifying the SCSI bus number (currently      */
    /* pSOSystem only supports one SCSI bus) 0-f, Y is a single hex    */
    /* digit specifying the drive number on a given SCSI bus 0-f, and  */
    /* ZZ is two hex digits specifying the partion number 00-ff. A     */
    /* partition number of zero specifies an unpartition drive. All    */
    /* the hex digits are lower case.                                  */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_SCSI && BSP_SCSI
        for (i=0; i<8; i++)
            for (j=0; j<5; j++)
                {
                GenerateString(namebuf, "rsd%x%x%.2x", 0, i, j);
                dnt_add(namebuf, DEV_SCSI + ((j << 8) + i));
                }
    #endif /* SC_DEV_SCSI && BSP_SCSI */

    /*-----------------------------------------------------------------*/
    /* Make the default names for the SCSI tape drives supported by    */
    /* pSOSystem. The names are specified as rstXY, where X is a       */
    /* single hex digit specifying the SCSI bus number (currently      */
    /* pSOSystem only supports one SCSI bus) 0-f, Y is a single hex    */
    /* digit specifying the drive number on a given SCSI bus 0-f.      */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_SCSI_TAPE && BSP_SCSI_TAPE
        for (i=0; i<8; i++)
            {
            GenerateString(namebuf, "rst%x%x", 0, i);
            dnt_add(namebuf, DEV_SCSI_TAPE + i);
            }
    #endif /* SC_DEV_SCSI_TAPE && BSP_SCSI_TAPE */

    /*-----------------------------------------------------------------*/
    /* Make the default names for the IDE disks supported by           */
    /* pSOSystem. The names are specified as ideYZZ, where Y is a      */
    /* single hex digit specifying the drive number on the IDE bus     */
    /* 0-f, and ZZ is two hex digits specifying the partion number     */
    /* 00-ff. A partition number of zero specifies an unpartition      */
    /* drive. All the hex digits are lower case.                       */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_IDE && BSP_IDE
        for (i=0; i<2; i++)
            for (j=0; j<5; j++)
                {
                GenerateString(namebuf, "ide%x%.2x", i, j);
                dnt_add(namebuf, DEV_IDE + ((j << 8) + i));
                }
    #endif /* SC_DEV_IDE && BSP_IDE */

    /*-----------------------------------------------------------------*/
    /* There is only one parallel driver in the system. Make a name    */
    /* for it.                                                         */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_PARALLEL
        dnt_add("printer", DEV_PARALLEL);
    #endif /* SC_DEV_PARALLEL */

    /*-----------------------------------------------------------------*/
    /* Add the names for the first and second floppy drives in the     */
    /* system.                                                         */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_FLOPPY && BSP_FLOPPY
        dnt_add("rfd0", DEV_FLOPPY);
        dnt_add("rfd1", (DEV_FLOPPY) | 1);
    #endif /* SC_DEV_FLOPPY */

    /*-----------------------------------------------------------------*/
    /* There is only one watchdog driver in the system. Make a name    */
    /* for it.                                                         */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_WATCHDOG
        dnt_add("watchdog", DEV_WATCHDOG);
    #endif /* SC_DEV_WATCHDOG */

    /*-----------------------------------------------------------------*/
    /* There is only one cmos driver in the system. Make a name for it.*/
    /*-----------------------------------------------------------------*/
    #if SC_DEV_CMOS
        dnt_add("cmos", DEV_CMOS);
    #endif /* SC_DEV_CMOS */

    /*-----------------------------------------------------------------*/
    /* Make default names for the TFTP pseudo device. Add names for    */
    /* the eight channels supported by default. If the number of       */
    /* channels supported is changed, add or subtract names as needed. */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_TFTP
        for (i=0; i<SC_MAX_TFTP_CHAN; i++)
            {
            GenerateString(namebuf, "tftp%x", i);
            dnt_add(namebuf, DEV_TFTP + i);
            }
    #endif /* SC_DEV_TFTP */

    /*-----------------------------------------------------------------*/
    /* There is only one "Pseudo" driver in the system. Make a name    */
    /* for it.                                                         */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_PSEUDO
        dnt_add("pseudo", DEV_PSEUDO);
    #endif /* SC_DEV_PSEUDO */

#if (SC_DEV_DLPI || SC_DEV_PPP || SC_DEV_SLIP)
    /*-----------------------------------------------------------------*/
    /* There is only one enetdlpi driver in the system.                */
    /* Make a name for it.                                             */
    /*-----------------------------------------------------------------*/
    #if SC_DEV_DLPI
        dnt_add("dlpi", DEV_DLPI);
    #endif /* SC_DEV_DLPI */

    /*-----------------------------------------------------------------*/
    /* There is only one enetslip driver in the system. Make a name for it.*/
    /*-----------------------------------------------------------------*/
    #if SC_DEV_DLPI_SLIP
        dnt_add("dlslip", DEV_DLPI_SLIP);
    #endif /* SC_DEV_DLPI_SLIP */

    /*-----------------------------------------------------------------*/
    /* There is only one enetppp driver in the system. Make a name for it.*/
    /*-----------------------------------------------------------------*/
    #if SC_DEV_DLPI_PPP
        dnt_add("dlppp", DEV_DLPI_PPP);
    #endif /* SC_DEV_DLPI_PPP */

#endif

#if (SC_DEV_LOG)
        dnt_add("log", DEV_LOG);
#endif

#if (SC_DEV_OTCP)

#if (SC_IP)
        dnt_add("ip", DEV_IP);
#endif
#if (SC_ARP)
        dnt_add("arp", DEV_ARP);
#endif
#if (SC_TCP)
        dnt_add("tcp", DEV_TCP);
#endif
#if (SC_UDP)
        dnt_add("udp", DEV_UDP);
#endif
#if (SC_RAW)
        dnt_add("raw", DEV_RAW);
#endif
#if (SC_LOOP)
        dnt_add("loop", DEV_LOOP);
#endif

#endif


#if (SC_DEV_OLAP)

#if (SC_PHPI)
        dnt_add("phpi", DEV_PHPI);
#endif

#if (SC_LAPB)
        dnt_add("lapb", DEV_LAPB);
#endif

#endif

#if (SC_DEV_OX25)

#if (SC_X25)
        dnt_add("x25", DEV_X25);
#endif

#if (SC_SNDCF)
        dnt_add("sndcf", DEV_SNDCF);
#endif

#if (SC_IPCONV)
        dnt_add("ipconv", DEV_IPCONV);
#endif

#endif

#if (SC_DEV_ISDN)

#if (SC_PH)
        dnt_add("ph", DEV_PH);
#endif

#if (SC_LAPD)
        dnt_add("q921", DEV_LAPD);
#endif

#if (SC_IPCD)
        dnt_add("ipcd", DEV_IPCD);
#endif

#endif

#if (SC_DEV_MLPP)

#if (SC_FRMUX)
        dnt_add("cfmux", DEV_FRMUX);
#endif

#if (SC_PIM)
        dnt_add("cpim", DEV_PIM);
#endif

#if (SC_PPP)
        dnt_add("cppp", DEV_PPP);
#endif

#endif
    /*-----------------------------------------------------------------*/
    /* Names for additional drivers may be added here.                 */
    /*-----------------------------------------------------------------*/

#endif /* SC_INIT_DNTENT */
}
#endif /* SC_PSOS || SC_PSOSM */

#if (SC_PNA || SC_PNET)
/***********************************************************************/
/*     SetUpNI: Set up Network Interfaces for pNA+                     */
/*                                                                     */
/*       INPUT: FreeMemPtr = A pointer to unused memory that can be    */
/*                           used to allocate space for a driver.      */
/*                           NOTE: Be sure to advance FreeMemPtr if    */
/*                           memory is allocates!                      */
/*                                                                     */
/*     RETURNS: The new address of free memory after the drivers have  */
/*              allocated space.                                       */
/*                                                                     */
/*       NOTES: This function does many precompile checks so errors    */
/*              can be found during compile-time rather then at run-   */
/*              time.                                                  */
/*                                                                     */
/*              Also, the Network Interface Table must be allocated    */
/*              before this function can be called.  There cannot be   */
/*              more then NC_NNI entries in this table.  NC_NNI is set */
/*              in sys_conf.h.  Before adding another interface, be    */
/*              sure to check sys_conf.h to see if NC_NNI is set to    */
/*              accommodate another one!                               */
/*                                                                     */
/*              See the pSOSystem Programmers Reference Manual         */
/*              for more information.                                  */
/*                                                                     */
/* To add a Network driver to pSOSystem, call InstallNi().  This adds  */
/* the Network Interface to the pNA+ Network Interface Table.          */
/* InstallNi() takes the following arguments:                          */
/*                                                                     */
/*     int (*entry)();        address of NI entry point                */
/*     int ipadd;             IP address                               */
/*     int mtu;               maximum transmission length              */
/*     int hwalen;            length of hardware address               */
/*     int flags;             intErface flags                          */
/*     int subnetaddr;        subnet mask                              */
/*     int dstipaddr;         destination ip address                   */
/*                                                                     */
/* See pSOSystem Programmer`s Reference Manual, Configuration Tables   */
/* Section, for more information about the Network Interface Table.    */
/* InstallNi() does not return a value.                                */
/*                                                                     */
/* If you are adding a driver that needs to be initialized before      */
/* either pSOS+ is initialized or the driver's init function is        */
/* called, then you can call a setup function (that you create) for    */
/* the driver (for example, see the CnslSetup call for the serial      */
/* device driver).  If your initialization function needs to allocate  */
/* memory, you may pass it the FreeMemPtr.  The function should return */
/* a new FreeMemPtr which points to the end of the memory it has       */
/* allocated.                                                          */
/*                                                                     */
/* When adding code to install a new driver, it is a good idea to add  */
/* precompile checks so errors can be found during compile-time rather */
/* then at run-time.  Precompile checks are used, for example, in the  */
/* code that installs the pSOSystem serial device driver below.        */
/*                                                                     */
/***********************************************************************/
UCHAR *SetUpNI(UCHAR *FreeMemPtr)
{
int IPaddr;

/*---------------------------------------------------------------------*/
/* Install the pSOSystem NI.                                           */
/*---------------------------------------------------------------------*/
#if BSP_LAN1
    if (SysVars.Lan1)
        {
        /*-------------------------------------------------------------*/
        /* If RARP is specified for the IP address, use it now to      */
        /* obtain the IP address for this interface.                   */
        /*-------------------------------------------------------------*/
        if (SysVars.Lan1IP == USE_RARP)
            {
            #if (SC_DEV_DLPI && SE_SHARE_NI)
                IPaddr = RarpEth((long (*)())DlpiEnet);
            #else
                IPaddr = RarpEth((long (*)())BSP_LAN1_ENTRY);
            #endif
            if (IPaddr == 0)
                SysInitFail(RarpErrTimeout);
            if (IPaddr == -1)
               SysInitFail(RarpErrNet);
            }
        else
            IPaddr = SysVars.Lan1IP;

        #if (SC_DEV_DLPI && SE_SHARE_NI)
            InstallNi((long (*)())DlpiEnet, IPaddr,
        #else
            InstallNi((long (*)())BSP_LAN1_ENTRY, IPaddr,
        #endif
                  BSP_LAN1_MTU, BSP_LAN1_HWALEN, BSP_LAN1_FLAGS,
                  SysVars.Lan1SubnetMask, 0);
        }
#endif

/*---------------------------------------------------------------------*/
/* Install the shared memory NI.                                       */
/*---------------------------------------------------------------------*/
#if BSP_SMEM
    #if ((SC_NISM_LEVEL != 1) && (SC_NISM_LEVEL != 2))
        #error "Illegal value for SC_NISM_LEVEL"
    #endif

    if (SysVars.Nism)
        {
        InstallNi((long (*)())NiSmem, SysVars.NismIP, NISM_MTU, 4, 0x8002,
                  SysVars.NismSubnetMask, 0);
        }
#endif
/*---------------------------------------------------------------------*/
/* Additional Network drivers may be installed here.                   */
/*---------------------------------------------------------------------*/

return FreeMemPtr;
}

#endif /* (SC_PNA || SC_PNET) */

#if SC_INIT_DNTABLE
/***********************************************************************/
/* GenerateString: A feature limited sprintf clone.                    */
/*                                                                     */
/*      INPUTS: dest   = character array to hold the output.           */
/*              format = point to the format string.                   */
/*              ...    = zero or more unsigned longs to be used for    */
/*                       %x output.                                    */
/*     RETURNS: none                                                   */
/*     OUTPUTS: none                                                   */
/*     NOTE(S): This is not a full clone of sprintf. Only the only     */
/*              two conversions supported in the format string are %x  */
/*              and %.<1-8>x . <1-8> means a single digit in the range */
/*              1 through 8. Also unlike a real sprintf the number of  */
/*              digits output is exactlyspecified by .<digit>, not the */
/*              minimum number. For example:                           */
/*                      GenerateString(MyDest, "%.1x", 0x1234);        */
/*              will only place the single digit '4' into the          */
/*              destination string (and the null terminator). %x is    */
/*              the same as %.1x .                                     */
/*                                                                     */
/*              The result of specifying a '%' without a valid         */
/*              conversion type following it is unspecified.           */
/*                                                                     */
/***********************************************************************/
static long GenerateString(char *dest, char *format, ...)
{
register ULONG value, position;
register char ch;
va_list ap;
const char NibbleToHex[17] = "0123456789abcdef";

va_start(ap, format);
while ((ch = *format++) != NULL)
    {
    if (ch != '%')
        {
        /*-------------------------------------------------------------*/
        /* If the current character isn't a '%' just copy it.          */
        /*-------------------------------------------------------------*/
        *dest++ = ch;
        }
    else
        {
        /*-------------------------------------------------------------*/
        /* The current character is a '%'. See if it is one of the     */
        /* supported formatting options.                               */
        /*-------------------------------------------------------------*/
        switch (ch = *format++)
            {
            case 'x':
                /*-----------------------------------------------------*/
                /* Output a single nibble.                             */
                /*-----------------------------------------------------*/
                value = va_arg(ap, ULONG);
                *dest++ = NibbleToHex[value & 0xf];
                break;

            case '.':
                /*-----------------------------------------------------*/
                /* Have %. so far. See if the next characters are a    */
                /* digit (1 through 8) and an 'x'.                     */
                /*-----------------------------------------------------*/
                if ((ch = *format) == 0)
                    break;

                position = ch - '0';
                ch = *(++format);

                if ((position < 1) || (position > 8) ||
                    (ch == 0) || (ch != 'x'))
                    break;

                /*-----------------------------------------------------*/
                /* Output the requested number of nibbles, from most   */
                /* to least significant.                               */
                /*-----------------------------------------------------*/
                value = va_arg(ap, ULONG);
                while (position-- > 0)
                    *dest++ = NibbleToHex[(value>>(position<<2)) & 0xf];

                format++;
                break;

            default:
                if (ch != 0)
                    *dest++ = ch;
                else
                    format--;
                break;
            }
        }
    }
*dest = 0;
va_end(ap);
}
#endif /* SC_INIT_DNTABLE */

