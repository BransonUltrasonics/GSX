/* httpXmlParser.c - Low level functions to parse XML docs */

/* Copyright 1999-2007,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
-------------------- 
01d,02feb10,h_y   support for LP64 
01c,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01b,13feb07,msa  Fix comments to remove APIgen errors.
01a,17jan02,jc   imported, renamed and modified from RCX codebase
*/

/*
DESCRIPTION
This module contains low level XML parsing functions.

INCLUDES FILES: httpLib.h
    httpSaxC.h
    httpXmlParser.h

*/

/**- include files **/
#include "httpLib.h"
#include "httpSaxC.h"
#include "httpXmlParser.h"
#include <string.h>

/**- local definitions **/

/**- default settings **/

/**- external functions **/

/**- external data **/

/**- internal functions **/

/**- public data **/

/**- private data **/
static sbyte *g_rootElem = NULL;
static BOOL g_bRoot = FALSE;
static MEM_POOL_ID g_memPool = NULL;
/**- private functions **/

static void xmlParseSkipSpace
(
    sbyte   **dataptr   /* [in/out] Adjust the buffer to skip over whitespace */
)
{
    while (NULL != *dataptr)
    {
        sbyte c = (*dataptr)[0];
        if (c == CHAR_SPACE ||
            c == CHAR_TAB ||
            c == CHAR_NEWLINE ||
            c == CHAR_RETURN)
            ++(*dataptr);
        else
            break;
    }

}/* xmlParseSkipSpace */

/*******************************************************************************
*
* xmlParseReplaceString - Replace certain charcters with others in string.
*
* DESCRIPTION:
*      Replace certain charcters with others in string.
*
* RETURNS:
*      none
* NOTES:
*
*
* NOMANUAL
*/
static void xmlParseReplaceString
(
    const sbyte *srcstr,    /* [in] Pointer to the source string                    */
    sbyte       **dest,     /* [out] The pointer to the target string               */
    const sbyte *oldstr,    /* [in] String to be replaced                           */
    const sbyte *newstr     /* [in] String to replace with                          */
)
{
    const sbyte   *src1 = srcstr;
    sbyte   *found;
    sbyte2  diff, length, i = 0;


    *dest = (sbyte*)srcstr;
    if (NULL == srcstr)
        return;
    diff = (sbyte2)(strlen((sbyte*)newstr) - strlen((sbyte*)oldstr));
    found = strstr(src1, (sbyte*)oldstr);
    while (NULL != found)
    {
        src1 = found + strlen((sbyte*)oldstr);
        found = strstr(src1, (sbyte*)oldstr);
        i++;
    }
    if (i > 0)
    {
        diff = (sbyte2) (diff * i);
        length = (sbyte2) strlen(((sbyte*)srcstr) + diff + 1);
        *dest = memPoolAlloc(g_memPool, (ubyte2) length + strlen((sbyte*)srcstr) + 1);
        if (NULL == *dest)
            return;
        memset(*dest, 0, (ubyte2) length + strlen((sbyte*)srcstr) + 1);
        **dest = CHAR_NULL;
        src1 = srcstr;
        found = strstr(src1, (sbyte*)oldstr);
        while (NULL != found)
        {
            strncat(*dest, src1, (size_t)(found - src1));
            strcat(*dest, (sbyte*)newstr);
            src1 = found + strlen((sbyte*)oldstr);
            found = strstr(src1, (sbyte*)oldstr);
        }
        strcat(*dest, src1);
        (void)memPoolFree(g_memPool, (sbyte*)srcstr);
    }

}/* xmlParseReplaceString */

/*******************************************************************************
*
* xmlParseTransformXMLToString - Transform XML data to string.
*
* DESCRIPTION:
*      Transform XML data to string.
*
* RETURNS:
*      none
* NOTES:
*
*
* NOMANUAL
*/
static void xmlParseTransformXMLToString
(
    sbyte       **srcstr    /* [in/out] The pointer to pointer to the source string */
)
{
    sbyte   *src = NULL;
    sbyte   *src1 = *srcstr;
    sbyte   *amp;

    amp = strchr(src1, '&');
    if (NULL == amp)
        return;

    xmlParseReplaceString(src1, &src, "&amp;", "&");
    src1 = src;
    xmlParseReplaceString(src1, &src, "&quot;", "\"");
    src1 = src;
    xmlParseReplaceString(src1, &src, "&apos;", "'");
    src1 = src;
    xmlParseReplaceString(src1, &src, "&lt;", "<");
    src1 = src;
    xmlParseReplaceString(src1, &src, "&gt;", ">");
    *srcstr = src;
}/* xmlParseTransformXMLToString */

