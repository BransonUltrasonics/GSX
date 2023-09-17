/* 01loader.cdf - target loader component */

/* 
* Copyright (c) 2004-2007, 2009-2011, 2013 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use of this software
* may be licensed only pursuant to the terms of an applicable Wind River license
* agreement. 
*/ 

/*
modification history
--------------------
01u,20sep13,h_k  moved INCLUDE_TLS_LOADER_SUPPORT from vxcore.
01t,03sep13,rlp  Removed symbol table components to its own file.
01s,28aug13,rlp  Removed INCLUDE_WDB_MDL_SYM_SYNC component
01r,16mar11,jmp  Performed some component naming/synopsis updates.
01q,15sep10,jmp  Removed init dependency between INCLUDE_SYM_TBL and
		 INCLUDE_ENV_VARS/INCLUDE_TASK_VARS components.
01p,30dec09,jpb  Remove INCLUDE_TASK_VARS from LP64 build.
01o,04oct07,mze  fix for 104829
01n,16aug07,pch  remove INCLUDE_TASK_VARS from SMP build
01m,16mar07,v_r  Added loader hooks init routine (req CQ:WIND00045277).
01l,25jun06,dlk  Make INCLUDE_NET_SYM_TBL not require INCLUDE_NET_INIT.
01k,17jan06,v_r  Added new loader hook facility component.
01j,11jan06,bpn  Added symbol table components.
01i,11aug05,pee  added dependency LOADER_CODE requires MODULE_MANAGER
01h,27jan05,dbt  Fixed INCLUDE_LOADER/INCLUDE_LOADER_CODE components.
01g,02oct04,bpn  Moved shell commands components to 01shell.cdf.
01f,29sep04,jn   Fix problem with symbol table config param 
01e,23sep04,jn   Add configuration parameter for loader
01d,18sep04,jn   Add initialization routine for unldLib
01c,31aug04,job  Fix problem with INCLUDE_LOADER_CODE in INIT_ORDER
01b,25aug04,job  Split INCLUDE_LOADER_CODE from INCLUDE_LOADER (SPR#101151)
01a,01jun04,bpn  Written from version 05v,12may04,dbt of 00vxWorks.cdf.


DESCRIPTION

This file contains the definition for the target loader components.
*/

Folder FOLDER_LOADER {
        NAME            loader components
	SYNOPSIS	VxWorks Kernel object module management
        CHILDREN        INCLUDE_UNLOADER \
                        INCLUDE_LOADER  \
			INCLUDE_LOADER_HOOKS \
                        INCLUDE_MODULE_MANAGER
        DEFAULTS        INCLUDE_LOADER
	_CHILDREN	FOLDER_TOOLS
}

Component INCLUDE_MODULE_MANAGER {
        NAME            module manager
        SYNOPSIS        Support library for module entities
        MODULES         moduleLib.o
        INIT_RTN        moduleLibInit ();
        HDR_FILES       moduleLib.h
}

/*
 * arch/<*>/arch.cdf contains the full definition for INCLUDE_LOADER_CODE.
 * It extends the skeletal definition provided here.
 */

Component INCLUDE_LOADER {
        NAME            target loader
	SYNOPSIS	Allows modules to be downloaded into running targets
        INIT_RTN        loadLibInit (STORE_ABS_SYMBOLS);
	CFG_PARAMS	STORE_ABS_SYMBOLS
        REQUIRES        INCLUDE_LOADER_CODE \
	                SELECT_SYM_TBL_INIT \
                        SELECT_COMPILER_INTRINSICS
}

Component INCLUDE_LOADER_CODE {
        NAME            target loader code
	SYNOPSIS	Architecture-independent loader internals
	MODULES         loadLib.o
        _CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_LOADER
	INCLUDE_WHEN	INCLUDE_LOADER
}

Component INCLUDE_LOADER_HOOKS {
	NAME		target loader hook facility
	SYNOPSIS	Pluggable hooks to extend the loader/unloader behavior
	MODULES		moduleHookLib.o
	HDR_FILES	private/moduleHookLibP.h
	INIT_RTN	moduleHookLibInit ();
        _CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_LOADER
}

Component INCLUDE_UNLOADER {
        NAME            target unloader
	SYNOPSIS	Permits removal of dynamically downloaded modules
        LINK_SYMS       unldByModuleId
        MODULES         unldLib.o
	HDR_FILES	unldLib.h
        INIT_RTN        unldLibInit ();
}

Component INCLUDE_TLS_LOADER_SUPPORT {
	NAME		__thread variables loader support
	SYNOPSIS	__thread variables loader support
	INIT_RTN	tlsLoadLibInit ();
	HDR_FILES	private/tlsLibP.h
	MODULES		tlsLoadLib.o
	_INIT_ORDER	usrToolsInit
	INIT_AFTER	usrLoaderInit
	INCLUDE_WHEN	INCLUDE_TLS \
			INCLUDE_LOADER
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter STORE_ABS_SYMBOLS {
	NAME		store absolute symbols
	SYNOPSIS	Whether to store absolute symbols in the system symbol table
	TYPE		BOOL
	DEFAULT		FALSE
}

InitGroup usrLoaderInit {
	INIT_RTN	usrLoaderInit ();
	SYNOPSIS	The target loader initialization sequence
	INIT_ORDER	INCLUDE_MODULE_MANAGER \
			INCLUDE_LOADER_CODE \
			INCLUDE_LOADER \
			INCLUDE_UNLOADER \
			INCLUDE_LOADER_HOOKS 
	_INIT_ORDER	usrToolsInit
	INIT_BEFORE	INCLUDE_TRIGGERING
}
