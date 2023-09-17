/* @(#) pSOSystem : $RCSfile: sys_conf.h,v $ $Revision: 1.1.1.1 $ $Date: 2001/11/05 17:47:11 $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/* [clearcase]
modification history
-------------------
*/

/***********************************************************************/
/*                                                                     */
/*    MODULE: sys_conf.h                                               */
/*     $Date: 2001/11/05 17:47:11 $                                    */
/*   PURPOSE: pSOSystem configuration definitions                      */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*           Copyright 1991 - 2000, Integrated Systems, Inc.           */
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
/*   The "Configuration and Startup" section of the "Programmer's      */
/*   Reference" manual in pSOSystem contains a complete description    */
/*   of the symbols that are defined in this file.                     */
/*                                                                     */
/***********************************************************************/
#ifndef _SYS_CONF_H
#define _SYS_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************/
/*                                                                     */
/* SC_APP_PARMS must be defined before you include sysvars.h file.     */
/* Since "sysvars.h" file depends on SC_APP_PARMS and BSP_PARMS        */
/* to define the Storage Parms Structure.                              */
/*                                                                     */
/* Defining this variable to a Non-Zero Value will create a space in   */
/* the Storage Parms corresponding to the value. The total space       */
/* allocated is the sum of the BSP_PARMS and SC_APP_PARMS.             */
/*                                                                     */
/* SC_APP_NAME define controls the application name string that will   */
/* be displayed during the boot dialog.                                */
/*                                                                     */
/***********************************************************************/
#define SC_APP_PARMS 0
#define SC_APP_NAME "SNMP Applications"

/*---------------------------------------------------------------------*/
/* sys/types.h contains common definitions, (example YES, NO and more) */
/*---------------------------------------------------------------------*/
#include <sys/types.h>

/*---------------------------------------------------------------------*/
/* sysvars.h contains the structures and definitions for the system    */
/* variables that are stored in NVRAM. These structures and            */
/* definitions are also used when creating the boot ROM for the        */
/* target system. By using the same sysvars.h file it will ensure      */
/* consistency of the values stored by the boot ROM and the downloaded */
/* RAM application.                                                    */
/*---------------------------------------------------------------------*/
#include <sysvars.h>

/*---------------------------------------------------------------------*/
/* psos.h header file defines IO_AUTOINIT and IO_NOAUTOINIT            */
/*---------------------------------------------------------------------*/
#include <psos.h>

/*---------------------------------------------------------------------*/
/* bsp_conf.h header file for CPU specific defines		       */
/*---------------------------------------------------------------------*/
#include <bsp_conf.h>

/*---------------------------------------------------------------------*/
/* If USE_RARP is set to YES, then the IP address is obtained from the */
/* network.                                                            */
/*---------------------------------------------------------------------*/
#define USE_RARP	NO

