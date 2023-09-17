/*-----------------------------------------------------------------------------
 * AdsAdapter.h  
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Vladimir Pustovalov
 * Description              declaration of functions and classes for ADS adapter
 *---------------------------------------------------------------------------*/

#ifndef INC_ADS_ADAPTER_INTERFACE_H
#define INC_ADS_ADAPTER_INTERFACE_H

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"

/* ADS compiled only if socket supported */
#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
#include "EcError.h"
#include "EcSocket.h"
#include "EcList.h"
#include "AdsProtocol.h"
#include "EcThread.h"

/*- DEFINES ------------------------------------------------------------------*/
#define ADS_CMD_RECV_TIMEOUT                (1000)   /* 1s */
#define ADS_CMD_ACCEPT_TIMEOUT              (50)

/*- STRUCTURES --------------------------------------------------------------*/

/* ADS Adapter parameters */
typedef struct _EC_T_ADS_ADAPTER_PARMS
{
    EC_T_ADS_ADAPTER_START_PARMS*   m_poPtsParams;
    EC_T_LINK_DRV_DESC*             m_pLinkLayer;
} EC_T_ADS_ADAPTER_PARMS;

/*- CLASSES -----------------------------------------------------------------*/

/*
 CAEAGetFramesStorage class provides functionality for storing result of 
 ADS Ethernet Adapter GetFrames command
*/
class CAEAGetFramesStorage
{
public:
    CAEAGetFramesStorage(EC_T_PBYTE buffer, EC_T_DWORD cbBuffer);

    EC_T_VOID StartAdding(EC_T_WORD exactFramesCount);

    /* stops adding new frames to the storage
       returns error code if storage is corrupted
       sets, count of bytes which is used by the storage */
    EC_T_DWORD StopAdding(EC_T_DWORD* pcbFinalSize);

    /* adds new frame to the storage
       returns error if not enough space */
    EC_T_DWORD AddFrame(const EC_T_BYTE* pbyFrame, EC_T_WORD dwSize);

private:
    /* return true if buffer has enough size for this offset */
    EC_INLINESTART EC_T_BOOL isEnoughSpace(EC_T_DWORD offset)
    {
        if ( m_bOutOfRange )
        {
            return EC_FALSE;
        }
        if ( m_cbBuffer < offset )
        {
            m_bOutOfRange = EC_TRUE;
            return EC_FALSE;
        }
        return EC_TRUE;
    } EC_INLINESTOP

    EC_INLINESTART EC_T_VOID safeSetUint16(EC_T_DWORD offset, EC_T_WORD value)
    {
        if (isEnoughSpace(offset + sizeof(EC_T_DWORD)))
        {
            CAmsField::SetUint16(m_buffer, offset, value);
        }
    } EC_INLINESTOP

    EC_INLINESTART EC_T_VOID setFrameLength(EC_T_DWORD frameIndex, EC_T_WORD frameLength)
    {
        EC_T_DWORD offset = 
            sizeof(EC_T_WORD)                   // frame count
            + frameIndex * sizeof(EC_T_WORD);   // frame lengths
        safeSetUint16(offset, frameLength);
    } EC_INLINESTOP

private:
    EC_T_PBYTE m_buffer;
    EC_T_DWORD m_cbBuffer;

    EC_T_WORD m_frameCount;
    
    EC_T_BOOL m_bOutOfRange;

    EC_T_DWORD m_nextFrameIndex;
    EC_T_DWORD m_nextFrameOffset;
};

/*
 CAEASendFramesData class provides functionality for reading data provided by 
 ADS Ethernet Adapter SendFrames command
*/
class CAEASendFramesData
{
public:
    static EC_T_DWORD MinimalSize(EC_T_VOID) { return sizeof(EC_T_WORD); }

    CAEASendFramesData(EC_T_PBYTE buffer, EC_T_DWORD cbBuffer);

    EC_T_WORD GetFrameCount(EC_T_VOID);

