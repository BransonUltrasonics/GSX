/** ---------------------------------------------------------------------------
 * \file        EcEap.h
 * \copyright   acontis technologies GmbH, Weingarten, Germany
 * \author      mdu
 * \brief       Acontis EAP stack API header file
 *---------------------------------------------------------------------------*/

#ifndef INC_ECEAP
#define INC_ECEAP 1

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECTYPE
#include <EcType.h>
#endif
#ifndef INC_ECOS
#include <EcOs.h>
#endif
#ifndef INC_ECLINK
#include <EcLink.h>
#endif
#ifndef INC_ECERROR
#include <EcError.h>
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define       EAP_E_CFGFILENOTFOUND     ((EC_T_DWORD)0x98140000)
#define EAP_SZTXT_E_CFGFILENOTFOUND     "EDC file not found"

#define       EAP_E_WRONGSTATE          ((EC_T_DWORD)0x98140001)
#define EAP_SZTXT_E_WRONGSTATE          "Wrong EAP state"

#define       EAP_E_INITTRANSMITDATA    ((EC_T_DWORD)0x98140002)
#define EAP_SZTXT_E_INITTRANSMITDATA    "Can not intialize transmit data"

#define EAP_STATE_CHANGE_TIMEOUT        (10000)

/*-TYPEDEFS------------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_GUID
{
    EC_T_DWORD      dwData1;
    EC_T_WORD       dwData2;
    EC_T_WORD       dwData3;
    EC_T_WORD       dwData4;
    EC_T_BYTE       byData5[6];
} EC_PACKED(1) EC_T_GUID;
#include EC_PACKED_INCLUDESTOP

/** EAP state */
typedef enum _EAP_T_STATE
{
    eEapState_UNKNOWN  = 0,         /*< unknown */
    eEapState_INIT     = 1,         /*< init */
    eEapState_PREOP    = 2,         /*< pre-operational */
    eEapState_SAFEOP   = 4,         /*< safe operational */
    eEapState_OP       = 8,         /*< operational */
    eEapState_BOOTSTRAP = 3,        /*< BootStrap */
    
    /* Borland C++ datatype alignment correction */
    eEapState_BCppDummy   = 0xFFFFFFFF
} EAP_T_STATE;

/** Configuration parameter type */
typedef enum _EAP_T_CNF_TYPE
{
    eEapCnfType_Unknown    = 0,
    eEapCnfType_Filename   = 1,
    
    /* Borland C++ datatype alignment correction */
    eEapCnfType_BCppDummy  = 0xFFFFFFFF
} EAP_T_CNF_TYPE;

/** Configuration parameter description */
typedef struct _EAP_T_CONFIGPARM_DESC
{
    EAP_T_CNF_TYPE      eCnfType;   /*< Type of configuration data (file/stream) */
    EC_T_BYTE*          pbyData;    /*< Configuration data */
    EC_T_DWORD          dwLen;      /*< length of configuration data */
} EAP_T_CONFIGPARM_DESC, *EAP_PT_CONFIGPARM_DESC;

/** Protocol type */
typedef enum _EAP_T_PROTOCOL_TYPE
{
    eEapProtocol_Ethernet = 0,      /*< EtherType 0x88A4, optimized link layer */
    eEapProtocol_UDP = 1,           /*< UPD Socket 0x88A4 */
    eEapProtocol_TCP = 2            /*< TCP Socket 0x88A4, NOT SUPPORTED FOR NOW */
} EAP_T_PROTOCOL_TYPE;

/** EAP stack initialization parameters */
typedef struct _EAP_T_INITPARMS
{
    EAP_T_PROTOCOL_TYPE eProtocolType;      /**< Protocol type */
    EC_T_WORD           wSocket;            /**< Socket port (UDP, TCP) */
    EC_T_LINK_PARMS*    poLinkParms;        /**< Parameters for the link layer */
    EC_T_BYTE           abyIpAddr[4];       /**< IP address */
} EAP_T_INITPARMS, *EAP_PT_INITPARMS;

