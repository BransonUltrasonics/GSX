/* mudUtils.h */

/* Copyright 2004-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef _WRS_CONFIG_LP64
STATUS readLineFromSocket(MUD_SOCKET sock, char * buffer, ubyte4 sizebuf);
#else
STATUS readLineFromSocket(MUD_SOCKET sock, char * buffer, ubyte8 sizebuf);
#endif
