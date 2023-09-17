/*-----------------------------------------------------------------------------
 * Copyright    acontis technologies GmbH, Weingarten, Germany
 * Response     Stefan Zintgraf
 * Description  EtherCAT master performance measurement routines
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"
#include "EcTimer.h"

/*-DEFINES-------------------------------------------------------------------*/
#define FILTVAL 128
#define OLD_WEIGHT (FILTVAL-1)
#define ROUNDINGVAL (FILTVAL/2)
#define AVG_MULT (FILTVAL/8)

/*-GLOBALS-------------------------------------------------------------------*/
#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
extern "C" EC_T_TSC_MEAS_DESC G_TscMeasDesc;
extern "C" EC_T_CHAR* G_aszMeasInfo[];
#endif

/*-LOCALS--------------------------------------------------------------------*/
class CPerfMeas
{
public:
    static EC_INLINESTART EC_T_VOID         Init(   EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                                   ,EC_T_UINT64             dwlFreqSet      /**< [in]     TSC frequency, 0: auto-calibrate */
                                                   ,EC_T_DWORD              dwNumMeas       /**< [in]     number of elements to be allocated in in pTscMeasDesc->aTscTime */
                                                   ,EC_T_FNMESSAGE          pfnMessage                                                                  )
    {
        EC_T_DWORD      dwCnt;
        EC_T_TSC_TIME*  aTscTime;
        CEcTimer*       poTimeout;

        aTscTime = (EC_T_TSC_TIME*)OsMalloc(dwNumMeas * sizeof(EC_T_TSC_TIME));
        OsDbgAssert(aTscTime != EC_NULL);
        OsMemset(aTscTime, 0, dwNumMeas * sizeof(EC_T_TSC_TIME));
        pTscMeasDesc->bMeasEnabled = EC_FALSE;
        pTscMeasDesc->dwNumMeas = dwNumMeas;
        poTimeout = EC_NEW(CEcTimer);
        pTscMeasDesc->pPrivateData = (EC_T_VOID*)poTimeout;
        OsMeasCalibrate(dwlFreqSet);
        s_qw100kHzFrequency = OsMeasGet100kHzFrequency();
        for (dwCnt = 0; dwCnt < dwNumMeas; dwCnt++)
        {
            aTscTime[dwCnt].bMeasReset = EC_TRUE;
        }
        pTscMeasDesc->aTscTime = aTscTime;
        pTscMeasDesc->pfnMessage = pfnMessage;
        poTimeout->Start(2);        /* start timeout helper to get values after first call of Show(...) */
    } EC_INLINESTOP

    static EC_INLINESTART EC_T_VOID         Deinit( EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */                     )
    {
        if (EC_NULL != pTscMeasDesc->aTscTime)
        {
            CEcTimer* poTimeout = (CEcTimer*)pTscMeasDesc->pPrivateData;
            pTscMeasDesc->bMeasEnabled = EC_FALSE;
            pTscMeasDesc->dwNumMeas = 0;
            SafeDelete(poTimeout);
            pTscMeasDesc->pPrivateData = EC_NULL;
            SafeOsFree(pTscMeasDesc->aTscTime);
        }
    } EC_INLINESTOP
                                     
    static EC_INLINESTART EC_T_VOID         Enable( EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */                     )
    {
        pTscMeasDesc->bMeasEnabled = EC_TRUE;
    } EC_INLINESTOP
                                     
    static EC_INLINESTART EC_T_VOID         Start(  EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                                   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index */                          )
    {
        if (pTscMeasDesc->bMeasEnabled && (EC_NULL != pTscMeasDesc->aTscTime))
        {
            OsDbgAssert(dwIndex < pTscMeasDesc->dwNumMeas);
            if (s_bIrqCtlEnabled)
            {
                OsTscMeasDisableIrq(pTscMeasDesc, dwIndex);
            }
            pTscMeasDesc->aTscTime[dwIndex].qwStart = OsMeasGetCounterTicks();
        }
    } EC_INLINESTOP
                         
