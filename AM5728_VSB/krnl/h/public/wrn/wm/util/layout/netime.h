/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 *  Id: netime.ldb,v 1.1 1998/04/08 17:01:14 wes Exp 
 */

#ifndef EPILOGUE_LAYOUT_NETIME_H
#define EPILOGUE_LAYOUT_NETIME_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for NETIME_MESSAGE */

#define SIZEOF_NETIME_MESSAGE (4)
#define PTR_NETIME_MESSAGE_TIMESTAMP(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_NETIME_MESSAGE_TIMESTAMP(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_)))
#define SET_NETIME_MESSAGE_TIMESTAMP(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))

#endif /* EPILOGUE_LAYOUT_NETIME_H */
