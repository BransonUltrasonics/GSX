/* Header files */
#include <vxWorks.h>
#include <syscallLib.h>
#include <string.h>
#include <errno.h>
#include <syscallArgs.h>
#include <scMemVal.h>
#include <objdef.h>
#include <ecat_def.h>
#include <errnoLib.h>
#define N_CUST_SYS_CALLS 5

// Custom System call input argument structures
struct obj_readScArgs
{
	int index;
	int subindex;
	int objSize;
	int *pObjEntry;
	int *pData;
	int CompleteAccess;
};


struct obj_getentryoffsetScArgs
{	
	int subindex;
	int *pObjEntry;
};

struct obj_getobjectlengthScArgs
{
	int index;
	int subindex;	
	OBJCONST TOBJECT OBJMEM *pObjEntry;
	int CompleteAccess;
};

struct obj_getobjecthandleScArgs
{
	ESS_UINT16 index;	
};



struct obj_writeScArgs
{
	int index;
	int *pODData;	
	int *offset;	
	int *ODdatasize;	
	int ObjLength;	
};

/* Custom System call function prototypes */
ESS_UINT8 obj_readSc( struct obj_readScArgs  * argsPtr);		
ESS_UINT16 obj_getentryoffsetSc(struct obj_getentryoffsetScArgs  * argsPtr);
ESS_UINT32 obj_getobjectlengthSc( struct obj_getobjectlengthScArgs * argsPtr);
OBJCONST TOBJECT OBJMEM *  obj_getobjecthandleSc( struct obj_getobjecthandleScArgs * argsPtr );
ESS_UINT8 obj_writeSc( struct obj_writeScArgs  * argsPtr);


/* Custom System call Table entry */
SYSCALL_RTN_TBL_ENTRY pRtnTbl [] =
 {
		 {(FUNCPTR) obj_readSc, 6, "obj_read", 0}, /* routine 0 */
		 {(FUNCPTR) obj_getentryoffsetSc, 2, "obj_getentryoffset", 1}, /* routine 1 */
		 {(FUNCPTR) obj_getobjectlengthSc, 4, "obj_getobjectlength", 2}, /* routine 2 */
		 {(FUNCPTR) obj_getobjecthandleSc, 1, "obj_getobjecthandle", 3}, /* routine 3 */
		 {(FUNCPTR) obj_writeSc, 5, "obj_write", 4} /* routine 4 */
};


/* Custom System call function definitions */
ESS_UINT8 obj_readSc( struct obj_readScArgs  * argsPtr)
{	 
	 ESS_UINT16 index= (ESS_UINT16)argsPtr->index;
	 ESS_UINT8 subindex=(ESS_UINT8)argsPtr->subindex;
	 ESS_UINT8 objSize=(ESS_UINT8)argsPtr->objSize;
	 OBJCONST TOBJECT OBJMEM * pObjEntry =(OBJCONST TOBJECT OBJMEM *)argsPtr->pObjEntry;
	 ESS_UINT16 MBXMEM * pData = (ESS_UINT16*)argsPtr->pData;
	 ESS_UINT8 bCompleteAccess = (ESS_UINT8)argsPtr->CompleteAccess;	 	
	 return OBJ_Read(index ,subindex,objSize,pObjEntry,pData,bCompleteAccess);	
}


ESS_UINT16 obj_getentryoffsetSc(struct obj_getentryoffsetScArgs  * argsPtr)
{
	ESS_UINT16 offset;
	ESS_UINT8 subindex=(ESS_UINT8)argsPtr->subindex;
	OBJCONST TOBJECT OBJMEM * pObjEntry =(OBJCONST TOBJECT OBJMEM *)argsPtr->pObjEntry;			
	offset = OBJ_GetEntryOffset( subindex, pObjEntry);	
	return offset;
}



ESS_UINT32 obj_getobjectlengthSc( struct obj_getobjectlengthScArgs * argsPtr)
{	 
	 ESS_UINT16 index= (ESS_UINT16)argsPtr->index;
	 ESS_UINT8 subindex=(ESS_UINT8)argsPtr->subindex;
	 OBJCONST TOBJECT OBJMEM * pObjEntry =(OBJCONST TOBJECT OBJMEM *)argsPtr->pObjEntry;
	 ESS_UINT8 bCompleteAccess = (ESS_UINT8)argsPtr->CompleteAccess;	 	 
	 return OBJ_GetObjectLength( index ,subindex,pObjEntry,bCompleteAccess);
}



OBJCONST TOBJECT OBJMEM *  obj_getobjecthandleSc( struct obj_getobjecthandleScArgs * argsPtr )
{		
	OBJCONST TOBJECT OBJMEM *ptr;
	ptr = (OBJCONST TOBJECT OBJMEM *)OBJ_GetObjectHandle(argsPtr->index);	
	return ptr;	
}



ESS_UINT8 obj_writeSc( struct obj_writeScArgs  * argsPtr)
{
	ESS_UINT16 index = (ESS_UINT16)argsPtr->index;	
	OBJCONST TOBJECT OBJMEM * pObjEntry = OBJ_GetObjectHandle(index);		
	ESS_UINT8 *ptrOd = (ESS_UINT8 *)(pObjEntry->pVarPtr);		
	UINT32 *pODdata = (UINT32 *)argsPtr->pODData;
	ESS_UINT16 *offset = (ESS_UINT16 *)argsPtr->offset;	
	ESS_UINT8 *ODdatasize = (ESS_UINT8 *)argsPtr->ODdatasize;
	ESS_UINT8 ObjLength = (ESS_UINT8)argsPtr->ObjLength;	
	ESS_UINT8 arrIndex;	
	for(arrIndex = 0; arrIndex < ObjLength; arrIndex++)
	{
		switch(ODdatasize[arrIndex])
		{
			case 1:
				* ( ptrOd + (offset[arrIndex]) ) = pODdata[arrIndex];				
				break;
			case 2:
				* (UINT16 *)( ptrOd + (offset[arrIndex]) ) = pODdata[arrIndex];				
				break;
			case 4:
				* (UINT32 *)( ptrOd + (offset[arrIndex]) ) = pODdata[arrIndex];				
				break;
			default:
				* (UINT64 *)( ptrOd + (offset[arrIndex]) ) = pODdata[arrIndex];
		}
		
	}	
	return 0;			
}


/* Custom System call registration function */
int addSyscall (void)
{
   STATUS  result = OK;
   int     groupNum = 3;
   BOOL    value = 0;

   result = syscallGroupRegister (groupNum, "ESSScGroup", N_CUST_SYS_CALLS, (SYSCALL_RTN_TBL_ENTRY *)&pRtnTbl, 0);
   return (result);
} 	/* end of addsyscall() */
