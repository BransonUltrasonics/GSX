/* prjConfig.c - dynamically generated configuration file */


/*
GENERATED: Wed Mar 09 14:43:48 EST 2022
DO NOT EDIT - file is regenerated whenever the project changes.
This file contains the non-BSP system initialization code
to Create a bootable VxWorks image (custom configured).
*/


/* includes */

#include <vxWorks.h>
#ifdef _WRS_CONFIG_COMPAT69
#   include <config.h>
#else /* !_WRS_CONFIG_COMPAT69 */
#   include <prjParams.h>
#endif /* _WRS_CONFIG_COMPAT69 */
#include <adrSpaceLib.h>
#include <applUtilLib.h>
#include <auxv.h>
#include <boardLib.h>
#include <cacheLib.h>
#include <cafe/cafe.h>
#include <cafe/ep/aggregator.h>
#include <cafe/ep/cafe_namespace.h>
#include <cafe/ep/elog_native.h>
#include <cafe/ep/file_stream_reader.h>
#include <cafe/ep/file_stream_writer.h>
#include <cafe/ep/print.h>
#include <cafe/ep/realtime_ep.h>
#include <cafe/ep/sysinfo.h>
#include <cafe/ep/tcf_upload.h>
#include <cafe/ep/timer_ep.h>
#include <cafe/ep/timestamp_normalizer.h>
#include <classLib.h>
#include <condVarLib.h>
#include <cplusLib.h>
#include <dataCollectorLib.h>
#include <dbgLib.h>
#include <dosFsLib.h>
#include <drv/erf/erfLib.h>
#include <drv/manager/device.h>
#include <edrLib.h>
#include <endLib.h>
#include <envLib.h>
#include <etherMultiLib.h>
#include <eventLib.h>
#include <excLib.h>
#include <fioLib.h>
#include <fsEventUtilLib.h>
#include <fsMonitor.h>
#include <ftpLib.h>
#include <hashLib.h>
#include <hookLib.h>
#include <hostFsLib.h>
#include <hostLib.h>
#include <hwif/util/vxbIsrDeferLib.h>
#include <hwif/util/vxbParamLib.h>
#include <hwif/vxBus.h>
#include <intLib.h>
#include <ioLib.h>
#include <iosLib.h>
#include <iosScLib.h>
#include <jobQueueLib.h>
#include <loadElfLib.h>
#include <logLib.h>
#include <lstLib.h>
#include <math.h>
#include <miiBus.h>
#include <moduleLib.h>
#include <msgQLib.h>
#include <mux2Lib.h>
#include <muxLib.h>
#include <net/mbuf.h>
#include <net/utils/ifconfig.h>
#include <netBufLib.h>
#include <netDrv.h>
#include <netLib.h>
#include <netdb.h>
#include <objLib.h>
#include <pipeDrv.h>
#include <private/adrSpaceLibP.h>
#include <private/condVarLibP.h>
#include <private/coprocLibP.h>
#include <private/cplusLibP.h>
#include <private/dbgLibP.h>
#include <private/eventDefsP.h>
#include <private/eventPointLibP.h>
#include <private/eventPointStubLibP.h>
#include <private/excLibP.h>
#include <private/fioLibP.h>
#include <private/ftpLibP.h>
#include <private/isrLibP.h>
#include <private/jobLibP.h>
#include <private/kernelBaseLibP.h>
#include <private/kernelLibP.h>
#include <private/kwriteLibP.h>
#include <private/ledLibP.h>
#include <private/logLibP.h>
#include <private/memPartLibP.h>
#include <private/moduleHookLibP.h>
#include <private/offsetsP.h>
#include <private/pgMgrLibP.h>
#include <private/pgPoolLibP.h>
#include <private/pgPoolPhysLibP.h>
#include <private/pgPoolVirtLibP.h>
#include <private/poolLibP.h>
#include <private/pthreadLibP.h>
#include <private/qLibP.h>
#include <private/randomNumGenP.h>
#include <private/rtpDbgCmdLibP.h>
#include <private/rtpLibP.h>
#include <private/rtpSigLibP.h>
#include <private/sdLibP.h>
#include <private/shellCmdP.h>
#include <private/shellLibP.h>
#include <private/sigLibP.h>
#include <private/svDataCollectorLibP.h>
#include <private/sysDbgLibP.h>
#include <private/syscallLibP.h>
#include <private/taskMemLibP.h>
#include <private/timerLibP.h>
#include <private/tipLibP.h>
#include <private/tlsLibP.h>
#include <private/trgLibP.h>
#include <private/vmLibP.h>
#include <private/vxMemProbeLibP.h>
#include <private/workQLibP.h>
#include <private/wvFileUploadPathLibP.h>
#include <private/wvUploadPathP.h>
#include <ptyDrv.h>
#include <rBuffLib.h>
#include <rawFsLib.h>
#include <rebootLib.h>
#include <remLib.h>
#include <romfsLib.h>
#include <rtpLib.h>
#include <sched.h>
#include <sdLib.h>
#include <selectLib.h>
#include <semLib.h>
#include <shellConfigLib.h>
#include <shellInterpLib.h>
#include <shellLib.h>
#include <shlLib.h>
#include <sigLib.h>
#include <sigevent.h>
#include <smaLib.h>
#include <sockLib.h>
#include <stdio.h>
#include <string.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/int/vxbIsrHandler.h>
#include <symLib.h>
#include <sysLib.h>
#include <sysSymTbl.h>
#include <syscallLib.h>
#include <syscallTbl.h>
#include <sysctlLib.h>
#include <syslog.h>
#include <taskHookLib.h>
#include <taskLib.h>
#include <taskUtilLib.h>
#include <taskVarLib.h>
#include <tcfLib.h>
#include <tickLib.h>
#include <time.h>
#include <timerDev.h>
#include <timexLib.h>
#include <trgLib.h>
#include <ttyLib.h>
#include <tyLib.h>
#include <unistd.h>
#include <unldLib.h>
#include <usrLib.h>
#include <vmLib.h>
#include <vxAtomicLib.h>
#include <vxLib.h>
#include <vxbPru.h>
#include <vxdbgLibInit.h>
#include <vxworks/cafe_vx_timestamp.h>
#include <vxworks/ep/aux_clock_ep.h>
#include <vxworks/ep/performance_profiler_ep.h>
#include <vxworks/ep/sys_clock_ep.h>
#include <vxworks/ep/vxworks_kernel_vx.h>
#include <vxworks/ep/vxworks_polling.h>
#include <vxworks/ep/vxworks_rtp.h>
#include <wdLib.h>
#include <wvLib.h>
#include <wvTmrLib.h>


/* imports */

IMPORT char etext [];                   /* defined by loader */
IMPORT char end [];                     /* defined by loader */
IMPORT char edata [];                   /* defined by loader */

/* forward declarations */

