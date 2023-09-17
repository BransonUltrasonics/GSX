/*-----------------------------------------------------------------------------
 * CEcIoImage.h             header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              interface for the CIoImage class.
 *---------------------------------------------------------------------------*/

#ifndef INC_ECIOIMAGE
#define INC_ECIOIMAGE


class CEcIoImage
{
public:
    static  CEcIoImage*     CreateInstance( EC_T_INT        nIn, 
                                            EC_T_INT        nOut, 
                                            struct _EC_T_MEMORY_LOGGER* pMLog);
    static EC_T_VOID DestroyInstance(CEcIoImage* pIns, struct _EC_T_MEMORY_LOGGER* pMLog);

    EC_T_BYTE* GetInputPtr(  EC_T_DWORD offs, EC_T_DWORD size );
    EC_T_BYTE* GetOutputPtr( EC_T_DWORD offs, EC_T_DWORD size );
    EC_T_DWORD GetInputSize()  { return m_nIn; }
    EC_T_DWORD GetOutputSize() { return m_nOut;}	
    EC_T_BOOL  AllocOk()       { return (((0==m_nIn)||(EC_NULL!=m_pIn))&&((0==m_nOut)||(EC_NULL!=m_pOut))); }

protected:
	CEcIoImage(EC_T_DWORD nIn, EC_T_DWORD nOut, struct _EC_T_MEMORY_LOGGER* pMLog);
    ~CEcIoImage();

	EC_T_BYTE*			m_pIn;
	EC_T_BYTE*			m_pOut;
	EC_T_DWORD			m_nIn;
	EC_T_DWORD			m_nOut;
};


#endif /* INC_ECIOIMAGE */
