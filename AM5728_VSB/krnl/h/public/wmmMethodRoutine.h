/* wmmMethodRoutine.h - Method Routine for WMB */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,05may04,gsr  added routine to convert SNMP errors to
                 WMB errors SPR 90299
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmmMethodRoutineh__
#define __INCwmmMethodRoutineh__

#ifdef __cplusplus
extern "C" {
#endif
STATUS wmmRegisterSNMPObject
    (
    sbyte       *objName,           /* [in] Object name              */
    ubyte4      *Id,                /* [in] OID                      */
    ubyte2      oidLen,             /* [in] OID Len                  */
    ubyte2      dataType,           /* [in] Data Type                */
    ubyte2      accessRead,         /* [in] Read access value        */
    ubyte2      accessWrite,        /* [in] Write access value       */
    void        *pData              /* [in] Pointer to the data to be stored    */
    );

sbyte4 wmmErrorSNMPToWMB
	(
	ubyte2      snmpError    /* [in] SNMP Error number to translate   */
    );
#ifdef __cplusplus
}
#endif

#endif /* __INCwmmMethodRoutineh__ */
