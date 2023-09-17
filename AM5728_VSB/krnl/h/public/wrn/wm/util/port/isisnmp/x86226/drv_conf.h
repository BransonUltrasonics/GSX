/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/isisnmp/x86226/drv_conf.h,v 1.1.1.1 2001/11/05 17:47:11 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: drv_conf.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:11  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:23:00  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:10:13  meister
 * Update copyright message
 *
 * Revision 1.3  1998/08/20 18:01:21  josh
 * sys_conf.h cleanup
 *
 * Revision 1.2  1998/08/19 21:38:34  josh
 * adding copyright stuff.
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/


/* @(#) pSOSystem x86/V2.2.6: apps.v22/xxxx/drv_conf.c 3.3 (snmpv2) 98/01/09 12:28:00 */
/***********************************************************************/
/*                                                                     */
/*   MODULE:  drv_conf.c                                               */
/*   DATE:    98/01/09                                                 */
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
#if SC_DEV_SPI
    #include "spi.h"
#endif
#if (SC_PSE)
#include <psecfg.h>
#include <sys/stream.h>
#endif
#if (SC_DEV_DLPI)
extern long DlpiEnet();
#endif

#define NULLF (ULONG) ((void (*)()) 0)

void InstallNi(int (*entry)(), int, int, int, int, int, int);

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
/*     unsigned long rsvd1,           reserved                         */
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
#endif

/*---------------------------------------------------------------------*/
/* Install Pseudo driver                                               */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_PSEUDO)
    InstallDriver(SC_DEV_PSEUDO, PseudoIOInit, NULLF, NULLF,
                  PseudoIORead, PseudoIOWrite, PseudoIOCntrl, 0, 0, 0);
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
    InstallDriver(SC_DEV_RAMDISK, RdskInit, NULLF, NULLF, RdskRead,
                  RdskWrite, NULLF, 0, 0, 0);
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
    InstallDriver(SC_DEV_SCSI, SdrvInit, NULLF, NULLF, SdskRead,
                  SdskWrite, SdrvCntrl, 0, 0, 0);
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
#endif

/*---------------------------------------------------------------------*/
/* Install pSE DLPI driver                                             */
/* NOTE: This driver must be called from a task it cannot use the      */
/*       auto initialization freature of pSOS+                         */
/*---------------------------------------------------------------------*/
#if (SC_DEV_DLPI)
  {
    #if(SC_DEV_DLPI > SC_DEVMAX)
        #error "SC_DEV_DLPI cannot be > SC_DEVMAX"
    #endif
    extern long DlpiNullNI();
    extern UCHAR *DlpiInit(UCHAR *, long, long);
    extern int DlpiAddNI(long, long(*)(), long, long, char *, long);
    extern void enetinit();
    extern struct streamtab enetinfo;
    char *DlpiErrMsg = "DlpiAddNI failed: NI Error";
    static const psedrvparam_t eth0param = {"eth0", DT_CLONE,
                                             &enetinfo, 0, 0};

    InstallDriver(SC_DEV_DLPI, enetinit, NULLF, NULLF, NULLF,
                  NULLF, NULLF, (long)&eth0param, 0, 0);

    /*-------------------------------------------------------------*/
    /* Call DlpiInit to initialize the Streams DLPI driver         */
    /*   Arguments passed to DlpiInit are:                         */
    /*     1. Free memory address                                  */
    /*     2. Maximum NI interface number                          */
    /*     3. Maximum number of streams per interface              */
    /*-------------------------------------------------------------*/
    FreeMemPtr = DlpiInit(FreeMemPtr, 1, 6);

    /*-------------------------------------------------------------*/
    /* Call DlpiAddNI to add an NI interface to the Streams DLPI   */
    /* driver.  DlpiAddNI must be called for each NI driver        */
    /* interface used with the Streams DLPI driver.                */
    /*   Arguments passed to DlpiAddNI are:                        */
    /*     1. NI interface number (should correspond to the pNA+   */
    /*          interface number if driver is shared with pNA+)    */
    /*     2. NI entry point                                       */
    /*     3. NI flags                                             */
    /*          IFF_SELFLOOP 0x0001  loopback for self-address     */
    /*          IFF_POLL_NI  0x0002  driver must be polled         */
    /*          IFF_RAWMEM   0x2000  driver accepts mblk link list */
    /*     4. NI major device number                               */
    /*     5. NI multicast address                                 */
    /*     6. NI poll frequency (every # of elapsed ticks)         */
    /*-------------------------------------------------------------*/
    #if (BSP_LAN1 && SE_SHARE_NI)
        if (DlpiAddNI(1, (long(*)())BSP_LAN1_ENTRY, 0x2000,
                SC_DEV_DLPI, 0, 0))
            SysInitFail(DlpiErrMsg);
    #else
        if (DlpiAddNI(0, (long(*)())DlpiNullNI, 0, SC_DEV_DLPI, 0, 0))
            SysInitFail(DlpiErrMsg);
    #endif
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
#endif

/*---------------------------------------------------------------------*/
/* Additional drivers may be installed here.                           */
/*---------------------------------------------------------------------*/

return FreeMemPtr;
}
#endif /* SC_PSOS || SC_PSOSM */

#if SC_PNA
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
            InstallNi((int (*)())DlpiEnet, IPaddr,
        #else
            InstallNi((int (*)())BSP_LAN1_ENTRY, IPaddr,
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
        InstallNi((int (*)())NiSmem, SysVars.NismIP, NISM_MTU, 4, 0x8002,
                  SysVars.NismSubnetMask, 0);

        /*-------------------------------------------------------------*/
        /* Set up the NI Shared Memory driver by calling its function  */
        /*  "NiSmemSetup".                                             */
        /*-------------------------------------------------------------*/
        FreeMemPtr = NiSmemSetup(SC_NISM_BUFFS, FreeMemPtr,
                                 SC_NISM_LEVEL);
        }
#endif
/*---------------------------------------------------------------------*/
/* Additional Network drivers may be installed here.                   */
/*---------------------------------------------------------------------*/

return FreeMemPtr;
}

#endif
