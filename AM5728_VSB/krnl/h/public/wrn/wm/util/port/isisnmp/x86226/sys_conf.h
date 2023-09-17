/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/isisnmp/x86226/sys_conf.h,v 1.1.1.1 2001/11/05 17:47:11 tneale Exp $ */

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
 * $Log: sys_conf.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:11  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:00  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:13  meister
 * Update copyright message
 *
 * Revision 1.2  1998/08/19 21:38:35  josh
 * adding copyright stuff.
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/


/* @(#) pSOSystem x86/V2.2.6: apps.v22/xxxx/sys_conf.h (snmpv2 386) 3.21 98/01/12 10:03:13 */
/***********************************************************************/
/*                                                                     */
/*   MODULE:  sys_conf.h                                               */
/*   DATE:    98/01/12                                                 */
/*   PURPOSE: pSOSystem configuration definitions                      */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*           Copyright 1991 - 1997, Integrated Systems, Inc.           */
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
/*   This is a header file which defines all of the system-level       */
/*   parameters that are likely to need changing.                      */
/*                                                                     */
/*   It has been set up to be as close as possible to the settings     */
/*   which are appropriate for this application.  However, you may     */
/*   need to make some changes to parameters to reflect your           */
/*   environment, such as IP addresses, etc.                           */
/*                                                                     */
/*   Chapter 7 of the pSOSystem Getting Started manual contains a      */
/*   complete description of the symbols that are defined in this      */
/*   file.                                                             */
/*                                                                     */
/***********************************************************************/
#if __cplusplus
extern "C" {
#endif

#ifndef _SYS_CONF_H
#define _SYS_CONF_H

/***********************************************************************/
/*                                                                     */
/* SC_APP_PARMS Must be defined before you include sysvars.h file.     */
/* Since "sysvars.h" file depends on SC_APP_PARMS and BSP_PARMS        */
/* to define the Storage Parms Structure.                              */
/*                                                                     */
/* Defining this variable to a Non-Zero Value will create a space in   */
/* the Storage Parms. structure for your application use.              */
/*                                                                     */
/***********************************************************************/
#define SC_APP_PARMS 0
#define SC_APP_NAME  "snmpv2 Application"

/*---------------------------------------------------------------------*/
/* types.h contains common definitions for example YES and NO          */
/*---------------------------------------------------------------------*/
#include <types.h>

/***********************************************************************/
/* sysvars.h contains the structures and definitions for the system    */
/* variables that are stored in NVRAM. These structures and            */
/* definitions are also used when creating the boot rom for the        */
/* target system. By using the same sysvars.h file it will insure the  */
/* consistency of the values stored by the boot rom and later read by  */
/* the downloaded application.                                         */
/***********************************************************************/
#include <sysvars.h>

/*---------------------------------------------------------------------*/
/* include psos.h for defines IO_AUTOINIT and IO_NOAUTOINIT            */
/*---------------------------------------------------------------------*/
#include <psos.h>

/***********************************************************************/
/*                                                                     */
/*  These symbols are used to define other symbols in this file.       */
/*  They should never be changed.                                      */
/*                                                                     */
/***********************************************************************/
#define USE_RARP 0


/***********************************************************************/
/*                                                                     */
/*                 B A S I C   P A R A M E T E R S                     */
/*                                                                     */
/*  The parameters in this section determine the fashion in which      */
/*  many of the other parameters in this file will be used.            */
/*                                                                     */
/*  Many of the parameters in this file have names beginning with      */
/*  either "SC_" or "SD_".  Those beginning with "SC_" are ALWAYS      */
/*  controlled by the values you set in this file.  The values of      */
/*  the "SD_" parameters can be determined either by the definitions   */
/*  given in this file, or by the data found in the target board's     */
/*  parameter storage area.  SC_SD_PARAMETERS determines this.  If     */
/*  SC_SD_PARAMETERS is "SYS_CONF", then the values given in this      */
/*  file will be used for the SD_ parameters.  If SC_SD_PARAMETERS     */
/*  is "STORAGE", then pSOSystem will attempt to use the values found  */
/*  in the board's parameter storage area for the SD_ variables.  If   */
/*  the parameter storage area has not been initialized or has been    */
/*  corrupted, then the values given in this file will be used.  They  */
/*  will also be written to the storage area.                          */
/*                                                                     */
/*  If SC_SD_PARAMETERS is STORAGE, you may enable a "startup dialog"  */
/*  which will allow you to view, and optionally change, the           */
/*  parameter values found in the storage area.  The dialog is enabled */
/*  by setting SC_STARTUP_DIALOG to YES.  In this case,                */
/*  SD_STARTUP_DELAY specifies the number of seconds that the dialog   */
/*  will wait for input before proceeding to boot the system.          */
/*                                                                     */
/* SC_SD_DEBUG_MODE determines how the system will operate, as follows:*/
/*                                                                     */
/*     DBG_SA: Boot pROBE+ in standalone mode.                         */
/*                                                                     */
/*     DBG_XS: Boot into pROBE+ and wait for the host debugger         */
/*             via a serial connection                                 */
/*                                                                     */
/*     DBG_XN: Boot into pROBE+ and wait for the host debugger         */
/*             via a network connection                                */
/*                                                                     */
/*     DBG_AP: Same as DBG_SA, but also does a pROBE+ "silent startup",*/
/*             meaning that pROBE+ initializes itself without          */
/*             printing a startup banner.  pROBE+ then initializes     */
/*             pSOS+ and starts the application running.   This mode   */
/*             was created to simplify implementation of the pSOSystem */
/*             Boot ROMs.  Note that when this mode is used, the value */
/*             you specify for RC_SMODE (see "pROBE+ Configuration     */
/*             Parameters", below) is overridden.                      */
/*                                                                     */
/*    STORAGE: Use the mode (DBG_SA, DBG_XS, or DBG_XN) found in the   */
/*             parameter storage area.  If a valid mode is not found,  */
/*             then use DBG_SA.                                        */
/*                                                                     */
/*  An important consequence of using SC_SD_DEBUG_MODE=STORAGE should  */
/*  be noted.  If you use the TFTP bootloader in the pSOSystem ROMs    */
/*  to download and start your system, you will be setting             */
/*  SC_SD_DEBUG_MODE = DBG_AP in your target board's parameter storage */
/*  area.  Thus, if you have left SC_SD_DEBUG_MODE set to STORAGE in   */
/*  this file, your downloaded system will also use DBG_AP, meaning    */
/*  that the application will begin running immediately after the      */
/*  TFTP bootloader has loaded and started your system.                */
/*                                                                     */
/***********************************************************************/
#define SC_SD_PARAMETERS    STORAGE
#define SC_STARTUP_DIALOG   NO
#define SC_BOOT_ROM         NO
#define SD_STARTUP_DELAY    60
#define SC_SD_DEBUG_MODE    STORAGE

/***********************************************************************/
/*                                                                     */
/*        O P E R A T I N G   S Y S T E M   C O M P O N E N T S        */
/*                                                                     */
/*  Setting the definition line to YES will cause that component to    */
/*  be built into the system.  NO means the component will NOT be      */
/*  put into the system.  It is an error to specify both SC_PSOS       */
/*  and SC_PSOSM as present!                                           */
/*                                                                     */
/***********************************************************************/
#define SC_PSOS             YES  /* pSOS+ real-time kernel             */
#define SC_PSOSM             NO  /* pSOS+ real-time multiproc kernel   */
#define SC_PROBE            YES  /* pROBE+ (processor svcs)            */
#define SC_PROBE_DISASM     YES  /* pROBE+ (disassembler)              */
#define SC_PROBE_DASM_INTEL  NO  /* pROBE+ (Intel format disassembler) */
                                 /* for x86 Targets Only               */
#define SC_PROBE_CIE        YES  /* pROBE+ (console executive)         */
#define SC_PROBE_QUERY      YES  /* pROBE+ (query services)            */
#define SC_PROBE_DEBUG      YES  /* pROBE+ (debug interface executive) */
#define SC_PHILE            YES  /* pHILE+ file system manager         */
#define SC_PREPC            YES  /* pREPC+ C run-time library          */
#define SC_PNA              YES  /* pNA+ TCP/IP networking manager     */
#define SC_PRPC             YES  /* pRPC+ RPC component                */
#define SC_PSE               NO  /* pSE+ streams component             */
#define SC_PSKT              NO  /* pSKT+ SKT library component        */
#define SC_PTLI              NO  /* pTLI+ TLI library component        */
#define SC_PMONT             NO  /* pMONT+                             */
#define SC_PROFILER          NO  /* Profiler is enabled/Disabled       */

/*---------------------------------------------------------------------*/
/* If set to IO_AUTOINIT device drivers that are installed be the      */
/* InstallDriver function will have their autoinit field set. This     */
/* will cause pSOS+ call the drivers initialization function when      */
/* pSOS+ starts up. The de_init call will not have to be called for    */
/* each driver if this is done.                                        */
/*                                                                     */
/* NOTE: This Auto Initialization will not work on all drivers.        */
/* Setting IO_AUTOINIT only effects drivers that can use the Auto      */
/* Initialization feature.                                             */
/*                                                                     */
/* If set to IO_NOAUTOINIT no automatic driver initialization will     */
/* happen.                                                             */
/*---------------------------------------------------------------------*/
#define SC_AUTOINIT         IO_AUTOINIT

/*---------------------------------------------------------------------*/
/* If SC_PMONT SC_AUTOINIT will always be set to 1 because pMONT needs */
/* drivers to be automatically initialized.                            */
/*---------------------------------------------------------------------*/
#if SC_PMONT
    #undef  SC_AUTOINIT
    #define SC_AUTOINIT         IO_AUTOINIT
#endif

/***********************************************************************/
/*                                                                     */
/*      S E R I A L   C H A N N E L   C O N F I G U R A T I O N        */
/*                                                                     */
/*  * SD_DEF_BAUD specifies the default baud rate to be used for the   */
/*    serial channels                                                  */
/*  * SC_APP_CONSOLE specifies the serial channel number (starting     */
/*    from 1) which should be used for the application's "console".    */
/*  * SC_PROBE_CONSOLE specifies the serial channel number (starting   */
/*    from 1) which pROBE+ should use to display output and receive    */
/*    commands, or to communicate with the host debugger.              */
/*  * SC_PROBE_HOST specifies the serial channel number (starting      */
/*    from 1) which should be used for the pROBE+ "host" channel.      */
/*    This is not normally required, so it can be disabled by          */
/*    using a value of 0.  The host channel is explained in the        */
/*    pROBE+ User's Manual.                                            */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_BAUD           9600
#define SC_APP_CONSOLE        PCCON
#define SC_PROBE_CONSOLE      PCCON
#define SC_RBUG_PORT          0

/***********************************************************************/
/* SC_NumNon_pSOSChan is the number of non pSOS users of serial        */
/* channels. These are users that will be initiated before pSOS        */
/* such as pROBE.                                                      */
/* NOTE: These channels will not be closed on a soft reset.            */
/***********************************************************************/
#if (SC_RBUG_PORT && SC_PROBE_CONSOLE && (SC_PROBE_CONSOLE != PCCON))
#define SC_NumNon_pSOSChan 2
#else
#define SC_NumNon_pSOSChan 1
#endif

/***********************************************************************/
/*                                                                     */
/*              L A N   C O N F I G U R A T I O N                      */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define SD_LAN1               YES
#define SD_LAN1_IP            0x80e001b7
#define SD_LAN1_SUBNET_MASK   0xffffff00

/*---------------------------------------------------------------------*/
/* Maximum number of multicast addresses to be used by the lan         */
/* interface. Must not exceed maximum supported by the lan driver.     */
/*---------------------------------------------------------------------*/
#define SC_LAN1_NMCAST        0

/***********************************************************************/
/*                                                                     */
/*      S H A R E D   M E M O R Y   C O N F I G U R A T I O N          */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

/***********************************************************************/
/*                                                                     */
/*                B U S   C O N F I G U R A T I O N                    */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define SD_VME_BASE_ADDR      0x01000000

/*---------------------------------------------------------------------*/
/* If you want to build a Shared Memory Interface for the network or   */
/* multi-processing pSOS+m, set SD_SM_NODE (a non-zero value) to the   */
/* node number of THIS node.                                           */
/*---------------------------------------------------------------------*/
#define SD_SM_NODE            0

/*---------------------------------------------------------------------*/
/* Shared memory network interface - SC_NISM_LEVEL should be 1 if this */
/* will be the FIRST system to use the SMNI on this board.  It should  */
/* be 2 if this will not be the first system to use the SMNI on this   */
/* board.  Usually this means that SC_NISM_LEVEL should be 1 if you    */
/* are building Boot ROMs, and 2 for a system that will be downloaded  */
/* and started using the Boot ROMs.  See the comments at the beginning */
/* of drivers/ni_smem.c for more explanation.                          */
/*---------------------------------------------------------------------*/
#define SD_NISM               NO
#define SD_NISM_IP            0x00000000
#define SD_NISM_DIRADDR       0x00000400
#define SC_NISM_BUFFS         30
#define SC_NISM_LEVEL         2
#define SD_NISM_SUBNET_MASK   0


/*---------------------------------------------------------------------*/
/* If you want to build a multi-node pSOS+m system, set SD_KISM        */
/* to the number of nodes in the system.                               */
/*---------------------------------------------------------------------*/
#define SD_KISM               0
#define SD_KISM_DIRADDR       0x00000580


/***********************************************************************/
/*                                                                     */
/*         M I S C E L L A N E O U S   N E T W O R K I N G             */
/*                       P A R A M E T E R S                           */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_GTWY_IP        0


/***********************************************************************/
/* SC_RAM_SIZE                                                         */
/*                                                                     */
/* Normally, pSOSystem uses ALL of the memory remaining on a board for */
/* dynamic allocation ("region 0").  You may override this by setting  */
/* SC_RAM_SIZE to a non-zero value.  If you do, pSOSystem will not     */
/* touch any memory after address (BSP_RAM_BASE + SC_RAM_SIZE - 1).    */
/*                                                                     */
/* NOTE: This is useful when making a BOOT ROM and you want to ensure  */
/* that the ROM's RAM area does not overflow the space alloted for it. */
/***********************************************************************/
#define SC_RAM_SIZE           0

/***********************************************************************/
/*                                                                     */
/*                     I / O   D E V I C E S                           */
/*                                                                     */
/*  Each device may be included in the system by specifying a major    */
/*  number for it, which determines its slot in the pSOS+ I/O switch   */
/*  table.  To leave a device driver out of the system, use NO for     */
/*  the major number.                                                  */
/*                                                                     */
/*  Note the following:                                                */
/*    * Major device 0 is reserved and cannot be used for any of these */
/*      devices.  Setting a device number to 0 here is the same as NO. */
/*    * No device number may be higher than SC_DEVMAX (SC_DEVMAX may   */
/*      be increased, if desired)                                      */
/*    * The lines defining the symbols DEV_SERIAL, DEV_TIMER, etc,     */
/*      should not be changed.  These are for use by application       */
/*      programs as the "device number" parameter on de_* calls.       */
/*                                                                     */
/***********************************************************************/
#define SC_DEV_SERIAL         1         /* Serial driver               */
#define SC_DEV_TIMER          2         /* Periodic tick timer         */
#define SC_DEV_RAMDISK       NO         /* RAM disk                    */
#define SC_DEV_CONSOLE        4         /* PC-Console driver           */
#define SC_DEV_SCSI          NO         /* SCSI bus                    */
#define SC_DEV_SCSI_TAPE     NO         /* SCSI bus, tape device       */
#define SC_DEV_IDE           NO         /* IDE Driver                  */
#define SC_DEV_FLOPPY        NO         /* Floppy Driver               */
#define SC_DEV_TFTP          NO         /* TFTP pseudo driver          */
#define SC_DEV_SPI           NO         /* SPI driver                  */
#define SC_DEV_DLPI          NO         /* DLPI pseudo driver          */
#define SC_DEV_OTCP          NO         /* 12 TCP/IP for OpEN          */
#define SC_IP              SC_DEV_OTCP  /* 12 IP                       */
#define SC_ARP               NO         /* 13 ARP                      */
#define SC_TCP               NO         /* 14 TCP                      */
#define SC_UDP               NO         /* 15 UDP                      */
#define SC_RAW               NO         /* 16 RAW                      */
#define SC_LOOP              NO         /* 17 LOOP = (SC_DEV_OTCP + 5) */
#define SC_DEV_SOSI          NO         /* 18 OSI for OpEN             */
#define SC_DEV_PSEUDO        NO         /* PSEUDO driver               */
#define SC_DEV_PARALLEL      NO         /* Parallel Port Driver        */
#define SC_DEV_CMOS          NO         /* CMOS Driver                 */
#define SC_DEV_WATCHDOG      NO         /* Watchdog Driver             */
#define SC_DEV_OLAP          NO         /* LAP Driver                  */
#define SC_PHPI        (SC_DEV_OLAP)    /* Phpi driver                 */
#define SC_LAPB        (SC_DEV_OLAP + 0)/* LAPB driver Change 0 to 1   */
#define SC_CLAPB       (SC_DEV_OLAP + 0)/* LAPB driver Change 0 to 2   */
#define SC_DEV_OX25    (SC_DEV_OLAP + 0)/* LAPB driver Change 0 to 3   */
#define SC_X25         (SC_DEV_OX25)    /* X25 driver                  */
#define SC_CX25        (SC_DEV_OX25 + 0)/* X25 driver  Change 0 to 1   */
#define SC_SNDCF             NO         /* sndcf driver                */
#define SC_CSNDCF            NO         /* csndcf driver               */
#define SC_IPCONV            NO
#define SC_CIPCONV           NO

#define SC_DEVMAX            32         /* Maximum device number       */

#define DEV_SERIAL      (SC_DEV_SERIAL     << 16)
#define DEV_PARALLEL    (SC_DEV_PARALLEL   << 16)
#define DEV_TIMER       (SC_DEV_TIMER      << 16)
#define DEV_RAMDISK     (SC_DEV_RAMDISK    << 16)
#define DEV_SCSI        (SC_DEV_SCSI       << 16)
#define DEV_SCSI_TAPE   (SC_DEV_SCSI_TAPE  << 16)
#define DEV_PSEUDO      (SC_DEV_PSEUDO     << 16)
#define DEV_DLPI        (SC_DEV_DLPI       << 16)
#define DEV_TFTP        (SC_DEV_TFTP       << 16)
#define DEV_SPI         (SC_DEV_SPI        << 16)
#define DEV_WATCHDOG    (SC_DEV_WATCHDOG   << 16)
#define DEV_FLOPPY      (SC_DEV_FLOPPY     << 16)
#define DEV_IDE         (SC_DEV_IDE        << 16)
#define DEV_CMOS        (SC_DEV_CMOS       << 16)
#define DEV_CONSOLE     (SC_DEV_CONSOLE    << 16)

#if (BSP_CPUFAMILY == FAMILY_X86) && (SC_APP_CONSOLE == PCCON)
    #define CONSOLE   DEV_CONSOLE
#else
    #define CONSOLE   DEV_SERIAL
#endif

/***********************************************************************/
/*                                                                     */
/* M E M O R Y   M A N A G E M E N T   L I B R A R Y   P A R A M S     */
/*                                                                     */
/***********************************************************************/
#define SC_MMULIB  NO

/***********************************************************************/
/*                                                                     */
/* C O M P O N E N T   C O N F I G U R A T I O N   P A R A M E T E R S */
/*                                                                     */
/*  These parameters should work as is for this application.  You      */
/*  may want to change some of them if you start customizing this      */
/*  application.                                                       */
/*                                                                     */
/***********************************************************************/

/*---------------------------------------------------------------------*/
/* pSOS+ configuration parameters                                      */
/*---------------------------------------------------------------------*/
#define KC_RN0USIZE    0x100    /* region 0 unit size                  */
#define KC_NTASK       25       /* max number of tasks                 */
#define KC_NQUEUE      10       /* max number of message queues        */
#define KC_NSEMA4      30       /* max number of semaphores            */
#define KC_NMSGBUF     256      /* max number of message buffers       */
#define KC_NTIMER      10       /* max number of timers                */
#define KC_NLOCOBJ     50       /* max number of local objects         */
#define KC_TICKS2SEC   100      /* clock tick interrupt frequency      */
#define KC_TICKS2SLICE 10       /* time slice quantum, in ticks        */
#define KC_INTSTK      0x2000   /* interrupt stack size                */
#define KC_SYSSTK      0x2000   /* pSOS+ system stack size (bytes)     */
#define KC_ROOTSSTK    0x4000   /* ROOT supervisor stack size          */
#define KC_ROOTUSTK    0        /* ROOT user stack size                */
#define KC_ROOTMODE    (T_SUPV | T_ISR) /* ROOT initial mode           */
#define KC_ROOTPRI     230      /* ROOT task priority                  */

/*---------------------------------------------------------------------*/
/* The following are examples for modifying the following defines      */
/*                                                                     */
/* Using a pSOSystem routine as a fatal error handler                  */
/* #define KC_FATAL    ((void (*)()) SysInitFail)                      */
/*                                                                     */
/* Using a user written routine as a fatal error handler               */
/* extern void MyHandler (void);                                       */
/* #define KC_FATAL    ((void (*)()) MyHandler)                        */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define KC_STARTCO     0        /* callout at task activation          */
#define KC_DELETECO    0        /* callout at task deletion            */
#define KC_SWITCHCO    0        /* callout at task switch              */

#define KC_FATAL       0        /* fatal error handler address         */
#define KC_IDLECO      0        /* Idle Task Callout                   */
#define KC_RTCINITCO   0        /* RTC INIT Callout                    */

/*---------------------------------------------------------------------*/
/* pSOS+m configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define MC_NGLBOBJ      8       /* size of gbl obj table               */
#define MC_NAGENT      10       /* number of RPC agents in this node   */
#define MC_FLAGS       SEQWRAP_ON /* operating mode flags              */

/*---------------------------------------------------------------------*/
/* The following are examples for modifying the following definition   */
/*                                                                     */
/* Using a pSOSystem routine as a roster change handler                */
/* #define MC_ROSTER   ((void (*)()) SysInitFail)                      */
/*                                                                     */
/* Using a user written routine as a roster change handler             */
/* extern void MyHandler (void);                                       */
/* #define MC_ROSTER   ((void (*)()) MyHandler)                        */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define MC_ROSTER       0       /* addr of user roster change callout  */
#define MC_KIMAXBUF   100       /* maximum KI packet buffer length     */
#define MC_ASYNCERR     0       /* asynchronous calls error callout    */

/*---------------------------------------------------------------------*/
/* pROBE configuration parameters - note that the SMODE flag bits are  */
/* irrelevant if SC_SD_DEBUG_MODE = DBG_AP.                            */
/*---------------------------------------------------------------------*/
#define TD_BRKOPC      0xCC     /* Instruction break opcode            */
#define TD_ILEV        (1 << 12)/* pROBE+ interrupt mask               */
#define TD_FLAGS       (NODOTS_MASK | TD_ILEV | NOTUPD_MASK)/* flags   */
#define TD_DBGPRI      244      /* Debugger task priority              */

/*---------------------------------------------------------------------*/
/* The variable defined down should be set to the Starting address for */
/* for pROBE+ DATA AREA. This is required by all the smaller boards.   */
/* NOTE:                                                               */
/*       Setting of this value should have atleast 0x4400 bytes Free   */
/*       Space from that location.                                     */
/*---------------------------------------------------------------------*/
#define TD_DATASTART   0x0

/*---------------------------------------------------------------------*/
/* pHILE+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define FC_LOGBSIZE    9        /* Block size (base-2 exponent)        */
#define FC_NBUF        6        /* Number of cache buffers             */
#define FC_NMOUNT      3        /* Max # of mounted volumes            */
#define FC_NFCB        10       /* Max # of opened files per system    */
#define FC_NCFILE      2        /* Max # of opened files per task      */
#define FC_NDNLC       0        /* Max # of cached directory entries   */
#define FC_MSDOS       1        /* MS-DOS volume mount flag            */
#define FC_CDROM       0        /* CD-ROM volume mount flag            */

/*---------------------------------------------------------------------*/
/* pREPC+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define LC_BUFSIZ      128      /* I/O buffer size                     */
#define LC_NUMFILES    5        /* Max number of open files per task   */
#define LC_WAITOPT     0        /* Wait option for memory allocation   */
#define LC_TIMEOPT     0        /* Timeout option for mem allocation   */
#define LC_SSIZE       512      /* Size of print buffer                */

/*---------------------------------------------------------------------*/
/* pNA+ configuration parameters                                       */
/*                                                                     */
/* NOTE: If you get pNA+ errno 5F01 FAT_INSUFFMEM you may need to      */
/*       decrease the number of buffers allocated here for pNA+.       */
/*       In general this error means pNA+ could not allocate enough    */
/*       memory from Region 0 for the number of buffers requested.     */
/*---------------------------------------------------------------------*/
#define NC_NNI         5        /* size of pNA NI Table                */
#define NC_NROUTE      10       /* size of pNA Routing Table           */
#define NC_NARP        20       /* size of pNA ARP table               */
#define NC_DEFUID      0        /* Default User ID of a task           */
#define NC_DEFGID      0        /* Default Group ID of a task          */
#define NC_HOSTNAME    "scg"    /* Hostname of the node                */
#define NC_NHENTRY     8        /* Number of Host table entries        */

#define NC_NMCSOCS     0        /* Number of IP multicast sockets      */
#define NC_NMCMEMB     0        /* Number of distict IP multicast      */
                                /* group memberships per interface     */
 
#define NC_NNODEID     0        /* Network NODE ID for unnumbered link */

#define NC_NSOCKETS  128        /* Number of sockets in the system     */
#define NC_NDESCS    128        /* # of socket descriptors/task        */
#define NC_MBLKS     300        /* # of message blocks in the system   */

#define NC_BUFS_0     64        /* number of 0 length buffers          */
#define NC_BUFS_32     0        /* number of 32 length buffers         */
#define NC_BUFS_64     0        /* number of 64 length buffers         */
#define NC_BUFS_128  256        /* number of 128 byte buffers          */
#define NC_BUFS_256    0        /* number of 256 byte buffers          */
#define NC_BUFS_512    0        /* number of 512 byte buffers          */
#define NC_BUFS_1024  16        /* number of 1k byte buffers           */
#define NC_BUFS_2048  48        /* number of 2k byte buffers           */
#define NC_BUFS_4096   0        /* number of 4k byte buffers           */

#define NC_MAX_BUFS    9        /* max number of NC_BUFS types         */

/*---------------------------------------------------------------------*/
/* The following are examples for modifying the following defines      */
/*                                                                     */
/* Using a user written routine as a pNA+ Signal Handler               */
/* extern void MySigHandler (void);                                    */
/* #define NC_SIGNAL   ((void (*)()) MySigHandler)                     */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define NC_SIGNAL      0                           /* pNA+ Signal Hdlr */

/*---------------------------------------------------------------------*/
/* pSE+ configuration parameters                                       */
/*---------------------------------------------------------------------*/
#define SE_NBUFS_0     64       /* number of 0 length buffers          */
#define SE_NBUFS_32    128      /* number of 32 byte buffers           */
#define SE_NBUFS_64    128      /* number of 64 byte buffers           */
#define SE_NBUFS_128   256      /* number of 128 byte buffers          */
#define SE_NBUFS_256   32       /* number of 256 byte buffers          */
#define SE_NBUFS_512   32       /* number of 512 byte buffers          */
#define SE_NBUFS_1024  16       /* number of 1k byte buffers           */
#define SE_NBUFS_2048  16       /* number of 2k byte buffers           */
#define SE_NBUFS_4096  8        /* number of 4k byte buffers           */
#define SE_MAX_BUFS    9        /* max number of stream buffer types   */

#define SE_MAX_MODULES 3        /* max number of streams modules       */
#define SE_SHARE_NI    NO       /* LAN-NI with streams?                */

#define SE_DATA_SIZE   0x0C00   /* size of pSE data area               */
                                /* (must be at least 3K)               */
#define SE_TASK_PRIO   250      /* priority for pSE task               */
#define SE_STACK_SIZE  0x2000   /* stack size for pSE task             */
#define SE_DEF_UID     0        /* default user id                     */
#define SE_DEF_GID     0        /* default group id                    */
#define SE_N_FDS       32       /* max # of system-wide                */
                                /*  stream descriptors                 */
#define SE_N_TASKFDS   8        /* max # of per-task stream            */
                                /* descriptors                         */
#define SE_N_LINKS    32        /* max # of multiplexing links         */
#define SE_N_TIMEOUTS 32        /* max # of timeout requests           */
#define SE_N_BUFCALLS 32        /* max # of bufcall requests           */
#define SE_N_QUEUES   256       /* # of queues                         */
#define SE_N_MBLKS    512       /* reserved for future use, must be 0  */

/*---------------------------------------------------------------------*/
/* Loader configuration parameters                                     */
/*                                                                     */
/* YES indicates for each module to be linked in. NO means do not link */
/* object module loader into this application.                         */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define LD_MAX_LOAD      8      /* Max. # of active loads              */
#define LD_ELF_MODULE    NO     /* ELF object-load-module              */
#define LD_SREC_MODULE   NO     /* SREC object-load-module             */
#define LD_COFF_MODULE   NO     /* COFF object-load-module             */
#define LD_IEEE_MODULE   NO     /* IEEE object-load-module             */
#define LD_IHEX_MODULE   NO     /* IHEX object-load-module             */

/*---------------------------------------------------------------------*/
/* pMONT+ configuration parameters - note that PM_DEV and PM_BAUD are  */
/* only relevant to serial communication (PM_CMODE = 2)                */
/*---------------------------------------------------------------------*/
#define PM_CMODE         1      /* 1 = networking, 2 = serial          */
#define PM_DEV           2      /* Minor Device number  for            */
                                /* serial channel if used              */
#define PM_BAUD          9600   /* Baud rate for serial chnl           */
#define PM_TRACE_BUFF    0      /* Address of trace buffer --          */
                                /* Allocated by pSOSystem if 0         */
#define PM_TRACE_SIZE    0x6000 /* Size of trace buffer                */
#define PM_TIMER         NO     /* 2nd timer for finer time            */
                                /* within data collection              */

/*---------------------------------------------------------------------*/
/* General Serial Block Configuration Parameters                       */
/*---------------------------------------------------------------------*/
#define GS_BUFS_0     64        /* number of 0 length buffers          */
#define GS_BUFS_32    128       /* number of 32 byte buffers           */
#define GS_BUFS_64    128       /* number of 64 byte buffers           */
#define GS_BUFS_128   256       /* number of 128 byte buffers          */
#define GS_BUFS_256   32        /* number of 256 byte buffers          */
#define GS_BUFS_512   32        /* number of 512 byte buffers          */
#define GS_BUFS_1024  16        /* number of 1k byte buffers           */
#define GS_BUFS_2048  16        /* number of 2k byte buffers           */
#define GS_BUFS_4096  8         /* number of 4k byte buffers           */

#define GS_MBLKS      680
#define GS_MAX_BUFS    9        /* max number of serial buffer types   */

/*---------------------------------------------------------------------*/
/* TCP/IP for OpEN configuration parameters                            */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* Configurable parameters for the Network layer (IP)                  */
/*---------------------------------------------------------------------*/
#define OTCP_IPMAX              8       /* Max # of open IP device     */
#define OTCP_IPNINTERFACES      4       /* Max # of network interfaces */
#define OTCP_IPNROUTES  64              /* Max #of routing tbl entries */

/*---------------------------------------------------------------------*/
/* Configurable parameters for ARP                                     */
/*---------------------------------------------------------------------*/
#define OTCP_ARPMAX             8       /* Max # of open ARP device    */
#define OTCP_ARPNINTERFACES     4       /* Max # of ARP interfaces     */
#define OTCP_ARPNENTRIES        32      /* Max # of ARP table entries  */

/*---------------------------------------------------------------------*/
/* Configurable parameters for TCP                                     */
/*---------------------------------------------------------------------*/
#define OTCP_TCPMAX             64      /* Max # of open TCP device    */

/*---------------------------------------------------------------------*/
/* Configurable parameters for UDP                                     */
/*---------------------------------------------------------------------*/
#define OTCP_UDPMAX             32      /* Max # of open UDP device    */

/*---------------------------------------------------------------------*/
/* Configurable parameters for RAW                                     */
/*---------------------------------------------------------------------*/
#define OTCP_RAWMAX             32      /* Max # of open RAW device    */

/*---------------------------------------------------------------------*/
/* Configurable parameters for LOOP                                    */
/*---------------------------------------------------------------------*/
#define OTCP_LOOPMAX             2      /* Max # of open LOOP device   */



#endif /* _SYS_CONF_H */

#if __cplusplus
}
#endif
