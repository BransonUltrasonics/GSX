/*-----------------------------------------------------------------------------
 * AtEmRasClnt.h             file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2007/5/4::7:18
 *---------------------------------------------------------------------------*/

#ifndef INC_ATEMRASCLNT
#define INC_ATEMRASCLNT 1

/*-PREDEFINES-FOR-INCLUDED-HEADERS-HEREIN------------------------------------*/
#undef  INSTANCE_MASTER_DEFAULT     /* always redefine to RAS default value */
#define INSTANCE_MASTER_DEFAULT     ((EC_T_DWORD)0x00010000) /* see CLIENT_CONNECTIONID */


/*-INCLUDES------------------------------------------------------------------*/
#include <AtEmRasType.h>
#include <AtEmRasError.h>
#include <EcOs.h>
#include "AtEmRasClntVersion.h"
#include <AtEthercat.h>

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/*-DEFINES-------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(4)
typedef struct _ATEMRAS_T_CLNTCONDESC
{
    ATEMRAS_T_IPADDR    oAddr;              /**< [in]   Server Address */
    EC_T_WORD           wPort;              /**< [in]   Server Port */
    EC_T_DWORD          dwWatchDog;         /**< [in]   Watchdog interval when to send IDL packets */
    EC_T_DWORD          dwCycleTime;        /**< [in]   Cycle Time for Recv Polling */
    EC_T_DWORD          dwWDTOLimit;        /**< [in]   Amount of cycles without receiving commands (idles) before
                                              *         Entering state wdexpired 
                                              */
    EC_T_DWORD          dwRecvPrio;         /**< [in]   Thread Priority of RAS Receive Thread */
    EC_T_DWORD          dwPktAdminSize;     /**< [in]   Slave Packet Administrative List Size */
    EC_T_PVOID          pvConHandle;        /**< [in]   Connection Handle needed for disconnect */

    EC_T_DWORD          dwInstanceID;       /**< [in]   OEM Master Instance. Only used if OEM Key given. */
    EC_T_UINT64         qwOemKey;           /**< [in]   OEM Key */
} EC_PACKED(4) ATEMRAS_T_CLNTCONDESC, *ATEMRAS_PT_CLNTCONDESC;
#define ATEMRAS_T_CLNTCONDESC_SIZE  sizeof(ATEMRAS_T_CLNTCONDESC)

typedef struct _ATEMRAS_T_CLNTPARMS
{
    EC_T_DWORD          dwKeepAliveTrigger; /**< [in]   obsolete */
    EC_T_DWORD          dwAdmPrio;          /**< [in]   Priority of Administrative task */
    EC_T_DWORD          dwAdmStackSize;     /**< [in]   Stack size of Administrative task */

    EC_T_PVOID          pvNotifCtxt;        /**< [in]   Notification context returned while calling pfNotification */
    EC_PF_NOTIFY        pfNotification;     /**< [in]   Function pointer called to notify error and status 
                                              *         information generated by Remote API Layer */
} EC_PACKED(4) ATEMRAS_T_CLNTPARMS, *ATEMRAS_PT_CLNTPARMS;
#define ATEMRAS_T_CLNTPARMS_SIZE    sizeof(ATEMRAS_T_CLNTPARMS)
#include EC_PACKED_INCLUDESTOP

/*-FUNCTION DECLARATION------------------------------------------------------*/
ATECAT_API EC_T_DWORD  emRasClntGetVersion( EC_T_VOID );
ATECAT_API EC_T_DWORD  emRasClntInit(ATEMRAS_T_CLNTPARMS oParms);
ATECAT_API EC_T_DWORD  emRasClntClose(EC_T_DWORD dwTimeout);
ATECAT_API EC_T_DWORD  emRasClntAddConnection(ATEMRAS_T_CLNTCONDESC* pConDesc, EC_T_DWORD* pdwIdMask);
ATECAT_API EC_T_DWORD  emRasClntRemoveConnection(EC_T_VOID* pvConHandle, EC_T_DWORD dwTimeout);

#if (defined INCLUDE_RAS_TRACESUPPORT)
ATECAT_API EC_T_DWORD  emRasClntTraceEnable(EC_T_BOOL bEnable);
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INC_ATEMRASCLNT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