IMPORT STATUS ledDrv (void);
IMPORT STATUS pruDrv (void);
IMPORT STATUS adrSpaceShellCmdInit (void);
IMPORT void usrBoardLibInit (void );
IMPORT void usrAimCpuInit (void);
IMPORT void vxCpuInit (void);                           IMPORT VOID vxMmuEarlyInit (UINT32 baseAddr, UINT32 linkAddr, UINT32 length);                         IMPORT void excVecBaseSet (UINT32 base);
IMPORT STATUS dbgShellCmdInit ();
IMPORT void vxbClkLibInit(void);
IMPORT STATUS usrFsShellCmdInit ();
IMPORT STATUS vxbDmaLibInit (void);
IMPORT STATUS eepromDrv (void);
IMPORT void endEtherHdrInit (void);
IMPORT STATUS eventPointLibInit (void);
IMPORT STATUS eventPointStubLibInit (void);
IMPORT void usrFdtInit (void *, int);
IMPORT STATUS vxbGpioLibInit (void);
IMPORT STATUS vxbIntLibInit (UINT32);
IMPORT STATUS vxbIsrHandlerInit (UINT32, UINT32);
extern STATUS logInit (int, int, int);
extern void miiBusMonitorTaskInit (void);
extern void miiBusLibInit (void);
IMPORT void mmanLibInit (void);
extern STATUS moduleShellCmdInit (void);
IMPORT STATUS hostnameSetup (char * );
IMPORT STATUS objInfoInit (void);
IMPORT void excOsmInit (int, int);
IMPORT STATUS vxbPinMuxLibInit (void);    
IMPORT void pmapInit (void);
extern STATUS umaskLibInit (int);
extern STATUS pxUserGroupLibInit (void);
IMPORT void usrRtpAppInit (void);
extern STATUS usrRtpLibInit();
IMPORT STATUS rtpShowShellCmdInit (void);
IMPORT void windScInit (void);
IMPORT void mmanScLibInit (void);
extern void usrPosixScInit (void);
IMPORT void sdScInit (void);
IMPORT void shlScLibInit (void);
IMPORT STATUS sockScLibInit (void);
IMPORT STATUS sysctlScLibInit (void);
IMPORT void secHashMd5OpensslInit(void);
IMPORT void secHashSha1OpensslInit(void);
IMPORT void secHashSha256OpensslInit(void);
IMPORT void semBCreateLibInit (void);
IMPORT void semCCreateLibInit (void);
IMPORT void semMCreateLibInit (void);
IMPORT STATUS sdShowShellCmdInit (void);
extern int sigeventLibInit (void);
IMPORT void usrStacktraceInit(void);
extern char * startupScriptFieldSplit (char * field);
IMPORT STATUS symShellCmdInit ();
extern void syslogLibInit (void);
IMPORT void taskCreateLibInit (void);
IMPORT STATUS taskShellCmdInit ();
IMPORT STATUS tyLibInit (int xoffThresh, int xonThresh, size_t wrtThresh, BOOL monTrapDisable);
extern STATUS unloadShellCmdInit (void);
extern void usrAppInit (void);
IMPORT STATUS vmShowShellCmdInit (void);
IMPORT STATUS vxbAuxClkLibInit (void);
IMPORT STATUS vxbSysClkLibInit (void);
IMPORT STATUS vxbTimestampLibInit (void);
IMPORT void vxmux_mux_mblk_init (void);


/* BSP_STUBS */



/* configlettes */

#include <sysComms.c>
#include <armMonitorStub.c>
#include <cafeConfigs.c>
#include <cafe_service_configlette.c>
#include <cplusdiabIos.c>
#include <cplusdiabLang.c>
#include <edrDefaultPolicy.c>
#include <edrStub.c>
#include <intrinsics.c>
#include <ipcom_config.c>
#include <ipcom_ipd_config.c>
#include <ipnet_config.c>
#include <ipnet_radvd_config.c>
#include <iptcp_config.c>
#include <pl310Access.c>
#include <sysClkInit.c>
#include <usrAdjustDtb.c>
#include <usrAimCpu.c>
#include <usrBanner.c>
#include <usrBoardLibInit.c>
#include <usrBootLine.c>
#include <usrBreakpoint.c>
#include <usrCache.c>
#include <usrCoprocs.c>
#include <usrCplus.c>
#include <usrDebugAgent.c>
#include <usrDosfs.c>
#include <usrExtCache.c>
#include <usrFdt.c>
#include <usrFtp.c>
#include <usrHwSysctl.c>
#include <usrIsrDefer.c>
#include <usrKernel.c>
#include <usrKernelStack.c>
#include <usrMemDesc.c>
#include <usrMmuInit.c>
#include <usrNetApplUtil.c>
#include <usrNetBoot.c>
#include <usrNetDaemon.c>
#include <usrNetEndLib.c>
#include <usrNetHostTblCfg.c>
#include <usrNetIfconfig.c>
#include <usrNetM2ifLib.c>
#include <usrNetRemoteCfg.c>
#include <usrNetSysctl.c>
#include <usrPmap.c>
#include <usrPosixSc.c>
#include <usrRomfs.c>
#include <usrRtpAppInitBootline.c>
#include <usrScript.c>
#include <usrSdMmc.c>
#include <usrSecureConfig.c>
#include <usrSemLib.c>
#include <usrSerial.c>
#include <usrShell.c>
#include <usrSma.c>
#include <usrSmaEnd.c>
#include <usrSmcProtocol.c>
#include <usrStacktrace.c>
#include <usrStandalone.c>
#include <usrStartup.c>
#include <usrStatTbl.c>
#include <usrSysSymTbl.c>
#include <usrSysctl.c>
#include <usrSyslog.c>
#include <usrSystemViewer.c>
#include <usrTip.c>
#include <usrVxbTimerSys.c>
#include <usrVxdbg.c>
#include <usrWindview.c>


/******************************************************************************
*
* usrKernelExtraInit - Extended kernel facilities
*/

void usrKernelExtraInit (void)
    {
                                        /* This component includes the hash library. This routine                         library supports the creation and maintenance of a                         chained hash table. Hash tables efficiently store hash                         nodes for fast access. They are frequently used for                         symbol tables, or other name-to-identifier routines.                         A chained hash table is an array of singly-linked list                         heads, with one list head per element of the hash table.                         During creation, a hash table is passed two                         user-definable routines, the hashing routine, and the                         hash node comparator. The hashTblCreate( ) routine                         creates a hash table. The hashTblInit( ) routine                         initializes a hash table. The hashTblDelete() routine                         deletes a hash table. The hashTblTerminate() routine                         terminates a hash table. The hashTblDestroy() routine                         destroys a hash table. The hashTblPut() routine puts a                         hash node into the specified hash table. The                         hashTblFind() routine finds a hash node that matches                         the specified key. The hashTblRemove() routine removes                         a hash node from a hash table. The hashTblEach()                         routine calls a routine for each node in a hash table.                         The hashFuncIterScale() routine provides an iterative                         scaling hashing function for strings. The hashFuncModulo()                         routine provides a hashing function using a remainder                         technique. The hashFuncMultiply() routine is a                         multiplicative hashing function. The hashKeyCmp() routine                         compares keys as 32-bit identifiers. The hashKeyStrCmp()                         routine compares keys based on strings they point to. */
    taskVarInit ();                     /* This component allows global variables to be made                         private to a task. */
    excSigLibInit();                    /* Enables signal generation on exception */
    sigInit (POSIX_SIGNAL_MODE);        /* This component provides the sigLib library. This                         library provides the signal interfaces in the RTP                         environment. The signal model in user-mode is designed                         to follow the POSIX process model. Signals alter the                         flow of control of tasks by communicating asynchronous                         events within or between task contexts. Using the APIs                         provided by this library, signals can be sent from an                         RTP task to either another RTP or a public task in                         another RTP. Signals can be sent to an RTP using the                         kill() or raise() routines, and will be caught by any                         task in that RTP which has unmasked that signal.                         Signals can also be sent to specific tasks in the                         current or another RTP using the taskKill() routine.                         Tasks that receive signals can either be waiting                         synchronously for the signal, or can have their                         signal mask setup to unblock that signal. If there                         is no such task waiting for the signal, the signal                         remains pended in the RTP and is delivered when                         one such task becomes available. */
    sigeventLibInit ();                 /* This component provides kernel signal event support.                         This is required for SIGEV_THREAD support in an RTP. */
                                        /* Includes the POSIX directory utilities. */
    pxUserGroupLibInit();               /* This component adds support for POSIX user and group                         in the kernel environment. */
    pthreadLibInit ();                  /* This component includes support for POSIX threads in                         the kernel. */
    clockLibInit (HIGH_RES_POSIX_CLOCK, POSIX_CLOCK_TIMER); /* This component includes support for POSIX clocks. */
    timerLibInit ();                    /* This component provides support for POSIX timers. */
    timerOpenInit ();                   /* open/close/unlink timer modules */
    randomSWNumGenInit ();              /* This component provides the randomSWNGenLib library.                         This library provides the implementation to generate                         software random number. When an application requests                         random bytes, the content of the destination buffer is                         first used as random seed. Thereafter the random state                         buffer is hashed and the hash output is returned as                         random bytes. If more bytes are requested than the                         hash output size, the hash output is used as seed and                         thereafter the random state buffer is hashed again.                         This process is repeated until the requested number                         of random bytes as been generated. */
    jobQueueLibInit();                  /* Includes the jobQueueLib library. It provides a prioritized work deferral mechanism. */
    jobQueueUtilInit ();                /* This component supports clean termination of job                   queue processing, and job queue and standard job pool 			           deletion. */
    syscallHookLibInit ();              /* Add hook routines to be called on system calls. */
    symLibInit ();                      /* Set of routines to manipulate symbols and symbol tables */
    }



