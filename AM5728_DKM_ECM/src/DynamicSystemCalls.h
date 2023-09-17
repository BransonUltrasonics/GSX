/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef DYNAMICSYSTEMCALLS_H_
#define DYNAMICSYSTEMCALLS_H_

#include  <sys/types.h>
#include  <EcType.h>

#define NO_OF_SYS_CALL 			6

#define NO_OF_PARAM_DL_REQ		6
#define NO_OF_PARAM_UL_REQ		5
#define NO_OF_PARAM_EMPTY		0
#define NO_OF_PARAM_FOE_DL_REQ		5

#define DL_REQ_SYMB_NAME		"COE_DL_REQ"
#define UL_REQ_SYMB_NAME		"COE_UL_REQ"
#define ECAT_STATE_SYMB_NAME	"ECAT_STATE"
#define EXIT_ECAT_SYMB_NAME		"ECAT_EXIT"
#define FOE_DL_REQ_SYMB_NAME    "FOE_DL_REQ"
#define ECAT_BUS_SCAN_COUNT		"ECAT_SCAN_COUNT"

#define GROUP_NAME				"SC_EC_MASTER"
#define GROUP_NUMBER			3

typedef struct SDO_DOWNLOAD_REQ
{
	int iODIndex;
	int iODSubIndex;
	int iSlaveInstance;
	int iAccessType;
	int idataLength;
	int *pSdoData;
}SDO_DOWNLOAD_REQ;

typedef struct SDO_UPLOAD_REQ
{
	int iODIndex;
	int iODSubIndex;
	int iSlaveInstance;
	int idataLength;
	int iAccessType;
}SDO_UPLOAD_REQ;

typedef struct FOE_DOWNLOAD_REQ
{
	int iSlaveInstance;
	int iFileNameLength;
	int *iFileName;
	int iFileLength;
	int *pFileData;
	
}FOE_DOWNLOAD_REQ;


STATUS SystemCallRegisty();
EC_T_DWORD CoeSdoDownloadReqSysCall(SDO_DOWNLOAD_REQ *DL_REQ_DATA);
EC_T_DWORD CoeSdoUploadReqSysCall(SDO_UPLOAD_REQ *UL_REQ_DATA);
EC_T_DWORD FoeDownloaddReqSysCall(FOE_DOWNLOAD_REQ *FOE_DL_REQ_DATA);
EC_T_STATE EcatGetMasterStateSysCall();
void ExitEcatMasterSysCall();


#endif /* DYNAMICSYSTEMCALLS_H_ */
