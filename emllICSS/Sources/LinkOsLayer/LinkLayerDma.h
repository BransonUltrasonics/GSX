/*-----------------------------------------------------------------------------
 * LinkLayerDma.h           header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Vladimir Pustovalov
 * Description              Link layer DMA class implementation
 *---------------------------------------------------------------------------*/

#ifndef INC_LINK_LAYER_DMA_H
#define INC_LINK_LAYER_DMA_H

/*-INCLUDE-------------------------------------------------------------------*/

#include "LinkOsLayer.h"
#include <EcError.h>

/*-REQUIREPMENTS-------------------------------------------------------------*/

/*
   LinkLayerDma class requires following definitions:

#define LINK_LAYER_PAGE_SIZE
#define LINK_LAYER_DMA_ALIGNMENT
#define LINK_LAYER_RXDMABUFCNT  
#define LINK_LAYER_TXDMABUFCNT
#define LINK_LAYER_RXBUFLEN
#define LINK_LAYER_TXBUFLEN
#define LINK_LAYER_DESCRIPTOR

   LINK_LAYER_DESCRIPTOR is a definition for a class with static functions
   which provides functionality to control DMA descriptors directly
   Use GemDescriptor class as a sample

*/


/*-DEFINES-------------------------------------------------------------------*/
#if (!defined EXCLUDE_LL_DMA)

#define PAGE_UP(addr)         EcSizeAlignUp((addr), LINK_LAYER_PAGE_SIZE)
#define DMA_ALIGN(addr)       EcSizeAlignUp((addr), LINK_LAYER_DMA_ALIGNMENT)

#define NEXT_RX_DESC_IDX(idx) (((idx) + 1) & (Des.RxCount() - 1))
#define NEXT_TX_DESC_IDX(idx) (((idx) + 1) & (Des.TxCount() - 1))

#define NEXT_RX_DMABUF_IDX(idx)  (((idx) + 1) & (LINK_LAYER_RXDMABUFCNT - 1))
#define RX_DMABUF_PTR(idx)  ((T_DMA_FRAMEBUFENTRY *) (this->pRxBuffer + (idx) * LINK_LAYER_RXBUFLEN))

#define NEXT_TX_DMABUF_IDX(idx)  (((idx) + 1) & (LINK_LAYER_TXDMABUFCNT - 1))
#define TX_DMABUF_PTR(idx)  ((T_DMA_FRAMEBUFENTRY *) (this->pTxBuffer + (idx) * LINK_LAYER_TXBUFLEN))

#endif /*#if (!defined EXCLUDE_LL_DMA) */

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/

typedef enum
{
  BS_FREE = 0,
  BS_ALLOC,
  BS_DMA_STARTED,
} T_DMA_BUFSTATE;
  
// Buffer management entry. Size is 16 byte in order to make debugging more comfortable.
typedef struct _T_DMA_BUFENTRY
{
   EC_T_DWORD         dwMagicKey;    // Magic key for sanity checks.
   T_DMA_BUFSTATE     eBufState;
   EC_T_DWORD         dwDescIdx;     // Assigned descriptor index (set during send operation)
   EC_T_DWORD         dwFrameLen;    // Debugging: This info is also stored in the DMA descriptor
} T_DMA_BUFENTRY, *PT_DMA_BUFENTRY;
#define DMA_BUFENTRY_SIZE 16 // Constant expression, because VS preprocessor doesn't understand sizeof()

// Helper access structure (used for casting raw memory)
#if (CACHELINE_SIZE > DMA_BUFENTRY_SIZE)
#  define DMA_BUFENTRY_FRAME_OFFSET CACHELINE_SIZE
#else
#  define DMA_BUFENTRY_FRAME_OFFSET DMA_BUFENTRY_SIZE
#endif

typedef struct _T_DMA_FRAMEBUFENTRY
{
   T_DMA_BUFENTRY bufentry;
#if (DMA_BUFENTRY_FRAME_OFFSET != DMA_BUFENTRY_SIZE)
   EC_T_BYTE       abyCacheAlignmentBuffer[DMA_BUFENTRY_FRAME_OFFSET - DMA_BUFENTRY_SIZE];
#endif
   EC_T_BYTE       pFrame[1];     // Frame buffer
} T_DMA_FRAMEBUFENTRY, *PT_DMA_FRAMEBUFENTRY;


