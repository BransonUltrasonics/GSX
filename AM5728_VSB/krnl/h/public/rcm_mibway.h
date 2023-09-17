/* rcm_mibway.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

/*----------------------------------------------------------------------
 *
 * NAME CHANGE NOTICE:
 *
 * On May 11th, 1999, Rapid Logic changed its corporate naming scheme.
 * The changes are as follows:
 *
 *      OLD NAME                        NEW NAME
 *
 *      OpenControl                     RapidControl
 *      WebControl                      RapidControl for Web
 *      JavaControl                     RapidControl for Applets
 *      MIBway                          MIBway for RapidControl
 *
 *      OpenControl Backplane (OCB)     RapidControl Backplane (RCB)
 *      OpenControl Protocol (OCP)      RapidControl Protocol (RCP)
 *      MagicMarkup                     RapidMark
 *
 * The source code portion of our product family -- of which this file
 * is a member -- will fully reflect this new naming scheme in an upcoming
 * release.
 *
 *
 * RapidControl, RapidControl for Web, RapidControl Backplane,
 * RapidControl for Applets, MIBway, RapidControl Protocol, and
 * RapidMark are trademarks of Rapid Logic, Inc.  All rights reserved.
 *
 */
/*
$History: rcm_mibway.h $
 *
 * *****************  Version 14  *****************
 * User: James        Date: 10/12/00   Time: 2:34p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added prototype for new function.
 *
 * *****************  Version 13  *****************
 * User: James        Date: 6/20/00    Time: 2:28p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Cleaned up banner info.
 *
 * User: James        Date: 5/18/00    Time: 5:48p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added support for dynamic MIB lookup tables. JAB
 *
 * User: Epeterson    Date: 4/25/00    Time: 2:25p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Include history and enable auto archiving feature from VSS


*/
#ifndef __MIBWAY_HEADER__
#define __MIBWAY_HEADER__

/* constants */
#define kNoSuchInstance	100

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

/* prototypes */
void OCSNMP_CreateTable(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuf,
                               void *pDataObjectInC, sbyte *pHtmlArgs);

void OCSNMP_EndRow(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuf,
                          void *pDataObjectInC, sbyte *pHtmlArgs);

void OCSNMP_EndTable(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuf,
                            void *pDataObjectInC, sbyte *pHtmlArgs);

void OCSNMP_TableNext(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuf,
                             void *pDataObjectInC, sbyte *pHtmlArgs);

void OCSNMP_GetInstance(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuffer,
                               void *pDataObjectInC, sbyte *pHtmlArgs);

#ifdef __MIBWAY_ENABLE_STRING_INSTANCE__
void OCSNMP_GetAsciiInstance(WMB_SESSION_T_WMM *p_envVar, void *pOutputBuffer,
                                    void *pDataObjectInC, sbyte *pArgs);
#endif

STATUS OCSNMP_MoreDataInTable(WMB_SESSION_T_WMM *p_envVar, sbyte *pTableName,
                                       BOOL *pMoreData);


void OCSNMP_BuildFullMibName(WMB_SESSION_T_WMM *p_envVar, sbyte *pRetFullMibName,
                                    sbyte *pMagicMibMarkup, sbyte *pHtmlArgs,
                                    BOOL *p_boolInTable);

STATUS OCSNMP_ConstructSnmpEnviron(WMB_SESSION_T_WMM *p_envVar);
STATUS OCSNMP_SetPostFlag(WMB_SESSION_T_WMM *p_envVar);
void		OCSNMP_UndoSetList(WMB_SESSION_T_WMM *p_envVar);

#ifdef __RLI_DYNAMIC_MIB_LOOKUP_SUPPORT__
void
MIBWAY_GetPrim_AddMibLookup(WMB_SESSION_T_WMM *p_envVar, void *pHtmlOutputBuf, void *pSrcObject, char *pArgs);

int
MIBWAY_SetPrim_AddMibLookup(WMB_SESSION_T_WMM *p_envVar, void *pDestObject, void *pInputData, char *pArgs);

void MIBWAY_FreeDynamicLookup(void *);

#endif /* __RLI_DYNAMIC_MIB_LOOKUP_SUPPORT__ */

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __MIBWAY_HEADER__ */
