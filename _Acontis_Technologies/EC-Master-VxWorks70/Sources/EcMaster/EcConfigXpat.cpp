/*-----------------------------------------------------------------------------
 * EcConfigXpat.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description
 *---------------------------------------------------------------------------*/
/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcLink.h>
#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EthernetServices.h>
#include <EcServices.h>
#include <EcConfig.h>

#if (defined INCLUDE_XPAT)
#include <EcMasterCommon.h>
#include <EcEthSwitch.h>
#include "EcConfigXpat.h"

/*-DEFINES-------------------------------------------------------------------*/
#undef VERBOSE_DEBUG
#endif /*(defined INCLUDE_XPAT)*/
#define DISABLED_SM  (EC_T_WORD)(0xFFFF)

/*-CEcCRC32------------------------------------------------------------------*/
static const EC_T_DWORD SC_adwCrcTab[] = {
    /* 0x00 */ 0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    /* 0x04 */ 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    /* 0x08 */ 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    /* 0x0C */ 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    /* 0x10 */ 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    /* 0x14 */ 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    /* 0x18 */ 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    /* 0x1C */ 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    /* 0x20 */ 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    /* 0x24 */ 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    /* 0x28 */ 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    /* 0x2C */ 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    /* 0x30 */ 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    /* 0x34 */ 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    /* 0x38 */ 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    /* 0x3C */ 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    /* 0x40 */ 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    /* 0x44 */ 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    /* 0x48 */ 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    /* 0x4C */ 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    /* 0x50 */ 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    /* 0x54 */ 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    /* 0x58 */ 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    /* 0x5C */ 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    /* 0x60 */ 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    /* 0x64 */ 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    /* 0x68 */ 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    /* 0x6C */ 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    /* 0x70 */ 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    /* 0x74 */ 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    /* 0x78 */ 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    /* 0x7C */ 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    /* 0x80 */ 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    /* 0x84 */ 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    /* 0x88 */ 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    /* 0x8C */ 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    /* 0x90 */ 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    /* 0x94 */ 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    /* 0x98 */ 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    /* 0x9C */ 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    /* 0xA0 */ 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    /* 0xA4 */ 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    /* 0xA8 */ 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    /* 0xAC */ 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    /* 0xB0 */ 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    /* 0xB4 */ 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    /* 0xB8 */ 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    /* 0xBC */ 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    /* 0xC0 */ 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    /* 0xC4 */ 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    /* 0xC8 */ 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    /* 0xCC */ 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    /* 0xD0 */ 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    /* 0xD4 */ 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    /* 0xD8 */ 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    /* 0xDC */ 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    /* 0xE0 */ 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    /* 0xE4 */ 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    /* 0xE8 */ 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    /* 0xEC */ 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    /* 0xF0 */ 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    /* 0xF4 */ 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    /* 0xF8 */ 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    /* 0xFC */ 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

/* class CEcCRC32 */
EC_T_VOID CEcCRC32::AddData(EC_T_BYTE* pbyData, EC_T_DWORD dwLen)
{
    EC_T_DWORD  dwIdx = 0;
    EC_T_DWORD  dwTabIdx = 0;

    for (dwIdx = 0; dwIdx < dwLen; dwIdx++)
    {
        dwTabIdx = ((m_dwCrcSum ^ pbyData[dwIdx]) & 0x000000FF);
        m_dwCrcSum = (((m_dwCrcSum >> 8) & 0x00FFFFFF) ^ SC_adwCrcTab[dwTabIdx]);
    }
}
/*-IECDataStore----------------------------------------------------------------*/
class IECDataStore
{
public:
    IECDataStore()
    {
        m_oCrc32.Init();
        m_pConfig = EC_NULL;
    }
    IECDataStore(EC_PT_CONFIGPARM_DESC pConfig) :m_pConfig(pConfig){};
    virtual ~IECDataStore(){};

    virtual EC_T_PVOID  Open() = 0;
    virtual EC_T_INT    ReadData(EC_T_VOID* pvDst, EC_T_INT nLen) = 0;
    virtual EC_T_INT    Error() = 0;
    virtual EC_T_INT    EndOfData() = 0;
    virtual EC_T_INT    Close() = 0;

    virtual EC_T_DWORD  ChkSum(EC_T_VOID)
    {
        return m_oCrc32.GetSum();
    }

    virtual size_t    GetThisSize() = 0;
protected:
    CEcCRC32                m_oCrc32;

private:
    EC_PT_CONFIGPARM_DESC   m_pConfig;
};

class CEcFileStore : public IECDataStore
{
public:
    CEcFileStore(EC_PT_CONFIGPARM_DESC pConfig) :m_pConfig(pConfig)
    {
        OsDbgAssert(eCnfType_Filename == pConfig->eCnfType);
        m_hFile = EC_NULL;
    };
    virtual ~CEcFileStore()
    {
        this->Close();
        m_hFile = EC_NULL;
        m_pConfig = EC_NULL;
    }

    virtual EC_T_PVOID Open()
    {
        EC_T_PVOID pRet = EC_NULL;

        m_hFile = OsCfgFileOpen(((EC_T_CHAR*)m_pConfig->pbyData));

        if (EC_NULL != m_hFile)
        {
            pRet = this;
        }

        return pRet;
    }
    virtual EC_T_INT ReadData(EC_T_VOID* pvDst, EC_T_INT nLen)
    {
        EC_T_INT  nRead = 0;

        if (EC_NULL != m_hFile)
        {
            nRead = OsCfgFileRead(m_hFile, pvDst, nLen);
        }

        if ((m_pConfig->eCnfType != eCnfType_Datadiag) && (m_pConfig->eCnfType != eCnfType_GenPreopENI))
        {
            m_oCrc32.AddData((EC_T_PBYTE)pvDst, nRead);
        }

        return nRead;
    }
    virtual EC_T_INT Error()
    {
        EC_T_INT nErr = -1;

        if (EC_NULL != m_hFile)
        {
            nErr = OsCfgFileError(m_hFile);
        }

        return nErr;
    }
    virtual EC_T_INT EndOfData()
    {
        EC_T_INT nEOF = EC_TRUE;

        if (EC_NULL != m_hFile)
        {
            nEOF = OsCfgFileEof(m_hFile);
        }
        return nEOF;
    }
    virtual EC_T_INT Close()
    {
        EC_T_INT nRet = (EC_T_INT)EC_E_INVALIDSTATE;

        if (EC_NULL != m_hFile)
        {
            nRet = OsCfgFileClose(m_hFile);
            m_hFile = EC_NULL;
        }
        return nRet;
    }
    virtual size_t    GetThisSize() { return sizeof(*this); }

private:
    EC_PT_CONFIGPARM_DESC   m_pConfig;
    EC_T_PVOID              m_hFile;
};

class CEcMemStore : public IECDataStore
{
public:
    CEcMemStore(EC_PT_CONFIGPARM_DESC pConfig) :m_pConfig(pConfig)
    {
        OsDbgAssert((eCnfType_Data == pConfig->eCnfType) || (eCnfType_Datadiag == pConfig->eCnfType) || (eCnfType_GenPreopENI == m_pConfig->eCnfType)
            || (eCnfType_GenPreopENIWithCRC == pConfig->eCnfType) || (eCnfType_GenOpENI == pConfig->eCnfType));
        m_pbyCur = EC_NULL;
    };
    virtual ~CEcMemStore()
    {
        m_pbyCur = EC_NULL;
        m_pConfig = EC_NULL;
    }

    virtual EC_T_PVOID Open()
    {
        EC_T_PVOID pRet = EC_NULL;

        m_pbyCur = m_pConfig->pbyData;

        if (EC_NULL != m_pbyCur)
        {
            pRet = this;
        }

        return pRet;
    }
    virtual EC_T_INT ReadData(EC_T_VOID* pvDst, EC_T_INT nLen)
    {
        EC_T_INT    nRead = 0;
        EC_T_INT    nSpace = 0;

        if (EC_NULL == m_pbyCur)
        {
            nRead = 0;
            goto Exit;
        }

        nSpace = (EC_T_INT)(((m_pConfig->pbyData) + m_pConfig->dwLen) - (m_pbyCur));

        if (nSpace <= nLen)
        {
            nRead = nSpace;
        }

        if (nLen < nSpace)
        {
            nRead = nLen;
        }

        OsMemcpy(pvDst, m_pbyCur, nRead);
        if (nRead < nLen)
        {
            OsMemset((((EC_T_BYTE*)pvDst) + nRead), 0, (nLen - nRead));
        }

        m_pbyCur += nRead;

        if ((eCnfType_Datadiag == m_pConfig->eCnfType) || (eCnfType_GenPreopENI == m_pConfig->eCnfType))
        {
            m_oCrc32.SetSum(0);
        }
        else
        {
            m_oCrc32.AddData((EC_T_PBYTE)pvDst, nRead);
        }

    Exit:
        return nRead;
    }
    virtual EC_T_INT Error()
    {
        return EC_E_NOERROR;
    }
    virtual EC_T_INT EndOfData()
    {
        EC_T_INT nEOD = EC_TRUE;
        EC_T_INT nSpace = 0;

        if (EC_NULL == m_pbyCur)
        {
            nEOD = EC_TRUE;
            goto Exit;
        }

        nSpace = (EC_T_INT)(((m_pConfig->pbyData) + m_pConfig->dwLen) - (m_pbyCur));

        if (!(0 < nSpace))
        {
            nEOD = EC_TRUE;
        }
        else
        {
            nEOD = EC_FALSE;
        }

    Exit:
        return nEOD;
    }
    virtual EC_T_INT Close()
    {
        m_pbyCur = EC_NULL;
        return 0;
    }
    virtual size_t    GetThisSize() { return sizeof(*this); }

private:
    EC_PT_CONFIGPARM_DESC   m_pConfig;
    EC_T_PBYTE              m_pbyCur;
};

#if (defined INCLUDE_XPAT)
/*-LOCALS--------------------------------------------------------------------*/

/********************************************************************************/
/** \brief
*
* \return
*/
static EC_T_UINT XmlGetBinData( 
    const EC_T_CHAR*    szInpInit,   
    const EC_T_DWORD    dwInpLen,    
    EC_T_BYTE*          s, 
    EC_T_UINT           nLength )
{
    EC_T_UINT i, j = dwInpLen/2;     
    if ( nLength < j )
    {
        j = 0;
        goto Exit;
    }

    for ( i = 0; i < j; i++ )
    {
        EC_T_BYTE tmp = szInpInit[2*i+1];

        if ( tmp >= L'0' && tmp <= '9' )
        {
            s[i]  = (EC_T_BYTE)(tmp - L'0');
        }
        else if ( tmp >= L'a' && tmp <= 'f' )
        {
            s[i]  = (EC_T_BYTE)(10 + (EC_T_BYTE)(tmp - L'a'));
        }
        else if ( tmp >= L'A' && tmp <= 'F' )
        {
            s[i]  = (EC_T_BYTE)(10 + (EC_T_BYTE)(tmp - L'A'));
        }
        else
        {
            j = 0;
            goto Exit;
        }

        tmp = szInpInit[2*i];

        if ( tmp >= L'0' && tmp <= '9' )
        {
            s[i]  |= (EC_T_BYTE)(tmp - L'0') << 4;
        }
        else if ( tmp >= L'a' && tmp <= 'f' )
        {
            s[i]  |= (10 + (EC_T_BYTE)(tmp - L'a')) << 4;
        }
        else if ( tmp >= L'A' && tmp <= 'F' )
        {
            s[i]  |= (10 + (EC_T_BYTE)(tmp - L'A')) << 4;
        }
        else
        {
            j = 0;
            goto Exit;
        }
    }

Exit:
    return j;
}

/********************************************************************************/
/** \brief
*
* \return
*/
static EC_T_BOOL XmlGetBinDataNoAlloc( 
    EC_T_BYTE* s,
    const EC_T_CHAR*    szInpInit,   
    const EC_T_DWORD    dwInpLen)
{
    EC_T_BOOL   bOk = EC_TRUE;
    EC_T_UINT   len;
    EC_T_UINT   i = 0;

    len = dwInpLen/2;
    for ( i = 0; i < len; i++ )
    {
        EC_T_BYTE tmp = szInpInit[2*i+1];

        if ( tmp >= L'0' && tmp <= '9' )
        {
            s[i]  = (EC_T_BYTE)(tmp - L'0');
        }
        else if ( tmp >= L'a' && tmp <= 'f' )
        {
            s[i]  = (EC_T_BYTE)(10 + (EC_T_BYTE)(tmp - L'a'));
        }
        else if ( tmp >= L'A' && tmp <= 'F' )
        {
            s[i]  = (EC_T_BYTE)(10 + (EC_T_BYTE)(tmp - L'A'));
        }
        else
        {
            break;
        }

        tmp = szInpInit[2*i];

        if ( tmp >= L'0' && tmp <= '9' )
        {
            s[i]  |= (EC_T_BYTE)(tmp - L'0') << 4;
        }
        else if ( tmp >= L'a' && tmp <= 'f' )
        {
            s[i]  |= (10 + (EC_T_BYTE)(tmp - L'a')) << 4;
        }
        else if ( tmp >= L'A' && tmp <= 'F' )
        {
            s[i]  |= (10 + (EC_T_BYTE)(tmp - L'A')) << 4;
        }
        else
        {
            break;
        }
    }

    if ( i != len )
    {
        len = 0;
        bOk = EC_FALSE;
    }

    return bOk;
}


#ifdef INCLUDE_LOG_MESSAGES
const EC_T_CFG_STATE_DESCRIPTION    C_aCfgStateDescription[] =
{
    {eCfgState_Root,                                            "/"},
    {eCfgState_EcCfg,                                           "/EtherCATConfig"},
    {eCfgState_EcCfg_Cfg,                                       "/EtherCATConfig/Config"},

    {eCfgState_EcCfg_ProductKey,                                "/EtherCATConfig/ProductKey"},
    {eCfgState_EcCfg_Cfg_ProductKey,                            "/EtherCATConfig/Config/ProductKey"},

    {eCfgState_EcCfg_Cfg_Mas,                                   "/EtherCATConfig/Config/Master"},
    {eCfgState_EcCfg_Cfg_Mas_Info,                              "/EtherCATConfig/Config/Master/Info"},
    {eCfgState_EcCfg_Cfg_Mas_Info_Name,                         "/EtherCATConfig/Config/Master/Info/Name"},
    {eCfgState_EcCfg_Cfg_Mas_Info_Destination,                  "/EtherCATConfig/Config/Master/Info/Destination"},
    {eCfgState_EcCfg_Cfg_Mas_Info_Source,                       "/EtherCATConfig/Config/Master/Info/Source"},
    {eCfgState_EcCfg_Cfg_Mas_Info_EtherType,                    "/EtherCATConfig/Config/Master/Info/EtherType"},
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates,                     "/EtherCATConfig/Config/Master/MailboxStates"},
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates_StartAddr,           "/EtherCATConfig/Config/Master/MailboxStates/StartAddr"},
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates_Count,               "/EtherCATConfig/Config/Master/MailboxStates/Count"},
    {eCfgState_EcCfg_Cfg_Mas_EoE,                               "/EtherCATConfig/Config/Master/EoE"},
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxPorts,                      "/EtherCATConfig/Config/Master/EoE/MaxPorts"},
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxFrames,                     "/EtherCATConfig/Config/Master/EoE/MaxFrames"},
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxMACs,                       "/EtherCATConfig/Config/Master/EoE/MaxMACs"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds,                             "/EtherCATConfig/Config/Master/InitCmds"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        "/EtherCATConfig/Config/Master/InitCmds/InitCmd"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Transition,             "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Transition"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_BeforeSlave,            "/EtherCATConfig/Config/Master/InitCmds/InitCmd/BeforeSlave"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Comment,                "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Comment"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Requires,               "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Requires"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cmd,                    "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Cmd"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Adp,                    "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Adp"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Ado,                    "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Ado"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Addr,                   "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Addr"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Data,                   "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Data"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_DataLength,             "/EtherCATConfig/Config/Master/InitCmds/InitCmd/DataLength"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cnt,                    "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Cnt"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Retries,                "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Retries"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,               "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Validate"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Data,          "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Validate/Data"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_DataMask,      "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Validate/DataMask"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Timeout,       "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Validate/Timeout"},
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Timeout,                "/EtherCATConfig/Config/Master/InitCmds/InitCmd/Timeout"},
    {eCfgState_EcCfg_Cfg_Mas_Props,                             "/EtherCATConfig/Config/Master/Properties"},
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop,                        "/EtherCATConfig/Config/Master/Properties/Property"},
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop_Name,                   "/EtherCATConfig/Config/Master/Properties/Property/Name"},
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop_Value,                  "/EtherCATConfig/Config/Master/Properties/Property/Value"},

    {eCfgState_EcCfg_Cfg_Slv,                                       "/EtherCATConfig/Config/Slave"},
    {eCfgState_EcCfg_Cfg_Slv_Info,                                  "/EtherCATConfig/Config/Slave/Info"},
    {eCfgState_EcCfg_Cfg_Slv_Info_Name,                             "/EtherCATConfig/Config/Slave/Info/Name"},
    {eCfgState_EcCfg_Cfg_Slv_Info_PhysAddr,                         "/EtherCATConfig/Config/Slave/Info/PhysAddr"},
    {eCfgState_EcCfg_Cfg_Slv_Info_AutoIncAddr,                      "/EtherCATConfig/Config/Slave/Info/AutoIncAddr"},
    {eCfgState_EcCfg_Cfg_Slv_Info_Identification,                   "/EtherCATConfig/Config/Slave/Info/Identification"},
    {eCfgState_EcCfg_Cfg_Slv_Info_Identification_Ado,               "/EtherCATConfig/Config/Slave/Info/Identification/Ado"},
    {eCfgState_EcCfg_Cfg_Slv_Info_Physics,                          "/EtherCATConfig/Config/Slave/Info/Physics"},
    {eCfgState_EcCfg_Cfg_Slv_Info_VendorId,                         "/EtherCATConfig/Config/Slave/Info/VendorId"},
    {eCfgState_EcCfg_Cfg_Slv_Info_ProductCode,                      "/EtherCATConfig/Config/Slave/Info/ProductCode"},
    {eCfgState_EcCfg_Cfg_Slv_Info_RevisionNo,                       "/EtherCATConfig/Config/Slave/Info/RevisionNo"},
    {eCfgState_EcCfg_Cfg_Slv_Info_SerialNo,                         "/EtherCATConfig/Config/Slave/Info/SerialNo"},
    {eCfgState_EcCfg_Cfg_Slv_Info_ProductRevision,                  "/EtherCATConfig/Config/Slave/Info/ProductRevision"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                           "/EtherCATConfig/Config/Slave/ProcessData"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send,                      "/EtherCATConfig/Config/Slave/ProcessData/Send"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitStart,             "/EtherCATConfig/Config/Slave/ProcessData/Send/BitStart"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitLength,            "/EtherCATConfig/Config/Slave/ProcessData/Send/BitLength"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv,                      "/EtherCATConfig/Config/Slave/ProcessData/Recv"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitStart,             "/EtherCATConfig/Config/Slave/ProcessData/Recv/BitStart"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitLength,            "/EtherCATConfig/Config/Slave/ProcessData/Recv/BitLength"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm0"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm1"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm2"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm3"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm4"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm5"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm6"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                       "/EtherCATConfig/Config/Slave/ProcessData/Sm7"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                     "/EtherCATConfig/Config/Slave/ProcessData/RxPdo"},
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Index,               "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Index"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Exclude,             "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Exclude"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,               "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Entry"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Index,             "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Entry/Index"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Subindex,          "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Entry/SubIndex"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Bitlen,            "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Entry/BitLen"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype,          "/EtherCATConfig/Config/Slave/ProcessData/RxPdo/Entry/DataType"},
#endif
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                     "/EtherCATConfig/Config/Slave/ProcessData/TxPdo"},
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Index,               "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Index"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Name,                "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Name"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Exclude,             "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Exclude"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,               "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Entry"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Index,             "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Entry/Index"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Subindex,          "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Entry/SubIndex"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Bitlen,            "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Entry/BitLen"},
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype,          "/EtherCATConfig/Config/Slave/ProcessData/TxPdo/Entry/DataType"},
#endif
    {eCfgState_EcCfg_Cfg_Slv_Group,                                 "/EtherCATConfig/Config/Slave/Group"},

    {eCfgState_EcCfg_Cfg_Slv_Mbx,                                   "/EtherCATConfig/Config/Slave/Mailbox"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send,                              "/EtherCATConfig/Config/Slave/Mailbox/Send"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Start,                        "/EtherCATConfig/Config/Slave/Mailbox/Send/Start"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Length,                       "/EtherCATConfig/Config/Slave/Mailbox/Send/Length"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,                              "/EtherCATConfig/Config/Slave/Mailbox/Recv"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Start,                        "/EtherCATConfig/Config/Slave/Mailbox/Recv/Start"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Length,                       "/EtherCATConfig/Config/Slave/Mailbox/Recv/Length"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_PollTime,                     "/EtherCATConfig/Config/Slave/Mailbox/Recv/PollTime"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_StatusBitAddr,                "/EtherCATConfig/Config/Slave/Mailbox/Recv/StatusBitAddr"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Protocol,                          "/EtherCATConfig/Config/Slave/Mailbox/Protocol"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE,                               "/EtherCATConfig/Config/Slave/Mailbox/CoE"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds,                         "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                    "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Fixed,              "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Fixed"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_CompleteAccess,     "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/CompleteAccess"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Transition,         "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Transition"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Comment,            "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Comment"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Timeout,            "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Timeout"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Ccs,                "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Ccs"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Index,              "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Index"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_SubIndex,           "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/SubIndex"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Data,               "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Data"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Disabled,           "/EtherCATConfig/Config/Slave/Mailbox/CoE/ICmds/ICmd/Disabled"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_Prof,                          "/EtherCATConfig/Config/Slave/Mailbox/CoE/Profile"},

    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE,                               "/EtherCATConfig/Config/Slave/Mailbox/EoE"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds,                         "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                    "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Fixed,              "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd/Fixed"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Transition,         "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd/Transition"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment,            "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd/Comment"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Timeout,            "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd/Timeout"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Data,               "/EtherCATConfig/Config/Slave/Mailbox/EoE/ICmds/ICmd/Data"},


#if (defined INCLUDE_SOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE,                               "/EtherCATConfig/Config/Slave/Mailbox/SoE"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds,                         "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                    "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Fixed,              "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Fixed"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Transition,         "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Transition"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Comment,            "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Comment"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Timeout,            "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Timeout"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_OpCode,             "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/OpCode"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_DriveNo,            "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/DriveNo"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_IDN,                "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/IDN"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Elements,           "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Elements"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Attribute,          "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Attribute"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Data,               "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Data"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Disabled,           "/EtherCATConfig/Config/Slave/Mailbox/SoE/ICmds/ICmd/Disabled"},
#endif




