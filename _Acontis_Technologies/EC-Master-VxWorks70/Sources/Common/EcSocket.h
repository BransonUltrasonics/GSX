/*-----------------------------------------------------------------------------
 * EcSocket.h               header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              CMsgQueue, CEcSocket and derivatives declaration
 *---------------------------------------------------------------------------*/

#ifndef ECSOCKET_H
#define ECSOCKET_H    1

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECERROR
#include "EcError.h"
#endif

#ifndef INC_ETHERNETSERVICES
#include "EthernetServices.h"
#endif

#if (defined EC_SOCKET_SUPPORTED)
#ifndef INC_ECTIMER
#include "EcFiFo.h"
#endif

#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif

#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
#  include "msgqueue.h"
#endif /* EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED) || defined(EC_SOCKET_RTOSLIB_SUPPORTED)
#if (defined RTOS_32) && !(defined _RTEOS_H)
  /* Fix Rteos.h defining STRICT and _CRT_SECURE_NO_WARNINGS without check */
#  undef  STRICT
#  undef  _CRT_SECURE_NO_WARNINGS
#endif /* RTOS_32 && !_RTEOS_H */
#  include "rtosLib.h"
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */

/*-DEFINES-------------------------------------------------------------------*/
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
#  ifndef EC_SOCKET_MSGQUEUE_SUPPORTED
#  define EC_SOCKET_MSGQUEUE_SUPPORTED
#  endif
#endif /* EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
#  ifndef EC_SOCKET_MSGQUEUE_SUPPORTED
#  define EC_SOCKET_MSGQUEUE_SUPPORTED
#  endif
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */


#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
#define EC_INVALID_QUEUE_ID                     ((EC_T_DWORD)-1)
#define EC_LOGON_QUEUE_ID                       0
#define EC_MAX_QUEUE_ID                         (EC_INVALID_QUEUE_ID-2)
#define LOGON_QUEUE_NAME                        "RAS_LOGON"
#define DATA_QUEUE_CLIENT_TO_SERVER_NAME_FORMAT "RAS_DATA_%d_CLIENT_TO_SERVER"
#define DATA_QUEUE_SERVER_TO_CLIENT_NAME_FORMAT "RAS_DATA_%d_SERVER_TO_CLIENT"
#define MSGQUEUE_MSG_SIZE                       (10240) /**< max. payload size per queue */
#define MSGQUEUE_MAX_MESSAGES                   40      /**< buffered msg count per queue */

#define EC_SOCKET_MSGQUEUE_SENT_TIMEOUT         (1000)  /* 1s */
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
#define EC_SOCKET_RTOSSHM_API_SYNC_TIMEOUT      (3000)  /* 3s */
#define EC_SOCKET_RTOSSHM_NAME                  "EcSocketMsgQueueRtosShmPort02"
#define EC_SOCKET_RTOSSHM_MAX_CLIENTS           5       /**< max. parallel socket connections */
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */

/*-TYPEDEFS------------------------------------------------------------------*/
typedef enum _ATEMRAS_T_SOCKTYPE
{
    emrassocktype_unknown   = 0,
#if (defined EC_SOCKET_IP_SUPPORTED)
    emrassocktype_tcp       = 1,
    emrassocktype_udp       = 2,
#endif /* EC_SOCKET_IP_SUPPORTED */

#if defined(EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    emrassocktype_msg       = 3,
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */
#if defined(EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
    emrassocktype_rtosshm   = 4,
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */
#if defined(EC_SOCKET_RTOSLIB_SUPPORTED)
    emrassocktype_rtoslib   = 5,
#endif /* EC_SOCKET_RTOSLIB_SUPPORTED */

    /* Borland C++ datatype alignment correction */
    emrassocktype_BCppDummy = 0xFFFFFFFF
} EC_T_SOCKTYPE;

#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
typedef struct _EC_T_MSGQUEUE_MSG
{
    EC_T_DWORD dwLen;                                   /**< message length */
    EC_T_BYTE  abyBuffer[MSGQUEUE_MSG_SIZE];            /**< payload */
} EC_T_MSGQUEUE_MSG, *EC_PT_MSGQUEUE_MSG;
#endif