/*---------------------------------------------------------------------*/
/* SC_VERSION specifies the revision level of the application.	       */
/*---------------------------------------------------------------------*/
#define SC_VERSION	300

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
/*  The SC_STARTUP_DIALOG define either enables the boot dialog if set */
/*  to YES, or disables the boot dialog if set to NO.                  */
/*                                                                     */
/*  SC_BOOT_ROM when set to YES, configures the application as a       */
/*  ROMable application. If set to NO, treats it as a RAM application. */
/*                                                                     */
/*  SC_SD_DEBUG_MODE determines the mode of the system, as follows:    */
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
/*             Boot ROMs. Note that when this mode is used, SMODE_MASK */
/*             in TD_FLAGS is always set. (See pROBE+ configuration    */
/*             parameters below.)                                      */
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
/*  put into the system.                                               */
/*---------------------------------------------------------------------*/
/*  pSOS+: The following set of macros control which type of kernel    */
/*    will be added to the system and also controls if we need to use  */
/*    the kernel from ROM instead of downloading each time.            */
/*              SC_PSOS : For single processor, unprotected kernel     */
/*                        Valuse can be NO/YES/USEROM                  */
/*              SC_PSOSP: For single processor, protected kernel       */
/*                        Valuse can be NO/YES/USEROM                  */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*  pROBE+: The following set of macros control which features will be */
/*    built in to the pROBE+ Target Agent. pROBE+ can run either in    */
/*    console mode (dumb terminal) or as a back-end to a Source Level  */
/*    Debugger on the host.  Not  all modules are needed for all cases.*/
/*    By eliminating modules you can save memory. The definition of    */
/*    the modules are as follows:                                      */
/*                                                                     */
/*    SC_PROBE - pROBE+ core module.  This must always be present if   */
/*       any part of pROBE+ is to be used. Setting this to NO will     */
/*       force all other pROBE+ modules to NO (even if they say YES    */
/*       in this file).                                                */
/*                                                                     */
/*    SC_PROBE_CIE - pROBE+ Console Executive. Set this to YES if you  */
/*       wish to control pROBE+ from a dumb terminal.  If you only     */
/*       use a source debugger and/or pRISM+, you can save memory by   */
/*       setting it to NO. If it is YES, you also need to sepcify the  */
/*       serial port used by the pROBE+ via SC_PROBE_CONSOLE.          */
/*                                                                     */
/*    SC_PROBE_DISSAM - pROBE+ Console Dissassembler. If you set       */
/*       SC_PROBE_CIE to YES and wish to view disassembled memory, set */
/*       this to YES as well. If you are using a host debugger and/or  */
/*       pRISM+, you can save memory by setting this to NO.            */
/*                                                                     */
/*    SC_PROBE_QUERY  - pROBE+ Query Services which provides the pSOS+ */
/*       awareness. This module allows you to query the information    */
/*       about pSOS+ obejects, issue a console-induced manual break    */
/*       or halt-request from the host debugger, set the task-specific */
/*       breakpoints, service call breakpoints and task-dispatch       */
/*       breakpoints. Unless you have severe memory restrictions or    */
/*       your application does not use pSOS+ kernel, it is suggested   */
/*       that you set this to YES.                                     */
/*                                                                     */
/*    SC_PROBE_DEBUG  - pROBE+ Remote Debug Module.  This module is    */
/*       used to enable communication between a host based source      */
/*       debugger and/or pRISM+ to the pROBE+ target agent using a     */
/*       light weight binary protocol.  This must be set to YES to     */
/*       enable pROBE+ to communicate with a source debugger or the    */
/*       pRISM+ host tools. It can be set to NO if you only use the    */
/*       pROBE+ console (dumb terminal) mode. If it is YES, the        */
/*       Target Agent Manager module SC_TAM must be set to YES.        */
/*                                                                     */
/*    SC_PROBE_HELP - pROBE+ online help module. Setting this define   */
/*       to YES provides the user with pROBE+ commands, when in        */
/*       stand-alone mode.                                             */
/*---------------------------------------------------------------------*/
/*    SC_TAM - Target Agent Manager. This module is used to enable     */
/*       communication between a host and the target agents. The       */
/*       target agents are SC_PROBE_DEBUG, and TAM_HOSTIO_AGENT.       */
/*       SC_TAM must be set to YES to use any of the target agents.    */
/*                                                                     */
/*       IF SC_TAM is YES and the host is connected to the target      */
/*       via network, you must set SC_PNET to YES. If the host is      */
/*       connected to the target via a serial connection, must         */
/*       select the proper serial port via SC_RBUG_PORT. Note if a     */
/*       serial port is used by the TAM for the host connection,       */
/*       this port should not be used by the application except for    */
/*       SC_PROBE_CONSOLE. For example, you should not set             */
/*       SC_RBUG_PORT and SC_APP_CONSOLE to the same serial port       */
/*       unless you don't use the host debugger when the               */
/*       application is running.                                       */
/*---------------------------------------------------------------------*/
/*    SC_PHILE through SC_EVLOG - Setting the definition line to       */
/*       YES will cause that component to be built into the system.    */
/*       Setting it to NO means the component will NOT be put into the */
/*       system.                                                       */
/*                                                                     */
/***********************************************************************/
#define SC_PSOS             YES  /* pSOS+ real-time kernel             */
#define SC_PSOSP             NO  /* pSOS+ real-time protected kernel   */
#define SC_PSOS_QUERY        NO  /* pSOS+ Query Services               */
#define SC_PROBE            YES  /* pROBE+ (processor svcs)            */
#define SC_PROBE_DISASM     YES  /* pROBE+ (disassembler)              */
#define SC_PROBE_CIE        YES  /* pROBE+ (console executive)         */
#define SC_PROBE_QUERY      YES  /* pROBE+ (query services)            */
#define SC_PROBE_DEBUG      YES  /* pROBE+ (debug interface agent) */
#define SC_PROBE_HELP       YES  /* pROBE+ Help command Handler.       */
#define SC_TAM              YES          /* Target Agent Manager.      */
#define SC_PHILE             NO  /* pHILE+ file system manager         */
#define SC_PHILE_PHILE       NO  /* pHILE+ (pHILE+ real-time file sys) */
#define SC_PHILE_MSDOS       NO  /* pHILE+ (MS-DOS FAT file system)    */
#define SC_PHILE_NFS         NO  /* pHILE+ (NFS client)                */
#define SC_PHILE_CDROM       NO  /* pHILE+ (ISO 9660 CD-ROM file sys)  */
#define SC_PREPC            YES  /* pREPC+ C run-time library          */
#define SC_PNA              YES  /* pNA+ TCP/IP networking manager     */
#define SC_PNET             YES  /* pNET Library for BOOT ROMS         */
#define SC_PRPC              NO  /* pRPC+ RPC component                */
#define SC_PSE_PRPC          NO  /* pRPC+ RPC component Over pSE+      */
#define SC_PSE               NO  /* pSE+ streams component             */
#define SC_PSKT              NO  /* pSKT+ SKT library component        */
#define SC_PTLI              NO  /* pTLI+ TLI library component        */
#define SC_PMONT            YES  /* pMONT+                             */
#define SC_PLM               NO  /* pLM+ Shared library manager        */
#define SC_PROFILER          NO  /* RTA profiler configuration         */
#define SC_RTEC              NO  /* RTA run-time error checker library */
#define SC_CXX_PKG           NO  /* C++ Support Package.               */
#define SC_IOSTREAM          NO  /* C++ I/O streams package.           */
#define SC_POSIX             NO  /* POSIX component (core)             */
#define SC_POSIX_MESSAGE_PASSING    NO  /* POSIX Named Message Queues  */
#define SC_POSIX_SEMAPHORES         NO  /* POSIX Named Semaphores      */
#define SC_EXM               NO  /* Exception Manager                  */
#define SC_EVLOG             NO  /* Event Log Services                 */

/*---------------------------------------------------------------------*/
/* If SC_QBIND is set to YES then the Quick bindings for pSOS+ System  */
/* calls will be used. If it is set to NO then the normal bindings     */
/* will be used.                                                       */
/*                                                                     */
/* Quick bindings will not be supported for the applications with      */
/* USER mode Tasks. So make sure your Application does not have any    */
/* USER mode Tasks.                                                    */
/*---------------------------------------------------------------------*/
#define SC_QBIND            NO     /* Quick bindings for pSOSystem calls*/

/*---------------------------------------------------------------------*/
/* If SC_ADDRCHK is set to YES, then address validation takes place    */
/* in the bindings before making system calls. If set to NO, no        */
/* validation takes place.                                             */
/*---------------------------------------------------------------------*/
#define SC_ADDRCHK          NO   /* Address validation for system calls*/

/*---------------------------------------------------------------------*/
/* System wide maximum name length.                                    */
/*---------------------------------------------------------------------*/
#define SC_NAMELEN          8

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
/*								       */
/* If SC_PMONT SC_AUTOINIT will be automatically redefined to 1 because*/
/* pMONT needs drivers to be automatically initialized.		       */
/*								       */
/*---------------------------------------------------------------------*/
#define SC_AUTOINIT         IO_AUTOINIT

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
/*  * SC_RBUG_PORT  specifies the serial channel number (starting      */
/*    from 1) which will be used for the connnect between the host     */
/*    debugger and the pROBE+ on the target. This is not needed if     */
/*    the host debugger is using the network connection. It can be     */
/*    disabled by using a value of 0. Note the port specified by       */
/*    SC_RBUG_PORT port should not be used by the application except   */
/*    for SC_PROBE_CONSOLE. For example, you should not set            */
/*    SC_RBUG_PORT and SC_APP_CONSOLE to the same serial channel       */
/*    unless you don't use the host debugger when the application is   */
/*    running.                                                         */
/*                                                                     */
/*    Above three macros SC_PROBE_CONSOLE, SC_APP_CONSOLE and          */
/*    SC_RBUG_PORT uses SERIAL_DRVRNUM(0) as default. This means that  */
/*    the Low-Level Serial Driver 0 (1) is being used default.         */
/*                                                                     */
/*    If you need to use another driver for any of the above macros,   */
/*    you need to change "0" to the driver number you want. Consult    */
/*    the "README" file in BSP directory for number of different       */
/*    drivers supported and for the SERIAL_DRVRNUM mapping for those   */
/*    drivers.                                                         */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_BAUD           9600
#define SC_APP_CONSOLE        (1 + SERIAL_DRVRNUM(0))
#define SC_PROBE_CONSOLE      (1 + SERIAL_DRVRNUM(0))
#define SC_RBUG_PORT          (0 + SERIAL_DRVRNUM(0))

