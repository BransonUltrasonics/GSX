/* 01rtp.cdf - Real Time Process component definitions */

/*
 * Copyright (c) 2003-2010, 2012-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22apr16,emj  CERT: Update to produce partially linked objects (US79607)
18dec14,ymz  split INCLUDE_SC_IOS for CERT subset and non-cert subset
08dec14,v_r  CERT: RTP symbol table support now disabled
09nov14,gls  removed INCLUDE_SYSCTL for the certification profile
02dec14,v_r  RTP symbol table support now optional
08oct14,gls  added INCLUDE_RTP_SIGNALS
12may14,cww  added CERT restriction
05nov15,rr   add vxAtomicScLib.o to INCLUDE_SC_KERNEL 
30sep15,rr   Added RTP_MEM_FILL parameter (V7COR-3084)
29jan15,jmp  Increased default RTP_HOOK_TBL_SIZE (V7COR-2525).
09jan14,h_k  moved INCLUDE_RTP_APPL_INIT_CMD_SHELL_SCRIPT to
             02shell_commands.cdf.
10jan12,pcs  decouple INCLUDE_SHARED_DATA from INCLUDE_SHL since shared
             libraries no longer use shared data. (iter23 merge)
02aug13,pcs  updated to use config option _WRS_CONFIG_COMPAT69_ADR_SPACE
09mar10,pad  Do not show RTP_OVERLAPPED_ADDRESS_SPACE, RTP_CODE_REGION_SIZE
	     and RTP_CODE_REGION_START parameters for VxWorks 64-bit.
27jan10,cww  Added memRtpLibInit to header file
25jan10,kk   change to use overlap as default
08jan10,zl   split INCLUDE_MMAP from INCLUDE_SC_MMAN. Moved KERNEL_HEAP_SIZE
             parameter to 02mm.cdf
08dec09,cww  Added INCLUDE_TASK_STACK_ALLOC
22sep09,kk   hide INCLUDE_RTP_POSIX_PSE52 (WIND00182189)
04sep09,kk   remove unused RTP_KERNEL_STACK_SIZE parameter (WIND00180527)
13aug09,cww  Updated ordering of RTP init routines
23jun08,pad  Added the RTP_OVERLAPPED_ADDRESS_SPACE parameter to switch
	     between flat and overlapped virtual memory models.
07may08,pad  Added the RTP_CODE_REGION_SIZE and RTP_CODE_REGION_START
	     parameters.
17oct07,mfj  Fixing problem with KERNEL_HEAP_SIZE (WIND00102855)
01aug07,act  io footprint work (from pcm)
06jul06,pcs  RTP heap options is now configurable.
17mar06,pad  Fixed pxCpuTimerLib.o module name (SPR #119108).
14feb06,jln  added INCLUDE_POSIX_THREAD_CPUTIME component
18jan06,pad  Added component INCLUDE_RTP_POSIX_PSE52 and parameter
	     RTP_FD_NUM_MAX. Changed call to rtpLibInit().
18aug05,kk   updated synopsis for RTP_SIGNAL_QUEUE_SIZE
03aug05,kk   make the RTP_SIGNAL_QUEUE_SIZE default to be 32 to conform to
             POSIX
09jul05,yvp  Hook components now depend on INCLUDE_HOOKS.
22jun05,yvp  Added dependency on INCLUDE_TASK_RESTART and 
	     INCLUDE_TASK_CREATE_DELETE for system calls.
12may05,kk   added dependency of VXEVENTS for RTP (SPR# 109130)
20apr05,gls  added INCLUDE_OBJ_OWNERSHIP (SPR #106150)
15apr05,yvp  Fixed INCLUDE_RTP_APPL_INIT_STRING default value (SPR 107365).
	     Changed INCLUDE_RTP_APPL_USER configlette (SPR 105759).
26feb05,pcs  Modified to reflect the change to the prototype of fn.
             rtpLibInit
27jan05,dbt  Removed dependency on loader (SPR #101436).
24nov04,kk   ensure RTP_HOOKS gets included when SHL is included
24nov04,fle  Fixed simple quotes in components synopses
23nov04,zl   added type definition to KERNEL_HEAP_SIZE.
22nov04,yvp  Added synopsis for RTP startup facility components.
15nov04,yvp  Changed prototype for usrRtpAppInitString().
11nov04,job  System call components added to usrScInit init group.
	     Fix INCLUDE_MEM_RTP; added INCLUDE_SC_SD, INCLUDE_SC_SHL
18oct04,fr   added INCLUDE_OBJ_OPEN to INCLUDE_RTP (SPR 101491)
14oct04,pad  Moved rtpHookLibInit in the usrRoot group, to be initialized
             before rtpLib (SPR #102595).
02oct04,gls  added private/sdLibP.h to INCLUDE_SHARED_DATA component
02oct04,bpn  Moved shell commands components to 01shell.cdf.
25sep04,dcc  added objRtpLib.o to the list of rtp modules
23sep04,kk   added INCLUDE_RTP_SHOW_SHELL_CMD
02sep04,yvp  rtpHookLibInit() prototype change.
31aug04,job  Made INCLDUE_RTP require INCLUDE_LOADER_CODE (SPR#101151)
28aug04,dbt  Fixed previous modification.
27aug04,gls  added SD_HOOK_TBL_SIZE
28jul04,md   Removed need for INCLUDE_EDR_SYSDBG_FLAG
28jun04,job  Changed INCLUDE_SYSCTL_API to INCLUDE_SYSCTL
21jun04,kk   added INIT_ORDER for RTP_HOOKS & SYSCALL_HOOKS, added
             INCLUDE_SHL_SHELL_CMD
20may04,job  INCLUDE_RTP now requires INCLUDE_SYSCTL_API
20may04,gls  renamed mmScLib to mmanScLib
18may04,yvp  Added INCLUDE_RTP_APPL_INIT_CMD_SHELL_SCRIPT.
18may04,bpn  Added invisible INCLUDE_RTP_SHELL_C component.
03may04,kk   added INCLUDE_SHL_SHOW component
09apr04,kk   added INCLUDE_SHL components
07may04,yvp  Added RTP Startup Facility Components INCLUDE_RTP_APPL_*
06apr04,gls  added INCLUDE_SHARED_DATA_SHOW
06apr04,job  Pass TASK_STACK_OVERFLOW_SIZE and TASK_STACK_UNDERFLOW_SIZE
	     to rtpLibInit(). Removed the INIT_AFTER line from the
	     definition of INCLUDE_RTP_APPL
05apr04,yvp  Moved RTP_APPL component inclusion in tree to this file. Made
             RTP component a child of the OS folder.
02apr04,tam  fixed init of SHARTED_DATA (SPR 95720)
03mar04,md   include ED&R policies when RTP's are included
24feb04,yvp  Added component INCLUDE_RTP_APPL.
18feb04,ans  Added RTP_SIGNAL_QUEUE_SIZE CFG_PARAMS in INCLUDE_RTP component.
12dec03,yvp  Added syscallTbl.h to INCLUDE_RTP inlcude list.
08dec03,m_s  removed definition of SC_POSIX
05dec03,zl   added INCLUDE_MEM_RTP component, RTP_HEAP_INIT_SIZE parameter
04dec03,ans  removed REQUIRES SC_POSIX from INCLUDE_RTP component
03dec03,bpn  Fixed init of the component INCLUDE_RTP_SHELL_CMD.
21oct03,dat  SC_IOS subcomponent, general cleanup/refactor of RTP components.
11nov03,kk   added INCLUDE_WHEN on INCLUDE_SHOW_ROUTINES for all show
             components
04nov03,kk   added SHARED DATA component
23oct03,tam  added KERNEL_HEAP_SIZE parameter to RTP component.
12nov03,yvp  Added support for RTP hooks.
31oct03,yvp  Added INCLUDE_RTP_ARCH.
03oct03,ans  INCLUDE_RTP now REQUIRES INCLUDE_SC_POSIX
29sep03,pad  Now force the inclusion of system call handler components.
22sep03,bpn  Added INCLUDE_RTP_SHELL_CMD component.
04sep03,nrj  added auxiliary vector module
19aug03,kk   removed rtpElfLoadLib.o, rtpALib.o, excScPpcALib.o 
             from INCLUDE_RTP. The ALib.o files should be arch specific.
             made kernel stack size a page.
	     added INCLUDE_RTP_SHOW component
15jul03,job  Fixed spelling mistakes
14jul03,nrj  added modules that are arch specific, system call
27jun03,nrj  written to include RTP
*/