/** EAP variable information */
/** \sa eapGetVarInfo() */
typedef struct _EAP_T_VAR_INFO
{
    EC_T_WORD           wSize;              /**< variable size in bytes */
    EC_T_WORD           wIndex;             /**< index of corresponding object */   
    EC_T_CHAR           szName[256];        /**< name */
    EC_T_CHAR           szSymbolName[256];  /**< symbolic name */
    EC_T_GUID           GUID;               /**< type as GUID */
} EAP_T_VAR_INFO, *EAP_PT_VAR_INFO;

typedef EC_T_HANDLE EAP_VAR_HANDLE;
typedef EC_T_VOID (*EAP_T_PFVAR_CB)(EC_T_VOID* Context, EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar);

/*-FUNCTION DECLARATION------------------------------------------------------*/

/********************************************************************************/
/** \brief  Gets the version of EAP stack.
*
* \param    [out]  dwVersion        Softwareversion
*
* \return   always EC_E_NOERROR
*/
ATECAT_API EC_T_DWORD eapGetVersion(EC_T_DWORD *dwVersion);

/********************************************************************************/
/** \brief  Configures the EAP stack.
* \details  Reads a configuration file (EDC).
*
* \param    [in]   hPort       EAP stack handle
* \param    [in]   oConfig     Parameter description structure
* \sa       EAP_T_CONFIGPARM_DESC
*
* \return   Error code or EC_E_NOERROR if succeeded
*/
ATECAT_API EC_T_DWORD eapConfigure(EC_T_HANDLE hPort, EAP_T_CONFIGPARM_DESC oConfig);

/********************************************************************************/
/** \brief  Initializes and starts the EAP stack.
* \details  Gives back a stack handle, this handle will used 
*           in further functions calls
*
* \param    [in]   oParms Initialization parameters structure
* \param    [out]  phPort EAP stack handle
* \sa       EAP_T_INITPARMS
*
* \return   Error code or EC_E_NOERROR if succeeded
*/
ATECAT_API EC_T_DWORD eapInit(EAP_T_INITPARMS oParms, EC_T_HANDLE* hPort);
/********************************************************************************/
/** \brief  Deinitializes the EAP stack
* \details  After this function call the handle hPort is invalid and can not
*           be used for further calls.
*
* \param    [in]   hPort    EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*/
ATECAT_API EC_T_DWORD eapDeinit(EC_T_HANDLE hPort);

/********************************************************************************/
/** \brief  Switches the state of the stack SM (state machine)
*
* \param    [in]   hPort        EAP stack handle
* \param    [in]   eNewState    New SM state
* \param    [in]   dwTimeout    Time out for operation completion
* \sa       EAP_T_STATE
*
* \return   Error code or EC_E_NOERROR if succeeded
*/
ATECAT_API EC_T_DWORD eapSetState(EC_T_HANDLE hPort, EAP_T_STATE eNewState, EC_T_DWORD dwTimeout);
/********************************************************************************/
/** \brief  Gives back the current state of the stack SM (state machine)
*
* \param    [in]   hPort       EAP stack handle
* \param    [out]  eState      Current SM state (\sa ::EAP_T_STATE)
* \sa       EAP_T_STATE
*
* \return   Error code or EC_E_NOERROR if succeeded
*/
ATECAT_API EC_T_DWORD eapGetState(EC_T_HANDLE hPort, EAP_T_STATE* eState);

