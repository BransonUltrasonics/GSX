/*-----------------------------------------------------------------------------
 * EcEthSwitch.h            header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

#ifndef INC_ECETHSWITCH
#define INC_ECETHSWITCH

/*-INCLUDES------------------------------------------------------------------*/
#include "AtEthercat.h"

#include "EcHashTable.h"

#define PORT_NAME_MAXLEN    30

/*---------------------------------------------------------------------------*/
#if 0
struct IEthernetPort
{	/*  implemented by port and used from outside */
    ~IEthernetPort(){};

    EC_T_DWORD  SwitchFrame(EC_T_BOOL bIsInitCmd, ETHERNET_FRAME* pData, EC_T_DWORD nData, EC_T_DWORD prio, EC_T_DWORD nTimeStamp, EC_T_DWORD nPort)=0;
    EC_T_DWORD  SendFrameComplete(EC_T_BOOL bIsInitCmd, ETHERNET_FRAME* pData)=0;
    EC_T_DWORD  AllocSendBuffer(EC_T_DWORD nData, ETHERNET_FRAME** ppData)=0;
    EC_T_DWORD  FreeSendBuffer(ETHERNET_FRAME* pData)=0;
};
#endif

/*---------------------------------------------------------------------------*/

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef	struct _SWITCH_DATA_BUFFER
{
    EC_T_LISTENTRY  list;
    EC_T_DWORD      dwRefCnt;
    EC_T_DWORD      nData;
    EC_T_DWORD      nTimeStamp;
    EC_T_DWORD      nPort;
    ETHERNET_FRAME	head;
    EC_T_BYTE       byData[ETHERNET_MAX_FRAMEBUF_LEN-ETHERNET_FRAME_LEN];
} SWITCH_DATA_BUFFER, *PSWITCH_DATA_BUFFER;


/*---------------------------------------------------------------------------*/

#if 0
struct IEthernetPortInternal
{	/* implemented by port and used from switch */
    ~IEthernetPortInternal(){};

    EC_T_DWORD  SendFrame(EC_T_BOOL bIsInitCmd, PSWITCH_DATA_BUFFER pDesc, EC_T_DWORD prio)=0;
    EC_T_CHAR*  GetPortName(void)=0;
};
#endif

class CEcEoEUplink;
class CEthernetPort;

typedef	struct _SWITCH_MAC_ENTRY
{
    EC_T_LISTENTRY      list;
    CEthernetPort*      ipPort;
    EC_T_DWORD          ttl;
    ETHERNET_ADDRESS    mac;
} SWITCH_MAC_ENTRY, *PSWITCH_MAC_ENTRY;

typedef	struct	_SWITCH_PORT_ENTRY
{
    CEthernetPort*      ipPort;
} SWITCH_PORT_ENTRY, *PSWITCH_PORT_ENTRY;

/*---------------------------------------------------------------------------*/

#if 0
struct IEthernetSwitch
{   /* implemented by switch and used by port */
    ~IEthernetSwitch(){};

    EC_T_DWORD  PortConnect(CEthernetPort* ipPort)=0;
    EC_T_DWORD  PortDisconnect(CEthernetPort* ipPort)=0;

    EC_T_DWORD  AllocDataDesc(PSWITCH_DATA_BUFFER* ppData)=0;
    EC_T_DWORD  ReturnDataDesc(PSWITCH_DATA_BUFFER pData)=0;
    EC_T_DWORD  SwitchFrame(EC_T_BOOL bIsInitCmd, PSWITCH_DATA_BUFFER pData, CEthernetPort* ipSrcPort, EC_T_DWORD prio)=0;
};
#endif


/*****************************************************************************/
/** \brief Class that implements the virtual Ethernet switch.
*
*/
class CEcEthSwitch /*: public IEthernetSwitch*/
{
public:
    /*************************************************************************/
    /** \brief Constructor
    *
    * \param maxPorts Maximal number of ports that can be connected to the switch.
    * \param maxFrames Maximal number of frames that can be queued.
    * \param maxMACs  Maximal number of frames that are stored by the switch.
    *
    * \return 
    */
    CEcEthSwitch(EC_T_INT maxPorts, EC_T_INT maxFrames, EC_T_INT maxMACs, struct _EC_T_MEMORY_LOGGER* pMLog);
    ~CEcEthSwitch();

