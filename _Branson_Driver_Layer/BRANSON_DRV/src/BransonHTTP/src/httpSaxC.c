/* httpSaxC.c - SAX interface to XML parser */

/* Copyright 1999-2007,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01d,26jan10,h_y  support for LP64
01c,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01b,13feb07,msa  Fix comments to remove APIgen errors.
01a,17jan02,jc   imported, renamed and modified from RCX codebase
*/

/*
DESCRIPTION
This module implements a SAX interface to the Wind River Web XML parser.

INCLUDES FILES: wm.h
				httpSaxC.h
				httpXmlParser.h

*/




/**- include files **/
#include "wm.h"
#include "httpSaxC.h"
#include "httpXmlParser.h"

/**- local definitions **/

/**- default settings **/

/**- external functions **/

/**- external data **/

/**- internal functions **/

/**- public data **/

/**- private data **/

/**- private functions **/

/*******************************************************************************
*
* sax_startParser - sax_startParser
*
*  DESCRIPTION:
*      Start the parsing of a document.
*  PARAMETERS:
*      pXmlEnv     - Pointer to the environment structure
*      pDochandler - Pointer to the Document Handler callback function.
*      pBuffer     - Pointer to the XML document to parse
*      bufSize     - Buffer size
*
*  RETURNS:
*      STATUS
*  NOTES:
*
*
* NOMANUAL
*/
extern STATUS sax_startParser
	(
    XML_DOC_ENV     *pXmlEnv,       /* [in] Pointer to the environment structure    */
    saxCallBack     pDocHandler,    /* [in] Pointer to the document handler function. */
    sbyte           *pBuffer,       /* [in] Pointer to the XML document to parse */
    unsigned long   bufSize         /* [in] Buffer size                          */
	)
	{
    STATUS    rc = OK;

    pDocHandler(pXmlEnv, kSAX_START_DOCUMENT, NULL);

    rc = xmlParseDocument( pXmlEnv, pBuffer, pDocHandler );

    pDocHandler(pXmlEnv, kSAX_END_DOCUMENT, NULL);

    return rc;

	}/* sax_startParser */

/*******************************************************************************
*
*  sax_getAttribute - Return attribute name within the current element.
*
*  DESCRIPTION:
*      Return the name of the attribute within the current element.
*  PARAMETERS:
*      pXmlEnv,    - Pointer to the environment structure
*      szAttrib    - Pointer to a buffer that will contain the name.
*      index       - 0 based index pointing to the attribute in the list
*
*  RETURNS:
*      STATUS
*  NOTES:
*
*
* NOMANUAL
*/
extern STATUS sax_getAttribute
	(
    XML_DOC_ENV   *pXmlEnv,       /* [in] Pointer to the environment structure    */
    ubyte2        index,          /* [in] Index of the attribute requested. */
    sbyte         **ppAttrib,     /* [out] In return contains the name of the attribute */
    sbyte         **ppValue       /* [out] In return contains the value of the attribute  */
	)
	{
    STATUS    rc = OK;

    xmlParseGetAttribute(pXmlEnv, index, ppAttrib, ppValue);

    return rc;
	}/* sax_getAttributeName */


/*******************************************************************************
*
*  sax_getAttributeCount - Get the number of attributes for the current element.
*
*  DESCRIPTION:
*      Returns the number of attributes for the current element.
*  PARAMETERS:
*      pXmlEnv    - Pointer to the environment structure
*      pElement   - Name of the element (tag)
*      pCount  - Pointer to a buffer that will hold attribute count of the current element.
*
*  RETURNS:
*      STATUS
*  NOTES:
*
*
* NOMANUAL
*/
extern STATUS sax_getAttributeCount
	(
    XML_DOC_ENV   *pXmlEnv,   /* [in] Pointer to the environment structure    */
    sbyte         *pElement,  /* [in] Element name                            */
    ubyte2        *pCount     /* [out] In return contains number of attributes of the current element */
	)
	{
    STATUS    rc = OK;

    xmlParseGetAttributeCount(pXmlEnv, pElement, pCount);

    return rc;
	}/* sax_getAttributeCount */
