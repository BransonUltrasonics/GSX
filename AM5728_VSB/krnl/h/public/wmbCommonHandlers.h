/* wmbCommonHandlers.h - handlers for WindMarks provided with WIND MANAGE */

/*
modification history
--------------------
01b,14aug03,adb  added wmbFileIncludeHandler
01a,22jul03,adb  added wmbGetColumnarHandler
*/

#ifndef __INCwmbCommonHandlersh
#define __INCwmbCommonHandlersh

STATUS wmbGetInstanceHandler 
    (
    WMB_TID_T   tid, 
    WMB_OBJ_T   obj, 
    COMMAND_T   cmd
    );

STATUS wmbGetIndexInstanceHandler 
    (
    WMB_TID_T   tid, 
    WMB_OBJ_T   obj, 
    COMMAND_T   cmd
    );

STATUS wmbGetColumnarHandler 
    (
    WMB_TID_T   tid, 
    WMB_OBJ_T   obj, 
    COMMAND_T   cmd
    );

STATUS wmbFileIncludeHandler 
    (
    WMB_TID_T   tid, 
    WMB_OBJ_T   obj, 
    COMMAND_T   cmd
    );

sbyte * wmbCommonTrimInstance 
    (
    sbyte *, 
    sbyte *, 
    sbyte*, 
    void *, 
    BOOL
    );

#endif /* ___INCwmbCommonHandlersh */

