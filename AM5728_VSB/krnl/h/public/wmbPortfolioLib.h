/* wmbPortfolioLib.h - Portfolio Manager definitions */

/* Copyright (c) 2004 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01b,31jan13,r_w  disable portfolio feature
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#if 0 /* disable portfolio feature */
#ifndef __INCwmbPortfolioLibh
#define __INCwmbPortfolioLibh

/* WindMark Definitions */
#define WMB_PORTFOLIO_REG           1
#define WMB_PORTFOLIO_GET           2
#define WMB_PORTFOLIO_DEL           3
#define WMB_PORTFOLIO_REG_NAME      "WMB_PORTFOLIO_wmbPortfolioRegister"
#define WMB_PORTFOLIO_GET_NAME      "WMB_PORTFOLIO_wmbPortfolioRequest"
#define WMB_PORTFOLIO_DEL_NAME      "WMB_PORTFOLIO_wmbPortfolioDeregister"

/*
Portfolio manager Event definitions. These events are placed in the eventID of the
WMB_EVENT_T structure. The eventInfo[0] field contains the component ID.
*/
#define WMB_PORTFOLIO_CREATED_EVENT    0x0001
#define WMB_PORTFOLIO_DELETED_EVENT    0x0002


/* XML tags */
#define WMB_XML_PORTFOLIO   "PORTFOLIO"
#define WMB_XML_PCB         "PCB"
#define WMB_XML_CID         "CID"
#define WMB_XML_PTYPE       "TYPE"
#define WMB_XML_PFLAGS      "FLAGS"
#define WMB_XML_PFILE       "FILE"

#define WMB_XML_REQUEST     "REQUEST"
#define WMB_XML_BLADE       "BLADE"
#define WMB_XML_EXTENSION   "EXTENSION"

#define WMB_XML_CID_TAG         1
#define WMB_XML_PTYPE_TAG       2
#define WMB_XML_PFILE_TAG       3
#define WMB_XML_PFLAGS_TAG      4
#define WMB_XML_BLADE_TAG       5
#define WMB_XML_EXTENSION_TAG   6

#define XML_HEADER1 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define DTD_HEADER1  "<!DOCTYPE PORTFOLIO SYSTEM \"\">\n"


typedef enum
    {
    WMB_PTYPE_UNKNOWN = 0,
    WMB_PTYPE_FILE,
    WMB_PTYPE_WINDMARK
    }
    WMB_PORTFOLIO_TYPE_T;

typedef sbyte * WMB_FILENAME_T;

/*
Portfolio Control Block.
*/

typedef struct WMB_PORTFOLIO_S
    {
    WMB_CID_T               cid;            /* unique id for this component */
    WMB_PORTFOLIO_TYPE_T    type;           /* type of this portfolio */
    sbyte *                 fileName;       /* filename of portfolio location */
    ubyte4                  flags;          /* flags associated with this portfolio */
    ubyte2                  currentTag;     /* For XML processing           */
    struct WMB_PORTFOLIO_S *pListNext;     /* Next portfolio CB in the selected filter */
    struct WMB_PORTFOLIO_S *pNext;         /* Next portfolio CB in the list    */
    }
    WMB_PORTFOLIO_T;

typedef struct WMB_PCB_ANCHOR_S
    {
    ubyte2              numPCB;     /* Number of PCBs       */
    WMB_PORTFOLIO_T *   pPCBList;  /* Linked list of PCBs  */
    }
    WMB_PCB_ANCHOR_T;

typedef struct WMB_PM_REQUEST_S
    {
    WMB_CID_T       cid;            /* id for this component          */
    sbyte *         fileName;       /* filename of portfolio location */
    sbyte *         blade;          /* Blade name                     */
    sbyte *         extension;      /* Extension                      */
    ubyte2          currentTag;     /* For XML processing           */
    }WMB_PM_REQUEST_T;


/*
ACCESS MACROS
*/
#define wmbPortfolioCBCIDGet(x)         (x->cid)
#define wmbPortfolioCBCIDSet(x, y)      (x->cid = y)
#define wmbPortfolioCBtypeGet(x)        (x->type)
#define wmbPortfolioCBtypeSet(x,y)      (x->type = y)
#define wmbPortfolioCBfileNameGet(x)    (x->fileName)
#define wmbPortfolioCBfileNameSet(x, y) (x->fileName = y)
#define wmbPortfolioCBflagIsSet(x, y)   ((x->flags & y) == y)
#define wmbPortfolioCBflagSet(x, y)     (x->flags &= y)
#define wmbPortfolioCBlistNextGet(x)    (x->pListNext)
#define wmbPortfolioCBlistNextSet(x, y) (x->pListNext = y)
#define wmbPortfolioCBNextGet(x)        (x->pNext)
#define wmbPortfolioCBNextSet(x, y)     (x->pNext = y)


/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmbPortfolioStart (void);
STATUS  wmbPortfolioStop  (void);
STATUS  wmbPortfolioNew (WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioFromCIDGet (WMB_CID_T cid, WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioFromFileNameGet (sbyte * pFileName, WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioRegister (WMB_CID_T component, WMB_PORTFOLIO_TYPE_T type,
                              WMB_FILENAME_T fileName, ubyte4 flags);
void    wmbPortfolioDeregister (WMB_CID_T cid, WMB_FILENAME_T filename);
STATUS  wmbPortfolioGet (sbyte * pBlade, sbyte * pExtension, WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioHandler (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T cmd);
STATUS  wmbMakePCBfromXML (sbyte * pXml, WMB_PORTFOLIO_T * * pPcb);
STATUS  wmbMakeRequestfromXML (sbyte * pXml, WMB_PM_REQUEST_T * pPrcb);
STATUS  wmbMakeXMLFromPCB (WMB_PORTFOLIO_T * pPcb, sbyte * * ppXml);
STATUS  wmbMakeXMLFromRequest (WMB_PM_REQUEST_T * pPrcb, sbyte * * ppXml);
STATUS  wmbPortfolioDocPCBHandler (XML_ENV_T * pXmlEnv, SAX_FUNCTION_T functionID,
                                   XML_DATUM_T * pXmlData);
STATUS  wmbPortfolioDocREQHandler (XML_ENV_T * pXmlEnv, SAX_FUNCTION_T functionID,
                                   XML_DATUM_T * pXmlData);
STATUS  wmbPortfolioFromCIDGetRemote (WMB_CID_T cid, WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioFromFileNameGetRemote (sbyte * fileName, WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioGetRemote (sbyte * pBlade, sbyte * pExtension,
                               WMB_PORTFOLIO_T * * ppPcb);
STATUS  wmbPortfolioRequestRemote ( WMB_PM_REQUEST_T * pmReq, WMB_PORTFOLIO_T * * ppPcb);
void    wmbPortfolioDeregisterRemote (WMB_CID_T cid, WMB_FILENAME_T filename);
STATUS  wmbPortfolioRegisterRemote (WMB_CID_T component, WMB_PORTFOLIO_TYPE_T type,
                                    WMB_FILENAME_T fileName, ubyte4 flags);
STATUS  wmbPortfolioRegDeregRemote (WMB_PORTFOLIO_T * pmPort, sbyte * pWindMark);

#ifdef __cplusplus
}
#endif

#define wmbPortfolioInit    wmbPortfolioStart

#endif /* __INCwmbPortfolioLibh */
#endif
