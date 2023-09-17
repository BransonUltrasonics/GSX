/* 00comp_net_appl_util.cdf - Component configuration file */

/*
 * Copyright 2004, 2010, 2012-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27feb,zan  Add INCLUDE_APPL_LOG_UTIL to FOLDER_KERNEL
07feb12,dlk  Fix defect WIND00331675.
14sep10,h_x  CQ:WIND00141200
18sep04,dlk  Remove explicit depencency upon INCLUDE_NBIO_LOG.
12sep04,dlk  written
*/

Component INCLUDE_APPL_LOG_UTIL {
	NAME		Stack/Application Logging Utility
	SYNOPSIS	Stack and Application Logging Utility
	HDR_FILES	applUtilLib.h \
			private/fioLibP.h
	CONFIGLETTES	usrNetApplUtil.c
	INIT_RTN	usrNetApplUtilInit ();
	CFG_PARAMS	APPL_LOG_FD_CFG \
			APPL_LOG_LEVEL_CFG \
			APPL_KERN_LOG_FD_CFG \
			APPL_KERN_LOG_LEVEL_CFG
	MODULES		applUtilLib.o
	_CHILDREN	FOLDER_KERNEL	
}

Parameter APPL_LOG_FD_CFG {
        NAME		Default application logging FD
        SYNOPSIS	Default application logging file descriptor for non-kernel facilities
        DEFAULT		STD_OUT
        TYPE		int
}

Parameter APPL_LOG_LEVEL_CFG {
        NAME		Default application logging mask and flags
        SYNOPSIS	Default application logging mask and flags for non-kernel facilities
        DEFAULT		(_LOG_DEC_DFLT | _LOG_LVL_DFLT)
        TYPE		int
}

Parameter APPL_KERN_LOG_FD_CFG {
        NAME		Default kernel logging FD
        SYNOPSIS	Default logging file descriptor for KERN_LOG facility
        DEFAULT		KERN_LOG_FD
        TYPE		int
}

Parameter APPL_KERN_LOG_LEVEL_CFG {
        NAME		Default kernel logging mask and flags
        SYNOPSIS	Default logging mask and flags for KERN_LOG facility
        DEFAULT		(_LOG_DEC_DFLT | _LOG_LVL_DFLT)
        TYPE		int
}