/*-classes-------------------------------------------------------------------*/
#if (!defined EXCLUDE_LL_DMA)

#ifdef UNDER_CE
    typedef DMA_ADAPTER_OBJECT T_DMA_ADAPTER_OBJECT;
#else
    typedef EC_T_DWORD         T_DMA_ADAPTER_OBJECT;
#endif


//
// DMA_OS_PARAMS structure is used for DMA initializing
// We have to do it, because Link OS layer has hard codes to use oAdapterObject internally
//
typedef struct
{
    T_DMA_ADAPTER_OBJECT   oAdapterObject;
    EC_T_DWORD             dwLosalHandle;
} DMA_OS_PARAMS;


//
// LinkLayerDma is responsible of DMA memory access for the link layer
// All layer specific operation is done by LINK_LAYER_DESCRIPTOR class 
//

class LinkLayerDma
{
   /* DMA memory format :
   * |----------------------------------------------|
   * | RX data buffers                              |
   * |                                              |
   * |----------------------------------------------|
   * | TX data buffers                              |
   * |                                              |
   * |----------------------------------------------|
   * | padding to 16 byte boundary                  |
   * |----------------------------------------------|
   * | Descriptor space for RX descriptors          |
   * |                                              |
   * |----------------------------------------------|
   * | Descriptor space for TX descriptors          |
   * |                                              |
   * |----------------------------------------------|
   * | currently unused 4K                          |
   * |----------------------------------------------|
   * | padding to 4K boundary                       |
   * |----------------------------------------------|
   */
public:
   LINK_LAYER_DESCRIPTOR Des; // short name for descriptor info class

public:
   // Initializes DMA memory
   // returns error code
   inline EC_T_DWORD Initialize(DMA_OS_PARAMS* pAdapter, EC_T_DWORD dwRxDesCnt = 0, EC_T_DWORD dwTxDesCnt = 0);
   
   // frees DMA memory
   inline void Free(DMA_OS_PARAMS* pAdapter);

   // fills corresponding DMA descriptor fields as ready for send
   // this descriptor will be freed by hardware by successful send
   inline void SendAndFree(
      EC_T_BYTE*  pbyFrame, /*< frame data buffer */
      EC_T_DWORD  dwSize    /*< size of the frame buffer */
      );

   // looks for free DMA buffer of required size and returns it 
   // returns EC_NULL if no free memory was found
   inline EC_T_BYTE* AllocTxBuffer(EC_T_DWORD dwFrameLen);
   
   // returns true, if memory specified by pBuffer is correct DMA buffer
   inline EC_T_BOOL IsCorrectTxBuffer(EC_T_BYTE* pBuffer);

   inline void FreeTxBuffer(EC_T_BYTE* pData, EC_T_DWORD dwSize);

   // reads current DMA status and returns address of the received data
   // if no data available, returns NULL
   // fills pdwFrameLen with the length of received buffer
   inline EC_T_BYTE* Receive(EC_T_DWORD* pdwFrameLen);

   inline void FreeRxBuffer(EC_T_BYTE* pData, EC_T_DWORD dwSize);

   // returns physical address of receive descriptors
   EC_T_DWORD GetRxDescPa()
   {
      return dma_va2pa((EC_T_DWORD)this->pRxDesc);
   }
   
   // returns physical address of receive descriptors
   EC_T_DWORD GetTxDescPa()
   {
      return dma_va2pa((EC_T_DWORD)this->pTxDesc);
   }

