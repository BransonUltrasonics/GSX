/* wmbSNMPLib.h - SNMP Lib header for WMB */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbSNMPLibh__
#define __INCwmbSNMPLibh__

#ifdef __cplusplus
extern "C" {
#endif

#define WMB_MAX_SNMP_ENTRIES    16

typedef struct WMB_SNMP_ENTRY_S
    {
    WMB_CID_T       component;      /* Component that registered this table */
    sbyte2          numEntry;       /* Number of Entries                    */
    sbyte2          maxEntry;       /* Max number of Entries                */
    WMB_ENTRY_T     *pEntryArray;   /* Pointer to the entry array           */
    }WMB_SNMP_ENTRY_T;

STATUS wmbSNMPResourceRegister
    (
    WMB_CID_T   component,          /* [in] Component ID            */
    sbyte       *objName,           /* [in] Object name             */
    ubyte4      *Id,                /* [in] OID                     */
    ubyte2      oidLen,             /* [in] OID Len                 */
    ubyte2      dataType,           /* [in] Data Type               */
    ubyte2      accessRead,         /* [in] Read access value       */
    ubyte2      accessWrite,        /* [in] Write access value      */
    ubyte2      flags,              /* [in] Flags                   */
    WMB_HANDLER_T wmbHandler        /* [in] Pointer to the WMB Handler function */
    );

STATUS wmbSNMPObjectEntryCreate
    (
    WMB_CID_T   component,      /* [in] Component ID                        */
    sbyte2      maxMIBObjects   /* [in] Number of Entries to allocate for   */
    );

void wmbSNMPObjectEntryDelete
    (
    WMB_CID_T   component       /* [in] Component ID                        */
    );

STATUS wmbSNMPResourceArrayRegister
    (
    WMB_CID_T   component,         /* [in] Component ID            */
    sbyte       *path,              /* [in] Path of the component   */
    WMB_HANDLER_T wmbHandler        /* [in] Pointer to the WMB Handler function */
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCwmbSNMPLibh__ */