/*
This file contains descriptions for all of the RTP components.
*/

/* Attach main RTP folder to the root folder, don't make it a default */

Folder	FOLDER_RTP {
	NAME		Real Time Process 
	SYNOPSIS	This folder contains different components and parameters \
			that can be added to configure real time processes.
	_CHILDREN	FOLDER_OS
	CHILDREN	INCLUDE_RTP \
                        INCLUDE_RTP_SIGNALS \
			INCLUDE_SHARED_DATA \
			INCLUDE_SHL \
			INCLUDE_RTP_HOOKS \
			INCLUDE_SYSCALL_HOOKS
	DEFAULTS	INCLUDE_RTP
}

Component INCLUDE_RTP {
	NAME		RTP
	SYNOPSIS	Real Time Process.
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		rtpLib.o rtpAuxvLib.o syscallLib.o objRtpLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        INIT_RTN        rtpLibInit (RTP_HOOK_TBL_SIZE, SYSCALL_HOOK_TBL_SIZE, \
				    RTP_SIGNAL_QUEUE_SIZE, \
				    TASK_USER_EXC_STACK_OVERFLOW_SIZE, \
				    TASK_USER_EXEC_STACK_OVERFLOW_SIZE, \
				    TASK_USER_EXEC_STACK_UNDERFLOW_SIZE, \
				    RTP_FD_NUM_MAX, RTP_MEM_FILL);
	HDR_FILES	rtpLib.h private/rtpLibP.h auxv.h syscallLib.h \
			syscallTbl.h private/syscallLibP.h
#ifndef _WRS_CONFIG_CERT
	REQUIRES	INCLUDE_MEM_RTP \
			INCLUDE_TASK_STACK_ALLOC \
			INCLUDE_SC_KERNEL \
			INCLUDE_SC_IOS \
			INCLUDE_SC_MMAN \
                        INCLUDE_SYSCTL \
			INCLUDE_RTP_ARCH \
			INCLUDE_OBJ_OPEN \
			INCLUDE_OBJ_OWNERSHIP \
			INCLUDE_VXEVENTS
#else
	REQUIRES	INCLUDE_MEM_RTP \
			INCLUDE_TASK_STACK_ALLOC \
			INCLUDE_SC_KERNEL \
			INCLUDE_SC_IOS \
			INCLUDE_SC_MMAN \
			INCLUDE_RTP_ARCH \
			INCLUDE_OBJ_OPEN \
			INCLUDE_OBJ_OWNERSHIP \
			INCLUDE_VXEVENTS
#endif /* _WRS_CONFIG_CERT */

#ifdef _WRS_CONFIG_COMPAT69_ADR_SPACE
	CFG_PARAMS	RTP_HOOK_TBL_SIZE \
			SYSCALL_HOOK_TBL_SIZE \
			RTP_SIGNAL_QUEUE_SIZE \
			RTP_HEAP_INIT_SIZE \
			RTP_HEAP_DEFAULT_OPTIONS \
			RTP_FD_NUM_MAX \
			RTP_CODE_REGION_START \
			RTP_CODE_REGION_SIZE \
			RTP_OVERLAPPED_ADDRESS_SPACE \
			RTP_MEM_FILL
#else /* _WRS_CONFIG_COMPAT69_ADR_SPACE */
	CFG_PARAMS	RTP_HOOK_TBL_SIZE \
			SYSCALL_HOOK_TBL_SIZE \
			RTP_SIGNAL_QUEUE_SIZE \
			RTP_HEAP_INIT_SIZE \
			RTP_HEAP_DEFAULT_OPTIONS \
			RTP_FD_NUM_MAX \
			RTP_MEM_FILL
#endif /* _WRS_CONFIG_COMPAT69_ADR_SPACE */
}