    EC_T_DWORD  AllocDataDesc(SWITCH_DATA_BUFFER** ppData);
    EC_T_DWORD  ReturnDataDesc(SWITCH_DATA_BUFFER* pData);
	
    /********************************************************************************/
    /** \brief Switches the Ethernet frame from the source port to the destination port.
    *
    * If the destination port is unknown or the broadcast MAC address is used as destination the frame is
    * sent to all connected ports.
    *
    * \param pData Data buffer containing the Ethernet frame.
    * \param ipSrcPort Pointer to the source Ethernet port.
    * \param prio Priority.
    *
    * \return
    */
    EC_T_DWORD  SwitchFrame(EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pData, CEthernetPort* ipSrcPort, EC_T_DWORD prio );

    /********************************************************************************/
    /** \brief Sends an Ethernet frame to the destination port.
    *
    * \param pData Data buffer containing the frame to be sent.
    * \param ipDestPort Port the frame should be sent to.
    * \param priority Priority.
    *
    * \return Result of the call.
    */
    EC_T_DWORD  SendFrameToPort(EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pData, CEthernetPort* ipDestPort, EC_T_DWORD prio);

    /********************************************************************************/
    /** \brief Connects an Ethernet port to the switch
    *
    * \param ipPort Pointer to the CEthernetPort interface of the port to be connected.
    *
    * \return Result of the call.
    */
    EC_T_DWORD  PortConnect(CEthernetPort* ipPort);

    /********************************************************************************/
    /** \brief Disconnects an Ethernet port from the switch.
    *
    * \param ipPort Pointer to the CEthernetPort interface of the port to be disconnected.
    *
    * \return Result of the call.	
    */
    EC_T_DWORD  PortDisconnect(CEthernetPort* ipPort);

    EC_T_DWORD  FlushMacMap() {if (EC_NULL != m_pMacMap) m_pMacMap->RemoveAll(); return EC_E_NOERROR;}

    struct _EC_T_MEMORY_LOGGER* GetMemLog() {return m_pMLog;}

    static CEcEthSwitch* GetInstance(EC_T_DWORD dwInstanceId);
    static EC_T_VOID ReleaseInstance(CEcEthSwitch* pSwitch);

protected:
    EC_T_LISTENTRY                                      m_listFreeData;
    CHashTableDyn<ETHERNET_ADDRESS, SWITCH_MAC_ENTRY>*  m_pMacMap;
    EC_T_LISTENTRY                                      m_listTtlMac;

    CHashTableDyn<EC_T_UINT64, SWITCH_PORT_ENTRY>*	    m_pPortMap;

    EC_T_VOID*                                          m_poFreeDataLock;

private:
    PSWITCH_DATA_BUFFER                                 m_pDataBuffer;
    EC_T_INT                                            m_dwMaxFrames;
    EC_T_DWORD                                          m_currTTL;
    struct _EC_T_MEMORY_LOGGER*                         m_pMLog;
};

/*---------------------------------------------------------------------------*/

typedef EC_T_DWORD	(*PSendFrameCallback)(EC_T_BOOL bIsInitCmd, EC_T_VOID* pContext, ETHERNET_FRAME* pData, EC_T_DWORD nData);

#define ETHERNET_SWITCH_HIGH_PRIO       4


/********************************************************************************/
/** \brief Class that implements the virtual Ethernet port.
*
* \return 
*/
class CEthernetPort 
{
public:
    /********************************************************************************/
    /** \brief Constructor
    *
    * \param ipSwitch Ethernet switch the port should be connected to.
    * \param pCallback Pointer to a callback function that should be called for sending a frame.
    * \param pContext Pointer to the context of the callback.
    *
    * \return 
    */
    CEthernetPort(EC_T_DWORD dwInstanceId, PSendFrameCallback pCallback, EC_T_VOID* pContext, EC_T_CHAR* szPortName);
    ~CEthernetPort();