#if (defined INCLUDE_FOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,                         "/EtherCATConfig/Config/Slave/Mailbox/BootStrap"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send,                    "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Send"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Start,              "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Send/Start"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Length,             "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Send/Length"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,                    "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Recv"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Start,              "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Recv/Start"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Length,             "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Recv/Length"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_PollTime,           "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Recv/PollTime"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_StatusBitAddr,      "/EtherCATConfig/Config/Slave/Mailbox/BootStrap/Recv/StatusBitAddr"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                               "/EtherCATConfig/Config/Slave/BootMailbox"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send,                          "/EtherCATConfig/Config/Slave/BootMailbox/Send"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Start,                    "/EtherCATConfig/Config/Slave/BootMailbox/Send/Start"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Length,                   "/EtherCATConfig/Config/Slave/BootMailbox/Send/Length"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,                          "/EtherCATConfig/Config/Slave/BootMailbox/Recv"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Start,                    "/EtherCATConfig/Config/Slave/BootMailbox/Recv/Start"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Length,                   "/EtherCATConfig/Config/Slave/BootMailbox/Recv/Length"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_PollTime,                 "/EtherCATConfig/Config/Slave/BootMailbox/Recv/PollTime"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_StatusBitAddr,            "/EtherCATConfig/Config/Slave/BootMailbox/Recv/StatusBitAddr"},
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Protocol,                      "/EtherCATConfig/Config/Slave/BootMailbox/Protocol"},
#endif
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout,                           "/EtherCATConfig/Config/Slave/Mailbox/Timeout"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_RetReq,                    "/EtherCATConfig/Config/Slave/Mailbox/Timeout/ReturningRequest"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_Response,                  "/EtherCATConfig/Config/Slave/Mailbox/Timeout/Response"},
#if (defined INCLUDE_AOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE,                               "/EtherCATConfig/Config/Slave/Mailbox/AoE"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_NetId,                         "/EtherCATConfig/Config/Slave/Mailbox/AoE/NetId"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds,                         "/EtherCATConfig/Config/Slave/Mailbox/AoE/ICmds"},
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                    "/EtherCATConfig/Config/Slave/Mailbox/AoE/ICmds/ICmd"},
#endif

    {eCfgState_EcCfg_Cfg_Slv_ICmds,                                 "/EtherCATConfig/Config/Slave/ICmds"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                            "/EtherCATConfig/Config/Slave/ICmds/ICmd"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Transition,                 "/EtherCATConfig/Config/Slave/ICmds/ICmd/Transition"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_BeforeSlave,                "/EtherCATConfig/Config/Slave/ICmds/ICmd/BeforeSlave"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment,                    "/EtherCATConfig/Config/Slave/ICmds/ICmd/Comment"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Requires,                   "/EtherCATConfig/Config/Slave/ICmds/ICmd/Requires"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cmd,                        "/EtherCATConfig/Config/Slave/ICmds/ICmd/Cmd"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Adp,                        "/EtherCATConfig/Config/Slave/ICmds/ICmd/Adp"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Ado,                        "/EtherCATConfig/Config/Slave/ICmds/ICmd/Ado"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Addr,                       "/EtherCATConfig/Config/Slave/ICmds/ICmd/Addr"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Data,                       "/EtherCATConfig/Config/Slave/ICmds/ICmd/Data"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_DataLength,                 "/EtherCATConfig/Config/Slave/ICmds/ICmd/DataLength"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cnt,                        "/EtherCATConfig/Config/Slave/ICmds/ICmd/Cnt"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Retries,                    "/EtherCATConfig/Config/Slave/ICmds/ICmd/Retries"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,                   "/EtherCATConfig/Config/Slave/ICmds/ICmd/Validate"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Data,              "/EtherCATConfig/Config/Slave/ICmds/ICmd/Validate/Data"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_DataMask,          "/EtherCATConfig/Config/Slave/ICmds/ICmd/Validate/DataMask"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Timeout,           "/EtherCATConfig/Config/Slave/ICmds/ICmd/Validate/Timeout"},
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Timeout,                    "/EtherCATConfig/Config/Slave/ICmds/ICmd/Timeout"},

    {eCfgState_EcCfg_Cfg_Slv_PreviousPort,                          "/EtherCATConfig/Config/Slave/PreviousPort"},

    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_Port,                     "/EtherCATConfig/Config/Slave/PreviousPort/Port"},
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_PhysAddr,                 "/EtherCATConfig/Config/Slave/PreviousPort/PhysAddr"},
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_Selected,                 "/EtherCATConfig/Config/Slave/PreviousPort/Selected"},

    {eCfgState_EcCfg_Cfg_Slv_HotConnect,                            "/EtherCATConfig/Config/Slave/HotConnect"},
#if (defined INCLUDE_HOTCONNECT)
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupName,                  "/EtherCATConfig/Config/Slave/HotConnect/GroupName"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupMemberCnt,             "/EtherCATConfig/Config/Slave/HotConnect/GroupMemberCnt"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,                "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd"},

    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Comment,              "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Comment"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Requires,             "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Requires"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cmd,                  "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Cmd"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Adp,                  "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Adp"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Ado,                  "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Ado"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Data,                 "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Data"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_DataLength,           "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/DataLength"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cnt,                  "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Cnt"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Retries,              "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Retries"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,             "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Validate"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Data,        "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Validate/Data"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_DataMask,    "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Validate/DataMask"},
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Timeout,     "/EtherCATConfig/Config/Slave/HotConnect/IdentifyCmd/Validate/Timeout"},
#endif

    {eCfgState_EcCfg_Cfg_Cyclic,                                    "/EtherCATConfig/Config/Cyclic"},
    {eCfgState_EcCfg_Cfg_Cyclic_Comment,                            "/EtherCATConfig/Config/Cyclic/Comment"},
    {eCfgState_EcCfg_Cfg_Cyclic_CycleTime,                          "/EtherCATConfig/Config/Cyclic/CycleTime"},
    {eCfgState_EcCfg_Cfg_Cyclic_Priority,                           "/EtherCATConfig/Config/Cyclic/Priority"},
    {eCfgState_EcCfg_Cfg_Cyclic_TaskId,                             "/EtherCATConfig/Config/Cyclic/TaskId"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame,                              "/EtherCATConfig/Config/Cyclic/Frame"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Comment,                      "/EtherCATConfig/Config/Cyclic/Frame/Comment"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                          "/EtherCATConfig/Config/Cyclic/Frame/Cmd"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_State,                    "/EtherCATConfig/Config/Cyclic/Frame/Cmd/State"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Comment,                  "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Comment"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cmd,                      "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Cmd"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Adp,                      "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Adp"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Ado,                      "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Ado"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Addr,                     "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Addr"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Data,                     "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Data"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_DataLength,               "/EtherCATConfig/Config/Cyclic/Frame/Cmd/DataLength"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cnt,                      "/EtherCATConfig/Config/Cyclic/Frame/Cmd/Cnt"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_InputOffs,                "/EtherCATConfig/Config/Cyclic/Frame/Cmd/InputOffs"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_OutputOffs,               "/EtherCATConfig/Config/Cyclic/Frame/Cmd/OutputOffs"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos,                   "/EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,             "/EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos/CopyInfo"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Src,         "/EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos/CopyInfo/SrcBitOffs"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Dst,         "/EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos/CopyInfo/DstBitOffs"},
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Size,        "/EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos/CopyInfo/BitSize"},

    {eCfgState_EcCfg_Cfg_ProcessImage,                              "/EtherCATConfig/Config/ProcessImage"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs,                       "/EtherCATConfig/Config/ProcessImage/Inputs"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_ByteSize,              "/EtherCATConfig/Config/ProcessImage/Inputs/ByteSize"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,              "/EtherCATConfig/Config/ProcessImage/Inputs/Variable"},
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Name,         "/EtherCATConfig/Config/ProcessImage/Inputs/Variable/Name"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Comment,      "/EtherCATConfig/Config/ProcessImage/Inputs/Variable/Comment"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_DataType,    	"/EtherCATConfig/Config/ProcessImage/Inputs/Variable/DataType"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitSize,      "/EtherCATConfig/Config/ProcessImage/Inputs/Variable/BitSize"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitOffs,      "/EtherCATConfig/Config/ProcessImage/Inputs/Variable/BitOffs"},
#endif
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs,                      "/EtherCATConfig/Config/ProcessImage/Outputs"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_ByteSize,             "/EtherCATConfig/Config/ProcessImage/Outputs/ByteSize"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,             "/EtherCATConfig/Config/ProcessImage/Outputs/Variable"},
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Name,        "/EtherCATConfig/Config/ProcessImage/Outputs/Variable/Name"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Comment,     "/EtherCATConfig/Config/ProcessImage/Outputs/Variable/Comment"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_DataType,   	"/EtherCATConfig/Config/ProcessImage/Outputs/Variable/DataType"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitSize,     "/EtherCATConfig/Config/ProcessImage/Outputs/Variable/BitSize"},
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitOffs,     "/EtherCATConfig/Config/ProcessImage/Outputs/Variable/BitOffs"},
#endif

#if (defined INCLUDE_OEM)
    {eCfgState_EcCfg_Manufacturer,                                  "/EtherCATConfig/Manufacturer"},
#endif
};
static EC_T_DWORD S_dwNumCfgStateDescriptionEntries = sizeof(C_aCfgStateDescription)/sizeof(EC_T_CFG_STATE_DESCRIPTION);

const EC_T_CHAR* CfgStateDescription(EC_T_CFG_PARSER_STATE ECfgParserState)
{
EC_T_DWORD dwCfgStateDescriptionEntry;
EC_T_BOOL bTagFound = EC_FALSE;
EC_T_CHAR* szDescription = EC_NULL;

    for( dwCfgStateDescriptionEntry = 0; dwCfgStateDescriptionEntry < S_dwNumCfgStateDescriptionEntries; dwCfgStateDescriptionEntry++ )
    {
        if (C_aCfgStateDescription[dwCfgStateDescriptionEntry].CfgParserState == ECfgParserState)
        {
            szDescription = (EC_T_CHAR*)C_aCfgStateDescription[dwCfgStateDescriptionEntry].szDescription;
            bTagFound = EC_TRUE;
            break;
        }
    }
    if (!bTagFound)
    {
        szDescription = (EC_T_CHAR*)"Unknown Tag";
    }
    return szDescription;
}
#endif

const EC_T_CFG_STATE_CHANGE_DESC    C_aCfgStateChangeDesc[] =
{
    {eCfgState_Root,                                            eCfgState_EcCfg,    eSnglTg,    eTgSupp,    C_szTgEtherCATConfig    },
#if (defined INCLUDE_OEM)
    {eCfgState_Root,                                            eCfgState_EcCfg,    eSnglTg,    eTgSupp,    C_szTgEncryptedEtherCATConfig    },
    {eCfgState_EcCfg,                                           eCfgState_EcCfg_Manufacturer,  eFrstTg,    eTgSupp,    C_szTgManufacturer  },
#endif
    {eCfgState_EcCfg,                                           eCfgState_EcCfg_Cfg,    eFrstTg,    eTgSupp,    C_szTgConfig    },
    {eCfgState_EcCfg,                                           eCfgState_EcCfg_ProductKey,    eLstTg,    eTgSupp,    C_szTgProductKey    },
    {eCfgState_EcCfg_Cfg,                                       eCfgState_EcCfg_Cfg_Mas,    eFrstTg,    eTgSupp,    C_szTgMaster    },
    {eCfgState_EcCfg_Cfg,                                       eCfgState_EcCfg_Cfg_Slv,    eMdlTg,    eTgSupp,    C_szTgSlave    },
    {eCfgState_EcCfg_Cfg,                                       eCfgState_EcCfg_Cfg_Cyclic,    eMdlTg,    eTgSupp,    C_szTgCyclic    },
    {eCfgState_EcCfg_Cfg,                                       eCfgState_EcCfg_Cfg_ProcessImage,    eMdlTg,    eTgSupp,    C_szTgProcessImage    },
    {eCfgState_EcCfg_Cfg,                                       eCfgState_EcCfg_Cfg_ProductKey,    eLstTg,    eTgSupp,    C_szTgProductKey    },

    /* ProductKey */
    {eCfgState_EcCfg_ProductKey,                                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProductKey,                            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    /* /Config/Master */
    {eCfgState_EcCfg_Cfg_Mas,                                   eCfgState_EcCfg_Cfg_Mas_Info,    eFrstTg,    eTgSupp,    C_szTgInfo    },
    {eCfgState_EcCfg_Cfg_Mas,                                   eCfgState_EcCfg_Cfg_Mas_MailboxStates,    eMdlTg,    eTgSupp,    C_szTgMailboxStates    },
    {eCfgState_EcCfg_Cfg_Mas,                                   eCfgState_EcCfg_Cfg_Mas_EoE,    eMdlTg,    eTgSupp,    C_szTgEoE    },
    {eCfgState_EcCfg_Cfg_Mas,                                   eCfgState_EcCfg_Cfg_Mas_ICmds,    eMdlTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Mas,                                   eCfgState_EcCfg_Cfg_Mas_Props,    eLstTg,    eTgSupp,    C_szTgProperties    },
    {eCfgState_EcCfg_Cfg_Mas_Info,                              eCfgState_EcCfg_Cfg_Mas_Info_Name,    eFrstTg,    eTgSupp,    C_szTgName    },
    {eCfgState_EcCfg_Cfg_Mas_Info,                              eCfgState_EcCfg_Cfg_Mas_Info_Destination,    eMdlTg,    eTgSupp,    C_szTgDestination    },
    {eCfgState_EcCfg_Cfg_Mas_Info,                              eCfgState_EcCfg_Cfg_Mas_Info_Source,    eMdlTg,    eTgSupp,    C_szTgSource    },
    {eCfgState_EcCfg_Cfg_Mas_Info,                              eCfgState_EcCfg_Cfg_Mas_Info_EtherType,    eLstTg,    eTgSupp,    C_szTgEtherType    },
    {eCfgState_EcCfg_Cfg_Mas_Info_Name,                         eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_Info_Destination,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_Info_Source,                       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_Info_EtherType,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates,                     eCfgState_EcCfg_Cfg_Mas_MailboxStates_StartAddr,    eFrstTg,    eTgSupp,    C_szTgStartAddr    },
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates,                     eCfgState_EcCfg_Cfg_Mas_MailboxStates_Count,    eLstTg,    eTgSupp,    C_szTgCount    },
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates_StartAddr,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_MailboxStates_Count,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_EoE,                               eCfgState_EcCfg_Cfg_Mas_EoE_MaxPorts,    eFrstTg,    eTgSupp,    C_szTgMaxPorts    },
    {eCfgState_EcCfg_Cfg_Mas_EoE,                               eCfgState_EcCfg_Cfg_Mas_EoE_MaxFrames,    eMdlTg,    eTgSupp,    C_szTgMaxFrames    },
    {eCfgState_EcCfg_Cfg_Mas_EoE,                               eCfgState_EcCfg_Cfg_Mas_EoE_MaxMACs,    eLstTg,    eTgSupp,    C_szTgMaxMACs    },
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxPorts,                      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxFrames,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_EoE_MaxMACs,                       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds,                             eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,             eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Transition,  eFrstTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_BeforeSlave, eMdlTg,     eTgSupp,    C_szTgBeforeSlave    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Comment,     eMdlTg,     eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Requires,    eMdlTg,     eTgSupp,    C_szTgRequires    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cmd,         eMdlTg,     eTgSupp,    C_szTgCmd    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Adp,         eMdlTg,     eTgSupp,    C_szTgAdp    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Ado,         eMdlTg,     eTgSupp,    C_szTgAdo    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Addr,        eMdlTg,     eTgSupp,    C_szTgAddr   },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Data,        eMdlTg,     eTgSupp,    C_szTgData   },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_DataLength,  eMdlTg,     eTgSupp,    C_szTgDataLength },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cnt,         eMdlTg,     eTgSupp,    C_szTgCnt        },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Retries,     eMdlTg,     eTgSupp,    C_szTgRetries    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,    eLstTg,     eTgSupp,    C_szTgRetries    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Timeout,     eLstTg,     eTgSupp,    C_szTgRetries    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Transition,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_BeforeSlave,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Comment,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Requires,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cmd,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Adp,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Ado,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Addr,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Data,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_DataLength,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cnt,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Retries,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Data,    eFrstTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_DataMask,    eMdlTg,    eTgSupp,    C_szTgDataMask    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Timeout,    eLstTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Data,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_DataMask,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Timeout,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Timeout,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_Props,                             eCfgState_EcCfg_Cfg_Mas_Props_Prop,    eSnglTg,    eTgSupp,    "Property"    },
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop,                        eCfgState_EcCfg_Cfg_Mas_Props_Prop_Name,    eFrstTg,    eTgSupp,    "Name"    },
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop,                        eCfgState_EcCfg_Cfg_Mas_Props_Prop_Value,    eLstTg,    eTgSupp,    "Value"    },
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop_Name,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Mas_Props_Prop_Value,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    /*/Config/Slave */
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_Info,    eFrstTg,    eTgSupp,    C_szTgInfo    },
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_ProcessData,    eMdlTg,    eTgSupp,    C_szTgProcessData    },
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_Mbx,    eMdlTg,    eTgSupp,    C_szTgMailbox    },
#if (defined INCLUDE_FOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_BootMbx,    eMdlTg,    eTgSupp,    C_szTgBootMailbox    },
#endif
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_ICmds,    eMdlTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_PreviousPort,    eMdlTg,    eTgSupp,    C_szTgPreviousPort    },
    {eCfgState_EcCfg_Cfg_Slv,                                    eCfgState_EcCfg_Cfg_Slv_DC,    eLstTg,    eTgSupp,    C_szTgDC    },
                                            
    {eCfgState_EcCfg_Cfg_Slv,                                   eCfgState_EcCfg_Cfg_Slv_Group,    eSnglTg,    eTgSupp,    C_szTgGroup    },
                                            
    {eCfgState_EcCfg_Cfg_Slv,                                   eCfgState_EcCfg_Cfg_Slv_HotConnect,    eMdlTg,    eTgSupp,    C_szTgHotConnect    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_Name,    eFrstTg,    eTgSupp,    C_szTgName    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_PhysAddr,    eFrstTg,    eTgSupp,    C_szTgPhysAddr    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_AutoIncAddr,    eMdlTg,    eTgSupp,    C_szTgAutoIncAddr    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_Identification,    eFrstTg,    eTgSupp,    C_szTgIdentification    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_Physics,    eMdlTg,    eTgSupp,    C_szTgPhysics    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_VendorId,    eMdlTg,    eTgSupp,    C_szTgVendorId    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_ProductCode,    eMdlTg,    eTgSupp,    C_szTgProductCode    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_RevisionNo,    eMdlTg,    eTgSupp,    C_szTgRevisionNo    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_SerialNo,    eMdlTg,    eTgSupp,    C_szTgSerialNo    },
    {eCfgState_EcCfg_Cfg_Slv_Info,                              eCfgState_EcCfg_Cfg_Slv_Info_ProductRevision,    eLstTg,    eTgSupp,    C_szTgProductRevision    },
    {eCfgState_EcCfg_Cfg_Slv_Info_Name,                         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_PhysAddr,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_AutoIncAddr,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_Identification,               eCfgState_EcCfg_Cfg_Slv_Info_Identification_Ado, eFrstTg, eTgSupp, C_szTgAdo },
    {eCfgState_EcCfg_Cfg_Slv_Info_Identification_Ado,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_Physics,                      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_VendorId,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_ProductCode,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_RevisionNo,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_SerialNo,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Info_ProductRevision,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Send,    eFrstTg,    eTgSupp,    C_szTgSend    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv,    eMdlTg,    eTgSupp,    C_szTgRecv    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,    eMdlTg,    eTgSupp,    C_szTgSm0    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,    eMdlTg,    eTgSupp,    C_szTgSm1    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,    eMdlTg,    eTgSupp,    C_szTgSm2    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,    eMdlTg,    eTgSupp,    C_szTgSm3    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,    eMdlTg,    eTgSupp,    C_szTgSm4    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,    eMdlTg,    eTgSupp,    C_szTgSm5    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,    eMdlTg,    eTgSupp,    C_szTgSm6    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,    eMdlTg,    eTgSupp,    C_szTgSm7    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,    eMdlTg,    eTgSupp,    C_szTgRxPdo    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData,                       eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,    eLstTg,    eTgSupp,    C_szTgTxPdo    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send,                  eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitStart,    eFrstTg,    eTgSupp,    C_szTgBitStart    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send,                  eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitLength,    eLstTg,    eTgSupp,    C_szTgBitLength    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitStart,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitLength,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv,                  eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitStart,    eFrstTg,    eTgSupp,    C_szTgBitStart    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv,                  eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitLength,   eLstTg,     eTgSupp,    C_szTgBitLength    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitStart,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitLength,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Type,           eSnglTg,    eTgSupp,    "Type"          },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MinSize,        eSnglTg,    eTgSupp,    "MinSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MaxSize,        eSnglTg,    eTgSupp,    "MaxSize"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_DefaultSize,    eSnglTg,    eTgSupp,    "DefaultSize"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_StartAddress,   eSnglTg,    eTgSupp,    "StartAddress"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_ControlByte,    eSnglTg,    eTgSupp,    "ControlByte"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Virtual,        eSnglTg,    eTgSupp,    "Virtual"       },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Enable,         eSnglTg,    eTgSupp,    "Enable"        },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Watchdog,       eSnglTg,    eTgSupp,    "Watchdog"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7,                   eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Pdo,            eSnglTg,    eTgSupp,    "Pdo"           },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Type,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MinSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MaxSize,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_DefaultSize,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_StartAddress,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_ControlByte,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Virtual,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Enable,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Watchdog,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Pdo,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Index,     eFrstTg,   eTgSupp,    "Index"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Name,      eMdlTg,    eTgSupp,    "Name"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Exclude,   eMdlTg,    eTgSupp,    "Exclude"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,     eLstTg,    eTgSupp,    "Entry"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Index,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Name,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Exclude,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Index,   eFrstTg,   eTgSupp,    "Index"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Subindex,eMdlTg,    eTgSupp,    "SubIndex"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Bitlen,  eMdlTg,    eTgSupp,    "BitLen"    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Name,    eMdlTg,    eTgSupp,    "Name"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Comment, eMdlTg,    eTgSupp,    "Comment"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype,eLstTg,    eTgSupp,    "DataType"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Index,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Subindex,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Bitlen,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Name,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp, EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Comment,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp, EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Index,     eFrstTg,   eTgSupp,    "Index"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Name,      eLstTg,    eTgSupp,    "Name"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Exclude,   eMdlTg,    eTgSupp,    "Exclude"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                 eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,     eLstTg,    eTgSupp,    "Entry"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Index,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Name,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Exclude,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Index,   eFrstTg,   eTgSupp,    "Index"     },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Subindex,eMdlTg,    eTgSupp,    "SubIndex"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Bitlen,  eMdlTg,    eTgSupp,    "BitLen"    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Name,    eMdlTg,    eTgSupp,    "Name"      },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Comment, eMdlTg,    eTgSupp,    "Comment"   },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry,           eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype,eLstTg,    eTgSupp,    "DataType"  },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Index,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Subindex,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Bitlen,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Name,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp, EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Comment,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp, EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#else
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo,                 eCfgState_ERROR,    eFrstTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo,                 eCfgState_ERROR,    eFrstTg,    eTgNotSupp,    EC_NULL    },
#endif
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_Send,    eFrstTg,    eTgSupp,    C_szTgSend    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,    eMdlTg,    eTgSupp,    C_szTgRecv    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_Protocol,    eMdlTg,    eTgSupp,    C_szTgProtocol    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout,    eMdlTg,    eTgSupp,    C_szTgTimeout    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_VoE,    eMdlTg,    eTgSupp,    C_szTgVoE    },