Component INCLUDE_RTP_ARCH {
        NAME            Arch components for RTP
        _CHILDREN       FOLDER_NOT_VISIBLE
        REQUIRES        INCLUDE_RTP
}

Component INCLUDE_RTP_SIGNALS {
	NAME		RTP signals
	SYNOPSIS	POSIX signal support for RTPs
	MODULES		rtpSigLib.o rtpSigScLib.o
	INIT_RTN	rtpSigLibInit ();
	REQUIRES	INCLUDE_RTP INCLUDE_SIGNALS
	INCLUDE_WHEN	INCLUDE_RTP \
			INCLUDE_SIGNALS
	HDR_FILES	rtpLib.h private/rtpSigLibP.h private/sigLibP.h
	INIT_AFTER      INCLUDE_RTP	
}

#ifndef _WRS_CONFIG_CERT
Component INCLUDE_RTP_SYMBOL_TABLE {
	NAME		RTP symbol table support
	SYNOPSIS	Symbol table support for RTPs. Allows to register \
			symbols when or after loading RTPs and shared \
			libraries as a debugging aid.
	MODULES		loadRtpSymLib.o
	INIT_RTN	loadRtpSymLibInit ();
	_INIT_ORDER	usrSymTblInit
	INIT_AFTER      INCLUDE_RTP
	INCLUDE_WHEN	INCLUDE_SYM_TBL INCLUDE_RTP
        _CHILDREN       FOLDER_NOT_VISIBLE
}
#endif /* _WRS_CONFIG_CERT */