   /* prevent processor to use memory from the cache */
   inline EC_T_VOID CacheInvalidate(volatile EC_T_VOID* pMemory, EC_T_DWORD dwSize)
   {
       EC_UNREFPARM(pMemory); /* some OS have empty cache functions */
       EC_UNREFPARM(dwSize);
       LinkOsCacheDataInvalidateBuff(pMemory, dma_va2pa((EC_T_DWORD)pMemory), dwSize);
   }
   /* flushes cache to the memory */
   inline EC_T_VOID CacheSync(volatile EC_T_VOID* pMemory, EC_T_DWORD dwSize)
   {
       EC_UNREFPARM(pMemory); /* some OS have empty cache functions */
       EC_UNREFPARM(dwSize);
       LinkOsCacheDataSync(pMemory, dma_va2pa((EC_T_DWORD)pMemory), dwSize);
   }

protected:
   inline EC_T_BOOL IsDescOwnedByHardware(EC_T_DWORD dwDescIdx);

   // Calculates total size of DMA memory required.
   // results are stored at this members
   inline void CalculateSize();

   // Allocates DMA memory
   // returns EC_TRUE on successful allocation, EC_FALSE on failure.
   inline EC_T_BOOL DmaAllocate(DMA_OS_PARAMS* pAdapter);

   inline EC_T_BOOL IsDMAFinished(T_DMA_FRAMEBUFENTRY* buffer);

   // Finds free Rx buffer and attaches it to the corresponding DMA descriptor
   inline void AllocRxBuffer(EC_T_DWORD dwRxDescrIdx);

   // frees DMA buffer identified by pData pointer
   // pBufferBase can be this->pRxBuffer or this->pTxBuffer
   // dwDataSize, pBufferBase, dwBufferSize are used for debug purpose only
   inline static void FreeBuffer(EC_T_BYTE* pData, EC_T_DWORD dwDataSize, 
      volatile EC_T_BYTE* pBufferBase, EC_T_DWORD dwBufferSize);

   // Setup and link RX DMA descriptors
   inline void SetupRxDtors();
   inline void SetupBuffers();

private:
   inline EC_T_DWORD dma_va2pa(EC_T_DWORD virtaddr)
   {
      return dwDMAPhysAddr + (virtaddr - dwDMAVirtAddr);
   }

   inline EC_T_DWORD dmadtor_va2pa(EC_T_DWORD virtaddr)
   {
      return dwDmaDtorBasePa + (virtaddr - dwDmaDtorBase);
   }

   inline EC_T_DWORD dma_pa2va(EC_T_DWORD physaddr)
   {
      return dwDMAVirtAddr + (physaddr - dwDMAPhysAddr);
   }

   inline EC_T_DWORD dmadtor_pa2va(EC_T_DWORD physaddr)
   {
      return dwDmaDtorBase + (physaddr - dwDmaDtorBasePa);
   }

protected:

   EC_T_DWORD           dwDmaDtorBase; // descriptors base address
   EC_T_DWORD           dwDmaDtorBasePa;

   EC_T_DWORD           dwDMASize; // size of dynamically allocated DMA buffer
   EC_T_DWORD           dwDMAVirtAddr; // base address of dynamically allocated DMA buffer (cached virt address)
   EC_T_DWORD           dwDMAVirtAddrUncached; // base address of dynamically allocated DMA buffer (uncached virt address)
   EC_T_DWORD           dwDMAPhysAddr; // base address of dynamically allocated DMA buffer (phys address)
   EC_T_DWORD           dwDMAOffset; // offset to aligned address (relative to DMA buffer base address)

   // RX - receive
   volatile EC_T_BYTE*  pRxDesc; // Descriptor base address (len: dwRxDescSize byte)
   EC_T_DWORD           dwRxDescSize; // Size of all RX descriptors (Des.RxDesSize() * Des.RxCount()) in byte
   volatile EC_T_BYTE*  pRxBuffer; // Buffer base address (len: dwRxBufferSize byte)
   EC_T_DWORD           dwRxBufferSize; // Size of all RX buffers (GEM_RXDMABUFCNT * GEM_RXBUFLEN) in byte
   EC_T_DWORD           dwRxDescIdx; // Ring buffer index (RX T_DMA_BUFENTRY)
   EC_T_DWORD           dwRxBufIdx; // Ring buffer index (RX buffers)

