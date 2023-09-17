/*================================================================
** Copyright 2000, Clark Cooper
** All rights reserved.
**
** This is free software. You are permitted to copy, distribute, or modify
** it under the terms of the MIT/X license (contained in the COPYING file
** with this distribution.)
*/

#ifndef EXPAT_OS_H
#define EXPAT_OS_H

#include <EcOs.h>
#include <stddef.h>

#ifndef EXCLUDE_XML_BUILDING_EXPAT
#define XML_BUILDING_EXPAT  1
#endif

#ifndef XML_CONTEXT_BYTES
#define XML_CONTEXT_BYTES   1024
#endif

/* we will assume all Windows platforms are little endian */
#ifndef BYTEORDER
#define BYTEORDER           1234
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4201 4244 4100 4514 4127)
#endif

#endif /* EXPAT_OS_H */