#if (defined INCLUDE_AOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_AoE,    eMdlTg,    eTgSupp,    C_szTgAoE    },
#endif
                                            
#if (defined INCLUDE_FOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,    eMdlTg,    eTgSupp,    C_szTgBootStrap    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,    eMdlTg,    eTgSupp,    C_szTgBootstrap    },
#endif
                                            
#if (defined INCLUDE_SOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_SoE,    eMdlTg,    eTgSupp,    C_szTgSoE    },
#endif
                                            
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_EoE,    eMdlTg,    eTgSupp,    C_szTgEoE    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_CoE,    eMdlTg,    eTgSupp,    C_szTgCoE    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx,                               eCfgState_EcCfg_Cfg_Slv_Mbx_FoE,    eLstTg,    eTgSupp,    C_szTgFoE    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Length,    eLstTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Start,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Length,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Length,    eMdlTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_PollTime,    eMdlTg,    eTgSupp,    C_szTgPollTime    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv,                          eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_StatusBitAddr,    eLstTg,    eTgSupp,    C_szTgStatusBitAddr    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Start,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Length,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_PollTime,                 eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_StatusBitAddr,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Protocol,                      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds,    eFrstTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_Prof,    eLstTg,    eTgSupp,    "Profile"    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds,                     eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,    eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Fixed,    eFrstTg,    eTgSupp,    C_szTgFixed    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_CompleteAccess,    eMdlTg,    eTgSupp,    C_szTgCompleteAccess    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Transition,    eMdlTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Comment,    eMdlTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Timeout,    eMdlTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Ccs,    eMdlTg,    eTgSupp,    C_szTgCcs    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Index,    eMdlTg,    eTgSupp,    C_szTgIndex    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_SubIndex,    eMdlTg,    eTgSupp,    C_szTgSubIndex    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Data,    eMdlTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Disabled,    eLstTg,    eTgSupp,    C_szTgDisabled    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Fixed,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_CompleteAccess, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Transition,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Comment,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Timeout,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Ccs,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Index,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_SubIndex,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Data,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Disabled,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_Prof,                      eCfgState_ERROR,    eSnglTg,    eTgNotSupp,    EC_NULL    },
#if (defined INCLUDE_FOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,                     eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send,    eFrstTg,    eTgSupp,    C_szTgSend    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap,                     eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,    eMdlTg,    eTgSupp,    C_szTgRecv    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Length,    eLstTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Start,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Length,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Length,    eMdlTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_PollTime,    eMdlTg,    eTgSupp,    C_szTgPollTime    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv,                eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_StatusBitAddr,    eLstTg,    eTgSupp,    C_szTgStatusBitAddr    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Start,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Length,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_PollTime,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_StatusBitAddr,  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#endif
#if (defined INCLUDE_SOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds,    eSnglTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds,                     eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,    eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Fixed,    eFrstTg,    eTgSupp,    C_szTgFixed    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Transition,    eMdlTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Comment,    eMdlTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Timeout,    eMdlTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_OpCode,    eMdlTg,    eTgSupp,    C_szTgOpCode    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_DriveNo,    eMdlTg,    eTgSupp,    C_szTgDriveNo    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_IDN,    eMdlTg,    eTgSupp,    C_szTgIDN    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Elements,    eMdlTg,    eTgSupp,    C_szTgElements    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Attribute,    eMdlTg,    eTgSupp,    C_szTgAttribute    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Data,    eMdlTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Disabled,    eLstTg,    eTgSupp,    C_szTgDisabled    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Fixed,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Transition,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Comment,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Timeout,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_OpCode,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_DriveNo,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_IDN,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Elements,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Attribute,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Data,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Disabled,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#endif                                            
#if (defined INCLUDE_AOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds,    eFrstTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_NetId,    eSnglTg,    eTgSupp,    C_szTgNetId    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds,                     eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,    eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Fixed,    eFrstTg,    eTgSupp,    C_szTgFixed    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Transition,    eMdlTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Comment,    eMdlTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Timeout,    eMdlTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Data,    eMdlTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Fixed,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Transition,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Comment,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Timeout,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Data,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_NetId,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#endif
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE,                           eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds,    eSnglTg,    eTgSupp,    C_szTgInitCmds    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds,                     eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,    eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment,    eFrstTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Transition,    eMdlTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment,    eMdlTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Timeout,    eMdlTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd,                eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Data,    eLstTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Fixed,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Transition,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Timeout,        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Data,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#if (defined INCLUDE_FOE_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                           eCfgState_EcCfg_Cfg_Slv_BootMbx_Send,    eFrstTg,    eTgSupp,    C_szTgSend    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                           eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,    eMdlTg,    eTgSupp,    C_szTgRecv    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                           eCfgState_EcCfg_Cfg_Slv_BootMbx_Protocol,    eMdlTg,    eTgSupp,    C_szTgProtocol    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                           eCfgState_EcCfg_Cfg_Slv_BootMbx_CoE,    eLstTg,    eTgSupp,    C_szTgCoE    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx,                           eCfgState_EcCfg_Cfg_Slv_BootMbx_FoE,    eLstTg,    eTgSupp,    C_szTgFoE    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_CoE,                       eCfgState_ERROR,    eSnglTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_FoE,                       eCfgState_ERROR,    eSnglTg,    eTgNotSupp,    EC_NULL    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Length,    eLstTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Start,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Length,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Start,    eFrstTg,    eTgSupp,    C_szTgStart    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Length,    eMdlTg,    eTgSupp,    C_szTgLength    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_PollTime,    eMdlTg,    eTgSupp,    C_szTgPollTime    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv,                      eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_StatusBitAddr,    eLstTg,    eTgSupp,    C_szTgStatusBitAddr    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Start,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Length,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_PollTime,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_StatusBitAddr,        eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_BootMbx_Protocol,                  eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
#endif
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout,                       eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_RetReq,    eFrstTg,    eTgSupp,    C_szTgRetReq    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout,                       eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_Response,    eLstTg,    eTgSupp,    C_szTgResponse    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_RetReq,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_Response,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },

    {eCfgState_EcCfg_Cfg_Slv_ICmds,                             eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                     eSnglTg,    eTgSupp,    C_szTgInitCmd    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Transition,          eFrstTg,    eTgSupp,    C_szTgTransition    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_BeforeSlave,         eMdlTg,     eTgSupp,    C_szTgBeforeSlave    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment,             eMdlTg,     eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Requires,            eMdlTg,     eTgSupp,    C_szTgRequires    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cmd,                 eMdlTg,     eTgSupp,    C_szTgCmd    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Adp,                 eMdlTg,     eTgSupp,    C_szTgAdp    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Ado,                 eMdlTg,     eTgSupp,    C_szTgAdo    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Addr,                eMdlTg,     eTgSupp,    C_szTgAddr    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Data,                eMdlTg,     eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_DataLength,          eMdlTg,     eTgSupp,    C_szTgDataLength    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cnt,                 eMdlTg,     eTgSupp,    C_szTgCnt    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Retries,             eMdlTg,     eTgSupp,    C_szTgRetries    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,            eLstTg,     eTgSupp,    C_szTgValidate    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd,                        eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Timeout,             eLstTg,     eTgSupp,    C_szTgTimeout     },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Transition,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_BeforeSlave,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Requires,               eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cmd,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Adp,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Ado,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Addr,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Data,                   eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_DataLength,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cnt,                    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Retries,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Data,       eFrstTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_DataMask,   eMdlTg,     eTgSupp,    C_szTgDataMask    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate,               eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Timeout,    eLstTg,     eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Data,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_DataMask,      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Timeout,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Timeout,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#if (defined INCLUDE_DC_SUPPORT)
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClock,    eSnglTg,    eTgSupp,    C_szTgReferenceClock    },
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClockP,    eSnglTg,    eTgSupp,    C_szTgReferenceClockP    },
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_EcCfg_Cfg_Slv_DC_CycleTime0,    eSnglTg,    eTgSupp,    C_szTgCycleTime0    },
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_EcCfg_Cfg_Slv_DC_CycleTime1,    eSnglTg,    eTgSupp,    C_szTgCycleTime1    },
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_EcCfg_Cfg_Slv_DC_ShiftTime,    eSnglTg,    eTgSupp,    C_szTgShiftTime    },
    {eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClock,                 eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClockP,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_DC_CycleTime0,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_DC_CycleTime1,                     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_DC_ShiftTime,                      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#else
    {eCfgState_EcCfg_Cfg_Slv_DC,                                eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp,    EC_NULL    },
#endif
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort,                      eCfgState_EcCfg_Cfg_Slv_PreviousPort_Port,    eSnglTg,    eTgSupp,    C_szPPPort    },
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort,                      eCfgState_EcCfg_Cfg_Slv_PreviousPort_PhysAddr,    eSnglTg,    eTgSupp,    C_szPPPhysAddr    },
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort,                      eCfgState_EcCfg_Cfg_Slv_PreviousPort_Selected,    eSnglTg,    eTgSupp,    "Selected"    },
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_Port,                 eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_PhysAddr,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_PreviousPort_Selected,             eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#if (defined INCLUDE_HOTCONNECT)
    {eCfgState_EcCfg_Cfg_Slv_HotConnect,                        eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupName,    eSnglTg,    eTgSupp,    C_szHCGroupName    },              
    {eCfgState_EcCfg_Cfg_Slv_HotConnect,                        eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupMemberCnt,    eSnglTg,    eTgSupp,    C_szHCGroupMemberCnt    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect,                        eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,    eSnglTg,    eTgSupp,    C_szHCIdentCommand    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupName,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupMemberCnt,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
                                            
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Comment,    eFrstTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Requires,    eMdlTg,    eTgSupp,    C_szTgRequires    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cmd,    eMdlTg,    eTgSupp,    C_szTgCmd    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Adp,    eMdlTg,    eTgSupp,    C_szTgAdp    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Ado,    eMdlTg,    eTgSupp,    C_szTgAdo    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Data,    eMdlTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_DataLength,    eMdlTg,    eTgSupp,    C_szTgDataLength    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cnt,    eMdlTg,    eTgSupp,    C_szTgCnt    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Retries,    eMdlTg,    eTgSupp,    C_szTgRetries    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd,            eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,    eLstTg,    eTgSupp,    C_szTgValidate    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Comment,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Requires,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cmd,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Adp,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Ado,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_DataLength,       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cnt,              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Retries,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,         eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Data,    eFrstTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,         eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_DataMask,    eMdlTg,    eTgSupp,    C_szTgDataMask    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate,         eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Timeout,    eLstTg,    eTgSupp,    C_szTgTimeout    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Data,    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_DataMask,eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Timeout, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#else
    {eCfgState_EcCfg_Cfg_Slv_HotConnect,                        eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
#endif
    /*/Config/Cyclic */
    {eCfgState_EcCfg_Cfg_Cyclic,                                eCfgState_EcCfg_Cfg_Cyclic_Comment,    eFrstTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Cyclic,                                eCfgState_EcCfg_Cfg_Cyclic_CycleTime,    eMdlTg,    eTgSupp,    C_szCycleTime    },
    {eCfgState_EcCfg_Cfg_Cyclic,                                eCfgState_EcCfg_Cfg_Cyclic_Priority,    eMdlTg,    eTgSupp,    C_szPriority    },
    {eCfgState_EcCfg_Cfg_Cyclic,                                eCfgState_EcCfg_Cfg_Cyclic_TaskId,    eMdlTg,    eTgSupp,    C_szTaskId    },
    {eCfgState_EcCfg_Cfg_Cyclic,                                eCfgState_EcCfg_Cfg_Cyclic_Frame,    eLstTg,    eTgSupp,    C_szFrame    },
    {eCfgState_EcCfg_Cfg_Cyclic_Comment,                        eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_CycleTime,                      eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Priority,                       eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_TaskId,                         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame,                          eCfgState_EcCfg_Cfg_Cyclic_Frame_Comment,    eFrstTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame,                          eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,    eLstTg,    eTgSupp,    C_szTgCmd    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Comment,                  eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_State,    eFrstTg,    eTgSupp,    C_szTgState    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Comment,    eMdlTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cmd,    eMdlTg,    eTgSupp,    C_szTgCmd    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Adp,    eMdlTg,    eTgSupp,    C_szTgAdp    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Ado,    eMdlTg,    eTgSupp,    C_szTgAdo    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Addr,    eMdlTg,    eTgSupp,    C_szTgAddr    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Data,    eMdlTg,    eTgSupp,    C_szTgData    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_DataLength,    eMdlTg,    eTgSupp,    C_szTgDataLength    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cnt,    eMdlTg,    eTgSupp,    C_szTgCnt    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_InputOffs,    eMdlTg,    eTgSupp,    C_szTgInputOffs    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_OutputOffs,    eMdlTg,    eTgSupp,    C_szTgOutputOffs    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd,                      eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos,    eLstTg,    eTgSupp,    "CopyInfos"    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_State,                eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Comment,              eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cmd,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Adp,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Ado,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Addr,                 eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Data,                 eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_DataLength,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cnt,                  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_InputOffs,            eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_OutputOffs,           eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
                                            
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos,               eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,    eSnglTg,    eTgSupp,    "CopyInfo"    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,         eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Src,    eFrstTg,    eTgSupp,    "SrcBitOffs"    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,         eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Dst,    eMdlTg,    eTgSupp,    "DstBitOffs"    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo,         eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Size,    eLstTg,    eTgSupp,    "BitSize"    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Src,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Dst,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Size,    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
                                            
    /*/Config/ProcessImage */
    {eCfgState_EcCfg_Cfg_ProcessImage,                          eCfgState_EcCfg_Cfg_ProcessImage_Inputs,    eFrstTg,    eTgSupp,    C_szTgInputs    },
    {eCfgState_EcCfg_Cfg_ProcessImage,                          eCfgState_EcCfg_Cfg_ProcessImage_Outputs,    eLstTg,    eTgSupp,    C_szTgOutputs    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs,                   eCfgState_EcCfg_Cfg_ProcessImage_Inputs_ByteSize,    eFrstTg,    eTgSupp,    C_szTgByteSize    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs,                   eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,    eLstTg,    eTgSupp,    C_szTgVariable    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_ByteSize,          eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Name,    eSnglTg,    eTgSupp,    C_szTgName    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Comment,    eSnglTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_DataType,    eSnglTg,    eTgSupp,    C_szTgDataType    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitSize,    eSnglTg,    eTgSupp,    C_szTgBitSize    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitOffs,    eLstTg,    eTgSupp,    C_szTgBitOffs    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Name,     eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Comment,  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_DataType, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitSize,  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitOffs,  eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#else
    {eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable,          eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
#endif
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs,                  eCfgState_EcCfg_Cfg_ProcessImage_Outputs_ByteSize,    eFrstTg,    eTgSupp,    C_szTgByteSize    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs,                  eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,    eLstTg,    eTgSupp,    C_szTgVariable    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_ByteSize,         eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#if (defined INCLUDE_VARREAD)
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Name,    eFrstTg,    eTgSupp,    C_szTgName    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Comment,    eSnglTg,    eTgSupp,    C_szTgComment    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_DataType,    eSnglTg,    eTgSupp,    C_szTgDataType    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitSize,    eSnglTg,    eTgSupp,    C_szTgBitSize    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitOffs,    eLstTg,    eTgSupp,    C_szTgBitOffs    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Name,    eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Comment, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_DataType,eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitSize, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitOffs, eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#else
    {eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable,         eCfgState_ERROR,    eValTg,    eTgNotSupp,    EC_NULL    },
#endif
#if (defined INCLUDE_OEM)
    {eCfgState_EcCfg,                                           eCfgState_EcCfg_Manufacturer, eLstTg,    eTgSupp,    EC_NULL    },
    {eCfgState_EcCfg_Manufacturer,                              eCfgState_ERROR_NoSubNode,    eValTg,    eTgSupp,    EC_NULL    },
#endif
};
static EC_T_DWORD S_dwNumStateChangeDescEntries = sizeof(C_aCfgStateChangeDesc)/sizeof(EC_T_CFG_STATE_CHANGE_DESC);


/********************************************************************************/
/** \brief Read fixed amount of binary data from string.
*
* \return N/A.
*/
EC_T_BOOL CEcConfigXpat::CfgReadFixedSizeBinData
(EC_T_CHAR* achInputData
,EC_T_INT   nInputDataLen
,EC_T_DWORD dwOutputDataSize
,EC_T_BYTE* pbyOutputData
)
{
    EC_T_BOOL   bOk = EC_TRUE;
    EC_T_UINT   uRes;

    if (((EC_T_DWORD)nInputDataLen == 2*dwOutputDataSize) && (achInputData != EC_NULL))
    {
        uRes = XmlGetBinData( achInputData, nInputDataLen, pbyOutputData, dwOutputDataSize);

        if (uRes != dwOutputDataSize)
        {
            EC_DBGMSG( "CEcConfigXpat::CfgReadFixedSizeBinData(): error processing %s (result size = %d, requested size = %d\n", CfgStateDescription(m_ECfgParserState), uRes, dwOutputDataSize);
            m_ECfgParserState = eCfgState_ERROR;
            bOk = EC_FALSE;
        }
    }
    else
    {
        EC_DBGMSG( "CEcConfigXpat::CfgReadFixedSizeBinData(): error reading %s, InputDataPtr = 0x%x, data len = %d, requested = %d\n", CfgStateDescription(m_ECfgParserState), achInputData, nInputDataLen, 2*dwOutputDataSize);
        m_ECfgParserState = eCfgState_ERROR;
        bOk = EC_FALSE;
    }
    return bOk;
}

/********************************************************************************/
/** \brief Read variable amount of binary data from string.
*
* \return pointer to binary output data.
*/
EC_T_BOOL CEcConfigXpat::CfgReadVariableSizeBinData
(EC_T_CHAR*     achInputData        /* [in]  input data */
,EC_T_INT       nInputDataLen       /* [in]  length of input data */
,EC_T_WORD*     pwDataLen           /* [out] number of binary data */
,EC_T_BYTE**    ppbyData
,EC_T_WORD*     pwBufferDataLen     /* [in/out] number of binary data */
)
{
    EC_T_BOOL   bOk = EC_TRUE;

    *pwDataLen = (EC_T_WORD)(nInputDataLen/2);
    if (*pwDataLen > *pwBufferDataLen)
    {
        *pwBufferDataLen = (EC_T_WORD)(((*pwDataLen+127)/128)*128);
        *ppbyData = (EC_T_BYTE*)OsRealloc(*ppbyData, *pwBufferDataLen);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::pbyData", *ppbyData, *pwBufferDataLen);
    }

    XmlGetBinDataNoAlloc(*ppbyData, achInputData, nInputDataLen);

    return bOk;
}


/********************************************************************************/
/** \brief Read BOOLEAN data from string.
*
* \return EC_TRUE in case of success, EC_FALSE otherwise.
*/
EC_T_BOOL CEcConfigXpat::CfgReadBoolean
(EC_T_CHAR*  achInputData
,EC_T_BOOL*  pbOutputData
)
{
EC_T_BOOL   bOk = EC_TRUE;

    OsDbgAssert( pbOutputData != EC_NULL );

    *pbOutputData = EC_FALSE;
    if (OsStrncmp(achInputData, "0", 1) == 0)
    {
        *pbOutputData = EC_FALSE;
    }
    else if (OsStrncmp(achInputData, "1", 1) == 0)
    {
        *pbOutputData = EC_TRUE;
    }
    else if (OsStrncmp(achInputData, "true", 4) == 0)
    {
        *pbOutputData = EC_TRUE;
    }
    else if (OsStrncmp(achInputData, "false", 5) == 0)
    {
        *pbOutputData = EC_FALSE;
    }
    else
    {
        EC_DBGMSG( "CEcConfigXpat::CfgReadBoolean(): invalid boolean value (%s) at %s\n", achInputData, CfgStateDescription(m_ECfgParserState));
        bOk = EC_FALSE;
    }

    return bOk;
}




/********************************************************************************/
/** \brief Read DWORD data from string.
*
* \return EC_TRUE in case of success, EC_FALSE otherwise.
*/
EC_T_BOOL CEcConfigXpat::CfgReadDword
(EC_T_CHAR*  achInputData
,EC_T_INT    nInputDataLen
,EC_T_DWORD* pdwOutputData
)
{
    EC_T_CHAR szNumber[64]={0};
    EC_T_BOOL  bOk = EC_TRUE;

    OsMemcpy(szNumber, achInputData, EC_MIN(nInputDataLen, (int)(sizeof(szNumber)-1)));

    if (0 == OsMemcmp(szNumber,"#x",2))
    {
        szNumber[0]='0';
    }

    if (EC_NULL == pdwOutputData)
    {
        bOk = EC_FALSE;
        goto Exit;
    }

    EC_SETDWORD(pdwOutputData, OsStrtoul(szNumber, EC_NULL, 0));

Exit:
    return bOk;
}


/********************************************************************************/
/** \brief Read WORD data from string.
*
* \return EC_TRUE in case of success, EC_FALSE otherwise.
*/
EC_T_BOOL CEcConfigXpat::CfgReadWord
(EC_T_CHAR*  achInputData
,EC_T_INT    nInputDataLen
,EC_T_WORD*  pwOutputData
)
{
    EC_T_CHAR szNumber[64]={0};
    EC_T_BOOL  bOk = EC_TRUE;

    OsMemcpy(szNumber, achInputData, EC_MIN(nInputDataLen, (int)(sizeof(szNumber)-1)));

    if (0 == OsMemcmp(szNumber,"#x",2))
    {
        szNumber[0]='0';
    }

    if (EC_NULL == pwOutputData)
    {
        bOk = EC_FALSE;
        goto Exit;
    }

    EC_SETWORD(pwOutputData, (EC_T_WORD)OsStrtoul(szNumber, EC_NULL, 0));

Exit:
    return bOk;
}

/********************************************************************************/
/** \brief Read init command transition value.
*
* \return N/A
*/
EC_T_VOID CEcConfigXpat::CfgReadTransition(EC_T_CHAR* achInputData, EcInitCmdDesc* pEcInitCmdDesc)
{
    if (OsStrncmp(achInputData, "IP", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_I_P ));
    }
    else if (OsStrncmp(achInputData, "IB", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_I_B ));
    }
    else if (OsStrncmp(achInputData, "BI", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_B_I ));
    }
    else if (OsStrncmp(achInputData, "PS", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_P_S ));
    }
    else if (OsStrncmp(achInputData, "PI", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_P_I ));
    }
    else if (OsStrncmp(achInputData, "SP", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_S_P ));
    }
    else if (OsStrncmp(achInputData, "SO", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_S_O ));
    }
    else if (OsStrncmp(achInputData, "SI", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_S_I ));
    }
    else if (OsStrncmp(achInputData, "OS", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_O_S ));
    }
    else if (OsStrncmp(achInputData, "OP", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_O_P ));
    }
    else if (OsStrncmp(achInputData, "OI", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_O_I ));
    }
    else if (OsStrncmp(achInputData, "II", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_I_I));
    }
    else if (OsStrncmp(achInputData, "PP", 2) == 0)
    {
        EC_ECINITCMDDESC_SET_TRANSITION(pEcInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pEcInitCmdDesc) | ECAT_INITCMD_P_P));
    }
    else
    {
        EC_DBGMSG( "CEcConfigXpat::CfgReadTransition(): invalid transition value (%s) at %s\n", achInputData, CfgStateDescription(m_ECfgParserState));
    }
}