Component INCLUDE_RTP_SHOW {
	NAME		RTP show
	SYNOPSIS	This component provides support for the rtpMemShow() \
			routine. It displays the RTP's private mappings.
	MODULES		rtpShow.o
	INIT_RTN	rtpShowInit ();
	HDR_FILES	rtpLib.h
	INCLUDE_WHEN	INCLUDE_RTP \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit

}

Component INCLUDE_SHARED_DATA {
	NAME		Shared data region support in RTPs or kernel
	SYNOPSIS	provide data sharing between RTPs or kernel
	MODULES		sdLib.o
	INIT_RTN	sdLibInit (SD_HOOK_TBL_SIZE);
	CFG_PARAMS	SD_HOOK_TBL_SIZE
	REQUIRES	INCLUDE_RTP INCLUDE_SC_SD
	HDR_FILES	sdLib.h private/sdLibP.h
}

Component INCLUDE_SHARED_DATA_SHOW {
	NAME		Shared data show
	SYNOPSIS	Display information on Shared Data.
	MODULES		sdShow.o
	INIT_RTN	sdShowInit ();
	REQUIRES	INCLUDE_SHARED_DATA
	HDR_FILES	sdLib.h
	INCLUDE_WHEN	INCLUDE_SHARED_DATA \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
	INIT_AFTER      INCLUDE_RTP_SHOW	
}

Component INCLUDE_SHL {
	NAME		shared library support in RTPs
	SYNOPSIS	provide sharing of code between RTPs
	MODULES		shlLib.o
	INIT_RTN	shlLibInit ();
	REQUIRES	INCLUDE_RTP \
			INCLUDE_SC_SHL \
			INCLUDE_RTP_HOOKS
	HDR_FILES	shlLib.h
}