/***********************************************************************/
/* SC_NumNon_pSOSChan is the number of non-pSOS users of the serial    */
/* channels. These are users that will be initiated before pSOS,       */
/* such as pROBE.                                                      */
/*                                                                     */
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
#define SD_LAN1               NO          /* Disable the LAN interface */
#define SD_LAN1_IP            0x00000000  /* IP address of the node    */
#define SD_LAN1_SUBNET_MASK   0x00000000  /* subnet mask for the node  */

/***********************************************************************/
/*                                                                     */
/*                B U S   C O N F I G U R A T I O N                    */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define SD_VME_BASE_ADDR      0x01000000  /* VME bus base address      */

/***********************************************************************/
/*                                                                     */
/*      S H A R E D   M E M O R Y   C O N F I G U R A T I O N          */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

/*---------------------------------------------------------------------*/
/* If you want to build a "Shared Memory Interface" for the network    */
/* set SD_SM_NODE to the node number of this node ( a non-zero value). */
/*---------------------------------------------------------------------*/
#define SD_SM_NODE            0      /* Node number for this node      */

/*---------------------------------------------------------------------*/
/* Shared memory network interface (SMNI) - SC_NISM_LEVEL should be 1  */
/* if this will be the FIRST system to use the SMNI on this board.  It */
/* should be 2 if this will not be the first system to use SMNI on this*/
/* board.  Usually this means that SC_NISM_LEVEL should be 1 if you    */
/* are building Boot ROMs, and 2 for a system that will be downloaded  */
/* and started using the Boot ROMs.  See the comments at the beginning */
/* of drivers/ni_smem.c for more explanation.                          */
/*---------------------------------------------------------------------*/
#define SD_NISM               NO       /* Disable/Enable Shared Mem NI */
#define SD_NISM_IP            0x00000000    /* IP address of this node */
#define SD_NISM_DIRADDR       0x00000400 /* Bus addr of SMNI directory */
#define SC_NISM_BUFFS         30            /* Num of buffers for SMNI */
#define SC_NISM_LEVEL         2         /* SMNI Level, 1 if first node */
#define SD_NISM_SUBNET_MASK   0x00000000 /* Subnet mask for the SMNI   */

/***********************************************************************/
/*                                                                     */
/*         M I S C E L L A N E O U S   N E T W O R K I N G             */
/*                       P A R A M E T E R S                           */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_GTWY_IP        0x00000000  /* Default gateway IP address*/

/***********************************************************************/
/* SC_RAM_SIZE define controls the end of RAM as seen by pSOSystem.    */
/* If it is set to zero, then pSOSystem uses ALL of the memory         */
/* remaining on a board for dynamic allocation ("region 0"). You may   */
/* override this by setting SC_RAM_SIZE to a non-zero value. If you do,*/
/* pSOSystem will not touch any memory after the specified address.    */
/*                                                                     */
/* NOTE: This is useful when making a BOOT ROM and you want to ensure  */
/* that the ROM's RAM area does not overflow the space allocated for it*/
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
/*                                                                     */
/*    * Major device 0 is reserved and cannot be used for any of these */
/*      devices.  Setting a device number to 0 here is the same as NO. */
/*                                                                     */
/*    * No device number may be higher than SC_DEVMAX (SC_DEVMAX may   */
/*      be increased, if desired)                                      */
/*                                                                     */
/*    * The lines defining the symbols DEV_SERIAL, DEV_TIMER, etc,     */
/*      should not be changed.  These are for use by application       */
/*      programs as the "device number" parameter on de_* calls.       */
/*                                                                     */
/***********************************************************************/
#define SC_DEV_SERIAL         1         /* 1  Serial driver            */
#define SC_DEV_TIMER          2         /* 2  Periodic tick timer      */
#define SC_DEV_RAMDISK       NO         /* 3  RAM disk                 */
#define SC_DEV_CONSOLE       NO         /* 4  PC-Console driver        */
#define SC_DEV_SCSI          NO         /* 5  SCSI bus                 */
#define SC_DEV_SCSI_TAPE     NO         /* 6  SCSI bus, tape device    */
#define SC_DEV_IDE           NO         /* 7  IDE Driver               */
#define SC_DEV_FLOPPY        NO         /* 8  Floppy Driver            */
#define SC_DEV_NTFTP         NO         /* 9  New TFTP pseudo driver   */
#define SC_DEV_TFTP          NO         /* 10 TFTP pseudo driver       */
#define SC_DEV_HTTP          NO         /* 11 HTTP pseudo driver       */
#define SC_DEV_SPI           NO         /* 12 SPI driver               */
#define SC_DEV_DLPI          NO         /* 13 DLPI pseudo driver       */
#define SC_DEV_PSCONSOLE     NO         /* 14 Pseudo Console driver    */
#define SC_DEV_MEMLOG        NO         /* 15 Memory log driver        */
#define SC_DEV_RDIO          NO         /* 16 pROBE+ RDIO driver       */
#define SC_DEV_NULL          17         /* 17 Null Device Driver       */
#define SC_DEV_PARALLEL      NO         /* 18 Parallel Port Driver     */
#define SC_DEV_CMOS          NO         /* 19 CMOS Driver              */
#define SC_DEV_WATCHDOG      NO         /* 20 Watchdog Driver          */
#define SC_DEV_LOG           NO         /* 21 STREAMS log driver       */
#define SC_DEV_PSMUX         NO         /* 22 Sample Mux driver        */
#define SC_DEV_PSLWR         NO         /* 23 Sample loopback driver   */
#define SC_DEV_SLLWR         NO         /* 24 Sample loopback driver   */
#define SC_DEV_PIPE          NO         /* 25 Pipe Driver              */
#define SC_DEV_SIMCONSOLE    NO         /* 26 pSOSim Console Driver    */

