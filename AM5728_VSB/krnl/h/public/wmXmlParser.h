/* wmXmlParser.h - definitions for httpXmlParser.c */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01b,20feb04,jws  copyright added
01a,17jan02,jc   imported, renamed and modified from RCX codebase
*/


#ifndef __INCwmXmlParserh
#define __INCwmXmlParserh


#define CHAR_NULL           '\0'
#define CHAR_SPACE          ' '
#define CHAR_TAB            '\t'
#define CHAR_NEWLINE        '\n'
#define CHAR_RETURN         '\r'
#define CHAR_LT             '<'
#define CHAR_GT             '>'
#define CHAR_EXCLAMATION    '!'
#define CHAR_SLASH          '/'
#define CHAR_EQ             '='
#define CHAR_QUOTE          '\"'
#define CHAR_OPENBR         '['


#define STRING_XML              "<?xml"
#define STRING_ENDXML           "?>"
#define STRING_LTQM             "<?"
#define STRING_QMGT             "?>"
#define STRING_LTGT             "<>"
#define STRING_LTEX             "<!"
#define STRING_EXGT             "!>"
#define STRING_LTSL             "</"
#define STRING_SLGT             "/>"
#define STRING_BEGINCOMMENT     "<!--"
#define STRING_ENDCOMMENT       "--!>"
#define STRING_NLRET            "\n\r"
#define STRING_WHITESPACE       " \t\n\r"
#define STRING_XMLSYMBOLS       "<>&\"'"
#define STRING_DOCTYPE          "<!DOCTYPE"
#define STRING_SYSTEM           "SYSTEM"
#define STRING_PUBLIC           "PUBLIC"
#define STRING_GTNL             ">\n"


/* SAX function definitions */

typedef enum SAX_FUNCTION_ID_S
    {
    SAX_INVALID_K,
    SAX_START_DOCUMENT_K,
    SAX_PRESCAN_K,
    SAX_END_DOCUMENT_K,
    SAX_START_ELEMENT_K,
    SAX_END_ELEMENT_K,
    SAX_ATTRIBUTE_K,
    SAX_CHARACTERS_K,
    SAX_IGNORABLE_WHITESPACE_K,
    SAX_PROCESSING_INSTRUCTION_K
    }
    SAX_FUNCTION_T;

typedef struct XML_DATUM_T
    {
    struct XML_DATUM_T * pPrev;
    struct XML_DATUM_T * pNext;
    sbyte *     pStart;         /* start of working data    */
#ifndef _WRS_CONFIG_LP64
    sbyte4      length;         /* length of working data   */
#else
    sbyte8      length;         /* length of working data   */
#endif
    sbyte *     pName;
#ifndef _WRS_CONFIG_LP64
    sbyte4      nameLen;
#else
    sbyte8      nameLen;
#endif
    sbyte *     pValue;
#ifndef _WRS_CONFIG_LP64
    sbyte4      valueLen;
#else
    sbyte8      valueLen;
#endif
#ifndef _WRS_CONFIG_LP64
    sbyte4      attributes;
#else
    sbyte8      attributes;
#endif
    sbyte4      type;
    }
    XML_DATUM_T;


typedef struct XML_ENV_T
    {
    XML_DATUM_T *   pRoot;
    sbyte *         pXmlDocument; /* buffer containing xml document */
#ifndef _WRS_CONFIG_LP64
    sbyte4          xmlDocumentSize;
#else
    sbyte8          xmlDocumentSize;
#endif
    sbyte *         p_DTDUrl;
    MEM_POOL_ID     pool_id;
    void *          pCallBack;
    void *          pState;         /* storage for parsed data */
    void *          pCookie;        /* storage reserved for the the caller's use */
    }
    XML_ENV_T;

typedef STATUS (*SAX_CALLBACK_T)(XML_ENV_T * pXmlEnv, SAX_FUNCTION_T, XML_DATUM_T * pDatum);

/* Function Prototypes */


#ifdef __cplusplus
extern "C" {
#endif

void    wmXmlParserTransformToXMLString (sbyte ** srcstr);
STATUS  wmXmlParserGetAttribute (XML_ENV_T * pXmlEnv);
#ifndef _WRS_CONFIG_LP64
void    wmXmlParserGetAttributeCount (XML_ENV_T * pXmlEnv, sbyte * pElement,
                                   sbyte4 * pNumAttr);
#else
void    wmXmlParserGetAttributeCount (XML_ENV_T * pXmlEnv, sbyte * pElement,
                                   sbyte8 * pNumAttr);
#endif
STATUS  wmXmlParserDocument (XML_ENV_T * pXmlEnv);
#ifndef _WRS_CONFIG_LP64
STATUS  wmXmlParserStart (XML_ENV_T * pXmlEnv, SAX_CALLBACK_T pDocHandler,
                          sbyte *pBuffer, ubyte4 bufSize);
#else
STATUS  wmXmlParserStart (XML_ENV_T * pXmlEnv, SAX_CALLBACK_T pDocHandler,
                          sbyte *pBuffer, ubyte8 bufSize);
#endif
void wmXmlParserStop(XML_ENV_T *pXmlEnv);

#ifdef __cplusplus
}
#endif

#endif /* __INCwmXmlParserh */
