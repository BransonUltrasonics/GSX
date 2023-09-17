/*-----------------------------------------------------------------------------
 * AdsAdapter.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Vladimir Pustovalov
 * Description              Ads Adapter implementation
 *---------------------------------------------------------------------------*/

#include "AdsAdapter.h"

#if (defined INCLUDE_ADS_ADAPTER) && (defined EC_SOCKET_IP_SUPPORTED)
/*---------------------------------------------------------------------------*/
/*-CONSTANTS-----------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static const int ADS_TCP_PORT = 48898;

static const EC_T_DWORD STACK_SIZE_DEFAULT = 0x4000;
static const EC_T_DWORD THREADNAME_MAX_LEN = 32;
static const EC_T_CHAR THREADNAME_WORKER[] = "ADS Worker Thread 0x%04lx";
static const EC_T_CHAR THREADNAME_ACCEPTOR[] = "ADS Server Acceptor Thread";

static const EC_T_DWORD HANDLE_HARDCODE = 0x12345678;

static const EC_T_DWORD LINK_CONNECTED = 0;
static const EC_T_DWORD LINK_DISCONNECTED = 1;


/*---------------------------------------------------------------------------*/
/*-CAEAGetFramesStorage class implementation---------------------------------*/
/*---------------------------------------------------------------------------*/

CAEAGetFramesStorage::CAEAGetFramesStorage(EC_T_PBYTE buffer, EC_T_DWORD cbBuffer)
    : m_buffer(buffer)
    , m_cbBuffer(cbBuffer)
    , m_frameCount(0)
    , m_bOutOfRange(EC_FALSE)
    , m_nextFrameIndex(0)
    , m_nextFrameOffset(0)
{
    OsDbgAssert(EC_NULL != m_buffer);
    OsDbgAssert(m_cbBuffer >= sizeof(EC_T_WORD)); // one word at least

    // assume no frames
    StartAdding(0);
}

EC_T_VOID CAEAGetFramesStorage::StartAdding(EC_T_WORD frameCount)
{
    m_frameCount = frameCount;
    
    // set frame count at the header
    CAmsField::SetUint16(m_buffer, 0, frameCount);

    // set frame lengths
    for (EC_T_WORD frameIndex = 0; frameIndex < m_frameCount; frameIndex++)
    {
        setFrameLength(frameIndex, 0);
    }

    m_nextFrameOffset = 
        sizeof(EC_T_WORD)                   // frame count
        + m_frameCount * sizeof(EC_T_WORD); // frame lengths
}

EC_T_DWORD CAEAGetFramesStorage::StopAdding(EC_T_DWORD* pcbFinalSize)
{
    if (m_bOutOfRange)
    {
        return EC_E_INVALIDSIZE;
    }
    OsDbgAssert(EC_NULL != pcbFinalSize);
    
    *pcbFinalSize = m_nextFrameOffset;
    
    return EC_E_NOERROR;
}

EC_T_DWORD CAEAGetFramesStorage::AddFrame(const EC_T_BYTE* pbyFrame, EC_T_WORD dwSize)
{
    if ( !isEnoughSpace(m_nextFrameOffset + dwSize) )
    {
        return EC_E_INVALIDSIZE;
    }
    if ( m_nextFrameIndex >= m_frameCount )
    {
        m_bOutOfRange = EC_TRUE;
        return EC_E_INVALIDSIZE;
    }
    
    EC_T_PBYTE framePlace = m_buffer + m_nextFrameOffset;

    OsMemcpy(framePlace, pbyFrame, dwSize);
    setFrameLength(m_nextFrameIndex, dwSize);

    // move to the next frame
    m_nextFrameOffset += dwSize;
    m_nextFrameIndex++;

    return EC_E_NOERROR;
}

/*---------------------------------------------------------------------------*/
/*-CAEASendFramesData class implementation-----------------------------------*/
/*---------------------------------------------------------------------------*/

EC_T_WORD CAEASendFramesData::iterator::GetFrameLength(EC_T_VOID) 
{ 
    return CAmsField::GetUint16(m_dataSizesPosition, 0); 
}

CAEASendFramesData::iterator& CAEASendFramesData::iterator::operator++()
{
    EC_T_WORD length = GetFrameLength();
    m_dataPosition += length;
    m_dataSizesPosition += sizeof(EC_T_WORD);

    return *this;
}

bool CAEASendFramesData::iterator::operator!=(const CAEASendFramesData::iterator& other) const
{
    return m_dataSizesPosition != other.m_dataSizesPosition;
}


CAEASendFramesData::CAEASendFramesData(EC_T_PBYTE buffer, EC_T_DWORD cbBuffer)
    : m_buffer(buffer)
    , m_cbBuffer(cbBuffer)
{
    OsDbgAssert(EC_NULL != m_buffer);
    OsDbgAssert(m_cbBuffer >= sizeof(EC_T_WORD)); // one word at least
}

EC_T_WORD CAEASendFramesData::GetFrameCount(EC_T_VOID)
{
    return CAmsField::GetUint16(m_buffer, 0);
}

CAEASendFramesData::iterator CAEASendFramesData::begin()
{
    EC_T_PBYTE dataSizesPosition = m_buffer + sizeof(EC_T_WORD);
    EC_T_PBYTE dataPosition = dataSizesPosition + GetFrameCount() * sizeof(EC_T_WORD);

    return iterator(dataPosition, dataSizesPosition);
}

CAEASendFramesData::iterator CAEASendFramesData::end()
{
    EC_T_PBYTE dataSizesPosition = m_buffer + sizeof(EC_T_WORD);
    EC_T_PBYTE justAfterDataSize = dataSizesPosition + GetFrameCount() * sizeof(EC_T_WORD);
    
    // for end iterator, data can be ignored
    return iterator(EC_NULL, justAfterDataSize);
}


/*---------------------------------------------------------------------------*/
/*-CAEACache class implementation---------------------------------------*/
/*---------------------------------------------------------------------------*/

CAEACache::CAEACache()
    : m_aFrame(EC_NULL)
    , m_wMaxFrameCount(0)
    , m_wNextFreeFrameIndex(0)
{
}

