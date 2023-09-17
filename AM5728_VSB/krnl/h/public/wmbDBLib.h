/* wmbDBLib.h */

/* Copyright 2004, 2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,09jul10,m_z  remove compile warning
01c,30mar10,shi  VxWorks 64bit support (WIND00193649)
01b,04mar05,dlr  Fix WorkBench passive warning.
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbDBLibh
#define __INCwmbDBLibh

/* constants */

#define WMB_DATABASE_ENTRY_NAME_SIZE_K  32

/* use for building static WMB_TABLE_T initalization tables */

#define DB_ROW_INT(n, d, h, f)  {n, &d,  DATATYPE_INTEGER_K, sizeof(d), 0, 0, f, h}
#define DB_ROW_STR(n, d, h, f)  {n,  d,  DATATYPE_STRING_K,  sizeof(d), 0, 0, f, h}

#define MAGIC_DATABASE_K    0xabcd1234
#define wmbDBBless(x)       wmMagicBless(x, MAGIC_DATABASE_K)
#define wmbDBBad(x)         wmMagicBad(x, MAGIC_DATABASE_K)

/* object access */

#define kDisabledAccess     0
#define kReadAccess         1
#define kWriteAccess        2
#define kEncryptData        4
#define kReadWriteAccess    (kReadAccess | kWriteAccess)

/* cache object access */

#define kCacheNO            0
#define kCacheRead          1
#define kCacheWrite         2
#define kCacheReadWrite     (kCacheRead | kCacheWrite)

/* element(MagicMarkup) types */

#define kMacroCommand       0
#define kIndexedElement     1
#define kNormalElement      2

/* datatypes & structures */

/* non-static, i.e., runtime data */

typedef struct DB_EXEC_T
    {
    ubyte2          flags;
    ubyte2          readers;            /* number of readers of this entry */
    void *          id;                 /* component or session id */
    WMB_LOCK_HEAD_T *pLockHead;         /* pointer to the head of the Lock queue */
    struct DB_TABLE_T *pParent;         /* pointer to any parent table */
    }
    DB_EXEC_T;

typedef struct  DB_ENTRY_T
    {
    WMB_ENTRY_T *   pEntry;             /* static registration info */
    DB_EXEC_T       exec;
    }
    DB_ENTRY_T;

typedef struct  DB_TABLE_T
    {
    WMB_TABLE_T *   pTable;             /* static registration info */
    DB_EXEC_T       exec;
    }
    DB_TABLE_T;

/* lump consolidated entries into blocks */

typedef struct DB_BLOCK_T
    {
    struct DB_BLOCK_T * pNext;
    DB_ENTRY_T *        pDbEntries;
    WMB_ENTRY_T *       pWmbEntries;
    DB_TABLE_T *        pDbTables;
    WMB_TABLE_T *       pWmbTables;
    sbyte *             pPath;
    sbyte4              rows;
    }
    DB_BLOCK_T;


typedef struct DB_NAMES_T
    {
    struct DB_NAMES_T * pNext;
    sbyte * pData;              /* data storage area        */
    sbyte4  max;                /* total storage allocated  */
    sbyte4  used;               /* how much used up         */
    }
    DB_NAMES_T;


typedef struct WMB_DB_T
    {
#ifndef __NO_MAGIC__
    ubyte4                      magic;  /* data integrity */
#endif
    ubyte4                      flags;
    sbyte4                      count;       /* number of elements in db */
    sbyte4                      max;         /* maximum number of elements */
    sbyte4                      tableCount;  /* number of table elements in db */
    sbyte4                      tableMax;    /* maximum number table of elements */
    OS_SEM_T                    semRead;
    OS_SEM_T                    semWrite;
    MEM_POOL_ID                 memID;
    DB_BLOCK_T *                pBlock;
    DB_ENTRY_T *                pEntries;
    DB_TABLE_T *                pTables;
    struct WMB_HASH_TABLE_T *   pHash;
    struct WMB_HASH_TABLE_T *   pTableHash;
    }
    WMB_DB_T;


