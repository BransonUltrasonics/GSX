/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Dynamic custom system call function definitions for accessing ESS APIs from RTP application
 
**********************************************************************************************************/


/* Header files */
#ifndef DYNAMICSYSTEMCALL_H_
#include "DynamicSystemCall.h"
#include <stdio.h>
#endif

/*		
******************************************************************************************************************

*      Function    :   obj_readSc
*      Description :   Wrapper function to call Dynamic Custom System call to read from OD  
*      Input       :   ESS_UINT16 index - Index of OD, ESS_UINT8 subindex - Sub index of OD
*      				   ESS_UINT32 objSize - Total Size of OD, TOBJECT *pObjEntry - Pointer to OD  
*      				   ESS_UINT16 * pData - Buffer in which Data is read, ESS_UINT8 bCompleteAccess - Full read
*      Output      :   Success or Failure of OD Read
*      Called by   :    :: obj_readSc

******************************************************************************************************************
*/

ESS_UINT8 obj_readSc( ESS_UINT16 index, ESS_UINT8 subindex, ESS_UINT32 objSize, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess )
{	
	return syscall((_Vx_usr_arg_t)index, (_Vx_usr_arg_t)subindex, (_Vx_usr_arg_t)objSize, (_Vx_usr_arg_t)pObjEntry , (_Vx_usr_arg_t)pData , (_Vx_usr_arg_t)bCompleteAccess, 0, 0, SYSCALL_NUMBER(3,0));
}



/*		
******************************************************************************************************************

*      Function    :   obj_getentryoffsetSc
*      Description :   Wrapper function to call Dynamic Custom System call to get the offset of data in OD  
*      Input       :   subindex - Sub index of OD
*      				   TOBJECT *pObjEntry - Pointer to OD  
*      Output      :   Offset of Each data member of OD
*      Called by   :    :: obj_getentryoffsetSc

******************************************************************************************************************
*/

ESS_UINT16 obj_getentryoffsetSc(ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry)
{
	return syscall((_Vx_usr_arg_t)subindex, (_Vx_usr_arg_t)pObjEntry , 0,  0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,1));
}



/*		
******************************************************************************************************************

*      Function    :   obj_getobjectlengthSc
*      Description :   Wrapper function to call Dynamic Custom System call to find the Size of OD
*      Input       :   ESS_UINT16 index - Index of OD, ESS_UINT8 subindex - Sub index of OD
*      				   TOBJECT *pObjEntry - Pointer to OD
*      				   ESS_UINT8 bCompleteAccess - Full read or Single data read
*      Output      :   Size of OD
*      Called by   :    :: obj_getobjectlengthSc

******************************************************************************************************************
*/

ESS_UINT32 obj_getobjectlengthSc( ESS_UINT16 index, ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT8 bCompleteAccess)
{
	return syscall((_Vx_usr_arg_t)index, (_Vx_usr_arg_t)subindex, (_Vx_usr_arg_t)pObjEntry  , (_Vx_usr_arg_t)bCompleteAccess, 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
}



/*		
******************************************************************************************************************

*      Function    :   obj_getobjecthandleSc
*      Description :   Wrapper function to call Dynamic Custom System call to get the Pointer to OD
*      Input       :   index - index of OD
*      Output      :   Address of OD of specific Index
*      Called by   :    :: obj_getobjecthandleSc

******************************************************************************************************************
*/

OBJCONST TOBJECT OBJMEM *  obj_getobjecthandleSc( ESS_UINT16 index )
{
	return (OBJCONST TOBJECT OBJMEM *)syscall((_Vx_usr_arg_t)index, 0, 0, 0, 0, 0, 0, 0, SYSCALL_NUMBER(3,3) );
}



/*		
******************************************************************************************************************

*      Function    :   obj_writeSc
*      Description :   Wrapper function to call Dynamic Custom System call to write to OD
*      Input       :   index - Index of OD, pODdata - Pointer to the Data to be written
*      				   offset - Array of offset of each data in OD, ODdatasize - Size of each data  
*      				   ObjLength - Number of data members in the OD
*      Output      :   Success or Failure of OD Write
*      Called by   :    :: obj_writeSc

******************************************************************************************************************
*/

ESS_UINT8 obj_writeSc( ESS_UINT16 index, UINT32 *pODdata, ESS_UINT16 *offset, ESS_UINT8 *ODdatasize, ESS_UINT8 ObjLength )
{	
	return syscall((_Vx_usr_arg_t)index, (_Vx_usr_arg_t)pODdata, (_Vx_usr_arg_t)offset, (_Vx_usr_arg_t)ODdatasize, (_Vx_usr_arg_t)ObjLength, 0, 0, 0, SYSCALL_NUMBER(3,4));	
}