/********************************************************************************/
/** \brief Read init command transition value.
*
* \return transition value
*/
EC_T_DWORD CEcConfigXpat::CfgReadTransition(EC_T_CHAR* achInputData)
{
EC_T_DWORD dwTransition = 0;

    if (OsStrncmp(achInputData, "II", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_I_I;
    }
    else if (OsStrncmp(achInputData, "IP", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_I_P;
    }
    else if (OsStrncmp(achInputData, "IB", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_I_B;
    }
    else if (OsStrncmp(achInputData, "BI", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_B_I;
    }
    else if (OsStrncmp(achInputData, "PP", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_P_P;
    }
    else if (OsStrncmp(achInputData, "PS", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_P_S;
    }
    else if (OsStrncmp(achInputData, "PI", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_P_I;
    }
    else if (OsStrncmp(achInputData, "SP", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_S_P;
    }
    else if (OsStrncmp(achInputData, "SO", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_S_O;
    }
    else if (OsStrncmp(achInputData, "SI", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_S_I;
    }
    else if (OsStrncmp(achInputData, "OS", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_O_S;
    }
    else if (OsStrncmp(achInputData, "OP", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_O_P;
    }
    else if (OsStrncmp(achInputData, "OI", 2) == 0)
    {
        dwTransition = ECAT_INITCMD_O_I;
    }
    else
    {
        EC_DBGMSG( "CEcConfigXpat::CfgReadTransition(): invalid transition value (%s) at %s\n", achInputData, CfgStateDescription(m_ECfgParserState));
    }
    return dwTransition;
}



/********************************************************************************/
/** \brief Read command state value.
*
* \return N/A
*/
EC_T_WORD CEcConfigXpat::CfgReadState
(EC_T_CHAR*  achInputData
)
{
    EC_T_WORD wDeviceState = 0;

    if (OsStrncmp(achInputData, "INIT", 4) == 0)
    {
        wDeviceState = DEVICE_STATE_INIT;
    }
    else if (OsStrncmp(achInputData, "PREOP", 5) == 0)
    {
        wDeviceState = DEVICE_STATE_PREOP;
    }
    else if (OsStrncmp(achInputData, "SAFEOP", 6) == 0)
    {
        wDeviceState = DEVICE_STATE_SAFEOP;
    }
    else if (OsStrncmp(achInputData, "OP", 2) == 0)
    {
        wDeviceState = DEVICE_STATE_OP;
    }
    else
    {
        EC_DBGMSG( "CEcConfigXpat::CfgReadState(): invalid state value (%s) at %s\n", achInputData, CfgStateDescription(m_ECfgParserState));
    }
    return wDeviceState;
}

/********************************************************************************/
/** \brief Expand configuration data pointer array (array management).
*
* \return EC_TRUE if array could be expanded.
*/
EC_T_BOOL CEcConfigXpat::ExpandPointerArray
(EC_T_VOID*** pppvArray     /* [in]  pointer to the array */
,EC_T_DWORD*  pdwCurSize   /* [in,out]  Current size */
,EC_T_DWORD   dwExpandSize  /* [in]  expand size */
)
{
EC_T_VOID** ppvNewArray;
EC_T_DWORD  dwIndex;
EC_T_BOOL   bOk = EC_TRUE;

    ppvNewArray = EC_NEW(EC_T_PVOID [*pdwCurSize+dwExpandSize]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::ExpandPointerArray", ppvNewArray, (*pdwCurSize+dwExpandSize) * sizeof(EC_T_PVOID));
    if (ppvNewArray == EC_NULL)
    {
        EC_ERRORMSG(("CEcConfigXpat::ExpandPointerArray() - no memory while expanding pointer array\n"));
        m_ECfgParserState = eCfgState_ERROR;
        bOk = EC_FALSE;
    }
    else
    {
        OsMemset( ppvNewArray, 0, (*pdwCurSize+dwExpandSize) * sizeof(EC_T_VOID*));

        /* save old array pointer values */
        for( dwIndex = 0; dwIndex < *pdwCurSize; dwIndex++ )
        {
            ppvNewArray[dwIndex] = (*pppvArray)[dwIndex];
        }
        /* delete old array pointer values */
        if (EC_NULL != *pppvArray)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::ExpandPointerArray", *pppvArray, (*pdwCurSize+dwExpandSize) * sizeof(EC_T_PVOID));
            SafeDeleteArray( *pppvArray );
        }
        /* return new values */
        *pppvArray = ppvNewArray;
        *pdwCurSize += dwExpandSize;
    }
    return bOk;
}


#if (defined INCLUDE_VARREAD)
/********************************************************************************/
/** \brief Create new packed Pdo entry.
*
* \return N/A.
*/
EC_T_BOOL CEcConfigXpat::CfgCreateECatPdoCmd(EC_T_XPAT_SLAVE* pXpatSlave, EC_T_SLAVE_PDO_DESC* pEcatPdoDesc)
{   
    EC_T_BOOL bOk = EC_TRUE;
    EC_T_WORD wSm = pEcatPdoDesc->wSm;

    /* if set, this PDO will be included in the process image */
    if ((DISABLED_SM != wSm) && pXpatSlave->aSmSettings[wSm].bEnabled)
    {
        EC_T_SLAVE_SM_DESC* pSmDesc = &pXpatSlave->aSmSettings[wSm];

        /* if an enabled sync manager with pd size == 0 found, look for a previous sm with left pd space */
        if (0 == pSmDesc->dwPDSize)
        {
            for (EC_T_INT nSmIdx = wSm - 1; nSmIdx >= 0; nSmIdx--)
            {
                EC_T_SLAVE_SM_DESC* pSmDescPrev = &pXpatSlave->aSmSettings[nSmIdx];
                
                if (pSmDescPrev->bEnabled && (pSmDescPrev->bInput == pSmDesc->bInput))
                {
                    if (pSmDescPrev->dwPDOffsetCur < (pSmDescPrev->dwPDOffset + pSmDescPrev->dwPDSize))
                    {
                        EC_T_DWORD dwPDSizeLeft = (pSmDescPrev->dwPDOffset + pSmDescPrev->dwPDSize) - pSmDescPrev->dwPDOffsetCur;
                        
                        /* assign left pd space to sync manager */
                        pSmDesc->dwPDSize = dwPDSizeLeft;
                        pSmDesc->dwPDOffset = pSmDescPrev->dwPDOffsetCur;
                        pSmDesc->dwPDOffsetCur = pSmDescPrev->dwPDOffsetCur;

                        /* finish previous sync manager */
                        pSmDescPrev->dwPDSize -= dwPDSizeLeft;
                        break;
                    }
                }
            }
        }
        if ((0 != pSmDesc->dwPDSize) && ((pSmDesc->dwPDOffsetCur + pEcatPdoDesc->oCurPdo.nBitLen) <= (pSmDesc->dwPDOffset + pSmDesc->dwPDSize)))
        {
            EC_T_SLAVE_PDO_ENTRY* pSlavePdoEntry = EC_NULL;

            /* create pdo storage */
            if ((pEcatPdoDesc->dwPdoCount + 1) > pEcatPdoDesc->dwPdoArraySize)
            {
                bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&(pEcatPdoDesc->apoPdo), &(pEcatPdoDesc->dwPdoArraySize), 10);
                if (!bOk)
                {
                    goto Exit;
                }
            }

            pSlavePdoEntry = (EC_T_SLAVE_PDO_ENTRY*)OsMalloc(sizeof(EC_T_SLAVE_PDO_ENTRY));
            EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::CfgCreateECatPdoCmd", pSlavePdoEntry, sizeof(EC_T_SLAVE_PDO_ENTRY));
            if (EC_NULL == pSlavePdoEntry)
            {
                EC_ERRORMSG(("CEcConfigXpat::CfgCreateECatPdoCmd() - no memory for RxPDO entry\n"));
                bOk = EC_FALSE;
                goto Exit;
            }
            /* assigned bit offset to pdo */
            pEcatPdoDesc->oCurPdo.nBitOffs = (EC_T_INT)pSmDesc->dwPDOffsetCur;
            pSmDesc->dwPDOffsetCur += pEcatPdoDesc->oCurPdo.nBitLen;
            OsMemcpy(pSlavePdoEntry, &pEcatPdoDesc->oCurPdo, sizeof(EC_T_SLAVE_PDO_ENTRY));
            
            pEcatPdoDesc->apoPdo[pEcatPdoDesc->dwPdoCount] = pSlavePdoEntry;
            pEcatPdoDesc->dwPdoCount++;
        }
    }

Exit:
    return bOk;
}

EC_T_BOOL CEcConfigXpat::CfgReadSmType(EC_T_CHAR* achInputData, EC_T_BOOL* pbIsInput)
{
    EC_T_BOOL bOk = EC_TRUE;

    if (OsStrncmp(achInputData, "Inputs", 6) == 0)
    {
        *pbIsInput = EC_TRUE;
    }
    else if (OsStrncmp(achInputData, "Outputs", 7) == 0)
    {
        *pbIsInput = EC_FALSE;
    }
    else
    {
        bOk = EC_FALSE;
        EC_DBGMSG("CEcConfigXpat::CfgReadSmType(): invalid type value (%s) at %s\n", achInputData, CfgStateDescription(m_ECfgParserState));
    }
    return bOk;
}

EC_T_BOOL CEcConfigXpat::CfgAssignPDOffsetToSm(EC_T_XPAT_SLAVE* pXpatSlave, EC_T_WORD wSm)
{
    EC_T_DWORD dwPDSize = 0;
    EC_T_DWORD dwPDOffset = 0;
    EC_T_SLAVE_SM_DESC* pSmDesc = &pXpatSlave->aSmSettings[wSm];

    if (pSmDesc->bInput)
    {
        dwPDSize = pXpatSlave->pEniSlave->adwPDInSize[pXpatSlave->wPDInCurEntry];
        dwPDOffset = pXpatSlave->pEniSlave->adwPDInOffset[pXpatSlave->wPDInCurEntry];
        if (EC_CFG_SLAVE_PD_SECTIONS > pXpatSlave->wPDInCurEntry)
        {
            pXpatSlave->wPDInCurEntry++;
        }
    }
    else
    {
        dwPDSize = pXpatSlave->pEniSlave->adwPDOutSize[pXpatSlave->wPDOutCurEntry];
        dwPDOffset = pXpatSlave->pEniSlave->adwPDOutOffset[pXpatSlave->wPDOutCurEntry];
        if (EC_CFG_SLAVE_PD_SECTIONS > pXpatSlave->wPDOutCurEntry)
        {
            pXpatSlave->wPDOutCurEntry++;
        }
    }
    pSmDesc->dwPDSize      = dwPDSize;
    pSmDesc->dwPDOffset    = dwPDOffset;
    pSmDesc->dwPDOffsetCur = dwPDOffset;

    return EC_TRUE;
}
#endif

#if (defined INCLUDE_AOE_SUPPORT)

/********************************************************************************/
/** \brief  Extracts the Aoe NetId (ASCII notation: "xxx.xxx.xxx.xxx.xxx.xxx")
*
* \return  EC_TRUE in case of success, EC_FALSE in case of an invalid parameter or
*          syntax error
*/
EC_T_BOOL CEcConfigXpat::ParseAoeNetId
(
    EC_T_CHAR* pszStreamBytes,
    EC_T_INT   nStreamLen,
    EC_T_BYTE* pbyAoeNetId
    )
{
#define MIN_AOE_NET_ID_STRING_LEN (6*1 + 5)
#define MAX_AOE_NET_ID_STRING_LEN (6*3 + 5)
    EC_T_CHAR pbyAoeNetIdTmp[MAX_AOE_NET_ID_STRING_LEN +1] = {0};
    EC_T_CHAR* pszPch  = EC_NULL;
    EC_T_INT   nCnt    = 0;
    EC_T_BOOL bRetVal  = EC_TRUE;

    /* parameter check */
    if ((EC_NULL == pszStreamBytes) || (EC_NULL == pbyAoeNetId)
     || (nStreamLen > MAX_AOE_NET_ID_STRING_LEN) || (nStreamLen < MIN_AOE_NET_ID_STRING_LEN))
    {
        bRetVal = EC_FALSE;
        goto Exit;
    }

    OsMemcpy(pbyAoeNetIdTmp, pszStreamBytes, nStreamLen);

    /* Get AoE NetID */
    pszPch = OsStrtok(pbyAoeNetIdTmp, ".");
    for (nCnt = 0; nCnt <= 5; nCnt++)
    {
        /* check if a token could be extracted */
        if (pszPch == EC_NULL)
        {
            bRetVal = EC_FALSE;
            break;
        }

        /* convert the token to a figure */
        pbyAoeNetId[nCnt] = (EC_T_BYTE)OsStrtoul(pszPch, EC_NULL, 10);

        /* get the next token */
        if (nCnt <= 3)
        {
            pszPch = OsStrtok( EC_NULL, ".");
        }
        else if (nCnt == 4)
        {
            pszPch = OsStrtok( EC_NULL, " ");
        }
    }

    Exit:

    /* check if a syntax error or paramter error has occured */
    if (bRetVal == EC_FALSE)
    {
        OsMemset(pbyAoeNetId, 0, 6);
    }

    return bRetVal;
}
#endif /*INCLUDE_AOE_SUPPORT*/

/********************************************************************************/
/** \brief Create new EtherCAT slave create parameters.
*
* \return N/A.
*/
EC_T_BOOL CEcConfigXpat::CfgCreateEcSlaveCreateParams(EC_T_ENI_SLAVE* pEniSlave)
{
    if (0 != pEniSlave->wPhysAddr)
    {
        /* reconfigure array of free address ranges for BT Temporary address generation */
        m_poEcConfigData->RemoveAddressFromRange(pEniSlave->wPhysAddr);
    }

#if (defined INCLUDE_FOE_SUPPORT)
        if (pEniSlave->sParm.bBootStrapSupport)
        {
            if ((pEniSlave->sParm.bCyclicPhysicalMBoxBootPolling == 0) && (pEniSlave->sParm.bLogicalStateMBoxBootPolling == 0))
            {
                /* set to polling */
                pEniSlave->sParm.bCyclicPhysicalMBoxBootPolling = EC_TRUE;
            }
        }
#endif
   return EC_TRUE;
}

/********************************************************************************/
/** \brief Creates the EC_T_XPAT_SLAVE object
*/
EC_T_XPAT_SLAVE* CEcConfigXpat::CreateXpatSlave(CEcConfigXpat*  poEcConfigXpat)
{
    EC_T_XPAT_SLAVE* pXpatSlave = EC_NULL;
    EC_T_ENI_SLAVE*  pEniSlave = EC_NULL;
    EC_T_DWORD       dwRetVal = EC_E_ERROR;

    pXpatSlave = (EC_T_XPAT_SLAVE*)OsMalloc(sizeof(EC_T_XPAT_SLAVE));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::CreateXpatSlave", pXpatSlave, sizeof(EC_T_XPAT_SLAVE));
    if (pXpatSlave == EC_NULL)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    OsMemset(pXpatSlave, 0, sizeof(EC_T_XPAT_SLAVE));

    /* EniSlave */
    dwRetVal = m_poEcConfigData->CreateEniSlave(&pEniSlave);
    if (dwRetVal != EC_E_NOERROR)
        goto Exit;

    pXpatSlave->pEniSlave = pEniSlave;
    pXpatSlave->poEcConfigXpat = poEcConfigXpat;

Exit:
    /* clean up on error */
    if (dwRetVal != EC_E_NOERROR)
    {
        m_dwLastError = dwRetVal;

        if (pXpatSlave != EC_NULL)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::CreateXpatSlave", pXpatSlave, sizeof(EC_T_XPAT_SLAVE));
            OsFree(pXpatSlave);
            pXpatSlave = EC_NULL;
        }
        if (pEniSlave != EC_NULL)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::CreateXpatSlave", pEniSlave, sizeof(EC_T_ENI_SLAVE));
            OsFree(pEniSlave);
        }
    }

    return pXpatSlave;
}

#if (defined INCLUDE_VARREAD)
/********************************************************************************/
/** \brief Destroys PDO entries
*/
EC_T_VOID DestroyPdoEntriesDesc( EC_T_SLAVE_PDO_DESC* pEcatPdoDesc, struct _EC_T_MEMORY_LOGGER* pMLog )
{
EC_T_DWORD dwIndex;

    for( dwIndex = 0; dwIndex < pEcatPdoDesc->dwPdoCount; dwIndex++ )
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CONFIG, pMLog, "DestroyPdoEntriesDesc", pEcatPdoDesc->apoPdo[dwIndex], sizeof(EC_T_SLAVE_PDO_ENTRY));
        SafeOsFree(pEcatPdoDesc->apoPdo[dwIndex]);
    }
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CONFIG, pMLog, "DestroyPdoEntriesDesc", pEcatPdoDesc->apoPdo, sizeof(EC_T_PVOID) * pEcatPdoDesc->dwPdoArraySize);
    SafeDeleteArray( pEcatPdoDesc->apoPdo );
}
#endif /* INCLUDE_VARREAD */

/********************************************************************************/
/** \brief Destroys the EC_T_XPAT_SLAVE object
*/
EC_T_VOID CEcConfigXpat::DeleteXpatSlave(EC_T_XPAT_SLAVE*  pXpatSlave)
{
#if (defined INCLUDE_VARREAD)
    DestroyPdoEntriesDesc(&pXpatSlave->oCurRxPdoDesc, GetMemLog());
    DestroyPdoEntriesDesc(&pXpatSlave->oCurTxPdoDesc, GetMemLog());
#endif
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::DeleteXpatSlave", pXpatSlave, sizeof(EC_T_XPAT_SLAVE));
    SafeOsFree(pXpatSlave);
}




/********************************************************************************/
/** \brief XML start element handler.
*
* \return
*/
extern "C" XML_Error XMLCALL StartElementHandler(void* pUserData, const XML_Char* pTgName, const XML_Char** apAttributes )
{
    CEcConfigXpat* pEcConfig = (CEcConfigXpat*)pUserData;

    return pEcConfig->StartElementHandler( pTgName, apAttributes );
}

XML_Error CEcConfigXpat::StartElementHandler( const XML_Char* pTgName, const XML_Char** apAttributes )
{
    EC_T_BOOL  bOk = EC_TRUE;
    EC_T_BOOL  bCfgStateChanged;

#ifndef INCLUDE_VARREAD
    EC_UNREFPARM(apAttributes);
#endif

    m_bNewDataElement = EC_TRUE;                    /* new element: skip whitespace character in character data handler */

    m_nXmlCharLen = 0;
    OsDbgAssert(!m_bDataHandlerExecuted);  // EndElementHandler should have been called before

    if (!m_bSmartParseSkipNodes && !m_bErrorHit)
    {
        m_ELastCfgParserState = m_ECfgParserState;

        switch (m_ECfgParserState)
        {
        case eCfgState_INIT:
            if (OsStrcmp(pTgName, C_szTgEtherCATConfig) == 0)
            {
#if (defined INCLUDE_OEM)
                m_bEncrypted = EC_FALSE;
                m_ECfgParserState = eCfgState_EcCfg;
            }
            else if (OsStrcmp(pTgName, C_szTgEncryptedEtherCATConfig) == 0)
            {
                m_bEncrypted = EC_TRUE;
#endif
                m_ECfgParserState = eCfgState_EcCfg;
            }
            else
            {
                EC_ERRORMSGC(("CEcConfigXpat::StartElementHandler() - invalid first tag, expected %s\n", C_szTgEtherCATConfig));
                m_ECfgParserState = eCfgState_ERROR;
            }
            break;

        case eCfgState_ERROR:
            break;

        case eCfgState_ERROR_NoSubNode:
            OsDbgAssert(EC_TRUE);
            m_bSmartParseSkipNodes = EC_TRUE;
            break;

        default:
        {
        EC_T_DWORD dwStateChangeEntryIndex = 0;
        EC_T_BOOL bStateNotInList = EC_FALSE;
            if ( (m_dwCurFirstStateChangeEntryIndex == 0xFFFFFFFF)                                                     // current first index invalid?
               ||(C_aCfgStateChangeDesc[m_dwCurFirstStateChangeEntryIndex].wCfgParserOldState != (EC_T_WORD)m_ECfgParserState)     // new basic index?
              )
            {
                m_dwCurFirstStateChangeEntryIndex = 0xFFFFFFFF;
                /* search first entry index which fits to the current parser state */
                for( dwStateChangeEntryIndex = 0; dwStateChangeEntryIndex < S_dwNumStateChangeDescEntries; dwStateChangeEntryIndex++ )
                {
                    if (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserOldState == (EC_T_WORD)m_ECfgParserState)
                    {
                        m_dwCurFirstStateChangeEntryIndex = dwStateChangeEntryIndex;
                        break;
                    }
                }
                if (m_dwCurFirstStateChangeEntryIndex == 0xFFFFFFFF )
                {
                    EC_ERRORMSGC(("CEcConfigXpat::StartElementHandler() - FATAL ERROR: state %d not in list (tag=\"%s\")\n", m_ECfgParserState, CfgStateDescription(m_ECfgParserState)));
                    m_dwCurFirstStateChangeEntryIndex = 0;
                    bStateNotInList = EC_TRUE;
                }
            }
            dwStateChangeEntryIndex = m_dwCurFirstStateChangeEntryIndex;

            /* either the new state is not in the list or it must fit to the base state (old state) */
            OsDbgAssert(bStateNotInList || (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserOldState == m_ECfgParserState));

            if (!( (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wTgLocation==eFrstTg)
                ||(C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wTgLocation==eSnglTg)
                ||bStateNotInList
                )
              )
            {
                if ((C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wIsTgSupported != eTgNotSupp))
                {
                    EC_ERRORMSGC(("CEcConfigXpat::StartElementHandler() - unexpected unsupported tags below %s: first tag = %s\n", CfgStateDescription(m_ECfgParserState), pTgName));
                }
                m_ECfgParserState = m_ELastCfgParserState;
                m_bSmartParseSkipNodes = EC_TRUE;
                break;
            }

            /* the state we begin to search must either be the first tag of the same "old states" or it must be a single tag */
            OsDbgAssert( (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wTgLocation==eFrstTg)
                        ||(C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wTgLocation==eSnglTg)
                        ||bStateNotInList
                       );

            if (!bStateNotInList && (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wIsTgSupported == eTgSupp))
            {
                /* search entry which fits to the new tag */
                bCfgStateChanged = EC_FALSE;
                while( C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserOldState == (EC_T_WORD)m_ECfgParserState)
                {
                    if (OsStrcmp(C_aCfgStateChangeDesc[dwStateChangeEntryIndex].szTg,pTgName)==0 )
                    {
                        /* tag fits, select new state */
                        m_ECfgParserState = (EC_T_CFG_PARSER_STATE)C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserNewState;
                        bCfgStateChanged = EC_TRUE;
                        break;
                    }
                    dwStateChangeEntryIndex++;
                }
                if (!bCfgStateChanged)
                {
                    EC_ERRORMSGC(("CEcConfigXpat::StartElementHandler() - unknown tag below %s: %s\n", CfgStateDescription(m_ECfgParserState), pTgName));
                    m_bSmartParseSkipNodes = EC_TRUE;
                }
            }
            else
            {
                OsDbgAssert(bStateNotInList || (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wIsTgSupported == eTgNotSupp));
                if ((C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wIsTgSupported != eTgNotSupp))
                {
                    EC_ERRORMSGC(("CEcConfigXpat::StartElementHandler() - unexpected unsupported tags below %s: first tag = %s\n", CfgStateDescription(m_ECfgParserState), pTgName));
                }
                m_ECfgParserState = m_ELastCfgParserState;
                m_bSmartParseSkipNodes = EC_TRUE;
            }
        } /* default: */
        } /* switch (m_ECfgParserState) */

        switch (m_ECfgParserState)
        {
            case eCfgState_EcCfg_Cfg_Slv:
            case eCfgState_EcCfg_Cfg_Cyclic:
            case eCfgState_EcCfg_Cfg_ProcessImage:
                break;
            default:
                break;
        }

        if (m_bSmartParseSkipNodes || m_bErrorHit )
        {
            m_bSmartParseSkipNodes = EC_TRUE;
            m_nSmartParseParentSkipNodeDepth = m_nDepth;
        }
        else
        {
            /* check if new configuration entities have to be created */
            if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd)
            {
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
            }
#if (defined INCLUDE_HOTCONNECT)
            else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd)
            {
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
            }
#endif
            switch (m_ECfgParserState)
            {
#if (defined INCLUDE_OEM)
            case eCfgState_EcCfg:
            {
            EC_T_DWORD dwCnt = 0;

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                    XML_Char* szValue = (XML_Char*)apAttributes[dwCnt];

                    if (szValue == EC_NULL)
                        break;

                    /* iterate to value in apAttributes */
                    dwCnt++;

                    if (OsStrncmp(szValue, C_szAttrEncryptionVersion, OsStrlen(C_szAttrEncryptionVersion) + 1) == 0)
                    {
                        if (!m_bEncrypted)
                        {
                            m_bErrorHit = EC_TRUE;
                            break;
                        }

                        szValue = (XML_Char*)apAttributes[dwCnt];
                        CfgReadDword(szValue, (EC_T_INT)OsStrlen(szValue) + 1, &m_poCfgMaster->dwEncryptionVersion);
                    }
                }
            } break;
#endif
            case eCfgState_EcCfg_Cfg_Mas_EoE:
                m_poCfgMaster->bEoE = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd:
#if (defined INCLUDE_HOTCONNECT)
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd:
#endif
                {
                    /* new master or slave init command */
                    OsMemset( &m_oCurEcatInitCmdDesc.EcInitCmdsDesc, 0, sizeof(EcInitCmdDesc));
                    m_oCurEcatInitCmdDesc.wDataLen = 0;
                    m_oCurEcatInitCmdDesc.bOnlyDataLen = EC_FALSE;
                    m_oCurEcatInitCmdDesc.wValidateDataLen = 0;
                    m_oCurEcatInitCmdDesc.wValidateDataMaskLen = 0;
                    m_oCurEcatInitCmdDesc.dwCommentLen = 0;

                    EC_ECINITCMDDESC_SET_CNT(&m_oCurEcatInitCmdDesc.EcInitCmdsDesc, ECAT_WCOUNT_DONT_CHECK);
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv:
                {
                    /* new xpat slave entry */
                    bOk = EC_TRUE;
                    m_dwNumCfgSlaves++;
                    if (m_dwNumCfgSlaves > m_dwCfgSlaveArraySize)
                    {
                        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&m_apoCfgSlave, &m_dwCfgSlaveArraySize, C_dwCfgSlaveAllocGranularity);
                    }
                    if (bOk)
                    {
                        OsDbgAssert(m_poCurCfgSlave == EC_NULL);
                        m_poCurCfgSlave = CreateXpatSlave(this);
                        if (m_poCurCfgSlave == EC_NULL)
                        {
                            EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for xpat slave entry\n"));
                            m_ECfgParserState = eCfgState_ERROR;
                        }
                        else
                        {
                            m_apoCfgSlave[m_dwNumCfgSlaves - 1] = m_poCurCfgSlave;
                            m_poCurEniSlave = m_poCurCfgSlave->pEniSlave;
                        }
                    }
                    else
                    {
                        m_ECfgParserState = eCfgState_ERROR;
                        m_dwNumCfgSlaves--;
                    }
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_Identification:
                {
                    EC_T_DWORD dwCnt = 0;
                    XML_Char* pAttribute = EC_NULL;
                    XML_Char*  szValue = EC_NULL;
                    bOk = EC_FALSE;

                    OsDbgAssert(m_poCurEniSlave != EC_NULL);
                    
                    /* process attributes */
                    for (dwCnt = 0;; dwCnt++)
                    {
                        pAttribute = (XML_Char*)apAttributes[dwCnt];

                        if (pAttribute == EC_NULL)
                            break;
                        dwCnt++;
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        if (szValue == EC_NULL)
                        {
                            break;
                        }
                        if (OsStrncmp(pAttribute, "Value", 5) == 0)
                        {
                            bOk = CfgReadWord(szValue, (EC_T_INT)OsStrlen(szValue), &m_poCurEniSlave->wIdentificationValue);
                        }
                    }
                    if (!bOk)
                    {
                        m_ECfgParserState = eCfgState_ERROR;
                    }
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_PreviousPort:
                {
                    /* new previous port entry */
                    if (m_poCurEniSlave->wNumPreviousPorts < EC_MAX_NUMOF_PREV_PORT)
                    {
                        m_poCurEniSlave->wNumPreviousPorts++;
                    }
                    else
                    {
                        EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for PreviousPort entry\n"));
                        m_ECfgParserState = eCfgState_ERROR;
                    }
                }
                break;

#if (defined INCLUDE_DC_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_DC:
                m_poCurEniSlave->sParm.bDc = EC_TRUE;
                break;
#endif

#if (defined INCLUDE_HOTCONNECT)
            case eCfgState_EcCfg_Cfg_Slv_HotConnect:
                {
                    /* new HotConnect entry */
                    if (m_poEcConfigData->CreateHCGroup(&m_pCurHotConnectGroup) != EC_E_NOERROR)
                    {
                        EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for HC Group entry\n"));
                        m_ECfgParserState = eCfgState_ERROR;
                    }
                }
                break;
#endif /* INCLUDE_HOTCONNECT */

            case eCfgState_EcCfg_Cfg_Slv_Mbx:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bMailbox = EC_TRUE;
                break;

#if (defined INCLUDE_VARREAD)
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo:
            {
                EC_T_DWORD dwCnt = 0;

                m_poCurCfgSlave->oCurRxPdoDesc.wSm = DISABLED_SM;
                OsMemset(&m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo, 0, sizeof(EC_T_SLAVE_PDO_ENTRY));

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                    XML_Char* pAttribute = (XML_Char*)apAttributes[dwCnt];
                    XML_Char* szValue = EC_NULL;
                    
                    if (pAttribute == EC_NULL)
                        break;

                    dwCnt++;
                    szValue = (XML_Char*)apAttributes[dwCnt];
                    if (szValue == EC_NULL)
                        break;

                    if (OsStrncmp(pAttribute, "Sm", 2) == 0)
                    {
                        bOk = CfgReadWord(szValue, (EC_T_INT)OsStrlen(szValue), &m_poCurCfgSlave->oCurRxPdoDesc.wSm);
                    }
                }
            } break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype:
            {
                EC_T_DWORD dwCnt = 0;

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                XML_Char* pAttribute = (XML_Char*)apAttributes[dwCnt];

                    if (pAttribute == EC_NULL)
                        break;
                    dwCnt++;
                    if (OsStrncmp(pAttribute, "SwapData", 8) == 0)
                    {
                        m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo.bSwapData = EC_TRUE;
                    }
                }
            } break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo:
            {
                EC_T_DWORD dwCnt = 0;

                m_poCurCfgSlave->oCurTxPdoDesc.wSm = DISABLED_SM;
                OsMemset(&m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo, 0, sizeof(EC_T_SLAVE_PDO_ENTRY));

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                    XML_Char* pAttribute = (XML_Char*)apAttributes[dwCnt];
                    XML_Char* szValue = EC_NULL;

                    if (pAttribute == EC_NULL)
                        break;

                    dwCnt++;
                    szValue = (XML_Char*)apAttributes[dwCnt];
                    if (szValue == EC_NULL)
                        break;

                    if (OsStrncmp(pAttribute, "Sm", 2) == 0)
                    {
                        bOk = CfgReadWord(szValue, (EC_T_INT)OsStrlen(szValue), &m_poCurCfgSlave->oCurTxPdoDesc.wSm);
                    }
                }
            } break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype:
            {
                EC_T_DWORD dwCnt = 0;

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                XML_Char* pAttribute = (XML_Char*)apAttributes[dwCnt];
                    if (pAttribute == EC_NULL)
                        break;
                    dwCnt++;
                    if (OsStrncmp(pAttribute, "SwapData", 8) == 0)
                    {
                        m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo.bSwapData = EC_TRUE;
                    }
                }
            } break;
#endif /* INCLUDE_VARREAD */

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd:
                {
                    /* new slave coe init command entry */
                    EC_T_DWORD dwCnt = 0;
                    XML_Char*  pAttribute = EC_NULL;
                    XML_Char*  szValue = EC_NULL;

                    OsDbgAssert(m_poCurEniSlave != EC_NULL);
                    OsMemset(&m_oMbxInitCmdDesc, 0, sizeof(EC_T_SLAVE_MBX_INIT_CMD_DESC));

                    /* process attributes */
                    for (dwCnt = 0;; dwCnt++)
                    {
                        pAttribute = (XML_Char*)apAttributes[dwCnt];
                        if (pAttribute == EC_NULL)
                        {
                            break;
                        }
                        dwCnt++;
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        if (szValue == EC_NULL)
                        {
                            break;
                        }
                        if (OsStrncmp(pAttribute, "Fixed", 5) == 0)
                        {
                            CfgReadBoolean(szValue, &m_oMbxInitCmdDesc.bFixed);
                        }
                        else if (OsStrncmp(pAttribute, "CompleteAccess", 14) == 0)
                        {
                            CfgReadBoolean(szValue, &m_oMbxInitCmdDesc.uMbx.coe.bCompleteAccess);
                        }
                    }
                    
                } /* case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd: */
                break;

#if (defined INCLUDE_SOE_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd:
                {
                    /* new slave soe init command entry */
                    EC_T_STRING oString;
                    EC_T_DWORD dwCnt = 0;
                    XML_Char*  pAttribute = EC_NULL;
                    XML_Char*  szValue = EC_NULL;

                    OsDbgAssert(m_poCurEniSlave != EC_NULL);
                    OsMemset(&m_oMbxInitCmdDesc, 0, sizeof(EC_T_SLAVE_MBX_INIT_CMD_DESC));

                    /* process attributes */
                    for (dwCnt = 0;; dwCnt++)
                    {
                        pAttribute = (XML_Char*)apAttributes[dwCnt];
                        if (pAttribute == EC_NULL)
                        {
                            break;
                        }
                        dwCnt++;
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        if (szValue == EC_NULL)
                        {
                            break;
                        }
                        InitString(oString, pAttribute, (EC_T_DWORD)OsStrlen(pAttribute), GetMemLog());
                        if (IsEqualString(&oString, "Fixed", 5))
                        {
                            CfgReadBoolean(szValue, &m_oMbxInitCmdDesc.bFixed);
                        }

                        DeleteStatString(oString, GetMemLog());
                    }
                } /* case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd: */
                break;
#endif
#if (defined INCLUDE_AOE_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd:
                {
                    /* new slave aoe init command entry */
                    EC_T_STRING oString; 
                    EC_T_DWORD dwCnt = 0;
                    XML_Char*  pAttribute = EC_NULL;
                    XML_Char*  szValue = EC_NULL;
    
                    OsDbgAssert(m_poCurEniSlave != EC_NULL);
                    OsMemset(&m_oMbxInitCmdDesc, 0, sizeof(EC_T_SLAVE_MBX_INIT_CMD_DESC));
                    
                    /* process attributes */
                    for (dwCnt = 0;; dwCnt++)
                    {
                        pAttribute = (XML_Char*)apAttributes[dwCnt];
                        if (pAttribute == EC_NULL)
                        {
                            break;
                        }
                        dwCnt++;
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        if (szValue == EC_NULL)
                        {
                            break;
                        }
                        InitString(oString, pAttribute, (EC_T_DWORD)OsStrlen(pAttribute), GetMemLog());
                        if (IsEqualString(&oString, "Fixed", 5))
                        {
                            CfgReadBoolean(szValue, &m_oMbxInitCmdDesc.bFixed);
                        }

                        DeleteStatString(oString, GetMemLog());
                    }
                } /* case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd: */
                break;
#endif
            case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd:
                {
                    /* new slave eoe init command entry */
                    EC_T_DWORD dwCnt = 0;
                    XML_Char*  pAttribute = EC_NULL;
                    XML_Char*  szValue = EC_NULL;

                    OsDbgAssert(m_poCurEniSlave != EC_NULL);
                    OsMemset(&m_oMbxInitCmdDesc, 0, sizeof(EC_T_SLAVE_MBX_INIT_CMD_DESC));
                    
                    /* process attributes */
                    for (dwCnt = 0;; dwCnt++)
                    {
                        pAttribute = (XML_Char*)apAttributes[dwCnt];
                        if (pAttribute == EC_NULL)
                        {
                            break;
                        }
                        dwCnt++;
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        if (szValue == EC_NULL)
                        {
                            break;
                        }
                        if (OsStrncmp(pAttribute, "Fixed", 5) == 0)
                        {
                            CfgReadBoolean(szValue, &m_oMbxInitCmdDesc.bFixed);
                        }
                    }
                } /* case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd: */
                break;

            case eCfgState_EcCfg_Cfg_Cyclic:
                {
                /* new cyclic entry */
                    OsDbgAssert(m_poCurCfgCyclic == EC_NULL);
                    
                    if (m_poEcConfigData->CreateCyclicEntry(&m_poCurCfgCyclic) != EC_E_NOERROR)
                    {
                        EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for cyclic entry\n"));
                        m_ECfgParserState = eCfgState_ERROR;
                        m_dwLastError = EC_E_NOMEMORY;
                    }
                } break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame:
                {
                    /* new cyclic frame */
                    OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                    OsDbgAssert(m_pCurCyclicFrameDesc == EC_NULL);

                    if (m_poEcConfigData->CreateCyclicFrame(m_poCurCfgCyclic, &m_pCurCyclicFrameDesc) != EC_E_NOERROR)
                    {
                        EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for cyclic frame\n"));
                        m_ECfgParserState = eCfgState_ERROR;
                        m_dwLastError = EC_E_NOMEMORY;
                    }
                } break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd:
                {
                    /* (re)initialize cyclic command configuration description */
                    OsMemset(&m_oCurCycCmdCfgDesc, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));
                } break;

            /*********************************************************/
            /*** /EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos ***/
            /*********************************************************/
            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo:
                {
                    m_poCurCopyInfoEntry = m_poEcConfigData->CreateCopyInfoEntry();
                    if (m_poCurCopyInfoEntry == EC_NULL)
                    {
                        EC_ERRORMSG(("CEcConfigXpat::StartElementHandler() - no memory for copy info entry\n"));
                        m_ECfgParserState = eCfgState_ERROR;
                    }
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv:
                {
                    m_poCurEniSlave->wPDInEntries++;
                    if (m_poCurEniSlave->wPDInEntries > EC_CFG_SLAVE_PD_SECTIONS )
                    {
                        m_poCurEniSlave->wPDInEntries--;
                    }
                } break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Send:
                {
                    m_poCurEniSlave->wPDOutEntries++;
                    if (m_poCurEniSlave->wPDOutEntries > EC_CFG_SLAVE_PD_SECTIONS )
                    {
                        m_poCurEniSlave->wPDOutEntries--;
                    }
                } break;

#if (defined INCLUDE_VARREAD)
			case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable:
			case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable:
                {
                    /* new process image var. entry */
                    OsMemset(&m_oCurProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                } break;
#endif
#if (defined INCLUDE_OEM)
            case eCfgState_EcCfg_Manufacturer:
            {
            EC_T_DWORD dwCnt = 0;

                /* process attributes */
                for (dwCnt = 0; ; dwCnt++)
                {
                XML_Char* szValue = (XML_Char*)apAttributes[dwCnt];

                    if (szValue == EC_NULL)
                        break;

                    /* iterate to value in apAttributes */
                    dwCnt++;

                    if (OsStrncmp(szValue, C_szAttrSignature, OsStrlen(C_szAttrSignature) + 1) == 0)
                    {
                        szValue = (XML_Char*)apAttributes[dwCnt];
                        CfgReadDword(szValue, (EC_T_INT)OsStrlen(szValue) + 1, &m_poCfgMaster->dwManufacturerSignature);
                    }
                }
            } break;
#endif
            default:
                break;
            } /* switch (m_ECfgParserState) */
        } /* else if (m_bSmartParseSkipNodes ) */
        m_bGoingDown = EC_TRUE;
    } /* if (!m_bSmartParseSkipNodes && !m_bErrorHit ) */

    m_nDepth++;

    return ((m_ECfgParserState == eCfgState_ERROR) || !bOk) ? XML_ERROR_NO_MEMORY : XML_ERROR_NONE;
}

/********************************************************************************/
/** \brief XML end element handler
*
* \return
*/
extern "C" {
    static XML_Error EndElementHandler( void* pUserData, const XML_Char* szTgName)
    {
        CEcConfigXpat* pEcConfig = (CEcConfigXpat*)pUserData;

        return pEcConfig->EndElementHandler( szTgName);
    }
}

/********************************************************************************/
/** \brief
*
* \return
*/
XML_Error CEcConfigXpat::EndElementHandler( const char* szTgName)
{
    EC_T_BOOL       bOk = EC_TRUE;
    EC_T_DWORD      dwStateChangeEntryIndex;
    EC_T_DWORD      dwRes = EC_E_NOERROR;
    EC_T_BOOL       bStateChanged = EC_FALSE;

    EC_UNREFPARM(szTgName);

    /* finally execute the data handler */
    OsDbgAssert(m_bExecuteDataHandler == EC_FALSE);
    m_bExecuteDataHandler = EC_TRUE;
    if (CharacterDataHandler( m_pXmlCharDataBuf, m_nXmlCharLen ) != XML_ERROR_NONE)
    {
        m_ECfgParserState = eCfgState_ERROR;
        m_bErrorHit = EC_TRUE;
    }

    m_nXmlCharLen = 0;
    m_bExecuteDataHandler = EC_FALSE;

    if (!m_bSmartParseSkipNodes && !m_bErrorHit )
    {
        /* finalize configuration entity */
        if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd)
        {
            /* master init command */
            m_oCurEcatInitCmdDesc.bMasterInitCmd = EC_TRUE;
            dwRes = m_poEcConfigData->CreateECatInitCmd(&m_poCfgMaster->apPkdEcatInitCmdDesc, 
                &m_poCfgMaster->wNumEcatInitCmds, &m_oCurEcatInitCmdDesc);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv)
        {
            EC_T_BOOL bIsHCGroupHead = EC_FALSE;

            OsDbgAssert(m_poCurEniSlave != EC_NULL);

            /* slave entry */
            bOk = CfgCreateEcSlaveCreateParams(m_poCurEniSlave);

            /* insert current slave to current hot connect group */
#if (defined INCLUDE_HOTCONNECT)
            if (EC_NULL != m_pCurHotConnectGroup && !m_pCurHotConnectGroup->bIsGroupComplete)
            {
                m_poEcConfigData->AddHCGroupMember(m_pCurHotConnectGroup, m_poCurEniSlave);
                /* Check if this slave contains a HotConnect entry */
                if (m_pCurHotConnectGroup->apGroupMember[0] == m_poCurEniSlave)
                {
                    bIsHCGroupHead = EC_TRUE;
                }
            }
#endif
            /* only first slave or HC group head slaves could have no previous port information */
            if (0 == m_poCurEniSlave->wNumPreviousPorts)
            {
                if ((m_poCurEniSlave->wAutoIncAddr != 0) && !bIsHCGroupHead)
                {
                    m_bErrorHit = EC_TRUE;
                    m_dwLastError = EC_E_XML_PREV_PORT_MISSING;
                }
            }
            m_poCurCfgSlave = EC_NULL;
            m_poCurEniSlave = EC_NULL;
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd)
        {
            /* slave init command */
            m_oCurEcatInitCmdDesc.bMasterInitCmd = EC_FALSE;
            dwRes = m_poEcConfigData->CreateECatInitCmd(&m_poCurEniSlave->apPkdEcatInitCmdDesc, 
                &m_poCurEniSlave->wNumEcatInitCmds, &m_oCurEcatInitCmdDesc);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_HotConnect)
        {
#if !(defined INCLUDE_HOTCONNECT)
            EC_ERRORMSG(("CEcConfigXpat::EndElementHandler() - ENI file contains HotConnect configuration!\n"));
            m_bErrorHit = EC_TRUE;
            m_dwLastError = EC_E_NOTSUPPORTED;
#endif
        }
#if (defined INCLUDE_HOTCONNECT)
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_HotConnect_IdentifyCmd)
        {
            /* slave identify command */
            m_oCurEcatInitCmdDesc.bMasterInitCmd = EC_FALSE;
            dwRes = m_poEcConfigData->CreateECatInitCmd(&m_pCurHotConnectGroup->apPkdEcatIdentifyCmdDesc,
                &m_pCurHotConnectGroup->wNumEcatIdentifyCmds, &m_oCurEcatInitCmdDesc);
        }
#endif
#if (defined INCLUDE_VARREAD)
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Entry)
        {
            /* slave RxPdo entry */
            bOk = CfgCreateECatPdoCmd(m_poCurCfgSlave, &m_poCurCfgSlave->oCurRxPdoDesc);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Entry)
        {
            /* slave TxPdo entry */
            bOk = CfgCreateECatPdoCmd(m_poCurCfgSlave, &m_poCurCfgSlave->oCurTxPdoDesc);
        }
#endif
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd)
        {
            /* slave coe mailbox init command */
            if (!m_oMbxInitCmdDesc.uMbx.coe.bDisabled)
            {
                dwRes = m_poEcConfigData->CreateECatMbxCoeCmd(m_poCurEniSlave, &m_oMbxInitCmdDesc);
            }
        }