#define SC_DEVMAX            26         /* Maximum device number       */

#define DEV_SERIAL      (SC_DEV_SERIAL     << 16)
#define DEV_PARALLEL    (SC_DEV_PARALLEL   << 16)
#define DEV_TIMER       (SC_DEV_TIMER      << 16)
#define DEV_RAMDISK     (SC_DEV_RAMDISK    << 16)
#define DEV_SCSI        (SC_DEV_SCSI       << 16)
#define DEV_SCSI_TAPE   (SC_DEV_SCSI_TAPE  << 16)
#define DEV_PSCONSOLE   (SC_DEV_PSCONSOLE  << 16)
#define DEV_SYSCONSOLE  ((SC_DEV_PSCONSOLE  << 16) + SYSCONSOLE_DEV)
#define DEV_PSEUDO      ((SC_DEV_PSCONSOLE  << 16) + PSEUDO_DEV)
#define DEV_STDIN       ((SC_DEV_PSCONSOLE  << 16) + STDIN_DEV)
#define DEV_STDOUT      ((SC_DEV_PSCONSOLE  << 16) + STDOUT_DEV)
#define DEV_STDERR      ((SC_DEV_PSCONSOLE  << 16) + STDERR_DEV)
#define DEV_NULL        (SC_DEV_NULL       << 16)
#define DEV_MEMLOG      (SC_DEV_MEMLOG     << 16)
#define DEV_RDIO        (SC_DEV_RDIO       << 16)
#define DEV_DLPI        (SC_DEV_DLPI       << 16)
#define DEV_TFTP        (SC_DEV_TFTP       << 16)
#define DEV_NTFTP       (SC_DEV_NTFTP      << 16)
#define DEV_HTTP        (SC_DEV_HTTP       << 16)
#define DEV_SPI         (SC_DEV_SPI        << 16)
#define DEV_WATCHDOG    (SC_DEV_WATCHDOG   << 16)
#define DEV_FLOPPY      (SC_DEV_FLOPPY     << 16)
#define DEV_IDE         (SC_DEV_IDE        << 16)
#define DEV_CMOS        (SC_DEV_CMOS       << 16)
#define DEV_CONSOLE     (SC_DEV_CONSOLE    << 16)
#define DEV_LOG         (SC_DEV_LOG        << 16)
#define DEV_PSMUX       (SC_DEV_PSMUX      << 16)
#define DEV_PSLWR       (SC_DEV_PSLWR      << 16)
#define DEV_SLLWR       (SC_DEV_SLLWR      << 16)
#define DEV_PIPE        (SC_DEV_PIPE       << 16)
#define DEV_SIMCONSOLE  (SC_DEV_SIMCONSOLE << 16)

/***********************************************************************/
/*            p S E U D O   D R I V E R   D E F I N E S                */
/*                                                                     */
/* The defines below control the behavour of the pSEUDO driver if the  */
/* SC_DEV_PSCONSOLE is enabled.                                        */
/*                                                                     */
/***********************************************************************/
#define	SC_PSCNSL_SHARED_CHAN	2        /* Num of shared channels     */
#define	SC_PSCNSL_PRIVATE_CHAN	4        /* Number of private channels */
#define	SC_PSCNSL_MAX_CUSTOM	1        /* NUmber of custome device   */
#define	SC_PSCNSL_DEFAULT_DEV	CONSOLE  /* Default console device     */

/***********************************************************************/
/*           T F T P   D R I V E R   C O N F I G U R A T I O N         */
/*                                                                     */
/* The defines below define the Configuration Parameters needed by the */
/* TFTP driver.                                                        */
/*                                                                     */
/***********************************************************************/
#define SC_MAX_TFTP_CHAN    1       /* Maximum number of TFTP channels */

/***********************************************************************/
/*           H T T P    S E R V E R   C O N F I G U R A T I O N        */
/*                                                                     */
/* The SC_MAX_HTTP_CHAN define controls the maximum number of HTTP     */
/* channels in the system.                                             */
/*                                                                     */
/***********************************************************************/
#define SC_MAX_HTTP_CHAN    1

/***********************************************************************/
/*        P I P E   D R I V E R   C O N F I G U R A T I O N            */
/*                                                                     */
/* The SC_MAX_PIPE_CHAN define controls the maximum number of PIPE     */
/* channels that will be configured in the system.                     */
/*                                                                     */
/***********************************************************************/
#define SC_MAX_PIPE_CHAN    1

/***********************************************************************/
/*                                                                     */
/*              M M U   L I B R A R Y   P A R A M S                    */
/*                                                                     */
/* The SC_MMULIB define controls the MMU library call-outs when pROBE+ */
/* is entered. Setting this to YES enables the callouts, while setting */
/* this to NO disables the callouts when pROBE+ is entered and BSP_MMU */
/* is enabled.                                                         */
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
#define KC_NTASK       12       /* max number of tasks                 */
#define KC_NQUEUE      10       /* max number of message queues        */
#define KC_NSEMA4      30       /* max number of semaphores            */
#define KC_NTIMER      10       /* max number of timers                */
#define KC_NMUTEX      10       /* Max number of mutexes               */
#define KC_NCVAR       5        /* Max number of condition variables   */
#define KC_NTVAR       5        /* Max number of task variables        */
#define KC_NCOCB       5        /* Max number of callouts              */
#define KC_NTSD        5        /* Max # of Task Specific Data entries */
#define KC_NLOCOBJ     50       /* max number of local objects         */
#define KC_NMSGBUF     256      /* max number of message buffers       */
#define KC_NSIGACT     5        /* max number of SIGACT structures     */
#define KC_TICKS2SEC   100      /* clock tick interrupt frequency      */
#define KC_TICKS2SLICE 10       /* time slice quantum, in ticks        */
#define KC_MAXDNTENT   10        /* max number of device names in DNT   */
#define KC_DNLEN       10       /* max length of a device name in DNT  */
#define KC_SYSSTK      0x0C00   /* pSOS+ system stack size (bytes)     */
#define KC_ROOTSSTK    0x0800   /* ROOT supervisor stack size          */
#define KC_IDLESTK     0x0400   /* IDLE stack size                     */
#define KC_ROOTUSTK    0        /* ROOT user stack size                */
#define KC_ROOTMODE    (T_SUPV | T_ISR) /* ROOT initial mode           */
#define KC_ROOTPRI     230      /* ROOT task priority                  */
#define KC_NIO         (SC_DEVMAX+1) /* # of devices in initial iojtab */
#define KC_MAXIO       (SC_DEVMAX+1) /* Max # of devices in the system */

