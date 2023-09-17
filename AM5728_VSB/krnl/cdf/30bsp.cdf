/* 30bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2013, 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21oct16,yya  refactored secure component (F6283)
12oct16,npc  added _CHILDREN for component INCLUDE_ACTLR_NEED_SMC
             and INCLUDE_L2ACTLR_NEED_SMC (V7PRO-3346)
10nov15,cfm  added INCLUDE_AUXREG_RTN, INCLUDE_ACTLR_NEED_SMC and
             INCLUDE_L2ACTLR_NEED_SMC (US69494)
26aug13,c_t  created

DESCRIPTION

This file contains updated descriptions for some vxWorks components
defined in 00bsp.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

#ifdef _WRS_CONFIG_EXTERNAL_CACHE
Component INCLUDE_EXT_L2_CACHE {
        NAME            Cortex A9 External L2 Cache Support
        SYNOPSIS        Cortex A9 External L2 Cache Support
        INIT_RTN        usrExtL2CacheInit();
        CONFIGLETTES    usrExtCache.c
        INCLUDE_WHEN    INCLUDE_CACHE_ARCH
        INIT_AFTER      INCLUDE_CPU_INIT
        INIT_BEFORE     INCLUDE_CACHE_ENABLE
        _INIT_ORDER     usrInit
        _CHILDREN       FOLDER_NOT_VISIBLE
}
#endif /* _WRS_CONFIG_EXTERNAL_CACHE */

#ifndef _WRS_CONFIG_COMPAT69_ARM_ARCH

/*******************************************************/
/* ARM Secure Configuration Selection                  */
/*******************************************************/

Component INCLUDE_SECURE_CFG {
    NAME            Secure Mode Configlette
    SYNOPSIS        Secure Mode Configlette
    CONFIGLETTES    usrSecureConfig.c
    _REQUIRES       INCLUDE_CPU_INIT_ARCH
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Selection SELECT_SECURE_CONFIG {
    NAME            ARM secure mode configuration
    SYNOPSIS        ARM secure mode configuration.
    COUNT           1-1
    _REQUIRES       INCLUDE_CPU_INIT_ARCH
    _CHILDREN       FOLDER_BSP_CONFIG
    CHILDREN        INCLUDE_SECURE_CONFIG_STANDARD
    DEFAULTS        INCLUDE_SECURE_CONFIG_STANDARD
}

Component INCLUDE_SECURE_CONFIG_STANDARD {
    NAME            Standard
    SYNOPSIS        VxWorks is running in Secure world \
                    which is the standard secure configuration.
}

Component INCLUDE_SECURE_CONFIG_OPTEE {
    NAME            OP-TEE
    SYNOPSIS        VxWorks is running in Non-Secure world \
                    and OP-TEE is running as secure monitor; \
                    communication is done in OP-TEE defined protocol.
    REQUIRES	    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_SECURE_CONFIG_TISM {
    NAME            TISM
    SYNOPSIS        VxWorks is running in Non-Secure world; \
                    TI proprietary secure monitor or its compatible is running in secure world; \
                    communication is done in TI proprietary secure monitor protocol.
    REQUIRES	    COMPONENT_NOT_SUPPORTED
}

/*********************************************************/
/* SMC protocol configurations                           */
/* This protocol consists simple SMC call at particular  */
/* positions: ACTLR, L2AUXCTRL and PL310. TISM can fit   */
/* into this protocol template.                          */
/*********************************************************/

Component INCLUDE_TISM_SMC_CFG {
    NAME            TISM SMC Protocol configuration
    SYNOPSIS        TISM SMC Protocol configuration
    CONFIGLETTES    usrSmcProtocol.c
    CFG_PARAMS      SMC_ACTLR_IMM           \
                    SMC_ACTLR_VAL           \
                    SMC_L2ACTLR_IMM         \
                    SMC_L2ACTLR_VAL         \
                    SMC_PL310_CTRL_IMM      \
                    SMC_PL310_CTRL_VAL      \
                    SMC_PL310_AUXCTRL_IMM   \
                    SMC_PL310_AUXCTRL_VAL   \
                    SMC_PL310_LATCTRL_IMM   \
                    SMC_PL310_LATCTRL_VAL
    _REQUIRES       INCLUDE_CPU_INIT_ARCH
    _CHILDREN       FOLDER_NOT_VISIBLE
}

/*******************************************************/
/* SMC for ACTLR                                       */
/*******************************************************/

Parameter SMC_ACTLR_IMM {
        NAME            SMC call immediate value for ACTLR
        SYNOPSIS        SMC call immediate value in the SMC call for updating ACTLR, \
                        if the default value is not align with the board document, \
                        it should be overriden in BSP
        DEFAULT         0
        TYPE            uint
}

Parameter SMC_ACTLR_VAL {
        NAME            SMC call number for ACTLR
        SYNOPSIS        SMC call number for updating ACTLR, \
                        if the default value is not align with the board document, \
                        it should be overriden in BSP
        DEFAULT         280
        TYPE            uint
}

/*******************************************************/
/* SMC for L2 AUX CTLR (for Cortex-A15)                */
/*******************************************************/

Parameter SMC_L2ACTLR_IMM {
        NAME            SMC call immediate value for L2ACTLR
        SYNOPSIS        SMC call immediate value in the SMC call for updating L2ACTLR, \
                        if the default value is not align with the board document, \
                        it should be overriden in BSP
        DEFAULT         0
        TYPE            uint
}

Parameter SMC_L2ACTLR_VAL {
        NAME            SMC call number for L2ACTLR
        SYNOPSIS        SMC call number for updating L2ACTLR, \
                        if the default value is not align with the board document, \
                        it should be overriden in BSP
        DEFAULT         1
        TYPE            uint
}

/*******************************************************/
/* SMC for PL310 registers                             */
/*******************************************************/

Parameter SMC_PL310_CTRL_IMM {
        NAME            SMC call immediate value for PL310 control register
        SYNOPSIS        SMC call immediate value in the SMC call for updating PL310 control register.
        DEFAULT         0
        TYPE            uint
}

Parameter SMC_PL310_CTRL_VAL {
        NAME            SMC call number for PL310 control register
        SYNOPSIS        SMC call number for updating PL310 control register.
        DEFAULT         258
        TYPE            uint
}

Parameter SMC_PL310_AUXCTRL_IMM {
        NAME            SMC call immediate value for PL310 auxiliary control register
        SYNOPSIS        SMC call immediate value in the SMC call for updating PL310 auxiliary control register.
        DEFAULT         0
        TYPE            uint
}

Parameter SMC_PL310_AUXCTRL_VAL {
        NAME            SMC call number for PL310 auxiliary control register
        SYNOPSIS        SMC call number for updating PL310 auxiliary control register.
        DEFAULT         265
        TYPE            uint
}

Parameter SMC_PL310_LATCTRL_IMM {
        NAME            SMC call immediate value for PL310 latency control register
        SYNOPSIS        SMC call immediate value in the SMC call for updating PL310 latency control register.
        DEFAULT         0
        TYPE            uint
}

Parameter SMC_PL310_LATCTRL_VAL {
        NAME            SMC call number for PL310 latency control register
        SYNOPSIS        SMC call number for updating PL310 latency control register.
        DEFAULT         274
        TYPE            uint
}

#endif /* !_WRS_CONFIG_COMPAT69_ARM_ARCH */
