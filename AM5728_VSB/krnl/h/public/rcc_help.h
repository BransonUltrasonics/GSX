/* rcc_help.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      changed 'C' to 'extern "C"'
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history.
*/



#ifndef __RCC_HELP_H__
#define __RCC_HELP_H__

typedef struct EditKeys {
    ubyte  key;
    sbyte *helpText;
} EditKeys;

#define HELP_FLAG_SHOW_NO       0x00000001
#define HELP_FLAG_HIDE_BLANK    0x00000002
#define HELP_FLAG_NEXT          0x00000004
#define HELP_FLAG_EXECUTABLE    0x00000008
#define HELP_FLAG_ALLOC_TITLE   0x00000010
#define HELP_FLAG_ALLOC_PREFIX  0x00000020
#define HELP_FLAG_ALLOC_DELIM   0x00000040
#define HELP_FLAG_ALLOC_QUOTE   0x00000080
#define HELP_FLAG_ALLOC_UNQUOTE 0x00000100
#define HELP_FLAG_ALLOC_NODE    0x00000200
#define HELP_FLAG_ERROR         0x00000400
#define HELP_FLAG_THIS          0x00000800
#define HELP_FLAG_SAME_LINE     0x00001000
#define HELP_FLAG_PARAMS        0x00002000
#define HELP_FLAG_SKIP_NODE     0x00004000
#define HELP_FLAG_INDENT        0x00008000
#define HELP_FLAG_FIXED_WIDTH   0x00010000
#define HELP_FLAG_WIDTH         0x00020000
#define HELP_FLAG_LEADER        0x00040000
#define HELP_FLAG_LEAF_NEXT     0x00080000 /* leaf only shows next node         */

/* help defaults */

#ifndef kRCC_HELP_WIDTH
#define kRCC_HELP_WIDTH 30
#endif

#ifndef kRCC_HELP_LEADER
#define kRCC_HELP_LEADER ' '
#endif

#ifndef kRCC_HELP_TITLE
#define kRCC_HELP_TITLE NULL
#endif

#ifndef kRCC_HELP_PREFIX
#define kRCC_HELP_PREFIX NULL
#endif

#ifndef kRCC_HELP_NODE_DELIMITER
#define kRCC_HELP_NODE_DELIMITER NULL
#endif

#ifndef kRCC_HELP_DELIMITER
#define kRCC_HELP_DELIMITER NULL
#endif

#ifndef kRCC_HELP_SYNTAX
#define kRCC_HELP_SYNTAX NULL
#endif

#ifndef kRCC_HELP_SYNTAX_TAIL
#define kRCC_HELP_SYNTAX_TAIL NULL
#endif

#ifndef __RCC_NO_HELP_INDENT__
#define HELP_INDENT_SET_M(pCliEnv)      INDENT_SET_HERE(pCliEnv)
#define HELP_INDENT_RESET_M(pCliEnv)    MEDIT_SetIndent(pCliEnv, 0)
#else
#define HELP_INDENT_SET_M(pCliEnv)
#define HELP_INDENT_RESET_M(pCliEnv)
#endif


#ifdef __cplusplus
extern "C" {
#endif

void    RCC_HELP_CustomHelpString(WMB_SESSION_T * pCliEnv, cmdNode *pNode,
                                          paramDefn *pParam);
STATUS  RCC_HELP_ErrorHelp(WMB_SESSION_T * pCliEnv, cmdNode *pNode);
STATUS  RCC_HELP_Globals(WMB_SESSION_T * pCliEnv);
void    RCC_HELP_PrintLabel(WMB_SESSION_T * pCliEnv, cmdNode *pNode,
                                    sbyte *text, sbyte *pWrapper);
void    RCC_HELP_NodeHandler(WMB_SESSION_T * pCliEnv, cmdNode *pNode);
STATUS  RCC_HELP_RetrieveHelp(WMB_SESSION_T * pCliEnv, cmdNode *pNode,
                                      BOOL immediate);
STATUS  RCC_HELP_SetFeature(WMB_SESSION_T *  pCliEnv, ubyte4 feature,
                                    sbyte * pText);

#ifdef __cplusplus
}
#endif

#define RCC_HELP_EditHelp   RCC_SYSTEM_EditHelp

#endif /* __RCC_HELP_H__ */