    class iterator
    {
    public:
        iterator(EC_T_PBYTE dataPosition, EC_T_PBYTE dataSizesPosition)
            : m_dataPosition(dataPosition), m_dataSizesPosition(dataSizesPosition) {}

        EC_T_PBYTE GetFrameData(EC_T_VOID) { return m_dataPosition; }
        EC_T_WORD GetFrameLength(EC_T_VOID);

        iterator& operator++();
        bool operator!=(const iterator& other) const;
    
    private:
        EC_T_PBYTE m_dataPosition;
        EC_T_PBYTE m_dataSizesPosition;
    };

    iterator begin();
    iterator end();

private:
    EC_T_PBYTE m_buffer;
    EC_T_DWORD m_cbBuffer;
};

/*
 CAEACache class provides functionality for caching frames of 
 ads Ethernet adapter
*/
class CAEACache
{
public:
    CAEACache();
    ~CAEACache();

    EC_T_DWORD AllocateCache(EC_T_WORD maxFrameCount);

    EC_T_VOID AddFrame(const EC_T_BYTE* pbyFrame, EC_T_WORD dwSize);
    
    // returns TRUE if cache has free space for more frames
    EC_T_BOOL HasFreeSpace(EC_T_VOID);

    EC_T_WORD GetFrameCount(EC_T_VOID);

    EC_T_WORD GetFrameSize(EC_T_WORD frameIndex);
    const EC_T_BYTE* GetFrameData(EC_T_WORD frameIndex);

    EC_T_VOID FreeAllFrames(EC_T_VOID);

private:
    struct SFrame
    {
        EC_T_WORD dwSize;
        EC_T_BYTE data[ETHERNET_MAX_FRAME_LEN];
    };

    SFrame* m_aFrame; // dynamically allocated array of frames

    EC_T_WORD m_wMaxFrameCount;
    EC_T_WORD m_wNextFreeFrameIndex;
};

/*
 CAdsEthAdapter class provides functionality for 
*/
class CAdsEthAdapter
{
public:
    CAdsEthAdapter(EC_T_LINK_DRV_DESC* pLinkLayer);
    ~CAdsEthAdapter();
    
    EC_T_DWORD Connect(
        const SAEAConnectRequest& request,
        SAEAConnectResponse* pResponse);
    
    EC_T_DWORD Disconnect(EC_T_DWORD handle);

    EC_T_VOID GetFrames(
        const SAEAGetFramesRequest& request,
        CAEAGetFramesStorage& frames    /* [out]  storage for the frames*/
        );

    EC_T_DWORD SendFrames(EC_T_DWORD handle, CAEASendFramesData& frames, 
        SAEASendFramesResponse* pResponse);

/* methods */
private:
    // reads MAC address from link layer or uses default on error
    EC_T_VOID GetEthernetAddress(EC_T_BYTE* pbyEthernetAddress);

    EC_T_VOID RecvFrames2Cache(EC_T_DWORD nFrameCnt);

    EC_T_VOID CheckForLinkError(EC_T_DWORD* pLinkError);

    EC_T_DWORD SendFrame(EC_T_BYTE* pbyFrame, EC_T_DWORD dwSize);

    EC_T_DWORD LinkIoctl(EC_T_DWORD dwCode, EC_T_LINK_IOCTLPARMS* pParms)
    {
        if ((EC_NULL == m_pLL) || (EC_NULL == m_pLL->pfEcLinkIoctl))
        {
            return EC_E_INVALIDPARM;
        }

        return m_pLL->pfEcLinkIoctl(m_pLL->pvLinkInstance, dwCode, pParms);
    }

/* members */
private:
    EC_T_LINK_DRV_DESC* m_pLL; /* link layer */

    CAEACache m_cacheRx;

    EC_T_DWORD m_totalFramesSend;
    EC_T_BOOL m_bAllocSupported;
    EC_T_BOOL m_bIsFlushFramesRequired;
};

/*
 * CAmsPacket class provides methods for accessing packet header and data
*/
class CAmsPacket
{
public:
    CAmsPacket();
    ~CAmsPacket();

