/* tgsBasic.h - tilcon infrastructure header */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created (US15550)
*/

/*
DESCRIPTION

This file includes basic headers of Tilcon library.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tilcon_H_
#define _INCLUDE_tilcon_H_

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <new>

#include "tilcon/basic/tgsBasicTypes.h"
#include "tilcon/basic/tgsPrimitive.h"
#include "tilcon/basic/tgsColor.h"
#include "tilcon/basic/tgsArray.h"
#include "tilcon/basic/tgsPairArray.h"
#include "tilcon/basic/tgsSmartPointer.h"
#include "tilcon/basic/tgsAnsiString.h"
#include "tilcon/basic/tgsWideString.h"
#include "tilcon/basic/tgsTypes.h"

#endif /* _INCLUDE_tilcon_H_ */