#if (defined INCLUDE_SOE_SUPPORT)
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd)
        {
            /* slave soe mailbox init command */
            if (!m_oMbxInitCmdDesc.uMbx.soe.bDisabled)
            {
                dwRes = m_poEcConfigData->CreateECatMbxSoeCmd(m_poCurEniSlave, &m_oMbxInitCmdDesc);
            }
        }
#endif
#if (defined INCLUDE_AOE_SUPPORT)
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd)
        {
            /* slave Aoe mailbox init command */
            dwRes = m_poEcConfigData->CreateECatMbxAoeCmd(m_poCurEniSlave, &m_oMbxInitCmdDesc);
        }
#endif
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd)
        {
            /* slave eoe mailbox init command */
            dwRes = m_poEcConfigData->CreateECatMbxEoeCmd(m_poCurEniSlave, &m_oMbxInitCmdDesc);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Cyclic )
        {
            OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
            m_poCurCfgCyclic = EC_NULL;
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Cyclic_Frame)
        {
            OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
            OsDbgAssert(m_pCurCyclicFrameDesc != EC_NULL);
            m_pCurCyclicFrameDesc = EC_NULL;
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd)
        {
            /* cyclic cmd */
            OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
            OsDbgAssert(m_pCurCyclicFrameDesc != EC_NULL);
            bOk = m_poEcConfigData->CreateCyclicFrameCmd(m_pCurCyclicFrameDesc, &m_oCurCycCmdCfgDesc, EC_NULL);
        }