   // TX - transmit
   volatile EC_T_BYTE*  pTxDesc;     // TX descriptor base address
   EC_T_DWORD           dwTxDescSize; // Size of all TX descriptors (Des.TxDesSize() * Des.TxCount()) in byte
   volatile EC_T_BYTE*  pTxBuffer; // Buffer base address (len: dwTxBufferSize byte)
   EC_T_DWORD           dwTxBufferSize; // Size of all TX buffers (GEM_TXDMABUFCNT * GEM_TXBUFLEN) in byte
   EC_T_DWORD           dwTxDescIdx; // Next free TX Ring buffer index (TX T_DMA_BUFENTRY)
   EC_T_DWORD           dwTxBufIdx; // Ring buffer index (TX buffers)
};

/******************************************************************************
*                     LinkLayerDma class implementation 
******************************************************************************/

EC_T_DWORD LinkLayerDma::Initialize(DMA_OS_PARAMS* pAdapter, EC_T_DWORD dwTxDesCnt, EC_T_DWORD dwRxDesCnt)
{
   //
   // Setup DMA
   //

   LinkOsMemset(this, 0, sizeof(LinkLayerDma));

   Des.dwTxDesCnt = dwTxDesCnt;
   Des.dwRxDesCnt = dwRxDesCnt;
   CalculateSize();

   // Allocate DMA memory
   if ( !DmaAllocate(pAdapter))
   {
      SYS_ERRORMSGC(("EcLinkOpen(): Error Mapping DMA memory\n"));
      return EC_E_NOMEMORY;
   }    

   DBG("DMA buffer before align: VirtAddr 0x%x, VirtAddrUncached 0x%x, PhysAddr 0x%x\n",
      this->dwDMAVirtAddr, this->dwDMAVirtAddrUncached, this->dwDMAPhysAddr);

   // Make sure that start address is page aligned
   this->dwDMAOffset = PAGE_UP(this->dwDMAVirtAddr) - this->dwDMAVirtAddr;
   this->dwDMAVirtAddr += this->dwDMAOffset;
   this->dwDMAPhysAddr += this->dwDMAOffset;
   this->dwDMASize -= this->dwDMAOffset;

   // Initialize DMA space with zeros
   LinkOsMemset(this->dwDMAVirtAddr, 0, this->dwDMASize);

   // Store address of RX buffers
   this->pRxBuffer = (EC_T_BYTE *) this->dwDMAVirtAddr;

   // Store address of TX buffers
   this->pTxBuffer = (EC_T_BYTE *) this->pRxBuffer + this->dwRxBufferSize;

   EC_T_DWORD dmaDtorBase = DMA_ALIGN(this->dwDMAVirtAddr + this->dwRxBufferSize + this->dwTxBufferSize);
   this->dwDmaDtorBase = dmaDtorBase;
   this->dwDmaDtorBasePa = dma_va2pa(dmaDtorBase);

   // reset descriptors memory
   LinkOsMemset((EC_T_BYTE *)dmaDtorBase, 0, this->dwTxDescSize + this->dwRxDescSize);

   // Store address of RX descriptors
   this->pRxDesc = (volatile EC_T_BYTE *)dmaDtorBase;

   // Store address of TX descriptors
   this->pTxDesc = (volatile EC_T_BYTE *)(dmaDtorBase + this->dwRxDescSize);

   DBG("VirtAddr: DmaDtorBase 0x%08x, DmaBase 0x%08x\n", 
      dmaDtorBase,
      this->dwDMAVirtAddr);
   DBG("PhysAddr: DmaDtorBase 0x%08x, DmaBase 0x%08x\n", 
      this->dwDmaDtorBasePa,
      this->dwDMAPhysAddr);

   SetupBuffers();
   SetupRxDtors();
   Des.TxInitAll(this->pTxDesc, GetTxDescPa());

   Des.ReportFramesInfo(this->pRxDesc, this->pTxDesc);

   return EC_E_NOERROR;
}