/*******************************************************************************
* xmlParseProcessInstruction - Process the XML instructions in <? tags.
*
* DESCRIPTION:
*      Process the <? tags.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      ppBuffer   - Pointer to pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the XML command.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
static STATUS xmlParseProcessInstruction
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               **ppBuffer, /* [in/out] Pointer to pointer to the input buffer  */
    saxCallBack         pCallback   /* [in] Pointer to the Callback function    */
)
{
    STATUS    rc = OK;
    sbyte       *data, *endTag, *pi;
    size_t      length;

    data = *ppBuffer;
    endTag = strstr(data, STRING_QMGT);
    if (NULL == endTag)
        rc = ERROR_GENERAL;
    else
    {
        if (!strncmp(data, STRING_XML, strlen(STRING_XML)))
            data = endTag + strlen(STRING_QMGT) + 1;
        else if (!strncmp(data, STRING_LTQM, strlen(STRING_LTQM)))
        {
            data += strlen(STRING_LTQM);
            length = (size_t)(endTag - data);

            pi = memPoolAlloc(g_memPool,length + 1);
            if (NULL == pi)
                rc = ERROR_MEMMGR_NO_MEMORY;
            else
            {
                STRNCPY(pi, data, length);
                pi[length] = CHAR_NULL;
                xmlParseTransformXMLToString(&pi);
                pCallback(pXmlEnv, kSAX_PROCESSING_INSTRUCTION, pi);
                data = endTag + 1;
            }
            (void)memPoolFree(g_memPool,pi);
        }
    }
    *ppBuffer = data;

    return rc;
}/* xmlParseProcessInstruction */

/*******************************************************************************
*
* xmlParseProcessComment - Process XML <!-- (comment) tags.
*
* DESCRIPTION:
*      Process the <!-- tags.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      ppBuffer   - Pointer to pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the XML command.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
static STATUS xmlParseProcessComment
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               **ppBuffer, /* [in/out] Pointer to pointer to the input buffer  */
    saxCallBack         pCallback   /* [in] Pointer to the Callback function    */
)
{
    STATUS    rc = OK;
    sbyte       *data, *endTag;

    data = *ppBuffer;
    endTag = strstr(data, STRING_ENDCOMMENT);
    if (NULL == endTag)
        rc = ERROR_GENERAL;
    else
        data = endTag + strlen(STRING_ENDCOMMENT) + 1;
    *ppBuffer = data;

    return rc;

}/* xmlParseProcessComment */

/*******************************************************************************
*
* xmlParseProcessDoctype -  Process teh XML <! (doctype) tags.
*
* DESCRIPTION:
*      Process the <! tags.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      ppBuffer   - Pointer to pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the XML command.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
static STATUS xmlParseProcessDoctype
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               **ppBuffer, /* [in/out] Pointer to pointer to the input buffer  */
    saxCallBack          pCallback  /* [in] Pointer to the Callback function    */
)
{
    STATUS    rc = OK;
    sbyte       *data, *endTag, *docType, *sp, *dtdUrl;
    unsigned long      length;

    data = *ppBuffer;
    endTag = strchr(data, CHAR_GT);
    if (NULL == endTag)
        rc = ERROR_GENERAL;
    else
    {
        docType = data;
        data = endTag + 1;

        if (!strncmp(docType, STRING_DOCTYPE, strlen(STRING_DOCTYPE)))
        {
            docType += strlen(STRING_DOCTYPE);
            xmlParseSkipSpace( &docType );
            g_rootElem = docType;
            sp = strchr(docType, CHAR_SPACE);
            if (NULL == sp)
                rc = ERROR_GENERAL;
            else
            {
                docType = sp + 1;
                xmlParseSkipSpace( &docType );
                if (!strncmp(docType, STRING_SYSTEM, strlen(STRING_SYSTEM)))
                {
                }
                else if (!strncmp(docType, STRING_PUBLIC, strlen(STRING_PUBLIC)))
                {
                }
                else
                    rc = ERROR_GENERAL;
            }
            if (OK == rc)
            {
                xmlParseSkipSpace( &docType );
                dtdUrl = docType;
                length = (unsigned long)(endTag - dtdUrl);
                pXmlEnv->p_DTDUrl = NULL;
                if (length > 0)
                {
                    pXmlEnv->p_DTDUrl = memPoolAlloc(g_memPool,length + 1);
                    if (NULL != pXmlEnv->p_DTDUrl)
                    {
                        STRNCPY(pXmlEnv->p_DTDUrl, dtdUrl, length);
                        pXmlEnv->p_DTDUrl[length] = CHAR_NULL;
                    }
                }
            }

        }
        else
            rc = ERROR_GENERAL;
    }
    *ppBuffer = data;

    return rc;

}/* xmlParseProcessDoctype */