    static EC_INLINESTART EC_T_TSC_TIME*    End(    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                                   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index */                          )
    {
        EC_T_UINT64     qwDiff = 0;
        EC_T_DWORD      dwDiffLow;              /* [1/10] usec */
        EC_T_TSC_TIME*  pTscTime = EC_NULL;
        EC_T_BOOL       bReset = EC_FALSE;

        if (pTscMeasDesc->bMeasEnabled && (EC_NULL != pTscMeasDesc->aTscTime))
        {
            EC_T_UINT64 qwEnd = OsMeasGetCounterTicks();

            OsDbgAssert(dwIndex < pTscMeasDesc->dwNumMeas);
            if (s_bIrqCtlEnabled)
            {
                OsTscMeasEnableIrq(pTscMeasDesc, dwIndex);
            }
            pTscTime = &pTscMeasDesc->aTscTime[dwIndex];
            if (pTscTime->qwStart == 0)
            {
                goto Exit;
            }
            /* reset? */
            if (pTscTime->bMeasReset)
            {
                pTscTime->dwCurr  = 0;
                pTscTime->dwAvg   = 0;
                pTscTime->dwMin   = (EC_T_DWORD)ULONG_MAX;
                pTscTime->dwMax   = 0;
                pTscTime->bMeasReset = EC_FALSE;
                bReset = EC_TRUE;
            }        
            pTscTime->qwEnd = qwEnd;
            if ((pTscTime->qwEnd < pTscTime->qwStart) && (0 == EC_HIDWORD(pTscTime->qwStart)))
            {
                /* assume 32bit rollover */
                qwDiff = EC_LODWORD(pTscTime->qwEnd) - EC_LODWORD(pTscTime->qwStart);
            }
            else
            {
                qwDiff = pTscTime->qwEnd - pTscTime->qwStart;
            }
            
            /* convert to [1/10] usec */
            if (EC_HIDWORD(qwDiff) & 0xFF000000)  /* for big values don't multiply to prevent from overrun */
            {
                qwDiff = (qwDiff / s_qw100kHzFrequency) * 100;
            }
            else
            {
                qwDiff = (qwDiff * 100) / s_qw100kHzFrequency;
            }
            
            dwDiffLow = EC_LODWORD(qwDiff); //limited to 429 Seconds
            pTscTime->dwCurr = dwDiffLow;
            if (bReset)
            {
                pTscTime->dwAvg  = AVG_MULT*10*dwDiffLow;
            }
            else
            {
                /* pTscTime->dwAvg  = ((pTscTime->dwAvg * 31) + (10*dwDiffLow) + 16) / 32; */
                pTscTime->dwAvg  = ((pTscTime->dwAvg * OLD_WEIGHT) + (AVG_MULT * 10 * dwDiffLow) + ROUNDINGVAL) / FILTVAL;
            }
            pTscTime->dwMax  = EC_MAX(pTscTime->dwMax, dwDiffLow);
            pTscTime->dwMin  = EC_MIN(pTscTime->dwMin, dwDiffLow);
        }
    Exit:
        return pTscTime;
    } EC_INLINESTOP
                            
    static EC_INLINESTART EC_T_VOID         Reset(  EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                                   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index, 0xFFFFFFFF: all indexes */ )
    {
        EC_T_DWORD dwIdx = 0;

        if (EC_NULL == pTscMeasDesc)
        {
#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
            pTscMeasDesc = &G_TscMeasDesc;
#else
            return;
#endif
        }
        
        if (0xFFFFFFFF == dwIndex)
        {
            for (dwIdx = 0; dwIdx < pTscMeasDesc->dwNumMeas; dwIdx++)
            {
                pTscMeasDesc->aTscTime[dwIdx].bMeasReset = EC_TRUE;
            }
        }
        else if (dwIndex < pTscMeasDesc->dwNumMeas)
        {
            pTscMeasDesc->aTscTime[dwIndex].bMeasReset = EC_TRUE;
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }
    } EC_INLINESTOP

    static EC_INLINESTART EC_T_VOID         Disable(EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */                     )
    {
        pTscMeasDesc->bMeasEnabled = EC_FALSE;
        Reset(pTscMeasDesc, 0xFFFFFFFF);
    } EC_INLINESTOP

