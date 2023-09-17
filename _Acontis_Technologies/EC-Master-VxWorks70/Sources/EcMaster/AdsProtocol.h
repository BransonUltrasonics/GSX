/*-----------------------------------------------------------------------------
 * AdsProtocol.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Vladimir Pustovalov
 * Description              ADS protocol declaration
 *---------------------------------------------------------------------------*/


#ifndef INC_ADS_PROTOCOL_H
#define INC_ADS_PROTOCOL_H

/*-INCLUDES------------------------------------------------------------------*/

#include <AtEthercat.h>


/*-DEFINES-------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   ADS Command IDs
 *---------------------------------------------------------------------------*/
#define ADSCMDID_INVALID                (EC_T_WORD)0x00
#define ADSCMDID_READDEVICEINFO         (EC_T_WORD)0x01
#define ADSCMDID_READ                   (EC_T_WORD)0x02
#define ADSCMDID_WRITE                  (EC_T_WORD)0x03
#define ADSCMDID_READSTATE              (EC_T_WORD)0x04
#define ADSCMDID_WRITECTRL              (EC_T_WORD)0x05
#define ADSCMDID_ADDDEVICENOTE          (EC_T_WORD)0x06
#define ADSCMDID_DELDEVICENOTE          (EC_T_WORD)0x07
#define ADSCMDID_DEVICENOTE             (EC_T_WORD)0x08
#define ADSCMDID_READWRITE              (EC_T_WORD)0x09


/*-----------------------------------------------------------------------------
 *  State flags
 *---------------------------------------------------------------------------*/

#define AMSCMDSF_REQUEST        0x0000
#define AMSCMDSF_RESPONSE       0x0001
#define AMSCMDSF_ADSCMD         0x0004


/*-----------------------------------------------------------------------------
 * AMS format - offsets of AMS Packet fields 
 *---------------------------------------------------------------------------*/

/* AMS TCP header member offsets*/
#define AMS_OFFS_TCP_HEADER_RESERVED  0
#define AMS_OFFS_TCP_HEADER_LENGTH    sizeof(EC_T_WORD)

#define AMS_OFFS_HEADER               (AMS_OFFS_TCP_HEADER_LENGTH + sizeof(EC_T_DWORD))

/* AMS header member offsets*/

#define AMS_OFFS_TARGET_ID    (AMS_OFFS_HEADER)
#define AMS_OFFS_TARGET_PORT  (AMS_OFFS_TARGET_ID   + EC_T_AOE_NETID_SIZE)
#define AMS_OFFS_SOURCE_ID    (AMS_OFFS_TARGET_PORT + sizeof(EC_T_WORD))
#define AMS_OFFS_SOURCE_PORT  (AMS_OFFS_SOURCE_ID   + EC_T_AOE_NETID_SIZE)

#define AMS_OFFS_COMMAND      (AMS_OFFS_SOURCE_PORT + sizeof(EC_T_WORD))
#define AMS_OFFS_STATE        (AMS_OFFS_COMMAND     + sizeof(EC_T_WORD))
#define AMS_OFFS_DATA_LENGTH  (AMS_OFFS_STATE       + sizeof(EC_T_WORD))
#define AMS_OFFS_ERROR_CODE   (AMS_OFFS_DATA_LENGTH + sizeof(EC_T_DWORD))
#define AMS_OFFS_INVOKE_ID    (AMS_OFFS_ERROR_CODE  + sizeof(EC_T_DWORD))
#define AMS_OFFS_DATA         (AMS_OFFS_INVOKE_ID   + sizeof(EC_T_DWORD))

#define AMS_HEAD_SIZE         (AMS_OFFS_DATA)


/*-----------------------------------------------------------------------------
 * ADS ReadWrite command format - offsets for request and response
 *---------------------------------------------------------------------------*/

/* request field offsets */
#define ADS_RW_REQ_OFFS_INDEXGROUP   0
#define ADS_RW_REQ_OFFS_INDEXOFFSET  (ADS_RW_REQ_OFFS_INDEXGROUP + sizeof(EC_T_DWORD))
#define ADS_RW_REQ_OFFS_READLENGTH   (ADS_RW_REQ_OFFS_INDEXOFFSET + sizeof(EC_T_DWORD))
#define ADS_RW_REQ_OFFS_WRITELENGTH  (ADS_RW_REQ_OFFS_READLENGTH + sizeof(EC_T_DWORD))
#define ADS_RW_REQ_OFFS_DATA         (ADS_RW_REQ_OFFS_WRITELENGTH + sizeof(EC_T_DWORD))
    