#if (defined EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
/* Shared Memory format
 +-----------------------------------------+ ---------------------------------- + --------------------+
 + Sign | Thread IDs (DWORD * MAX_CLIENTS) |  Queue allocation                  | 1x                  |
 +-----------------------------------------+ ---------------------------------- + --------------------+
 + WrC  | RdC  | First  |   Last  |  Size  | Writer,Reader Cnt | EC_T_FIFO_DESC | 1                   |
 +-----------------------------------------+                                    | 2                   |
 + Indexes   (DWORD * dwMaxMessages)       +              FiFo                  | 3                   |
 +-----------------------------------------+ ---------------------------------- + ...                 |
 + len (WORD)  | data (dwMaxMessage)       + 1           Buffer                 | 1 + 2 * MAX_CLIENTS |
 + len (WORD)  | data (dwMaxMessage)       + 2                                  |                     |
 + ...         | ...                       + ...                                |                     |
 + len (WORD)  | data (dwMaxMessage)       + dwMaxMessages                      |                     |
 +-----------------------------------------+ ---------------------------------- + --------------------+
*/
typedef union _EC_T_WRITERREADERCOUNT
{
    EC_T_DWORD dwData;
    struct
    {
        EC_T_WORD   wWriterCount;                       /**< connected writers count at queue   */
        EC_T_WORD   wReaderCount;                       /**< connected readers count at queue   */
    } s;
} EC_T_WRITERREADERCOUNT, *EC_PT_WRITERREADERCOUNT;

typedef struct _EC_T_RTOSSHM_QUEUE
{
    EC_T_WRITERREADERCOUNT WriterReaderCount;           /**< connected writers/readers count    */
    EC_T_FIFO_DESC  FifoDesc;                           /**< fifo of indexes inside Message[]   */
    EC_T_DWORD      adwFifoData[MSGQUEUE_MAX_MESSAGES]; /**< indexes inside Message[]           */
    EC_T_MSGQUEUE_MSG Message[MSGQUEUE_MAX_MESSAGES];   /**< payload                            */
} EC_T_RTOSSHM_QUEUE;

#define EC_T_RTOSSHM_SIGNATURE_UNINITIALIZED 0x0
#define EC_T_RTOSSHM_SIGNATURE_INITIALIZING  0x1abbbba1
#define EC_T_RTOSSHM_SIGNATURE_INITIALIZED   0x6affffa6
typedef struct _EC_T_RTOSSHM
{
    EC_T_DWORD dwSignature;     /**< See EC_T_RTOSSHM_SIGNATURE_...      */
    EC_T_RTOSSHM_QUEUE Queue[1 + 2 * EC_SOCKET_RTOSSHM_MAX_CLIENTS]; /**< logon queue + full-duplex sockets */
} EC_T_RTOSSHM;

typedef struct _EC_T_MSGQUEUE_OPTIONS
{
    EC_T_DWORD dwMaxMessages;   /**< buffered msg count */
    EC_T_DWORD cbMaxMessage;    /**< msg size */
    EC_T_BOOL  bReadAccess;     /**< read or write access */
} EC_T_MSGQUEUE_OPTIONS;
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */

/*-CLASS---------------------------------------------------------------------*/
#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
/***************************************************************************************************/
/**
\brief Abstraction to OS API calls like ReadMsgQueue, CreateMsgQueue, etc.
*/
class CMsgQueue
{
public:
    CMsgQueue();
    virtual ~CMsgQueue();

    virtual EC_T_DWORD Open(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId = EC_INVALID_QUEUE_ID);
    virtual EC_T_DWORD Create(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId = EC_INVALID_QUEUE_ID);
    virtual EC_T_BOOL  IsConnected(EC_T_VOID);
    virtual EC_T_DWORD Close(EC_T_VOID);

    virtual EC_T_DWORD Read(EC_T_BYTE* pbyBuffer, EC_T_DWORD* pdwNumberOfBytesRead, EC_T_DWORD dwTimeout);
    virtual EC_T_DWORD Write(EC_T_BYTE* pbyBuffer, EC_T_DWORD dwDataSize, EC_T_DWORD dwTimeout);