    /********************************************************************************/
    /** \brief Checks if port is connected to switch
     *
     * \return TRUE if connected to Switch
     */
    EC_T_BOOL  IsConnected(EC_T_VOID) { return m_bConnected; }

    /********************************************************************************/
    /** \brief Disconnects port from switch
     */
 	EC_T_VOID  Disconnect(EC_T_VOID);

    /* IEthernetPort */

    /********************************************************************************/
    /** \brief Switches an Ethernet frame.
    *
    * \param pData Ethernet Frame to be switched.
    * \param nData Size of the Ethernet frame.
    * \param priority Priority.
    *
    * \return 
    */
    EC_T_DWORD  SwitchFrame(EC_T_BOOL bIsInitCmd, ETHERNET_FRAME* pData, EC_T_DWORD nData, EC_T_DWORD prio, EC_T_DWORD nTimeStamp = 0, EC_T_DWORD nPort = 0);
	
    /********************************************************************************/
    /** \brief Called after a previously sent frame has been processed.
    *
    * \param pData Ethernet frame that is completed.
    *
    * \return 
    */
    EC_T_DWORD  SendFrameComplete(EC_T_BOOL bIsInitCmd, ETHERNET_FRAME* pData);

    /********************************************************************************/
    /** \brief Allocates send buffer
    *
    * \return 
    */
    EC_T_DWORD  AllocSendBuffer(EC_T_DWORD nData, ETHERNET_FRAME** ppData);
	
    /********************************************************************************/
    /** \brief Frees previously allocated buffer.
    *
    * \return 
    */
    EC_T_DWORD  FreeSendBuffer(ETHERNET_FRAME* pData);

    /********************************************************************************/
    /** \briefCalled by the switch to send the frame.
    *
    * \return 
    */
    EC_T_DWORD  SendFrame(EC_T_BOOL bIsInitCmd, PSWITCH_DATA_BUFFER pDesc, EC_T_DWORD prio);

    SWITCH_DATA_BUFFER* GetQueuedSendFrame(EC_T_DWORD* pdwPrio);
    SWITCH_DATA_BUFFER* GetQueuedSendFrameNoLock(EC_T_DWORD* pdwPrio);

    EC_T_CHAR*  GetPortName(void)
                            { return m_szPortName; }

    EC_T_VOID   LockPendingReceive(EC_T_VOID)
          { OsLock(m_poPendingReceiveLock); }
    EC_T_VOID   UnlockPendingReceive(EC_T_VOID)
          { OsUnlock(m_poPendingReceiveLock); }

protected:
    EC_T_DWORD          m_dwInstanceId;
    EC_T_LISTENTRY      m_listSendFifoL;
    EC_T_LISTENTRY      m_listSendFifoH;
    EC_T_VOID*          m_poSendFifoLock;
    EC_T_VOID*          m_poPendingReceiveLock;
    PSendFrameCallback  m_pCallback;
    EC_T_VOID*          m_pContext;
    EC_T_CHAR           m_szPortName[PORT_NAME_MAXLEN];

public:
    /*! \var    m_pPendingReceive
        \brief  Reference to a frame which was sent via the ports callback
                function ``m_pCallback`` with a ``EC_S_PENDING`` return.
	*/
    PSWITCH_DATA_BUFFER m_pPendingReceive; /* TODO can be removed? -> EoE Endpoint: EC_T_LINK_FRAMEDESC::pvContext */

protected:
    /*! \var    m_pPendingSend
        \brief  Reference to a frame which was allocated but not yet sent.
                Set to ``NULL`` when there is no allocated frame.
	*/
    PSWITCH_DATA_BUFFER m_pPendingSend;

    EC_T_BOOL           m_bConnected;

private:
    EC_T_BOOL           m_bSendFrameLocked;
};



#if (defined INCLUDE_EOE_ENDPOINT)