/******************************************************************************
*
* usrIosExtraInit - extended I/O system
*/

void usrIosExtraInit (void)
    {
    jobTaskLibInit (JOB_TASK_STACK_SIZE); /* Task-level work deferral. */
    excInit (MAX_ISR_JOBS);             /* This component includes the excLib facility. This                         generic exception handling facility includes the                         following routines: excInit() which initializes the                         exception handling package; excJobAdd() which requests                         a task-level function call from the interrupt level;                         excJobAddDefer() which requests a deferred task-level                         function call; and excHookAdd() which specifies a                         routine to be called with exceptions. */
    syslogLibInit ();                   /* This component provides the system logging facility.                         The syslogLib facility can be configured to log                         messages before it is initialized. These messages are                         stored in static memory until initialization is complete                         and the system logger has started (tLogTask). As the                         POSIX clocks facility is also not yet initialized                         when log messages are written to static memory, the                         timestamp assigned to these messages is not accurate                         (it starts from 1970-01-01T00:00:00.000000Z). */
    logInit (consoleFd, MAX_LOG_MSGS, LOG_TASK_PRIORITY); /* Provides the logLib library. This library handles                         message logging. It is usually used to display error                         messages on the system console, but such messages can                         also be sent to a disk file or printer. The logMsg()                         and logTask() routines are the basic components of the                         logging system. The logMsg() routine has the same                         calling sequence as printf(), but instead of formatting                         and outputting the message directly, it sends the format                         string and arguments to a message queue. The logTask()                         task waits for messages on this message queue. It                         formats each message according to the format string and                         arguments in the message, prepends the ID of the sender,                         and writes it on one or more file descriptors that have                         been specified as logging output streams (by logInit()                         or subsequently set by logFdSet() or logFdAdd()). */
    pipeDrv (PIPE_MAX_OPEN_FDS);        /* This component enables the use of pipes. Pipes provide an alternative 			interface to the message queue facility that goes through the 			VxWorks I/O system. Pipe devices are managed by the pipeDrv virtual 			I/O device, and use the kernel message queue facility to bear the 			actual message traffic. Tasks write messages to pipes, which are then 			read by other tasks. This allows you to implement a client-server 			model of intertask communications. For more information about pipes, 			see the ioLib entry in the VxWorks 7 Application COREOS API Reference, 			and the pipeDrv entry in the VxWorks 7 Kernel COREOS API Reference. */
    stdioInit ();                       /* This component includes the buffered IO library.  */
    fioLibInit ();                      /* This component includes the formatting for printf, scanf,                         and so forth. */
    floatInit ();                       /* This component allows printf and other routines to                         format floats correctly.  */
    devInit (DEVICE_MANAGER_MAX_NUM_DEVICES); /* Device Manager */
    xbdInit ();                         /* This component includes support for an extended block device.  */
    fsMonitorInit ();                   /* This component includes the file system monitor.  */
    fsEventUtilLibInit ();              /* This component includes the file system event utilities.  */
    usrDosfsInit (DOSFS_DEFAULT_MAX_FILES, DOSFS_DEFAULT_CREATE_OPTIONS); /* MS-DOS-Compatible File System: dosFs main module */
    dosFsShowInit();                    /* Includes the show routines for Dos FS provided by the dosFsShow library. */
    rawFsInit (NUM_RAWFS_FILES);        /* Raw block device file system interface */
    ptyDrv ();                          /* Allows communication between processes */
    romfsFsInit (ROMFS_MAX_OPEN_FILES);  /* Includes a driver for read-only memory-based file system.                     It is initialized at runtime. */
    usrRomfsConfig ();                  /* Includes a read-only memory-based file system. */
    eepromDrv ();                       /* Attach the EEPROM device to standard I/O  */
    usrBootLineParse ((char *)BOOT_LINE_ADRS); /* This component provides support to parse some boot device                         configuration information. */
    }



/******************************************************************************
*
* usrShowInit - Enable object show routines
*/

void usrShowInit (void)
    {
    taskShowInit ();                    /* This component provides routines to show task-related                         information, such as task status, register values, and                         so forth. Task information is crucial as a debugging                         aid and user-interface convenience during the                         development cycle of an application. The taskShow()                         and taskRegsShow() routines are used to display task                         information. It is important to keep in mind that                         tasks may change their state or even get deleted                         between the time the information is gathered and the                         time it is utilized. Information provided by these                         routines should therefore be viewed as a snapshot of                         the system, and not relied upon unless the task is                         consigned to a known state, such as suspended. */
    classShowInit ();                   /* This component provides the classShow library which                         provides routines to show class-related information. */
    memShowInit ();                     /* This component includes the memShow library. This                         library contains memory partition information display                         routines. The memShow() routine shows blocks and                         statistics for the current heap partition. The                         memPartShow() routine shows available and allocated                         memory in the specified partition. */
    taskHookShowInit ();                /* This component provides support for the task hook show routines:                         taskHookShowInit() initializes the task hook show facility;                         taskCreateHookShow() shows the list of task create routines;                         taskDeleteHookShow() shows the list of task delete routines;                         taskSwitchHookShow() shows the list of task switch routines. */
    semShowInit ();                     /* This component includes the semShow library in your                         project. This library provides routines to show                         semaphore statistics, such as semaphore type,                         semaphore queuing method, tasks pended, and so forth.                         Routines in this library are meant to be used as                         debugging aids that display semaphore information to                         standard output. Due to the dynamic nature of                         semaphore operations, the information displayed may no                         longer be accurate by the time it is provided. */
    condVarShowInit ();                 /* This component provides the conVarShow() routine. This                         routine can be used to show information about a                         conditional variable. */
    msgQShowInit ();                    /* This component includes the msgQShow library in your                         project. This library provides routines to show                         message queue statistics, such as the task queuing                         method, messages queued, and receivers blocked. The                         msgQShowInit() routine links the message queue show                         facility into the VxWorks system. It is called                         automatically when you include this component in your                         project. The msgQShow() routine displays information                         about message queues, such as the task queuing method,                         messages queued, and receivers blocked. */
    wdShowInit ();                      /* This component provides the wdShow library. This                         library provides routines to show watchdog statistics,                         such as watchdog activity, a watchdog routine, and so                         forth. The wdShowInit() routine links the watchdog                         show facility into the VxWorks system. It is called                         automatically when this component is included in your                         project. The wdShow() routine shows information about                         a watchdog. */
    symShowInit ();                     /* Routines to display information about symbols and symbol tables */
    vmShowInit ();                      /* This component provides support for the virtual memory show                         routines for the shell C interpreter. */
    trgShowInit ();                     /* This component provides the trgShow library. This library 			provides routines to show event triggering information, 			such as list of triggers, associated actions, trigger states, 			and so on. The routine trgShowInit() links the triggering show 			facility into the VxWorks system. It is called automatically 			when this component is included. */
    rBuffShowInit ();                   /* Displays rBuff information and statistics about ring buffer performance. 			Optional and only available if you include RBUFF. */
    stdioShowInit ();                   /* stdio show routine */
                                        /* This component provides the handleShow library. This                         library provides routines to show handle information. */
    timerShowInit();                    /* This component includes a utility that allows the user to display                         a snap shot of the selected timer. It provides the timer_show()                         routine which shows information on a specified timer. It                         requires INCLUDE_POSIX_TIMERS. */
    adrSpaceShowInit();                 /* This component includes the adrSpaceShow() show function for 				the C interpreter. It can be used to display an overview of 				the address space usage at time of the call. */
    pgMgrShowInit();                    /* Page manager show routines: display information on 			page managers. */
    pgPoolShowInit();                   /* Generic page pool allocator show routines: display 			information on page pools. */
    rtpShowInit ();                     /* This component provides support for the rtpMemShow() 			routine. It displays the RTP's private mappings. */
    sdShowInit ();                      /* Display information on Shared Data. */
    shlShowInit ();                     /* Display information on Shared Libraries */
    coprocShowInit ();                  /* task coprocessor registers */
    }