#define DB_FLAG_STATIC          0x0001  /* data is always available         */
#define DB_FLAG_ALIASED         0x0002  /* this entry points to real entry  */
#define DB_FLAG_CALC            0x0004  /* data is calculated, not stored   */
#define DB_FLAG_QUALIFIED       0x0008  /* must be accessed using full name */
#define DB_FLAG_WRITE_LOCK      0x0010  /* data is unavailable              */
#define DB_FLAG_SESSION         0x0020  /* data is a session variable       */
#define DB_FLAG_INSTANCE        0x0040  /* use value as OID to get SNMP data*/
#define DB_FLAG_STRING_OID      0x0080  /* cookie is ascii oid              */
#define DB_FLAG_COMPONENT       0x0100  /* data is a session variable       */
#define DB_FLAG_READONLY_K      0x0200  /* Resource is read only                    */
#define DB_FLAG_LEVEL_TABLE_K   0x0400  /* Lock at Table (default = Object)         */
#define DB_FLAG_LEVEL_COMP_K    0x0800  /* Lock at Component (default = Object)     */
#define DB_FLAG_LOCK_SELF_K     0x1000  /* handler locks the resources internally   */
#define DB_FLAG_MALLOC          0x2000  /* db entry must be free'd          */
#define DB_FLAG_OID_COOKIE_K    0x4000  /* entry's data cookie is binary OID */
#define DB_FLAG_INDEX_IMPLIED_K 0x8000  /* This is an OID-type IMPLIED Index */

#define wmbDBNameGet(x)         (x)->name
#define wmbDBNameSet(x, y)      STRNCPY((x)->name, y, WMB_DATABASE_ENTRY_NAME_SIZE_K - 1)
#define wmbDBMemGet(x)          (x)->memID
#define wmbDBMemSet(x,y)        (x)->memID = y

#define wmbDBCountGet(x)        (x)->count
#define wmbDBCountSet(x, y)     (x)->count  = y
#define wmbDBMaxGet(x)          (x)->max
#define wmbDBMaxSet(x, y)       (x)->max    = y
#define wmbDBHashGet(x)         (x)->pHash
#define wmbDBHashSet(x, y)      (x)->pHash  = y
#define wmbDBEntriesGet(x)      (x)->pEntries
#define wmbDBEntriesSet(x,y)    (x)->pEntries = y

#define wmbDBTableCountGet(x)       (x)->tableCount
#define wmbDBTableCountSet(x, y)    (x)->tableCount  = y
#define wmbDBTableMaxGet(x)         (x)->tableMax
#define wmbDBTableMaxSet(x, y)      (x)->tableMax    = y
#define wmbDBTableHashGet(x)        (x)->pTableHash
#define wmbDBTableHashSet(x, y)     (x)->pTableHash  = y
#define wmbDBTablesGet(x)           (x)->pTables
#define wmbDBTablesSet(x,y)         (x)->pTables = y


/*
#define wmbDBEntryFlagIsSet(x,y) ((x)->flags & y)
#define wmbDBEntryFlagSet(x,y)  ((x)->flags |= y)
*/

/* ENTRIES: access macros for wmbDB entries */

#define dbEntry(x)                       ((DB_ENTRY_T *) x)
#define wmbEntry(x)                      (dbEntry(x))->pEntry
#define dbExec(x)                        (dbEntry(x))->exec

#define wmbEntryNameGet(x)               wmbEntry(x)->pName
#define wmbEntryReadAccessGet(x)         wmbEntry(x)->accessRead
#define wmbEntryWriteAccessGet(x)        wmbEntry(x)->accessWrite
#define wmbEntryTypeGet(x)               wmbEntry(x)->type
#define wmbEntryDataGet(x)               wmbEntry(x)->pData
#define wmbEntryHandlerGet(x)            wmbEntry(x)->handler
#define wmbEntrySizeGet(x)               wmbEntry(x)->size
#define wmbEntryLocalGet(x)              wmbEntry(x)->localId
#define wmbEntryIdGet(x)                 dbExec(x).id
#define wmbEntryLockHeadGet(x)           dbExec(x).pLockHead
#define wmbEntryTableGet(x)              dbExec(x).pParent

#define wmbEntryNameSet(x,y)             (wmbEntry(x)->pName     = y)
#define wmbEntryReadAccessSet(x, y)      (wmbEntry(x)->accessRead  = y)
#define wmbEntryWriteAccessSet(x, y)     (wmbEntry(x)->accessWrite = y)
#define wmbEntryTypeSet(x, y)            (wmbEntry(x)->type      = y)
#define wmbEntryDataSet(x, y)            (wmbEntry(x)->pData     = y)
#define wmbEntryHandlerSet(x, y)         (wmbEntry(x)->handler   = y)
#define wmbEntrySizeSet(x, y)            (wmbEntry(x)->size      = y)
#define wmbEntryLocalSet(x, y)           (wmbEntry(x)->localId   = y)
#define wmbEntryIdSet(x, y)              (dbExec(x).id           = y)
#define wmbEntryLockHeadSet(x, y)        (dbExec(x).pLockHead    = y)
#define wmbEntryTableSet(x, y)           (dbExec(x).pParent      = y)

