/*  wmLinkListLib.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmLinkListLibh
#define __INCwmLinkListLibh

/* Structures & Datatypes */
typedef struct WM_LIST_S
    {
    void *              pObject;       /* pointer to data object  */
    struct WM_LIST_S * pNext;         /* next object in the list */
    } WM_LIST_T;

/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

WM_LIST_T *  wmLinkListConstruct  (void);
void    wmLinkListDestruct       (WM_LIST_T **lstToDelete,
                                     void (*funcDelete_pObject)(void *));
void    wmLinkListParamDestruct (WM_LIST_T **p_lstToDelete,
                                     void (*funcObject_Destruct)(void *, void *), void *second_param );

STATUS  wmLinkListAddObject      (WM_LIST_T *plstHead, void *pData);
STATUS  wmLinkListRemoveObject   (WM_LIST_T *plstHead,
                               BOOL (*funcComparisonHandler)(void *));

STATUS  wmLinkListSelectiveRemoveObject(WM_LIST_T *p_lstHead, void *pRemoveParam,
                                           BOOL    (*funcComparisonHandler)(void *, void *));

STATUS  wmLinkListTraverse       (WM_LIST_T *plstHead,
                                     void (*funcProcessObject)(void *));

void *  wmLinkListFindObject    (WM_LIST_T *Head, void *pVal,
                                     BOOL    (*Compare)(void*,void*));
Length  wmLinkListSize           (WM_LIST_T *plstHead);

void *  wmLinkListGetFirstObject( WM_LIST_T *plstHead );
void *  wmLinkListGetNextObject(WM_LIST_T **p_lstSpot);

#ifdef __cplusplus
}
#endif

#endif /* __INCwmLinkListLibh */