/******************************************************************************
*
* usrWindviewInit - 
*/

void usrWindviewInit (void)
    {
    windviewConfig ();                  /* Initialize and control event logging of the System Viewer. */
    wvTmrRegister ((UINTFUNCPTR) sysTimestamp,					(UINTFUNCPTR) sysTimestampLock,	        				(FUNCPTR) sysTimestampEnable,					(FUNCPTR) sysTimestampDisable,					(FUNCPTR) sysTimestampConnect,					(UINTFUNCPTR) sysTimestampPeriod,					(UINTFUNCPTR) sysTimestampFreq); /* Supports BSP-specific timestamp routines.  */
    rBuffLibInit ();                    /* System Viewer ring of buffers for event logging */
    wvFileUploadPathLibInit ();         /* Initialize the path for the file upload. */
    }



/******************************************************************************
*
* usrAnalysisInit - Runtime Analysis initialization
*/

void usrAnalysisInit (void)
    {
    cafe_cfg_namespace_init ();         /* Initialize the Cafe system */
    cafeVxworksKernelInit ();           /* Source of VxWorks events for Cafe analysis tools */
    cafeVxWorksPollingInit();           /* Emit vxworks data, like resource usage, triggered by input event */
    cafe_rtp_init ();                   /* Provides support for RTPs in Cafe */
    cafeAggregatorInit ();              /* Generic event aggregator */
    cafe_vx_timestamp_init ();          /* Provide timestamp for Cafe, using VxWorks sysTimestamp */
    realTimeCfgRegister ();             /* Restore time spacing of events to realtime */
    timestampNormalizerCfgRegister ();  /* Handle target timestamp rollover for Cafe */
    elog_native_cfg_init ();            /* Event buffering for Cafe instruments on VxWorks target */
    cafeTimerRegister ();               /* Interval timer, using timestamps of incoming events */
    cafeTcfUploadInit ();               /* Stream event data to the host over TCF */
    sys_clock_ep_init ();               /* Generates Cafe events based on system clock ticks */
    aux_clock_ep_init ();               /* Generates Cafe events based on auxiliary clock ticks */
    performance_profiler_od_ep_init (); /* Statistical sampler based on arbitrary events */
    cafeSysinfoInit ();                 /* Event processor providing bsp-specific information about the system */
    cafeFstreamReaderInit ();           /* Cafe event processor to read events from a file */
    cafeFstreamWriterInit ();           /* Cafe event processor to write events to a file */
    cafePrintRegister();                /* Event processor which prints all event data it receives to the console */
    }



/******************************************************************************
*
* usrLoaderInit - The target loader initialization sequence
*/

void usrLoaderInit (void)
    {
    moduleLibInit ();                   /* Support library for module entities */
    loadElfInit ();                     /* ELF loader */
    loadLibInit (STORE_ABS_SYMBOLS);    /* Allows modules to be downloaded into running targets */
    unldLibInit ();                     /* Permits removal of dynamically downloaded modules */
    moduleHookLibInit ();               /* Pluggable hooks to extend the loader/unloader behavior */
    }



/******************************************************************************
*
* usrSymTblInit - Enable onboard symbol tables
*/

void usrSymTblInit (void)
    {
    usrSysSymTblInit ();                /* initialize system symbol table */
    usrStandaloneInit ();               /* Preferred method if not booting from the network */
    usrStatTblInit ();                  /* Table of error strings for perror() */
    loadRtpSymLibInit ();               /* Symbol table support for RTPs. Allows to register 			symbols when or after loading RTPs and shared 			libraries as a debugging aid. */
    }



/******************************************************************************
*
* usrShellCmdInit - The kernel shell commands initialization sequence
*/

void usrShellCmdInit (void)
    {
    taskShellCmdInit ();                /* List of commands for the shell command interpreter related to tasks. */
    dbgShellCmdInit ();                 /* List of commands for the shell command interpreter related to debugging. */
    symShellCmdInit ();                 /* List of commands for the shell command interpreter related to symbol access. */
    usrFsShellCmdInit ();               /* List of commands for the shell command interpreter related to file system. */
    vmShowShellCmdInit ();              /* Virtual Memory show commands for the shell command interpreter.  */
    adrSpaceShellCmdInit ();            /* Address space commands for the shell command interpreter.  */
    sdShowShellCmdInit ();              /* Shared data show commands for the shell command interpreter.  */
    moduleShellCmdInit();               /* Target loader commands for the shell command interpreter. */
    unloadShellCmdInit();               /* Unloader commands for the shell command interpreter.  */
    rtpShellCmdInit ();                 /* List of commands for the shell command interpreter related to processes. */
    rtpShowShellCmdInit ();             /* List of process show commands for the shell command interpreter.  */
    shlShellCmdInit ();                 /* List of SHL commands for the shell command interpreter */
    }



/******************************************************************************
*
* usrShellInit - The kernel shell initialization sequence
*/

void usrShellInit (void)
    {
    shellLibInit ();                    /* Handles the shell core files */
    dbgLibInit ();                      /* Breakpoints and stack tracer on target. Not needed for 			remote debugging with Workbench. */
    vxdbgRtpLibInit ();                 /* Initialize process debugging library. */
    ledModeRegister (viLedLibInit);     /* Editing mode similar to the Vi editing mode */
    ledModeRegister (emacsLedLibInit);  /* Editing mode similar to the emacs editing mode */
    shellInterpRegister (shellInterpCInit); /* The C interpreter for the kernel shell */
    shellInterpRegister (shellInterpCmdInit); /* The command interpreter for the kernel shell */
    usrShellCmdInit ();                 /* The kernel shell commands initialization sequence */
    usrStartupScript (startupScriptFieldSplit (sysBootParams.startupScript)); /* Initial kernel shell script run at VxWorks startup */
    usrShell ();                        /* The kernel shell is spawned as a task. Any function                            that is invoked from the kernel shell, rather than                             spawned, runs in the shell's context. The task name                            for a shell on the console is "tShell0". The kernel                            shell is re-entrant, and more than one shell task can                          run at a time (hence the number suffix). In addition,                          if a user logs in remotely (using rlogin or telnet) to                         a VxWorks target, the name reflects that fact as well.                         For example, "tShellRem1". The "tShell" basename is                            configurable, see the VxWorks Kernel Shell User's Guide. */
    }



/******************************************************************************
*
* usrToolsInit - Software development tools
*/