void LinkLayerDma::Free(DMA_OS_PARAMS* pAdapter)
{
   if (this->dwDMAVirtAddr != 0)
   {
      this->dwDMASize      += this->dwDMAOffset;
      this->dwDMAVirtAddr  -= this->dwDMAOffset;
      this->dwDMAPhysAddr  -= this->dwDMAOffset;

      LinkOsFreeDmaBuffer(
         &pAdapter->oAdapterObject,
         (EC_T_BYTE *)this->dwDMAPhysAddr,
         (EC_T_BYTE *)this->dwDMAVirtAddr,
         (EC_T_BYTE *)this->dwDMAVirtAddrUncached,
         this->dwDMASize );

      this->dwDMAPhysAddr     = 0;
      this->dwDMAVirtAddr     = 0;
   }
}

// fills corresponding DMA descriptor fields as ready for send
// this descriptor will be freed by hardware by successful send
void LinkLayerDma::SendAndFree(
   EC_T_BYTE*  pbyFrame, /*< frame data buffer */
   EC_T_DWORD  dwSize    /*< size of the frame buffer */
   )
{
   // check buffer is correct
   T_DMA_FRAMEBUFENTRY* pBuffer = (T_DMA_FRAMEBUFENTRY*)(pbyFrame - DMA_BUFENTRY_FRAME_OFFSET);
   LinkOsDbgAssert(pBuffer->bufentry.dwMagicKey == BUFFER_MAGIC_TX);

   // call descriptor->Send
   EC_T_DWORD currentBuffer = (EC_T_DWORD)pbyFrame;
   EC_T_DWORD address = dma_va2pa(currentBuffer);
   Des.Send(this->pTxDesc, this->dwTxDescIdx, address, dwSize);

   /* descriptor information should be flushed to the memory */
   CacheSync(this->pTxDesc + this->dwTxDescIdx * Des.TxDesSize(), Des.TxDesSize());

   // mark buffer as DMA started
   pBuffer->bufentry.eBufState = BS_DMA_STARTED;
   pBuffer->bufentry.dwDescIdx = this->dwTxDescIdx;

   // Increment ring buffer pointer
   this->dwTxDescIdx = NEXT_TX_DESC_IDX(this->dwTxDescIdx);
}

EC_T_BYTE* LinkLayerDma::AllocTxBuffer(EC_T_DWORD dwFrameLen)
{
   EC_T_BYTE* result = EC_NULL;
   
   for (unsigned int i = 0; i < LINK_LAYER_TXDMABUFCNT; i++)
   {
      T_DMA_FRAMEBUFENTRY* buffer = TX_DMABUF_PTR(this->dwTxBufIdx);

      this->dwTxBufIdx = NEXT_TX_DMABUF_IDX(this->dwTxBufIdx);
      if ( buffer->bufentry.eBufState == BS_FREE 
         || IsDMAFinished(buffer) )
      {
         buffer->bufentry.dwFrameLen = dwFrameLen;
         buffer->bufentry.eBufState = BS_ALLOC;

         result = (EC_T_BYTE*)buffer->pFrame;
         break;
      }
   }
   
   return result;
}

EC_T_BOOL LinkLayerDma::IsCorrectTxBuffer(EC_T_BYTE* pBuffer)
{
   volatile EC_T_BYTE* pBufferV = (volatile EC_T_BYTE*)pBuffer;
   // verify address is out of buffer
   if ( pBufferV < this->pTxBuffer || pBufferV >= (this->pTxBuffer + this->dwTxBufferSize) )
   {
	  DBG("Incorrect address 0x%08x from 0x%08x to 0x%08x\n", pBufferV, this->pTxBuffer, this->pTxBuffer + this->dwTxBufferSize);
      return EC_FALSE;
   }

   // verify that buffer address is shifted on size of buffer descriptor
   EC_T_DWORD relativeAddress = (EC_T_DWORD)(pBufferV - this->pTxBuffer);
   if ( relativeAddress % LINK_LAYER_TXBUFLEN != DMA_BUFENTRY_FRAME_OFFSET )
   {
	  DBG("Incorrect shift 0x%08x from 0x%08x\n", pBufferV, this->pTxBuffer);
      return EC_FALSE;
   }

   return EC_TRUE;
}

void LinkLayerDma::FreeTxBuffer(EC_T_BYTE* pData, EC_T_DWORD dwSize)
{
   FreeBuffer(pData, dwSize, this->pTxBuffer, this->dwTxBufferSize);
}

