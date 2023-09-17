/* wmbTableLib.h - header file for wmbTableLib */

/* Copyright 2000-2003,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,30mar10,shi  VxWorks 64bit support (WIND00193649)
01a,16jun03,jws  Modification history and some WRS coding standards additions.
*/


/*
 * DESCRIPTION:
 *
 * To be added.
 *
 */

#ifndef __INCwmbTableLibh
#define __INCwmbTableLibh

/* bits in the flags member of table structure (defined in wmbTableLib.c) */

#define WMB_TABLE_FLAG_INVALID      0x80000000
#define WMB_TABLE_GET_NEXT_FAILED   1


#define wmbTable(x)     ((WMB_TABLE_T *) x)
#define dbTable(x)      ((DB_TABLE_T *) x)

#define wmbTableLockHeadGet(pTable)             dbTable(pTable)->exec.pLockHead
#define wmbTableLockHeadSet(pTable, pHead)      dbTable(pTable)->exec.pLockHead = pHead


#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmbTableColumnCreate (WMB_OBJ_T obj, sbyte4 baseLength,
                              sbyte * pTitle, sbyte4 width);

STATUS  wmbTableColumnDestroy (WMB_OBJ_T obj);

sbyte4  wmbTableColumnLength (WMB_OBJ_T obj);

void    wmbTableColumnLengthUpdate (WMB_OBJ_T obj);

STATUS  wmbTableRowNext (void * table);

STATUS  wmbTableCreate (WMB_SESSION_T * pSession, sbyte * pIndexColumn,
                        sbyte * pFirstInstance, sbyte * pSelectedColumns);

STATUS  wmbTableDestroy (WMB_SESSION_T * pSession);

STATUS  wmbTableEndRow (WMB_SESSION_T * pSession);

STATUS  wmbTableFilter (WMB_SESSION_T * pSession,
                        sbyte * pFilterString, sbyte * pFilterMode);

ubyte4  wmbTableFlagsGet (WMB_SESSION_T *);

STATUS  wmbTableGet (WMB_SESSION_T * pSession, sbyte * pColumnName, sbyte * pStringValue,
                     size_t buffSize);

STATUS  wmbTableInstanceGet (WMB_SESSION_T * pSession, sbyte * pTableName,
                             sbyte * pStringBuf, size_t buffSize,
                             size_t * bWritten);

BOOL    wmbTableMember (WMB_SESSION_T * pSession, WM_OID_T oid);

WM_BUFFER_T * wmbTableInstanceSfxGet (WMB_SESSION_T * pSession, char * pName);

STATUS  wmbTablePrint (WMB_SESSION_T * pSession);

WM_OID_T wmbTableRoot (WMB_TID_T tid);

STATUS  wmbTableSkipAhead (WMB_SESSION_T * pSession, sbyte4 skipAhead);

STATUS wmbTableInnermostGet (WMB_SESSION_T * pSession, sbyte * pTableName,
         sbyte * pStringBuf, ubyte4 buffSize, ubyte4 * wr);

#ifdef __cplusplus
}
#endif

#endif /* __INCwmbTableLibh*/