void usrToolsInit (void)
    {
    timexInit ();                       /* utility to measure function execution time */
    eventPointStubLibInit ();           /* stub eventpoint implementation */
    eventPointLibInit ();               /* eventpoints instrumentation */
    dataCollectorLibInit ();            /* This component provides data collectors for generic analysis 			tools. See the dataCollectorLib entry in the VxWorks 7 Kernel 			DEBUG ANALYSIS API Reference for detailed description of 				the different routines in this library.  */
    svDataCollectorLibInit ();          /* Data collectors for System Viewer */
    usrLoaderInit ();                   /* The target loader initialization sequence */
    usrStacktraceInit();                /* Stack tracing logic */
    usrSymTblInit ();                   /* Enable onboard symbol tables */
    trgLibInit ();                      /* Adds support for triggering for system and user events */
    usrVxdbgInit (VXDBG_EVT_TASK_PRIORITY,						   VXDBG_EVT_TASK_OPTIONS,						   VXDBG_EVT_TASK_STACK_SIZE); /* Support for the runtime debug library */
    usrWindviewInit ();                 /* usrWindviewInit */
    usrAnalysisInit ();                 /* Runtime Analysis initialization */
    tcfLibCheckStart ();                /* The Application Mode Agent is a modular TCF based communication system for the purpose of debugging, profiling, code patching and other device software development needs. This agent does not provide a secured network connection; It is advised to not include it in deployed devices or make sure it is not automatically started at boot time through boot line parameter or dedicated component. */
    tlsLoadLibInit ();                  /* __thread variables loader support */
    rtpDbgCmdLibInit ();                /* Allows to send commands to perform operations in a RTP context */
    usrTipInit (TIP_CONFIG_STRING, TIP_ESCAPE_CHARACTER); /* interactive utility to connect to and manage multiple serial lines */
    usrShowInit ();                     /* Enable object show routines */
    usrRtpLibInit ();                   /* List of commands for the shell C interpreter related to RTPs. */
    usrShellInit ();                    /* The kernel shell initialization sequence */
    usrSmaInit ();                      /* Support for stop mode debug agent.                         This agent does not provide a secured network connection;                         it is advised to not include it in your development system                         or make sure it is not automatically started at boot time                         through boot line parameter or dedicated component. */
    }



/******************************************************************************
*
* usrKernelCreateInit - Object creation routines
*/

void usrKernelCreateInit (void)
    {
    semDeleteLibInit ();                /* This component provides the semDeleteLib library that                         contains routines that terminate and delete semaphores.                         The semDelete() call terminates a semaphore and                         deallocates any associated memory. The deletion of a                         semaphore unblocks tasks pended on that semaphore; the                         routines that were pended return ERROR. Take care when                         deleting semaphores, particularly those used for mutual                         exclusion, to avoid deleting a semaphore out from under                         a task that already has taken (owns) that semaphore.                         Applications should adopt the protocol of only deleting                         semaphores that the deleting task has successfully taken. */
    taskCreateLibInit ();               /* This component provides the capability to                         dynamically instantiate and delete tasks. */
    taskMemLibInit ();                  /* taskStackAlloc and taskStackFree routines */
    msgQCreateLibInit ();               /* This component provides the msgQCreateLib library. This                         library contains routines for creating and deleting                         message queues. The routines to initialize and operate                         on message queues are implemented in msgQLib. */
    wdCreateLibInit ();                 /* This component provides wdCreateLib. This library                         provides creation and deletion routines for the                         watchdog facility. The routines to initialize and                         operate on watchdogs are implemented in wdLib. */
    }



/******************************************************************************
*
* usrRtpInit - RTP init routines
*/

void usrRtpInit (void)
    {
    rtpHookLibInit (RTP_HOOK_TBL_SIZE); /* Add hook routines to be called on RTP operations. */
    rtpLibInit (RTP_HOOK_TBL_SIZE, SYSCALL_HOOK_TBL_SIZE, 				    RTP_SIGNAL_QUEUE_SIZE, 				    TASK_USER_EXC_STACK_OVERFLOW_SIZE, 				    TASK_USER_EXEC_STACK_OVERFLOW_SIZE, 				    TASK_USER_EXEC_STACK_UNDERFLOW_SIZE, 				    RTP_FD_NUM_MAX, RTP_MEM_FILL); /* Real Time Process. */
    rtpSigLibInit ();                   /* POSIX signal support for RTPs */
    sdLibInit (SD_HOOK_TBL_SIZE);       /* provide data sharing between RTPs or kernel */
    shlLibInit ();                      /* provide sharing of code between RTPs */
    }



/******************************************************************************
*
* usrKernelCoreInit - Core kernel facilities
*/

void usrKernelCoreInit (void)
    {
    eventLibInit (VXEVENTS_OPTIONS);    /* This component provides event facilities. VxWorks                         events provide a means of communication and                         synchronization between tasks and other tasks,                         interrupt service routines (ISRs) and tasks,                         semaphores and tasks, and message queues and tasks.                         Events can be sent explicitly by tasks and ISRs,                         and can be sent when message queues or semaphores                         are free. Only tasks can receive events. Events can                         be used as a lighter-weight alternative to binary                         semaphores for task-to-task and ISR-to-task                         synchronization (because no object must be created).                         They can also be used to notify a task that a semaphore                         has become available, or that a message has arrived on                         a message queue. The events facility provides a                         mechanism for coordinating the activity of a task using                         up to thirty-two events that can be sent to it explicitly                         by other tasks and ISRs, or when semaphores and message                         queues are free. A task can wait on multiple events from                         multiple sources. Events thereby provide a means for                         coordination of a complex matrix of activity without                         allocation of additional system resources. */
                                        /* This library provides the interface to VxWorks binary                         semaphores. Binary semaphores are the most versatile,                         efficient, and conceptually simple type of semaphore.                         They can be used to: control mutually exclusive access                         to shared devices or data structures, or synchronize                         multiple tasks, or task-level and interrupt-level                         processes. Binary semaphores form the foundation of                         numerous VxWorks facilities. */
                                        /* This component provides the semCLib library. This                         library provides the interface to VxWorks counting                         semaphores. Counting semaphores are useful for                         guarding multiple instances of a resource. A counting                         semaphore can be viewed as a cell in memory whose                         contents keep track of a count. */
                                        /* This component provides the semRWLib and windSemRWLib                         libraries. semRWLib provides the interface to VxWorks                         reader/writer semaphores. Reader/writer semaphores                         provide a method of synchronizing groups of tasks that                         can be granted concurrent access to a resource with                         those tasks that require mutually exclusive access to                         that resource. Typically, this correlates to those tasks                         that intend to modify a resource and those which intend                         only to view it. windSemRWLib contains the                         implementation of the windSemRWDelete() routine. A call                         to windSemRWDelete() unblocks any tasks pending on                         a read/write semaphore. If the deletion safety option is                         enabled, any and all owners have their safety count                         decremented. If the inversion safe option is enabled,                         any and all owners have their priority inheritence                         information updated. */
    semOpenInit ();                     /* open/close/unlink semaphore modules */
    condVarOpenInit ();                 /* open/close/unlink condition variable modules */
    msgQLibInit ();                     /* This component provides message queue support using the                         msgQLib library. This library contains routines for                         creating and using message queues, the primary inter-task                         communication mechanism in VxWorks. Message queues allow                         a variable number of messages (varying in length) to be                         queued in first-in-first-out (FIFO) order. Any task or                         interrupt service routine can send messages to a                         message queue. Any task can receive messages from a                         message queue. Multiple tasks can send to, and receive                         from, the same message queue. Full-duplex communication                         between two tasks generally requires two message queues,                         one for each direction. For more information, see the                         reference information for msgQLib. */
    msgQOpenInit ();                    /* open/close/unlink message queue modules */
    wdLibInit ();                       /* This library provides a general watchdog timer facility.                         Any task can create a watchdog timer and use it to run a                         specified routine in the context of the system-clock ISR,                         after a specified delay. Once a timer has been created                         with wdCreate(), it can be started with wdStart(). The                         wdStart() routine specifies what routine to run, a                         parameter for that routine, and the amount of time (in                         ticks) before the routine is to be called. The timeout                         value is in ticks as determined by the system clock;                         see sysClkRateSet() for more information. After the                         specified delay ticks have elapsed, unless wdCancel()                         is called first to cancel the timer, the timeout routine                         is invoked with the parameter specified in the wdStart()                         call. The timeout routine is invoked whether the task                         which started the watchdog is running, suspended, or                         deleted. The timeout routine executes only once per                         wdStart() invocation; there is no need to cancel a timer                         with wdCancel() after it has expired, or in the                         expiration callback itself. Note that the timeout                         routine is invoked at interrupt level, rather than in                         the context of the task. Thus, there are restrictions                         on what the routine can do. Watchdog routines are                         constrained to the same rules as interrupt service                         routines. For example, they cannot take semaphores,                         issue other calls that can block, or use I/O system                         routines like printf(). Note: Watchdog routine invocation                         can be deferred. As such isrIdCurrent is either a valid                         ISR_ID or is NULL in the case of deferral. */
    taskOpenInit ();                    /* open/close/unlink task modules */
    vxdbgHooksInit ();                  /* hooks for the runtime debug library */
    vxdbgRtpLibInit ();                 /* Initialize process debugging library. */
    }



