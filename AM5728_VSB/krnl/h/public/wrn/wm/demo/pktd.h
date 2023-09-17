/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/pktd.h,v 1.1 2001/11/08 16:00:58 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1992-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 *****************************************************************************/

/*
 * $Log: pktd.h,v $
 * Revision 1.1  2001/11/08 16:00:58  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.9  2001/01/19 22:23:48  paul
 * Update copyright.
 *
 * Revision 2.8  2000/03/17 00:12:42  meister
 * Update copyright message
 *
 * Revision 2.7  1998/02/25 04:57:30  sra
 * Update copyrights.
 *
 * Revision 2.6  1998/02/20 23:07:23  sra
 * Add multicast and IPv6 support.
 *
 * Revision 2.5  1998/02/19 07:46:10  sra
 * Preliminary multicast support.
 *
 * Revision 2.4  1997/07/22 20:33:47  mrf
 * put constants in correct include file
 *
 * Revision 2.3  1997/03/20 06:53:05  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.1  1996/03/22 10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.3  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.2  1993/07/31  02:00:54  sra
 * Flush boolean_t from this file.
 *
 * Revision 1.1  1993/07/30  02:41:24  sra
 * Initial revision
 *
 *
 * Revision 1.3  1992/12/10  19:32:36  dab
 * Added fake PPP stuff for compatibility with FTP's router.
 *
 * Revision 1.2  1992/10/13  15:59:35  dab
 * Fixes for SLIP
 *
 * Revision 1.1  92/09/18  15:54:43  dab
 * Initial revision
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _PKTD_H_
#define _PKTD_H_

/*
 * The following definitions are from the FTP Software Packet Driver
 * specification.
 */

/*
 * Range in which the packet driver interrupt may lie
 */
#define PKTDRVR_MIN_INT         0x60
#define PKTDRVR_MAX_INT         0x7F

/*
 * The interface classes
 */
#define IC_ETHERNET             1
#define IC_PRONET10             2
#define IC_TOKENRING            3
#define IC_OMNINET              4
#define IC_APPLETALK            5
#define IC_SERIAL_LINE          6
#define IC_STARLAN              7
#define IC_ARCNET               8
#define IC_AX25                 9
#define IC_KISS                 10
#define IC_8023                 11
#define IC_FDDI                 12
#define IC_X25                  13
#define IC_LANSTAR              14
#define IC_FPPP                 16              /* "Fake" PPP (Niwot card) */

/* packet driver code still needs types byte-swapped */
#define PD_ET_IP   0x0008
#define PD_ET_ARP  0x0608
#define	PD_ET_IP6  0xDD86

/*
 * Packet driver error codes.
 */
#define PDE_NO_ERROR            0               /* Added */
#define PDE_BAD_HANDLE          1
#define PDE_NO_CLASS            2
#define PDE_NO_TYPE             3
#define PDE_NO_NUMBER           4
#define PDE_BAD_TYPE            5
#define PDE_NO_MULTICAST        6
#define PDE_CANT_TERMINATE      7
#define PDE_BAD_MODE            8
#define PDE_NO_SPACE            9               /* Added */
#define PDE_TYPE_INUSE          10              /* Added */
#define PDE_BAD_COMMAND         11              /* Added */
#define PDE_CANT_SEND           12              /* Added */
#define PDE_NO_PKT_DRVR         128             /* Added */

/*
 * Interface types (within each class)
 */
#define IT_ANY                  0xFFFF

/*
 * Devices in the Ethernet class
 */
#define _3COM_3C501             1
#define _3COM_3C505             2
#define INTERLAN_NI5010         3
#define BICC_4110               4
#define BICC_4117               5
#define INTERLAN_NP600          6
#define UB_NIC                  8
#define UNIVATION               9
#define TRW_PC2000              10
#define INTERLAN_NI5210         11
#define _3COM_3C503             12
#define _3COM_3C523             13
#define WESTERN_DIGITAL_WD8003  14

/*
 * ProNET-10
 */
#define PROTEON_P1300           1

/*
 * IEEE 802.5 & IBM Token Ring
 */
#define IBM_TOKEN_RING          1
#define PROTEON_P1340           2
#define PROTEON_P1344           2

/*
 * Driver Functionality Codes (bit-mapped, sort of)
 */