#define KC_STK_UNDERFLOW   NO   /* Stack underflow detection be enabled*/
#define KC_STK_OVERFLOW   YES   /* Stack overflow detection be enabled */
#define KC_GUARD_PAGES 1        /* Guard size (in pagesize) for stack  */
				/* underflow and overflow detection    */
#define KC_NDYNOBJ       0      /* Number of dynamic objects           */

/*---------------------------------------------------------------------*/
/* User and Supervisor stack lengths that are used by the MMU library  */
/* to create page table entries for the stack region in the virtual    */
/* space.                                                              */
/*---------------------------------------------------------------------*/
#define KC_USTKMAPLEN   0x100000
#define KC_SSTKMAPLEN   0x100000

/*---------------------------------------------------------------------*/
/* Default stack sizes to use if t_create values are 0.                */
/*---------------------------------------------------------------------*/
#define KC_DFTSSTK	8096       /* Default supervisor-mode stack size */
#define KC_DFTUSTK	8096             /* Default user-mode stack size */

/*---------------------------------------------------------------------*/
/* The following are examples for modifying the following defines      */
/*                                                                     */
/* To configure a user written routine as a fatal error handler, you   */
/* need to do the following:                                           */
/*                                                                     */
/* extern void MyHandler (void);                                       */
/* #define KC_FATAL    ((void (*)()) MyHandler)                        */
/*                                                                     */
/* except KC_RMONCO which should be of type:                           */
/*   ULONG (*)(ULONG, ULONG, ULONG, void *, ULONG, struct hrt_count *) */
/*                                                                     */
/* You should add the partial structure definition:                    */
/*    struct hrt_count;                                                */
/* to prevent compiler warnings, unless you include hrtimer.h.         */
/*---------------------------------------------------------------------*/
#define KC_STARTCO     0        /* callout at task activation          */
#define KC_DELETECO    0        /* callout at task deletion            */
#define KC_SWITCHCO    0        /* callout at task switch              */
#define KC_FATAL       0        /* fatal error handler address         */
#define KC_IDLECO      0        /* Idle Task Callout                   */

#define KC_RMONCO      0        /* Resource Monitoring Callout         */

/*---------------------------------------------------------------------*/
/* Example KC_SYSERRCO definition:                                     */
/* extern void MyCallout(void *TCB, ULONG Tid, ULONG Error,            */
/*     ULONG ErrorAddr)                                                */
/* #define KC_SYSERRCO (MyCallout)                                     */
/*---------------------------------------------------------------------*/
#define KC_SYSERRCO    0        /* System call error callout address   */

/*---------------------------------------------------------------------*/
/* Example KC_SIGDFTACO definition:                                    */
/* extern void MyCallout(ULONG sig)                                    */
/* #define KC_SIGDFTACO (MyCallout)                                    */
/*---------------------------------------------------------------------*/
#define KC_SIGDFTACO   0        /* POSIX signal default action callout */

/*---------------------------------------------------------------------*/
/*                pROBE+ configuration parameters                      */
/*                                                                     */
/* NOTE: The SMODE_MASK bit in TD_FLAGS is set as                      */
/*       SC_SD_DEBUG_MODE = DBG_AP.                                    */
/*---------------------------------------------------------------------*/
#define TD_ILEV        (MAX_ILEV << 12)    /* pROBE+ interrupt mask    */
#define TD_FLAGS       (STK_USAGE_MASK | STK_CHECK_MASK | NODOTS_MASK | TD_ILEV | NOTUPD_MASK) /* flags  */

/*---------------------------------------------------------------------*/
/* The variable defined below should be set to the Starting address    */
/* for pROBE+ DATA AREA. This is required by all the smaller boards.   */
/* NOTE:                                                               */
/*       Setting of this value should be a variable which is a array   */
/*       of characters with atleast 0x4400 bytes free space.           */
/*---------------------------------------------------------------------*/
#define TD_DATASTART   0x0

/*---------------------------------------------------------------------*/
/* Example for how to modify TD_URCOM_HANDLER to add user-written      */
/* handler to handle the user-defined commands from pROBE+ console:    */
/*                                                                     */
/* extern long probe_user_cmd_handler(UCHAR *);                        */
/* #define TD_URCOM_HANDLER probe_user_cmd_handler                     */
/*---------------------------------------------------------------------*/
#define TD_URCOM_HANDLER   0  /* user-defined cammand handler.         */

/*---------------------------------------------------------------------*/
/*                 pHILE+ configuration parameters                     */
/*---------------------------------------------------------------------*/
#define FC_LOGBSIZE    9        /* Block size (base-2 exponent)        */
#define FC_NBUF        6        /* Number of cache buffers             */
#define FC_NMOUNT      3        /* Max # of mounted volumes            */
#define FC_NFCB        10       /* Max # of opened files per system    */
#define FC_NCFILE      2        /* Max # of opened files per task      */
#define FC_NDNLC       0        /* Max # of cached directory entries   */

/*---------------------------------------------------------------------*/
/*                pHILE+ DATA AREA definitions.                        */
/*---------------------------------------------------------------------*/
#define FC_DATA        0        /* pHILE+ Data Area Starting Address   */
#define FC_DATASIZE    0        /* pHILE+ Data Area Size               */

/*---------------------------------------------------------------------*/
/* Set the FC_ERRCO define to install the callout handler for I/O      */
/* errors. The callout handler provided in FC_ERRCO will be called if  */
/* there is an error during an I/O operation on the Volume.            */
/*                                                                     */
/* To install the error callout handler: Follow the syntax below       */
/*                                                                     */
/* #include <phile.h>                                                  */
/* extern user_fcerrco(int, ULONG, const struct buffer_header *)       */
/* #define FC_ERRCO     user_fcerrco                                   */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define FC_ERRCO        0       /* I/O error call-out                  */