// reads current DMA status and returns address of the received data
// if no data available, returns NULL
// fills pdwFrameLen with the length of received buffer
EC_T_BYTE* LinkLayerDma::Receive(EC_T_DWORD* pdwFrameLen)
{
   CacheInvalidate(this->pRxDesc + this->dwRxDescIdx * Des.RxDesSize(), Des.RxDesSize());
   EC_T_BYTE* pData = NULL;
   EC_T_DWORD framePAddress;
   if ( Des.Receive(this->pRxDesc, this->dwRxDescIdx, &framePAddress, pdwFrameLen) )
   {
      // attach next free buffer to descriptor
      AllocRxBuffer(this->dwRxDescIdx);

      // shift index to the next descriptor
      this->dwRxDescIdx = NEXT_RX_DESC_IDX(this->dwRxDescIdx);

      pData = (EC_T_BYTE*)dma_pa2va(framePAddress);
   }
   
   return pData;
}

void LinkLayerDma::FreeRxBuffer(EC_T_BYTE* pData, EC_T_DWORD dwSize)
{
   FreeBuffer(pData, dwSize, this->pRxBuffer, this->dwRxBufferSize);
}

EC_T_BOOL LinkLayerDma::IsDescOwnedByHardware(EC_T_DWORD dwDescIdx)
{
   CacheInvalidate(this->pTxDesc + dwDescIdx * Des.TxDesSize(), Des.TxDesSize());
   return Des.IsOwnedByHardware(this->pTxDesc, dwDescIdx);
}

/*****************************************************************************/
/** LinkLayerDma protected methods implementation
*/
void LinkLayerDma::CalculateSize()
{
   this->dwTxDescSize = Des.TxDesSize() * Des.TxCount();
   this->dwRxDescSize = Des.RxDesSize() * Des.RxCount();
 

   this->dwTxBufferSize  = LINK_LAYER_TXDMABUFCNT * LINK_LAYER_TXBUFLEN;
   this->dwRxBufferSize  = LINK_LAYER_RXDMABUFCNT * LINK_LAYER_RXBUFLEN;

   this->dwDMASize       = 0;

   // RX Data buffers (each 2k)
   this->dwDMASize    += this->dwRxBufferSize;
   // TX Data buffers (each 2k)
   this->dwDMASize    += this->dwTxBufferSize;

   // Align this space (needed by GEM)
   this->dwDMASize    = DMA_ALIGN(this->dwDMASize);
   // RX Descriptors
   this->dwDMASize    += this->dwRxDescSize;
   // TX Descriptors
   this->dwDMASize    += this->dwTxDescSize;

   // Add one extra page then align to next page boundary
   this->dwDMASize    += LINK_LAYER_PAGE_SIZE;
   this->dwDMASize    = PAGE_UP(this->dwDMASize);
}

EC_T_BOOL LinkLayerDma::DmaAllocate(DMA_OS_PARAMS* pAdapter)
{
   EC_T_BYTE *pVirtAddrCached, *pVirtAddrUncached, *pPhysAddr;
   LinkOsAllocDmaBuffer(
     &pAdapter->oAdapterObject,
     &pVirtAddrCached,
     &pVirtAddrUncached,
     &pPhysAddr,
     this->dwDMASize);

   if (pVirtAddrCached == EC_NULL)
   {
      return EC_FALSE;
   }

   this->dwDMAVirtAddr          = (EC_T_DWORD)pVirtAddrCached;
   this->dwDMAVirtAddrUncached  = (EC_T_DWORD)pVirtAddrUncached;
   this->dwDMAPhysAddr          = (EC_T_DWORD)pPhysAddr;

   return EC_TRUE;
}

EC_T_BOOL LinkLayerDma::IsDMAFinished(T_DMA_FRAMEBUFENTRY* buffer)
{
   if ( buffer->bufentry.eBufState == BS_DMA_STARTED )
   {
      if (!IsDescOwnedByHardware(buffer->bufentry.dwDescIdx))
         return EC_TRUE;
   }
   return EC_FALSE;
}

