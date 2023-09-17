/*-----------------------------------------------------------------------------
 * EcPts.h       
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Florian Betz
 * Description              Pass-Through-Server
 *---------------------------------------------------------------------------*/

#ifndef INC_ECPTS
#define INC_ECPTS

/*-INCLUDES------------------------------------------------------------------*/
#include <EcOs.h>

/*-DEFINES-------------------------------------------------------------------*/
#define PTS_ETHERCAT_ETHERTYPE       0xA488    /* big endian */
#define PTS_ADMIN_MSG_ETHERTYPE      0xA588    /* big endian */

#define PTS_MAX_MSG_LEN              255

/* Administrative message type */
#define PTS_ADM_MSG_ERROR            1
#define PTS_ADM_MSG_KEEP_ALIVE       2

/*-TYPEDEFS------------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(1)

/* Part of the administrative message for the Pass-Through-Server. See also EC_T_PTS_ADMIN_MSG */
typedef struct _EC_T_PTS_ERROR_MSG
{
    EC_T_DWORD dwErrorCode; 
    EC_T_CHAR szAdditionalMsg[PTS_MAX_MSG_LEN];
} EC_PACKED(1) EC_T_PTS_ERROR_MSG;


/* Administrative message structure for the Pass-Through-Server */
typedef struct _EC_T_PTS_ADMIN_MSG
{
    EC_T_BYTE abyMacSource[6];  
    EC_T_BYTE abyMacDest[6];
    EC_T_WORD wEtherType;
    EC_T_WORD wDataLength;      /* length of the following administrative message */
    EC_T_WORD wMsgType;         /* type of the following administrative message */
    union EC_T_PTS_ADMIN_MSGS
    {
        EC_T_PTS_ERROR_MSG oInfoMsg;
    }oAdminMsgs;
} EC_PACKED(1) EC_T_PTS_ADMIN_MSG;

#include EC_PACKED_INCLUDESTOP

#endif /* INC_ECPTS */

/*-END OF SOURCE FILE--------------------------------------------------------*/