/*---------------------------------------------------------------------*/
/*                 pLM+ configuration parameters                       */
/*---------------------------------------------------------------------*/
#define LM_MAXREG      8        /* Max # of registered libraries */
#define LM_DATA        0        /* pLM+ data area */
#define LM_DATASIZE    0        /* pLM+ data area size */

/*---------------------------------------------------------------------*/
/* Set LM_DEFAULT_COUTS to YES if you need to use the pSOSystem Default*/
/* pLM Callouts. Set it to NO if you are providing the callouts.       */
/*                                                                     */
/* Setting LM_DEFAULT_COUTS to NO needs "LM_LOADCO" and "LM_UNLOADCO"  */
/* definitions to be valid address (Function address).                 */
/*---------------------------------------------------------------------*/
#define LM_DEFAULT_COUTS    YES

/*---------------------------------------------------------------------*/
/*                                                                     */
/* The following callouts needs to be defined by user, If users wants  */
/* their callouts to be used instead of pSOSystem default Callouts.    */
/*                                                                     */
/* If we are using pSOSystem default callouts, Users needs to add their*/
/* callouts to the pSOSystem callouts table using PssRegister_pLM_couts*/
/* and remove using PssDeregister_pLM_couts.                           */
/*                                                                     */
/* To have user callouts follow the syntax below:                      */
/*                                                                     */
/* #include <plm.h>                                                    */
/* extern ULONG user_load_co(const char *libname, ULONG scope,         */
/*                           ULONG version, const void *libinfo,       */
/*                           sl_attrib *attr);                         */
/* extern ULONG user_unload_co(const sl_attrib *attr);                 */
/*                                                                     */
/* #define LM_LOADCO      user_load_co                                 */
/* #define LM_UNLOADCO    user_unload_co                               */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define LM_LOADCO      0
#define LM_UNLOADCO    0

/*---------------------------------------------------------------------*/
/*         Target Agent Manager (TAM) configuration parameters         */
/*---------------------------------------------------------------------*/
#define TAM_MAX_AGENTS        6  /* Specify number of target agents    */
#define TAM_MAX_BUFFS        10  /* Specify number of buffers.         */
#define TAM_MAX_HDRS         50  /* Specify number of Tx. Headers.     */
#define TAM_NON_OS           NO  /* Is TAM running for Non-OS platform */
#define TAM_MIN_TIMEOUT       0  /* Minimum timeout used in TAM before */
                                 /* Re-trying the packet in System mode*/
#define TAM_IN_TASK_PRI     255  /* TAM Input Task priority.           */
#define TAM_OUT_TASK_PRI    255  /* TAM Output Task priority.          */

/*---------------------------------------------------------------------*/
/* Is there a User Configurable Escape Sequence Characters             */
/* This value should be multiple of 2.                                 */
/*---------------------------------------------------------------------*/
#define TAM_NESC_CHARS        0  /* Number of  escape Chars.           */

/*---------------------------------------------------------------------*/
/* If TAM_NESC_CHARS is non-zero, the following variable needs to      */
/* set to point to a "unsigned char *".                                */
/*                                                                     */
/* The character array should contain values in the format given below */
/*                                                                     */
/* CHAR1, ESC_CHAR1, CHAR2, ESC_CHAR2, ....                            */
/*                                                                     */
/* Where CHAR1 is the character that needs to be converted when sent   */
/* on wire, with ESC and ESC_CHAR1.                                    */
/*                                                                     */
/* The syantax to set will be:                                         */
/*                                                                     */
/* extern UCHAR escseq[];                                              */
/* #define TAM_ESCSEQ         escseq                                   */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define TAM_ESCSEQ            0

/*---------------------------------------------------------------------*/
/* Target Agents                                                       */
/*---------------------------------------------------------------------*/
#define TAM_HOSTIO_AGENT    YES     /* Host I/O via db_input/db_output */

/*---------------------------------------------------------------------*/
/*           Exception Manager configuration parameters                */
/*---------------------------------------------------------------------*/
/* Examples for how to modify EXM_SYSHNDL/EXM_TASKHNDL/EXM_SIGMAP      */
/*                                                                     */
/* To configure a user written routine as a system or default task     */
/* exception handler, you need, for example, to modify and add the     */
/* following lines:                                                    */
/*                                                                     */
/* extern long my_system_excp_handler();                               */
/* #define EXM_SYSHNDL   my_system_excp_handler                        */
/*                                                                     */
/* extern long my_default_task_excp_handler();                         */
/* #define EXM_SYSHNDL   my_default_task_excp_handler                  */
/*                                                                     */
/* To assign a mapping table to map exception to signal, you need, for */
/* example, modify and add the following lines:                        */
/*                                                                     */
/* extern ULONG my_exmSignalMapping[];                                 */
/* #define EXM_SIGMAP    my_exmSignalMapping                           */
/*                                                                     */
/* Here my_exmSignalMapping must be an array of                        */
/*    ULONG[EXM_MAX_GROUPS+1].                                         */
/*---------------------------------------------------------------------*/
#define EXM_SYSHNDL        0     /* system exception handler           */
#define EXM_TASKHNDL       0     /* default task exception handler     */
#define EXM_SIGMAP         0     /* signal to exception mapping table  */
#define EXM_SVCERR_MASK    0     /* syscall error exception mask       */


/*---------------------------------------------------------------------*/
/*          Event Log Services Configuration Parameters                */
/*---------------------------------------------------------------------*/
#define EVLC_RECSZ        48 /* System wide max event log record size  */
#define EVLC_STDBUFSZ    256 /* Standard event log buffer size         */
#define EVLC_STDSVRTY      2 /* Standard event log severity level      */

/*---------------------------------------------------------------------*/
/*                  pREPC+ configuration parameters                    */
/*---------------------------------------------------------------------*/
#define LC_BUFSIZ      (1 << FC_LOGBSIZE) /* I/O buffer size           */
#define LC_NUMFILES    5        /* Max number of open files per task   */
#define LC_WAITOPT     0        /* Wait option for memory allocation   */
#define LC_TIMEOPT     0        /* Timeout option for mem allocation   */