CAEACache::~CAEACache()
{
    OsFree(m_aFrame);
}

EC_T_DWORD CAEACache::AllocateCache(EC_T_WORD wMaxFrameCount)
{
    size_t memorySize = wMaxFrameCount * sizeof(SFrame);
    m_aFrame = (SFrame*)OsMalloc(memorySize);
    if ( EC_NULL == m_aFrame )
    {
        return EC_E_NOMEMORY;
    }
    OsMemset((void*)m_aFrame, 0, memorySize);
    
    m_wMaxFrameCount = wMaxFrameCount;
    return EC_E_NOERROR;
}

EC_T_VOID CAEACache::AddFrame(const EC_T_BYTE* pbyFrame, EC_T_WORD dwSize)
{
    OsDbgAssert(EC_NULL != pbyFrame);
    OsDbgAssert(dwSize > 0);
    OsDbgAssert(m_wNextFreeFrameIndex < m_wMaxFrameCount); // check for free space
    
    m_aFrame[m_wNextFreeFrameIndex].dwSize = dwSize;
    OsMemcpy(m_aFrame[m_wNextFreeFrameIndex].data, pbyFrame, dwSize);

    m_wNextFreeFrameIndex++;
}

EC_T_BOOL CAEACache::HasFreeSpace(EC_T_VOID)
{ 
    if ( m_wNextFreeFrameIndex < m_wMaxFrameCount )
        return EC_TRUE; 
    else
        return EC_FALSE;
}

EC_T_WORD CAEACache::GetFrameCount(EC_T_VOID)
{ 
    return m_wNextFreeFrameIndex;
}

EC_T_WORD CAEACache::GetFrameSize(EC_T_WORD frameIndex)
{
    OsDbgAssert(frameIndex < m_wNextFreeFrameIndex);
    return m_aFrame[frameIndex].dwSize;
}

const EC_T_BYTE* CAEACache::GetFrameData(EC_T_WORD frameIndex)
{
    OsDbgAssert(frameIndex < m_wNextFreeFrameIndex);
    return m_aFrame[frameIndex].data;
}

EC_T_VOID CAEACache::FreeAllFrames(EC_T_VOID)
{
    m_wNextFreeFrameIndex = 0;
}


/*---------------------------------------------------------------------------*/
/*-CAdsEthAdapter class implementation---------------------------------------*/
/*---------------------------------------------------------------------------*/

CAdsEthAdapter::CAdsEthAdapter(EC_T_LINK_DRV_DESC* pLinkLayer)
    : m_pLL(pLinkLayer)
    , m_totalFramesSend(0)
    , m_bAllocSupported(EC_TRUE)
{
    EC_T_LINK_IOCTLPARMS oLinkIoCtlParms = {0};

    /* get IsFramesTypeRequired, IsFlushFramesRequired, disregarding return code because IOCTL is optional for link layers */
    OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
    oLinkIoCtlParms.dwOutBufSize = sizeof(EC_T_BOOL);
    oLinkIoCtlParms.pbyInBuf    = (EC_T_PBYTE)&m_bIsFlushFramesRequired;
    LinkIoctl(EC_LINKIOCTL_IS_FLUSHFRAMES_REQUIRED, &oLinkIoCtlParms);
}

CAdsEthAdapter::~CAdsEthAdapter()
{
}

EC_T_DWORD CAdsEthAdapter::Connect(
        const SAEAConnectRequest& request,
        SAEAConnectResponse* pResponse
        )
{
    EC_T_DWORD dwRes = m_cacheRx.AllocateCache(request.wMaxRecvFrameCnt);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }
    
    GetEthernetAddress(pResponse->ethAddress.b);

    pResponse->dwHandle = HANDLE_HARDCODE;

    // reset m_totalFramesSend
    m_totalFramesSend = 0;

    return EC_E_NOERROR;
}

EC_T_DWORD CAdsEthAdapter::Disconnect(EC_T_DWORD handle)
{
    // TODO verify handle
    EC_UNREFPARM(handle);
    
    return EC_E_NOERROR;
}

EC_T_VOID CAdsEthAdapter::GetFrames(
        const SAEAGetFramesRequest& request,    
        CAEAGetFramesStorage& storage    /* [out]  storage for the frames*/
        )
{
    // TODO verify handle

    RecvFrames2Cache(request.dwFrameCnt);

    storage.StartAdding(m_cacheRx.GetFrameCount());

    for (EC_T_WORD i = 0; i < m_cacheRx.GetFrameCount(); i++)
    {
        storage.AddFrame(m_cacheRx.GetFrameData(i), m_cacheRx.GetFrameSize(i));
    }
    m_cacheRx.FreeAllFrames();
}

EC_T_DWORD CAdsEthAdapter::SendFrames(EC_T_DWORD handle, 
                                      CAEASendFramesData& frames, 
                                      SAEASendFramesResponse* pResponse)
{
    /* TODO verify handle */
    EC_UNREFPARM(handle);

    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    CheckForLinkError(&pResponse->dwLinkError);

    CAEASendFramesData::iterator it = frames.begin();
    for ( ; it != frames.end(); ++it )
    {
        EC_T_DWORD dwRes = SendFrame(it.GetFrameData(), it.GetFrameLength());
        if (EC_E_NOERROR != dwRes)
        {
            // save error code to return it to the client
            dwRetVal = dwRes;
            break;
        }

        m_totalFramesSend++;
    }
    pResponse->dwFramesSend = m_totalFramesSend;
    
    if (m_bIsFlushFramesRequired)
    {
        LinkIoctl(EC_LINKIOCTL_FLUSHFRAMES, EC_NULL);
    }

    return dwRetVal;
}

// reads MAC address from link layer or uses default on error
EC_T_VOID CAdsEthAdapter::GetEthernetAddress(EC_T_BYTE* pbyEthernetAddress)
{
    EC_T_DWORD dwRes = m_pLL->pfEcLinkGetEthernetAddress(m_pLL->pvLinkInstance, pbyEthernetAddress);

    // use default on error
    if (EC_E_NOERROR != dwRes)
    {
        OsMemcpy(pbyEthernetAddress, NullEthernetAddress.b, ETHERNET_ADDRESS_LEN);
    }
}