    // returns pointer to the packet buffer
    EC_T_PBYTE InternalBuffer(EC_T_VOID) { return m_buffer; }

    EC_T_DWORD InternalBufferSize(EC_T_VOID) const { return m_cbBuffer; }

    EC_T_BOOL IsCorruptedPacket(EC_T_DWORD dwHeaderLength);
    
    // allocates enough memory for packet buffer
    // returns EC_E_NOMEMORY or EC_E_NOERROR
    EC_T_DWORD ExtendBufferIfNeeded(EC_T_DWORD cbData);

    // get methods
    const EC_T_AOE_NETID& TargetId(EC_T_VOID) const;
    EC_T_WORD TargetPort(EC_T_VOID) const;
    const EC_T_AOE_NETID& SourceId(EC_T_VOID) const;
    EC_T_WORD SourcePort(EC_T_VOID) const;
    EC_T_WORD CommandId(EC_T_VOID) const;
    EC_T_WORD StateFlags(EC_T_VOID) const;
    EC_T_DWORD DataLength(EC_T_VOID) const;
    EC_T_DWORD ErrorCode(EC_T_VOID) const;
    EC_T_DWORD InvokeId(EC_T_VOID) const;
    EC_T_PBYTE GetDataBuffer(EC_T_VOID) const;
    EC_T_DWORD DataBufferSize(EC_T_VOID) const { return InternalBufferSize() - AMS_HEAD_SIZE; }

    // set methods
    EC_T_VOID TargetId(const EC_T_AOE_NETID& value);
    EC_T_VOID TargetPort(EC_T_WORD value);
    EC_T_VOID SourceId(const EC_T_AOE_NETID& value);
    EC_T_VOID SourcePort(EC_T_WORD value);
    EC_T_VOID CommandId(EC_T_WORD value);
    EC_T_VOID StateFlags(EC_T_WORD value);
    EC_T_VOID DataLength(EC_T_DWORD value);
    EC_T_VOID ErrorCode(EC_T_DWORD value);
    EC_T_VOID InvokeId(EC_T_DWORD value);

private:
    EC_INLINESTART EC_T_VOID ensureHeaderAllocated(EC_T_VOID)
	{
		OsDbgAssert(m_buffer != EC_NULL);
		OsDbgAssert(m_cbBuffer >= AMS_HEAD_SIZE);
	} EC_INLINESTOP

    EC_INLINESTART const EC_T_AOE_NETID& getId(EC_T_DWORD offset) const 
	{
		EC_T_BYTE* pValue = m_buffer + offset;
		const EC_T_AOE_NETID& id = (const EC_T_AOE_NETID&)*pValue;
		return id;
	} EC_INLINESTOP

    EC_INLINESTART EC_T_VOID setId(const EC_T_AOE_NETID& value, EC_T_DWORD offset)
	{
		ensureHeaderAllocated();

		const EC_T_BYTE* pValue = value.aby;

		memcpy(m_buffer + offset, pValue, sizeof(value.aby));
	} EC_INLINESTOP

private:
    /* explicitly restrict copy */
    CAmsPacket(const CAmsPacket&); /* DELETE */
    CAmsPacket& operator=(const CAmsPacket&);  /* DELETE */

private:
    EC_T_PBYTE m_buffer;
    EC_T_DWORD m_cbBuffer;
};

/*
 * CAdsReadWriteRequest class provides functionality for 
 * accessing fields of ReadWrite command request
*/
class CAdsReadWriteRequest
{
public:
    static EC_T_BOOL IsValid(const CAmsPacket& request);
    static EC_T_DWORD HeaderSize(EC_T_VOID) { return ADS_RW_REQ_OFFS_DATA; }

public:
    explicit CAdsReadWriteRequest(const CAmsPacket& request);
    
    /* get properties of the header */
    EC_T_DWORD GetIndexGroup(EC_T_VOID) const;
    EC_T_DWORD GetIndexOffset(EC_T_VOID) const;
    EC_T_DWORD GetReadLength(EC_T_VOID) const;
    EC_T_DWORD GetWriteLength(EC_T_VOID) const;
    