    EC_T_DWORD GetId(EC_T_VOID) { return m_dwId; }
    virtual EC_T_DWORD GetMaxMsgSize(EC_T_VOID);
    virtual EC_T_DWORD GetReaderCount(EC_T_DWORD* pdwReaderCount);
    virtual EC_T_DWORD GetWriterCount(EC_T_DWORD* pdwWriterCount);

#if (defined DEBUG)
    EC_T_CHAR  m_szQueueName[255];
#endif

protected:
    EC_T_DWORD m_dwId;      /**< EC_INVALID_QUEUE_ID if not opened or created */
    EC_T_BOOL  m_bOpened;   /**< TRUE if Open(...) was called successfully */
    EC_T_BOOL  m_bCreated;  /**< TRUE if Create(...) was called successfully */

private:
#if (defined EC_SOCKET_MSGQUEUE_WIN32_SUPPORTED)
    MSGQUEUEOPTIONS m_oMsgQueueOptions;
    HANDLE          m_hOsMsgQueue;
#endif

};
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
/***************************************************************************************************/
/**
\brief MsgQueue implementation for ECWIN
*/
class CMsgQueueRtosShm : public CMsgQueue
{
public:
    CMsgQueueRtosShm();
    virtual ~CMsgQueueRtosShm();

    static EC_T_DWORD RtosLibInit(EC_T_VOID);
    static EC_T_DWORD RtosLibDeinit(EC_T_VOID);

    static EC_T_DWORD RtosLibApiSyncEnter(EC_T_DWORD dwTimeout);
    static EC_T_DWORD RtosLibApiSyncExit(EC_T_VOID);

    EC_T_DWORD Open(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId = EC_INVALID_QUEUE_ID);
    EC_T_DWORD Create(EC_T_CHAR* szMsgQueueNameFormat, EC_T_BOOL bReadAccess, EC_T_DWORD dwMaxMessage, EC_T_DWORD dwMaxMessages, EC_T_DWORD dwId = EC_INVALID_QUEUE_ID);
    EC_T_BOOL  IsConnected(EC_T_VOID);
    virtual EC_T_DWORD Close(EC_T_VOID);

    virtual EC_T_DWORD Read(EC_T_BYTE* pbyBuffer, EC_T_DWORD* pdwNumberOfBytesRead, EC_T_DWORD dwTimeout);
    virtual EC_T_DWORD Write(EC_T_BYTE* pbyBuffer, EC_T_DWORD dwDataSize, EC_T_DWORD dwTimeout);

    virtual EC_T_DWORD GetMaxMsgSize(EC_T_VOID) {return m_oMsgQueueOptions.cbMaxMessage; }
    virtual EC_T_DWORD GetReaderCount(EC_T_DWORD* pdwReaderCount);
    virtual EC_T_DWORD GetWriterCount(EC_T_DWORD* pdwWriterCount);

private:
    EC_T_DWORD MapShm(EC_T_VOID);
    EC_T_DWORD CreateEvent(EC_T_DWORD dwId);

    EC_T_MSGQUEUE_OPTIONS   m_oMsgQueueOptions;
    RTOSLIB_HANDLE          m_hDataAvailableEvent;
    CFiFoList<EC_T_DWORD>*  m_poFiFo;

    static EC_T_DWORD       s_dwRtosLibInitCount;
    static HANDLE           s_hRtosLibApiSyncStartStopMutex;

    static EC_T_RTOSSHM*    s_poMap;
    static EC_T_BOOL        s_bShmInitialized; /**< TRUE if Shm was initialized by this process */
};
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */

class CEcSocket
{
public:
    CEcSocket();
    virtual ~CEcSocket() {}