/********************************************************************************/
/** \brief  Returns a number of transmit variables (0x6xxx objects)
* \details  The returned value can be used to allocate memory for
*           a eapGetTxVarList() call
* \warning  Memory for wNumOfVars has to be allocated prior this function call!
*
* \param    [in]   hPort        EAP stack handle
* \param    [out]  wNumOfVars   Number of transmit variables (0x6xxx objects)
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetTxVarList()
*/
ATECAT_API EC_T_DWORD eapGetNumOfTxVars(EC_T_HANDLE hPort, EC_T_WORD* wNumOfVars);
/********************************************************************************/
/** \brief  Returns an array with transmit varaibles indexes (0x6000 to 0x6FFF)
*           of configured variables.
* \details  The amount of variables is returned by eapGetNumOfTxVars()
* \warning  Memory for aIndexes has to be allocated prior this function call!
*
* \param    [in]   hPort        EAP stack handle
* \param    [out]  aIndexes     Transmit variables indexes (0x6000 to 0x6FFF)
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetNumOfTxVars()
*/
ATECAT_API EC_T_DWORD eapGetTxVarList(EC_T_HANDLE hPort, EC_T_WORD* aIndexes);

/********************************************************************************/
/** \brief  Returns a number of receive variables (0x7xxx objects)
* \details  The returned value can be used to allocate memory for
*           a eapGetRxVarList() call
* \warning  Memory for wNumOfVars has to allocated prior this function call!
*
* \param    [in]   hPort        EAP stack handle
* \param    [out]  wNumOfVars   Number of receive variables (0x7xxx objects)
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetRxVarList()
*/
ATECAT_API EC_T_DWORD eapGetNumOfRxVars(EC_T_HANDLE hPort, EC_T_WORD* wNumOfVars);
/********************************************************************************/
/** \brief  Returns an array with receive varaibles indexes (0x7000 to 0x7FFF) 
*           of configured variables.
* \details  The amount of variables is returned by eapGetNumOfRxVars()
* \warning  Memory for aIndexes has to allocated prior this function call!
*
* \param    [in]   hPort        EAP stack handle
* \param    [out]  aIndexes     Receive variables indexes (0x7000 to 0x7FFF)
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetNumOfRxVars()
*/
ATECAT_API EC_T_DWORD eapGetRxVarList(EC_T_HANDLE hPort, EC_T_WORD* aIndexes);

/********************************************************************************/
/** \brief  Gets variable handle by object index
* 
* \details  The object index has to be between 0x6000..0x6FFF and 0x7000..0x7FFF.
*           The memory for hHandle has to be allocated prior function call.
*           The handle will be use for further calls like eapGetVarInfo() or eapVarRead()
*
* \param    [in]        hPort       EAP stack handle
* \param    [out]       hHandle     handle of an EAP variable
* \param    [in]        wIndex      object index (within 0x6nnn or 0x7nnn range) 
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandleByName()
*/
ATECAT_API EC_T_DWORD eapGetHandleByIndex(EC_T_HANDLE hPort,  EAP_VAR_HANDLE* hVar,  EC_T_WORD wIndex);
/********************************************************************************/
/** \brief  Gets variable handle by name
* 
* \details  If more than one variable with given name exists, a first one (smallest
*           object index) will be taken. The handle will be use for further calls
*           like eapGetVarInfo() or eapVarRead().
*
* \param    [in]        hPort       EAP stack handle
* \param    [out]       hHandle     handle of an EAP variable
* \param    [in]        szName      variable name
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandleByIndex()
*/
ATECAT_API EC_T_DWORD eapGetHandleByName(EC_T_HANDLE hPort, EAP_VAR_HANDLE* hVar, EC_T_CHAR *szName);
/********************************************************************************/
/** \brief  Gets information about a variable
* 
* \details  The variable handle has to be obtained prior function call.
*           The buffer memory for pVarInfo has to be allocated as well.
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [out]       pVarInfo    information structure
* \sa       EAP_T_VAR_INFO
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName()
*/
ATECAT_API EC_T_DWORD eapGetVarInfo(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EAP_T_VAR_INFO* pVarInfo);