// Finds free Rx buffer and attaches it to the corresponding DMA descriptor
void LinkLayerDma::AllocRxBuffer(EC_T_DWORD dwRxDescrIdx)
{
   EC_T_BOOL bFoundFreeBuffer = EC_FALSE;
   
   // iterate through all buffers and try to find some free one
   for (EC_T_DWORD i = 0; i < LINK_LAYER_RXDMABUFCNT; i++)
   {
      if ( BS_FREE == RX_DMABUF_PTR(this->dwRxBufIdx)->bufentry.eBufState )
      {
         bFoundFreeBuffer = EC_TRUE;
         break;
      }
      this->dwRxBufIdx = NEXT_RX_DMABUF_IDX(this->dwRxBufIdx);
   }
   if ( bFoundFreeBuffer )
   {
      T_DMA_FRAMEBUFENTRY* pBuffer = RX_DMABUF_PTR(this->dwRxBufIdx);
      pBuffer->bufentry.eBufState = BS_ALLOC;
      pBuffer->bufentry.dwDescIdx = 0; // never use descriptor index for Rx buffers

      EC_T_DWORD paAddress = dma_va2pa((EC_T_DWORD)(EC_T_BYTE*)pBuffer->pFrame);

      Des.RxAttachBuffer(this->pRxDesc, dwRxDescrIdx, paAddress);

      /* descriptor information should be flushed to the memory */
      CacheSync(this->pRxDesc + dwRxDescrIdx * Des.RxDesSize(), Des.RxDesSize());

      this->dwRxBufIdx = NEXT_RX_DMABUF_IDX(this->dwRxBufIdx);
   }
}

//
// frees DMA buffer identified by pData pointer
//
void LinkLayerDma::FreeBuffer(EC_T_BYTE* pData, EC_T_DWORD dwDataSize, 
      volatile EC_T_BYTE* pBufferBase, EC_T_DWORD dwBufferSize)
{
   // it used in debug only
   EC_UNREFPARM(dwDataSize);
   EC_UNREFPARM(pBufferBase);
   EC_UNREFPARM(dwBufferSize);

   T_DMA_FRAMEBUFENTRY* pBuffer = (T_DMA_FRAMEBUFENTRY*)(pData - DMA_BUFENTRY_FRAME_OFFSET);

   LinkOsDbgAssert(pBuffer->bufentry.dwMagicKey == BUFFER_MAGIC_RX || pBuffer->bufentry.dwMagicKey == BUFFER_MAGIC_TX);

   // Check if frame address was allocated from the buffer pool
   LinkOsDbgAssert((pData >= pBufferBase) 
      && ((pData + dwDataSize) <= (pBufferBase + dwBufferSize)));
   
   pBuffer->bufentry.eBufState = BS_FREE;
}

//
// Setup and link RX DMA descriptors
//
void LinkLayerDma::SetupRxDtors()
{
	Des.RxInitAll(this->pRxDesc, GetRxDescPa());
   
   // attach buffer to the each descriptor
   for (EC_T_DWORD i = 0; i < Des.RxCount(); ++i)
   {  
      AllocRxBuffer(i);
   }
}

void LinkLayerDma::SetupBuffers()
{
   T_DMA_FRAMEBUFENTRY *pBufEntry;
   size_t i;

   for (i = 0; i < LINK_LAYER_RXDMABUFCNT; ++i)
   {
      pBufEntry = RX_DMABUF_PTR(i);
      pBufEntry->bufentry.dwMagicKey = BUFFER_MAGIC_RX;
      pBufEntry->bufentry.eBufState = BS_FREE;
   }

   for (i = 0; i < LINK_LAYER_TXDMABUFCNT; ++i)
   {
      pBufEntry = TX_DMABUF_PTR(i);
      pBufEntry->bufentry.dwMagicKey = BUFFER_MAGIC_TX;
      pBufEntry->bufentry.eBufState = BS_FREE;
   }
}

#endif /*#if (!defined EXCLUDE_LL_DMA) */

//
// LinkLayerBuffers is responsible of buffers allocation/deallocation
//