/*****************************************************************************/
/** \brief EthernetPort for the EoE-Uplink
*
*          The EthernetPort for the EoE-Uplink overrides the SendFrame-Method.
*          The new SendFrame-Method forward Ethernet packets only if an 
*          EoE-Driver has registered on the EoE-Uplink Port.
*          
*          With this implementation we avoid that the EthernetPort of the
*          EoEUplink absorbs all Ethernet-packet descriptors.          
*/
class CEoEUplinkEthernetPort : public CEthernetPort
{
private: 
    CEoEUplinkEthernetPort(const CEoEUplinkEthernetPort& port);

public:
     CEoEUplinkEthernetPort(EC_T_DWORD dwInstanceId, PSendFrameCallback pCallback, CEcEoEUplink* pContext, EC_T_CHAR* szPortName);
     EC_T_DWORD SendFrame(EC_T_BOOL bIsInitCmd, SWITCH_DATA_BUFFER* pDesc, EC_T_DWORD prio);

     CEcEoEUplink* m_pEoEUplink;
};


/*****************************************************************************/
/** \brief Definition of the EoE forwarding device.
*
*		   This EoE module is the mediator between a standard Ethernet port
*          (implemented with ``CEthernetPort``) and the outworlds TCP/IP/ETH
*          protocol stack. Think of it as the implementation of the uplink port
*          for our Ethernet switch.
*
*          The class implements all the functions as required by the Acontis
*          ECAT link layer API. Given this, the ``CEcEoEUplink`` functions as
*          the lower side of a link layer and can be seen/accessed as an
*          Ethernet capable network driver.
*
*          The ``CEcEoEUplink`` class as a link layer implements frame
*          allocation. The user must allocate frames with the
*          ``EoELinkAllocSendFrame`` function and is then responsible for this
*          frame buffer. The user can free these frames with the
*          ``EoELinkSendAndFreeFrame()`` or the ``EoELinkFreeSendFrame()``
*          function.
*/
class CEcEoEUplink
{
public:

    /*************************************************************************/
    /** \brief Constructor
    *
    *          During construction we create an Ethernet port ``CEthernetPort``
    *          and immediately connect it to our parent Ethernet switch.
    *
    * \param  [in]  pVirtualEthernetSwitch  Reference to our parent switch.
    */
    CEcEoEUplink(EC_T_DWORD dwInstanceId, CEcEthSwitch* pVirtualEthernetSwitch);
    ~CEcEoEUplink();

    /*************************************************************************/
    /** \brief Register the driver/adapter to the OS
    *
    *          This function can be called from the application at a suitable
    *          time during startup or it can be called implicitly by the EtherCAT
    *          master during startup.
    *
    * \param  [in]  dwInstanceID       Instance identifier; the same as the
    *                                  master Stacks Identifier in the
    *                                  ``S_aMultiMaster[]`` array.
    * \param  [in]  pvSwitch           Pointer to the instance of a ``CEcEthSwitch``
    *                                  object.
    * \param  [in]  szEoEDrvIdent      Pointer to a string giving the name of the
    *                                  driver/adapter we shall represent.
    * \param  [in]  pLinkDrvDesc       Pointer to a structure with information about
    *                                  exported driver/adapter functions. The function
    *                                  pointers are filled by this function.
    */
    static EC_T_DWORD EoELinkRegister(EC_T_DWORD dwInstanceId, CEcEthSwitch* pSwitch, EC_T_CHAR* szEoEDrvIdent, EC_T_LINK_DRV_DESC* pLinkDrvDesc);
    static EC_T_DWORD EoELinkUnregister(EC_T_LINK_DRV_DESC* pLinkDrvDesc);

