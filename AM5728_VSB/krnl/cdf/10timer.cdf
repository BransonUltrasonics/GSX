/* 10dma.cdf - Component configuration file */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
23jun16,l_z  fix parent for FOLDER_CLOCK. (V7PRO-1419)
26apr16,emj  CERT: Update to produce partially linked objects
15arp16,l_z  fix typo for FOLDER_SUBSYSTEM. (V7PRO-3016)
06sep15,l_z  remove unsupported RTC component. (V7PRO-2300)
29apr15,l_z  update default min/max sys/aux rate. (V7PRO-2017)
23sep14,l_z  create folder for sys/aux/timestamp component. (V7PRO-102)
27mar14,cww  added system clocks and timestamp components from 00bsp.cdf
29jan14,wap  Fix INCLUDE_TIMER_SYS so that it pulls in INCLUDE_SYSCLK_INIT
30dec13,cww  use fixed init order for usrClkInit()
18oct13,x_z  fixed initialization order for INCLUDE_TIMER_STUB.
16jul13,l_z  created for VxBus GEN2
*/

Folder FOLDER_TIMER_SUB {
    NAME            Timer support subsystem
    SYNOPSIS        This folder contains components and parameters related to the timer support subsystem
    _CHILDREN       FOLDER_SUBSYSTEM
    CHILDREN        INCLUDE_VXB_SYS_CLK     \
                    INCLUDE_VXB_AUX_CLK     \
                    INCLUDE_VXB_TIMESTAMP   \
                    INCLUDE_VXB_TIMESTAMP64 \
                    INCLUDE_TIMER_SYS       \
                    INCLUDE_TIMER_SYS_SHOW 
}

Folder FOLDER_CLOCK {
    NAME            Clock support based on VxBus timer system
    SYNOPSIS        Clock support based on VxBus timer system
    _CHILDREN       FOLDER_SUBSYSTEM
    CHILDREN        INCLUDE_SYSCLK_INIT     \
                    INCLUDE_AUX_CLK         \
                    INCLUDE_TIMESTAMP       \
                    INCLUDE_TIMESTAMP64
}

InitGroup usrClkInit {
    INIT_RTN        usrClkInit ();
    SYNOPSIS        clock system initialization
    INIT_ORDER      INCLUDE_TIMER_SYS       \
                    INCLUDE_VXB_SYS_CLK     \
                    INCLUDE_VXB_AUX_CLK     \
                    INCLUDE_SYSCLK_INIT     \
                    INCLUDE_VXB_TIMESTAMP
}

Component INCLUDE_VXB_SYS_CLK {
    NAME            VxBus system clock support
    SYNOPSIS        Includes support for the VxBus system clock.
    PROTOTYPE       STATUS vxbSysClkLibInit (void);
    INIT_RTN        vxbSysClkLibInit();
    INCLUDE_WHEN    INCLUDE_SYSCLK_INIT INCLUDE_TIMER_SYS
    REQUIRES        INCLUDE_SYSCLK_INIT INCLUDE_TIMER_SYS
}

Component INCLUDE_VXB_AUX_CLK {
    NAME            VxBus auxiliary clock support
    SYNOPSIS        Includes support for the VxBus auxiliary clock.
    PROTOTYPE       STATUS vxbAuxClkLibInit (void);
    INIT_RTN        vxbAuxClkLibInit();
    INCLUDE_WHEN    INCLUDE_AUX_CLK INCLUDE_TIMER_SYS
    REQUIRES        INCLUDE_AUX_CLK INCLUDE_TIMER_SYS
}

Component INCLUDE_VXB_TIMESTAMP {
    NAME            VxBus timestamp support
    SYNOPSIS        Includes support for the VxBus timestamps.
    PROTOTYPE       STATUS vxbTimestampLibInit (void);
    INIT_RTN        vxbTimestampLibInit();
    INCLUDE_WHEN    INCLUDE_TIMESTAMP INCLUDE_TIMER_SYS
    REQUIRES        INCLUDE_TIMESTAMP INCLUDE_TIMER_SYS
}

Component INCLUDE_VXB_TIMESTAMP64 {
    NAME            VxBus 64-bit resolution Timestamp Support
    SYNOPSIS        VxBus 64-bit resolution Timestamp Support
    INCLUDE_WHEN    INCLUDE_TIMESTAMP64 INCLUDE_VXB_TIMESTAMP
    REQUIRES        INCLUDE_TIMESTAMP64 INCLUDE_VXB_TIMESTAMP
}