Component INCLUDE_SHL_SHOW {
	NAME		Shared Library Show
	SYNOPSIS	Display information on Shared Libraries
	MODULES		shlShow.o
	INIT_RTN	shlShowInit ();
	REQUIRES	INCLUDE_SHL
	HDR_FILES	shlLib.h
	INCLUDE_WHEN	INCLUDE_SHL \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
	INIT_AFTER      INCLUDE_RTP_SHOW	
}

Component INCLUDE_RTP_SHELL_C {
	NAME		RTP Shell C Routines
	SYNOPSIS	List of commands for the shell C interpreter related to RTPs.
	MODULES		usrRtpLib.o
	INIT_RTN	usrRtpLibInit ();
	PROTOTYPE	extern STATUS usrRtpLibInit();
	_INIT_ORDER	usrToolsInit
	_CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_RTP
	INCLUDE_WHEN	INCLUDE_RTP INCLUDE_SHELL_INTERP_C
}


Component INCLUDE_RTP_HOOKS {
	NAME		RTP Hook Support
	SYNOPSIS	Add hook routines to be called on RTP operations.
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		rtpHookLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	INIT_RTN	rtpHookLibInit (RTP_HOOK_TBL_SIZE);
	REQUIRES	INCLUDE_RTP \
                        INCLUDE_HOOKS
}

Component INCLUDE_SYSCALL_HOOKS {
	NAME		System Call Hook Support
	SYNOPSIS	Add hook routines to be called on system calls.
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		syscallHookLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	INIT_RTN	syscallHookLibInit ();
	REQUIRES	INCLUDE_RTP \
                        INCLUDE_HOOKS
	_INIT_ORDER	usrKernelExtraInit
}

/* 
 * INCLUDE_RTP_POSIX_PSE52 is a utility component. This component
 * does not drag in code, it simply sets a set of parameters appropriate
 * for PSE52 support. This component alone does not provide PSE52 support,
 * it requires inclusion of the set of CDF components associated with PSE52,
 * as done in the BUNDLE_RTP_POSIX_PSE52 bundle.
 * This component is hidden to avoid confusion. Users are encouraged to 
 * use the BUNDLE_RTP_POSIX_PSE52 to pick up PSE52 support.
 */