    /*************************************************************************/
    /** \brief Open this Ethernet uplink port for the outside world for use as
    *          a network driver (compare ``driver open function``).
    *
    * \param  [in]  pvLinkParms   Pointer to a structure with information about
    *                             the properties of our uplink.
    *
    * \param  [in]  pfReceiveFrameCallback  Pointer the the outside worlds
    *                             function where to put packets/message frames
    *                             destined to the outside world.
    * \param  [in]  pfLinkNotifyCallback    Pointer to the outside worlds
    *                             function where to signal a newly received
    *                             packet/message frame destined to the outside
    *                             world. Implemented and used only in case of
    *                             NON polling mode. (unused)
    * \param  [in]  pvContext     Pointer to the object which is the owner of
    *                             the above mentioned callback functions.
    * \param  [out] ppvInstance   Pointer to this ``CEcEoEUplink`` instance.
	*
    * \return EC_E_NOERROR or error code.
    */
    static EC_T_DWORD EoELinkOpen(EC_T_VOID* pvLinkParms,
                                  EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback,
                                  EC_T_LINK_NOTIFY pfLinkNotifyCallback,
                                  EC_T_VOID* pvContext,
                                  EC_T_VOID** ppvInstance);


    /*************************************************************************/
    /** \brief Close the connection between our Ethernet port and it's parent
	*          switch.
    *
    * \param  [in]  pvInstance   Pointer to this ``CEcEoEUplink`` instance.
	*
    * \return EC_E_NOERROR or error code.
    */
    static EC_T_DWORD EoELinkClose(EC_T_VOID* pvInstance);


    /*************************************************************************/
    /** \brief Send a frame to the Ethernet switch.
    *
	*         This function clearly is called from the outside world (the
    *         TCP/IP/ETH protocol stack respectively). The intention is to
    *         send a standard Ethernet packet via EoE out the EtherCAT network
    *         interface.
    *
    * \param  [in]  pvInstance      Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in]  pLinkFrameDesc  Pointer to a descriptor identifying the
    *                               packet which shall be sent.
	*
    * \return EC_E_NOERROR or error code.
    */
    static EC_T_DWORD EoELinkSendFrame(EC_T_VOID* pvInstance,
                                       EC_T_LINK_FRAMEDESC* pLinkFrameDesc);

   
    /*************************************************************************/
    /** \brief Send a frame to the Ethernet switch and free the no longer used
    *          frame buffer afterwards.
    *
	*         This function clearly is called from the outside world (the
    *         TCP/IP/ETH protocol stack respectively). The intention is to
    *         send a standard Ethernet packet via EoE out the EtherCAT network
    *         interface.
    *
    * \param  [in]  pvInstance      Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in]  pLinkFrameDesc  Pointer to a descriptor identifying the
    *                               packet which shall be sent.
	*
    * \return EC_E_NOERROR or error code.
    */
    static EC_T_DWORD EoELinkSendAndFreeFrame(EC_T_VOID* pvInstance,
                                              EC_T_LINK_FRAMEDESC* pLinkFrameDesc);


    /*************************************************************************/
    /** \brief Poll for newly received frames.
    *
    *        This function clearly is called from the outside world (the
	*        TCP/IP/ETH protocol stack respectively). The intention is to
    *        check for Ethernet frames which came in via EoE on the EtherCAT
    *        network interface.
    *
    *        The function is used only when the ``CEcEoEUplink`` runs in
    *        polling mode. The active mode can be checked by calling the
    *        ``EcLinkGetMode()`` function.
    *
    *        Upon a ``EC_E_NOERROR`` the caller can test the ``dwSize`` member
    *        of the ``pLinkFrameDesc`` function parameter. When != 0, a newly
    *        received message frame was copied to the buffer.
    *
	* \param  [in]  pvInstance      Pointer to a ``CEcEoEUplink`` instance.
    * \param  [out] pLinkFrameDesc  Pointer to a descriptor identifying an
    *                               empty frame storage where to copy the
    *                               received frame.
    *
    * \return EC_E_NOERROR or error code.
    */
    static EC_T_DWORD EoELinkRecvFrame(EC_T_VOID* pvInstance,
                                       EC_T_LINK_FRAMEDESC* pLinkFrameDesc);