#define wmbEntryCidSet(x, y)            dbExec(x).cid          = y

#define wmbEntryReader(x)               dbExec(x).readers

/* these represent runtime flags */

#define wmbEntryFlagSet(x,y)            dbExec(x).flags = (ubyte2)(dbExec(x).flags | (ubyte2)(y))
#define wmbEntryFlagClear(x,y)          dbExec(x).flags = (ubyte2)(dbExec(x).flags & (ubyte2)(~y))
#define wmbEntryFlagGet(x,y)            (y == (dbExec(x).flags & y))

/* these represent static initialized flags */

#define wmbDBEntryFlagSet(x,y)          wmbEntry(x)->flags |= y
#define wmbDBEntryFlagClear(x,y)        wmbEntry(x)->flags &= ~y
#define wmbDBEntryFlagGet(x,y)          y == (wmbEntry(x)->flags & y)



/* TABLES: access macros for wmbDB Tables */

#define dbTable(x)                       ((DB_TABLE_T *) x)
/* #define wmbTable(x)                      (dbTable(x))->pTable */
#define dbTableExec(x)                   (dbTable(x))->exec

#define wmbTblNameGet(x)                wmbTable(x)->pName
#define wmbTableIndexCountGet(x)        wmbTable(x)->indexCount
#define wmbTableIndexNamesGet(x)        wmbTable(x)->pIndexNames
#define wmbTableEntryCountGet(x)        wmbTable(x)->memberCount
#define wmbTableFirstEntryGet(x)        wmbTable(x)->firstEnum
#define dbTableIdGet(x)                 dbExec(x).id
#define dbTableLockHeadGet(x)           dbExec(x).pLockHead


#define wmbTableNameSet(x,y)            wmbTable(x)->pName   = y
#define wmbTableIndexCountSet(x,y)      wmbTable(x)->indexCount = y
#define wmbTableIndexNamesSet(x,y)      wmbTable(x)->pIndexNames  = y
#define wmbTableEntryCountSet(x,y)      wmbTable(x)->memberCount = y
#define wmbTableFirstEntrySet(x,y)      wmbTable(x)->firstEnum = y
#define wmbTableLocalSet(x, y)          wmbTable(x)->localId = y
#define dbTableIdSet(x, y)              dbExec(x).id         = y
#define dbTableLockHeadSet(x, y)        dbExec(x).pLockHead  = y

#define wmbTableCidSet(x, y)             dbExec(x).cid        = y

#define wmbTableReader(x)                dbExec(x).readers

/* these represent runtime flags */

#define wmbTableFlagSet(x,y)             dbExec(x).flags = (ubyte2)(dbExec(x).flags | (ubyte2)(y))
#define wmbTableFlagClear(x,y)           dbExec(x).flags = (ubyte2)(dbExec(x).flags & (ubyte2)(~y))
#define wmbTableFlagGet(x,y)             y == (dbExec(x).flags & y)

/* these represent static initialized flags */

#define wmbDBTableFlagSet(x,y)          wmbTable(x)->flags |=  y
#define wmbDBTableFlagClear(x,y)        wmbTable(x)->flags &|=  ~y
#define wmbDBTableFlagGet(x,y)          y == (wmbTable(x)->flags & y)


/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

/* ENTRY Prototypes */
STATUS wmbEntryReadLock (WMB_DB_T * pDB, DB_ENTRY_T * pEntry);
STATUS wmbEntryWriteLock (WMB_DB_T * pDB, DB_ENTRY_T * pEntry);
STATUS wmbEntryReadUnlock (WMB_DB_T * pDB, DB_ENTRY_T * pEntry);
STATUS wmbEntryWriteUnlock (WMB_DB_T * pDB, DB_ENTRY_T * pEntry);

DB_ENTRY_T * wmbDBEntryGet (WMB_DB_T * pDB, sbyte * pName,
                            void * pID, BOOL isSession);