#if (defined INCLUDE_VARREAD)
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable)
        {
            dwRes = m_poEcConfigData->CreateProcessVarInfoEntry(&m_oCurProcessVarInfo, EC_TRUE, EC_NULL);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable)
        {
            dwRes = m_poEcConfigData->CreateProcessVarInfoEntry(&m_oCurProcessVarInfo, EC_FALSE, EC_NULL);
        }
        else if (m_ECfgParserState == eCfgState_EcCfg_Cfg_ProcessImage_Inputs ||
                 m_ECfgParserState == eCfgState_EcCfg_Cfg_ProcessImage_Outputs)
        {
            EC_T_BOOL bIsInputData = (m_ECfgParserState == eCfgState_EcCfg_Cfg_ProcessImage_Inputs) ? EC_TRUE : EC_FALSE;
            EC_T_DWORD dwProcessVarEntryIdx = 0;
            EC_T_DWORD dwNumProcessVarEntries = bIsInputData ? m_poEcConfigData->GetNumInpProcessVarInfo() : m_poEcConfigData->GetNumOutpProcessVarInfo();

            /* iterate through the complete list of process variables and search for known complex data types */
            for (dwProcessVarEntryIdx = 0; dwProcessVarEntryIdx < dwNumProcessVarEntries; dwProcessVarEntryIdx++)
            {
                /* get the next data type to check */
                EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo = EC_NULL;
                m_poEcConfigData->GetProcessVarInfoEntry(&pProcessVarInfo, dwProcessVarEntryIdx, bIsInputData);
                /* search for index in TxPDOs resp. in RxPDOs */

                EC_T_BOOL  bFound = EC_FALSE;
                EC_T_DWORD dwSlaveIdx = 0;

                for (dwSlaveIdx = 0; (dwSlaveIdx < (EC_T_DWORD)m_poEcConfigData->GetNumSlaves()) && !bFound; dwSlaveIdx++)
                {
                    EC_T_DWORD dwPDIdx = 0;
                    EC_T_XPAT_SLAVE* pCfgSlave = m_apoCfgSlave[dwSlaveIdx];
                    EC_T_SLAVE_PDO_DESC* poCurPdoDesc = bIsInputData ? &pCfgSlave->oCurTxPdoDesc : &pCfgSlave->oCurRxPdoDesc;
                    if (0 != poCurPdoDesc->dwPdoCount)
                    {
                        for (dwPDIdx = 0; dwPDIdx < poCurPdoDesc->dwPdoCount; dwPDIdx++)
                        {
                            EC_T_SLAVE_PDO_ENTRY* pPdo = poCurPdoDesc->apoPdo[dwPDIdx];

                            if (((pProcessVarInfo->nBitOffs == pPdo->nBitOffs) && (pProcessVarInfo->nBitSize == pPdo->nBitLen)) ||
                                ((pProcessVarInfo->nBitOffs >= pPdo->nBitOffs) && (pProcessVarInfo->nBitOffs < (pPdo->nBitOffs + pPdo->nBitLen))))
                            {
                                pProcessVarInfo->wFixedAddr = pCfgSlave->pEniSlave->wPhysAddr;

                                pProcessVarInfo->wIndex = pPdo->wIndex;
                                pProcessVarInfo->wSubIndex = pPdo->wSubIndex;
                                pProcessVarInfo->wPdoIndex = pPdo->wPdoIndex;
                                /* fill swap information */
                                if (pPdo->bSwapData)
                                {
                                    EC_T_CYC_SWAP_INFO* pSwapInfo = m_poEcConfigData->CreateSwapInfoEntry();
                                    if (pSwapInfo == EC_NULL)
                                    {
                                        EC_ERRORMSG(("CEcConfigXpat::EndElementHandler() - no memory for swap info entry\n"));
                                        m_ECfgParserState = eCfgState_ERROR;
                                        break;
                                    }
                                    else
                                    {
                                        /* fill swap entry */
                                        pSwapInfo->bIsInputData = bIsInputData;
                                        pSwapInfo->wBitOffs = EC_LOWORD((EC_T_DWORD)pProcessVarInfo->nBitOffs);
                                        pSwapInfo->wBitSize = (EC_T_WORD)pProcessVarInfo->nBitSize;
                                    }
                                }
                                bFound = EC_TRUE;
                                break;
                            }
                        }
                    }
                    else
                    {
                        EC_T_WORD   wNumPDEntries = bIsInputData ? pCfgSlave->pEniSlave->wPDInEntries : pCfgSlave->pEniSlave->wPDOutEntries;
                        EC_T_DWORD* adwPDOffset   = bIsInputData ? pCfgSlave->pEniSlave->adwPDInOffset : pCfgSlave->pEniSlave->adwPDOutOffset;
                        EC_T_DWORD* adwPDSize     = bIsInputData ? pCfgSlave->pEniSlave->adwPDInSize : pCfgSlave->pEniSlave->adwPDOutSize;

                        for (dwPDIdx = 0; dwPDIdx < wNumPDEntries; dwPDIdx++)
                        {
                            if ((((EC_T_DWORD)pProcessVarInfo->nBitOffs == adwPDOffset[dwPDIdx]) && ((EC_T_DWORD)pProcessVarInfo->nBitSize == adwPDSize[dwPDIdx])) ||
                                (((EC_T_DWORD)pProcessVarInfo->nBitOffs >= adwPDOffset[dwPDIdx]) && ((EC_T_DWORD)pProcessVarInfo->nBitOffs < (adwPDOffset[dwPDIdx] + adwPDSize[dwPDIdx]))))
                            {
                                pProcessVarInfo->wFixedAddr = pCfgSlave->pEniSlave->wPhysAddr;
                                pProcessVarInfo->wIndex = 0;
                                pProcessVarInfo->wSubIndex = 0;
                                pProcessVarInfo->wPdoIndex = 0;
                                bFound = EC_TRUE;
                                break;
                            }
                        }
                    }
                }

            }
        }
#endif /* INCLUDE_VARREAD */

        /* search first entry index which fits to the current parser state (for going back) */
        for( dwStateChangeEntryIndex = 0; dwStateChangeEntryIndex < S_dwNumStateChangeDescEntries; dwStateChangeEntryIndex++ )
        {
            if (C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserNewState == (EC_T_WORD)m_ECfgParserState)
            {
                m_ECfgParserState = (EC_T_CFG_PARSER_STATE)C_aCfgStateChangeDesc[dwStateChangeEntryIndex].wCfgParserOldState;
                bStateChanged = EC_TRUE;
                break;
            }
        }
        if (!bStateChanged)
        {
            EC_ERRORMSG(("CEcConfigXpat::EndElementHandler() - FATAL ERROR: state %d not in list (tag=\"%s\")\n", m_ECfgParserState, CfgStateDescription(m_ECfgParserState)));
            m_ECfgParserState = eCfgState_ERROR;
        }
    } /* if (!m_bSmartParseSkipNodes && !m_bErrorHit ) */
    m_bGoingDown = EC_FALSE;

    m_nDepth--;

    if ((m_nSmartParseParentSkipNodeDepth > 0) && (m_nSmartParseParentSkipNodeDepth == m_nDepth))
    {
        OsDbgAssert(m_bSmartParseSkipNodes);
        m_bSmartParseSkipNodes = EC_FALSE;
        m_nSmartParseParentSkipNodeDepth = 0;
    }
    m_bDataHandlerExecuted = EC_FALSE;

    return ((m_ECfgParserState == eCfgState_ERROR) || !bOk || dwRes != EC_E_NOERROR) ? XML_ERROR_NO_MEMORY : XML_ERROR_NONE;
}

/********************************************************************************/
/** \brief XML character data handler.
*
* \return
*/
extern "C"
{
    static XML_Error XMLCALL CharacterDataHandler(void* pUserData, const XML_Char* pCharData, EC_T_INT nLen)
    {
        CEcConfigXpat* pEcConfig = (CEcConfigXpat*)pUserData;
        return pEcConfig->CharacterDataHandler( pCharData, nLen);
    }
}

/********************************************************************************/
/** \brief XML
*
* \return
*/
XML_Error CEcConfigXpat::CharacterDataHandler(const XML_Char* pCharData, EC_T_INT nLen)
{
    XML_Char*       pszTmp      = EC_NULL;
    EC_T_INT        nCnt;
    EC_T_DWORD      dwTmp;
    EC_T_BOOL       bOk;
    EC_T_BOOL       bTmp;
    XML_Error       eRetVal = XML_ERROR_NONE;

    pszTmp = (XML_Char*)pCharData;

    /* skip leading whitespace characters (only when entered the first time for a new data element) */
    if (m_bNewDataElement)
    {
        while (0 != nLen)
        {
            if ((0x20 == (pszTmp[0])) /* space */
             || ('\r' == (pszTmp[0])) /* cr */
             || ('\n' == (pszTmp[0])) /* lf */
             || ('\t' == (pszTmp[0])) /* tab */ )
            {
                pszTmp++;
                nLen--;
            }
            else
            {
                break;
            }
        }
        m_bNewDataElement = EC_FALSE;
    }

    if ((nLen != 0) && (EC_NULL != pszTmp))
    {
        if (!m_bExecuteDataHandler && !m_bSmartParseSkipNodes && !m_bErrorHit && m_bGoingDown)
        {
        EC_T_INT nCopyLen;
            /* store data temporarily (only when going down in the tree) */
            if ((m_nXmlCharLen + nLen) > m_nXmlCharDataBufSize)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::~m_pXmlCharDataBuf", m_pXmlCharDataBuf, m_nXmlCharDataBufSize);

                m_nXmlCharDataBufSize = m_nXmlCharLen + nLen + MAX_XML_DATALEN;

                m_pXmlCharDataBuf = (XML_Char*)OsRealloc(m_pXmlCharDataBuf, m_nXmlCharDataBufSize);
                if (m_pXmlCharDataBuf == EC_NULL)
                {
                    EC_ERRORMSG(("CEcConfigXpat::CharacterDataHandler() - no memory for XML char buffer\n"));
                    eRetVal = XML_ERROR_NO_MEMORY;
                    goto Exit;
                }
                EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::m_pXmlCharDataBuf", m_pXmlCharDataBuf, m_nXmlCharDataBufSize);
            }

            nCopyLen = nLen;
            OsMemcpy( &m_pXmlCharDataBuf[m_nXmlCharLen], pCharData, nCopyLen*sizeof(XML_Char));
            m_nXmlCharLen += nCopyLen;
        }

/*
        EC_DBGMSG( "    DataHandler for %s\n", CfgStateDescription(m_ECfgParserState));    // error search
        if (m_bDataHandlerExecuted)
        {
            EC_DBGMSG( "ERROR: - DataHandler for %s was called more than once!!\n", CfgStateDescription(m_ECfgParserState));    // error search
        }
*/
        if (!m_bSmartParseSkipNodes && !m_bErrorHit && !m_bDataHandlerExecuted && m_bExecuteDataHandler)
        {
            switch (m_ECfgParserState)
            {
            /********************/
            /********************/
            /**** ProductKey ****/
            /* not in ETG.2100! */
            /********************/
            case eCfgState_EcCfg_ProductKey:
            case eCfgState_EcCfg_Cfg_ProductKey:
                m_poEcConfigData->SetProductKey(pszTmp, nLen);
                break;

            /**********************/
            /**********************/
            /*** /Config/Master ***/
            /**********************/
            /**********************/
            case eCfgState_EcCfg_Cfg_Mas_Info_Name:
                // currently not used
                break;

            case eCfgState_EcCfg_Cfg_Mas_Info_Destination:
            case eCfgState_EcCfg_Cfg_Mas_Info_Source:
                CfgReadFixedSizeBinData(pszTmp, nLen, ETHERNET_ADDRESS_LEN,
                                         (m_ECfgParserState == eCfgState_EcCfg_Cfg_Mas_Info_Destination) ?
                                         (EC_T_BYTE*)&m_poCfgMaster->oMacDest
                                         : (EC_T_BYTE*)&m_poCfgMaster->oMacSrc);
                break;
            case eCfgState_EcCfg_Cfg_Mas_Info_EtherType:
                CfgReadFixedSizeBinData(pszTmp, nLen, sizeof(EC_T_WORD), (EC_T_BYTE*)m_poCfgMaster->abyEthType);
                break;
            case eCfgState_EcCfg_Cfg_Mas_MailboxStates_StartAddr:
                CfgReadDword(pszTmp, nLen, &m_poCfgMaster->dwLogAddressMBoxStates);
                break;
            case eCfgState_EcCfg_Cfg_Mas_MailboxStates_Count:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCfgMaster->wSizeAddressMBoxStates = (EC_T_WORD)dwTmp;
                break;
            case eCfgState_EcCfg_Cfg_Mas_EoE_MaxPorts:
                CfgReadDword(pszTmp, nLen, &m_poCfgMaster->dwMaxPorts);
                break;
            case eCfgState_EcCfg_Cfg_Mas_EoE_MaxFrames:
                CfgReadDword(pszTmp, nLen, &m_poCfgMaster->dwMaxFrames);
                break;
            case eCfgState_EcCfg_Cfg_Mas_EoE_MaxMACs:
                CfgReadDword(pszTmp, nLen, &m_poCfgMaster->dwMaxMACs);
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Transition:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Transition:
                CfgReadTransition( pszTmp, &m_oCurEcatInitCmdDesc.EcInitCmdsDesc );
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_BeforeSlave:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_BeforeSlave:
                CfgReadBoolean( pszTmp, &bTmp );
                if (bTmp)
                {
                    EcInitCmdDesc* pInitCmdDesc = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc;
                    EC_ECINITCMDDESC_SET_TRANSITION(pInitCmdDesc, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) | ECAT_INITCMD_BEFORE));
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Comment:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment:
#if (defined DEBUG_SLAVE_INITCMD)
                /* debug specific slave and init command */
                if (m_ECfgParserState == eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Comment )
                {
                    if ((m_dwNumSlaves == 4) && (m_poCurCfgSlave->oEcatInitCmdsDesc.wNumInitCmds == 0x11))
                    {
                        m_dwNumSlaves = m_dwNumSlaves;
                    }
                }
#endif
                m_oCurEcatInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN-1, nLen);
                OsMemcpy(m_oCurEcatInitCmdDesc.chCommentData, pszTmp, m_oCurEcatInitCmdDesc.dwCommentLen);
                m_oCurEcatInitCmdDesc.chCommentData[m_oCurEcatInitCmdDesc.dwCommentLen] = 0;
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Requires:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Requires:
                /* if (OsStrncmp(pszTmp, "cycle", 5) == 0)
                {
                    EC_ECINITCMDDESC_SET_NEWCYCLE(&pCurrInitCmdsDesc->EcInitCmdsDesc, 1);
                }
                else if (OsStrncmp(pszTmp, "frame", 5) == 0)
                {
                    EC_ECINITCMDDESC_SET_NEWFRAME(&pCurrInitCmdsDesc->EcInitCmdsDesc, 1);
                } */
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cmd:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cmd:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    pHeader->uCmdIdx.swCmdIdx.byCmd = (EC_T_BYTE)dwTmp;
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Adp:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Adp:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    EC_ICMDHDR_SET_ADDR_ADP(pHeader, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Ado:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Ado:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    EC_ICMDHDR_SET_ADDR_ADO(pHeader, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Addr:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Addr:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    EC_ICMDHDR_SET_ADDR(pHeader, dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Data:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Data:
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wDataLen,
                    &m_pbyBufferData, &m_wBufferDataLen);
                m_oCurEcatInitCmdDesc.pbyData = m_pbyBufferData;
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_DataLength:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_DataLength:
                CfgReadWord(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wDataLen);
                m_oCurEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Cnt:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Cnt:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_CNT(&m_oCurEcatInitCmdDesc.EcInitCmdsDesc, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Retries:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Retries:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_RETRIES(&m_oCurEcatInitCmdDesc.EcInitCmdsDesc, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Data:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Data:
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wValidateDataLen,
                    &m_oCurEcatInitCmdDesc.pbyValidateData, &m_wBufferValidateDataLen);
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_DataMask:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_DataMask:
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wValidateDataMaskLen,
                    &m_oCurEcatInitCmdDesc.pbyValidateDataMask, &m_wBufferValidateDataMaskLen);
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Validate_Timeout:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Validate_Timeout:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurEcatInitCmdDesc.wValidateTimeout = (EC_T_WORD)dwTmp;
                }
                break;
            case eCfgState_EcCfg_Cfg_Mas_ICmds_ICmd_Timeout:
            case eCfgState_EcCfg_Cfg_Slv_ICmds_ICmd_Timeout:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_INITCMDTIMEOUT(&m_oCurEcatInitCmdDesc.EcInitCmdsDesc, (EC_T_WORD)dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Mas_Props_Prop_Name:
                if (m_poEcConfigData->m_dwNumMasterPropEntries + 1 > m_poEcConfigData->m_dwMasterPropArraySize)
                {
                    m_poEcConfigData->m_dwMasterPropArraySize += 5;
                    m_poEcConfigData->m_aMasterProps = (EC_T_MASTER_PROP_DESC*)OsRealloc(m_poEcConfigData->m_aMasterProps, 
                        sizeof(EC_T_MASTER_PROP_DESC) * (m_poEcConfigData->m_dwMasterPropArraySize));
                    if (m_poEcConfigData->m_aMasterProps == EC_NULL)
                    {
                        m_poEcConfigData->m_dwMasterPropArraySize = 0;
                        OsDbgAssert(EC_FALSE);
                        EC_ERRORMSG(("CEcConfigXpat::CharacterDataHandler() - no memory for XML char buffer\n"));
                        eRetVal = XML_ERROR_NO_MEMORY;
                        goto Exit;
                    }
                }
                
                if (m_poEcConfigData->m_dwNumMasterPropEntries < m_poEcConfigData->m_dwMasterPropArraySize)
                {
                    EC_T_INT nCopyLen = EC_MIN(nLen, EC_MASTER_PROP_MAX_NAME_LENGTH-1);
                    OsMemcpy(m_poEcConfigData->m_aMasterProps[m_poEcConfigData->m_dwNumMasterPropEntries].szNameString, pszTmp, nCopyLen);
                    m_poEcConfigData->m_aMasterProps[m_poEcConfigData->m_dwNumMasterPropEntries].szNameString[nCopyLen] = 0;
                }
                break;

            case eCfgState_EcCfg_Cfg_Mas_Props_Prop_Value:
                if (m_poEcConfigData->m_dwNumMasterPropEntries < m_poEcConfigData->m_dwMasterPropArraySize)
                {
                    EC_T_INT nCopyLen = EC_MIN(nLen, EC_MASTER_PROP_MAX_VALUE_LENGTH-1);
                    OsMemcpy(m_poEcConfigData->m_aMasterProps[m_poEcConfigData->m_dwNumMasterPropEntries].szValueString, pszTmp, nCopyLen);
                    m_poEcConfigData->m_aMasterProps[m_poEcConfigData->m_dwNumMasterPropEntries].szValueString[nCopyLen] = 0;
                    m_poEcConfigData->m_dwNumMasterPropEntries++;
                }
                break;

            /*********************/
            /*********************/
            /*** /Config/Slave ***/
            /*********************/
            /*********************/
            case eCfgState_EcCfg_Cfg_Slv_Info_Name:
                {
                    EC_T_INT    nMaxLen = EC_MIN(nLen, ECAT_DEVICE_NAMESIZE-1);
                    OsMemcpy(m_poCurEniSlave->szName, pszTmp, nMaxLen);
                    m_poCurEniSlave->szName[nMaxLen] = '\0';
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_PhysAddr:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wPhysAddr );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_AutoIncAddr:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wAutoIncAddr );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_Identification_Ado:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wIdentificationAdo);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_Physics:
                /* currently not supported */
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                OsMemcpy(m_poCurEniSlave->szPhysics, pszTmp, EC_MIN(nLen, ECAT_DEVICE_PHYSICSSIZE));
                m_poCurEniSlave->szPhysics[nLen] = '\0';
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_VendorId:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwVendorId);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_ProductCode:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwProductCode);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_RevisionNo:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwRevisionNumber );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_SerialNo:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwSerialNumber );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Info_ProductRevision:
                /* currently not supported */
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitStart:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                if (m_poCurEniSlave->wPDOutEntries < EC_CFG_SLAVE_PD_SECTIONS )
                {
                EC_T_DWORD dwBitStart = 0;

                    CfgReadDword(pszTmp, nLen, &dwBitStart);                    
                    m_poEcConfigData->m_dwLowestSlaveSend = EC_MIN(m_poEcConfigData->m_dwLowestSlaveSend, dwBitStart);
                    m_poCurEniSlave->adwPDOutOffset[(m_poCurEniSlave->wPDOutEntries-1)] = dwBitStart;
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Send_BitLength:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                if (m_poCurEniSlave->wPDOutEntries < EC_CFG_SLAVE_PD_SECTIONS )
                {
                EC_T_WORD  wBitLen   = 0;
                EC_T_DWORD dwBitStart = m_poCurEniSlave->adwPDOutOffset[(m_poCurEniSlave->wPDOutEntries-1)];

                    CfgReadWord(pszTmp, nLen, &wBitLen);
                    m_poEcConfigData->m_dwHighestSlaveSend = EC_MAX(m_poEcConfigData->m_dwHighestSlaveSend, (dwBitStart + wBitLen));
                    m_poCurEniSlave->adwPDOutSize[(m_poCurEniSlave->wPDOutEntries-1)] = wBitLen;
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitStart:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                if (m_poCurEniSlave->wPDInEntries < EC_CFG_SLAVE_PD_SECTIONS )
                {
                EC_T_DWORD dwBitStart = 0;

                    CfgReadDword(pszTmp, nLen, &dwBitStart);
                    m_poEcConfigData->m_dwLowestSlaveRecv = EC_MIN(m_poEcConfigData->m_dwLowestSlaveRecv, dwBitStart);
                    m_poCurEniSlave->adwPDInOffset[(m_poCurEniSlave->wPDInEntries-1)] = dwBitStart;
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Recv_BitLength:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                if (m_poCurEniSlave->wPDInEntries < EC_CFG_SLAVE_PD_SECTIONS )
                {
                EC_T_WORD wBitLen   = 0;
                EC_T_DWORD dwBitStart = m_poCurEniSlave->adwPDInOffset[(m_poCurEniSlave->wPDInEntries-1)];

                    CfgReadWord(pszTmp, nLen, &wBitLen);
                    m_poEcConfigData->m_dwHighestSlaveRecv = EC_MAX(m_poEcConfigData->m_dwHighestSlaveRecv, (dwBitStart + wBitLen));
                    m_poCurEniSlave->adwPDInSize[(m_poCurEniSlave->wPDInEntries-1)] = wBitLen;
                }
                break;
#if (defined INCLUDE_VARREAD)
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Watchdog:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Enable:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MinSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_MaxSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_DefaultSize:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_StartAddress:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_ControlByte:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Virtual:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Watchdog:
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[0].bInput); 
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm0_Pdo:
                /* enable sm if it has at least one <pdo> entry */
                if (!m_poCurCfgSlave->aSmSettings[0].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 0);
                    m_poCurCfgSlave->aSmSettings[0].bEnabled = EC_TRUE;
                }
                break;            
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[1].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm1_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[1].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 1);
                    m_poCurCfgSlave->aSmSettings[1].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[2].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm2_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[2].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 2);
                    m_poCurCfgSlave->aSmSettings[2].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[3].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm3_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[3].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 3);
                    m_poCurCfgSlave->aSmSettings[3].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[4].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm4_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[4].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 4);
                    m_poCurCfgSlave->aSmSettings[4].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[5].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm5_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[5].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 5);
                    m_poCurCfgSlave->aSmSettings[5].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[6].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm6_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[6].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 6);
                    m_poCurCfgSlave->aSmSettings[6].bEnabled = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Type:
                CfgReadSmType(pszTmp, &m_poCurCfgSlave->aSmSettings[7].bInput);
                break;
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_Sm7_Pdo:
                if (!m_poCurCfgSlave->aSmSettings[7].bEnabled)
                {
                    CfgAssignPDOffsetToSm(m_poCurCfgSlave, 7);
                    m_poCurCfgSlave->aSmSettings[7].bEnabled = EC_TRUE;
                }
                break;