    /*************************************************************************/
    /** \brief Allocate a frame buffer.
    *
    *          The function allocates a pure frame buffer (memory space for an
    *          Ethernet frame including header, nothing more) and attaches it
    *          to the frame descriptor ``pLinkFrameDesc`` provided by the
    *          caller.
    *
	* \param  [in] pvInstance       Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in] ppLinkFrameDesc  Pointer to a descriptor identifying an
    *                               empty frame descriptor (no memory for
    *                               the Ethernet frame attached). 
    * \param  [in] dwSize           Pointer to a ``CEcEoEUplink`` instance.
    *
    * \return EC_E_NOERROR       Function succeeded.
    *         EC_E_NOTSUPPORTED  This link layer does not support frame
    *                            allocation.
    */
    static EC_T_DWORD EoELinkAllocSendFrame(EC_T_VOID* pvInstance,
                                            EC_T_LINK_FRAMEDESC* pLinkFrameDesc,
                                            EC_T_DWORD dwSize);


    /*************************************************************************/
    /** \brief Deallocate a frame buffer.
    *
    *          The function returns the pure frame buffer (memory space for an
    *          Ethernet frame including header, nothing more) to the buffer/
    *          memory pool it came from. The frame descriptor ``pLinkFrameDesc``
    *          provided by the caller is cleared out (and stays, it cannot be
    *          returned!).
    *
    *          The function should only be called, when the caller/application
    *          did allocate a frame and for what reason ever did decide not to
    *          send it via the ``EoELinkSendAndFreeFrame()`` function.
    *
	* \param  [in] pvInstance      Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in] pLinkFrameDesc  Pointer to a descriptor which contains a
    *                              reference to the memory buffer which shall
    *                              be returned.
    *
    * \return EC_E_NOERROR       Function succeeded.
    *         EC_E_NOTSUPPORTED  This link layer does not support frame
    *                            allocation.
    */
    static EC_T_VOID EoELinkFreeSendFrame(EC_T_VOID* pvInstance,
                                          EC_T_LINK_FRAMEDESC* pLinkFrameDesc);


    /*************************************************************************/
    /** \brief Deallocate a frame buffer.
    *
    *          The function returns the pure frame buffer (memory space for an
    *          Ethernet frame including header, nothing more) to the buffer/
    *          memory pool it came from. The frame descriptor ``pLinkFrameDesc``
    *          provided by the caller is cleared out (and stays, it cannot be
    *          returned!)
    *
	* \param  [in] pvInstance      Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in] pLinkFrameDesc  Pointer to a descriptor which contains a
    *                              reference to the memory buffer which shall
    *                              be returned.
    *
    * \return EC_E_NOERROR       Function succeeded.
    *         EC_E_NOTSUPPORTED  This link layer does not support frame
    *                            allocation.
    */
    static EC_T_VOID EoELinkFreeRecvFrame(EC_T_VOID* pvInstance,
                                          EC_T_LINK_FRAMEDESC* pLinkFrameDesc);


    /*************************************************************************/
    /** \brief Retrieve the link layers Ethernet address.
    *
    *
	* \param  [in] pvInstance          Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in] pbyEthernetAddress  Pointer to an empty memory location,
    *                                  where to copy the Ethernet address.
    *
    * \return EC_E_NOERROR  Function succeeded or individual error code.
    */
    static EC_T_DWORD EoELinkGetEthernetAddress(EC_T_VOID* pvInstance,
                                                EC_T_BYTE* pbyEthernetAddress);


    /*************************************************************************/
    /** \brief Retrieve the link layers current status.
    *
    *
	* \param  [in] pvInstance  Pointer to a ``CEcEoEUplink`` instance.
    *
    * \return One of the ``EC_T_LINKSTATUS``enumerations describing the current
    *         status.
    */
    static EC_T_LINKSTATUS EoELinkGetStatus(EC_T_VOID* pvInstance);


    /*************************************************************************/
    /** \brief Retrieve the link layers physical speed.
    *
    *
	* \param  [in] pvInstance  Pointer to a ``CEcEoEUplink`` instance.
    *
    * \return The actual link speed in MBit/sec.
    */
    static EC_T_DWORD EoELinkGetSpeed(EC_T_VOID* pvInstance);