/* TABLE Prototypes */
STATUS
wmbTableReadLock (WMB_DB_T * pDB, DB_TABLE_T * pTable);
STATUS wmbTableWriteLock (WMB_DB_T * pDB, DB_TABLE_T * pTable);
STATUS wmbTableReadUnlock (WMB_DB_T * pDB, DB_TABLE_T * pTable);
STATUS wmbTableWriteUnlock (WMB_DB_T * pDB, DB_TABLE_T * pTable);

DB_TABLE_T * wmbDBTableGet (WMB_DB_T * pDb, sbyte * pTableName,
                            void * sessionOrComponent, BOOL previousArgIsSession);

ubyte2  wmbDBTableNxCountGet (DB_TABLE_T * tableEntry);
sbyte * wmbDBTableNxNameGet  (DB_TABLE_T * tableEntry, ubyte2 i);


/* wmbDB initialization routines */

STATUS wmbDBConstruct (MEM_POOL_ID memPoolID, sbyte4 entryCount, sbyte4 tableCount,
                       sbyte * pName, BOOL temp, WMB_DB_T ** ppDB);

STATUS wmbDBLockDown(WMB_DB_T * pDB);


/* query wmbDB methods */

STATUS wmbDBQueryValue   (WMB_DB_T * pDB, sbyte *pName, sbyte **ppValue );

/* methods for accessing data objects within the indigenous system */

STATUS wmbDBWriteData      (WMB_DB_T * pDB, sbyte *pName, sbyte *pData, sbyte *pArgs);
STATUS wmbDBReadData       (WMB_DB_T * pDB, sbyte *pName, sbyte *pBuf, sbyte *pArgs);


/* Private Prototypes */

STATUS  wmbDBEntryCreate (WMB_DB_T * pDB, sbyte * pName, void  * pData,
                          DATATYPE_T dataType, size_t size, ubyte4 flags,
                          WMB_HANDLER_T handler, DB_ENTRY_T ** ppEntry);

STATUS  wmbDbEntryTempGet (WMB_ID_T backplane, void * pId,
                           sbyte * pName, DB_ENTRY_T * * ppEntry);

STATUS  wmbDBAdd (WMB_DB_T * pDB, WMB_CID_T component,
                  WMB_ENTRY_T * pEntries, sbyte4 entryCount,
                  sbyte * pPath);

STATUS  wmbDBEntriesShow (WMB_SESSION_T * pSession, sbyte * pMatch, BOOL showValues);

STATUS  wmbEntryAllowed (WMB_SESSION_T * pSession, DB_ENTRY_T * pEntry, BOOL write);

sbyte * wmbDBEntryName(void * pEntry);


STATUS  wmbDBTableCreate (WMB_DB_T *      pDB,         /* database pointer */
                          sbyte *         pName,       /* database entry name pointer */
                          sbyte4          indexCount,  /* Number of index names for table */
                          sbyte * *       pIndexNames, /* Pointer to array of index names */
                          sbyte4          memberCount, /* Number of WindMark member entries */
                          WMB_ENTRY_T *   firstEnum,   /* Index to first member WindMark entry */
                          DB_TABLE_T **   ppTable      /* pointer to new db entry pointer */
                          );

STATUS  wmbDbTableTempGet (WMB_ID_T backplane, void * pId,
                           sbyte * pName, DB_TABLE_T * * ppTable);

STATUS  wmbDBTableAdd (WMB_DB_T * pDB, WMB_CID_T component, WMB_TABLE_T * pTables,
                  sbyte4 rows, sbyte * pPath);

STATUS  wmbDBTablesShow (WMB_SESSION_T * pSession, sbyte * pMatch, BOOL showValues);

DB_ENTRY_T * wmbDBEntriesIterator
    (
    sbyte *         pMatch,     /* wild card match: "abc*","compname / *" (but remove spaces), NULL */
    void *      	pID,		/* Session/Component ID,or NULL for all entries */
    BOOL        	isSession,	/* pID = (isSession) ? session:component id */
    ubyte4        * p_ulIndex	/* iterator index. First time should = zero */
    );

STATUS  wmbTableAllowed (WMB_SESSION_T * pSession, DB_TABLE_T * pTable, BOOL write);

sbyte * wmbDBTableName(void * pTable);


STATUS  wmbDBList (WMB_SESSION_T * pSession);

STATUS  wmbDBDestruct (WMB_DB_T *  pDB);

STATUS  wmbDBRemove (WMB_CID_T cid);

#ifdef __cplusplus
}
#endif

#endif /* __INCwmbDBLibh */
