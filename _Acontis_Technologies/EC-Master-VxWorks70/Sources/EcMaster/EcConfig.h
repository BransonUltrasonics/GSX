/*-----------------------------------------------------------------------------
 * EcConfig.h               header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description				
 *---------------------------------------------------------------------------*/

#ifndef INC_ECCONFIG
#define INC_ECCONFIG

/*-INCLUDES------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/

typedef struct _EC_T_CONFIGPARM_DESC
{
  EC_T_CNF_TYPE     eCnfType;       /*< Type of configuration data (file/stream) */
  EC_T_PBYTE        pbyData;        /*< Configuration data */
  EC_T_DWORD        dwLen;          /*< length of configuration data */
} EC_T_CONFIGPARM_DESC, *EC_PT_CONFIGPARM_DESC;

typedef struct _ECX_T_ADDRRANGE
{
    EC_T_WORD   wBegin;
    EC_T_WORD   wEnd;
} ECX_T_ADDRRANGE;


/*-CLASS---------------------------------------------------------------------*/

class CEcCRC32
{
public:
    CEcCRC32()
    {
        m_dwStartingValue = 0xffffffff;
    };
    ~CEcCRC32(){};

    EC_T_VOID Init(EC_T_VOID)   { m_dwCrcSum = m_dwStartingValue; }
    EC_T_VOID AddData(EC_T_BYTE* pbyData, EC_T_DWORD dwLen);

    EC_T_DWORD GetSum( EC_T_VOID )  { return m_dwCrcSum;}

    EC_T_VOID SetSum( EC_T_DWORD dwCrcSum ) { m_dwCrcSum = dwCrcSum; }
private:
    EC_T_DWORD m_dwCrcSum;
    EC_T_DWORD m_dwStartingValue;
};


#endif /* INC_ECCONFIG */

/*-END OF SOURCE FILE--------------------------------------------------------*/