EC_T_VOID CAdsEthAdapter::RecvFrames2Cache(EC_T_DWORD nFrameCnt)
{
    EC_T_LINK_FRAMEDESC frame = {0};
    EC_T_BOOL tryToRecv = EC_TRUE;

    while ( tryToRecv && m_cacheRx.HasFreeSpace() )
    {
        tryToRecv = EC_FALSE;
        m_pLL->pfEcLinkRecvFrame(m_pLL->pvLinkInstance, &frame);

        if ( 0 != frame.dwSize && EC_NULL != frame.pbyFrame )
        {
            if ( frame.dwSize < ETHERNET_MAX_FRAMEBUF_LEN  )
            {
                m_cacheRx.AddFrame(frame.pbyFrame, (EC_T_WORD)frame.dwSize);

                if ( m_cacheRx.HasFreeSpace() && m_cacheRx.GetFrameCount() < nFrameCnt)
                {
                    tryToRecv = EC_TRUE;
                }
            }
        
            m_pLL->pfEcLinkFreeRecvFrame(m_pLL->pvLinkInstance, &frame);
        }
    }
}

EC_T_VOID CAdsEthAdapter::CheckForLinkError(EC_T_DWORD* pLinkError)
{
    *pLinkError = LINK_CONNECTED;

    EC_T_LINKSTATUS status = m_pLL->pfEcLinkGetStatus(m_pLL->pvLinkInstance);
    if ( status == eLinkStatus_DISCONNECTED )
    {
        *pLinkError = LINK_DISCONNECTED;
    }
}

EC_T_DWORD CAdsEthAdapter::SendFrame(EC_T_BYTE* pbyFrame, EC_T_DWORD dwSize)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    EC_T_LINK_FRAMEDESC frame = {0};
    
    if (m_bAllocSupported)
    {
        dwRes = m_pLL->pfEcLinkAllocSendFrame(m_pLL->pvLinkInstance, &frame, dwSize);
        if (dwRes == EC_E_NOTSUPPORTED)
        {
            m_bAllocSupported = EC_FALSE;
        }
    }

    if (m_bAllocSupported)
    {
        // copy frame to the allocated buffer and send
        OsMemcpy(frame.pbyFrame, pbyFrame, dwSize);
        dwRes = m_pLL->pfEcLinkSendAndFreeFrame(m_pLL->pvLinkInstance, &frame);
    }
    else
    {
        frame.pbyFrame = pbyFrame;
        frame.dwSize = dwSize;
        dwRes = m_pLL->pfEcLinkSendFrame(m_pLL->pvLinkInstance, &frame);
    }

    return dwRes;
}

/*---------------------------------------------------------------------------*/
/*-CAmsPacket class implementation-------------------------------------------*/
/*---------------------------------------------------------------------------*/

CAmsPacket::CAmsPacket()
    : m_buffer(EC_NULL)
    , m_cbBuffer(0)
{
}

CAmsPacket::~CAmsPacket()
{
    OsFree(m_buffer);
    //SafeDeleteArray(m_buffer);
}

EC_T_BOOL CAmsPacket::IsCorruptedPacket(EC_T_DWORD dwHeaderLength)
{
    if ( dwHeaderLength < AMS_HEAD_SIZE )
        return EC_TRUE;
    if ( m_cbBuffer < AMS_HEAD_SIZE )
        return EC_TRUE;

    // header was read, check other fields
    EC_T_WORD reserved = CAmsField::GetUint16(m_buffer, AMS_OFFS_TCP_HEADER_RESERVED); 
    if ( 0 != reserved )
        return EC_TRUE;

    // check length from the header
    EC_T_DWORD amsLength = CAmsField::GetUint32(m_buffer, AMS_OFFS_TCP_HEADER_LENGTH); 
    EC_T_DWORD lengthDiff = AMS_HEAD_SIZE - AMS_OFFS_HEADER;
    if ( amsLength - lengthDiff != DataLength() )
        return EC_TRUE;

    return EC_FALSE;
}

EC_T_DWORD CAmsPacket::ExtendBufferIfNeeded(EC_T_DWORD cbData)
{
    EC_T_DWORD cbBuffer = cbData + AMS_HEAD_SIZE;
    if ( cbBuffer > m_cbBuffer )
    {
        EC_T_PBYTE newBuffer = (EC_T_PBYTE)OsMalloc(cbBuffer);
        if ( !newBuffer )
        {
            return EC_E_NOMEMORY;
        }

        // copy old data if needed
        if ( m_buffer != 0 )
        {
            OsMemcpy(newBuffer, m_buffer, m_cbBuffer);
            OsFree(m_buffer);
        }
        else
        {
            // set reserved field in TCP header
            CAmsField::SetUint32(newBuffer, AMS_OFFS_TCP_HEADER_RESERVED, 0);
        }
        
        m_buffer = newBuffer;
        m_cbBuffer = cbBuffer;
    }

    return EC_E_NOERROR;
}

// get methods

const EC_T_AOE_NETID& CAmsPacket::TargetId(EC_T_VOID) const
{
    return getId(AMS_OFFS_TARGET_ID);
}

EC_T_WORD CAmsPacket::TargetPort(EC_T_VOID) const
{
    return CAmsField::GetUint16(m_buffer, AMS_OFFS_TARGET_PORT); 
}

const EC_T_AOE_NETID& CAmsPacket::SourceId(EC_T_VOID) const
{
    return getId(AMS_OFFS_SOURCE_ID);
}

EC_T_WORD CAmsPacket::SourcePort(EC_T_VOID) const
{
    return CAmsField::GetUint16(m_buffer, AMS_OFFS_SOURCE_PORT); 
}

EC_T_WORD CAmsPacket::CommandId(EC_T_VOID)  const
{ 
    return CAmsField::GetUint16(m_buffer, AMS_OFFS_COMMAND); 
}

EC_T_WORD CAmsPacket::StateFlags(EC_T_VOID)  const
{ 
    return CAmsField::GetUint16(m_buffer, AMS_OFFS_STATE); 
}

