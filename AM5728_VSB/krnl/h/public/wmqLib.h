/* wmqLib.h - header for WindManage MsgQueue */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmqLibh
#define __INCwmqLibh

void    wmqInit (sbyte **free_ptr, sbyte4 *free_size);

STATUS  wmqBroadcast (OS_MSGQ_T qid, sbyte * pMsg_buf, sbyte4 msgSize, sbyte4 * pCount);

STATUS  wmqCreate (sbyte4 maxMsgs, sbyte4 msgSize, ubyte4 flags, OS_MSGQ_T *Qid);
STATUS  wmqDelete (OS_MSGQ_T Qid);

STATUS wmqReceive (OS_MSGQ_T qid, sbyte * pMsg_buf, sbyte4 bufferSize,
                   ubyte4 timeout, sbyte4 * pBytesInBuffer);

STATUS wmqSend (OS_MSGQ_T qID, sbyte * pMsg, sbyte4 msgSize,
                sbyte4 timeout, sbyte4 priority);

#endif /* __INCwmqLibPh */