    virtual EC_T_DWORD Bind(EC_T_IPADDR* pAddr, EC_T_WORD wPort) = 0;
    virtual EC_T_DWORD Listen(EC_T_VOID) = 0;
    virtual EC_T_DWORD Accept(CEcSocket** ppoNewSocket, EC_T_DWORD dwTimeout) = 0;
    virtual EC_T_DWORD Connect(EC_T_IPADDR* pAddr, EC_T_WORD wPort) = 0;
    EC_T_DWORD Disconnect(EC_T_BOOL bRespawn);
    EC_T_BOOL  IsDisconnect(EC_T_VOID) {return m_bDisc;} // for compatibility TODO: remove
    virtual EC_T_DWORD Shutdown(EC_T_VOID) = 0;
    virtual EC_T_DWORD Close(EC_T_VOID)    = 0;
    virtual EC_T_DWORD Respawn(EC_T_VOID)  = 0;

    virtual EC_T_DWORD RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* dwLen, EC_T_DWORD dwTimeout) = 0;
    virtual EC_T_DWORD SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen) = 0;

    EC_T_DWORD  GetAddr(EC_T_VOID)  {return m_dwAddr;}
    EC_T_WORD   GetPort(EC_T_VOID)  {return m_wPort;}

protected:
    EC_T_BOOL           m_bConnected;
    EC_T_BOOL           m_bBound;
    EC_T_SOCKTYPE       m_eType;
    EC_T_BOOL           m_bDisc;

    EC_T_DWORD          m_dwAddr;
    EC_T_WORD           m_wPort;
};

#if (defined EC_SOCKET_IP_SUPPORTED)
class CEcSocketIp : public CEcSocket
{
public:
    CEcSocketIp(EC_T_SOCKTYPE   eType);
    CEcSocketIp(EC_T_SOCKTYPE   eType,
        EC_T_SOCKET             pSock,
        EC_T_SOCKADDR_IN        oAddr
    );
    virtual ~CEcSocketIp();

    static EC_T_DWORD InitSocketLayer(EC_T_VOID);
    static EC_T_DWORD DeInitSocketLayer(EC_T_VOID);

    /* CEcSocket interface methods */

    virtual EC_T_DWORD Bind(EC_T_IPADDR* pAddr, EC_T_WORD wPort);
    virtual EC_T_DWORD Listen(EC_T_VOID);
    virtual EC_T_DWORD Accept(CEcSocket** ppoNewSocket, EC_T_DWORD dwTimeout);
    virtual EC_T_DWORD Connect(EC_T_IPADDR* pAddr, EC_T_WORD wPort);
    virtual EC_T_DWORD Shutdown(EC_T_VOID);
    virtual EC_T_DWORD Close(EC_T_VOID);
    virtual EC_T_DWORD Respawn(EC_T_VOID);

    virtual EC_T_DWORD RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* dwLen, EC_T_DWORD dwTimeout);
    virtual EC_T_DWORD SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen);

    /* Ip specific methods */
    virtual EC_T_DWORD RecvData(EC_T_PBYTE pbyData,
        EC_T_DWORD*     dwLen,
        EC_T_DWORD      dwTimeout,
        EC_T_SOCKADDR*  poSrcAddress,
        EC_T_DWORD*     pdwSrcAddressLen=EC_NULL);

    virtual EC_T_DWORD SendData(
        EC_T_PBYTE      pbyData,
        EC_T_DWORD      dwLen,
        EC_T_SOCKADDR*  poDstAddress,
        EC_T_DWORD      dwDstAddressLen=0);

    EC_T_SOCKET GetHandle(EC_T_VOID) { return m_pSockHandle; }

private:
    EC_T_SOCKET         m_pSockHandle;
    EC_T_SOCKADDR_IN    m_oSockAddr;

    static EC_T_DWORD   s_dwInitSocketLayerCount;   /**< How often socket library initialization was successfully called */
};
#endif /* EC_SOCKET_IP_SUPPORTED */

#if (defined EC_SOCKET_MSGQUEUE_SUPPORTED)
class CEcSocketMsgQueue : public CEcSocket
{
public:
    CEcSocketMsgQueue();
    virtual ~CEcSocketMsgQueue();