/********************************************************************************/
/** \brief  Registers a callback function to be called,
*           when a variable changes its state
* 
* \details  The variable handle has to be obtained prior function call.
*           Use EC_NULL as fnCallback to unregister.
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [in]        fnCallback  callback function
* \param    [in]        pvContext   context, can be NULL
* \sa       EAP_T_PFVAR_CB
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName()
*/
ATECAT_API EC_T_DWORD eapVarRegisterCallback(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EAP_T_PFVAR_CB fnCallback, EC_T_VOID* pvContext);

/********************************************************************************/
/** \brief  Returns a value of a variable
* 
* \details  The variable handle has to be obtained prior function call.
*           The buffer memory has to be allocated as well. During operation the
*           curresponding memory will be locked.
*           The parameter dwDataLen [in]/[out] contains the buffer size before
*           and after function call
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [out]       pData       data buffer
* \param    [in,out]    dwDataLen   buffer size before/after function call
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName(), eapVarReadNoLock(),
*           eapVarWriteNoLock(), eapVarWrite()
*/
ATECAT_API EC_T_DWORD eapVarRead(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EC_T_PVOID pData, EC_T_DWORD* dwDataLen);
/********************************************************************************/
/** \brief  Writes a value to a variable
* 
* \details  The variable handle has to be obtained prior function call.
*           During operation the curresponding memory will be locked.
*           the parameter dwDataLen [in]/[out] contains the buffer size
*           before and after function call.
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [in]        pData       data buffer
* \param    [in,out]    dwDataLen   buffer size before/after function call
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName(), eapVarReadNoLock(),
*           eapVarWriteNoLock(), eapVarRead()
*/
ATECAT_API EC_T_DWORD eapVarWrite(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EC_T_PVOID pData, EC_T_DWORD* dwDataLen);
/********************************************************************************/
/** \brief  Sets the memory lock for access to variables.
* 
* \details  In a multithreading enveronment protects against unexpected
*           memory changes.
*
* \param    [in]        hPort       EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapVarReadNoLock(), eapVarWriteNoLock(), eapVarUnlock()
*/
ATECAT_API EC_T_DWORD eapVarLock(EC_T_HANDLE hPort);
/********************************************************************************/
/** \brief  Releases locked memory for access after eapVarLock()
* 
* \param    [in]        hPort       EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapVarReadNoLock(), eapVarWriteNoLock(), eapVarLock()
*/
ATECAT_API EC_T_DWORD eapVarUnlock(EC_T_HANDLE hPort);

/********************************************************************************/
/** \brief  Returns a value of a variable
* 
* \details  The variable handle has to be obtained prior function call.
*           The buffer memory has to be allocated as well.
*           No memory lock during operation. The parameter dwDataLen [in]/[out]
*           contains the buffer size before and after function call.
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [out]       pData       data buffer
* \param    [in,out]    dwDataLen   buffer size bifore/after function call
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName(), eapVarLock(), eapVarUnlock(),
*           eapVarRead()
*/
ATECAT_API EC_T_DWORD eapVarReadNoLock(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EC_T_PVOID pData, EC_T_DWORD* dwDataLen);
/********************************************************************************/
/** \brief  Writes a value to a variable
* 
* \details  The variable handle has to be obtained prior function call.
*           No memory lock during operation. the parameter dwDataLen [in]/[out]
*           contains the buffer size before and after function call.
*
* \param    [in]        hPort       EAP stack handle
* \param    [in]        hVar        handle of an EAP variable
* \param    [in]        pData       data buffer
* \param    [in,out]    dwDataLen   buffer size before/after function call
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapGetHandlebyIndex(), eapGetHandleByName(), eapVarLock(), eapVarUnlock(),
*           eapVarWrite()
*/
ATECAT_API EC_T_DWORD eapVarWriteNoLock(EC_T_HANDLE hPort, EAP_VAR_HANDLE hVar, EC_T_PVOID pData, EC_T_DWORD* dwDataLen);