/******************************************************************************
*
* usrClkInit - clock system initialization
*/

void usrClkInit (void)
    {
    usrVxbTimerSysInit();               /* Includes support for the VxBus timer. */
    vxbSysClkLibInit();                 /* Includes support for the VxBus system clock. */
    vxbAuxClkLibInit();                 /* Includes support for the VxBus auxiliary clock. */
    sysClkInit ();                      /* Initializes the system clock. */
    vxbTimestampLibInit();              /* Includes support for the VxBus timestamps. */
    }



/******************************************************************************
*
* usrScInit - The system call initialization sequence
*/

void usrScInit (void)
    {
    usrPosixScInit ();                  /* This component includes system call handlers for                         POSIX-based calls. */
    sockScLibInit ();                   /* System call support for socket API */
    memRtpLibInit (RTP_HEAP_INIT_SIZE,RTP_HEAP_DEFAULT_OPTIONS); /* Initializes memory parameters to pass to RTP */
    windScInit ();                      /* system call handlers for WIND kernel  */
    mmanScLibInit ();                   /* system call handlers for memory management */
    sdScInit ();                        /* system call handlers for shared data */
    shlScLibInit ();                    /* system call handlers for shared libraries */
                                        /* system call handler for RTP prlimits */
                                        /* system call handler for pipes */
    iosScLibInit ();                    /* IOS system calls */
    sysctlScLibInit ();                 /* System call support for sysctl() */
    }



/******************************************************************************
*
* usrNetMib2Init - Initialize MIB2
*/

void usrNetMib2Init (void)
    {
    usrNetM2IfStart ();                 /* MIB2 Variables for Interface Layer */
    }



/******************************************************************************
*
* usrNetworkInit0 - Initialize the network subsystem for drivers
*/

void usrNetworkInit0 (void)
    {
    netBufLibInitialize (NETBUF_LEADING_CLSPACE_DRV); /* Network Buffer Library */
    netBufPoolInit ();                  /* netBufLib Generic Pool */
    linkBufPoolInit ();                 /* Network Buffer Library */
    vxmux_null_buf_init ();             /* Minimal clusterless network pool implementation for IPCOM */
    usrNetDaemonInit(NET_JOB_NUM_CFG,                        NET_TASK_DEFAULT_PRIORITY, NET_TASK_OPTIONS,                        NET_TASK_STACKSIZE); /* This component includes support for the network daemon.                         The network daemon, tNet0, performs network driver and                         network protocol processing for the VxWorks network stack. */
    usrNetMib2Init ();                  /* Initialize MIB2 */
    muxCommonInit();                    /* MUX common support (all protocol and device styles) */
    muxLibInit();                       /* Support muxBind() protocols and their APIs */
    mux2LibInit();                      /* Support mux2Bind() protocols and their APIs */
    vxmux_mux_mblk_init ();             /* MUX private support for M_BLK/Ipcom_pkt conversion */
    mux2OverEndInit();                  /* Support mux2Bind() protocols over END-style devices */
    endEtherHdrInit ();                 /* M_BLK ethernet/802.3 header build and parse */
    endLibInit();                       /* Support for END-style network devices. */
    }



/******************************************************************************
*
* usrIosCoreInit - core I/O system
*/

void usrIosCoreInit (void)
    {
    iosInit (NUM_DRIVERS, NUM_FILES, "/null"); /* Basic IO system component */
    iosPathLibInit ();                  /* File System IO component */
    iosRmvLibInit ();                   /* Removable IO component */
    iosPxLibInit (IOS_POSIX_PSE52_MODE); /* POSIX IO component */
    iosRtpLibInit ();                   /* RTP IO component */
                                        /* Miscellaneous IO component */
    selectInit (NUM_FILES);             /* This provides the kernel selectLib library which includes select() 			and related functions. */
    usrIsrDeferInit ();                 /* ISR deferral facility */
    tyLibInit (TYLIB_XOFF_PCNT, TYLIB_XON_PCNT, 				TYLIB_WRT_THRESHLD, TYLIB_MON_TRAP_DISABLE); /* Allows communication between processes */
    ttyDrv ();                          /* Provides terminal device access to serial channels. */
    coprocLibInit();                    /* Generalized coprocessor support */
    eepromDrv ();                       /* Attach the EEPROM device to standard I/O  */
    pruDrv ();                          /* Attach the LED device to standard I/O  */
    ledDrv ();                          /* Attach the LED device to standard I/O  */
    }



/******************************************************************************
*
* usrNetHostInit - 
*/

void usrNetHostInit (void)
    {
    usrNetHostTblSetup ();              /* host table support */
                                        /* get servce by name */
    hostnameSetup (pTgtName);           /* assign local hostname to target */
    }



/******************************************************************************
*
* usrNetRemoteInit - 
*/

void usrNetRemoteInit (void)
    {
    remLibInit(RSH_STDERR_SETUP_TIMEOUT); /* Remote Command Library */
    remLibSysctlInit();                 /* Network sysctl tree support for remlib */
    usrFtpInit();                       /* File Transfer Protocol (FTP) library */
                                        /* Allows access to file system on boot host */
    usrNetRemoteCreate ();              /* Allows access to file system on boot host */
    }



/******************************************************************************
*
* usrNetUtilsInit - Initialize network utility routines
*/

void usrNetUtilsInit (void)
    {
    usrNetIfconfigInit ();              /* ifconfig */
    }



/******************************************************************************
*
* usrNetworkInit - Initialize the reset of the network subsystem
*/

void usrNetworkInit (void)
    {
    usrNetBoot ();                      /* Copy boot parameters for futher use by network */
    usrNetmaskGet ();                   /* Extracts netmask value from address field */
    usrNetSysctlInit();                 /* Networking System control support */
    usrNetHostInit ();                  /* initialize host table */
    rtpHostLibInit ();                  /* network sysctl tree support for host tables */
    rtpGetaddrinfoInit ();              /* network sysctl tree support for RTP getaddrinfo() */
    usrNetRemoteInit ();                /* initialize network remote I/O access */
    usrNetEndLibInit();                 /* Support for network devices using MUX/END interface. */
    ipcom_config_usr_create ();         /* Includes the IPCOM user-side functions. */
    ipcom_config_create ();             /* Includes VxWorks IPCOM support. */
    netSysctlInit (TRUE);               /* Integrates IPNet sysctl nodes into VxWorks sysctl tree. */
    usrNetUtilsInit ();                 /* Initialize network utility routines */
    }