    /*************************************************************************/
    /** \brief Retrieve the link layers physical speed.
    *
    *
	* \param  [in] pvInstance  Pointer to a ``CEcEoEUplink`` instance.
    *
    * \return EcLinkMode_INTERRUPT Operate the link layer in interrupt mode.
    *                              This involves usage of the receive callback
    *                              function.
    *         EcLinkMode_POLLING   Operate the link layer in polling mode. This
    *                              means the outside world (TCP/IP/ETH protocol
    *                              stack periodically polls for newly received
    *                              frames.
    */
    static EC_T_LINKMODE EoELinkGetMode(EC_T_VOID* pvInstance);


    /*************************************************************************/
    /** \brief I/O Control function to the link layer.
    *
    *   This function offers direct access to special functions of this link
    *   layer driver. The action/function is selected via the second function
    *   parameter ``dwCode``.
    *
	* \param  [in]     pvInstance  Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in]     dwCode      I/O Control functional code; it selects the
    *                              desired IOCTL action.
    * \param  [in,out] pParms      Set of parameters for the I/O Control call.
    *                              Type of parameter set depends on the exact
    *                              I/O Control code ``dwCode``.
    *
    * \return EC_E_NOERROR  Function succeeded or individual error code.
    */
    static EC_T_DWORD EoELinkIoctl(EC_T_VOID* pvInstance,
                                   EC_T_DWORD dwCode,
                                   EC_T_LINK_IOCTLPARMS* pParms);
    
    
    /*************************************************************************/
    /** \brief Handle a frame coming in from the Ethernet switch.
    *
    *   This function is given to the attached Ethernet port ``CEthernetPort``
    *   as callback function. The port calls this function whenever it's
    *   parent switch wants to send a frame out the port. We in turn are
    *   the implementation of an uplink port and forward the frame to the
    *   outside world, the TCP/IP/ETH protocol stack respectively.
    *
	* \param  [in]  bIsInitCmd  Flag from the ECAT master mailbox implementation.
    *                           No action, only forwarding.
    * \param  [in]  pContext    Pointer to a ``CEcEoEUplink`` instance.
    * \param  [in]  pData       Pointer to the Ethernet frame we must handle.
    *                           Pointing directly to the Ethernet header.
    * \param  [in]  nData       Length of the indicated Ethernet frame, in bytes.
    *
    * \return EC_E_NOERROR  Function succeeded or individual error code.
    */
    static EC_T_DWORD EoELinkIncomingFrameCbk(EC_T_BOOL bIsInitCmd,
                                              EC_T_VOID* pContext,
                                              ETHERNET_FRAME* pData,
                                              EC_T_DWORD nData); 


    /**************************************************************************
    *           member variables
    **************************************************************************/

    /*! \var    m_bIsOpen
        \brief  Flag indication whether we are open or not.
	*/
    EC_T_BOOL   m_bIsOpen;


private:
    /*! \var    m_pEthernetSwitch
        \brief  Pointer to the Ethernet switch which our Ethernet port instance
		        is connected to.
	*/
    EC_T_DWORD  m_dwInstanceId;


    /*! \var    m_pEthernetPort
        \brief  Pointer to the Ethernet port. This is a port we own and it is
		        our communication path to the Ethernet switch.
	*/
    CEthernetPort*      m_pEthernetPort;


    /*! \var    m_pvCallerContext
        \brief  Context or handle of the caller provided by the caller during
                the ``EoELinkOpen`` call.
	*/
    EC_T_VOID*  m_pvCallerContext;

    /*! \var    m_pfReceiveCbk
        \brief  Pointer to a callback function for frames that came in from our
                parent switch and must be forwarded to the outside world, the
                TCP/IP/ETH protocol stack respectively. 
	*/
    EC_T_RECEIVEFRAMECALLBACK  m_pfReceiveCbk;


    /*! \var    m_pfNotificationCbk
        \brief  Pointer to a callback function for a not yet defined kind of
                indication from us (as link layer) to the outside worlds
                application; the TCP/IP/ETH protocol stack respectively.
	*/
    EC_T_LINK_NOTIFY  m_pfNotificationCbk;


    struct _EC_T_MEMORY_LOGGER* GetMemLog();
};
#endif /* INCLUDE_EOE_ENDPOINT */

#endif /* INC_ECETHSWITCH */

/*-END OF SOURCE FILE--------------------------------------------------------*/