EC_T_DWORD CAmsPacket::DataLength(EC_T_VOID) const 
{ 
    return CAmsField::GetUint32(m_buffer, AMS_OFFS_DATA_LENGTH); 
}

EC_T_DWORD CAmsPacket::ErrorCode(EC_T_VOID) const 
{ 
    return CAmsField::GetUint32(m_buffer, AMS_OFFS_ERROR_CODE); 
}

EC_T_DWORD CAmsPacket::InvokeId(EC_T_VOID) const 
{ 
    return CAmsField::GetUint32(m_buffer, AMS_OFFS_INVOKE_ID); 
}

EC_T_PBYTE CAmsPacket::GetDataBuffer(EC_T_VOID) const
{
    // if data buffer is called, it meens space should be enough
    OsDbgAssert(m_cbBuffer >= AMS_HEAD_SIZE);
    OsDbgAssert(DataLength() >= 0);

    return m_buffer + AMS_HEAD_SIZE;
}

// set methods

EC_T_VOID CAmsPacket::TargetId(const EC_T_AOE_NETID& value)
{
    setId(value, AMS_OFFS_TARGET_ID);
}

EC_T_VOID CAmsPacket::TargetPort(EC_T_WORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint16(m_buffer, AMS_OFFS_TARGET_PORT, value);
}

EC_T_VOID CAmsPacket::SourceId(const EC_T_AOE_NETID& value)
{
    setId(value, AMS_OFFS_SOURCE_ID);
}

EC_T_VOID CAmsPacket::SourcePort(EC_T_WORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint16(m_buffer, AMS_OFFS_SOURCE_PORT, value);
}

EC_T_VOID CAmsPacket::CommandId(EC_T_WORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint16(m_buffer, AMS_OFFS_COMMAND, value);
}

EC_T_VOID CAmsPacket::StateFlags(EC_T_WORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint16(m_buffer, AMS_OFFS_STATE, value);
}

EC_T_VOID CAmsPacket::DataLength(EC_T_DWORD value)
{
    ensureHeaderAllocated();
    
    // set data field in ams header
    CAmsField::SetUint32(m_buffer, AMS_OFFS_DATA_LENGTH, value);

    // set length field in TCP header
    EC_T_DWORD amsPacketSize = value + AMS_HEAD_SIZE - AMS_OFFS_HEADER;
    CAmsField::SetUint32(m_buffer, AMS_OFFS_TCP_HEADER_LENGTH, amsPacketSize);
}

EC_T_VOID CAmsPacket::ErrorCode(EC_T_DWORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint32(m_buffer, AMS_OFFS_ERROR_CODE, value);
}

EC_T_VOID CAmsPacket::InvokeId(EC_T_DWORD value)
{
    ensureHeaderAllocated();
    
    CAmsField::SetUint32(m_buffer, AMS_OFFS_INVOKE_ID, value);
}

/*---------------------------------------------------------------------------*/
/*-CAdsReadWriteRequest class implementation---------------------------------*/
/*---------------------------------------------------------------------------*/

EC_T_BOOL CAdsReadWriteRequest::IsValid(const CAmsPacket& request)
{
    if ( 0 == request.InternalBufferSize())
        return EC_FALSE;

    EC_T_DWORD bufferSize = request.InternalBufferSize() - AMS_HEAD_SIZE;
    if ( bufferSize < HeaderSize() )
        return EC_FALSE;

    if ( ADSCMDID_READWRITE != request.CommandId() )
        return EC_FALSE;

    if ( request.DataLength() < HeaderSize() )
        return EC_FALSE;

    // verify write length for buffer size and data size
    
    EC_T_DWORD writeLength = CAmsField::GetUint32(request.GetDataBuffer(), ADS_RW_REQ_OFFS_WRITELENGTH);
    if ( writeLength > bufferSize - HeaderSize() )
        return EC_FALSE;

    if ( writeLength > request.DataLength() - HeaderSize() )
        return EC_FALSE;

    return EC_TRUE;
}

CAdsReadWriteRequest::CAdsReadWriteRequest(const CAmsPacket& request)
    : m_request(request)
{
    OsDbgAssert(IsValid(m_request));
}


EC_T_DWORD CAdsReadWriteRequest::GetIndexGroup(EC_T_VOID) const
{
    return CAmsField::GetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_INDEXGROUP);
}

EC_T_DWORD CAdsReadWriteRequest::GetIndexOffset(EC_T_VOID) const
{
    return CAmsField::GetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_INDEXOFFSET);
}

EC_T_DWORD CAdsReadWriteRequest::GetReadLength(EC_T_VOID) const
{
    return CAmsField::GetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_READLENGTH);
}

EC_T_DWORD CAdsReadWriteRequest::GetWriteLength(EC_T_VOID) const
{
    return CAmsField::GetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_WRITELENGTH);
}
    
EC_T_PBYTE CAdsReadWriteRequest::GetWriteData(EC_T_VOID) const
{
    return m_request.GetDataBuffer() + ADS_RW_REQ_OFFS_DATA;
}

EC_T_VOID CAdsReadWriteRequest::SetIndexGroup(EC_T_DWORD value)
{
    OsDbgAssert(m_request.DataLength() >= HeaderSize());

    CAmsField::SetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_INDEXGROUP, value);
}

EC_T_VOID CAdsReadWriteRequest::SetIndexOffset(EC_T_DWORD value)
{
    OsDbgAssert(m_request.DataLength() >= HeaderSize());

    CAmsField::SetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_INDEXOFFSET, value);
}

EC_T_VOID CAdsReadWriteRequest::SetReadLength(EC_T_DWORD value)
{
    OsDbgAssert(m_request.DataLength() >= HeaderSize());

    CAmsField::SetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_READLENGTH, value);
}

EC_T_VOID CAdsReadWriteRequest::SetWriteLength(EC_T_DWORD value)
{
    OsDbgAssert(m_request.DataLength() >= HeaderSize());

    CAmsField::SetUint32(m_request.GetDataBuffer(), ADS_RW_REQ_OFFS_WRITELENGTH, value);
}