class LinkLayerBuffers
{
public:
    /* memory format :
    * |----------------------------------------------|
    * | buffer used flag                             |
    * |----------------------------------------------|
    * | buffers                                      |
    * |----------------------------------------------|
    */
    // Initializes buffers memory
    // returns error code
    inline EC_T_DWORD Initialize(EC_T_DWORD dwBufferCount, EC_T_DWORD dwBufferSize);

    inline EC_T_BOOL IsInitialized() { return m_pFirstBuffer != EC_NULL; }

    inline EC_T_VOID Deinitialize();

    inline EC_T_DWORD Alloc(EC_T_BYTE** ppbyFrame);

    inline EC_T_VOID FreeBuffer(EC_T_BYTE* pbyFrame);

private:
    inline EC_T_DWORD NextIdx(EC_T_DWORD dwIdx) { return (dwIdx + 1) % m_dwBufferCount; }

private:

    EC_T_DWORD m_dwBufferCount;
    EC_T_DWORD m_dwBufferSize;

    EC_T_DWORD m_dwBufferIdx;  /* index of the next buffer to check */

    EC_T_BOOL* m_pUsedFlag; /* shows that buffer is in use or not */

    EC_T_BYTE* m_pFirstBuffer;
};


/******************************************************************************
*                     LinkLayerBuffers class implementation
******************************************************************************/

inline EC_T_DWORD LinkLayerBuffers::Initialize(EC_T_DWORD dwBufferCount, EC_T_DWORD dwBufferSize)
{
    size_t allocSize = (dwBufferSize + sizeof(EC_T_BOOL)) * dwBufferCount;

    EC_T_BYTE* pMemory = (EC_T_BYTE*)LinkOsMalloc(allocSize);
    if (!pMemory)
        return EC_E_NOMEMORY;

    LinkOsMemset(pMemory, 0, allocSize);

    /* array of used flag is in the beginning */
    m_pUsedFlag = (EC_T_BOOL*)pMemory;

    /* buffers begin after flags */
    m_pFirstBuffer = pMemory + sizeof(EC_T_BOOL) * dwBufferCount;

    m_dwBufferCount = dwBufferCount;
    m_dwBufferSize = dwBufferSize;

    m_dwBufferIdx = 0;

    return EC_E_NOERROR;
}

inline EC_T_VOID LinkLayerBuffers::Deinitialize()
{
    /* array of used flag is in the beginning */
    if (m_pUsedFlag)
        LinkOsFree(m_pUsedFlag);

    m_pUsedFlag = EC_NULL;
    m_pFirstBuffer = EC_NULL;
    m_dwBufferCount = 0;
    m_dwBufferSize = 0;
}

inline EC_T_DWORD LinkLayerBuffers::Alloc(EC_T_BYTE** ppbyFrame)
{
    /* look for free buffer */
    EC_T_DWORD dwIdx;
    for (dwIdx = 0; dwIdx < m_dwBufferCount; dwIdx++)
    {
        if (!m_pUsedFlag[m_dwBufferIdx])
        {
            m_pUsedFlag[m_dwBufferIdx] = EC_TRUE;
            *ppbyFrame = m_pFirstBuffer + m_dwBufferIdx * m_dwBufferSize;
            return EC_E_NOERROR;
        }
        m_dwBufferIdx = NextIdx(m_dwBufferIdx);
    }

    *ppbyFrame = EC_NULL;

    return EC_E_NOMEMORY;
}

inline EC_T_VOID LinkLayerBuffers::FreeBuffer(EC_T_BYTE* pbyFrame)
{
    if (!m_pFirstBuffer)
        return;
    
    /* check address is not out of range */
    LinkOsDbgAssert(pbyFrame >= m_pFirstBuffer);
    LinkOsDbgAssert(pbyFrame < m_pFirstBuffer + m_dwBufferCount * m_dwBufferSize);

    /* calculate buffer index and mark as free */
    EC_T_DWORD dwIdx = (EC_T_DWORD)((pbyFrame - m_pFirstBuffer) / m_dwBufferSize);
    m_pUsedFlag[dwIdx] = EC_FALSE;
}


#endif // INC_LINK_LAYER_DMA_H