    EC_T_PBYTE GetWriteData(EC_T_VOID) const;

    /* set properties of the header */
    EC_T_VOID SetIndexGroup(EC_T_DWORD value);
    EC_T_VOID SetIndexOffset(EC_T_DWORD value);
    EC_T_VOID SetReadLength(EC_T_DWORD value);
    EC_T_VOID SetWriteLength(EC_T_DWORD value);

private:
    /* explicitly restrict copy */
    CAdsReadWriteRequest(const CAdsReadWriteRequest&); /* DELETE */
    CAdsReadWriteRequest& operator=(const CAdsReadWriteRequest&);  /* DELETE */

private:
    const CAmsPacket& m_request;
};

/*
 * CAdsReadWriteResponse class provides functionality for 
 * accessing fields of ReadWrite command response
*/
class CAdsReadWriteResponse
{
public:
    static EC_T_DWORD MinimalSize(EC_T_VOID) { return ADS_RW_RES_OFFS_DATA; }

public:
    /* set properties */
    static EC_T_VOID SetResult(CAmsPacket& packet, EC_T_DWORD value);
    static EC_T_VOID SetLength(CAmsPacket& packet, EC_T_DWORD value);

    /* get properties */
    static EC_T_PBYTE GetData(const CAmsPacket& packet);
};

/*
 CAEASerializer class provides methods for serializing of 
 ADS Ethernet Adapter commands
*/
class CAEASerializer
{
public:
    /* Loads connect command request data from the packet 
     * returns error on wrong format */
    static EC_T_DWORD LoadConnectCmd(
        const CAdsReadWriteRequest& rawRequest, 
        SAEAConnectRequest* pCommandRequest);

    /* Saves connect command response data to the packet 
     * returns error if not enough memory */
    static EC_T_DWORD SaveConnectCmd(
        CAmsPacket& packet,
        const SAEAConnectResponse& response);

    /* Loads connect command request data from the packet 
     * returns error on wrong format */
    static EC_T_DWORD LoadGetFramesCmd(
        const CAdsReadWriteRequest& rawRequest, 
        SAEAGetFramesRequest* pCommandRequest);

    /* Saves connect command response data to the packet 
     * returns error if not enough memory */
    static EC_T_DWORD SaveSendFramesCmd(
        CAmsPacket& packet,
        const SAEASendFramesResponse& response);
};

/*
 CAEAPort class handles ADS Ethernet Adapter commands
*/
class CAEAPort
{
public:
    CAEAPort(CAdsEthAdapter* pAdapter);
    ~CAEAPort();
    
    EC_T_VOID ReadWriteCommand(const CAmsPacket& request, CAmsPacket& response);

private:
    EC_T_VOID FillAdsError(CAmsPacket& response, EC_T_DWORD dwErrCode);

    // separate handlers for each command
    EC_T_DWORD connectCommand(const CAdsReadWriteRequest& request, CAmsPacket& response);
    EC_T_DWORD disconnectCommand(const CAdsReadWriteRequest& request);
    EC_T_DWORD getFramesCommand(const CAdsReadWriteRequest& request, CAmsPacket& response);
    EC_T_DWORD sendFramesCommand(const CAdsReadWriteRequest& request, CAmsPacket& response);

private:
    CAdsEthAdapter* m_pAdapter;
};


//
// CAdsRequestProcessor class process ADS commands
//

class CAdsRequestProcessor
{
public:
    CAdsRequestProcessor(EC_T_LINK_DRV_DESC* pLinkLayer);

    EC_T_VOID Process(const CAmsPacket& request, CAmsPacket& response);

private:
    CAdsEthAdapter m_adapter;
    CAEAPort       m_port;
};


//
// CAmsSrvClient receives AMS requests and sends responses via socket
//

class CAmsSrvClient
{
public:
    CAmsSrvClient(CEcSocket* pSocket, EC_T_LINK_DRV_DESC* pLinkLayer);
    ~CAmsSrvClient();
    