/*******************************************************************************
* xmlParseProcessCharacters - 
*
* DESCRIPTION:
*      Process the <? tags.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      ppBuffer   - Pointer to pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the XML command.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
static STATUS xmlParseProcessCharacters
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               **ppBuffer, /* [in/out] Pointer to pointer to the input buffer  */
    saxCallBack         pCallback   /* [in] Pointer to the Callback function    */
)
{
    STATUS    rc = OK;
    sbyte       *data, *endTag, *characters;
    unsigned long      length;

    data = *ppBuffer;
    endTag = strchr(data, CHAR_LT);
    if (NULL == endTag)
        rc = ERROR_GENERAL;
    else
    {
        length      = (unsigned long)(endTag - data);
        characters  = memPoolAlloc(g_memPool,length + 1);
        if (NULL == characters)
            rc = ERROR_MEMMGR_NO_MEMORY;
        else
        {
            STRNCPY(characters, data, length);
            characters[length] = CHAR_NULL;
            xmlParseTransformXMLToString(&characters);
            pCallback(pXmlEnv, kSAX_CHARACTERS, characters);
            data = endTag;
        }
/*
 * Is more efficient if we let the application delete the data. This can
 * be rather large to copy around too much.
         memPoolFree(g_memPool,characters);
 */
    }
    *ppBuffer = data;

    return rc;
}/* xmlParseProcessCharacters */

/*******************************************************************************
* xmlParseProcessElement - 
*
* DESCRIPTION:
*      Process the element tags.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      ppBuffer   - Pointer to pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the XML command.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
static STATUS xmlParseProcessElement
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               **ppBuffer, /* [in/out] Pointer to pointer to the input buffer  */
    saxCallBack         pCallback   /* [in] Pointer to the Callback function    */
)
{
    STATUS    rc = OK;
    sbyte       *data, *endTag, *element, *tag;
    unsigned long      length;
    BOOL     bEndTag = FALSE;

    data = *ppBuffer;
    endTag = strchr(data, CHAR_GT);
    if (NULL == endTag)
        rc = ERROR_GENERAL;
    else
    {
        if (!strncmp(data, STRING_LTSL, strlen(STRING_LTSL)))
        {/* end tag */
            data += strlen(STRING_LTSL);
            bEndTag = TRUE;
        }
        else
            data += sizeof(sbyte);
        length  = (unsigned long)(endTag - data);
        tag = memPoolAlloc(g_memPool,length + 1);
        if (NULL == tag)
            rc = ERROR_MEMMGR_NO_MEMORY;
        else
        {
            element = tag;
            STRNCPY(element, data, length);
            element[length] = CHAR_NULL;
/* This section crashed if DOCTYPE did not appear in the XML doc
            if (g_bRoot)
            {
                if (strncmp(element, g_rootElem, length))
                    rc = ERROR_GENERAL;
            }
*/
            if (OK == rc)
            {
                if (bEndTag)
                {
                    pCallback(pXmlEnv, kSAX_END_ELEMENT, element);
                }
                else
                {
                    sbyte   *sp, *eq, *sl;

                    pXmlEnv->tag = element;
                    pXmlEnv->numAttr = 0;
                    while ((OK == rc) && (NULL != element) && (CHAR_NULL != *element) &&
                        (CHAR_SLASH != *element) && (CHAR_GT != *element))
                    {
                        sp = STRPBRK(element, STRING_WHITESPACE);
                        if (NULL != sp)
                        {
                            *sp = CHAR_NULL;
                            element = sp + 1;
                            xmlParseSkipSpace(&element);
                            if ((NULL != element) && (CHAR_NULL != *element))
                            {
                                if (0 == pXmlEnv->numAttr)
                                    pXmlEnv->attrib = element;
                                eq = strchr(element, CHAR_EQ);
                                if (NULL != eq)
                                {
                                    pXmlEnv->numAttr++;
                                    element = eq + sizeof(sbyte);
                                    xmlParseSkipSpace(&element);
                                    if ((NULL != element) && (CHAR_NULL != *element))
                                    {
                                        if (CHAR_QUOTE != *element)
                                            rc = ERROR_GENERAL;
                                        else
                                        {
                                            element += sizeof(sbyte);
                                            eq = strchr(element, CHAR_QUOTE);
                                            if (NULL == eq)
                                                rc = ERROR_GENERAL;
                                            else
                                                element = eq;
                                        }
                                    }
                                }
                                else
                                    break;
                            }
                            else
                                break;
                        }
                        else
                            break;
                    }
                    if ((NULL != element) && (CHAR_NULL != *element))
                    {
                        sl = strchr(element, CHAR_SLASH);
                        if (NULL != sl)
                        {
                            bEndTag = TRUE;
                            *sl = CHAR_NULL;
                        }
                    }
                    pCallback(pXmlEnv, kSAX_START_ELEMENT, pXmlEnv->tag);
                    if (bEndTag)
                        pCallback(pXmlEnv, kSAX_END_ELEMENT, pXmlEnv->tag);
                }
            }
            memPoolFree(g_memPool,tag);
        }
    }
    if (OK == rc)
        data = endTag + 1;
    *ppBuffer = data;

    return rc;
}/* xmlParseProcessElement*/

