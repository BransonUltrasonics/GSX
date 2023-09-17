/*-----------------------------------------------------------------------------
 * EcIoImageDefault.cpp     implementation file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              implementation of the CEcIoImage class.
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>
#include <EcIoImage.h>

/*---------------------------------------------------------------------------*/
CEcIoImage* CEcIoImage::CreateInstance(EC_T_INT nIn, EC_T_INT nOut, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    CEcIoImage* poEcIoImage = EC_NEW(CEcIoImage(nIn, nOut, pMLog));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::poEcIoImage", poEcIoImage, sizeof(CEcIoImage));
    if(EC_NULL==poEcIoImage || !((CEcIoImage*)poEcIoImage)->AllocOk() )
    {
        SafeDelete(poEcIoImage);
    }
    return poEcIoImage;
}


/*-Construction/Destruction--------------------------------------------------*/
CEcIoImage::CEcIoImage(EC_T_DWORD nIn, EC_T_DWORD nOut, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    m_pIn  = EC_NULL;
    m_pOut = EC_NULL;
    m_nIn  = nIn;
    m_nOut = nOut;

    if (m_nIn > 0)
    {
        m_pIn = (EC_T_BYTE*)OsMalloc(m_nIn);
        EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::m_pIn", m_pIn, m_nIn);
        OsDbgAssert(m_pIn != EC_NULL);
        if (m_pIn != EC_NULL)
        {
            OsMemset(m_pIn, 0, m_nIn);
        }
    }
    if (m_nOut > 0)
    {
        m_pOut = (EC_T_BYTE*)OsMalloc(m_nOut);
        EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::m_pOut", m_pOut, m_nOut);
        OsDbgAssert(m_pOut != EC_NULL);
        if (m_pOut != EC_NULL)
        {
            OsMemset(m_pOut, 0, m_nOut);
        }
    }
}

EC_T_VOID CEcIoImage::DestroyInstance(CEcIoImage* pIns, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    if (pIns)
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::~m_pOut", pIns->m_pOut, pIns->m_nOut);
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::~m_pIn", pIns->m_pIn, pIns->m_nIn);
        SafeOsFree(pIns->m_pOut);
        SafeOsFree(pIns->m_pIn);
        
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_INTERFACE, pMLog, "CEcIoImage::DestroyInstance", pIns, sizeof(CEcIoImage));
        SafeDelete(pIns);
    }
}

CEcIoImage::~CEcIoImage()
{
}

/********************************************************************************/
/** \brief 
*
* \return 
*/
EC_T_BYTE* CEcIoImage::GetInputPtr( EC_T_DWORD offs, EC_T_DWORD size )
{
    if ( m_pIn && (offs + size <= m_nIn) )
        return &m_pIn[offs];
    else
        return EC_NULL;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_BYTE* CEcIoImage::GetOutputPtr( EC_T_DWORD offs, EC_T_DWORD size )
{
    if ( m_pOut && (offs + size <= m_nOut) )
        return &m_pOut[offs];
    else
        return EC_NULL;
}


/*-END OF SOURCE FILE--------------------------------------------------------*/