#endif /* INCLUDE_VARREAD */
            case eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Start:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMboxOutStart );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_Send_Length:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord(pszTmp, nLen, &m_poCurEniSlave->wMboxOutLen);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Start:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMboxInStart );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_Length:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMboxInLen);
                break;

#if (defined INCLUDE_VARREAD)
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Index:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo.wPdoIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Name:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxPdo_Exclude:
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Index:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo.wIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Subindex:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo.wSubIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Bitlen:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurRxPdoDesc.oCurPdo.nBitLen = (EC_T_INT)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Name:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Comment:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_RxEntry_Datatype:
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Index:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo.wPdoIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Name:
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxPdo_Exclude:
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Index:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo.wIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Subindex:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo.wSubIndex = (EC_T_WORD)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Bitlen:
                CfgReadDword(pszTmp, nLen, &dwTmp);
                m_poCurCfgSlave->oCurTxPdoDesc.oCurPdo.nBitLen = (EC_T_INT)dwTmp;
                break;

            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Name:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Comment:
            case eCfgState_EcCfg_Cfg_Slv_ProcessData_TxEntry_Datatype:
                break;
#endif /* INCLUDE_VARREAD */

#if (defined INCLUDE_FOE_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Start:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootOutStart );
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Send_Length:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootOutLen );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Start:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootInStart );
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_Length:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootInLen);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_PollTime:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bCyclicPhysicalMBoxBootPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wCyclePhysicalMBoxBootPollingTime);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_BootStrap_Recv_StatusBitAddr:
                // currently not supported
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bLogicalStateMBoxBootPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wSlaveLogicalAddressMBoxBootState);
                break;

                /* @@@@@@@ Subject to change if Bootstrap mailbox is identifiable */
            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Start:
                /* @@@@@@@ BootStrap mailbox config has to be fixed up, when identified */
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootOutStart );
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Send_Length:
                /* @@@@@@@ BootStrap mailbox config has to be fixed up, when identified */
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootOutLen );
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Start:
                /* @@@@@@@ BootStrap mailbox config has to be fixed up, when identified */
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootInStart );
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_Length:
                /* @@@@@@@ BootStrap mailbox config has to be fixed up, when identified */
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wMbxBootInLen);
                m_poCurEniSlave->sParm.bBootStrapSupport = EC_TRUE;
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_PollTime:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bCyclicPhysicalMBoxBootPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wCyclePhysicalMBoxBootPollingTime);
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Recv_StatusBitAddr:
                // currently not supported
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bLogicalStateMBoxBootPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wSlaveLogicalAddressMBoxBootState);
                break;

            case eCfgState_EcCfg_Cfg_Slv_BootMbx_Protocol:
                // currently not used
                break;
#endif
            case eCfgState_EcCfg_Cfg_Slv_PreviousPort_Port:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                dwTmp = pszTmp[0]; pszTmp++;
                /*CfgReadDword( pszTmp, nLen, &dwTmp);*/
                if ((dwTmp >= 'A') && (dwTmp <= 'D'))
                {
                    m_poCurEniSlave->aoPreviousPort[m_poCurEniSlave->wNumPreviousPorts-1].wPortNumber = EC_LOWORD(dwTmp-'A');
                }
                else
                {
                    m_poCurEniSlave->aoPreviousPort[m_poCurEniSlave->wNumPreviousPorts-1].wPortNumber = ESC_PORT_INVALID;
                    EC_DBGMSG("CEcConfigXpat::CharacterDataHandler(): error processing %s: found %c, but must be A, B, C or D!\n", CfgStateDescription(m_ECfgParserState), EC_LOWORD(dwTmp));
                    eRetVal = XML_ERROR_ABORTED;
                }
                break;

            case eCfgState_EcCfg_Cfg_Slv_PreviousPort_PhysAddr:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &dwTmp);
                m_poCurEniSlave->aoPreviousPort[m_poCurEniSlave->wNumPreviousPorts-1].wSlaveAddress = EC_LOWORD(dwTmp);
                break;

            case eCfgState_EcCfg_Cfg_Slv_PreviousPort_Selected:
                break;

#if (defined INCLUDE_DC_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClock:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &dwTmp);
                m_poCurEniSlave->bDcReferenceClock = (dwTmp == 1)?EC_TRUE:EC_FALSE;
                break;
            case eCfgState_EcCfg_Cfg_Slv_DC_ReferenceClockP:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &dwTmp);
                m_poCurEniSlave->bDcPotentialRefClock = (dwTmp == 1)?EC_TRUE:EC_FALSE;
                break;
            case eCfgState_EcCfg_Cfg_Slv_DC_CycleTime0:
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwDcRegisterCycleTime0 );
                break;

            case eCfgState_EcCfg_Cfg_Slv_DC_CycleTime1:
                CfgReadDword( pszTmp, nLen, &m_poCurEniSlave->dwDcRegisterCycleTime1 );
                break;

            case eCfgState_EcCfg_Cfg_Slv_DC_ShiftTime:
                break;
#endif
#if (defined INCLUDE_HOTCONNECT)
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupName:
                break;

            case eCfgState_EcCfg_Cfg_Slv_HotConnect_GroupMemberCnt:
                {
                    OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                    OsDbgAssert(0 == m_pCurHotConnectGroup->dwNumGroupMembers);
                    if (0 == m_pCurHotConnectGroup->dwNumGroupMembers)
                    {
                        dwTmp = 0;
                        CfgReadDword(pszTmp, nLen, &m_pCurHotConnectGroup->dwNumGroupMembersListed);
                    }
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Comment:
                {
                    OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);

                    m_oCurEcatInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN - 1, nLen);
                    OsMemcpy(m_oCurEcatInitCmdDesc.chCommentData, pszTmp, m_oCurEcatInitCmdDesc.dwCommentLen);
                    m_oCurEcatInitCmdDesc.chCommentData[m_oCurEcatInitCmdDesc.dwCommentLen] = 0;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Requires:
                {
                }break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cmd:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = (EC_T_BYTE)dwTmp;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Adp:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    EC_ICMDHDR_SET_ADDR_ADP(pHeader, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Ado:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    ETYPE_EC_CMD_HEADER* pHeader = &m_oCurEcatInitCmdDesc.EcInitCmdsDesc.EcCmdHeader;
                    EC_ICMDHDR_SET_ADDR_ADO(pHeader, (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Data:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wDataLen, &m_pbyBufferData, &m_wBufferDataLen);
                m_oCurEcatInitCmdDesc.pbyData = m_pbyBufferData;
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_DataLength:
                {
                    CfgReadWord(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wDataLen);
                    m_oCurEcatInitCmdDesc.bOnlyDataLen = EC_TRUE;
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Cnt:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_CNT(&(m_oCurEcatInitCmdDesc.EcInitCmdsDesc), (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Retries:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_RETRIES(&(m_oCurEcatInitCmdDesc.EcInitCmdsDesc), (EC_T_WORD)dwTmp);
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Data:
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wValidateDataLen,
                    &m_oCurEcatInitCmdDesc.pbyValidateData, &m_wBufferValidateDataLen);
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_DataMask:
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oCurEcatInitCmdDesc.wValidateDataMaskLen,
                    &m_oCurEcatInitCmdDesc.pbyValidateDataMask, &m_wBufferValidateDataMaskLen);
                break;
            case eCfgState_EcCfg_Cfg_Slv_HotConnect_IdCmd_Validate_Timeout:
                OsDbgAssert(m_pCurHotConnectGroup != EC_NULL);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ECINITCMDDESC_SET_INITCMDTIMEOUT(&(m_oCurEcatInitCmdDesc.EcInitCmdsDesc), (EC_T_WORD)dwTmp);
                }
                break;
#endif
            case eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_PollTime:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bCyclicPhysicalMBoxPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wCyclePhysicalMBoxPollingTime);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_Recv_StatusBitAddr:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                m_poCurEniSlave->sParm.bLogicalStateMBoxPolling = EC_TRUE;
                CfgReadWord( pszTmp, nLen, &m_poCurEniSlave->wSlaveLogicalAddressMBoxState);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_Protocol:
                OsDbgAssert(m_poCurEniSlave != EC_NULL);
                if (nLen >=3 )
                {
                    for( nCnt = 0; nCnt <= nLen-3; nCnt++ )
                    {
                        if (OsMemcmp(&pszTmp[nCnt], "CoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bCoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
                        else if (OsMemcmp(&pszTmp[nCnt], "EoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bEoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
#if (defined INCLUDE_FOE_SUPPORT)
                        else if (OsMemcmp(&pszTmp[nCnt], "FoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bFoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
#endif
#if (defined INCLUDE_SOE_SUPPORT)
                        else if (OsMemcmp(&pszTmp[nCnt], "SoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bSoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
#endif
                        else if (OsMemcmp(&pszTmp[nCnt], "VoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bVoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
#if (defined INCLUDE_AOE_SUPPORT)
                        else if (OsMemcmp(&pszTmp[nCnt], "AoE", 3) == 0 )
                        {
                            m_poCurEniSlave->sParm.bAoeSupport = EC_TRUE;
                            nCnt += 2;
                        }
#endif
                    }
                }
                break;
            case eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_RetReq:
                {
                 /* Not yet implemented */
                }break;
            case eCfgState_EcCfg_Cfg_Slv_Mbx_Timeout_Response:
                {
                    /* Not yet implemented */
                }break;
#if (defined INCLUDE_AOE_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_NetId:
                {
                    EC_T_BOOL bParseSuccess = ParseAoeNetId(pszTmp, nLen, m_poCurEniSlave->abyAoeNetId);
                    if (!bParseSuccess)
                    {
                        m_bErrorHit = EC_TRUE;
                        m_dwLastError = EC_E_XML_AOE_NETID_INVALID;
                    }
                }break;
            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Comment:
            {
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN-1, nLen);
                OsMemcpy(m_oMbxInitCmdDesc.chCommentData, pszTmp, m_oMbxInitCmdDesc.dwCommentLen);
                m_oMbxInitCmdDesc.chCommentData[m_oMbxInitCmdDesc.dwCommentLen] = 0;
            }break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Transition:
            {
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwTransition |= CfgReadTransition( pszTmp);
            }break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Timeout:
            {
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_oMbxInitCmdDesc.dwTimeout );
            }break;
            case eCfgState_EcCfg_Cfg_Slv_Mbx_AoE_ICmds_ICmd_Data:
            {
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oMbxInitCmdDesc.wDataLen,
                    &m_pbyBufferData, &m_wBufferDataLen);
                m_oMbxInitCmdDesc.pbyData = m_pbyBufferData;
            }break;
#endif
            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Comment:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN - 1, nLen);
                OsMemcpy(m_oMbxInitCmdDesc.chCommentData, pszTmp, m_oMbxInitCmdDesc.dwCommentLen);
                m_oMbxInitCmdDesc.chCommentData[m_oMbxInitCmdDesc.dwCommentLen] = 0;
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Transition:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwTransition |= CfgReadTransition( pszTmp);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Timeout:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_oMbxInitCmdDesc.dwTimeout );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Ccs:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.coe.dwCcs );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Index:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.coe.dwIndex);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_SubIndex:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.coe.dwSubIndex);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Disabled:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadBoolean(pszTmp, &m_oMbxInitCmdDesc.uMbx.coe.bDisabled);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_CoE_ICmds_ICmd_Data:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oMbxInitCmdDesc.wDataLen,
                    &m_pbyBufferData, &m_wBufferDataLen);
                m_oMbxInitCmdDesc.pbyData = m_pbyBufferData;
                break;

#if (defined INCLUDE_SOE_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Comment:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN - 1, nLen);
                OsMemcpy(m_oMbxInitCmdDesc.chCommentData, pszTmp, m_oMbxInitCmdDesc.dwCommentLen);
                m_oMbxInitCmdDesc.chCommentData[m_oMbxInitCmdDesc.dwCommentLen] = 0;
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Transition:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwTransition |= CfgReadTransition( pszTmp);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Timeout:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_oMbxInitCmdDesc.dwTimeout );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_OpCode:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.soe.dwOpCode);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_DriveNo:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.soe.dwDriveNo);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_IDN:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.soe.dwIDN);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Elements:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.soe.dwElements);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Attribute:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword(pszTmp, nLen, &m_oMbxInitCmdDesc.uMbx.soe.dwAttribute);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Disabled:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadBoolean(pszTmp, &m_oMbxInitCmdDesc.uMbx.soe.bDisabled);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_SoE_ICmds_ICmd_Data:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadVariableSizeBinData(pszTmp, nLen, &m_oMbxInitCmdDesc.wDataLen,
                    &m_pbyBufferData, &m_wBufferDataLen);
                m_oMbxInitCmdDesc.pbyData = m_pbyBufferData;
                break;

#endif /* INCLUDE_SOE_SUPPORT */

            case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Transition:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwTransition |= CfgReadTransition( pszTmp);
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Comment:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                m_oMbxInitCmdDesc.dwCommentLen = EC_MIN(MAX_INITCMD_COMMENT_LEN - 1, nLen);
                OsMemcpy(m_oMbxInitCmdDesc.chCommentData, pszTmp, m_oMbxInitCmdDesc.dwCommentLen);
                m_oMbxInitCmdDesc.chCommentData[m_oMbxInitCmdDesc.dwCommentLen] = 0;
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Timeout:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_oMbxInitCmdDesc.dwTimeout );
                break;

            case eCfgState_EcCfg_Cfg_Slv_Mbx_EoE_ICmds_ICmd_Data:
                OsDbgAssert(m_poCurCfgSlave != EC_NULL);
                CfgReadVariableSizeBinData(pszTmp, nLen,  &m_oMbxInitCmdDesc.wDataLen, 
                    &m_pbyBufferData, &m_wBufferDataLen);
                m_oMbxInitCmdDesc.pbyData = m_pbyBufferData;
                break;

            /**********************/
            /*** /Config/Cyclic ***/
            /**********************/
            case eCfgState_EcCfg_Cfg_Cyclic_Comment:
                OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_CycleTime:
                OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurCfgCyclic->dwCycleTime);
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Priority:
                OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                CfgReadDword( pszTmp, nLen, &m_poCurCfgCyclic->dwPriority );
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_TaskId:
                {
                    /* max DWORD is 4294967295 */
                    EC_T_CHAR szCheckTaskIdIsNumeric[11];
                    OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                    CfgReadDword(pszTmp, nLen, &m_poCurCfgCyclic->dwTaskId);

                    /* check TaskId is numeric */
                    szCheckTaskIdIsNumeric[10] = '\0';
                    OsSnprintf(szCheckTaskIdIsNumeric, EC_MIN(nLen, 10) + 1, "%u", m_poCurCfgCyclic->dwTaskId);
                    if (0 != OsStrncmp(szCheckTaskIdIsNumeric, pszTmp, EC_MIN(nLen, 10)))
                    {
                        EC_ERRORMSGC(("CEcConfigXpat::CharacterDataHandler() TaskId is not numeric! SendCycFramesByTaskId will NOT work!\n"));
                    }
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Comment:
                //OsDbgAssert(EC_FALSE);
                //EC_DBGMSG( "CEcConfigXpat::CharacterDataHandler(): entry %s not handled\n", CfgStateDescription(m_ECfgParserState));
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_State:
                {
                    m_oCurCycCmdCfgDesc.oEcCycCmdDesc.wConfOpStatesMask |= CfgReadState(pszTmp);
                } break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Comment:
                //OsDbgAssert(EC_FALSE);
                //EC_DBGMSG( "CEcConfigXpat::CharacterDataHandler(): entry %s not handled\n", CfgStateDescription(m_ECfgParserState));
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cmd:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = (EC_T_BYTE)dwTmp;
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Adp:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.bAdpValid = EC_TRUE;
                    EC_ICMDHDR_SET_ADDR_ADP(&(m_oCurCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), (EC_T_WORD)dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Ado:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.bAdoValid = EC_TRUE;
                    EC_ICMDHDR_SET_ADDR_ADO(&(m_oCurCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), (EC_T_WORD)dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Addr:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.bAddrValid = EC_TRUE;
                    EC_ICMDHDR_SET_ADDR(&(m_oCurCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), (EC_T_DWORD)dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Data:
                OsDbgAssert(m_poCurCfgCyclic != EC_NULL);
                OsDbgAssert(m_pCurCyclicFrameDesc != EC_NULL);
                EC_DBGMSG( "CEcConfigXpat::CharacterDataHandler(): entry %s not handled\n", CfgStateDescription(m_ECfgParserState));
                EC_DBGMSG( "CEcConfigXpat::CharacterDataHandler(): sending pre-configured cyclic data is not supported!!!\n" );
                OsDbgAssert(EC_FALSE);
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_DataLength:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    EC_ICMDHDR_SET_LEN_LEN(&(m_oCurCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), (EC_T_WORD)dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_Cnt:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.oEcCycCmdDesc.bCheckWkc = EC_TRUE;
                    m_oCurCycCmdCfgDesc.oEcCycCmdDesc.wConfiguredWkc = (EC_T_WORD)dwTmp;
                    m_oCurCycCmdCfgDesc.oEcCycCmdDesc.wExpectedWkc = (EC_T_WORD)dwTmp;
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_InputOffs:
                /* this command is responsible for reading the input process data (LRD) */
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {                   
                    m_oCurCycCmdCfgDesc.bInputOffsValid = EC_TRUE;
                    m_oCurCycCmdCfgDesc.dwInputOffs = dwTmp;
                    /* only check logical commands here */
                    m_poEcConfigData->SetInputOffset(&m_oCurCycCmdCfgDesc.oEcCycCmdDesc, dwTmp);
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_OutputOffs:
               /* this command is responsible for writing the output process data (LWR) */
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_oCurCycCmdCfgDesc.bOutputOffsValid = EC_TRUE;
                    m_oCurCycCmdCfgDesc.dwOutputOffs = dwTmp;
                    /* only check logical commands here */
                    m_poEcConfigData->SetOutputOffset(&m_oCurCycCmdCfgDesc.oEcCycCmdDesc, dwTmp);
                }
                break;

            /*********************************************************/
            /*** /EtherCATConfig/Config/Cyclic/Frame/Cmd/CopyInfos ***/
            /*********************************************************/
            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Src:
                OsDbgAssert(m_poEcConfigData->GetNumCopyInfoEntries() != 0);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_poCurCopyInfoEntry->wSrcBitOffs = (EC_T_WORD)dwTmp;
                    m_poCurCopyInfoEntry->wTaskId     = (EC_T_WORD)m_poCurCfgCyclic->dwTaskId;
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Dst:
                OsDbgAssert(m_poEcConfigData->GetNumCopyInfoEntries() != 0);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_poCurCopyInfoEntry->wDstBitOffs = (EC_T_WORD)dwTmp;
                }
                break;

            case eCfgState_EcCfg_Cfg_Cyclic_Frame_Cmd_CInfos_CInfo_Size:
                OsDbgAssert(m_poEcConfigData->GetNumCopyInfoEntries() != 0);
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_poCurCopyInfoEntry->wBitSize = (EC_T_WORD)dwTmp;
                }
                break;

            /***********************************/
            /*** /Config/ProcessImage/Inputs ***/
            /***********************************/
            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_ByteSize:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_poEcConfigData->m_dwInputByteSize = dwTmp;
                }
                break;

#if (defined INCLUDE_VARREAD)
            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Name:
                OsStrncpy(m_oCurProcessVarInfo.szName, pszTmp,
                		  EC_MIN(MAX_PROCESS_VAR_NAME_LEN_EX, (EC_T_DWORD)nLen));
                m_oCurProcessVarInfo.szName[nLen] = '\0';
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_Comment:
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_DataType:
                if (nLen != 0)
                {
                    /* determine string temporarily for comparison in DataTyeName2... */
                    EC_T_CHAR cTmp = pszTmp[nLen];
                    pszTmp[nLen] = '\0';
                    m_oCurProcessVarInfo.wDataType = DataTypeName2DataTypeIdentifier((EC_T_BYTE*)pszTmp);
                    pszTmp[nLen] = cTmp;

                    if (DEFTYPE_NULL == m_oCurProcessVarInfo.wDataType)
                    {
                        OsStrncpy(m_szPDVarInfoSpecificDataType, pszTmp, nLen);
                        m_szPDVarInfoSpecificDataType[nLen] = '\0';
                        m_oCurProcessVarInfo.szSpecificDataType = m_szPDVarInfoSpecificDataType;
                    }
                }
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitSize:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
				{
					m_oCurProcessVarInfo.nBitSize = (EC_T_INT)dwTmp;
				}
                break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Inputs_Variable_BitOffs:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
				{
					m_oCurProcessVarInfo.nBitOffs = (EC_T_INT)dwTmp;
				}
                break;
#endif

			/***********************************/
            /*** /Config/ProcessImage/Outputs ***/
            /***********************************/
            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_ByteSize:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
                {
                    m_poEcConfigData->m_dwOutputByteSize = dwTmp;
                }
                break;

#if (defined INCLUDE_VARREAD)
            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Name:
                OsStrncpy(m_oCurProcessVarInfo.szName, pszTmp,
                		  EC_MIN(MAX_PROCESS_VAR_NAME_LEN_EX, (EC_T_DWORD)nLen));
                m_oCurProcessVarInfo.szName[nLen] = '\0';
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_Comment:
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_DataType:
                if (nLen != 0)
                {
                    /* determine string temporarily for comparison in DataTyeName2... */
                    EC_T_CHAR cTmp = pszTmp[nLen]; 
                    pszTmp[nLen] = '\0';
                    m_oCurProcessVarInfo.wDataType = DataTypeName2DataTypeIdentifier((EC_T_BYTE*)pszTmp);
                    pszTmp[nLen] = cTmp;

                    if (DEFTYPE_NULL == m_oCurProcessVarInfo.wDataType)
                    {
                        OsStrncpy(m_szPDVarInfoSpecificDataType, pszTmp, nLen);
                        m_szPDVarInfoSpecificDataType[nLen] = '\0';
                        m_oCurProcessVarInfo.szSpecificDataType = m_szPDVarInfoSpecificDataType;
                    }
                }
            	break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitSize:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
				{
					m_oCurProcessVarInfo.nBitSize = (EC_T_INT)dwTmp;
				}
                break;

            case eCfgState_EcCfg_Cfg_ProcessImage_Outputs_Variable_BitOffs:
                bOk = CfgReadDword(pszTmp, nLen, &dwTmp);
                if (bOk)
				{
					m_oCurProcessVarInfo.nBitOffs = (EC_T_INT)dwTmp;
				}
                break;
#endif
            case eCfgState_EcCfg_Cfg_Slv_Group:
                break;
            case eCfgState_EcCfg_Cfg_Slv_Mbx_FoE:
                break;
#if (defined INCLUDE_COE_PDO_SUPPORT)
            case eCfgState_EcCfg_Cfg_Slv_BootMbx_CoE:
                break;
#elif (defined INCLUDE_FOE_SUPPORT )
            case eCfgState_EcCfg_Cfg_Slv_BootMbx_FoE:
                break;
#endif

#if (defined INCLUDE_OEM)
            case eCfgState_EcCfg_Manufacturer:
                /* Manufacturer name currently not used */
                break;
#endif

            default:
                EC_DBGMSG( "CEcConfigXpat::CharacterDataHandler(): entry %s not handled\n", CfgStateDescription(m_ECfgParserState));
            }
        } /* if (!m_bSmartParseSkipNodes && !m_bErrorHit && !m_bDataHandlerExecuted && m_bExecuteDataHandler ) */
        if (m_bExecuteDataHandler )
        {
            m_bDataHandlerExecuted = EC_TRUE;
        }
    }
