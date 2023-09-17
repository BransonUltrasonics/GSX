/*-----------------------------------------------------------------------------
 * EcRedDevice.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              implementation of the CEcRedDevice class.
 *---------------------------------------------------------------------------*/


/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#if (defined INCLUDE_RED_DEVICE)

#include <EthernetServices.h>
#include <EcLink.h>
#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EcRedDevice.h>

#include <EcMasterCommon.h>
#include <EcEthSwitch.h>
#include <EcServices.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcDevice.h>

#include <EcMaster.h>

/*-CONSTRUCTION/DESTRUCTION--------------------------------------------------*/
CEcRedDevice::CEcRedDevice(CEcDeviceBase* poEcDevice) 
: 
    CEcDeviceBase(EC_TRUE),
    m_poEcDevice(poEcDevice)
{
    m_pMaster = m_poEcDevice->GetMaster();
}


CEcRedDevice::~CEcRedDevice()
{
    CloseDevice(EC_FALSE);
}


/*-IECDEVICE-----------------------------------------------------------------*/


/********************************************************************************/
/** \brief Opens the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcRedDevice::OpenDevice(EC_T_VOID* pParms)
{
EC_T_DWORD dwRetVal    = EC_E_ERROR;

    /* call base class first */
    dwRetVal = OpenDeviceBase(pParms, this, ReceiveFrameCallbackGateway);
    if (dwRetVal != EC_E_NOERROR)
    {
        goto Exit;
    }

    /* no errors */
    m_bOpened = EC_TRUE;
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}   


/********************************************************************************/
/** \brief Closes the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcRedDevice::CloseDevice(EC_T_BOOL bDeinitForConfiguration)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check state */
    if( !m_bOpened )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* call base class last */
    dwRetVal = CloseDeviceBase(bDeinitForConfiguration);
    if (dwRetVal != EC_E_NOERROR)
    {
        goto Exit;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief This function is called by the link layer after a new frame was received.
*
* \return status code
*/
EC_T_VOID CEcRedDevice::ReceiveFrameCallback(EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed)
{
    /* incoming frames are currently handled directly */
    ((CEcDevice*)m_poEcDevice)->ProcessFrame(pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize, EC_TRUE, pbFrameProcessed);
}


/********************************************************************************/
/** \brief This function is called by the link layer after a new frame was received.
*
* \return status code
*/
EC_T_DWORD CEcRedDevice::ReceiveFrameCallbackGateway(EC_T_VOID* pvContext, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed)
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* incoming frames are currently handled directly */
    OsDbgAssert( pvContext != EC_NULL );
    if (pvContext != EC_NULL)
    {
        ((CEcRedDevice*)pvContext)->ReceiveFrameCallback(pLinkFrameDesc, pbFrameProcessed);
    }
    else
    {
        /* this frame will never be processed! */
        *pbFrameProcessed = EC_TRUE;
    }
    return dwRetVal;
}

#endif /* (defined INCLUDE_RED_DEVICE) */

/*-END OF SOURCE FILE--------------------------------------------------------*/
