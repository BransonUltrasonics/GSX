/* rcm_EV_CNV.h    (OpenControl to Envoy Display Conversion Methods) */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
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
$History: rcm_ev_cnv.h $
 *
 * *****************  Version 8  *****************
 * User: James        Date: 6/20/00    Time: 2:28p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Cleaned up banner info.
 *
 * User: Epeterson    Date: 4/25/00    Time: 2:25p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Include history and enable auto archiving feature from VSS


*/
#ifndef __OCEV_CNV_HEADER__
#define __OCEV_CNV_HEADER__

#ifdef __SNMP_API_ENABLED__

/* typedefs */
typedef void *(*ConvertHtmlToBin)(void *, short, sbyte *, void *);
typedef void  (*DisplayBinToHtml)(void *, short, void *, char *, BOOL);

/* prototypes */

extern DisplayBinToHtml OCEV_CNV_GetBinToHtmlMethod(short SNMP_DataType);
extern ConvertHtmlToBin OCEV_CNV_GetHtmlToBinMethod(short SNMP_DataType);

extern void OCSNMP_OverloadGetMethod(void *p_envVar,
                                     sbyte *pHtmlArgs, sbyte *pHtmlOutputBuf,
                                     DisplayBinToHtml p_funcDisplayBinToHtml);

extern int OCSNMP_OverloadSetMethod(void *p_envVar,
                                    sbyte *pHtmlArgs, sbyte *pHtmlInputBuf,
                                    ConvertHtmlToBin p_funcConvertHtmlToBin);

#endif /* __SNMP_API_ENABLED__ */

#endif /* __OCEV_CNV_HEADER__ */

