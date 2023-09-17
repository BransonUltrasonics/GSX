/* taskSysCall.h - VxWorks tasking system call definitions */

/*
 * Copyright (c) 2003-2011,2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
08mar14,gvm      enable VX_TASK_CTL_SET_TLS_BASE for non-COMPAT69 builds
01x,20jun11,jpb  Added VX_TASK_CTL_WAIT (Req: WIND00070880)
01w,12nov10,zl   changed _taskOpen() return type to TASK_ID
01v,05aug10,gvm  Added VX_TASK_CTL_SET_TLS_BASE. Available only on 64bit targets
01u,04mar10,zl   re-enabled VX_TASK_CTL_VAR_CMD for LP64
01t,21jan10,cww  Moved content to taskSysCallP.h
01s,14jan10,jpb  Updated taskCtl prototype.  Removed VX_TASK_CTL_VAR_CMD
                 conditions.
01r,03dec09,jpb  Updated for LP64 adaptation.
01q,27mar08,jpb  Added VX_TASK_CTL_TASKNAME_TO_ID_STRUCT.
01p,10oct07,gls  Added VX_TASK_CTL_UTCB_GET (Defect #00107604)
01o,29jun07,jmg  Added VX_TASK_CTL_DELETE
01n,03may07,jmg  Added VX_TASK_CTL_ATOMIC_OP, VX_TASK_CTL_CPU_AFFINITY_GET,
                 and VX_TASK_CTL_CPU_AFFINITY_SET.
01m,20sep06,jln  added VX_TASK_CTL_CHK_SCHEDULE
01l,03jul06,jmg  Added VX_TASK_CTL_PRI_NORMAL_GET to VX_TASK_CTL_PX_ATTR as
                 part of Defect # WIND00045158
01k,28feb06,jln  added VX_TASK_CTL_PX_ATTR_CREATE
01j,07feb06,kk   renamed taskOpenExt to taskOpenWithGuard
01i,08dec05,jln  added _taskOpenExt system call argument
01h,28jun05,jln  Add VX_TASK_CTL_PX_ATTR_XXX cases
01g,18apr04,hya  added VX_TASK_CTL_RESET.
01g,18feb04,aeg  changed _taskOpen() system call API.
01f,03dec03,kk   added VX_TASK_CTL_EXIT_REGISTER enum to taskCtl()
01e,27oct03,md   added VX_TASK_CTL_UTCB_SET enum value.
01d,27nov03,aeg  added _taskOpen() function declaration.
01c,22sep03,aeg  added VX_TASK_CTL_TASK_EXIT enum value.
01b,05sep03,dcc  moved taskOpen() and taskDelay() prototypes to taskLibCommon.h
01a,19aug03,aeg  written
*/

#ifndef __INCtaskSysCallh
#define __INCtaskSysCallh

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif

/* command codes for the taskCtl() system call */

typedef enum
    {
    VX_TASK_CTL_ACTIVATE,
    VX_TASK_CTL_DELETE,
    VX_TASK_CTL_SUSPEND,     
    VX_TASK_CTL_RESUME,         
    VX_TASK_CTL_RESTART,         
    VX_TASK_CTL_RESET,         
    VX_TASK_CTL_PRI_NORMAL_GET,         
    VX_TASK_CTL_PRIORITY_GET,     
    VX_TASK_CTL_PRIORITY_SET,    
    VX_TASK_CTL_VERIFY,        
    VX_TASK_CTL_VAR_ADD,      
    VX_TASK_CTL_VAR_DELETE,     
    VX_TASK_CTL_VAR_GET,        
    VX_TASK_CTL_VAR_SET,
    VX_TASK_CTL_TASK_EXIT,
    VX_TASK_CTL_UTCB_SET,
    VX_TASK_CTL_UTCB_GET,
    VX_TASK_CTL_EXIT_REGISTER,
    VX_TASK_CTL_PX_ATTR_SET,
    VX_TASK_CTL_PX_ATTR_GET,
    VX_TASK_CTL_PX_ATTR_CREATE,
    VX_TASK_CTL_CHK_SCHEDULE,
    VX_TASK_CTL_STACKSIZE_GET,
    VX_TASK_CTL_CPU_AFFINITY_GET,
    VX_TASK_CTL_CPU_AFFINITY_SET,
    VX_TASK_CTL_ATOMIC_OP,
    VX_TASK_CTL_TASKNAME_TO_ID,
    VX_TASK_CTL_WAIT
#if defined(_WRS_CONFIG_LP64) || !defined(_WRS_CONFIG_COMPAT69)
	,VX_TASK_CTL_SET_TLS_BASE
#endif	/* _WRS_CONFIG_LP64 */
    } VX_TASK_CTL_CMD;

/* command struct for VX_TASK_CTL_VAR_* commands */

/* 
 * Task vars are not supported for SMP but for user-side builds
 * we must have VX_TASK_CTL_VAR_CMD defined.
 */

typedef struct vx_task_ctl_var_cmd
    {
    long * pVariable;
    long   value;
    } VX_TASK_CTL_VAR_CMD;

/* argument structure for the _taskOpen() system call */

typedef struct vx_task_open_sc_args
    {
    const char * name;           /* task name - default name will be chosen */
    int          priority;       /* task priority */
    int          options;        /* VX_ task option bits */
    int		 mode;           /* object management mode bits */
    char       * pStackBase;     /* location of execution stack */
    size_t	 stackSize;      /* execution stack size */
    BOOL       * pTaskCreated;   /* new kernel task created? */
    void       * context;	 /* context value */
    FUNCPTR	 entryPt;	 /* application entry point */
    int		 argc;		 /* number of arguments to entry point */
    char      ** argv;	 	 /* arguments to application entry point */
    } VX_TASK_OPEN_SC_ARGS;

/* function declarations */

#ifndef _WRS_KERNEL
extern STATUS   taskCtl	  (TASK_ID tid, VX_TASK_CTL_CMD command, void * pArg, 
    			   size_t * pArgSize);
extern TASK_ID 	_taskOpen (VX_TASK_OPEN_SC_ARGS *pArgs);
#endif /* !_WRS_KERNEL */

#ifdef __cplusplus
}
#endif

#endif /* __INCtaskSysCallh */