/* response format */
#define ADS_RW_RES_OFFS_RESULT 0
#define ADS_RW_RES_OFFS_LENGTH (ADS_RW_RES_OFFS_RESULT + sizeof(EC_T_DWORD))
#define ADS_RW_RES_OFFS_DATA   (ADS_RW_RES_OFFS_LENGTH + sizeof(EC_T_DWORD))


/*-----------------------------------------------------------------------------
 * Offsets and sizes of ADS Ethernet Adapter command requests and responses
 *---------------------------------------------------------------------------*/

/* Connect command request */
#define AEACMD_CONNECT_REQ_MAXSEND_OFFSET  0
#define AEACMD_CONNECT_REQ_MAXRECV_OFFSET  (AEACMD_CONNECT_REQ_MAXSEND_OFFSET + sizeof(EC_T_WORD))
#define AEACMD_CONNECT_REQ_SIZE            (AEACMD_CONNECT_REQ_MAXRECV_OFFSET + sizeof(EC_T_WORD))

/* Connect command response */
#define AEACMD_CONNECT_RES_HANDLE_OFFSET   0
#define AEACMD_CONNECT_RES_MACADDR_OFFSET  (AEACMD_CONNECT_RES_HANDLE_OFFSET + sizeof(EC_T_DWORD))
#define AEACMD_CONNECT_RES_SIZE            (AEACMD_CONNECT_RES_MACADDR_OFFSET + ETHERNET_ADDRESS_LEN)

/* GetFrames command request */
#define AEACMD_GETFRM_REQ_FRAMECOUNT    0
#define AEACMD_GETFRM_REQ_SIZE          sizeof(EC_T_DWORD)
    
/* Connect command response */
#define AEACMD_SENDFRM_RES_LINKERROR_OFFSET    0
#define AEACMD_SENDFRM_RES_FRAMESSEND_OFFSET   (AEACMD_SENDFRM_RES_LINKERROR_OFFSET + sizeof(EC_T_DWORD))
#define AEACMD_SENDFRM_RES_SIZE                (AEACMD_SENDFRM_RES_FRAMESSEND_OFFSET + sizeof(EC_T_DWORD))

/*-----------------------------------------------------------------------------
 * Command IDs of ADS Ethernet Adapter
 *---------------------------------------------------------------------------*/

#define AEA_CONNECT_CMD     0x200
#define AEA_DISCONNECT_CMD  0x201
#define AEA_GET_FRAMES_CMD  0x202
#define AEA_SEND_FRAMES_CMD 0x203



/*-TYPES---------------------------------------------------------------------*/
/*
 * CAmsField class provides methods for accesing ams fields
*/

class CAmsField
{
public:
    static EC_T_WORD GetUint16(EC_T_PBYTE pData, EC_T_DWORD offset)
        { return EC_GET_FRM_WORD(pData + offset);  }
    static EC_T_DWORD GetUint32(EC_T_PBYTE pData, EC_T_DWORD offset)
        { return EC_GET_FRM_DWORD(pData + offset);  }
    static EC_T_VOID SetUint16(EC_T_PBYTE pData, EC_T_DWORD offset, EC_T_WORD value)
        { EC_SET_FRM_WORD(pData + offset, value);  }
    static EC_T_VOID SetUint32(EC_T_PBYTE pData, EC_T_DWORD offset, EC_T_DWORD value)
        { EC_SET_FRM_DWORD(pData + offset, value);  }
};


/*
 * SAEAConnectRequest structure holds parameters of 
 * ADS Ethernet Adapter Connect command request
*/

struct SAEAConnectRequest
{
    EC_T_WORD wMaxSendFrameCnt;
    EC_T_WORD wMaxRecvFrameCnt;
};

/*
 * SAEAConnectResponse structure holds parameters of 
 * ADS Ethernet Adapter Connect command response
*/

struct SAEAConnectResponse
{
    EC_T_DWORD       dwHandle; /* Handle in specification or connection ID */
    ETHERNET_ADDRESS ethAddress;
};

/*
 * SAEAConnectRequest structure holds parameters of 
 * ADS Ethernet Adapter Connect command request
*/

struct SAEAGetFramesRequest
{
    EC_T_DWORD  dwHandle;   /* Handle in specification or connection ID */
    EC_T_DWORD  dwFrameCnt; /* Maximum number of frames which can be read by client */
};

/*
 * SAEASendFramesResponse structure holds parameters of 
 * ADS Ethernet Adapter SendFrames command response
*/

struct SAEASendFramesResponse
{
    EC_T_DWORD dwLinkError;     /* 0 - noError, 1 - error */
    EC_T_DWORD dwFramesSend;    /* since connect */
};




#endif /* INC_ADS_PROTOCOL_H */