    static EC_INLINESTART EC_T_VOID         Show(   EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in]     measurement descriptor */
                                                   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index, 0xFFFFFFFF: all indexes */
                                                   ,EC_T_CHAR**             aszMeasCaption  /**< [in]     measurement caption */                        )
    {
#ifdef INCLUDE_LOG_MESSAGES
        CEcTimer* poShowTimeout;
        EC_T_DWORD dwCnt;
        EC_T_DWORD dwCntStart = 0;
        EC_T_DWORD dwCntEnd = 0;
        EC_T_DWORD dwAvgLow, dwAvgHigh;
        EC_T_DWORD dwAvg;

        if (EC_NULL == pTscMeasDesc)
        {
#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
            pTscMeasDesc = &G_TscMeasDesc;
            aszMeasCaption = G_aszMeasInfo;
#else
            return;
#endif
        }
        dwCntEnd = pTscMeasDesc->dwNumMeas - 1;

        if (pTscMeasDesc->bMeasEnabled)
        {
            poShowTimeout = (CEcTimer*)pTscMeasDesc->pPrivateData;
            OsDbgAssert(poShowTimeout != EC_NULL);
            if (!poShowTimeout->IsStarted())
            {
                poShowTimeout->Start(2000);
            }
            /* don't show results faster than every 2 seconds */
            if (poShowTimeout->IsElapsed())
            {
                poShowTimeout->Start(2000);
                EC_DBGMSG("============================================================================\n");
                if (0xFFFFFFFF != dwIndex)
                {
                    OsDbgAssert(dwIndex < pTscMeasDesc->dwNumMeas);
                    dwCntStart = dwCntEnd = dwIndex;
                }
                for (dwCnt = dwCntStart; dwCnt <= dwCntEnd; dwCnt++)
                {
                    if ((0xFFFFFFFF != pTscMeasDesc->aTscTime[dwCnt].dwAvg) && !pTscMeasDesc->aTscTime[dwCnt].bMeasReset)
                    {
                        /* average value = dwAvgHigh.dwAvgLow rounded to decimal
                           if 0.95 to 0.99 ==> dwAvgHigh increases by 1 and dwAvgLow becomes 0
                         */
                        dwAvg = pTscMeasDesc->aTscTime[dwCnt].dwAvg / AVG_MULT;
                        dwAvgHigh = (EC_T_DWORD)dwAvg / 100;
                        dwAvgLow = ((dwAvg % 100) + 5) / 10;
                        if (10 == dwAvgLow)
                        {
                            dwAvgHigh++;
                            dwAvgLow = 0;
                        }
                        EC_DBGMSG("PerfMsmt '%s' (min/avg/max) [usec]: %4d.%d/%4d.%d/%4d.%d\n",
                            aszMeasCaption[dwCnt],
                            pTscMeasDesc->aTscTime[dwCnt].dwMin / 10,
                            pTscMeasDesc->aTscTime[dwCnt].dwMin % 10,
                            dwAvgHigh,
                            dwAvgLow,
                            pTscMeasDesc->aTscTime[dwCnt].dwMax / 10,
                            pTscMeasDesc->aTscTime[dwCnt].dwMax % 10);
                    }
                }
            }
        }
#else
        EC_UNREFPARM(pTscMeasDesc);
        EC_UNREFPARM(dwIndex);
        EC_UNREFPARM(aszMeasCaption);
#endif /* INCLUDE_LOG_MESSAGES */
    } EC_INLINESTOP

    static EC_T_UINT64 s_qw100kHzFrequency; /* frequency in 100 kHz units (e.g. 1MHz = 10) */
    static EC_T_BOOL  s_bIrqCtlEnabled;
};
EC_T_UINT64 CPerfMeas::s_qw100kHzFrequency = 0;
EC_T_BOOL  CPerfMeas::s_bIrqCtlEnabled = EC_TRUE;

/***************************************************************************************************/
/**
@brief  Initialize performance measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasInit(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out]  measurement descriptor */
   ,EC_T_UINT64             dwlFreqSet      /**< [in]      TSC frequency, 0: auto-calibrate */
   ,EC_T_DWORD              dwNumMeas       /**< [in]      number of elements to be allocated in in pTscMeasDesc->aTscTime */
   ,EC_T_FNMESSAGE          pfnMessage
                                   )
{
    CPerfMeas::Init(pTscMeasDesc, dwlFreqSet, dwNumMeas, pfnMessage);
}

/***************************************************************************************************/
/**
@brief  Deinitialize performance measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasDeinit(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc   /**< [in,out]  measurement descriptor */
                                     )
{
    CPerfMeas::Deinit(pTscMeasDesc);
}

/***************************************************************************************************/
/**
@brief  Enable performance measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasEnable( 
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                     )
{
    CPerfMeas::Enable(pTscMeasDesc);
}

/***************************************************************************************************/
/**
@brief  Start measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasStart(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index */
                         )
{
    CPerfMeas::Start(pTscMeasDesc, dwIndex);
}

/***************************************************************************************************/
/**
@brief  End measurement

@return pointer to corresponding time descriptor
*/
extern "C" ATECAT_API EC_T_TSC_TIME* ecatPerfMeasEnd(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index */
                            )
{
    return CPerfMeas::End(pTscMeasDesc, dwIndex);
}

/***************************************************************************************************/
/**
@brief  Reset measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasReset(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
   ,EC_T_DWORD              dwIndex         /**< [in]     measurement index, 0xFFFFFFFF: all indexes */
)
{
    CPerfMeas::Reset(pTscMeasDesc, dwIndex);
}

/***************************************************************************************************/
/**
@brief  Disable performance measurement

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasDisable(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in,out] measurement descriptor */
                                     )
{
    CPerfMeas::Disable(pTscMeasDesc);
}

/***************************************************************************************************/
/**
@brief  Show measurement results

@return N/A
*/
extern "C" ATECAT_API EC_T_VOID ecatPerfMeasShow(
    EC_T_TSC_MEAS_DESC*     pTscMeasDesc    /**< [in]  measurement descriptor */
   ,EC_T_DWORD              dwIndex         /**< [in]  measurement index, 0xFFFFFFFF: all indexes */
   ,EC_T_CHAR**             aszMeasCaption  /**< [in]  measurement caption */
)
{
    CPerfMeas::Show(pTscMeasDesc, dwIndex, aszMeasCaption);
}

extern "C" ATECAT_API EC_T_VOID ecatPerfMeasSetIrqCtlEnabled(EC_T_BOOL bEnabled)
{
    CPerfMeas::s_bIrqCtlEnabled = bEnabled;
}
