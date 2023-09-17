/* wmbTable.h - header file for wmbTables */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmbTableStubH
#define __INCwmbTableStubH

/* create a table, select the indexing column, the OID of the first instance, and provide a comma separated list of columns */
/* the indexColumn string is also used to identify the table in all the API calls */
STATUS wmbTableCreate (RCB_SESSION_T * pSession, sbyte * indexColumn, sbyte * firstInstance, sbyte * selectedColumns);

/* set a OID filter for this table */
STATUS wmbTableFilter (RCB_SESSION_T * pSession, sbyte * indexColumn, sbyte * filterString, sbyte * filterMode);

/* skip a number of rows */
STATUS wmbTableSkipAhead (RCB_SESSION_T * pSession, sbyte * indexColumn, ubyte4 skipAhead);

/* get a table element from the _given_ column and the _current_ row, in string format */
STATUS wmbTableGet (RCB_SESSION_T * pSession, sbyte * indexColumn, sbyte * stringValue, ubyte4 buffSize);

/* move to the next table row */
STATUS wmbTableEndRow (RCB_SESSION_T * pSession, sbyte * indexColumn);

/* self-explanatory */
STATUS wmbTableDestroy (RCB_SESSION_T * pSession, sbyte * indexColumn);

#endif /* __INCwmbTableStubH */