/*---------------------------------------------------------------------------*/
/*-CAdsReadWriteResponse class implementation--------------------------------*/
/*---------------------------------------------------------------------------*/

EC_T_VOID CAdsReadWriteResponse::SetResult(CAmsPacket& packet, EC_T_DWORD value)
{
    OsDbgAssert(packet.InternalBufferSize() >= MinimalSize() + AMS_HEAD_SIZE);

    CAmsField::SetUint32(packet.GetDataBuffer(), ADS_RW_RES_OFFS_RESULT, value);
}

EC_T_VOID CAdsReadWriteResponse::SetLength(CAmsPacket& packet, EC_T_DWORD value)
{
    OsDbgAssert(packet.InternalBufferSize() >= MinimalSize() + AMS_HEAD_SIZE);

    CAmsField::SetUint32(packet.GetDataBuffer(), ADS_RW_RES_OFFS_LENGTH, value);
    
    // set data length also
    packet.DataLength(value + MinimalSize());
}

EC_T_PBYTE CAdsReadWriteResponse::GetData(const CAmsPacket& packet)
{
    OsDbgAssert(packet.InternalBufferSize() >= MinimalSize() + AMS_HEAD_SIZE);

    return packet.GetDataBuffer() + ADS_RW_RES_OFFS_DATA;
}

/*---------------------------------------------------------------------------*/
/*- CAEASerializer class implementation -------------------------------------*/
/*---------------------------------------------------------------------------*/

EC_T_DWORD CAEASerializer::LoadConnectCmd(
        const CAdsReadWriteRequest& rawRequest, 
        SAEAConnectRequest* pCommandRequest)
{
    if ( rawRequest.GetWriteLength() < AEACMD_CONNECT_REQ_SIZE )
    {
        return EC_E_INVALIDSIZE;
    }
    
    pCommandRequest->wMaxSendFrameCnt = CAmsField::GetUint16(rawRequest.GetWriteData(), AEACMD_CONNECT_REQ_MAXSEND_OFFSET);
    pCommandRequest->wMaxRecvFrameCnt = CAmsField::GetUint16(rawRequest.GetWriteData(), AEACMD_CONNECT_REQ_MAXRECV_OFFSET);

    return EC_E_NOERROR;
}

EC_T_DWORD CAEASerializer::SaveConnectCmd(
        CAmsPacket& packet,
        const SAEAConnectResponse& response)
{
    EC_T_DWORD dataSize = CAdsReadWriteResponse::MinimalSize() + AEACMD_CONNECT_RES_SIZE;
    
    EC_T_DWORD dwRes = packet.ExtendBufferIfNeeded(dataSize);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }
    
    CAdsReadWriteResponse::SetLength(packet, AEACMD_CONNECT_RES_SIZE);

    // set connection ID (Handle)
    EC_T_PBYTE data = CAdsReadWriteResponse::GetData(packet);
    CAmsField::SetUint32(data, AEACMD_CONNECT_RES_HANDLE_OFFSET, response.dwHandle);

    // set mac address
    EC_T_PBYTE macLocation = data + AEACMD_CONNECT_RES_MACADDR_OFFSET;
    OsMemcpy(macLocation, response.ethAddress.b, ETHERNET_ADDRESS_LEN);

    return EC_E_NOERROR;
}

EC_T_DWORD CAEASerializer::LoadGetFramesCmd(
        const CAdsReadWriteRequest& rawRequest, 
        SAEAGetFramesRequest* pCommandRequest)
{
    if ( rawRequest.GetWriteLength() < AEACMD_GETFRM_REQ_SIZE )
    {
        return EC_E_INVALIDSIZE;
    }
    
    pCommandRequest->dwHandle = rawRequest.GetIndexOffset();
    pCommandRequest->dwFrameCnt = CAmsField::GetUint32(rawRequest.GetWriteData(), AEACMD_GETFRM_REQ_FRAMECOUNT);

    return EC_E_NOERROR;
}

EC_T_DWORD CAEASerializer::SaveSendFramesCmd(
        CAmsPacket& packet,
        const SAEASendFramesResponse& response)
{
    EC_T_DWORD dataSize = CAdsReadWriteResponse::MinimalSize() + AEACMD_SENDFRM_RES_SIZE;
    EC_T_DWORD dwRes = packet.ExtendBufferIfNeeded(dataSize);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }
    
    EC_T_PBYTE pData = CAdsReadWriteResponse::GetData(packet);
    CAmsField::SetUint32(pData, AEACMD_SENDFRM_RES_LINKERROR_OFFSET, response.dwLinkError);
    CAmsField::SetUint32(pData, AEACMD_SENDFRM_RES_FRAMESSEND_OFFSET, response.dwFramesSend);
    
    CAdsReadWriteResponse::SetLength(packet, AEACMD_SENDFRM_RES_SIZE);

    return EC_E_NOERROR;
}

/*---------------------------------------------------------------------------*/
/*- CAEAPort class implementation -------------------------------------------*/
/*---------------------------------------------------------------------------*/

CAEAPort::CAEAPort(CAdsEthAdapter* pAdapter)
    : m_pAdapter(pAdapter)
{
}

CAEAPort::~CAEAPort()
{
}

EC_T_VOID CAEAPort::ReadWriteCommand(const CAmsPacket& request,
                                      CAmsPacket& response)
{
    OsDbgAssert(response.DataBufferSize()>= CAdsReadWriteResponse::MinimalSize());

    if ( !CAdsReadWriteRequest::IsValid(request) )
    {
        response.ErrorCode(ADSERR_DEVICE_INVALID_AMS_LENGTH);
        // NotifyAllClients( ADS_NOTIFY_INVALID_DATA_RECEIVED, 0, &oNotifyParms );

        return ;
    }
    CAdsReadWriteResponse::SetLength(response, 0);
    
    CAdsReadWriteRequest requestRW(request);
    
    EC_T_DWORD dwRes = EC_E_NOERROR;
    
    EC_T_DWORD indexGroup = requestRW.GetIndexGroup();

    switch (indexGroup)
    {
    case AEA_CONNECT_CMD:
        dwRes = connectCommand(requestRW, response);
        break;

    case AEA_DISCONNECT_CMD:
        dwRes = disconnectCommand(requestRW);
        break;

    case AEA_GET_FRAMES_CMD:
        dwRes = getFramesCommand(requestRW, response);
        break;

    case AEA_SEND_FRAMES_CMD:
        dwRes = sendFramesCommand(requestRW, response);
        break;

    default:
        // invalid command recieved
        dwRes = EC_E_AOE_INVALIDGRP;
        break;
    }

    FillAdsError(response, dwRes);
}