    EC_T_DWORD Start(EC_T_CHAR* szThreadName, EC_T_DWORD dwPrio);
    EC_T_DWORD Stop(EC_T_VOID);
    
    EC_T_BOOL IsStarted(EC_T_VOID) { return m_bIsStarted; }

protected:
    // thead proc called from another thread
    static EC_T_VOID threadProc(EC_T_PVOID pvParams);

    EC_T_VOID destroySocket(EC_T_VOID);

    EC_T_DWORD preallocatePackets(EC_T_VOID);

private:
    EC_T_VOID processRequests(EC_T_VOID);

    EC_T_VOID stopNoWait(EC_T_VOID);

private:
    /* explicitly restrict copy */
    CAmsSrvClient(const CAmsSrvClient&); /* DELETE */
    CAmsSrvClient& operator=(const CAmsSrvClient&);  /* DELETE */

private:
    CEcSocket* m_pSocket;
    CEcThread  m_thread;

    EC_T_BOOL m_bIsStarted;

    CAdsRequestProcessor m_processor;

    // just reuse memory allocated from previous commands
    CAmsPacket m_request;
    CAmsPacket m_response;
};


//
// CAmsConnectionsListener waits for new connection and creates separate client 
// instance for each new connection
//

class CAmsConnectionsListener
{
public:
    CAmsConnectionsListener(const EC_T_ADS_ADAPTER_PARMS& params);
    virtual ~CAmsConnectionsListener();

    EC_T_DWORD Start(CEcSocket* pSocket);

    EC_T_VOID ListenStep(EC_T_VOID);

    EC_T_VOID Stop(EC_T_VOID);

protected:

    // called when new socket connection established
    virtual EC_T_VOID onNewConnection(CEcSocket* pSocket);

    EC_T_DWORD addClient(CAmsSrvClient* pClient);

    EC_T_VOID stopAndFreeClients(EC_T_VOID);

private:
    /* explicitly restrict copy */
    CAmsConnectionsListener(const CAmsConnectionsListener&); /* DELETE */
    CAmsConnectionsListener& operator=(const CAmsConnectionsListener&);  /* DELETE */

private:
    EC_T_LINK_DRV_DESC* m_pLinkLayer;
    EC_T_DWORD          m_dwPtsThreadPriority;
    EC_T_AOE_NETID      m_targetNetID;
    EC_T_WORD           m_targetPort;

    CEcSocket* m_pListenSocket;

    CList<CAmsSrvClient*, CAmsSrvClient*> m_clients;
    EC_T_DWORD m_dwNextThreadID; // use for unique thread name name generation
};


//
// CAmsListener starts/stops CAmsConnectionsListener in separate thread
//

class CAmsListener
{
public:
    CAmsListener(const EC_T_ADS_ADAPTER_PARMS& params);
    virtual ~CAmsListener();

    EC_T_DWORD Start(EC_T_VOID);
    EC_T_DWORD Stop(EC_T_VOID);

protected:
    virtual CEcSocket* createSocket(EC_T_VOID);

    // the function is used as parameter for thread Proc
    // and calls ConnectionsListener Process method
    static EC_T_VOID listenStep(EC_T_PVOID pvParams);

private:
    /* explicitly restrict copy */
    CAmsListener(const CAmsListener&); /* DELETE */
    CAmsListener& operator=(const CAmsListener&);  /* DELETE */

private:
    CAmsConnectionsListener m_connections;
    CEcThread  m_thread;
    EC_T_DWORD m_dwThreadPriority;
};


/*-FUNCTION DECLARATION------------------------------------------------------*/
EC_T_DWORD AdsAdapterStart( EC_T_ADS_ADAPTER_PARMS* pParams, EC_T_PVOID* ppHandle );
EC_T_VOID  AdsAdapterStop(  EC_T_PVOID* ppHandle                            );

#endif /* INCLUDE_ADS_ADAPTER && defined EC_SOCKET_SUPPORTED */
#endif /* INC_ADS_ADAPTER_INTERFACE_H */

/*-END OF SOURCE FILE--------------------------------------------------------*/