Component INCLUDE_TIMER_SYS {
    NAME            VxBus timer support
    SYNOPSIS        Includes support for the VxBus timer.
    CONFIGLETTES    usrVxbTimerSys.c
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
    MODULES         vxbTimerLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_SYSCLK_INIT
    INIT_RTN        usrVxbTimerSysInit();
    CFG_PARAMS      SYSCLK_TIMER_NAME       \
                    SYSCLK_TIMER_NUM        \
                    AUXCLK_TIMER_NAME       \
                    AUXCLK_TIMER_NUM        \
                    TIMESTAMP_TIMER_NAME    \
                    TIMESTAMP_TIMER_NUM
}

Component INCLUDE_TIMER_SYS_SHOW {
    NAME            VxBus timer show routines
    SYNOPSIS        Include the VxBus timer show routines such as vxbSysClkShow() which shows the \
					system clock information, vxbTimestampShow() which shows the timestamp clock \
					information, and vxbAuxClkShow( ) which shows the auxiliary clock information.
    REQUIRES        INCLUDE_TIMER_SYS
}

Component INCLUDE_SYSCLK_INIT {
	NAME		System clock
	SYNOPSIS	Initializes the system clock.
	CONFIGLETTES	sysClkInit.c
	HDR_FILES	tickLib.h
	INIT_RTN	sysClkInit ();
	CFG_PARAMS	SYS_CLK_RATE	\
			SYS_CLK_RATE_MIN	\
			SYS_CLK_RATE_MAX
}

Parameter SYS_CLK_RATE_MIN {
	NAME		Minimum system clock rate
	SYNOPSIS	Sets the minimum system clock rate. Its default value is 60.
	DEFAULT		(60)
}

Parameter SYS_CLK_RATE_MAX {
	NAME		SYS clock rate max
	SYNOPSIS	Sets the maximum system clock rate. Its default value is 300.
	DEFAULT		(300)
}

Parameter SYS_CLK_RATE {
	NAME		System clock rate
	SYNOPSIS	Sets the number of ticks per second. Its default value is 60.
	TYPE		uint
	DEFAULT		(60)
}

Parameter SYSCLK_TIMER_NAME {
        NAME		System clock device name
		SYNOPSIS	Defines the system clock device name. If left NULL, it will be automatically assigned.
        TYPE		string
        DEFAULT		NULL
}

Parameter SYSCLK_TIMER_NUM {
        NAME		System clock timer number
        TYPE		int
        DEFAULT		0
}

Component INCLUDE_AUX_CLK {
	NAME		Auxiliary clock
	SYNOPSIS	AUX clock component
	CFG_PARAMS	AUX_CLK_RATE_MIN	\
			AUX_CLK_RATE_MAX
}

Parameter AUX_CLK_RATE_MIN {
	NAME		Minimum auxiliary clock rate
	SYNOPSIS	Sets the minimum auxilirary clock rate. Its default value is 60.
	DEFAULT	    (60)
}

Parameter AUX_CLK_RATE_MAX {
	NAME		Maximum auxiliary clock rate
	SYNOPSIS	Sets the maximum auxilirary clock rate. Its default value is 1000.
	DEFAULT		(1000)
}

Parameter AUX_CLK_RATE {
	NAME		Default auxiliary clock rate
	SYNOPSIS	Sets the number of ticks per second. Its default value is 100.
	DEFAULT		(100)
}

Parameter AUXCLK_TIMER_NAME {
        NAME		Auxiliary clock device name 
		SYNOPSIS	Defines the auxiliary clock device name. If left NULL, it will be automatically assigned.
        TYPE		string
        DEFAULT		NULL
}

Parameter AUXCLK_TIMER_NUM {
        NAME		Auxiliary Clock Timer Number
        TYPE		int
        DEFAULT		0
}

Component INCLUDE_TIMESTAMP {
	NAME		high resolution timestamping
	SYNOPSIS	BSP high resolution timestamp driver
	HDR_FILES	timerDev.h
#ifdef	_WRS_CONFIG_SMP
	INIT_RTN	sysSmpTimeBaseInit(0, 0);
	INIT_AFTER	INCLUDE_MMU_OPTIMIZE
	INIT_BEFORE	INCLUDE_USER_APPL
#endif	/* _WRS_CONFIG_SMP */
}

Component INCLUDE_TIMESTAMP64 {
	NAME		64-bit resolution timestamping
	SYNOPSIS	64-bit resolution timestamp driver
	REQUIRES	INCLUDE_TIMESTAMP
}

Parameter TIMESTAMP_TIMER_NAME {
        NAME		Timestamp Device Name (NULL is auto-assign)
        TYPE		string
        DEFAULT		NULL
}

Parameter TIMESTAMP_TIMER_NUM {
        NAME		Timestamp Timer Number
        TYPE		int
        DEFAULT		0
}

