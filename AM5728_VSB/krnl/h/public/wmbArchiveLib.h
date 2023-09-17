/* wmbArchiveLib.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbArchiveLibh
#define __INCwmbArchiveLibh




/* 1.1.2.3	Archive Registration
The archive registration structure is passed as an argument to the
archive registration function.  It contains information about the
archive being registered, including a function to call to
de-register this archive.  The de-registration function will be called if the backplane shuts down or if the component is de-registered without previously de-registering its archive(s).
*/

typedef void    WMB_ARCH_DEREG_T (WMB_ARCHIVE_TYPE_T * pType);
typedef void    WMB_ARCH_DEREG_CTL_T;   /* de-registration control information */

typedef struct WMB_ARCHIVE_REG_S
    {
	WMB_CID_T  	 	 		cid;			/* unique id for this component */
	WMB_ARCHIVE_TYPE_T		type;			/* type of this archive */
	sbyte *                 pFileName;		/* filename of archive location */
	ubyte4            	 	flags;			/* flags associated with this archive */
	WMB_ARCH_DEREG_T *      pDeRegRtn;		/* de-registration callback */
	WMB_ARCH_DEREG_CTL_T *  pDeRegCtl;		/* de-registration control information */
    } WMB_ARCHIVE_REG_T;




/* 1.1.2.3.1	Archive Registration Structure Creation and Destruction */

WMB_ARCHIVE_REG_T *wmbAchiveRegCreate(void);
void wmbAchiveRegDestroy(WMB_ARCHIVE_REG_T* pArchiveReg);


/* 1.1.2.3.2	Archive Registration Structure Access */
#if 0
	WMB_CID_T  	 	 		cid;			/* unique id for this component */
	WMB_ARCHIVE_TYPE_T		type;			/* type of this archive */
	sbyte *                 pFileName;		/* filename of archive location */
	ubyte4             	 	flags;			/* flags associated with this archive */
	WMB_ARCH_DEREG_T *      pDeRegRtn;		/* de-registration callback */
	WMB_ARCH_DEREG_CTL_T *  pDeRegCtl;		/* de-registration control information */
#endif


#define ARCHIVE_M(x,y)              (((WMB_ARCHIVE_REG_T *) x)->y)


#define RAW_CID_GET_M(x, y)	        *(y) = ARCHIVE_M(x, cid)
#define RAW_CID_SET_M(x, y)	        ARCHIVE_M(x, cid) = *(y)
#define RAW_FLAGS_GET_M(x, y)	    *(y) = ARCHIVE_M(x, flags)
#define RAW_FLAGS_SET_M(x, y)	    ARCHIVE_M(x, flags) = *(y)

#define ARCHIVE_CID_GET_M(x, y)		DATA_VALID_2_M (x, y, RAW_CID_GET_M (x, y))
#define ARCHIVE_CID_SET_M(x, y)		DATA_VALID_2_M (x, y, RAW_CID_SET_M (x,y))

#define WMB_ARCHIVE_RESET_M(x)      DATA_ZERO_M(x)

STATUS wmbArchiveRegSetCid(		WMB_ARCHIVE_REG_T* pArchiveReg, WMB_CID_T cid);


STATUS wmbArchiveRegSetCid(		WMB_ARCHIVE_REG_T* pArchiveReg, WMB_CID_T cid);
STATUS wmbArchiveRegSetFlags(   WMB_ARCHIVE_REG_T* pArchiveReg, ubyte4  flags);
STATUS wmbArchiveRegSetFileName(WMB_ARCHIVE_REG_T* pArchiveReg, sbyte * pFileName);
STATUS wmbArchiveRegSetDeregRtn(WMB_ARCHIVE_REG_T* pArchiveReg, WMB_RES_DEREG_T *pDeregRtn);
STATUS wmbArchiveRegSetDeregCtl(WMB_ARCHIVE_REG_T* pArchiveReg, WMB_RES_DEREG_CTL_T*   pDeregCtl);

WMB_CID_T  	 	 	    wmbArchiveRegGetCid(	 	WMB_RESOURCE_LIST_T* pResList);
ubyte4             	    wmbArchiveRegGetFlags(  	WMB_RESOURCE_LIST_T* pResList);
sbyte *			        wmbArchiveRegGetFileName(	WMB_RESOURCE_LIST_T* pResList);
struct WMB_RES_DEREG_T *wmbArchiveRegGetDeregRtn (	WMB_RESOURCE_LIST_T* pResList);
WMB_RES_DEREG_CTL_T *   wmbArchiveRegGetDeregCtl (	WMB_RESOURCE_LIST_T* pResList);


#ifdef __cplusplus
extern "C" {
#endif


/*
If a component de-registers without de-registering its resources,
each resource entry will be de-registered (using the call passed in on registration).
2.1.3	Archive Registration and Deregistration API
Data consumers will typically register a set of archives
to allow management via Web, CLI, SNMP, etc.  One call will be made
for each archive that is registered.
This function also returns a success or failure indication.
*/

STATUS wmbArchiveRegister (WMB_ARCHIVE_REG_T *pArchiveReg);

/*
At any time, a component can de-register one (or more) of its archives using the following API call.
*/

void wmbArchiveDeregister (WMB_CID_T cid, WMB_ARCHIVE_TYPE_T *pType);



#ifdef __cplusplus
}
#endif

#endif /* __INCwmbArchiveLibh */