/*---------------------------------------------------------------------*/
/* pREPC+ stdin, stdout and stderr device mapping.                     */
/*---------------------------------------------------------------------*/
#ifndef LC_STDIN	/* If stdin, stdout and stderr not defined in CPU
			   specific header file */
#if     (SC_DEV_PSCONSOLE)
#define LC_STDIN       "///dev/stdin"   /* The default STDIN  device   */
#define LC_STDOUT      "///dev/stdout"  /* The default STDOUT device   */
#define LC_STDERR      "///dev/stderr"  /* The default STDERR device   */

#elif   (SC_DEV_SERIAL)
#define LC_STDIN       "///dev/tty"     /* Make TTY the STDIN  device  */
#define LC_STDOUT      "///dev/tty"     /* Make TTY the STDOUT device  */
#define LC_STDERR      "///dev/tty"     /* Make TTY the STDERR device  */

#elif (SC_DEV_SIMCONSOLE)
#define LC_STDIN       "///dev/simconsole"  /* Make simconsole  STDIN  */
#define LC_STDOUT      "///dev/simconsole"  /* Make simconsole  STDOUT */
#define LC_STDERR      "///dev/simconsole"  /* Make simconsole  STDERR */

#else
#define LC_STDIN       "///dev/null"    /* Make NULL the STDIN  device */
#define LC_STDOUT      "///dev/null"    /* Make NULL the STDOUT device */
#define LC_STDERR      "///dev/null"    /* Make NULL the STDERR device */
#endif
#endif /* #ifndef LC_STDIN */

#if (SC_DEV_RAMDISK)
#define LC_TEMPDIR     DEV_RAMDISK      /* The default TEMPDIR device  */

#elif (SC_DEV_SCSI)
#define LC_TEMPDIR     DEV_SCSI         /* The default TEMPDIR device  */

#elif (SC_DEV_IDE)
#define LC_TEMPDIR     DEV_IDE          /* The default TEMPDIR device  */

#elif (SC_DEV_FLOPPY)
#define LC_TEMPDIR     DEV_FLOPPY       /* The default TEMPDIR device  */

#else
#define LC_TEMPDIR     0                /* The default TEMPDIR device  */
#endif

/*---------------------------------------------------------------------*/
/* pNET configuration parameters                                       */
/*---------------------------------------------------------------------*/
#define PNET_PORT      2

/*---------------------------------------------------------------------*/
/*                  pNA+ configuration parameters                      */
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
#define NC_NSOCKETS    4        /* Number of sockets in the system     */
#define NC_NDESCS      4        /* # of socket descriptors/task        */
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
#define NC_BUFS_8500   2        /* number of 8500 byte buffers. These  */
                                /* buffers are used for reassembling   */
                                /* fragmented packets                  */

#define NC_MAX_BUFS    9        /* max number of NC_BUFS types         */

#define NC_PNAMEM_NEWSCHEME     YES /* apply new pNA MEM management scheme*/

#define NC_BUFS_XX_INTERNAL     20  /* # of buffers for internal pNA usage */
#define NC_MBLKS_INT_PERCENT    15  /* % of mblks reserved for pNA internal */
#define NC_MBLKS_TX_PERCENT     40  /* % of mblks reserved for Tx Mem Pool */

#define NC_BUFS_0_TX_PERCENT    50  /* % of 0 length buffers used for Tx */
#define NC_BUFS_32_TX_PERCENT   50  /* % of 32 length buffers used for Tx */
#define NC_BUFS_64_TX_PERCENT   50  /* % of 64 length buffers used for Tx */
#define NC_BUFS_128_TX_PERCENT  50  /* % of 128 byte buffers used for Tx */
#define NC_BUFS_256_TX_PERCENT  50  /* % of 256 byte buffers used for Tx */
#define NC_BUFS_512_TX_PERCENT  50  /* % of 512 byte buffers used for Tx */
#define NC_BUFS_1024_TX_PERCENT 50  /* % of 1k byte buffers used for Tx */
#define NC_BUFS_2048_TX_PERCENT 50  /* % of 2k byte buffers used for Tx */
#define NC_BUFS_4096_TX_PERCENT 50  /* % of 4k byte buffers used for Tx */

/*---------------------------------------------------------------------*/
/* pNA+ daemon task configuration parameters                           */
/*---------------------------------------------------------------------*/
#define NC_DTASK_SSTKSZ         0x800   /* pNAD daemon task sstack size*/
#define NC_DTASK_USTKSZ         0x400   /* pNAD daemon task ustack size*/
#define NC_DTASK_PRIO           255     /* pNAD daemon task priority   */
#define NC_NEW_MULTITASK_SYNC   YES     /* deploy new pNA+ sync scheme */
#define NC_USE_MUTEX            YES     /* use pSOS MUTEX primitive    */
#define NC_NO_ZCOPY_USERMODE    YES     /* disable zero-copy from user-mode */

/*---------------------------------------------------------------------*/
/* pNA+ DATA AREA definitions.                                         */
/*---------------------------------------------------------------------*/
#define NC_DATA                 0       /* pNA+ Data Area Starting Addr*/
#define NC_DATASIZE             0       /* pNA+ Data Area Size         */

/*---------------------------------------------------------------------*/
/* The following are examples for modifying the following defines      */
/*                                                                     */
/* Using a user written routine as a pNA+ Signal Handler               */
/* extern void MySigHandler (void);                                    */
/* #define NC_SIGNAL   ((void (*)()) MySigHandler)                     */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define NC_SIGNAL      NULL                        /* pNA+ Signal Hdlr */

/*---------------------------------------------------------------------*/
/* pRPC+ configuration parameters                                      */
/*---------------------------------------------------------------------*/
#define NR_PMAP_PRIO      254                         /* task priority */
#define NR_PMAP_SSTACK    0x2000              /* supervisor stack size */
#define NR_PMAP_USTACK    0x100                     /* user stack size */
#define NR_PMAP_FLAGS     T_LOCAL                    /* t_create flags */
#define NR_PMAP_MODE      T_SUPV                       /* t_start mode */
#define NR_DEBUG_FLAG     NO     /* Turns on debug msgs from PMAP task */

/*---------------------------------------------------------------------*/
/* pRPC+ DATA AREA definitions.                                        */
/*---------------------------------------------------------------------*/
#define NR_DATA           0        /* pRPC+ Data Area Starting Address */
#define NR_DATASIZE       0        /* pRPC+ Data Area Size             */