/*******************************************************************************
*
* xmlParseTransformToXMLString - Prepare string to XML convention.
*
* DESCRIPTION:
*      This routine parses a string and replaces ASCII characters, such as _\&_
*      and _\<_ with thier XML equivalents, such as _&amp;_ and _&lt;_.
*
* RETURNS:
*      none
* NOTES:
*
*
* NOMANUAL
*/
extern void xmlParseTransformToXMLString
(
    sbyte       **srcstr    /* [in/out] The pointer to pointer to the source string */
)
{
    sbyte   *src;
    sbyte   *src1;

    src1 = STRPBRK(*srcstr, STRING_XMLSYMBOLS);
    if (NULL == src1)
        return;
    src1 = *srcstr;
    xmlParseReplaceString(src1, &src, "&", "&amp;");
    src1 = src;
    xmlParseReplaceString(src1, &src, "\"", "&quot;");
    src1 = src;
    xmlParseReplaceString(src1, &src, "'", "&apos;");
    src1 = src;
    xmlParseReplaceString(src1, &src, "<", "&lt;");
    src1 = src;
    xmlParseReplaceString(src1, &src, ">", "&gt;");
    *srcstr = src;
}/* xmlParseTransformToXMLString */


/*******************************************************************************
*
* xmlParseGetAttribute - Return the attribute name at the indicated position.
*
* DESCRIPTION:
*      Return the name of the attribute at the indicated position.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      pos        - Position of the attribute in the list.
*      ppName     - Name of the attribute.
*      ppValue    - Value of the attribute.
*
* RETURNS:
*      nothing
* NOTES:
*
*
* NOMANUAL
*/
extern void xmlParseGetAttribute
(
    XML_DOC_ENV     *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    ubyte2          pos,        /* [in] Index of the attribute              */
    sbyte           **ppName,   /* [out] Name of the attribute returned     */
    sbyte           **ppValue   /* [out] Value of the attribute             */
)
{
    sbyte   *attr, *eq, *sp, *qu, *qu2;
    sbyte2  i;

    *ppName = NULL;
    if (pos < pXmlEnv->numAttr)
    {
        attr = pXmlEnv->attrib;
        for (i = 0; i < pos; i++)
        {
            attr += strlen(attr);
            attr++;
        }
        xmlParseSkipSpace(&attr);
        eq = strchr(attr, CHAR_EQ);
        if (NULL != eq)
        {
            *ppName = memPoolAlloc(g_memPool,(unsigned long)(eq - attr + 1));
            if (NULL != *ppName)
            {
                STRNCPY(*ppName, attr, (size_t)(eq - attr));
                (*ppName)[eq - attr] = CHAR_NULL;
                sp = STRPBRK(*ppName, STRING_WHITESPACE);
                if (NULL != sp)
                    *sp = CHAR_NULL;
            }
            qu = strchr(eq, CHAR_QUOTE);
            if (NULL != qu)
            {
                qu += sizeof(sbyte);
                qu2 = strchr(qu, CHAR_QUOTE);
                if (NULL != qu2)
                {
                    *ppValue = memPoolAlloc(g_memPool,(unsigned long)(qu2 - qu + 1));
                    if (NULL != *ppValue)
                    {
                        STRNCPY(*ppValue, qu, (size_t)(qu2 - qu));
                        (*ppValue)[qu2 - qu] = CHAR_NULL;
                        xmlParseTransformXMLToString(ppValue);
                    }
                }
            }
        }
    }
}/* xmlParseGetAttribute */

