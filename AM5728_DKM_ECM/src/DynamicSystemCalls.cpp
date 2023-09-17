/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Dynamic Custom system calls for accessing SDO Read/Write APIs from User space  
 
**********************************************************************************************************/


#include  <syscallLib.h>
#include  <syscallArgs.h>
#include  <AtEthercat.h>
#include  "DynamicSystemCalls.h"
#include "SCInit.h"

extern EC_T_DWORD S_dwClntId;

/* Dynamic system call for SDO download request called from RTP */ 
EC_T_DWORD CoeSdoDownloadReqSysCall(SDO_DOWNLOAD_REQ *DL_REQ_DATA)
{
	EC_T_WORD dwODIndex			= 0;
	EC_T_WORD dwODSubIndex		= 0;
	EC_T_WORD wSlaveInstance	= 0;
	EC_T_BYTE dwAccess			= 0;
	EC_T_WORD wdataLength		= 0;
	EC_T_BYTE *pData			= NULL;
	
	dwODIndex 		= (EC_T_WORD)DL_REQ_DATA->iODIndex;
	dwODSubIndex	= (EC_T_WORD)DL_REQ_DATA->iODSubIndex;
	wSlaveInstance 	= (EC_T_WORD)DL_REQ_DATA->iSlaveInstance;
	dwAccess 		= (EC_T_BYTE)DL_REQ_DATA->iAccessType;
	wdataLength 	= (EC_T_WORD)DL_REQ_DATA->idataLength;
	pData 			= (EC_T_BYTE*)DL_REQ_DATA->pSdoData;
	
	return CoeSdoDownloadReq(dwODIndex, dwODSubIndex, wSlaveInstance, dwAccess, wdataLength, S_dwClntId, pData);
}

/* Dynamic system call for SDO upload request called from RTP */ 
EC_T_DWORD CoeSdoUploadReqSysCall(SDO_UPLOAD_REQ *UL_REQ_DATA)
{
	EC_T_WORD dwODIndex			= 0;
	EC_T_WORD dwODSubIndex		= 0;
	EC_T_WORD wSlaveInstance	= 0;
	EC_T_WORD wdataLength		= 0;
	EC_T_WORD dwAccess			= 0;
	
	dwODIndex 		= (EC_T_WORD)UL_REQ_DATA->iODIndex;
	dwODSubIndex	= (EC_T_WORD)UL_REQ_DATA->iODSubIndex;
	wSlaveInstance 	= (EC_T_WORD)UL_REQ_DATA->iSlaveInstance;
	wdataLength 	= (EC_T_WORD)UL_REQ_DATA->idataLength;
	dwAccess 		= (EC_T_WORD)UL_REQ_DATA->iAccessType;
	
	return CoeSdoUploadReq(dwODIndex, dwODSubIndex, wSlaveInstance, wdataLength, dwAccess, S_dwClntId);
}

/* Dynamic system call for getting master stack state - called from RTP */
EC_T_STATE EcatGetMasterStateSysCall()
{
	 return ecatGetMasterState();
}

/* Dynamic system call for Foe Download request called from RTP */ 
EC_T_DWORD FoeDownloadReqSysCall(FOE_DOWNLOAD_REQ *FOE_DL_REQ_DATA)
{
	EC_T_WORD wSlaveInstance	= 0;
	EC_T_WORD dwFileNameLen     = 0;
	EC_T_CHAR *szFileName       = NULL;
	EC_T_DWORD dwDataLen         = 0;
	EC_T_BYTE *pbyData          = NULL;
	
	wSlaveInstance 	= (EC_T_WORD)FOE_DL_REQ_DATA->iSlaveInstance;
	dwFileNameLen 	= (EC_T_WORD)FOE_DL_REQ_DATA->iFileNameLength;
	szFileName 		= (EC_T_CHAR*)FOE_DL_REQ_DATA->iFileName;
	dwDataLen 		= (EC_T_DWORD)FOE_DL_REQ_DATA->iFileLength;
	pbyData 		= (EC_T_BYTE*)FOE_DL_REQ_DATA->pFileData;
	
	return FoeDownloadReq(wSlaveInstance, dwFileNameLen, szFileName, dwDataLen, pbyData);
}


/* Dynamic system call for exit master stack application - called from RTP */
void ExitEcatMasterSysCall()
{
	bIsExitKeyPressed = true;
}

/*Dynamic system calls function registry table */
SYSCALL_RTN_TBL_ENTRY pRtnTbl [] =
 {
		 {(FUNCPTR)CoeSdoDownloadReqSysCall, NO_OF_PARAM_DL_REQ, DL_REQ_SYMB_NAME, 0},	 	/* routine 0 */
		 {(FUNCPTR)CoeSdoUploadReqSysCall, NO_OF_PARAM_UL_REQ, UL_REQ_SYMB_NAME, 1}, 		/* routine 1 */
		 {(FUNCPTR)EcatGetMasterStateSysCall, NO_OF_PARAM_EMPTY, ECAT_STATE_SYMB_NAME, 2}, 	/* routine 2 */
		 {(FUNCPTR)ExitEcatMasterSysCall, NO_OF_PARAM_EMPTY, EXIT_ECAT_SYMB_NAME, 3}, 		/* routine 3 */
		 {(FUNCPTR)FoeDownloadReqSysCall, NO_OF_PARAM_FOE_DL_REQ, FOE_DL_REQ_SYMB_NAME, 4}, /* routine 4 */
		 {(FUNCPTR)EcatGetBusScanTimeoutSysCall, NO_OF_PARAM_EMPTY, ECAT_BUS_SCAN_COUNT, 5} /* routine 5 */
};

/* Custom system call registration */
STATUS SystemCallRegisty()
{
   return syscallGroupRegister(GROUP_NUMBER, GROUP_NAME, NO_OF_SYS_CALL, (SYSCALL_RTN_TBL_ENTRY *)&pRtnTbl, 0);
}