#define PDFC_BASIC      1               /* this one isn't bit-mapped */
#define PDFC_EXTENDED   2
#define PDFC_HIGH_PERFORMANCE 4

/*
 * Extended driver receive modes
 */
#define PKT_RCVR_OFF            1
#define PKT_RCVR_UNICAST        2       /* unicast only */
#define PKT_RCVR_BCAST          3       /* unicast + b'cast */
#define PKT_RCVR_LIM_MCAST      4       /* unicast + b'cast + some m-cast */
#define PKT_RCVR_ALL_MCAST      5       /* unicast + b'cast + all m-cast  */
#define PKT_RCVR_PROMISCUOUS    6       /* everything */

/*
 * Define the pkt driver function codes
 */
#define PF_DRIVER_INFO          1
#define PF_ACCESS_TYPE          2
#define PF_RELEASE_TYPE         3
#define PF_SEND_PKT             4
#define PF_TERMINATE            5
#define PF_GET_ADDRESS          6
#define PF_RESET_INTERFACE      7
#define PF_SET_RCV_MODE         20
#define PF_GET_RCV_MODE         21
#define PF_SET_MULTICAST_LIST   22
#define PF_GET_MULTICAST_LIST   23
#define PF_GET_STATISTICS       24

typedef struct {
  int version;
  int pdtype;
  unsigned char class;
  unsigned char number;
  char far *name;
  char basic_flag;
} pkt_driver_info_t;

typedef struct {
  long int pkts_in;
  long int pkts_out;
  long int bytes_in;
  long int bytes_out;
  long int errors_in;
  long int errors_out;
  long int packets_dropped;
  long int pad[4];              /* Space for FTP Software to add new items. */

  /*
   * The remaining fields are TRW PC-2000 extensions
   */
  long int ints;                /* Interrupts */

  /*
   * XMIT errors...
   */
  long int cd_lost;             /* Lost Carrier Detect on XMIT */
  long int cts_lost;            /* Lost Clear-to-send on XMIT */
  long int xmit_dma_under;      /* XMIT DMA underrun */
  long int deferred;            /* XMITs deferred */
  long int sqe_lost;            /* SQE failed to follow XMIT */
  long int collisions;          /* Collisions during XMIT */
  long int ex_collisions;       /* XMITs terminated due to excess collisions */
  long int toobig;              /* User packet too big to XMIT */
  long int refused;             /* User packet refused, no XMIT resources */
  int max_send_pend;            /* High water mark of pending transmissions. */
  int place_holder_0;           /* Place holder for future use */

  /*
   * RCV errors ...
   */
  long int shorts;              /* RCVD packets < min length */
  long int longs;               /* RCVD packets > max length */
  long int skipped;             /* RCVD packets discarded by software. */
  long int crcerrs;             /* RCVD packets with CRC errors */
  long int alnerrs;             /* RCVD pkts w/ alignment errors*/
  long int rscerrs;             /* RCVD pkts but no 82586 rcv resources */
  long int ovrnerrs;            /* RCVD pkts lost due to rcv DMA overruns
                                /*  (occurs often using 82586 in loopback) */
  long int  unwanted;           /* RCVD packets skipped by software because */
                                /* no user wants that type. */
  long int  user_drops;         /* RCVD packets skipped by software because */
                                /* user would not provide a buffer. */
} pkt_driver_statistics_t;

struct pd_net {
  int pd_int;
  int handle;
  PD_UPCALL_RTN_T *device_receive_rtn;
  pkt_driver_info_t drvr_info;
};

/*
 * Packet driver routines.
 */

extern int pd_check_int
  (int pd_int);

extern int pd_driver_info
  (int handle,
   struct pd_net *pd_net);

extern int pd_access_type
  (struct pd_net *pd_net,
   int *handle,
   unsigned char far *type,
   int type_length);

extern int pd_release_type
  (struct pd_net *pd_net,
   int handle);

extern int pd_get_address
  (struct pd_net *pdnet,
   unsigned char far *address_buffer,
   int buffer_length);

extern int pd_send_pkt
  (struct pd_net *pd_net,
   unsigned char far *buffer,
   unsigned int buffer_length);

extern char *pd_perror
  (int error_code);

extern int pd_get_rcv_mode
  (struct pd_net *pdn,
   int *mode);

extern int pd_set_rcv_mode
  (struct pd_net *pdn,
   int mode);

#endif  /* _PKTD_H_ */