/********************************************************************************/
/** \brief  Process receive data frames
* 
* \details  This function has to be called cyclically in oreder to maintain
*           proper communication. The data can be only received synchron
*           to the device cycle.
*
* \param    [in]        hPort       EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessTxData(), eapGetCycleTime(), eapSetCycleTime(),
*           eapCalculateCycleTime(), eapJobOnTimer()
*/
ATECAT_API EC_T_DWORD eapJobProcessRxData(EC_T_HANDLE hPort);
/********************************************************************************/
/** \brief  Process transmit data frames
* 
* \details  This function has to be called cyclically in oreder to maintain
*           proper communication. The data can be only sent synchron to
*           the device cycle.
*
* \param    [in]        hPort       EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessRxData(), eapGetCycleTime(), eapSetCycleTime(),
*           eapCalculateCycleTime(), eapJobOnTimer()
*/
ATECAT_API EC_T_DWORD eapJobProcessTxData(EC_T_HANDLE hPort);
/********************************************************************************/
/** \brief  Performs cyclical tasks
*
* \details  Has to be called cyclically in the main application.
* 
* \param    [in]        hPort           EAP stack handle
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessRxData(), eapJobProcessTxData(), eapGetCycleTime(),
*           eapCalculateCycleTime(), eapSetCycleTime()
*/
ATECAT_API EC_T_DWORD eapJobOnTimer(EC_T_HANDLE hPort);
           
/********************************************************************************/
/** \brief  Gets the actual EAP cycle time
* 
* \param    [in]        hPort           EAP stack handle
* \param    [out]       dwCycleTime     cycle time in uS
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessRxData(), eapJobProcessTxData(), eapSetCycleTime(),
*           eapCalculateCycleTime(), eapJobOnTimer()
*/
ATECAT_API EC_T_DWORD eapGetCycleTime(EC_T_HANDLE hPort, EC_T_DWORD* dwCycleTime);
/********************************************************************************/
/** \brief  Calculates the best cycle time for proper communication
* 
* \details  The cycle time value will be calculated as "Greatest Common Divider"
*           of all timings in the EAP stack.
*
* \param    [in]        hPort           EAP stack handle
* \param    [out]       dwCycleTime     cycle time in uS
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessRxData(), eapJobProcessTxData(), eapSetCycleTime(),
*           eapCalculateCycleTime(), eapGetCycleTime(), eapJobOnTimer()
*/
ATECAT_API EC_T_DWORD eapCalculateCycleTime(EC_T_HANDLE hPort, EC_T_DWORD* dwCycleTime);
/********************************************************************************/
/** \brief  Sets a new EAP cycle time
* 
* \param    [in]        hPort           EAP stack handle
* \param    [out]       dwCycleTime     cycle time in uS
*
* \return   Error code or EC_E_NOERROR if succeeded
*
* \sa       eapJobProcessRxData(), eapJobProcessTxData(), eapGetCycleTime(),
*           eapCalculateCycleTime(), eapJobOnTimer()
*/
ATECAT_API EC_T_DWORD eapSetCycleTime(EC_T_HANDLE hPort, EC_T_DWORD dwCycleTime);

/********************************************************************************/
/** \brief  Returns text by id, i.e. textual description of an error code
*
* \param    [in]        dwTextId        text id
*
* \return   Text as a NULL terminated character string (can be NULL if no text found)
*/
ATECAT_API const EC_T_CHAR* eapGetText(EC_T_DWORD dwTextId);

ATECAT_API EC_T_VOID ecatPerfMeasInit( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_UINT64 dwlFreqSet, EC_T_DWORD dwNumMeas, EC_T_FNMESSAGE pfnMessage);
ATECAT_API EC_T_VOID ecatPerfMeasDeinit( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasEnable( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasDisable( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasStart( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_TSC_TIME* ecatPerfMeasEnd( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_VOID ecatPerfMeasReset( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_VOID ecatPerfMeasShow( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex, EC_T_CHAR** aszMeasInfo );

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INC_ECEAP */

/*-END OF SOURCE FILE--------------------------------------------------------*/