    EC_T_DWORD Bind(EC_T_IPADDR* pAddr, EC_T_WORD wPort) {EC_UNREFPARM(pAddr); EC_UNREFPARM(wPort); return EC_E_NOERROR;}
    EC_T_DWORD Listen(EC_T_VOID)   {return CreateLogonQueue();}
    EC_T_DWORD Accept(CEcSocket** ppoNewSocket, EC_T_DWORD dwTimeout);
    EC_T_DWORD Connect(EC_T_IPADDR* pAddr, EC_T_WORD wPort);
    EC_T_DWORD Shutdown(EC_T_VOID);
    EC_T_DWORD Close(EC_T_VOID);
    EC_T_DWORD Respawn(EC_T_VOID)  {return EC_E_NOERROR;}

    EC_T_DWORD RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* pdwLen, EC_T_DWORD dwTimeout);
    EC_T_DWORD SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen);

protected:
    virtual EC_T_DWORD CreateCommunicationQueues(EC_T_VOID);
    virtual EC_T_DWORD CreateLogonQueue(EC_T_VOID);
    virtual EC_T_DWORD OpenMsgQueues(EC_T_DWORD dwClientToServerQueueId);
    virtual EC_T_DWORD OpenLogonQueue(EC_T_VOID);

    CMsgQueue* m_poLogonQueue;  /**< client-to-server queue id exchange */
    CMsgQueue* m_poRxMsgQueue;
    CMsgQueue* m_poTxMsgQueue;

private:
    EC_T_MSGQUEUE_MSG  m_RxBuffer; /**< buffer holding the last received msg from queue */
    EC_T_DWORD         m_dwRxLen;  /**< partial read count from m_RxBuffer */
};
#endif /* EC_SOCKET_MSGQUEUE_SUPPORTED */

#if defined(EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED)
class CEcSocketRtosShm : public CEcSocketMsgQueue
{
public:
    CEcSocketRtosShm();
    virtual ~CEcSocketRtosShm();

    EC_T_DWORD CreateCommunicationQueues(EC_T_VOID);
    EC_T_DWORD CreateLogonQueue(EC_T_VOID);
    EC_T_DWORD OpenMsgQueues(EC_T_DWORD dwClientToServerQueueId);
    EC_T_DWORD OpenLogonQueue(EC_T_VOID);
};
#endif /* EC_SOCKET_MSGQUEUE_RTOSSHM_SUPPORTED */

#if defined(EC_SOCKET_RTOSLIB_SUPPORTED)
class CEcSocketRtosLib : public CEcSocket
{
public:
    CEcSocketRtosLib();
    CEcSocketRtosLib( RTOSLIB_HANDLE hNewSocket );
    virtual ~CEcSocketRtosLib();

    static EC_T_DWORD RtosLibInit(EC_T_VOID);
    static EC_T_DWORD RtosLibDeinit(EC_T_VOID);

    EC_T_DWORD Bind(EC_T_IPADDR* pAddr, EC_T_WORD wPort);
    EC_T_DWORD Listen(EC_T_VOID);
    EC_T_DWORD Accept(CEcSocket** ppoNewSocket, EC_T_DWORD dwTimeout);
    EC_T_DWORD Connect(EC_T_IPADDR* pAddr, EC_T_WORD wPort);
    EC_T_DWORD Shutdown(EC_T_VOID);
    EC_T_DWORD Close(EC_T_VOID);
    EC_T_DWORD Respawn(EC_T_VOID);

    EC_T_DWORD RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* pdwLen, EC_T_DWORD dwTimeout);
    EC_T_DWORD RecvData(EC_T_PBYTE pbyData, EC_T_DWORD* pdwLen, EC_T_DWORD dwTimeout, EC_T_SOCKADDR* poSrcAddress, EC_T_DWORD* pdwSrcAddressLen);
    EC_T_DWORD SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen);
    EC_T_DWORD SendData(EC_T_PBYTE pbyData, EC_T_DWORD dwLen, EC_T_SOCKADDR* poDstAddress, EC_T_DWORD dwDstAddressLen);

protected:
    RTOSLIB_HANDLE      m_hRtosSocket;

private:
    static EC_T_INT     s_nRtosLibInitCount;
};
#endif /* EC_SOCKET_RTOSLIB_SUPPORTED */

#endif /* EC_SOCKET_SUPPORTED */
#endif /* ECSOCKET_H */

/*-END OF SOURCE FILE--------------------------------------------------------*/
