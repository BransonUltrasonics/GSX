/*-----------------------------------------------------------------------------
 * CString.h                header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              Header for class C_String
 *---------------------------------------------------------------------------*/

#ifndef INC_STRING
#define INC_STRING

/*-DEFINES-------------------------------------------------------------------*/
#define TYPECSTRING     EC_T_STRING
#define TYPEPCSTRING    EC_T_STRING*

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef struct _EC_T_STRING 
{
    EC_T_DWORD  dwLen;
    EC_T_CHAR*  pData;
} EC_T_STRING;

/*-FUNCTION-DECLARATIONS-----------------------------------------------------*/
EC_T_INT    GetStringLen(               EC_T_STRING*        szString        );
EC_T_STRING*    CreateString(       const EC_T_CHAR*    szString,
                                    const EC_T_DWORD    dwLen,
                                    struct _EC_T_MEMORY_LOGGER* pMLog);
EC_T_CHAR*      GetString(          const EC_T_STRING*  szString        );

EC_T_VOID   CreateUninitializedString(  EC_T_STRING&        szString        );
EC_T_DWORD  InitString(                 EC_T_STRING&        szString,
                                        const EC_T_CHAR*    szInit,
                                        const EC_T_DWORD    dwLen,
                                        struct _EC_T_MEMORY_LOGGER* pMLog);
EC_T_VOID   DeleteStatString(           EC_T_STRING&        szString,
                                        struct _EC_T_MEMORY_LOGGER* pMLog);
EC_T_BOOL   IsEqualString(              const EC_T_STRING*  szString,
                                        const EC_T_CHAR*    szComp,
                                        const EC_T_DWORD    dwLen           );
#endif /* INC_STRING */

/*-END OF SOURCE FILE--------------------------------------------------------*/