/******************************************************************************
*
* usrSecHashInit - Initialize the secHash providers
*/

void usrSecHashInit (void)
    {
    secHashMd5OpensslInit();            /* Registers an MD5 provider for the secHash MD5 API. 			The implementation is based on the low-level openssl APIs. 			This provider does not require the full openssl lib to be linked in. */
    secHashSha1OpensslInit();           /* Registers an SHA1 provider for the secHash SHA1 API. 				The implementation is based on the low-level openssl APIs. 				This provider does not require the full openssl lib to be linked in. */
    secHashSha256OpensslInit();         /* Registers an SHA256 provider for the secHash SHA256 API. 			The implementation is based on the low-level openssl APIs. 			This provider does not require the full openssl lib to be linked in. */
    }



/******************************************************************************
*
* usrRoot - Entry point for post-kernel initialization
*/

void usrRoot (char *pMemPoolStart, unsigned memPoolSize)
    {
    usrKernelCoreInit ();               /* Core kernel facilities */
    poolLibInit ();                     /* This module contains the Memory Pool library. Pools                         provide a fast and efficient memory management when an                         aplication uses a large number of identically sized                         memory blocks (e.g. structures, objects) by minimizing                         the number of allocations from a memory partition. The                         use of pools also reduces possible fragmentation                         caused by frequent memory allocation and freeing.                         A pool is a dynamic set of statically sized memory                         blocks. All blocks in a pool are of the same size. The                         size of blocks are specified at pool creation and all                         the blocks will be aligned on the block size; the                 block size must be a power of 2. */
    memInit (pMemPoolStart, memPoolSize,                                  MEM_PART_DEFAULT_OPTIONS); /* This component provides the memLib library. This                         library provides the API for allocating and freeing                         blocks of memory of arbitrary size from an RTP heap.                         This library implements an RTP heap as a dedicated                         memory partition. One private heap is created                         automatically for every RTP. The library provides                         ANSI allocation routines and enhanced memory management                         features, including error handling and aligned                         allocation. Most of the memLib routines are simple                         wrappers to the memory partition management functions                         which implement the actual memory management                         functionalty. For more information about the                         memory partition management facility, see the reference                         entry for memPartLib. */
    memPartLibInit (pMemPoolStart, memPoolSize); /* This component includes the core memory partition                         manager. */
    kProxHeapInit (pMemPoolStart, memPoolSize); /* This component provides the kProxHeapLib library.                         This library provides the routine that initializes the                         kernel proximity heap partition. The kernel proximity                         heap is a memory partition created in the kernel system                         memory region. */
    pgPoolLibInit();                    /* Generic Page Pool Allocator */
    pgPoolVirtLibInit();                /* Page Pool Allocator for Virtual Space */
    pgPoolPhysLibInit();                /* Page Pool Allocator for Physical Space */
                                        /* basic MMU component */
    usrMmuInit ();                      /* MMU global map support */
    pmapInit();                         /* provides the functionality to map or unmap physical                         addresss to the kernel/RTP context. */
    kCommonHeapInit (KERNEL_COMMON_HEAP_INIT_SIZE, KERNEL_COMMON_HEAP_INCR_SIZE); /* The kernel common heap is the memory partition used by                         the kernel and kernel applications for dynamic memory                         allocation. Physical memory is allocated from the global                         RAM pool, virtual memory is allocated from the kernel                         virtual memory pool region. It is managed using the                         standard ANSI memory allocation functions, malloc(),                         free(), and so on. For more information, see the                         reference entries for memPartLib and memLib. */
    usrKernelCreateInit ();             /* Object creation routines */
    usrNetApplUtilInit ();              /* Stack and Application Logging Utility */
    memInfoInit ();                     /* This component provides the memInfo library. This library                         provides routines for obtaining information about a                         memory partition or the kernel heap. */
    envLibInit (ENV_VAR_USE_HOOKS);     /* This component provides the envLib library. This                         library provides a UNIX-compatible environment                         variable facility. Environment variables are created                         or modified with a call to putenv():                         putenv ("variableName=value"); . The value of a                         variable can be retrieved with a call to getenv(),                         which returns a pointer to the value string. Tasks can                         share a common set of environment variables, or they                         can optionally create their own private environments,                         either automatically when the task create hook is                         installed, or by an explicit call to envPrivateCreate().                         The task must be spawned with the VX_PRIVATE_ENV option                         set to receive a private set of environment variables.                         Private environments created by the task creation hook                         inherit the values of the environment of the task that                         called taskSpawn() (because task create hooks run in the                         context of the calling task). */
    edrStubInit ();                     /* Error detection and reporting (ED&R) error-injection stub. */
    usrSecHashInit ();                  /* Initialize the secHash providers */
    boardInit();                        /* call the boardInit() routine during system startup */
    usrDebugAgentBannerInit ();         /* Toggles agent specific elements in banner */
    usrShellBannerInit ();              /* Toggles shell specific elements in banner */
    usrSmaBannerInit ();                /* Add stop mode debug agent specific elements in banner */
    vxbClkLibInit();                    /* vxBus device clock subsystem */
    vxbDmaLibInit();                    /* vxBus DMA subsystem */
    vxbGpioLibInit();                   /* vxBus GPIO subsystem */
    vxbIsrHandlerInit (VXB_MAX_INTR_VEC, VXB_MAX_INTR_CHAIN); /* This component supports VxBus ISR handler module                     initialization. */
    vxbIntLibInit (VXB_MAX_INTR_DEFER); /* This component includes support for VxBus interrupt library                     initialization. */
    miiBusFdtLibInit();                 /* MII bus FDT subsystem */
    miiBusLibInit();                    /* Includes the MII bus subsystem.  */
    vxbPinMuxLibInit();                 /* vxBus pinmux subsystem */
    vxbParamLibInit ();                 /* VxBus driver parameter utilities */
    excOsmInit (TASK_USER_EXC_STACK_OVERFLOW_SIZE, VM_PAGE_SIZE); /* Handler for Exception Stack Overflow */
    taskStackGuardPageEnable();         /* insert underflow and overflow guard pages to kernel task stacks */
    taskStackNoExecEnable();            /* Exception and execution stacks for RTP and kernel 			tasks will be marked non-executable. */
    usrSysctlInit();                    /* System control function */
    erfLibInit (ERF_MAX_USR_CATEGORIES, ERF_MAX_USR_TYPES, ERF_TASK_STACK_SIZE); /* Provides support for the event reporting framework for use by other libraries. */
    usrIosCoreInit ();                  /* core I/O system */
    usrNetworkInit0 ();                 /* Initialize the network subsystem for drivers */
    usrHwSysctlInit();                  /* System control hardware info registration */
    vxbLibInit ();                      /* VxBus subsystem */
    intCpuUnlock(0);                     sysIntEnableFlagSet (); /* Enable interrupts at appropriate point in root task */
    usrSerialInit ();                   /* Serial IO component */
    usrClkInit ();                      /* clock system initialization */
    pgMgrBaseLibInit();                 /* Basic Page Manager Library */
    pgMgrLibInit();                     /* Page Manager Library */
    usrRtpInit ();                      /* RTP init routines */
    mmanLibInit ();                     /* Memory mapping and unmapping routines */
    usrKernelExtraInit ();              /* Extended kernel facilities */
    miiBusMonitorTaskInit();            /* This component spawns the MII bus monitor task. */
    usrSdMmcInit();                     /* FSL_IMX_SDHC config */
    usrIosExtraInit ();                 /* extended I/O system */
    sockLibInit ();                     /* Socket API */
    selTaskDeleteHookAdd ();            /* selectInit, part 2, install task delete hook */
    cplusCtorsLink ();                  /* run compiler generated initialization functions at system startup */
    usrCplusLibInit ();                 /* Basic support for C++ applications */
    cplusDemanglerInit ();              /* Support library for kernel shell and loader: provides human readable forms of C++ identifiers */
    usrScInit ();                       /* The system call initialization sequence */
    usrNetworkInit ();                  /* Initialize the reset of the network subsystem */
    usrBanner ();                       /* Displays the Wind River banner on startup. */
    usrToolsInit ();                    /* Software development tools */
    usrAppInit ();                      /* Calls usrAppInit() (in your usrAppInit.c project file)                         after startup. */
    usrRtpAppInit ();                   /* Launch RTP from a user-defined function. */
    usrRtpAppInitBootline ();           /* Launch RTP from a string-encoded list in the boot parameters. */
    }