Component INCLUDE_RTP_POSIX_PSE52 {
	NAME		POSIX PSE52 support for RTPs
	SYNOPSIS	Utility component used in configlette code to set parameters appropriately for POSIX PSE52.
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_POSIX_THREAD_CPUTIME {
        NAME            POSIX thread CPU-time clock
        SYNOPSIS        Support RTP-only thread CPU-time clock/timer
        MODULES         pxCpuTimerLib.o
        INIT_RTN        pxCpuTimeLibInit ();
        HDR_FILES       private/pxCpuTimeLibP.h
	_CHILDREN	FOLDER_RTP
	_INIT_ORDER	usrKernelExtraInit
	INIT_AFTER      INCLUDE_POSIX_TIMERS	
        REQUIRES        INCLUDE_POSIX_TIMERS INCLUDE_POSIX_CLOCKS \
                        INCLUDE_POSIX_PTHREAD_SCHEDULER
}

Parameter RTP_FD_NUM_MAX
	{
	NAME		Number of entries in an RTP fd table
	SYNOPSIS	Maximum number of file descriptors for an RTP. See also the NUM_FILES parameter.
	TYPE		uint
	DEFAULT		20
	}

Parameter RTP_HOOK_TBL_SIZE
	{
	NAME		number of entries in the RTP create/delete hook tables
	SYNOPSIS	number of entries in the RTP create/delete hook tables
        TYPE 		uint
        DEFAULT 	16
	}

Parameter SD_HOOK_TBL_SIZE
	{
	NAME		number of entries in the SD hook tables
	SYNOPSIS	number of entries in the SD hook tables
        TYPE 		uint
        DEFAULT 	8
	}

Parameter SYSCALL_HOOK_TBL_SIZE
	{
	NAME		number of entries in the system call hook tables
	SYNOPSIS	number of entries in the system call hook tables
        TYPE 		uint
        DEFAULT 	8
	}

Parameter RTP_HEAP_DEFAULT_OPTIONS {
        NAME            default memory partition options for RTP heap
        SYNOPSIS        default memory partition options for RTP heap
        TYPE            UINT
        DEFAULT         (MEM_ALLOC_ERROR_LOG_FLAG      | \
                         MEM_ALLOC_ERROR_EDR_WARN_FLAG | \
                         MEM_BLOCK_ERROR_LOG_FLAG      | \
                         MEM_BLOCK_ERROR_EDR_FATAL_FLAG | \
                         MEM_BLOCK_CHECK)
}

Parameter RTP_HEAP_INIT_SIZE
        {
        NAME            initial size of RTP heap
        SYNOPSIS        initial size of the heap of each RTP created; the \
                        HEAP_INITIAL_SIZE environment variable can override it.
        TYPE            uint
        DEFAULT         0x10000
        }

Parameter RTP_SIGNAL_QUEUE_SIZE
	{
	NAME		maximum number of queued signal for a RTP
	SYNOPSIS        maximum number of queued signal for a RTP. For POSIX, \
	                this value must be at least 32.
        TYPE 		uint
        DEFAULT 	32
	}

#ifdef _WRS_CONFIG_COMPAT69_ADR_SPACE
/*
 * For VxWorks 64-bit RTPs are always executed in an overlapped
 * virtual memory space and nothing about this is configurable.
 */

Parameter RTP_CODE_REGION_START
	{
	NAME		Memory area reserved for absolutely linked RTP
	SYNOPSIS	Base address of the virtual memory region reserved for \
			the text and data/bss segments (i.e. all loadable \
			sections) of absolutely linked RTP executables. If \
			the address is null no such region is reserved.
	TYPE		uint
	DEFAULT		0
	}

Parameter RTP_CODE_REGION_SIZE
	{
	NAME		Size of the area reserved for absolutely linked RTP
	SYNOPSIS	This is the size of the virtual memory region reserved \
			for the text and data/bss segments (i.e. all loadable \
			sections) of absolutely linked RTP executables. If \
			the size is null no such region is reserved.
	TYPE		uint
	DEFAULT		0
	}

Parameter RTP_OVERLAPPED_ADDRESS_SPACE
	{
	NAME		Use overlapped virtual address space for RTPs
	SYNOPSIS	Controls whether each RTP has its own private view of \
			the virtual address space (all views overlapping) or \
			whether they all share a flat virtual address space \
			(legacy). By default, RTPs run in overlapped mode.
	TYPE		bool
	DEFAULT		TRUE
	}
#endif /* _WRS_CONFIG_COMPAT69_ADR_SPACE */

Component INCLUDE_MEM_RTP {
	NAME		RTP memory configuration setup
	SYNOPSIS	Initializes memory parameters to pass to RTP
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		memRtpLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	HDR_FILES	private/rtpLibP.h
	INIT_RTN	memRtpLibInit (RTP_HEAP_INIT_SIZE,RTP_HEAP_DEFAULT_OPTIONS);
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER	usrScInit
}

Component INCLUDE_RTP_APPL_USER {
	NAME		RTP Startup Facility: User-defined code
	SYNOPSIS    	Launch RTP from a user-defined function.
	_CHILDREN       FOLDER_APPLICATION
	PROTOTYPE	void usrRtpAppInit (void);
	INIT_RTN	usrRtpAppInit ();
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Component INCLUDE_RTP_APPL_INIT_STRING {
	NAME		RTP Startup Facility: String-encoded RTP list
	SYNOPSIS    	Launch RTP from a string-encoded list.
	_CHILDREN       FOLDER_APPLICATION
	CONFIGLETTES	usrRtpAppInitString.c
	INIT_RTN	usrRtpAppInitString ();
	CFG_PARAMS	RTP_APPL_INIT_STRING
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Parameter RTP_APPL_INIT_STRING {
	NAME		String-encoded RTP list.
	SYNOPSIS	String-encoded list of RTPs to launch after bootup.
    	TYPE            string
	DEFAULT		""
}

Parameter RTP_MEM_FILL {
	NAME		Memory allocated to RTP cleared before allocation. 
	SYNOPSIS    RTP stack filled with 0xee, other RTP memory allocations \
	            filled with 0.Overrides VX_NO_STACK_FILL for RTP task execution \
	            stack. VX_GLOBAL_NO_STACK_FILL overrides RTP_MEM_FILL \
	            for RTP task execution stack. 
	TYPE		BOOL
	DEFAULT		TRUE
}

Component INCLUDE_RTP_APPL_INIT_BOOTLINE {
	NAME		RTP Startup Facility: Bootline-encoded RTP list
	SYNOPSIS    	Launch RTP from a string-encoded list in the boot parameters.
	_CHILDREN       FOLDER_APPLICATION
	CONFIGLETTES	usrRtpAppInitBootline.c
	INIT_RTN	usrRtpAppInitBootline ();
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

/*
 * system call sub-components are always included, be sure to
 * add them to the REQUIRES list for INCLUDE_RTP.  Since they are
 * not individually selectable, attach them to the NOT_VISIBLE folder.
 * They are initialized last, just before the user application.
 */

Component INCLUDE_SC_KERNEL {
	NAME		WIND system call handlers
	SYNOPSIS	system call handlers for WIND kernel 
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		windScLib.o vxAtomicScLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	PROTOTYPE	void windScInit (void);
	INIT_RTN	windScInit ();
#ifdef _WRS_CONFIG_CERT
	REQUIRES	INCLUDE_TASK_CREATE_DELETE
#else
	REQUIRES	INCLUDE_TASK_CREATE_DELETE	\
			INCLUDE_TASK_RESTART
#endif
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_MMAN {
	NAME		Memory management system call handlers
	SYNOPSIS	system call handlers for memory management
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		mmanScLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	PROTOTYPE	void mmanScLibInit (void);
	INIT_RTN	mmanScLibInit ();
	REQUIRES	INCLUDE_MMAP
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_SD {
	NAME		Shared Data system call handlers
	SYNOPSIS	system call handlers for shared data
	MODULES		sdScLib.o sdLib.o
	PROTOTYPE	void sdScInit (void);
	INIT_RTN	sdScInit ();
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_SHL {
	NAME		Shared Library system call handlers
	SYNOPSIS	system call handlers for shared libraries
	MODULES		shlScLib.o shlLib.o
	PROTOTYPE	void shlScLibInit (void);
	INIT_RTN	shlScLibInit ();
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_RTP_LIMITS {
	NAME		RTP limits system call handler
	SYNOPSIS	system call handler for RTP prlimits
	MODULES		rtpIoLib.o
#ifndef _WRS_CONFIG_CERT
	INCLUDE_WHEN	INCLUDE_SC_IOS
#endif
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_PIPE {
	NAME		pipe system call handler
	SYNOPSIS	system call handler for pipes
	MODULES		iosPipeScLib.o
#ifndef _WRS_CONFIG_CERT
	INCLUDE_WHEN	INCLUDE_SC_IOS
#endif
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_IOS {
	NAME		IOS system calls
	SYNOPSIS	IOS system calls
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
	MODULES		iosScLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
	INIT_RTN	iosScLibInit ();
	HDR_FILES	iosScLib.h
	REQUIRES	INCLUDE_RTP \
			INCLUDE_IO_RTP
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

/* Attach RTP_SHOW to SHOW ROUTINES folder, init with other show routines */

