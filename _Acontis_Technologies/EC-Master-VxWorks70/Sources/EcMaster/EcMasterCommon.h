/*-----------------------------------------------------------------------------
 * EcMasterCommon.h         header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

#ifndef INC_ECMASTERECOMMON
#define INC_ECMASTERECOMMON

/*-INCLUDES------------------------------------------------------------------*/

/*-DEFINES-------------------------------------------------------------------*/
#define EC_S_SUCCESS                    ((EC_T_DWORD)0x00000000)
#define EC_S_UNSUCCESSFUL               ((EC_T_DWORD)0xC0000001)
#define EC_S_INSUFFICIENT_RESOURCES     ((EC_T_DWORD)0xC000009A)
#define EC_S_DEVICE_BUSY                ((EC_T_DWORD)0x80000011)
#define EC_S_INFO_LENGTH_MISMATCH       ((EC_T_DWORD)0xC0000004)
#define EC_S_PENDING                    ((EC_T_DWORD)0x00000103) 
#define EC_S_INVALID_HANDLE             ((EC_T_DWORD)0xC0000008)

#define EC_AL_STATUS_INIT_VALUE         0xffff


/*-TYPEDEFS------------------------------------------------------------------*/

typedef struct _EC_T_LISTENTRY
{
    struct _EC_T_LISTENTRY *Flink;
    struct _EC_T_LISTENTRY *Blink;
} EC_T_LISTENTRY;

/*-MACROS--------------------------------------------------------------------*/
#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

#define RemoveTailList(ListHead) \
    (ListHead)->Blink;\
    {RemoveEntryList((ListHead)->Blink)}

#define RemoveEntryList(Entry) {\
    EC_T_LISTENTRY* _EX_Blink;\
    EC_T_LISTENTRY* _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

#define InsertTailList(ListHead,Entry) {\
    EC_T_LISTENTRY* _EX_Blink;\
    EC_T_LISTENTRY* _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

#define InsertHeadList(ListHead,Entry) {\
    EC_T_LISTENTRY* _EX_Flink;\
    EC_T_LISTENTRY* _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

#define PopEntryList(ListHead) \
    (ListHead)->Next;\
    {\
        PSINGLE_LIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != EC_NULL) {     \
            (ListHead)->Next = FirstEntry->Next;\
        }                             \
    }

#define PushEntryList(ListHead,Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry)

#define GetListHeadEntry(ListHead)  ((ListHead)->Flink)

#define GetListTailEntry(ListHead)  ((ListHead)->Blink)

#define GetListNextEntry(Entry)     ((Entry)->Flink)

#endif /* INC_ECMASTERECOMMON */


/*-END OF SOURCE FILE--------------------------------------------------------*/
