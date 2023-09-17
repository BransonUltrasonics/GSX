/* wmbVACMAccessLib.h - View based access security header file */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbVACMAccessLibh
#define __INCwmbVACMAccessLibh

#if defined __WMB_VIEW_ACCESS_MODEL__
#define ETC_ACCESS_EXACT  1
#define ETC_ACCESS_PREFIX 2

/* Storage definitions */
#define ETC_STO_OTHER  1
#define ETC_STO_VOL    2
#define ETC_STO_NONVOL 3
#define ETC_STO_PERM   4
#define ETC_STO_RONLY  5

/* Status definitions */
#define ETC_RS_RSNE    0
#define ETC_RS_ACTIVE  1
#define ETC_RS_NIS     2
#define ETC_RS_NREADY  3
#define ETC_RS_CAGO    4
#define ETC_RS_CAWAIT  5
#define ETC_RS_DESTROY 6


/* values for security models */
#define ETC_SEC_MODEL_ANY           0x00
#define ETC_SEC_MODEL_V1            0x01
#define ETC_SEC_MODEL_V2            0x02
#define ETC_SEC_MODEL_USM           0x03
#define ETC_SEC_MODEL_WMB_DEFAULT   0x100
#define ETC_SEC_MODEL_MAX           0x7fffffff

/* values for security levels */
#define ETC_SEC_LEVEL_NONE          0x01
#define ETC_SEC_LEVEL_AUTH          0x02
#define ETC_SEC_LEVEL_PRIV          0x03
#define ETC_SEC_LEVEL_WMB_DEFAULT   0x100

/* Group name max   */
#define ETC_ACCESS_GROUP_MAX        0x100
#define ETC_ACCESS_ACCESS_MAX       0x100
#define ETC_ACCESS_VIEW_MAX         0x100

typedef ubyte4 WMB_VACM_SECURITY_MODEL_T;
typedef ubyte4 WMB_VACM_SECURITY_LEVEL_T;

typedef struct WMB_VACM_GROUP_S
    {
    struct WMB_VACM_GROUP_S     *next;
    WMB_VACM_SECURITY_MODEL_T   model;      /* security model, an index */
    sbyte                       *uname;     /* security name, other index */
    ubyte2                      uname_len;  /* length of security name */
    sbyte*                      grp_name;   /* group name */
    sbyte2                      status;
    sbyte2                      storage;
    }WMB_VACM_GROUP_T;

typedef struct WMB_VACM_ACC_GRP_S
	{
	struct WMB_VACM_ACC_GRP_S   *next;
	sbyte                       *grp_name;	/* group name, an index */
	ubyte2                      grp_len;
	struct WMB_VACM_ACCESS_S    *access;
	} WMB_VACM_ACC_GRP_T;

typedef struct WMB_VACM_VIEW_S
    {
    sbyte                   *viewName;
    ubyte2                  viewLen;
    struct WMB_VACM_VIEW_S  *pNext;
    }WMB_VACM_VIEW_T;

typedef struct WMB_VACM_ACCESS_S
	{
	struct WMB_VACM_ACCESS_S    *next;
	WMB_VACM_ACC_GRP_T          *group;
	sbyte                       *prefix;	    /* context prefix, an index */
	ubyte2                      prefix_len;
	sbyte2                      prefix_match;   /* prefix match, exact - 1, prefix - 2 */
	WMB_VACM_SECURITY_MODEL_T   model;	        /* security model, an index */
	WMB_VACM_SECURITY_LEVEL_T   level;	        /* security level, an index */
	WMB_VACM_VIEW_T*            readview;	    /* name of view for reading */
	WMB_VACM_VIEW_T*            writeview;	    /* name of view for writing */
    sbyte2                      status;
    sbyte2                      storage;
	} WMB_VACM_ACCESS_T;

#ifndef MEMCMP_NULLOK
#define MEMCMP_NULLOK(s1, s2, n) (n ? MEMCMP(s1, s2, n) : 0)
#endif

typedef struct WMB_VACM_GROUP_ANCHOR_S
    {
    ubyte2              numGroups;
    WMB_VACM_GROUP_T    *groupList;
    }WMB_VACM_GROUP_ANCHOR_T;

typedef struct WMB_VACM_ACC_GRP_ANCHOR_S
    {
    ubyte2              numAccGrp;
    WMB_VACM_ACC_GRP_T  *accGrpList;
    }WMB_VACM_ACC_GRP_ANCHOR_T;

typedef ubyte4  WMB_VACM_VIEWGROUP_INDEX;
typedef struct WMB_VACM_VIEW_ENTRY_S
    {
    sbyte                           *viewName;
    ubyte4                          numGroups;
    WMB_VACM_VIEWGROUP_INDEX        *viewGroupList;
    }WMB_VACM_VIEW_ENTRY_T;

#define WMB_VACM_GroupStatusSet(G,s) (G->status = s)
#define WMB_VACM_GroupStatusGet(G)   (G->status)

#define WMB_VACM_Group_Set_Defaults(G) (MEMSET((G), 0, sizeof(WMB_VACM_GROUP_T)), \
				    (G)->storage = ETC_STO_NONVOL,        \
				    (G)->status  = ETC_RS_NREADY)