EC_T_VOID CAEAPort::FillAdsError(CAmsPacket& response, EC_T_DWORD dwErrCode)
{
    OsDbgAssert(response.DataBufferSize() >= CAdsReadWriteResponse::MinimalSize());

    EC_T_DWORD dwAdsError = ADSERR_NOERROR;
    switch (dwErrCode)
    {
    case EC_E_INVALIDSIZE : 
        dwAdsError = ADSERR_DEVICE_INVALIDSIZE;
        break;

    case EC_E_NOMEMORY : 
        dwAdsError = ADSERR_DEVICE_NO_MEMORY;
        break;

    case EC_E_AOE_INVALIDGRP : 
        dwAdsError = ADSERR_DEVICE_INVALIDGRP;
        break;

    default:
        break;
    }

    response.ErrorCode(dwAdsError);
    CAdsReadWriteResponse::SetResult(response, dwAdsError);
}

EC_T_DWORD CAEAPort::connectCommand(const CAdsReadWriteRequest& request, CAmsPacket& response)
{
    SAEAConnectRequest connectRequest;
    EC_T_DWORD dwRes = CAEASerializer::LoadConnectCmd(request, &connectRequest);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }

    // call connect
    SAEAConnectResponse responseData;
    dwRes = m_pAdapter->Connect(connectRequest, &responseData);

    if ( EC_E_NOERROR == dwRes )
    {
        dwRes = CAEASerializer::SaveConnectCmd(response, responseData);
    }

    return dwRes;
}

EC_T_DWORD CAEAPort::disconnectCommand(const CAdsReadWriteRequest& request)
{
    EC_T_DWORD handle = request.GetIndexOffset();

    return m_pAdapter->Disconnect(handle);
}

EC_T_DWORD CAEAPort::getFramesCommand(const CAdsReadWriteRequest& request, CAmsPacket& response)
{
    SAEAGetFramesRequest getFramesRequest;
    EC_T_DWORD dwRes = CAEASerializer::LoadGetFramesCmd(request, &getFramesRequest);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }

    // prepare response memory
    EC_T_DWORD readSize = request.GetReadLength();
    EC_T_DWORD dataSize = readSize + CAdsReadWriteResponse::MinimalSize();
    dwRes = response.ExtendBufferIfNeeded(dataSize);
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }

    // prepare parameters
    EC_T_PBYTE responseData = CAdsReadWriteResponse::GetData(response);
    CAEAGetFramesStorage storage(responseData, readSize);
    
    m_pAdapter->GetFrames(getFramesRequest, storage);

    // finalize response
    EC_T_DWORD cbFinalSize = 0;
    dwRes = storage.StopAdding(&cbFinalSize);

    CAdsReadWriteResponse::SetLength(response, cbFinalSize);

    return dwRes;
}

EC_T_DWORD CAEAPort::sendFramesCommand(const CAdsReadWriteRequest& request, CAmsPacket& response)
{
    if (request.GetWriteLength() < CAEASendFramesData::MinimalSize())
    {
        return EC_E_INVALIDSIZE;
    }

    // prepare SendFrames parameters
    EC_T_DWORD handle = request.GetIndexOffset();
    CAEASendFramesData frames(request.GetWriteData(), request.GetWriteLength());

    // run SendFrames
    SAEASendFramesResponse cmdResponse;
    EC_T_DWORD dwRes = m_pAdapter->SendFrames(handle, frames, &cmdResponse);
    if ( EC_E_NOERROR != dwRes )
    {
        // TODO add notification that send failed
    }

    return CAEASerializer::SaveSendFramesCmd(response, cmdResponse);
}


/*---------------------------------------------------------------------------*/
/*- CAdsRequestProcessor class implementation -------------------------------*/
/*---------------------------------------------------------------------------*/

CAdsRequestProcessor::CAdsRequestProcessor(EC_T_LINK_DRV_DESC* pLinkLayer)
    : m_adapter(pLinkLayer)
    , m_port(&m_adapter)
{
}

EC_T_VOID CAdsRequestProcessor::Process(const CAmsPacket& request, 
                                        CAmsPacket& response)
{
    // ensure packets are not empty
    OsDbgAssert(EC_NULL != response.InternalBuffer());

    // set default 
    response.StateFlags(AMSCMDSF_RESPONSE|AMSCMDSF_ADSCMD);
    response.DataLength(0);
    
    // copy fields
    response.CommandId(request.CommandId());
    response.InvokeId(request.InvokeId());

    // reverse AMSIds
    response.SourcePort(request.TargetPort());
    response.TargetPort(request.SourcePort());
    response.SourceId(request.TargetId());
    response.TargetId(request.SourceId());

    EC_T_WORD commandID = request.CommandId();
    if ( commandID == ADSCMDID_READWRITE )
    {
        m_port.ReadWriteCommand(request, response);
    }
    else
    {
        response.ErrorCode(ADSERR_DEVICE_UNKNOWN_CMD_ID);
    }
}

/*---------------------------------------------------------------------------*/
/*- helper functions for loading/saving packets from/to Socket --------------*/
/*---------------------------------------------------------------------------*/

/* reads data from sockets and fill it to AMS packet
 * returns error code on error */