/*******************************************************************************
*
* xmlParseGetAttributeCount - Return the number of attributes of the current element.
*
* DESCRIPTION:
*      Return the number of attributes of the current element.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      pElement   - Name of the element.
*      pNumAttr   - Number of attributes returned.
*
* RETURNS:
*      nothing
* NOTES:
*
*
* NOMANUAL
*/
extern void xmlParseGetAttributeCount
(
    XML_DOC_ENV     *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte           *pElement,  /* [in] Name of the element                 */
    ubyte2          *pNumAttr   /* [out] Returns the number of attributes    */
)
{
    *pNumAttr = 0;

    if (!STRCMP(pElement, pXmlEnv->tag))
        *pNumAttr = pXmlEnv->numAttr;

}/* xmlParseGetAttributeCount */

/*******************************************************************************
* xmlParseDocument - Parse the input buffer.
*
* DESCRIPTION:
*      Parse the input buffer.
*
* PARAMETERS:
*      pXmlEnv,   - Pointer to the XML_DOC_ENV structure
*      pBuffer    - Pointer to buffer to parse.
*      pCallback  - Pointer to the function to process the.
*
* RETURNS:
*      STATUS
* NOTES:
*
*
* NOMANUAL
*/
extern STATUS xmlParseDocument
(
    XML_DOC_ENV         *pXmlEnv,   /* [in] Pointer to the XML_DOC_ENV structure */
    sbyte               *pBuffer,   /* [in] Pointer to the input buffer         */
    saxCallBack         pCallback   /* [in] Pointer to the Callback function    */
)
{
    STATUS            rc = OK;
    sbyte               *data;

    data = pBuffer;

    xmlParseSkipSpace(&data);
    if (!strncmp(data, STRING_XML, strlen(STRING_XML)))
    {
        sbyte *qgt;

        qgt = strstr(data, STRING_ENDXML);
        if (NULL == qgt)
            rc = ERROR_GENERAL;
        else
            data = qgt + strlen(STRING_ENDXML);
    }
    else
        rc = ERROR_GENERAL;

    pXmlEnv->attrib = NULL;
    pXmlEnv->numAttr = 0;
    pXmlEnv->tag = NULL;
    g_memPool = httpGlobalMemPartIdGet();
    g_bRoot = TRUE;
    while ((OK == rc) && (NULL != data) && (CHAR_NULL != *data))
    {
        xmlParseSkipSpace(&data);
        if (NULL != data)
        {
            if (!strncmp(data, STRING_LTQM, strlen(STRING_LTQM)))
                rc = xmlParseProcessInstruction(pXmlEnv, &data, pCallback);
            else if (!strncmp(data, STRING_BEGINCOMMENT, strlen(STRING_BEGINCOMMENT)))
                rc = xmlParseProcessComment(pXmlEnv, &data, pCallback);
            else if (!strncmp(data, STRING_LTEX, strlen(STRING_LTEX)))
                rc = xmlParseProcessDoctype(pXmlEnv, &data, pCallback);
            else if (CHAR_LT != *data)
                rc = xmlParseProcessCharacters(pXmlEnv, &data, pCallback);
            else
            {
                rc = xmlParseProcessElement(pXmlEnv, &data, pCallback);
                g_bRoot = FALSE;
            }
        }
    }
    return rc;
}/* xmlParseDocument */