Exit:
    return eRetVal;
}

/********************************************************************************/
/** \brief Creates the configuration object
*
* \return
*/
CEcConfigXpat::CEcConfigXpat(CEcConfigData* poEcConfigData)
{
    OsMemset(&m_oXmlParser, 0, sizeof(XML_Parser));
    m_bXmlParserCreated = EC_FALSE;

    /* link member variables with CEcConfigData */
    m_poEcConfigData = poEcConfigData;

    m_nDepth = 0;
    m_hCfgFile = EC_NULL;

    m_bErrorHit = EC_FALSE;
    m_dwLastError = EC_E_NOERROR;

    m_bNewDataElement = EC_FALSE;

    m_ECfgParserState = eCfgState_INIT;
    m_ELastCfgParserState = eCfgState_UNKNOWN;
    m_bExecuteDataHandler = EC_FALSE;
    m_bGoingDown = EC_FALSE;
    m_bDataHandlerExecuted = EC_FALSE;
    m_nXmlCharDataBufSize = MAX_XML_DATALEN;
    m_pXmlCharDataBuf = (XML_Char*)OsMalloc(m_nXmlCharDataBufSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat m_pXmlCharDataBuf", m_pXmlCharDataBuf, m_nXmlCharDataBufSize);
    m_nXmlCharLen = 0;
    m_bSmartParseSkipNodes = EC_FALSE;
    m_nSmartParseParentSkipNodeDepth = 0;
    m_dwCurFirstStateChangeEntryIndex = 0xFFFFFFFF;

#if (defined INCLUDE_OEM)
    m_bEncrypted = EC_FALSE;
#endif
    m_poCurCfgSlave = EC_NULL;
    m_apoCfgSlave = EC_NULL;
    m_dwCfgSlaveArraySize = 0;
    m_dwNumCfgSlaves = 0;
    
    m_poCurEniSlave = EC_NULL;

#if (defined INCLUDE_HOTCONNECT)
    m_pCurHotConnectGroup = EC_NULL;
#endif
    /* cyclic commands */
    m_poCurCfgCyclic = EC_NULL;
    m_pCurCyclicFrameDesc = EC_NULL;

    /* copy infos */
    m_poCurCopyInfoEntry = EC_NULL;
    
    m_pbyBufferData = EC_NULL;
    m_wBufferDataLen = 0;
    m_wBufferValidateDataLen = 0;
    m_wBufferValidateDataMaskLen = 0;
    OsMemset(&m_oCurEcatInitCmdDesc, 0, sizeof(_EC_T_ECAT_INIT_CMDS_DESC));
    OsMemset(&m_oMbxInitCmdDesc, 0, sizeof(EC_T_SLAVE_MBX_INIT_CMD_DESC));
    OsMemset(&m_oCurCycCmdCfgDesc, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));
#if (defined INCLUDE_VARREAD)
    OsMemset(&m_oCurProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
#endif
    m_poCfgMaster = m_poEcConfigData->GetCfgMasterDesc();
    OsMemset(m_poCfgMaster, 0, sizeof(EcMasterDesc));
}

/********************************************************************************/
/** \brief
*
* \return
*/
CEcConfigXpat::~CEcConfigXpat()
{
    if (EC_NULL != m_hCfgFile)
    {
        ((IECDataStore*)m_hCfgFile)->Close();
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "~CEcConfigXpat::m_hCfgFile", m_hCfgFile, ((IECDataStore*)m_hCfgFile)->GetThisSize());
        delete(((IECDataStore*)m_hCfgFile));
    }

    if (m_bXmlParserCreated) XPTXML_ParserFree(m_oXmlParser);

    EC_T_DWORD dwSlvIdx;
    for( dwSlvIdx = 0; dwSlvIdx < m_dwNumCfgSlaves; dwSlvIdx++ )
    {
        DeleteXpatSlave( m_apoCfgSlave[dwSlvIdx] );
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::~m_apoCfgSlave", m_apoCfgSlave, m_dwCfgSlaveArraySize * sizeof(EC_T_VOID*));
    SafeDeleteArray( m_apoCfgSlave);
    OsDbgAssert(m_nXmlCharLen == 0 );
    OsDbgAssert(m_poCurCfgCyclic == EC_NULL);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigXpat::~m_pbyBufferData", m_pbyBufferData, m_wBufferDataLen);
    SafeOsFree(m_pbyBufferData);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigXpat::~m_oCurEcatInitCmdDesc.pbyValidateData", m_oCurEcatInitCmdDesc.pbyValidateData, m_wBufferValidateDataLen);
    SafeOsFree(m_oCurEcatInitCmdDesc.pbyValidateData);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigXpat::~m_oCurEcatInitCmdDesc.pbyValidateDataMask", m_oCurEcatInitCmdDesc.pbyValidateDataMask, m_wBufferValidateDataMaskLen);
    SafeOsFree(m_oCurEcatInitCmdDesc.pbyValidateDataMask);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::~m_pXmlCharDataBuf", m_pXmlCharDataBuf, m_nXmlCharDataBufSize);
    SafeOsFree(m_pXmlCharDataBuf);
}

/********************************************************************************/
/** \brief Initializes the EtherCAT configuration object
*
* \return
*/
EC_T_DWORD CEcConfigXpat::Init(EC_T_CONFIGPARM_DESC* pCfgParm)
{
    EC_T_INT        nLen;
    EC_T_DWORD      dwRetVal    = EC_E_NOERROR;
    IECDataStore*   pDS         = EC_NULL;

    m_nDepth = 0;

    switch (pCfgParm->eCnfType)
    {
    case eCnfType_Filename:
        pDS = EC_NEW(CEcFileStore(pCfgParm));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::Init pDS", pDS, sizeof(CEcFileStore));
        break;
    case eCnfType_Data:
    case eCnfType_Datadiag:
        pDS = EC_NEW(CEcMemStore(pCfgParm));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigXpat::Init pDS", pDS, sizeof(CEcMemStore));
        break;
    case eCnfType_None:
        dwRetVal = EC_E_NOERROR;
        goto Exit;
    default:
        EC_ERRORMSG(("CEcConfigXpat::Init() Unhandled paramter type!\n"));
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    m_hCfgFile = (EC_T_PVOID)pDS;   

    if (EC_NULL == pDS->Open())
    {
        EC_ERRORMSGC(("CEcConfigXpat::Init() Error reading file %s (e.g. not found)!\n", (EC_T_CHAR*)pCfgParm->pbyData));
        dwRetVal = EC_E_CFGFILENOTFOUND;
        goto Exit;
    }

    nLen = pDS->ReadData(m_achBuff, CFGFILE_BUFSIZE);

    if (pDS->Error())
    {
        dwRetVal = EC_E_CONFIGDATAREAD;
        EC_ERRORMSGC(("CEcConfigXpat::Init() Configuration file read error!\n"));
        goto Exit;
    }

    if ((0 == nLen) && (pDS->EndOfData()))
    {
        dwRetVal = EC_E_WRONG_FORMAT;
        EC_ERRORMSGC(("CEcConfigXpat::Init() Empty configuration file!\n"));
        goto Exit;
    }

    /* XML configuration file, parse file */
    m_oXmlParser = XPTXML_ParserCreate_MM(EC_NULL, EC_NULL, GetMemLog());
    m_bXmlParserCreated = EC_TRUE;
    ::XPTXML_SetUserData( m_oXmlParser, (void*)this );
    ::XPTXML_SetElementHandler( m_oXmlParser, ::StartElementHandler, ::EndElementHandler );
    ::XPTXML_SetCharacterDataHandler( m_oXmlParser, ::CharacterDataHandler );


    for (;;)
    {
        if (::XPTXML_Parse(m_oXmlParser, m_achBuff, nLen, pDS->EndOfData()) == XML_STATUS_ERROR)
        {
            EC_ERRORMSGC(("CEcConfigXpat::Init() Parse error at line %d col %d\n%s\n",
                    ::XPTXML_GetCurrentLineNumber(m_oXmlParser),
                    ::XPTXML_GetCurrentColumnNumber(m_oXmlParser),
                    ::XPTXML_ErrorString(::XPTXML_GetErrorCode(m_oXmlParser))));
            EC_ERRORMSGC(("Data Length %d CurrentLength %d\n",
                    pCfgParm->dwLen,
                    nLen
                    ));

            dwRetVal = EC_E_WRONG_FORMAT;
            goto Exit;
        }

        if (m_bErrorHit)
        {
            /* One possible cause to be here is if the configuration contains hot connect tags although INCLUDE_HOTCONNECT is disabled */
            EC_ERRORMSGC(("CEcConfigXpat::Init() Parser error : 0x%lx\n", m_dwLastError));
            dwRetVal = m_dwLastError;
            goto Exit;
        }

        nLen = pDS->ReadData(m_achBuff, CFGFILE_BUFSIZE);

        if (pDS->Error())
        {
            EC_ERRORMSG(("CEcConfigXpat::Init() Read error!\n"));
            goto Exit;
        }

        if ((0 == nLen) && (pDS->EndOfData()))
        {
            break;
        }
    }

    if (EC_NULL != m_hCfgFile)
    {
        m_poEcConfigData->SetConfigCheckSum(((IECDataStore*)m_hCfgFile)->ChkSum());
    }

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_VARREAD)
/***************************************************************************************************/
/**
\brief Converts the data type name of the ENI file to a numeric identifier
*/
EC_T_WORD CEcConfigXpat::DataTypeName2DataTypeIdentifier(
    EC_T_BYTE* szDataTypeName)                  /**< [in]  Data type name (string) */
{
    EC_T_WORD wDataType = DEFTYPE_NULL;

    /* Assign Base Data Type Index. See ETG.1020 Protocol Enhancements, chapter 23, table 92. */
         if (OsStrcmp(szDataTypeName, "BOOL") == 0)         wDataType = DEFTYPE_BOOLEAN;
    else if (OsStrcmp(szDataTypeName, "BOOLEAN") == 0)      wDataType = DEFTYPE_BOOLEAN;
    else if (OsStrcmp(szDataTypeName, "BIT") == 0)          wDataType = DEFTYPE_BOOLEAN;
    else if (OsStrcmp(szDataTypeName, "BYTE") == 0)         wDataType = DEFTYPE_BYTE;
    /* This conversion is deprecated. DEFTYPE_BYTE is correct. Conversion kept in V2.7 for compatibility. */
    else if (OsStrcmp(szDataTypeName, "BYTE") == 0)         wDataType = DEFTYPE_UNSIGNED8;
    else if (OsStrcmp(szDataTypeName, "WORD") == 0)         wDataType = DEFTYPE_WORD;
    else if (OsStrcmp(szDataTypeName, "DWORD") == 0)        wDataType = DEFTYPE_DWORD;
    else if (OsStrcmp(szDataTypeName, "BIT1") == 0)         wDataType = DEFTYPE_BIT1;
    else if (OsStrcmp(szDataTypeName, "BIT2") == 0)         wDataType = DEFTYPE_BIT2;
    else if (OsStrcmp(szDataTypeName, "BIT3") == 0)         wDataType = DEFTYPE_BIT3;
    else if (OsStrcmp(szDataTypeName, "BIT4") == 0)         wDataType = DEFTYPE_BIT4;
    else if (OsStrcmp(szDataTypeName, "BIT5") == 0)         wDataType = DEFTYPE_BIT5;
    else if (OsStrcmp(szDataTypeName, "BIT6") == 0)         wDataType = DEFTYPE_BIT6;
    else if (OsStrcmp(szDataTypeName, "BIT7") == 0)         wDataType = DEFTYPE_BIT7;
    else if (OsStrcmp(szDataTypeName, "BIT8") == 0)         wDataType = DEFTYPE_BIT8;
    else if (OsStrcmp(szDataTypeName, "BITARR8") == 0)      wDataType = DEFTYPE_BITARR8;
    else if (OsStrcmp(szDataTypeName, "BITARR16") == 0)     wDataType = DEFTYPE_BITARR16;
    else if (OsStrcmp(szDataTypeName, "BITARR32") == 0)     wDataType = DEFTYPE_BITARR32;
    else if (OsStrcmp(szDataTypeName, "INTEGER8") == 0)     wDataType = DEFTYPE_INTEGER8;
    else if (OsStrcmp(szDataTypeName, "SINT") == 0)         wDataType = DEFTYPE_INTEGER8;
    else if (OsStrcmp(szDataTypeName, "INTEGER16") == 0)    wDataType = DEFTYPE_INTEGER16;
    else if (OsStrcmp(szDataTypeName, "INT") == 0)          wDataType = DEFTYPE_INTEGER16;
    else if (OsStrcmp(szDataTypeName, "INTEGER24") == 0)    wDataType = DEFTYPE_INTEGER24;
    else if (OsStrcmp(szDataTypeName, "INT24") == 0)        wDataType = DEFTYPE_INTEGER24;
    else if (OsStrcmp(szDataTypeName, "INTEGER32") == 0)    wDataType = DEFTYPE_INTEGER32;
    else if (OsStrcmp(szDataTypeName, "DINT") == 0)         wDataType = DEFTYPE_INTEGER32;
    else if (OsStrcmp(szDataTypeName, "INTEGER40") == 0)    wDataType = DEFTYPE_INTEGER40;
    else if (OsStrcmp(szDataTypeName, "INT40") == 0)        wDataType = DEFTYPE_INTEGER40;
    else if (OsStrcmp(szDataTypeName, "INTEGER48") == 0)    wDataType = DEFTYPE_INTEGER48;
    else if (OsStrcmp(szDataTypeName, "INT48") == 0)        wDataType = DEFTYPE_INTEGER48;
    else if (OsStrcmp(szDataTypeName, "INTEGER56") == 0)    wDataType = DEFTYPE_INTEGER56;
    else if (OsStrcmp(szDataTypeName, "INT56") == 0)        wDataType = DEFTYPE_INTEGER56;
    else if (OsStrcmp(szDataTypeName, "INTEGER64") == 0)    wDataType = DEFTYPE_INTEGER64;
    else if (OsStrcmp(szDataTypeName, "LINT") == 0)         wDataType = DEFTYPE_INTEGER64;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED8") == 0)    wDataType = DEFTYPE_UNSIGNED8;
    else if (OsStrcmp(szDataTypeName, "USINT") == 0)        wDataType = DEFTYPE_UNSIGNED8;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED16") == 0)   wDataType = DEFTYPE_UNSIGNED16;
    else if (OsStrcmp(szDataTypeName, "UINT") == 0)         wDataType = DEFTYPE_UNSIGNED16;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED24") == 0)   wDataType = DEFTYPE_UNSIGNED24;
    else if (OsStrcmp(szDataTypeName, "UINT24") == 0)       wDataType = DEFTYPE_UNSIGNED24;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED32") == 0)   wDataType = DEFTYPE_UNSIGNED32;
    else if (OsStrcmp(szDataTypeName, "UDINT") == 0)        wDataType = DEFTYPE_UNSIGNED32;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED40") == 0)   wDataType = DEFTYPE_UNSIGNED40;
    else if (OsStrcmp(szDataTypeName, "UINT40") == 0)       wDataType = DEFTYPE_UNSIGNED40;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED48") == 0)   wDataType = DEFTYPE_UNSIGNED48;
    else if (OsStrcmp(szDataTypeName, "UINT48") == 0)       wDataType = DEFTYPE_UNSIGNED48;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED56") == 0)   wDataType = DEFTYPE_UNSIGNED56;
    else if (OsStrcmp(szDataTypeName, "UINT56") == 0)       wDataType = DEFTYPE_UNSIGNED56;
    else if (OsStrcmp(szDataTypeName, "UNSIGNED64") == 0)   wDataType = DEFTYPE_UNSIGNED64;
    else if (OsStrcmp(szDataTypeName, "ULINT") == 0)        wDataType = DEFTYPE_UNSIGNED64;
    else if (OsStrcmp(szDataTypeName, "REAL32") == 0)       wDataType = DEFTYPE_REAL32;
    else if (OsStrcmp(szDataTypeName, "REAL") == 0)         wDataType = DEFTYPE_REAL32;
    else if (OsStrcmp(szDataTypeName, "REAL64") == 0)       wDataType = DEFTYPE_REAL64;
    else if (OsStrcmp(szDataTypeName, "LREAL") == 0)        wDataType = DEFTYPE_REAL64;
    else if (OsStrncmp(szDataTypeName, "STRING", 6) == 0)   wDataType = DEFTYPE_VISIBLESTRING;
    else if (OsStrcmp(szDataTypeName, "GUID") == 0)         wDataType = DEFTYPE_GUID;
    else if (OsStrncmp(szDataTypeName, "ARRAY", 5) == 0)
    {
        /* skip "ARRAY [0..XXX] OF " */
        while (*szDataTypeName != ']' && *szDataTypeName != '\0')
        {
            szDataTypeName++;
        }
        szDataTypeName += 5;
             if (OsStrcmp(szDataTypeName, "BYTE")  == 0)    wDataType = DEFTYPE_ARRAY_OF_BYTE;
        else if (OsStrcmp(szDataTypeName, "UINT")  == 0)    wDataType = DEFTYPE_ARRAY_OF_UINT;
        else if (OsStrcmp(szDataTypeName, "INT")   == 0)    wDataType = DEFTYPE_ARRAY_OF_INT;
        else if (OsStrcmp(szDataTypeName, "SINT")  == 0)    wDataType = DEFTYPE_ARRAY_OF_SINT;
        else if (OsStrcmp(szDataTypeName, "DINT")  == 0)    wDataType = DEFTYPE_ARRAY_OF_DINT;
        else if (OsStrcmp(szDataTypeName, "UDINT") == 0)    wDataType = DEFTYPE_ARRAY_OF_UDINT;
    }
    /* See ETG.1020 Protocol Enhancements, chapter 23, table 93, 94. */

    return wDataType;
}
#endif /*(defined INCLUDE_VARREAD)*/

#endif /*(defined INCLUDE_XPAT)*/
/*-END OF SOURCE FILE--------------------------------------------------------*/