EC_T_DWORD ReadFromSocket(CEcSocket* pSocket, CAmsPacket* pPacket)
{
    OsDbgAssert(EC_E_NOERROR == pPacket->ExtendBufferIfNeeded(0));

    /* read header */
    EC_T_DWORD dwLen = AMS_HEAD_SIZE;
    EC_T_DWORD dwRes = pSocket->RecvData(pPacket->InternalBuffer(), &dwLen, ADS_CMD_RECV_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        return dwRes;
    }

    /* check for corrupted header */
    if (pPacket->IsCorruptedPacket(dwLen))
    {
        return EC_E_INVALIDDATA;
    }

    /* read data if needed */
    EC_T_DWORD cbData = pPacket->DataLength();
    if (cbData > 0)
    {
        dwRes = pPacket->ExtendBufferIfNeeded(cbData);
        if (EC_E_NOERROR == dwRes)
        {
            dwLen = cbData;
            dwRes = pSocket->RecvData(pPacket->GetDataBuffer(), &dwLen, ADS_CMD_RECV_TIMEOUT);
        }
    }

    return dwRes;
}

/* writes data to the socket
 * returns error code on error */
EC_T_DWORD WriteToSocket(CEcSocket* pSocket, CAmsPacket* pPacket)
{
    EC_T_DWORD dwPacketSize = AMS_HEAD_SIZE + pPacket->DataLength();
    
    return pSocket->SendData(pPacket->InternalBuffer(), dwPacketSize);
}


/*---------------------------------------------------------------------------*/
/*- CAmsClient class implementation -----------------------------------------*/
/*---------------------------------------------------------------------------*/

CAmsSrvClient::CAmsSrvClient(CEcSocket* pSocket, EC_T_LINK_DRV_DESC* pLinkLayer)
    : m_pSocket(pSocket)
    , m_bIsStarted(EC_FALSE)
    , m_processor(pLinkLayer)
{
}

CAmsSrvClient::~CAmsSrvClient()
{
    /* thread should be stopped, and socket deleted */
    OsDbgAssert(m_pSocket == EC_NULL);
}

EC_T_DWORD CAmsSrvClient::Start(EC_T_CHAR* szThreadName, EC_T_DWORD dwPrio)
{
    if ( EC_NULL == m_pSocket )
    {
        return EC_E_INVALIDPARM;
    }

    EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    EC_T_DWORD dwRes = preallocatePackets();
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRes = m_thread.Start(threadProc, this, szThreadName, dwPrio, STACK_SIZE_DEFAULT, ADS_CMD_RECV_TIMEOUT);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        // start failed, cleanup
        destroySocket();
    }

    return dwRetVal;
}

EC_T_DWORD CAmsSrvClient::Stop(EC_T_VOID)
{
    EC_T_DWORD dwRes = m_thread.Stop(ADS_CMD_RECV_TIMEOUT * 3);

    stopNoWait();

    return dwRes;
}

EC_T_VOID CAmsSrvClient::destroySocket(EC_T_VOID)
{
    SafeDelete(m_pSocket);
}

EC_T_DWORD CAmsSrvClient::preallocatePackets(EC_T_VOID)
{
    EC_T_DWORD dwRes = m_request.ExtendBufferIfNeeded(CAdsReadWriteRequest::HeaderSize());
    if ( EC_E_NOERROR != dwRes )
    {
        return dwRes;
    }
    return m_response.ExtendBufferIfNeeded(CAdsReadWriteResponse::MinimalSize());
}

EC_T_VOID CAmsSrvClient::threadProc(EC_T_PVOID pvParams)
{
    CAmsSrvClient* pThis = (CAmsSrvClient*)pvParams;

    pThis->processRequests();
}

EC_T_VOID CAmsSrvClient::processRequests(EC_T_VOID)
{
    EC_T_DWORD dwRes = ReadFromSocket(m_pSocket, &m_request);;
    if (EC_E_NOERROR == dwRes)
    {
        m_processor.Process(m_request, m_response);

        dwRes = WriteToSocket(m_pSocket, &m_response);
        if (EC_E_NOERROR != dwRes)
        {
            EC_DBGMSG("response error 0x%x\n\n", dwRes);
        }
    }
    else
    {
        if (EC_E_SOCKET_DISCONNECTED == dwRes)
        {
            m_pSocket->Disconnect(EC_FALSE);

// pvv TODO add client aborted unexpectly
            EC_DBGMSG("Client disconnected \n", dwRes);
            stopNoWait();
        }
        else
        // for timeout do nothing
        if (EC_E_TIMEOUT != dwRes)
        {
            EC_DBGMSG("error 0x%x\n", dwRes);
        }

        OsSleep(1);
    }
}

EC_T_VOID CAmsSrvClient::stopNoWait(EC_T_VOID)
{
    m_thread.Stop(EC_NOWAIT);
    
    destroySocket();
    m_bIsStarted = EC_FALSE;
}


/*---------------------------------------------------------------------------*/
/*- CAmsConnectionsListener class implementation-----------------------------*/
/*---------------------------------------------------------------------------*/

CAmsConnectionsListener::CAmsConnectionsListener(const EC_T_ADS_ADAPTER_PARMS& params)
    : m_pLinkLayer(params.m_pLinkLayer)
    , m_dwPtsThreadPriority(params.m_poPtsParams->dwThreadPriority)
    , m_targetNetID(params.m_poPtsParams->targetNetID)
    , m_targetPort(params.m_poPtsParams->targetPort)
    , m_pListenSocket(EC_NULL)
    , m_dwNextThreadID(1)
{
}

CAmsConnectionsListener::~CAmsConnectionsListener()
{
    OsDbgAssert(m_pListenSocket == EC_NULL);
}

EC_T_DWORD CAmsConnectionsListener::Start(CEcSocket* pSocket)
{
    if ( !pSocket )
    {
        return EC_E_NOMEMORY;
    }

    m_pListenSocket = pSocket;
    
    EC_T_IPADDR ipAddr = {0};
    ipAddr.dwAddr = INADDR_ANY;
    
    EC_T_DWORD dwRes = m_pListenSocket->Bind(&ipAddr, ADS_TCP_PORT);
    if ( EC_E_NOERROR == dwRes )
    {
        dwRes = m_pListenSocket->Listen();
    }
    
    // cleanup on error
    if ( EC_E_NOERROR != dwRes )
    {
        SafeDelete(m_pListenSocket);
    }

    return dwRes;
}

