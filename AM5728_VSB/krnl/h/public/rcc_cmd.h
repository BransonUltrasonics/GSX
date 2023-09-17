/* rcc_cmd.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history.
*/


#ifndef __RCC_CMD_H__
#define __RCC_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS RCC_CMD_Clear(WMB_SESSION_T * pCliEnv);
extern STATUS RCC_CMD_VxWorksShell(WMB_SESSION_T * pCliEnv);
extern STATUS RCC_CMD_Write(WMB_SESSION_T *pCliEnv, sbyte * recipient, sbyte * message);
extern STATUS RCC_CMD_Who(WMB_SESSION_T * pCliEnv);
extern STATUS RCC_CMD_Snmp_Auto(WMB_SESSION_T * pCliEnv);
extern STATUS RCC_CMD_Snmp_Commit(WMB_SESSION_T * pCliEnv);
extern STATUS RCC_CMD_SyslogWrite(WMB_SESSION_T * pCliEnv, sbyte * destination, sbyte * pBuf);

#ifdef __cplusplus
}
#endif

#define RCC_CMD_Exec        RCC_DB_Exec
#define RCC_CMD_ExecFile    RCC_DB_ExecFile
#define RCC_CMD_ExecRM      RCC_DB_ExecRM

#endif /* __RCC_CMD_H__ */