/*---------------------------------------------------------------------*/
/* User can change these TWO defines to have their own handlers.       */
/*---------------------------------------------------------------------*/
#define NR_GETHOSTNAME    nr_gethostname      /* Function to get local */
                                              /* host name             */
#define NR_GET_HENTBYNAME nr_get_hentbyname   /* Function to map host  */
                                              /* name to it's IP Addr  */

/*---------------------------------------------------------------------*/
/* pSE+ configuration parameters                                       */
/*---------------------------------------------------------------------*/
#define SE_NBUFS_0      64       /* number of 0 length buffers         */
#define SE_NBUFS_32     128      /* number of 32 byte buffers          */
#define SE_NBUFS_64     128      /* number of 64 byte buffers          */
#define SE_NBUFS_128    256      /* number of 128 byte buffers         */
#define SE_NBUFS_256    32       /* number of 256 byte buffers         */
#define SE_NBUFS_512    32       /* number of 512 byte buffers         */
#define SE_NBUFS_1024   16       /* number of 1k byte buffers          */
#define SE_NBUFS_2048   16       /* number of 2k byte buffers          */
#define SE_NBUFS_4096   8        /* number of 4k byte buffers          */
#define SE_MAX_BUFS     9        /* max number of stream buffer types  */
#define SE_MAX_MODULES  4        /* max number of streams modules      */
#define SE_SHARE_NI     NO       /* LAN-NI with streams?               */
#define SE_DATA_SIZE    0x1000   /* size of pSE data area              */
                                 /* (must be at least 3K)              */
#define SE_TASK_PRIO    250      /* priority for pSE task              */
#define SE_STACK_SIZE   0x2000   /* stack size for pSE task            */
#define SE_DEF_UID      0        /* default user id                    */
#define SE_DEF_GID      0        /* default group id                   */
#define SE_N_FDS        32       /* max # of system-wide               */
                                 /* stream descriptors                 */
#define SE_N_TASKFDS    8        /* max # of per-task stream           */
                                 /* descriptors                        */
#define SE_N_LINKS      32       /* max # of multiplexing links        */
#define SE_N_TIMEOUTS   32       /* max # of timeout requests          */
#define SE_N_BUFCALLS   32       /* max # of bufcall requests          */
#define SE_N_QUEUES     256      /* # of queues                        */
#define SE_N_QBANDS     (SE_N_QUEUES * 2)  /* # of qbands              */
#define SE_N_MBLKS      512      /* # of message blocks                */
#define SE_PRI_BANDS    NO       /* Whether priority bands are desired */
#define SE_STRLOG       YES      /* Whether the log facility is cnfgrd */
#define SE_TMO_RES      2        /* pSE Timer resolution in ticks      */
#define SE_MAX_AUTOPUSH 10       /* Max # of autopush entries          */
#define SE_SIGHANDLER   NULL     /* pSE global signal handler          */
#define SE_LOG_BUFSIZE  100      /* Maximum size of the log message    */
#define SE_NUM_LOG_BUFFERS   25  /* Number of log buffs to be allocated*/
#define SE_BUF_BORROW_LEVEL  0   /* The buffer borrowing level desired */
#define SE_NO_GLOBAL_LOCKING NO  /* Whether global locking is desired  */
#define SE_PCRASH_BUF_SIZE   8192   /* default pcrash buffer size      */
#define SE_HI_BUF_RESVN_PERCENT  12 /* Hi buffer reservation %         */
#define SE_MED_BUF_RESVN_PERCENT 13 /* Med buffer reservation %        */
#define SE_USE_MUTEX             YES /* Use mutex locks                */
#define SE_GETTICKS_FN      tm_getticks  /* Use the default tm_getticks*/
                                         /* function.                  */
#define SE_COMPAT_MODE      2    /* Compatibility mode Level.          */
#define SE_NO_ZCOPY_USERMODE  NO /* disable zero-copy from user-mode   */

/*---------------------------------------------------------------------*/
/* Following macro SE_PRINTF uses printf as the default print function.*/
/* If User needs to use different function, then change the proto-type.*/
/*---------------------------------------------------------------------*/
extern int        printf(const char *, ...);
#define SE_PRINTF       printf   /* pSE printf function                */

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
/* pMONT+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define PM_TRACE_BUFF    0      /* Address of trace buffer --          */
                                /* Allocated by pSOSystem if 0         */
#define PM_TRACE_SIZE    0x6000 /* Size of trace buffer                */
#define PM_TIMER         YES    /* 2nd timer for finer time            */
                                /* within data collection              */

/*---------------------------------------------------------------------*/
/* General Serial Block Configuration Parameters                       */
/*                                                                     */
/* GS_MBLKS defines the number of the message blocks. To allocate a    */
/* data block (GS buffer), a message block is needed. Usually, the     */
/* number of the message blocks will be the same as the total number   */
/* of the data blocks, but it can be differenet. If GS_MBLKS is not    */
/* defined or it is zero, the GSblkSetup() will use the total number   */
/* of the data blocks as the number of the message blocks.             */
/*---------------------------------------------------------------------*/
#define GS_BUFS_0      16       /* number of 0 byte buffers            */
#define GS_BUFS_32     16       /* number of 32 byte buffers           */
#define GS_BUFS_64      0       /* number of 64 byte buffers           */
#define GS_BUFS_128    16       /* number of 128 byte buffers          */
#define GS_BUFS_256     0       /* number of 256 byte buffers          */
#define GS_BUFS_512     0       /* number of 512 byte buffers          */
#define GS_BUFS_1024    0       /* number of 1k byte buffers           */
#define GS_BUFS_2048    0       /* number of 2k byte buffers           */
#define GS_BUFS_4096    1       /* number of 4k byte buffers           */

#define GS_MBLKS       48       /* number of message blocks            */
#define GS_MAX_BUFS    9        /* max number of serial buffer types   */

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------*/
/* components.h will perform a check of the available components for   */
/* pSOSystem and automatically disable the components which are not    */
/* available. This allows an application to be common to all pSOSystem */
/* targets. Applications that rely on the presence of a given          */
/* component will obviously not function on a target without that      */
/* component.                                                          */
/*---------------------------------------------------------------------*/
#include <components.h>

#endif /* _SYS_CONF_H */
