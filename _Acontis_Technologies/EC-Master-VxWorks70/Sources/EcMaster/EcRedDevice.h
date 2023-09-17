/*-----------------------------------------------------------------------------
 * EcRedDevice.h            header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description				interface for the CEcRedDevice class.
 *---------------------------------------------------------------------------*/


#ifndef INC_ECREDDEVICE
#define INC_ECREDDEVICE


/*-INCLUDES------------------------------------------------------------------*/

#if (defined INCLUDE_RED_DEVICE)

#include <EcDeviceBase.h>

/*-DEFINES/MACROS------------------------------------------------------------*/


/*-TYPEDEFS/ENUMS------------------------------------------------------------*/


/********************************************************************************/
/** \brief class for redundant EtherCAT connection device.
*
* \return OK or ERROR
*/
class CEcRedDevice : public CEcDeviceBase
{
public:

    CEcRedDevice(CEcDeviceBase* poEcDevice);
    virtual ~CEcRedDevice();

    /*-CEcDeviceBase overrides ----------------------------------------------*/
    virtual EC_T_DWORD OpenDevice(EC_T_VOID* pParms);
    virtual EC_T_DWORD CloseDevice(EC_T_BOOL bDeinitForConfiguration);


    /*-CEcRedDevice----------------------------------------------------------*/
public:

protected:
    virtual EC_T_VOID ReceiveFrameCallback(EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);
    static  EC_T_DWORD ReceiveFrameCallbackGateway(EC_T_VOID* pvContext, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);

protected:
    CEcDeviceBase* m_poEcDevice;

private:

};

#endif

#endif /* INC_ECREDDEVICE */


/*-END OF SOURCE FILE--------------------------------------------------------*/