EC_T_VOID CAmsConnectionsListener::ListenStep()
{
    CEcSocket* pSpawnSocket = EC_NULL;
    EC_T_DWORD dwRes = m_pListenSocket->Accept(&pSpawnSocket, ADS_CMD_ACCEPT_TIMEOUT);

    if ( EC_E_TIMEOUT == dwRes )
    {
        // do nothing
    }
    else if ( EC_E_NOERROR == dwRes )
    {
        onNewConnection(pSpawnSocket);
    }
    else
    {
        // for any error, just sleep and wait for recover
        OsSleep(ADS_CMD_RECV_TIMEOUT);
    }
}

EC_T_VOID CAmsConnectionsListener::Stop()
{
    stopAndFreeClients();
    
    SafeDelete(m_pListenSocket);
}

// called when new socket connection established 
EC_T_VOID CAmsConnectionsListener::onNewConnection(CEcSocket* pSocket)
{
    CAmsSrvClient* pClient = EC_NEW(CAmsSrvClient(pSocket, m_pLinkLayer));

    if ( pClient )
    {
        // create thread name
        EC_T_CHAR szThreadName[THREADNAME_MAX_LEN];
        OsSnprintf(szThreadName, sizeof(szThreadName) - 1, THREADNAME_WORKER, m_dwNextThreadID++);

        EC_T_DWORD dwRes = pClient->Start(szThreadName, m_dwPtsThreadPriority);
        if ( EC_E_NOERROR == dwRes )
        {
            // add client on successful start only
            addClient(pClient);
        }
        else
        {
            SafeDelete(pClient);
        }
    }
}

EC_T_DWORD CAmsConnectionsListener::addClient(CAmsSrvClient* pClient)
{
    m_clients.AddTail(pClient);
    
    EC_DBGMSG("New connection \n");

    return EC_E_NOERROR;
}

EC_T_VOID CAmsConnectionsListener::stopAndFreeClients()
{
    while (m_clients.GetCount() != 0)
    {
        CAmsSrvClient* pClient = m_clients.RemoveTail();
        if (pClient)
            pClient->Stop();
    
        SafeDelete(pClient);
    }
}

/*---------------------------------------------------------------------------*/
/*-CAmsListener class implementation-----------------------------------------*/
/*---------------------------------------------------------------------------*/

CAmsListener::CAmsListener(const EC_T_ADS_ADAPTER_PARMS& params)
    : m_connections(params)
    , m_dwThreadPriority(params.m_poPtsParams->dwThreadPriority)
{
    CEcSocketIp::InitSocketLayer();
}

CAmsListener::~CAmsListener()
{
    CEcSocketIp::DeInitSocketLayer();
}

EC_T_DWORD CAmsListener::Start(EC_T_VOID)
{
    EC_T_DWORD dwRes = m_connections.Start(createSocket());

    if ( EC_E_NOERROR == dwRes )
    {
        dwRes = m_thread.Start(listenStep, &m_connections,
            (EC_T_CHAR*)THREADNAME_ACCEPTOR, m_dwThreadPriority, STACK_SIZE_DEFAULT, ADS_CMD_RECV_TIMEOUT);

        // cleanup on error
        if ( EC_E_NOERROR != dwRes )
        {
            m_connections.Stop();
        }
    }

    return dwRes;
}

EC_T_DWORD CAmsListener::Stop()
{
    EC_T_DWORD dwRes = m_thread.Stop(ADS_CMD_RECV_TIMEOUT * 3);
    m_connections.Stop();

    return dwRes;
}

CEcSocket* CAmsListener::createSocket()
{
    return EC_NEW(CEcSocketIp(emrassocktype_tcp));
}

EC_T_VOID CAmsListener::listenStep(EC_T_PVOID pvParams)
{
    CAmsConnectionsListener* pListener = (CAmsConnectionsListener*)pvParams;
    pListener->ListenStep();
}


/*---------------------------------------------------------------------------*/
/*-ADS server helper functions-----------------------------------------------*/
/*---------------------------------------------------------------------------*/

EC_T_DWORD CreateAndStartAmsListener(EC_T_ADS_ADAPTER_PARMS* pParams, CAmsListener** ppInstance)
{
    EC_T_DWORD dwRes = EC_E_NOMEMORY;
    
    CAmsListener* pListener = EC_NEW(CAmsListener(*pParams));
    if ( pListener )
    {
        dwRes = pListener->Start();
    }

    // cleanup if fail
    if ( EC_E_NOERROR != dwRes )
    {
        SafeDelete(pListener);
    }

    *ppInstance = pListener;

    return dwRes;
}

EC_T_DWORD AdsAdapterStart(EC_T_ADS_ADAPTER_PARMS* pParams, EC_T_PVOID* ppHandle)
{
    if ( EC_NULL == pParams || EC_NULL == ppHandle )
    {
        return EC_E_INVALIDPARM;
    }
    if ( EC_NULL == pParams->m_poPtsParams || EC_NULL == pParams->m_pLinkLayer )
    {
        return EC_E_INVALIDPARM;
    }
    if (pParams->m_poPtsParams->dwSize < sizeof(EC_T_ADS_ADAPTER_START_PARMS))
    {
        return EC_E_INVALIDPARM;
    }

    CAmsListener* pAmsListener = EC_NULL;
    EC_T_DWORD dwRes = CreateAndStartAmsListener(pParams, &pAmsListener);

    *ppHandle = (EC_T_PVOID)pAmsListener;

    return dwRes;
}

EC_T_VOID AdsAdapterStop(EC_T_PVOID* ppHandle)
{
    if ( EC_NULL == ppHandle || EC_NULL == *ppHandle )
    {
        return;
    }
    
    CAmsListener* pAmsListener = (CAmsListener*)(*ppHandle);
    
    pAmsListener->Stop();
    SafeDelete(pAmsListener);
    
    *ppHandle = pAmsListener;
}
#endif /* INCLUDE_ADS_ADAPTER && defined EC_SOCKET_IP_SUPPORTED */

/*-END OF SOURCE FILE--------------------------------------------------------*/
