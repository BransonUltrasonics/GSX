/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef DYNAMICSYSTEMCALL_H_
#define DYNAMICSYSTEMCALL_H_


/* Header files */
#include <vxWorks.h>
#include <objdef.h>
#include <ecat_def.h>
#include <syscall.h>

/* Function Prototypes */
ESS_UINT8 obj_readSc( ESS_UINT16 index, ESS_UINT8 subindex, ESS_UINT32 objSize, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess );

ESS_UINT16 obj_getentryoffsetSc(ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);

ESS_UINT32 obj_getobjectlengthSc( ESS_UINT16 index, ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT8 bCompleteAccess);

OBJCONST TOBJECT OBJMEM *  obj_getobjecthandleSc( ESS_UINT16 index );

ESS_UINT8 obj_writeSc( ESS_UINT16 index, UINT32 *pODdata, ESS_UINT16 *offset, ESS_UINT8 *ODdatasize, ESS_UINT8 ObjLength );

#endif /* DYNAMICSYSTEMCALL_H_ */