#define WMB_VACM_Access_Set_Defaults(A) (MEMSET((A), 0, sizeof(WMB_VACM_ACCESS_T)),\
					(A)->prefix_match = ETC_ACCESS_EXACT, \
					(A)->storage = ETC_STO_NONVOL,        \
                    (A)->readview = NULL,   \
                    (A)->writeview = NULL,   \
					(A)->status  = ETC_RS_NIS)
#define WMB_VACM_Access_ReadViewSet(A, str, len) wmbVACMAccessViewSet(&(A->readview), str, len)
#define WMB_VACM_Access_WriteViewSet(A, str, len) wmbVACMAccessViewSet(&(A->writeview), str, len)
#define WMB_VACM_Access_ReadViewGet(A) (A->readview)
#define WMB_VACM_Access_WriteViewGet(A) (A->writeview)
#define WMB_VACM_AccessStatusSet(A,s) (A->status = s)
#define WMB_VACM_AccessStatusGet(A) (A->status)
#define wmbVACMViewGroupEntryGet(x) NULL

/*
 Function Prototypes
 */


#ifdef __cplusplus
extern "C" {
#endif
STATUS wmbVACMGroupLookup
    (
    WMB_VACM_SECURITY_MODEL_T   secModel,   /* [in] Security Model  */
    sbyte                       *userName,  /* [in] User Name to search for */
    sbyte2                      nameLength, /* [in] Length of the user name */
    WMB_VACM_GROUP_T            **ppGroup   /* [out] Returned Group pointer */
    );
STATUS wmbVACMGroupCreate
    (
    WMB_VACM_GROUP_T            **ppGroup   /* [out] Returned Group pointer         */
    );
void wmbVACMGroupDestroy
    (
    WMB_VACM_GROUP_T   *group  /* [in] Pointer to group to destroy */
    );
STATUS wmbVACMGroupNameSet
    (
    WMB_VACM_GROUP_T    *pGroup,    /* [in] Group pointer               */
    sbyte               *pName,     /* [in] Group Name                  */
    ubyte2              nameLen     /* [in] Length of the group name    */
    );
STATUS wmbVACMGroupInstall
    (
    WMB_VACM_GROUP_T        *in_group,      /* [in] Pointer to the group to install */
    WMB_VACM_SECURITY_MODEL_T   sec_model,  /* [in] Security model (v1, v2, v3,..   */
    sbyte                   *uname,         /* [in] User name                       */
    ubyte2                  uname_len       /* [in] User name length                */
    );
void wmbVACMGroupDeinstall
    (
    WMB_VACM_GROUP_T    *in_group   /* [in] Pointer to the group to deinstall   */
    );
STATUS wmbVACMAccessFind
    (
    sbyte                       *grp_name,  /* [in] Name of the group to search */
    ubyte2                      grp_len,    /* [in] Group name length           */
    sbyte                       *context,   /* [in] Backplane Name              */
    ubyte2                      con_len,    /* [in] Backplane name length       */
    WMB_VACM_SECURITY_MODEL_T   sec_model,  /* [in] Security model to use       */
    WMB_VACM_SECURITY_LEVEL_T   sec_level,  /* [in] Security level used         */
    WMB_VACM_ACCESS_T           **ppAccess  /* [out] Pointer to the access structure */
    );
void wmbVACMAccessDestroy
    (
    WMB_VACM_ACCESS_T *access   /* [in] Pointer to the access structure to destroy */
    );
STATUS wmbVACMAccessCreate
    (
    WMB_VACM_ACCESS_T **ppAccess    /* [out] Returned pointer to the access structure */
    );
STATUS wmbVACMAccessInstall
    (
    WMB_VACM_ACCESS_T   *in_access, /* [in] Pointer to the access structure     */
    sbyte               *grp_name,  /* [in] Group name                          */
    ubyte2              grp_len,    /* [in] Length of the group name            */
    WMB_VACM_SECURITY_MODEL_T   sec_model,  /* [in] Security Model              */
    WMB_VACM_SECURITY_LEVEL_T   sec_level   /* [in] Security Level              */
    );
void wmbVACMAccessDeinstall
    (
    WMB_VACM_ACCESS_T *in_access    /* [in] Pointer to the access to deinstall  */
    );
STATUS wmbVACMAccessViewSet
    (
    WMB_VACM_VIEW_T     **pViewRoot,    /* [in] pointer to the root of the view root */
    sbyte               *str,           /* [in] pointer to the view name string      */
    ubyte2              len             /* [in] length of the view name              */
    );
STATUS wmbVACMViewCheck
    (
    WMB_OBJECT_T    *pObject,       /* [in] Pointer to the start of object list   */
    WMB_VACM_VIEW_T *viewName       /* [in] Pointer to the view name              */
    );
STATUS wmbVACMViewRegister
    (
    sbyte *                 pComponent,     /* [in] Component Name                 */
    ubyte2                  numEntries,     /* [in] Number of entries to register   */
    WMB_VACM_VIEW_ENTRY_T   *pEntry         /* [in] Pointer to the start of Entry array */
    );
#ifdef __cplusplus
}
#endif

#endif /* __WMB_VIEW_ACCESS_MODEL__ */

#endif /* __INCwmbVACMAccessLibh */