/******************************************************************************
*
* usrInit - Pre-kernel initialization
*/

void usrInit (int startType)
    {
    {                         void (* cpuInit) (void) = vxCpuInit;                         void (* mmuEarlyInit) (UINT32, UINT32, UINT32) = vxMmuEarlyInit;                         cpuInit = (void (*) (void)) ((uintptr_t) cpuInit + LOCAL_MEM_PHYS_ADRS);                         mmuEarlyInit = (void (*) (UINT32, UINT32, UINT32)) ((uintptr_t) mmuEarlyInit + LOCAL_MEM_PHYS_ADRS);                         cpuInit ();                         mmuEarlyInit (STATIC_MMU_TABLE_BASE, LOCAL_MEM_LOCAL_ADRS, IMA_SIZE);                         excVecBaseSet ((UINT32)VEC_BASE_ADRS);                         } /* ARM arch cpu initialization */
    armMonitorStub(startType);          /* monitor stub (ARCH internal usage) */
    sysStart (startType);               /* Clears BSS and set up the vector table base address. */
    _func_kprintf = kprintf;            /* formatted kernel print facility */
    usrAdjustDtb ();                    /* adjust the dtb address (ARCH internal usage) */
    usrFdtInit ((void*)DTB_RELOC_ADDR, (int)DTB_MAX_LEN); /* Flat Device Tree library */
    usrBoardLibInit();                  /* Initialize board subsystem to supply BSP access APIs */
    usrAimCpuInit ();                   /* This component includes CPU library support. */
    cacheLibInit (USER_I_CACHE_MODE, USER_D_CACHE_MODE); /* include cache support */
    excShowInit ();                     /* This component provides detailed descriptions of exceptions                         in the error logs. */
    excVecInit ();                      /* This component provides architecture-dependent facilities                         for handling processor exceptions. */
    usrExtL2CacheInit();                /* Cortex A9 External L2 Cache Support */
    vxPl310AccessInit();                /* FDT L2 Cache PL310 driver */
    usrCacheEnable ();                  /* optionally enable caches */
    objOwnershipInit ();                /* This component provides the objOwnerLib library which                         includes object ownership routines. */
    objInfoInit ();                     /* This component provides object management routines                         that require lookup in a list of objects, such as                         the objNameToId() routine. */
    objLibInit ((OBJ_ALLOC_FUNC)FUNCPTR_OBJ_MEMALLOC_RTN,                                     (OBJ_FREE_FUNC)FUNCPTR_OBJ_MEMFREE_RTN,                                       OBJ_MEM_POOL_ID,                                                              OBJ_LIBRARY_OPTIONS); /* This component provides the objLib library. This                         library provides the interface to the VxWorks user                         object management facilities. */
    vxMemProbeInit ();                  /* Initialize vxMemProbe exception handler support */
    wvLibInit ();                       /* Low-level kernel instrumentation needed by System Viewer */
    classListLibInit ();                /* This component provides object class list management. */
    semLibInit ();                      /* Semaphores are the basis for synchronization and mutual                         exclusion in VxWorks. They are powerful in their                         simplicity and form the foundation for numerous VxWorks                         facilities. Different semaphore types serve different                         needs, and while the behavior of the types differs,                         their basic interface is the same. This library provides                         semaphore routines common to all VxWorks semaphore types.                         For all types, the two basic operations are semTake()                         and semGive(), the acquisition or relinquishing of a                         semaphore. Mutex semaphores offer the greatest speed                         while binary semaphores offer the broadest applicability.                         The semLib library provides all of the semaphore                         operations, including routines for semaphore control,                         deletion, and information. Semaphores must be validated                         before any semaphore operation can be undertaken. An                         invalid semaphore ID results in ERROR, and an                         appropriate `errno' is set. */
                                        /* This library provides the interface to VxWorks                         mutual-exclusion semaphores. Mutual-exclusion semaphores                         offer convenient options suited for situations requiring                         mutually exclusive access to resources. Typical                         applications include sharing devices and protecting                         data structures. Mutual-exclusion semaphores are used                         by many higher-level VxWorks facilities. The                         mutual-exclusion semaphore is a specialized version of                         the binary semaphore, designed to address issues inherent                         in mutual exclusion, such as recursive access to resources,                         priority inversion, and deletion safety. The fundamental                         behavior of the mutual-exclusion semaphore is                         identical to the binary semaphore (see the reference entry                         for semBLib), except for the following restrictions: it                         can only be used for mutual exclusion; it can only be                         given by the task that took it; it may not be taken or                         given from interrupt level; and the semFlush() operation is                         illegal. These last two operations have no meaning in                         mutual-exclusion situations. */
                                        /* This component provides the semMCreate() routine. This                         routine creates mutual-exclusion semaphores. */
    condVarLibInit ();                  /* Condition variables allow tasks to wait on both an                         event to occur, and to acquire a mutex that protects                         an object associated with that event. Thus, condition                         variables are acquired by taking a mutex, and waiting                         on the condition variable. When the event is signaled,                         a task is unblocked and the mutex given to that task,                         which can then access the protected resource. Note                         that signaling a condition variable only unblocks a                         waiting resource - if a condition variable is signaled                         before a task waits on that condition variable, then                         the waiting task must wait until the condition variable                         is signaled again. A condition variable can only be                         associated with one mutex at a time (if there are                         waiters, all waiters must have held the same mutex                         when the condition variable was waited on. No condition                         variable routine is callable from an ISR. For detailed                         descriptions of the routines provided in this                         component, see the reference entry for condVarLib  . */
    classLibInit ();                    /* This component provides object class management. */
    kernelBaseInit ();                  /* This is a required component. DO NOT REMOVE. */
    taskCreateHookInit ();              /* This component provides user callouts on task creation                         and deletion. */
                                        /* __thread variables support */
    sysDebugModeInit ();                /* This component provides a flag indicating the system                         is in 'debug' mode. */
    umaskLibInit(UMASK_DEFAULT);        /* This component adds support for the POSIX file mode                        creation mask in the kernel environment. */
    usrKernelInit (VX_GLOBAL_NO_STACK_FILL); /* Context switch and interrupt handling (DO NOT REMOVE). */
    }



/******************************************************************************
*
* tcfServicesInit - Register subsystem services before TCF agent is started.
*/

void tcfServicesInit ()
    {
    hostFsLibInit ();                   /* Provides a virtual file I/O driver for use with the Debug Agent.  I/O is performed on this virtual I/O device exactly as it would be on any device referencing a VxWorks file system.  File operations, such as read() and write(), move data over a virtual I/O channel created between the Debug Agent and a Workbench TCF client */
    cafe_service_init(DEBUG_AGENT_TASK_PRIORITY); /* Provides Cafe services for TCF */
    init_system_viewer ();              /* Network Agent for driving System Viewer from host-side tools. */
    }

